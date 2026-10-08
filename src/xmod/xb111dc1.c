/* Stand-in for FUN_1b11_1dc1 -- returns a far pointer in DX:AX.
 *
 * The caller does not merely keep the pointer.  m_24e7 immediately runs
 *     ((struct obj_a1 far *)p)->vt->f4(p, 0, 0, 2, "zzzzzzzz", 2, 2);
 * and vt is a *near* pointer sitting at offset 0 of the record.  Left zeroed
 * that resolves to DS:0000, f4 is read out of the interrupt vector table, and
 * the far call jumps into whatever the vector names -- which is how the linked
 * build died on its first frame with nothing in the log.
 *
 * So the record is given a real vtable on the way out.  It cannot be a static
 * initializer: the offset of a static is not a constant expression, and vt has
 * to hold a DGROUP offset, not a segment:offset pair.
 *
 * Filling this in does not disturb src/m24e7.c's byte-exactness.  That file
 * only lcalls FUN_1b11_1dc1; the stub's own body is never part of the range
 * check.py diffs.
 */
struct vtbl_a1 {
    void (far *f0)(char far *, int, int, int, char far *, int, int);
    void (far *f4)(char far *, int, int, int, char far *, int, int);
};

static void vt_nop(char far *a, int b, int c, int d, char far *e, int f, int g)
{
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g;
}
static struct vtbl_a1 the_vt = { vt_nop, vt_nop };

static char buf[64];

char far *FUN_1b11_1dc1(char far *p, int a, int b, int c, int d, int e)
{
    (void)p; (void)a; (void)b; (void)c; (void)d; (void)e;
    *(struct vtbl_a1 near **)buf = (struct vtbl_a1 near *)&the_vt;
    return buf;
}
