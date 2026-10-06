/* @target 0xF02E */
/* @func  FUN_1e25_0dde
 *
 * Target (94 bytes / 28 insns):
 *   F02E  55             push bp
 *   F02F  8bec           mov bp, sp
 *   F031  c45e06         les bx, ptr [bp + 6]
 *   F034  b000           mov al, 0
 *   F036  26884705       mov byte ptr es:[bx + 5], al
 *   F03A  268807         mov byte ptr es:[bx], al
 *   F03D  26884704       mov byte ptr es:[bx + 4], al
 *   F041  833efa1800     cmp word ptr [0x18fa], 0
 *   F046  7428           je 0xf070
 *   F048  268b4712       mov ax, word ptr es:[bx + 0x12]
 *   F04C  2689470c       mov word ptr es:[bx + 0xc], ax
 *   F050  26817f0cd200   cmp word ptr es:[bx + 0xc], 0xd2
 *   F056  7e06           jle 0xf05e
 *   F058  26c7470cd200   mov word ptr es:[bx + 0xc], 0xd2
 *   F05E  c45e06         les bx, ptr [bp + 6]
 *   F061  26837f0c28     cmp word ptr es:[bx + 0xc], 0x28
 *   F066  7d13           jge 0xf07b
 *   F068  26c7470c2800   mov word ptr es:[bx + 0xc], 0x28
 *   F06E  eb0b           jmp 0xf07b
 *   F070  c45e06         les bx, ptr [bp + 6]
 *   F073  268b4714       mov ax, word ptr es:[bx + 0x14]
 *   F077  2689470c       mov word ptr es:[bx + 0xc], ax
 *   F07B  c45e06         les bx, ptr [bp + 6]
 *   F07E  268b470c       mov ax, word ptr es:[bx + 0xc]
 *   F082  2689470e       mov word ptr es:[bx + 0xe], ax
 *   F086  26894710       mov word ptr es:[bx + 0x10], ax
 *   F08A  5d             pop bp
 *   F08B  cb             retf
 *
 * Resets three flag bytes, then clamps `v` into [0x28, 0xD2] when a global
 * selects the "scaled" source and copies it straight through otherwise.  The
 * final two stores reuse the value still live in `ax`, which is why they are
 * plain `mov es:[bx+N], ax` with no reload.
 *
 * The three zero stores are a chain assignment, not three statements - see the
 * note on `c->f4 = c->f0 = c->f5 = 0` below.
 */
struct cfg_s {
    unsigned char f0;
    unsigned char pad1[3];
    unsigned char f4;
    unsigned char f5;
    unsigned char pad2[6];
    int v;
    int w, x;
    int src1, src2;
};
int gflag;
void clampv(struct cfg_s far *c)
{
    c->f4 = c->f0 = c->f5 = 0;
    if (gflag != 0) {
        c->v = c->src1;
        if (c->v > 0xD2)
            c->v = 0xD2;
        if (c->v < 0x28)
            c->v = 0x28;
    } else {
        c->v = c->src2;
    }
    c->w = c->v;
    c->x = c->v;
}