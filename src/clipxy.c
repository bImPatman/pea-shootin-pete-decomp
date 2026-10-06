/* @target 0x8B74 */
/* FUN_18a2_0154 - 189 bytes, 66 instructions, shape + exact.
 *
 * Clamps a pair of coordinates (x, y) held through far pointers p and q,
 * adjusting the record fields as it goes.  The whole body sits inside one
 * basic-block region after the `f32 == 3` test, which is why bx keeps `a`
 * live from the initial `les bx,[bp+6]` load all the way to 0x8bd4 without a
 * reload; the merge at 0x8bd7 reloads it.
 *
 * The 0x136 clamp used to look unreachable: Ghidra's export skipped the basic
 * block at 0x8bb6..0x8bd6 entirely, so the listing jumped straight from the
 * `inc word ptr es:[bx+0xd]` to the merge.  tools/py/repairholes.py refills
 * those orphan holes (885 instructions over 24 functions), after which the
 * block decodes as the `y + f6 > 0x136` test and clamp below.
 *
 * Record fields used here: f6 +0x06, f08 +0x08, f0b +0x0b, f0d +0x0d,
 * f32 +0x32, f66 +0x66.
 */
struct rec_s {
    unsigned char pad0[6];
    int f6;
    int f08;
    unsigned char pad1[1];
    int f0b;
    int f0d;
    unsigned char pad3[0x23];
    unsigned char f32;
    unsigned char pad4[0x33];
    unsigned char f66;
};

void clipxy(struct rec_s far *a, int far *p, int far *q)
{
    int x = *p + a->f0b;
    int y = *q;
    if (a->f32 == 3) {
        y += a->f0d / 2 + 4;
        a->f0d++;
        if (y + a->f6 > 0x136) {
            a->f66 |= 1;
            a->f0d = 0;
            y = 0x136 - a->f6 - 7;
        }
    }
    if (x + a->f08 >= 0x280)
        x = 0x280 - a->f08 - 1;
    if (y < 0) {
        y = *q;
        a->f0d = -a->f0d;
    }
    if (x < 0)
        x = *p;
    *p = x;
    *q = y;
}