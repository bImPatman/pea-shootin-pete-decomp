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
byte-exact and linked, but the record it reads is zeroed, so m_f44c returns
before it can call `dac_write`, which emits 0x300 bytes to the VGA DAC (and would
paint garbage over the display).  `f44d.c` declares `g_3550` rather than defining
it -- src/dacupd.c owns it -- so the two link together.
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
# stubs.c must be present: it holds the run-time storage main() and the linked
# bodies share, plus stub_boot/emit/note that f44bstub.c calls.  m_b11a and
# m_f44d are now linked from their own reconstructions, not stubbed here.
#
# pal006a.asm is linked because dacupd.c calls it (as `dac_write`), not because
# it is wanted at run time: with g_3858 pointing at a zeroed record, m_f44c sees
# p[2] == 0 and returns before the DAC write ever happens.  Should that flag ever
# be set, this build would push 768 bytes at the DAC data port and paint the
# screen.  The module is the byte-exact hand-asm FUN_1fa6_006a.
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
    os.path.join(SRC, 'dacwrite.c'),  # stub
    os.path.join(SRC, 'nodefree.c'),
    os.path.join(SRC, 'nodefree.c'),
    os.path.join(SRC, 'nodefree.c'),
    os.path.join(SRC, 'nodefree.c'),
    os.path.join(SRC, 'xmod', 'xd19dc.c'),
    os.path.join(SRC, 'e256.c'),
    os.path.join(SRC, 'xmod', 'xe161d.c'),
    os.path.join(SRC, 'buftovg.c'),
    os.path.join(SRC, 'xmod', 'xb2vcb.c'),

    # f44d.c (exact m_f44d).  It declares `g_3550` extern, so dacupd.c above owns
    # it.  Both callees need a definition or Turbo Link runs away on the
    # unresolved `_pal_shift`/`_upload`: pal_shift is on m_f44d's live path, while
    # upload is reached only from the dead half the compiler still emits.  Both
    # are now byte-exact hand-asm -- pal002f.asm (FUN_1fa6_002f) and pal006a.asm
    # (FUN_1fa6_006a, linked above, which exports `_upload` too).
    os.path.join(SRC, 'f44d.c'),
    os.path.join(SRC, 'xmod', 'xshift.c'),
    os.path.join(SRC, 'xmod', 'xupload.c'),
    # os.path.join(SRC, 'pal002f.asm'),

    # Stand-ins for the far callees of the four reconstructed bodies (m_1df6,
    # m_24e7, m_013b, m_2c80).  Each of those source files carries its own
    # `@extra` list for check.py; this is the union of the four, minus
    # xmod/xe161d.c which is already linked above for e256.c.  They are needed
    # at link time because a far call to a symbol no module defines is the
    # exact condition Turbo Link answers with a runaway .MAP.
    #
    # The ones returning a far pointer all hand back a static 64-byte buffer,
    # so m_1df6's `g_27aa = FUN_1e25_000a(...)` and friends land on real memory
    # rather than on a null segment.  What they point *at* is still zeroes,
    # which matters for the dispatch table -- see g_dispatch in src/g13b2.c.
    os.path.join(SRC, 'xmod', 'xe25978.c'),
    os.path.join(SRC, 'xmod', 'xe256fe.c'),
    os.path.join(SRC, 'xmod', 'xe25b0f.c'),
    # FUN_1f44_029e is reconstructed (src/f4429e.c) and replaces the stand-in
    # xmod/x1f4429e.c.
    os.path.join(SRC, 'f4429e.c'),
    os.path.join(SRC, 'xmod', 'xd1914a.c'),
    os.path.join(SRC, 'xmod', 'xd1932f.c'),
    os.path.join(SRC, 'xmod', 'xd192fe.c'),
    os.path.join(SRC, 'xmod', 'x1e25b0.c'),
    os.path.join(SRC, 'xmod', 'x161d38e.c'),
    # FUN_1b11_034c is reconstructed (src/mb1134c.c) and replaces the stand-in
    # xmod/xb1134c.c; xmod/xfd301fa.c is the link stand-in for its glyph blitter.
    os.path.join(SRC, 'mb1134c.c'),
    os.path.join(SRC, 'xmod', 'xfd301fa.c'),
    os.path.join(SRC, 'xmod', 'xb1100b5.c'),
    os.path.join(SRC, 'xmod', 'xb11462.c'),
    os.path.join(SRC, 'xmod', 'x1028ef.c'),
    os.path.join(SRC, 'xmod', 'x1e2555.c'),
    os.path.join(SRC, 'xmod', 'x1e254a.c'),
    os.path.join(SRC, 'xmod', 'xf4404b3.c'),
    os.path.join(SRC, 'xmod', 'x0adb1da.c'),
    os.path.join(SRC, 'xmod', 'xb111597.c'),
    os.path.join(SRC, 'xmod', 'xb111a47.c'),
    os.path.join(SRC, 'xmod', 'xb111106.c'),
    os.path.join(SRC, 'xmod', 'x1000ef8.c'),
    os.path.join(SRC, 'xmod', 'x1000c92.c'),
    os.path.join(SRC, 'xmod', 'x1000c91.c'),
    os.path.join(SRC, 'xmod', 'x0f92084.c'),
    os.path.join(SRC, 'xmod', 'x0f9206.c'),
    os.path.join(SRC, 'xmod', 'x0df000e.c'),
    os.path.join(SRC, 'xmod', 'xb11195d.c'),
    # FUN_1b11_1566 is reconstructed (src/mb1566.c) and replaces the stand-in
    # xmod/xb111566.c, which still provides the symbol for the check harnesses.
    os.path.join(SRC, 'mb1566.c'),
    os.path.join(SRC, 'xmod', 'xb1115d6.c'),
    os.path.join(SRC, 'xmod', 'xb110056.c'),
    os.path.join(SRC, 'xmod', 'x0adb324.c'),
    os.path.join(SRC, 'xmod', 'x1e253c6.c'),
    os.path.join(SRC, 'xmod', 'xb1111cc.c'),
    os.path.join(SRC, 'xmod', 'xf02bf.c'),
    os.path.join(SRC, 'xmod', 'xb111653.c'),
    os.path.join(SRC, 'xmod', 'x0adb1ff.c'),
    os.path.join(SRC, 'xmod', 'x0adb1.c'),
    os.path.join(SRC, 'xmod', 'x0adb224.c'),
    os.path.join(SRC, 'xmod', 'x0adb2bc.c'),
    os.path.join(SRC, 'xmod', 'x11ae1.c'),
    os.path.join(SRC, 'xmod', 'x118e4.c'),
    os.path.join(SRC, 'xmod', 'xb11009a.c'),
    os.path.join(SRC, 'xmod', 'x163c7.c'),
    os.path.join(SRC, 'xmod', 'x163e4.c'),
    os.path.join(SRC, 'xmod', 'x163a9.c'),
    os.path.join(SRC, 'xmod', 'xe2503a4.c'),
    os.path.join(SRC, 'xmod', 'xe250a23.c'),
    os.path.join(SRC, 'xmod', 'xe250a89.c'),
    os.path.join(SRC, 'f440426.c'),
    os.path.join(SRC, 'xmod', 'xe253c6.c'),
    os.path.join(SRC, 'f440276.c'),
    os.path.join(SRC, 'xmod', 'x0ffe0e.c'),
    os.path.join(SRC, 'xmod', 'xf4403af.c'),
    os.path.join(SRC, 'xmod', 'xb111dc1.c'),
    os.path.join(SRC, 'xmod', 'xd1995c.c'),
    os.path.join(SRC, 'xmod', 'xd19104.c'),
    os.path.join(SRC, 'xmod', 'xb1119d2.c'),
    os.path.join(SRC, 'xmod', 'xf440188.c'),
    os.path.join(SRC, 'xmod', 'xf4400fe.c'),
    os.path.join(SRC, 'f4404a2.c'),
    os.path.join(SRC, 'xmod', 'xf4403f1.c'),
    # m_34e5's far callees (the body now lives in mcallees.c).
    os.path.join(SRC, 'xmod', 'x0df222.c'),
    os.path.join(SRC, 'xmod', 'x0df181.c'),
    os.path.join(SRC, 'xmod', 'xb110c4d.c'),
    os.path.join(SRC, 'xmod', 'xb11079e.c'),
    # m_0dd6's far callees that the four bodies above do not already supply
    # (x1e25b0.c for FUN_1e25_0b0f is linked earlier for m_2c80).
    os.path.join(SRC, 'xmod', 'x1e253a4.c'),
    os.path.join(SRC, 'xmod', 'x1e25a23.c'),
    os.path.join(SRC, 'xmod', 'x1e25a89.c'),
    # m_b11a's far callees (the body now lives in mcallees.c).
    os.path.join(SRC, 'xmod', 'xfd3.c'),
    os.path.join(SRC, 'xmod', 'xfc2006.c'),
    os.path.join(SRC, 'xmod', 'xd1968d.c'),
    os.path.join(SRC, 'xmod', 'xd1973e.c'),
    # m_154f / FUN_13b2_17ba's RTL number->ASCII helper (the HUD score).  It
    # must be linked, not just declared: TLINK still writes the .EXE when this
    # symbol is undefined, so the build looked clean while the HUD number-draw
    # called a null target.
    os.path.join(SRC, 'xmod', 'x1002937.c'),
    # m_38a2's far callees that the lists above do not already supply (the body
    # now lives in mcallees.c).
    os.path.join(SRC, 'xmod', 'xe250426.c'),
    os.path.join(SRC, 'xmod', 'xf44035c.c'),
    os.path.join(SRC, 'xmod', 'xfd3123.c'),
    # m_35dd's far callees that the lists above do not already supply (the body
    # now lives in mcallees.c).  FUN_0df0_025f hands back a static buffer.
    os.path.join(SRC, 'xmod', 'x0df025f.c'),
    # FUN_1f44_0319 (and its co-located near callee FUN_1f44_0309) is
    # reconstructed in src/f440319.c and replaces xmod/xf440319.c.
    os.path.join(SRC, 'f440319.c'),
    # The menu shape/colour wrappers above are now byte-exact reconstructions
    # (f4429e.c, f440276.c, f4404a2.c, f440319.c, f440426.c).  The deeper
    # primitives they call are byte-exact hand-asm too: fba0002.asm
    # (FUN_1fba_0002, planar rect fill) and pal007f.asm (FUN_1fa6_007f, palette
    # fade, which also carries its two near neighbours FUN_1fa6_00df/0127 so the
    # near `call` rel16 matches).  FUN_1000_1a1f, the int86x-style helper
    # FUN_1f44_0426 loads the DAC with, is still a C stand-in.
    os.path.join(SRC, 'xmod', 'xfba.c'),
    os.path.join(SRC, 'xmod', 'xfa6007f.c'),
    os.path.join(SRC, 'xmod', 'x1a1f.c'),
    # m_e2500a's / m_f4406's far callees (both bodies now live in mcallees.c,
    # replacing x1e2500a.c / x1f4406.c).  FUN_1000_0778 is a bump allocator so
    # g_27aa and g_3858 get distinct records; the rest are inert.  FUN_1000_2c09_1
    # (x1000c91.c) and FUN_1b11_0056 (xb110056.c) are already linked above.
    os.path.join(SRC, 'xmod', 'x0778.c'),
    os.path.join(SRC, 'xmod', 'x19ee.c'),
    os.path.join(SRC, 'xmod', 'x161d840.c'),
    os.path.join(SRC, 'xmod', 'x161d820.c'),
    os.path.join(SRC, 'xmod', 'x161d401.c'),
    os.path.join(SRC, 'xmod', 'x161d456.c'),
    os.path.join(SRC, 'xmod', 'x161da58.c'),
    os.path.join(SRC, 'xmod', 'x161d24a.c'),
    os.path.join(SRC, 'xmod', 'x161d363.c'),
    os.path.join(SRC, 'xmod', 'x161d338.c'),
    os.path.join(SRC, 'xmod', 'xfd3178.c'),
    os.path.join(SRC, 'xmod', 'xfd31e1.c'),
    os.path.join(SRC, 'xmod', 'xfd3150.c'),
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