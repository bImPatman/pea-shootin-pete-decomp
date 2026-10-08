/* @target FUN_13b2_24e7 */
/* @name m_24e7 */
/* @proto void __far m_24e7(char a) */
/* @module same */
/* @extra g13b2.c xmod/x0adb1da.c xmod/x0adb1ff.c xmod/xf440426.c xmod/xe253c6.c xmod/xb111653.c xmod/xf0276.c xmod/x1f4429e.c xmod/xb111106.c xmod/xb1111cc.c xmod/xf02bf.c xmod/x0ffe0e.c xmod/xf4403af.c xmod/xb111dc1.c xmod/xd1995c.c xmod/xd19104.c xmod/xb1119d2.c xmod/xf440188.c xmod/xf4400fe.c xmod/x1028ef.c xmod/xf4404a2.c xmod/xf4403f1.c xmod/xb1100b5.c xmod/xe161d.c xmod/xb1134c.c xmod/xb11462.c */
#include <string.h>

extern unsigned char g_2800, g_27f3, g_2805, g_27ec, g_27ed, g_27f7;
extern unsigned char g_27f5, g_27f8, g_2802, g_2801, g_2c61, g_2c60, g_18fa;
extern unsigned char g_27f9, g_27e6, g_27e7, g_27ef, g_27f6, g_27fa, g_27fb;
extern unsigned char g_2803, g_27ff, g_349e;
extern char far *g_27e2;
extern char far *g_3858;
extern char far *g_27aa;
extern char far *g_34ad;
extern char far *g_34a9;
extern char far *g_34a1;
extern char far *g_27ae;
extern char far *g_19e6;
extern unsigned char g_2c33[7];
extern unsigned char g_2c56[0x20];

struct dispatch_entry {
    char far *p_73a, *p_73e, *p_742, *p_746, *p_74a;
    void (far *fn)(void);
    char far *sub[6];
    unsigned char pad[4];
};
extern struct dispatch_entry g_dispatch[];

struct obj742 {
    char far *f0, *f4, *f8, *fc, *f10, *f14;
    unsigned char pad[0x1a];
    void (far *fn32)(void);
};

struct obj_e2 {
    unsigned char b0f[0xf];
    char far *f0f;
    char far *f13;
    unsigned char b17;
    void (far *fn18)(void);
};

struct vtbl_a1;
typedef void (far *fn_vt)(char far *, int, int, int, char far *, int, int);
struct vtbl_a1 {
    fn_vt f0;
    fn_vt f4;
};
struct obj_a1 {
    struct vtbl_a1 near *vt;
    unsigned char pad2[4];
    int x6;
    unsigned char pad7[0x32 - 0x8];
    unsigned char b32;
};

void FUN_0adb_01da(void);
void FUN_0adb_01ff(void);
int FUN_1f44_0426(char far *p);
int FUN_0e25_03c6(char far *p, char far *q);
int FUN_1b11_1653(char far *a, char far *b, int c);
int FUN_1f44_0276(char far *p, int a, int b);
int FUN_1f44_029e(char far *p, char a);
char far *FUN_1b11_1106(int a, int b, char far *s);
int FUN_1b11_11cc(char far *p, int v);
int FUN_1000_02bf(char far *p);
void FUN_0ffe_000e(int a, int b, int c, int d, int e, int f, char far *g,
                   int h, int i);
int FUN_1f44_03af(char far *p, int a, int b);
char far *FUN_1b11_1dc1(char far *p, int a, int b, int c, int d, int e);
int FUN_0d19_095c(char far *p);
int FUN_0d19_0104(char far *p, int a, int b, int c);
int FUN_1b11_19d2(char far *p);
int FUN_1f44_0188(char far *p, char a, char b, char c);
int FUN_1f44_00fe(char far *p);
char far *FUN_1000_28ef(int a, char far *b, int c);
int FUN_1f44_04a2(char far *p);
int FUN_1f44_03f1(char far *p);
int FUN_1b11_00b5(int a, int b);
int FUN_161d_08de(void);
void FUN_1b11_034c(int a, int b, char far *s, int c, int d, int e, int f);
void FUN_1b11_0462(int a, int b, char far *s);

void m_07f7(void);
void m_154f(int a);
void m_198d(void);
void m_202f(void);
void m_23f8(void);
int m_4eab(int a);

void m_07f7(void) { }
void m_154f(int a) { }
void m_198d(void) { }
void m_202f(void) { }
void m_23f8(void) { }

