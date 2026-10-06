/* @target 0xF531 */
/* @func  FUN_1f44_00f1
 *
 * Target (13 bytes / 6 insns):
 *   0F531  55          push bp
 *   0F532  8bec        mov bp, sp
 *   0F534  c45e06      les bx, ptr [bp + 6]
 *   0F537  26c6470300  mov byte ptr es:[bx + 3], 0
 *   0F53C  5d          pop bp
 *   0F53D  cb          retf
 *
 * Cross-module far function (retf, args at [bp+6]); same `les bx, [bp+6]`
 * idiom as setkind.c. This is the only difference from setkind.c: the byte
 * offset (3 vs 0x30) and the stored value (0 vs 1).
 */
void clrbit3(unsigned char far *rec)
{
    rec[3] = 0;
}