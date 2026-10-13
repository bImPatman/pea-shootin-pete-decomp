/* @target FUN_13b2_35dd */
/* @name   m_35dd */
/* @proto  void m_35dd(void) */
/* @module same */
/* @extra  g13b2.c xmod/xe253c6.c xmod/xf440426.c xmod/x1f4429e.c xmod/xf0276.c xmod/xb111106.c xmod/xb1111cc.c xmod/xf02bf.c xmod/x0df025f.c xmod/xb1134c.c xmod/x1028ef.c xmod/x1002937.c xmod/xf4404a2.c xmod/x161d38e.c xmod/x163a9.c xmod/xb1100b5.c xmod/x11ae1.c xmod/xf440319.c */
#include <string.h>

extern char far *g_27aa;
extern char far *g_27ae;
extern char far *g_3858;
extern unsigned char g_2c33[7];
extern unsigned char g_2c56[0x20];
extern unsigned long  g_num_cur;
extern unsigned char  g_7d6;

int  FUN_0e25_03c6(char far *p, char far *q);
int  FUN_1f44_0426(char far *p);
void FUN_1f44_029e(char far *p, char v);
void FUN_1f44_0276(char far *p, int a, int b);
char far *FUN_1b11_1106(int a, int b, char far *s);
void FUN_1b11_11cc(char far *p, int v);
int  FUN_1000_02bf(char far *p);
char far *FUN_0df0_025f(char a, int b);
void FUN_1b11_034c(int a, int b, char far *s, char c, int d, int e, char f);
char far *FUN_1000_28ef(int a, char far *b, int c);
char far *FUN_1000_2937(unsigned long v, char far *buf, int base);
void FUN_1f44_04a2(char far *p);
void FUN_161d_038e(void);
int  FUN_161d_03a9(int v);
void FUN_1b11_00b5(int a, int b);
int  FUN_1000_1ae1(void);
void FUN_1f44_0319(char far *p, int a, int b, int c);

void m_35dd(void)
{
    int i;
    int x;
    int y;

    FUN_0e25_03c6(g_27aa, "menu");
    FUN_1f44_0426(g_3858);
    FUN_1f44_029e(g_3858, 0);
    FUN_1f44_0276(g_3858, 0, 0);
    g_27ae = FUN_1b11_1106(0, 0, "slot");
    FUN_1b11_11cc(g_27ae, 0);
    FUN_1000_02bf(g_27ae);
    FUN_1f44_0276(g_3858, 0, 1);

    i = 0;
    y = 0x32;
    do {
        if (i == 0)
            x = 0x10;
        else
            x = 0x20;
        if (FUN_0df0_025f(i, 3))
            FUN_1b11_034c(0x32, y, "flag", x, 0x1f, 1, 0);
        FUN_1000_28ef(i + 1, g_2c33, 0xa);
        strcat(g_2c33, "z");
        FUN_1b11_034c(0x46, y, g_2c33, x, 0x1f, 1, 0);
        strcpy(g_2c33, FUN_0df0_025f(i, 2));
        FUN_1b11_034c(0x78, y, g_2c33, x, 0x1f, 1, 0);
        strcpy(g_2c33, FUN_0df0_025f(i, 1));
        FUN_1b11_034c(0xc8, y, g_2c33, x, 0x1f, 1, 0);
        y += 0xa;
        i++;
    } while (y != 0x96);

    strcpy(g_2c33, "0123456789abcdef");
    strcat(g_2c33, FUN_1000_2937(g_num_cur, g_2c56, 0xa));
    FUN_1b11_034c(0x3c, 0xaa, g_2c33, 0x38, 0x1f, 1, 0);
    FUN_1b11_034c(0x1e, 0xbe, "hint", 0x20, 0x1f, 1, 0);
    FUN_1f44_04a2(g_3858);

    i = 0x258;
    while (!FUN_1000_1ae1() && i--) {
        if (g_7d6) {
            FUN_161d_038e();
            if (FUN_161d_03a9(0))
                break;
        }
        FUN_1b11_00b5(1, 0);
    }

    FUN_1f44_0319(g_3858, 0, 1, 2);
}
