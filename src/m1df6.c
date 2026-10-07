/* @target FUN_13b2_1df6 */
/* @name   m_1df6 */
/* @proto  void m_1df6(void) */
/* @module same */
/* @extra  g13b2.c xmod/x1000ef8.c xmod/x1000c92.c xmod/x1000c91.c xmod/x1e2500a.c xmod/x0f92084.c xmod/x0f9206.c xmod/x161d38e.c xmod/x0df000e.c xmod/xb11195d.c xmod/xb111566.c xmod/xb1115d6.c xmod/xb110056.c xmod/x0adb324.c xmod/x1f4406.c xmod/x1e253c6.c xmod/x1f4429e.c xmod/xb1100b5.c xmod/xb111106.c xmod/xb1111cc.c xmod/xf02bf.c xmod/xb111653.c */

extern unsigned char g_2c60, g_2804, g_2807, g_2c61, g_1690, g_18f8;
extern char far *g_27aa, *g_34a9, *g_34ad, *g_2c2f, *g_3858, *g_27ae;
extern unsigned int g_2bfb, g_2bfd;

void FUN_1000_0ef8(void);
int FUN_1000_2c09_2(char far *a, char far *b);
int FUN_1000_2c09_1(char far *a);
char far *FUN_1e25_000a(int a, int b, char far *s);
void FUN_0f92_0084(void);
int FUN_0f92_0006(int a, int b);
void FUN_161d_038e(void);
void FUN_0df0_000e(void);
char far *FUN_1b11_195d(int a, int b);
char far *FUN_1b11_1566(int a, int b);
char far *FUN_1b11_15d6(char far *p, char far *s);
int FUN_1b11_0056(char far *s);
void FUN_0adb_0324(void);
char far *FUN_1f44_0006(int a, int b);
int FUN_1e25_03c6(char far *a, char far *b);
int FUN_1f44_029e(char far *p, int a);
int FUN_1b11_00b5(int a, int b);
char far *FUN_1b11_1106(int a, int b, char far *s);
void FUN_1b11_11cc(char far *p, int v);
int FUN_1000_02bf(char far *p);
int FUN_1b11_1653(char far *a, char far *b, int c);

/* Same-module helper FUN_13b2_1a81 (flat 0x55A1); stand-in body. */
void FUN_13b2_1a81(void) { }

void m_1df6(void)
{
    FUN_1000_0ef8();
    if (g_2c60 == 1) {
        FUN_1000_2c09_2("x", "y");
        FUN_1000_2c09_1("z");
    } else {
        FUN_1000_2c09_1("w");
    }
    g_27aa = FUN_1e25_000a(0, 0, "s5");
    FUN_13b2_1a81();
    g_2804 = 0;
    g_2807 = 0;
    g_2c61 = 0;
    FUN_0f92_0084();
    FUN_161d_038e();
    FUN_0f92_0006(0x1f91, 0x3b2);
    g_1690 = 0;
    g_18f8 = 1;
    FUN_0df0_000e();
    g_34a9 = FUN_1b11_195d(0, 0);
    g_34ad = FUN_1b11_1566(0, 0);
    FUN_1b11_1653(g_34ad, "s6", 0);
    g_2c2f = FUN_1b11_15d6(g_34ad, "s7");
    g_2bfb = *(unsigned int far *)(*(char far * far *)(g_2c2f + 6) + 4);
    g_2bfd = *(unsigned int far *)(*(char far * far *)(g_2c2f + 6) + 6);
    if (g_34ad == 0)
        FUN_1b11_0056("s8");
    FUN_0adb_0324();
    g_3858 = FUN_1f44_0006(0, 0);
    if (g_2c60 == 0) {
        FUN_1e25_03c6(g_27aa, "s9");
        FUN_1f44_029e(g_3858, 1);
        g_27ae = FUN_1b11_1106(0, 0, "sA");
        FUN_1b11_11cc(g_27ae, 1);
        FUN_1000_02bf(g_27ae);
        FUN_1f44_029e(g_3858, 0);
        FUN_1b11_00b5(0xf0, 0);
    }
}