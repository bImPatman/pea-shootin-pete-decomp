/* @target FUN_1f44_0276 */
/* @name   FUN_1f44_0276 */
/* @proto  void FUN_1f44_0276(char far *p, unsigned char a, unsigned char b) */
/* @module cross */
/* @extra  xmod/xfba.c */

extern void FUN_1fba_0002(int x, int y, int x2, int y2, int base, int color);

void FUN_1f44_0276(char far *p, unsigned char a, unsigned char b)
{
    FUN_1fba_0002(0, 0, 320, 200, b * 16000, a);
}
