/* @target FUN_13b2_2c80 */
/* @name   m_2c80 */
/* @proto  void m_2c80(void) */
/* @module same */
/* @extra  g13b2.c xmod/x1f4429e.c xmod/xd1914a.c xmod/xd1932f.c xmod/xd192fe.c xmod/x1e25b0.c xmod/x161d38e.c xmod/xb1134c.c xmod/xb1100b5.c xmod/xb11462.c xmod/x1028ef.c xmod/x1e2555.c xmod/x1e254a.c xmod/xf4404b3.c xmod/x0adb1da.c xmod/xb111597.c xmod/xb111a47.c xmod/xb111106.c */

#include <string.h>

extern unsigned char g_2c60, g_2800, g_27ec, g_2c61, g_27fc, g_18f8, g_27ee;
extern unsigned char g_2805, g_27f3, g_27f7, g_27ed, g_349f;
extern char far *g_3858, *g_34a1, *g_27aa, *g_34ad, *g_34a9;
extern char g_2c56[0x20];
extern char g_2c33[7];

unsigned long g_2bfimer;    /* aliases the g_2bff/g_2c01 counter for code shape */

void FUN_1f44_029e(char far *p, char v);
char far *FUN_0d19_014a(char far *p, char far *s, int v);
char far *FUN_0d19_032f(char far *p, int a, int b, int c);
char far *FUN_0d19_02fe(char far *p, int a, int b);
void FUN_1e25_0b0f(char far *p);
void FUN_161d_038e(void);
void FUN_1b11_034c(int a, int b, char far *s, int c, int d, int e, int f);
void FUN_1b11_00b5(int a, int b);
void FUN_1b11_0462(int a, int b, char far *s);
char far *FUN_1000_28ef(int a, char far *b, int c);
void FUN_1e25_055f(char far *p, char far *s);
void FUN_1e25_04aa(char far *p, char far *s);
void FUN_1f44_04b3(char far *p);
void FUN_0adb_01da(void);
void FUN_1b11_1597(char far *p);
void FUN_1b11_1a47(char far *p);
void FUN_1b11_1106(int a, int b, char far *s);
void FUN_13b2_3153(int v);
void FUN_13b2_31b7(void);

/* Same-module helpers.  FUN_13b2_3153/31b7 live ABOVE m_2c80 in the segment,
   so they are prototype-only here and defined after m_2c80, which makes BCC
   emit the aligning nop before its `push cs`. */
void FUN_13b2_154f(int v) { (void)v; }
void FUN_13b2_0e2f(void) { }
void FUN_13b2_1347(void) { }
void FUN_13b2_1a40(void) { }

void m_2c80(void)
{
    int i;
    int i_step;

    FUN_13b2_154f(1);
    FUN_1f44_029e(g_3858, g_349f);
    FUN_0d19_014a(g_34a1, "e1", 0);
    FUN_0d19_032f(g_34a1, 0, 0, 0);
    FUN_0d19_02fe(g_34a1, 1, 0xf);
    FUN_1e25_0b0f(g_27aa);
    g_2800 = 0;
    FUN_13b2_0e2f();
    FUN_13b2_1347();
    if (g_2c60 == 0) {
        if (g_27ec % 2)
            FUN_1e25_04aa(g_27aa, "e7f");
        else
            FUN_1e25_04aa(g_27aa, "e87");
        FUN_13b2_3153(0x78);
        if (g_27ec == 6 && g_2c61 != 0) {
            g_27fc = 1;
            FUN_1b11_034c(0x3c, 0x3c, "e8d", 0x60, 0x1f, 1, 0x63);
            FUN_1b11_034c(0x3c, 0x46, "ea7", 0x60, 0x1f, 1, 0x63);
            FUN_1b11_00b5(0xf0, 0);
        } else {
            i = 1;
            i_step = 0x3e8;
            while (g_27ee >= i) {
                FUN_1b11_0462(0x64, 0x3c, "ebc");
                strcpy(g_2c33, "ecbstr");
                strcat(g_2c33, FUN_1000_28ef(0x3e8, g_2c56, 0xa));
                strcat(g_2c33, "ed2");
                strcat(g_2c33, FUN_1000_28ef(i, g_2c56, 0xa));
                FUN_1b11_034c(0x64, 0x3c, g_2c33, 0x70, 0x1f, 1, 0x63);
                g_2bfimer += (long)i_step;
                FUN_13b2_154f(1);
                if (g_18f8)
                    FUN_1e25_055f(g_27aa, "ed6");
                else
                    FUN_1b11_00b5(0x1e, 0);
                FUN_1b11_00b5(0xa, 0);
                i_step += 0x3e8;
                i++;
            }
            if (g_27ee == 0) {
                FUN_1b11_034c(0x82, 0x3c, "edc", 0x70, 0x1f, 1, 0x63);
                FUN_1e25_055f(g_27aa, "ee7");
            }
            FUN_1b11_00b5(0x5a, 0);
        }
    }
    if (!g_2805) {
        if (!g_2c60)
            FUN_1f44_04b3(g_3858);
    } else {
        g_27f3 = 1;
        FUN_0adb_01da();
    }
    *g_3858 = 0;
    FUN_1b11_1597(g_34ad);
    FUN_1b11_1a47(g_34a9);
    FUN_13b2_1a40();
    g_27ec++;
    if (g_27f7) {
        if (++g_27ed == 3) {
            g_27fc = 1;
            FUN_13b2_31b7();
        }
        g_27ec = 0;
    }
}

void FUN_13b2_3153(int v) { (void)v; }
void FUN_13b2_31b7(void) { }