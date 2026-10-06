/* @target 0xF904 */
/* @func  FUN_1f44_04c4
 *
 * Target (34 bytes / 14 insns):
 *   F904  55             push bp
 *   F905  8bec           mov bp, sp
 *   F907  807e0a00       cmp byte ptr [bp + 0xa], 0
 *   F90B  7509           jne 0xf916
 *   F90D  c45e06         les bx, ptr [bp + 6]
 *   F910  26c60700       mov byte ptr es:[bx], 0
 *   F914  5d             pop bp
 *   F915  cb             retf
 *   F916  c45e06         les bx, ptr [bp + 6]
 *   F919  8a460a         mov al, byte ptr [bp + 0xa]
 *   F91C  26884701       mov byte ptr es:[bx + 1], al
 *   F920  26c60701       mov byte ptr es:[bx], 1
 *   F924  5d             pop bp
 *   F925  cb             retf
 *
 * Argument layout: the far pointer takes the 4-byte slot at [bp+6]..[bp+9],
 * so the trailing byte parameter lands at [bp+0xa].  Each arm re-loads the
 * pointer with its own `les`, which is what Borland does when a multi-return
 * function needs es again after the branch - so the early `return` here is
 * load-bearing, not cosmetic.
 */
struct tag_s {
    unsigned char state;
    unsigned char kind;
};

void settag(struct tag_s far *rec, unsigned char kind)
{
    if (kind == 0) {
        rec->state = 0;
        return;
    }
    rec->kind = kind;
    rec->state = 1;
}