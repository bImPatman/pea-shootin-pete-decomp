/* @target 0xDAEC */
/* @func  FUN_1d19_095c
 *
 * Target (13 bytes / 6 insns):
 *   0DAEC  55          push bp
 *   0DAED  8bec        mov bp, sp
 *   0DAEF  c45e06      les bx, ptr [bp + 6]
 *   0DAF2  26c6473001  mov byte ptr es:[bx + 0x30], 1
 *   0DAF7  5d          pop bp
 *   0DAF8  cb          retf
 *
 * Cross-module far function (retf, args at [bp+6]). The far pointer arrives
 * in the [bp+6] 4-byte slot, so Borland loads it with a single `les bx, [bp+6]`
 * and then indexes through es:bx with no segment reload.
 */
void setkind(unsigned char far *rec)
{
    rec[0x30] = 1;
}