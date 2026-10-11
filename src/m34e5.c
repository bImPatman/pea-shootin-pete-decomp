/* @target FUN_13b2_34e5 */
/* @name   m_34e5 */
/* @proto  void m_34e5(void) */
/* @module same */
/* @extra  g13b2.c xmod/x1f4429e.c xmod/xf0276.c xmod/xf4403f1.c xmod/xf02bf.c xmod/xb110c4d.c xmod/xb11079e.c xmod/x0df181.c xmod/x0df222.c */

#include <string.h>

extern unsigned char g_2805, g_27fc;
extern char far *g_3858;
extern char g_2c33[7];

unsigned long g_2bfimer;      /* DS:0x2BFF low word / 0x2C01 high word */

unsigned char FUN_1df0_0222(unsigned long v);
void       FUN_1df0_0181(unsigned char a, unsigned long b, char far *s, unsigned char c);
int        FUN_1f44_029e(char far *p, char v);
int        FUN_1f44_0276(char far *p, int a, int b);
int        FUN_1f44_03f1(char far *p);
char far  *FUN_1b11_0c4d(char far *p, char far *s, int n);
void       FUN_1b11_079e(char far *p, int flag);
void       FUN_1000_02bf(char far *p);
char far  *FUN_13b2_4ecf(char far *p);

void m_34e5(void)
{
    int v;
    char far *p;

    if (g_2805 == 1)
        return;

    if ((v = FUN_1df0_0222(g_2bfimer)) == 0)
        return;

    FUN_1f44_029e(g_3858, 0);
    FUN_1f44_0276(g_3858, 0, 0);
    FUN_1f44_03f1(g_3858);
    p = FUN_13b2_4ecf(0);
    strcpy(g_2c33, FUN_1b11_0c4d(p, "sb_green.l", 8));
    FUN_1df0_0181(v, g_2bfimer, g_2c33, g_27fc);
    if (p) {
        FUN_1b11_079e(p, 0);
        FUN_1000_02bf(p);
    }
}

char far *FUN_13b2_4ecf(char far *p)
{
    return p;
}