void m_24e7(char a)
{
    struct obj742 far *obj;

    g_2800 = 0;
    if (g_27f3)
        return;

    FUN_0adb_01da();
    if (g_2805) {
        g_27ec = m_4eab(2) + m_4eab(2) + m_4eab(2);
        g_27ed = 0;
        FUN_0adb_01ff();
    }

    FUN_1f44_0426(g_3858);
    g_3858[2] = 0;
    g_27ff = 0;
    g_27f9 = 0;
    g_27e6 = 0;
    g_27e7 = 0;
    g_27ef = 0;
    g_27f6 = 0;
    g_27fa = 0;
    g_27fb = 0;
    g_2803 = 0;

    FUN_0e25_03c6(g_27aa, g_dispatch[g_27ed].p_74a);

    {
        if (g_27ec % 7 == 0 && g_27ec != 0) {
            g_27f7 = 1;
            obj = (struct obj742 far *)g_dispatch[g_27ed].p_742;
            if (a) {
                g_2801 = g_2802 = 0;
                m_202f();
                m_198d();
                FUN_1b11_1653(g_34ad, obj->f0, 1);
                FUN_1b11_1653(g_34ad, obj->f4, 1);
                FUN_1b11_1653(g_34ad, obj->f8, 1);
                FUN_1b11_1653(g_34ad, obj->fc, 1);
                FUN_1b11_1653(g_34ad, obj->f10, 1);
                FUN_1b11_1653(g_34ad, obj->f14, 1);
            }
            FUN_1f44_0276(g_3858, 0, 0);
            FUN_1f44_0276(g_3858, 0, 1);
            FUN_1f44_029e(g_3858, 0);
            g_27ae = FUN_1b11_1106(0, 0, g_dispatch[g_27ed].p_746);
            g_27ae[0xa] = 1;
            FUN_1b11_11cc(g_27ae, 0);
            FUN_1000_02bf(g_27ae);
            FUN_0ffe_000e(0, 0, 0x140, 0x9b, 0, 0, g_19e6, 0x140, 0x140);
        } else {
            g_27f7 = 0;
            g_27e2 = g_dispatch[g_27ed].sub[g_27ec];
            g_27f5 = ((struct obj_e2 far *)g_27e2)->b17;
            if (g_27f5 <= 0x96) {
                g_27f8 = 2;
            } else {
                g_27f8 = 1;
            }
            if (a) {
                g_2801 = g_2802 = 0;
                m_23f8();
                FUN_1b11_1653(g_34ad, ((struct obj_e2 far *)g_27e2)->f0f, 1);
                FUN_1b11_1653(g_34ad, ((struct obj_e2 far *)g_27e2)->f0f + 0xf, 1);
                FUN_1b11_1653(g_34ad, ((struct obj_e2 far *)g_27e2)->f0f + 0x1e, 1);
                FUN_1b11_1653(g_34ad, ((struct obj_e2 far *)g_27e2)->f0f + 0x2d, 1);
                m_198d();
            }
            if (strcmp(g_dispatch[g_27ed].sub[g_27ec - 1],
                       g_dispatch[g_27ed].sub[g_27ec]) != 0) {
                FUN_1f44_0276(g_3858, 0, 0);
                FUN_1f44_0276(g_3858, 0, 1);
                FUN_1f44_029e(g_3858, 0);
                g_27ae = FUN_1b11_1106(0, 0, g_27e2);
                g_27ae[0xa] = 1;
                FUN_1b11_11cc(g_27ae, 0);
                FUN_1000_02bf(g_27ae);
FUN_0ffe_000e(0, 0, 0x140, 0x9b, 0, 0, g_19e6, 0x140, 0x140);
            }
            ((struct obj_e2 far *)g_27e2)->fn18();
        }
    }

    if (g_2c61)
        FUN_1b11_034c(0x5a, 0xa, "ABCDEFG", 0x60, 0x1f, 1, 2);
    FUN_1f44_03af(g_3858, 2, 0);
    FUN_1f44_03af(g_3858, 2, 1);
    m_154f(1);
    g_34a1 = FUN_1b11_1dc1(g_34a9, 0xe4, 0x8a2, 0x154, 0x8a2, 0);
    ((struct obj_a1 far *)g_34a1)->vt->f4(g_34a1, 0, 0, 2, "zzzzzzzz", 2, 2);
    FUN_0d19_095c(g_34a1);
    FUN_0d19_0104(g_34a1, 0x140, 0x136 - ((struct obj_a1 far *)g_34a1)->x6, 0);
    ((struct obj_a1 far *)g_34a1)->b32 = 2;
    if (g_27f7)
        obj->fn32();
    m_07f7();
    FUN_1b11_19d2(g_34a9);
    FUN_1f44_029e(g_3858, g_349e);

    FUN_1b11_034c(0xa0 - strlen(g_dispatch[g_27ed].p_73a) * 9 / 2,
                  0x3c, g_dispatch[g_27ed].p_73a, 0x70, 0x1f, 1, 0x63);

    if (!g_27f7) {
        if (((struct obj_e2 far *)g_27e2)->f13[0]) {
            FUN_1f44_0188(g_3858,
                          ((struct obj_e2 far *)g_27e2)->f13[0],
                          ((struct obj_e2 far *)g_27e2)->f13[1],
                          ((struct obj_e2 far *)g_27e2)->f13[2]);
            FUN_1f44_00fe(g_3858);
        }
        memcpy(g_2c33, "HIJKLMN", 7);
        strcat(g_2c33, FUN_1000_28ef(g_27ec + 1, g_2c56, 0xa));
    } else {
        strcpy(g_2c33, g_dispatch[g_27ed].p_73e);
    }

    FUN_1b11_034c(0xa0 - strlen(g_2c33) * 9 / 2,
                  0x46, g_2c33, 0x10, 0x1f, 1, 0x63);
    FUN_1b11_034c(0xa0 - strlen("qrstuvwxyz") * 9 / 2,
                  0x50, "qrstuvwxyz", 0x10, 0x1f, 1, 0x63);

    if (!g_2c60) {
        FUN_1f44_04a2(g_3858);
    } else {
        FUN_1f44_03f1(g_3858);
    }
    FUN_1b11_00b5(0x14, 1);
    if (g_18fa)
        while (FUN_161d_08de());

    FUN_1b11_0462(0xa0 - strlen(g_dispatch[g_27ed].p_73a) * 9 / 2,
                  0x3c, g_dispatch[g_27ed].p_73a);
    FUN_1b11_0462(0xa0 - strlen(g_2c33) * 9 / 2, 0x46, g_2c33);
    FUN_1b11_0462(0xa0 - strlen("bcdefghijk") * 9 / 2, 0x50, "bcdefghijk");

    g_2800 = 1;
}

int m_4eab(int a) { return 0; }