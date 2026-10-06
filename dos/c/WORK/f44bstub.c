/* Run-mode stand-in for m_f44b, kept in its own file so the two builds can
 * differ.
 *
 * m_f44b is byte-exact in src/f44b.c, but linking that one is opt-in
 * (build_game.py --with-partial) because it writes VGA CRTC registers through
 * its setcrtc helper even on the early branch.  src/run/stubs.c is linked by
 * both builds, so the stand-in cannot live there: two public definitions of
 * _m_f44b and Turbo Link loops emitting map entries until the disk fills,
 * which bcbuild reports as a LINK RUNAWAY rather than a duplicate-symbol error.
 * Keeping it separate lets the default build supply the stand-in and the
 * --with-partial build supply the reconstruction.
 */
/* Defined in run/stubs.c, which both builds link.  Prototypes are spelled out
   rather than pulled in from a header because bcbuild stages only the .c files
   into C:\WORK, so a local #include would not be there to be found. */
void stub_boot(void);
void note(char *name, char *target);

int m_f44b(char far *p, unsigned char v)
{
    static int logged;
    stub_boot();
    if (!logged) { logged = 1; note("m_f44b", "FUN_1f44_029e"); }
    return 0;
}
