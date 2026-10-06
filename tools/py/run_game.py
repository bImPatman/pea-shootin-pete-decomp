"""Run the linked PETE.EXE under DOSBox and show what it printed.

The game does not exit on its own: main() is an unconditional `for(;;)` whose
exit condition is g_27fc, and nothing reconstructed yet sets it.  So a run is
expected to be killed by the timeout, and that is reported as information
rather than treated as a crash.  Output captured before the kill is still
printed, which is the whole point of this script -- the logging stand-ins in
src/run/stubs.c name the first unresolved callee the game reaches.

A timeout, a non-zero DOSBox exit and a game that logged nothing are all
reported and still exit 0: a program this incomplete is expected to misbehave,
and the useful output is the log rather than the status code.  The one hard
failure is having no executable to run at all.  Pass --strict to exit non-zero
for anything short of a clean run.
"""
import os
import sys
import argparse

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dosbox

ROOT = dosbox.ROOT

# The game writes here itself, one fclose per line.  Nothing else is readable:
# DOSBox X buffers console output, the shell's `>>` redirect and a guest fflush
# alike until the process exits, and this program never exits, so every one of
# them comes back empty after the timeout kills the run.  See src/run/stubs.c.
GUEST_LOG = os.path.join(dosbox.OUT, 'GUEST.LOG')


def find_exe(name):
    for d in (os.path.join(ROOT, 'build'),
              dosbox.OUT,
              dosbox.WORK):
        p = os.path.join(d, name + '.EXE')
        if os.path.exists(p):
            return p
    return None


def main():
    ap = argparse.ArgumentParser(
        description='Run PETE.EXE under DOSBox and print its console output.')
    ap.add_argument('--name', default='PETE', help='executable name (default PETE)')
    ap.add_argument('--timeout', type=int, default=20,
                    help='seconds before the run is killed (default 20)')
    ap.add_argument('--strict', action='store_true',
                    help='exit non-zero unless the run finished cleanly')
    ap.add_argument('game_args', nargs='*',
                    help='arguments passed to the game')
    args = ap.parse_args()

    exe = find_exe(args.name.upper())
    if exe is None:
        print('no %s.EXE found in build\\, %s\\ or %s\\'
              % (args.name.upper(), dosbox.OUT, dosbox.WORK))
        print('build it first:  python tools\\py\\build_game.py')
        return 1

    os.makedirs(dosbox.WORK, exist_ok=True)
    if os.path.abspath(exe) != os.path.abspath(os.path.join(dosbox.WORK,
                                                           args.name.upper() + '.EXE')):
        dosbox.stage(exe)

    # Start from an empty guest log so a stale file cannot be mistaken for this
    # run's output.  The batch's own redirect is kept only for the GAME_START
    # marker, which answers "did DOSBox launch it at all"; every line the
    # program produces goes to GUEST_LOG instead.
    #
    # Only a plain append is used.  DOSBox X parses `2>&1` as two separate output
    # redirects -- it logged "Redirect output to ..\OUT\LOG.TXT" then "Redirect
    # output to &1" and wrote the game's output to a file literally named `&1`
    # -- so stderr is not folded in here.  Everything this program reports goes
    # through stdout anyway.
    #
    # %ERRORLEVEL% is not recorded either: DOSBox X's shell leaves it empty, so
    # the batch cannot report how the game ended.  Success is judged from DOSBox
    # exiting on its own, and everything else is in the log.
    if os.path.exists(GUEST_LOG):
        os.remove(GUEST_LOG)

    line = '%s %s >> ..\\OUT\\LOG.TXT' % (args.name.upper(),
                                          ' '.join(args.game_args))
    bat = ['@echo off', 'cd \\WORK',
           'echo GAME_START >> ..\\OUT\\LOG.TXT',
           line,
           'exit']
    open(os.path.join(dosbox.WORK, 'RUN.BAT'), 'wb').write(
        ('\r\n'.join(bat) + '\r\n').encode('latin1'))

    print('running %s  (timeout %ss)' % (os.path.relpath(exe, ROOT), args.timeout))
    print('-' * 60)

    rc, log = dosbox.run_batch('RUN.BAT', timeout=args.timeout)

    if os.path.exists(GUEST_LOG):
        print(open(GUEST_LOG, encoding='latin1', errors='replace')
              .read().replace('\r', '').rstrip())
    else:
        print('the game logged nothing at all')
        if 'GAME_START' not in log:
            print('DOSBox does not appear to have launched it either')
        elif log.strip():
            print('shell log:')
            print(log.replace('\r\n', '\n').rstrip())
    print('-' * 60)

    if rc == 124:
        print('still running when the timeout expired; killed (this is the '
              'expected outcome while g_27fc is never set)')
        return 1 if args.strict else 0
    if rc != 0:
        print('dosbox exited %s' % rc)
        return 1 if args.strict else 0

    print('the game returned to DOS on its own')
    return 0


if __name__ == '__main__':
    sys.exit(main())