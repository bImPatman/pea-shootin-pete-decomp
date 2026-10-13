/* @target FUN_1f44_04a2 */
/* @name   FUN_1f44_04a2 */
/* @proto  void FUN_1f44_04a2(char far *p) */
/* @module cross */
/* @extra  xmod/xg3550.c xmod/xfa6007f.c */

extern unsigned char g_3550[0x300];
extern void FUN_1fa6_007f(char far *buf);

void FUN_1f44_04a2(char far *p)
{
    FUN_1fa6_007f((char far *)g_3550);
}
