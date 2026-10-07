"""Link the decompiled game into one runnable PETE.EXE.

`check.py` builds a single target at a time and throws the executable away,
because its job is comparing bytes.  This script does the other job: it links
main() together with every callee that has been reconstructed, plus the logging
stand-ins in src/run/stubs.c for the ones that have not, and leaves a program
behind that starts up and runs.

Two things worth knowing before editing the lists below.

`main.c`'s `@extra` line is a check.py convention that the compiler ignores
(it is a comment), so the link list is spelled out here instead of being
derived.  Every entry needs a distinct base name: bcbuild stages sources into
one flat DOS directory, and two files called the same thing would overwrite each
other.

A reconstruction is only worth linking if calling it is harmless.  `dacupd.c` is
byte-exact but hands a pointer to `dac_write`, which emits 0x300 bytes to the
VGA DAC; inside DOSBox that just paints garbage over the display, so it stays
out of the default build.  `f44d.c` is left out for a different reason: it
defines `g_3550`, which `dacupd.c` also defines, and a link with both fails on
the duplicate symbol.
"""
import os
import sys
import argparse

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import bcbuild
import dosbox

ROOT = bcbuild.ROOT
SRC = os.path.join(ROOT, 'src')

# main() plus everything it calls that has been verified against PETE.EXE.
# stubs.c must be present: it is still the only definition of the unresolved far
# callees m_b11a and m_f44d, and main() reaches them with a hard `lcall`, so the
# link fails without it.
#
# dacwrite.c is linked because dacupd.c calls it, not because it is wanted at
# run time: with g_3858 pointing at a zeroed record, m_f44c sees p[2] == 0 and
# returns before the DAC write ever happens.  Should that flag ever be set, this
# build would push 768 bytes at the DAC data port and paint the screen.
#
# nodefree.c, e256.c and buftovg.c each need their own cross-module callee
# supplied as a separate module, because a call to a function in a different
# .c file is an lcall in the large model: xd19dc.c, xe161d.c and xb2vcb.c are
# those three, all still stand-ins.  None of the three can crash the run --
# every one of them loops on a count read out of the record stubs.c points at,
# and that record is zeroed, so the loops run zero times.
#
# f44bstub.c holds m_f44b's stand-in rather than stubs.c, because --with-partial
# swaps that one file out for the exact f44b.c.  A public symbol defined in both
# a file both builds link and a file only one of them links is a duplicate, and
# Turbo Link's response to a duplicate public symbol is to loop emitting map
# entries until the disk fills -- bcbuild catches that as a LINK RUNAWAY, not as
# an error message, so the split is deliberate.
CORE = [
    os.path.join(SRC, 'main.c'),
    # Owns every global shared between main() and mcallees.c.  Without it Turbo
    # Link reports them undefined and then runs away emitting PETE.MAP.
    os.path.join(SRC, 'g13b2.c'),
    os.path.join(SRC, 'mcallees.c'),
    os.path.join(SRC, 'run', 'stubs.c'),
    os.path.join(SRC, 'run', 'f44bstub.c'),
    os.path.join(SRC, 'keypoll.c'),
    os.path.join(SRC, 'keyread.c'),
    os.path.join(SRC, 'statebak.c'),
    os.path.join(SRC, 'clrbit3.c'),
    os.path.join(SRC, 'mb159.c'),
    os.path.join(SRC, 'dacupd.c'),
    os.path.join(SRC, 'dacwrite.c'),
    os.path.join(SRC, 'nodefree.c'),
    os.path.join(SRC, 'xmod', 'xd19dc.c'),
    os.path.join(SRC, 'e256.c'),
    os.path.join(SRC, 'xmod', 'xe161d.c'),
    os.path.join(SRC, 'buftovg.c'),
    os.path.join(SRC, 'xmod', 'xb2vcb.c'),
]

# Opt-in reconstructions that touch hardware when they run.  m_f44b writes VGA
# CRTC registers through its setcrtc helper even on the early branch, so it is
# no more run-safe than dacwrite.c is.  f44b.c replaces run/f44bstub.c outright
# rather than joining it; SWAPPED names the file that steps aside.
SWAPPED = os.path.join(SRC, 'run', 'f44bstub.c')

PARTIAL = [
    os.path.join(SRC, 'f44b.c'),
    os.path.join(SRC, 'xmod', 'xsetcrtc.c'),
]


def report(sources):
    missing = [s for s in sources if not os.path.exists(s)]
    for s in missing:
        print('missing source: %s' % os.path.relpath(s, ROOT))
    return not missing


def clean_work():
    """Empty C:\\WORK before linking.

    Not optional housekeeping.  bcbuild.unstage() looks in WORK before OUT, so a
    leftover file there wins over the one TLINK just produced, and a build can
    silently report success while copying a stale binary.  Stale .OBJ files are
    the same hazard: the linker only sees the ones named in the response file,
    but an old .EXE or .MAP in WORK will be picked up and copied out.
    """
    os.makedirs(dosbox.WORK, exist_ok=True)
    removed = 0
    for f in os.listdir(dosbox.WORK):
        p = os.path.join(dosbox.WORK, f)
        if os.path.isfile(p):
            os.remove(p)
            removed += 1
    return removed


def main():
    ap = argparse.ArgumentParser(
        description='Link the decompiled game into one runnable PETE.EXE.')
    ap.add_argument('--name', default='PETE',
                    help='output name, must be 8.3 safe (default PETE)')
    ap.add_argument('--with-partial', action='store_true',
                    help='also link f44b/xsetcrtc (they write VGA CRTC registers)')
    ap.add_argument('--no-asm', action='store_true',
                    help='skip the -S listings')
    args = ap.parse_args()

    sources = list(CORE)
    if args.with_partial:
        # The real m_f44b replaces its stand-in; it cannot sit alongside it.
        sources = [s for s in sources if os.path.normcase(s) != os.path.normcase(SWAPPED)]
        sources += PARTIAL
    if not report(sources):
        print('build aborted: a required source is absent')
        return 1

    print('cleaned %d stale file(s) from WORK' % clean_work())
    for s in sources:
        print('  link %s' % os.path.relpath(s, ROOT))

    r = bcbuild.build(sources, name=args.name.upper(),
                      want_asm=not args.no_asm)

    print('')
    print('flags %s   model %s' % (r['flags'], r['model']))
    print('rc %s   ok %s' % (r['rc'], r['ok']))

    if not r['ok']:
        # Surface the compiler's own words rather than a summary of them; the
        # first error is usually the real one and the rest are fallout.
        print('')
        print(r['log'].replace('\r\n', '\n').rstrip())
        print('')
        print('build failed: no %s.EXE was produced' % args.name.upper())
        return 1

    print('exe  %s' % os.path.relpath(r['exe'], ROOT))
    print('')
    print('run it with:  python tools\\py\\run_game.py')
    return 0


if __name__ == '__main__':
    sys.exit(main())