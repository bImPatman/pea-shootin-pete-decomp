/* @target 0xD48E */
/* @func  FUN_1d19_02fe
 *
 * Target (49 bytes / 17 insns):
 *   D48E  55             push bp
 *   D48F  8bec           mov bp, sp
 *   D491  c45e06         les bx, ptr [bp + 6]
 *   D494  8a460a         mov al, byte ptr [bp + 0xa]
 *   D497  2688472b       mov byte ptr es:[bx + 0x2b], al
 *   D49B  8a460c         mov al, byte ptr [bp + 0xc]
 *   D49E  2688472c       mov byte ptr es:[bx + 0x2c], al
 *   D4A2  26c6473700     mov byte ptr es:[bx + 0x37], 0
 *   D4A7  26807f2b04     cmp byte ptr es:[bx + 0x2b], 4
 *   D4AC  7507           jne 0xd4b5
 *   D4AE  26c6472dff     mov byte ptr es:[bx + 0x2d], 0xff
 *   D4B3  5d             pop bp
 *   D4B4  cb             retf
 *   D4B5  c45e06         les bx, ptr [bp + 6]
 *   D4B8  26c6472d00     mov byte ptr es:[bx + 0x2d], 0
 *   D4BD  5d             pop bp
 *   D4BE  cb             retf
 *
 * The compare re-reads the *stored* field (`cmp byte ptr es:[bx + 0x2b], 4`)
 * rather than testing the parameter in a register, so the source must compare
 * `rec->a` and not the incoming `x` - Borland folds the register form away.
 * Record layout recovered: bytes at +0x2b, +0x2c, +0x2d and +0x37.
 */
struct set_s {
    unsigned char pad0[0x2B];
    unsigned char a;
    unsigned char b;
    unsigned char c;
    unsigned char pad1[9];
    unsigned char d;
};

void setflds(struct set_s far *rec, unsigned char x, unsigned char y)
{
    rec->a = x;
    rec->b = y;
    rec->d = 0;
    if (rec->a == 4) {
        rec->c = 0xFF;
        return;
    }
    rec->c = 0;
}