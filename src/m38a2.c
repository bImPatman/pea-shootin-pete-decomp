/* @target FUN_13b2_38a2 */
/* @name   m_38a2 */
/* @proto  void m_38a2(void) */
/* @module same */
/* @extra  g13b2.c xmod/xe250426.c xmod/xe253c6.c xmod/xf44035c.c xmod/xf4404b3.c xmod/xf4404a2.c xmod/xf0276.c xmod/x1f4429e.c xmod/xf440188.c xmod/xf4400fe.c xmod/xb111106.c xmod/xb1111cc.c xmod/xf02bf.c xmod/xb1100b5.c xmod/xb110056.c xmod/xe256fe.c xmod/xfd3123.c */

extern int           g_2bf7, g_2bf9, g_2bfb, g_2bfd, g_19ea;
extern unsigned char g_2805, g_2c61, g_386d, g_3888, g_38a5, g_38b4, g_38bc;
extern unsigned char g_2808[];
extern char far *g_27aa;
extern char far *g_27ae;
extern char far *g_3858;

unsigned char FUN_1e25_0426(char far *p);
int  FUN_0e25_03c6(char far *p, char far *q);
int  FUN_1f44_0276(char far *p, int a, int b);
int  FUN_1f44_029e(char far *p, char a);
char far *FUN_1b11_1106(int a, int b, char far *s);
int  FUN_1b11_11cc(char far *p, int v);
int  FUN_1000_02bf(char far *p);
int  FUN_1f44_0188(char far *p, unsigned char a, unsigned char b, unsigned char c);
int  FUN_1f44_00fe(char far *p);
void FUN_1f44_04b3(char far *p);
int  FUN_1f44_04a2(char far *p);
int  FUN_1f44_035c(char far *p, char a, char b, char c);
int  FUN_1b11_00b5(int a, int b);
int  FUN_1b11_0056(char far *s);
void FUN_0e25_06fe(char far *p, char far *lit);
char FUN_1fd3_0123(int a, int b, int c);

void FUN_13b2_1f91(void);
void FUN_13b2_3cb6(int a);
void FUN_13b2_3d7c(void);
void FUN_13b2_3fb4(void);
void FUN_13b2_4134(void);

void FUN_13b2_1f91(void) { }

void m_38a2(void)
{
    int k;
    int n;
    int menu;
    int x;
    int y;
    int idx = 0;

    if (!FUN_1e25_0426(g_27aa))
        FUN_0e25_03c6(g_27aa, "0xfb9");

    g_2805 = 1;
    FUN_1f44_0276(g_3858, 0, 0);
    FUN_1f44_0276(g_3858, 0, 1);
    FUN_1f44_0276(g_3858, 0, 2);
    FUN_1f44_0276(g_3858, 0, 3);
    FUN_1f44_029e(g_3858, 0);
    g_27ae = FUN_1b11_1106(0, 0, "0xfc3");
    g_27ae[0x0a] = 3;
    FUN_1b11_11cc(g_27ae, 3);
    FUN_1000_02bf(g_27ae);
    FUN_1f44_029e(g_3858, 3);
    FUN_13b2_4134();
    FUN_1f44_0188(g_3858, 0xc0, 0xef, 2);
    FUN_1f44_00fe(g_3858);

    menu = 0;
    g_2bf7 = 0x3d;
    g_2bf9 = 0x42;
    for (y = 0x42; y < g_2bf9 + g_2bfd; y++) {
        for (x = g_2bf7; x < g_2bf7 + g_2bfb; x++) {
            g_2808[idx++] = FUN_1fd3_0123(x, y, g_19ea);
        }
    }

    FUN_13b2_3cb6(0);

restart:
    n = 0;
    while (g_3888 == 0 && g_38a5 == 0) {
        FUN_1b11_00b5(1, 1);
        if (g_38bc) {
            n = 0;
            menu++;
            if (menu > 5)
                menu = 0;
            while (g_38bc)
                FUN_1b11_00b5(1, 1);
            FUN_0e25_06fe(g_27aa, "0xfd0");
        }
        if (g_38b4) {
            n = 0;
            menu--;
            if (menu < 0)
                menu = 5;
            while (g_38b4)
                FUN_1b11_00b5(1, 1);
            FUN_0e25_06fe(g_27aa, "0xfdb");
        }
        if (g_386d)
            FUN_13b2_1f91();
        if (n++ == 0x4b0) {
            g_2805 = 1;
            menu = 6;
            break;
        }
        FUN_13b2_3cb6(menu);
    }

    switch (menu) {
    case 0:
        FUN_1f44_04b3(g_3858);
        g_2805 = 0;
        g_2c61 = 0;
        break;
    case 1:
        FUN_1f44_04b3(g_3858);
        g_2805 = 0;
        g_2c61 = 1;
        break;
    case 2:
        FUN_1f44_035c(g_3858, 3, 2, 2);
        k = 2;
        for (;;) {
            if (g_386d) {
                FUN_1f44_029e(g_3858, 3);
                while (g_386d)
                    ;
                goto restart;
            }
            if (g_3888 != 0 || g_38a5 != 0) {
                if (k == 1) {
                    FUN_1f44_029e(g_3858, 3);
                    while (g_3888 || g_38a5)
                        ;
                    goto restart;
                }
                FUN_1f44_035c(g_3858, k, k + 0xff, 2);
                FUN_1b11_00b5(0x1e, 0);
                k--;
            } else
                continue;
        }
    case 3:
        FUN_13b2_3fb4();
        FUN_1f44_029e(g_3858, 3);
        FUN_1f44_04a2(g_3858);
        while (g_3888 || g_38a5)
            ;
        goto restart;
    case 4:
        FUN_13b2_3d7c();
        FUN_1f44_029e(g_3858, 3);
        while (g_3888 || g_38a5)
            ;
        goto restart;
    case 5:
        FUN_13b2_1f91();
        break;
    default:
        FUN_1b11_0056("0xfe6");
        break;
    case 6:
        break;
    }

    FUN_1f44_0276(g_3858, 0, 0);
    FUN_1f44_0276(g_3858, 0, 1);
    FUN_1f44_0276(g_3858, 0, 2);
    FUN_1f44_0276(g_3858, 0, 3);
}

void FUN_13b2_3cb6(int a) { }
void FUN_13b2_3d7c(void) { }
void FUN_13b2_3fb4(void) { }
void FUN_13b2_4134(void) { }
