/* @target 0xD294 */
/* @func  FUN_1d19_0104
 *
 * Target (70 bytes / 23 insns):
 *   D294  55             push bp
 *   D295  8bec           mov bp, sp
 *   D297  807e0e00       cmp byte ptr [bp + 0xe], 0
 *   D29B  7406           je 0xd2a3
 *   D29D  d1660a         shl word ptr [bp + 0xa], 1
 *   D2A0  d1660c         shl word ptr [bp + 0xc], 1
 *   D2A3  c45e06         les bx, ptr [bp + 6]
 *   D2A6  8b460a         mov ax, word ptr [bp + 0xa]
 *   D2A9  2689470f       mov word ptr es:[bx + 0xf], ax
 *   D2AD  26894713       mov word ptr es:[bx + 0x13], ax
 *   D2B1  26894702       mov word ptr es:[bx + 2], ax
 *   D2B5  8b460c         mov ax, word ptr [bp + 0xc]
 *   D2B8  26894711       mov word ptr es:[bx + 0x11], ax
 *   D2BC  26894715       mov word ptr es:[bx + 0x15], ax
 *   D2C0  26894704       mov word ptr es:[bx + 4], ax
 *   D2C4  268b4702       mov ax, word ptr es:[bx + 2]
 *   D2C8  d1f8           sar ax, 1
 *   D2CA  2689471b       mov word ptr es:[bx + 0x1b], ax
 *   D2CE  268b4704       mov ax, word ptr es:[bx + 4]
 *   D2D2  d1f8           sar ax, 1
 *   D2D4  2689471d       mov word ptr es:[bx + 0x1d], ax
 *   D2D8  5d             pop bp
 *   D2D9  cb             retf
 *
 * `u *= 2` does not work here: Borland emits `mov dx,2 / imul dx` and the whole
 * function grows to 86 bytes.  `u <<= 1` gives the target's in-place
 * `shl word ptr [bp + 0xa], 1` - writing back through the parameter slot.
 *
 * The halving at the end re-reads the *stored* fields (`mov ax, es:[bx + 2]`)
 * rather than reusing `u`, so those two lines must be written against `r->x1`
 * and `r->y1`.  `sar` confirms the fields are signed `int`.
 */
struct rect_s {
    unsigned char pad0[2];
    int x1, y1;
    unsigned char pad1[9];
    int a1, b1, a2, b2;
    unsigned char pad2[4];
    int ha, hb;
};

void dblbox(struct rect_s far *r, int u, int v, unsigned char w)
{
    if (w) {
        u <<= 1;
        v <<= 1;
    }
    r->a1 = u;
    r->a2 = u;
    r->x1 = u;
    r->b1 = v;
    r->b2 = v;
    r->y1 = v;
    r->ha = r->x1 >> 1;
    r->hb = r->y1 >> 1;
}