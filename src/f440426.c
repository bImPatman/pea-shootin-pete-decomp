/* @target FUN_1f44_0426 */
/* @name   FUN_1f44_0426 */
/* @proto  void FUN_1f44_0426(void) */
/* @module cross */
/* @extra  xmod/x1a1f.c */

#include <dos.h>

extern int FUN_1000_1a1f(int no, union REGS far *in, union REGS far *out,
                         struct SREGS far *seg);

void FUN_1f44_0426(void)
{
    union REGS regs;
    struct SREGS segregs;
    char buf[0x300];
    int i;

    for (i = 0; i < 0x300; i++)
        buf[i] = 0;

    regs.x.ax = 0x1012;
    regs.x.bx = 0;
    regs.x.cx = 0x100;
    regs.x.dx = (unsigned)buf;
    segregs.es = _SS;
    FUN_1000_1a1f(0x10, &regs, &regs, &segregs);
}
