"""Drive DOSBox X to run a batch file in the emulated DOS environment.

The batch file's output is redirected to C:\\OUT\\LOG.TXT inside the mounted C:
drive; this module runs it and returns the log text.
"""
import os, subprocess, sys, time, shutil

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
DOSBOX = os.path.join(ROOT, 'tools', 'bin', 'dosboxx', 'mingw-build', 'mingw-sdl2', 'dosbox-x.exe')
CONF = os.path.join(ROOT, 'dos', 'dosbox.conf')
DOSC = os.path.join(ROOT, 'dos', 'c')          # writable C: drive
WORK = os.path.join(DOSC, 'WORK')             # scratch inside DOS
OUT = os.path.join(DOSC, 'OUT')               # logs / artefacts inside DOS
LOG = os.path.join(OUT, 'LOG.TXT')


def run_batch(dos_batch_name, timeout=300, log=LOG, quiet=True):
    """Run C:\\WORK\\<dos_batch_name> under DOSBox X. Return (rc, log_text)."""
    for d in (WORK, OUT):
        os.makedirs(d, exist_ok=True)
    if os.path.exists(log):
        os.remove(log)
    before = set(os.listdir(WORK))
    cmd = [DOSBOX, '-conf', CONF, '-c', 'C:\\WORK\\' + dos_batch_name, '-exit']
    if quiet:
        cmd.insert(1, '-nologo')
    try:
        p = subprocess.run(cmd, timeout=timeout, stdout=subprocess.DEVNULL,
                           stderr=subprocess.DEVNULL)
        rc = p.returncode
    except subprocess.TimeoutExpired:
        rc = 124
    text = ''
    if os.path.exists(log):
        text = open(log, encoding='latin1', errors='replace').read()
    if rc == 124:
        # Read the log even on timeout.  A run that has to be killed is exactly
        # the run whose output matters most, and returning early here threw away
        # everything the program had already printed.
        text = 'TIMEOUT after %ss%s' % (timeout, ('\r\n' + text) if text else '')
    return rc, text


def stage(local_path, dosname=None):
    """Copy a host file into C:\\WORK, return its DOS path."""
    os.makedirs(WORK, exist_ok=True)
    dosname = dosname or os.path.basename(local_path)
    shutil.copy2(local_path, os.path.join(WORK, dosname))
    return 'C:\\WORK\\' + dosname


def unstage(dosname):
    """Fetch a file back out of C:\\WORK (or C:\\OUT) to the host."""
    for base in (WORK, OUT):
        p = os.path.join(base, dosname)
        if os.path.exists(p):
            return p
    return None


def clear_out():
    if os.path.isdir(OUT):
        for f in os.listdir(OUT):
            p = os.path.join(OUT, f)
            if os.path.isfile(p):
                os.remove(p)


if __name__ == '__main__':
    print(run_batch(sys.argv[1])[1])