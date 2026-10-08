/* Same-module callees of main(), one per target function, all in one Borland
 * module so the cheap `nop / push cs / call rel16` form is preserved.
 *
 * Why they live here rather than at the bottom of main.c: `call rel16` only
 * reaches +-32K of the call site, and growing any one of these in place shifts
 * the displacement of every call after it, so main's own bytes move and the
 * frame loop can never be un-stubbed.  Splitting the module lets each function
 * be reconstructed independently.  main() now calls them with `lcall` instead,
 * which is also 5 bytes -- main keeps its 300-byte size but stops being
 * byte-exact, since the opcode differs.  That is the deliberate trade: main's
 * loop only exits when g_27fc is set, and the only code that sets it is behind
 * m_013b, one of these.
 *
 * Within this file the calls stay same-module, so the rel16 form is what the
 * target uses.
 *
 * Four of these are real now: m_1a40, m_1df6, m_24e7, m_013b and m_2c80.  They
 * were reconstructed one function at a time against PETE.EXE with
 * `check.py diff` (each still has its own standalone `src/<name>.c` used for
 * byte-exact verification); this file is the linked-together copy the game
 * actually runs.  m_013b calls m_2c80, and m_2c80 is the only writer of g_27fc,
 * so together they are what lets main()'s frame loop terminate.
 */

/* m_1a40's own far callees */
void node_free(char far *p);
void buf_to_vga(unsigned char far *p);
void pal_upload(char far *a, char far *b);

struct blk_s { unsigned char pad0[2]; unsigned char state; unsigned char pad1[3];
              unsigned char data[0x300]; };
void statebak(struct blk_s far *blk);
void clrbit3(unsigned char far *rec);

/* Globals main.c owns.  Addresses differ from the target's, but diffasm masks
   every absolute DS reference, so only the sharing has to be right. */
extern char far *g_27aa;
extern char far *g_34a9;
extern char far *g_34ad;
extern char far *g_3858;
extern char far *g_34a1;
extern char far *g_27ae;
extern char far *g_27e2;
extern char far *g_19e6;
extern char far *g_2c2f;
extern unsigned char g_27fc, g_27f3, g_27fd, g_27ff, g_2800;
extern unsigned char g_27ec, g_27ed, g_27f2, g_27f6, g_27f9, g_27fa;
extern unsigned char g_2c61, g_2c60;
extern unsigned int  g_2bf5;
extern unsigned int  g_19e4;
extern unsigned int  g_2bfb, g_2bfd;
extern unsigned char g_2805, g_27f7, g_349f, g_349e;
extern unsigned char g_27f5, g_27f8, g_2801, g_2802;
extern unsigned char g_27e6, g_27e7, g_27ef, g_27fb, g_2803;
extern unsigned char g_2804, g_2807;
extern unsigned char g_27ee;
extern unsigned char g_1690, g_18f8, g_18fa;
extern unsigned char g_2c33[7];
extern unsigned char g_2c56[0x20];

/* The shared dispatch table lives in src/g13b2.c. */
struct dispatch_entry {
    char far *p_73a, *p_73e, *p_742, *p_746, *p_74a;
    void (far *fn)(void);
    char far *sub[6];
    unsigned char pad[4];
};
extern struct dispatch_entry g_dispatch[];

/* Record layouts used by m_24e7. */
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
/* m_013b's bitfield record over g_34a1. */
struct bfi65 { unsigned char pad[0x65]; unsigned int : 2; unsigned int b2 : 1; };

#include <string.h>

/* m_2c80's score accumulator; declared before the body that reads it. */
unsigned long g_2bfimer;

/* ------------------------------------------------------------------ far ----
 * Cross-module callees.  One declaration per target function; where two of the
 * reconstruction files disagreed on a prototype the return value was dropped or
 * widened to whatever every call site ignores, because in this linked build they
 * are all still stand-ins from src/xmod. */
void FUN_0adb_01da(void);
void FUN_0adb_01ff(void);
void FUN_0adb_0324(void);
int  FUN_0d19_095c(char far *p);
int  FUN_0d19_0104(char far *p, int a, int b, int c);
char far *FUN_0d19_014a(char far *p, char far *s, int v);
char far *FUN_0d19_032f(char far *p, int a, int b, int c);
char far *FUN_0d19_02fe(char far *p, int a, int b);
int  FUN_0e25_03c6(char far *p, char far *q);
unsigned char FUN_0e25_0978(char far *p, char far *lit);
void FUN_0e25_06fe(char far *p, char far *lit);
void FUN_0e25_0b0f(char far *p);
int  FUN_0f92_0006(int a, int b);
void FUN_0f92_0084(void);
int  FUN_0ffe_000e(int a, int b, int c, int d, int e, int f, char far *g,
                   int h, int i);
void FUN_1000_0ef8(void);
int  FUN_1000_2c09_2(char far *a, char far *b);
int  FUN_1000_2c09_1(char far *a);
char far *FUN_1000_28ef(int a, char far *b, int c);
int  FUN_1000_02bf(char far *p);
int  FUN_161d_08de(void);
void FUN_161d_038e(void);
void FUN_0df0_000e(void);
void FUN_1b11_00b5(int a, int b);
int  FUN_1b11_0056(char far *s);
char far *FUN_1b11_1106(int a, int b, char far *s);
void FUN_1b11_11cc(char far *p, int v);
void FUN_1b11_034c(int a, int b, char far *s, int c, int d, int e, int f);
void FUN_1b11_0462(int a, int b, char far *s);
int  FUN_1b11_1653(char far *a, char far *b, int c);
char far *FUN_1b11_15d6(char far *p, char far *s);
void FUN_1b11_1597(char far *p);
void FUN_1b11_1a47(char far *p);
char far *FUN_1b11_195d(int a, int b);
char far *FUN_1b11_1566(int a, int b);
int  FUN_1b11_19d2(char far *p);
char far *FUN_1b11_1dc1(char far *p, int a, int b, int c, int d, int e);
char far *FUN_1e25_000a(int a, int b, char far *s);
int  FUN_1e25_03c6(char far *a, char far *b);
void FUN_1e25_04aa(char far *p, char far *s);
void FUN_1e25_055f(char far *p, char far *s);
void FUN_1e25_0b0f(char far *p);
char far *FUN_1f44_0006(int a, int b);
void FUN_1f44_0188(char far *p, char a, char b, char c);
int  FUN_1f44_00fe(char far *p);
void FUN_1f44_0276(char far *p, int a, int b);
void FUN_1f44_029e(char far *p, char v);
void FUN_1f44_03af(char far *p, int a, int b);
int  FUN_1f44_0426(char far *p);
int  FUN_1f44_04a2(char far *p);
int  FUN_1f44_03f1(char far *p);
void FUN_1f44_04b3(char far *p);

/* The four reconstructed bodies, in call order. */
void m_1df6(void);
void m_24e7(char a);
void m_013b(void);
void m_2c80(void);

/* Same-module callees of main() that are still stubs. */
void m_1a40(void);
void m_38a2(void);
void m_2415(void);
void m_0dd6(void);
void m_154f(int v);
void m_34e5(void);
void m_35dd(void);

/* Same-module callees of the four reconstructed bodies, still stubs. */
void m_07f7(void);
void m_198d(void);
void m_202f(void);
void m_23f8(void);
int  m_4eab(int a);
void m_0252(void);
void m_02b9(void);
void m_1393(int a);
void m_2f97(void);
void m_3153(int a);
void FUN_13b2_154f(int v);
void FUN_13b2_0e2f(void);
void FUN_13b2_1347(void);
void FUN_13b2_1a40(void);
void FUN_13b2_3153(int v);
void FUN_13b2_31b7(void);
void FUN_13b2_1a81(void);

/* ----------------------------------------------------------------- bodies --
 */

/* FUN_13b2_1a40, 65 bytes / 17 instructions.  Reconstructed and byte-exact --
 * it is a pure composition of the real far callees. */
void m_1a40(void)
{
    node_free(g_34a9);
    statebak(g_3858);
    clrbit3(g_3858);
    buf_to_vga(g_27aa);
}

/* FUN_13b2_1df6, 411 B / 127 ins.  Initialises the picture buffer, node list
 * and frame record main() and m_24e7 both read. */
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

/* FUN_13b2_24e7, 1945 B / 676 ins.  Byte-exact when built standalone as
 * src/m24e7.c; here it shares the module with the other four so the near calls
 * to m_07f7 / m_198d / m_202f / m_23f8 stay rel16. */
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

/* FUN_13b2_013b, 279 B / 104 ins.  Dispatches the frame; its call to m_2c80 is
 * what eventually sets g_27fc and lets main() leave the frame loop. */
void m_013b(void)
{
    if (g_27ff) {
        if (g_2bf5 > 0x96) {
            if (!FUN_0e25_0978(g_27aa, "8c4"))
                FUN_0e25_06fe(g_27aa, "8cc");
            m_0252();
        }
        if (g_2bf5++ == 0xff) {
            g_27ff = 0;
            m_02b9();
        }
    }

    if (((struct bfi65 far *)g_34a1)->b2 && g_2800) {
        if (!g_27f2 && !g_27f6 && !g_2c61) {
            m_2f97();
            m_24e7(0);
        } else {
            g_34a1[0x65] &= (unsigned char)0xfb;
        }
    }

    g_dispatch[g_27ed].fn();

    if (g_27f9 || !g_27fa) {
        if (g_34a1[0x32] != 2) {
            g_2800 = 0;
            m_1393(5);
            FUN_0e25_0b0f(g_27aa);
            m_3153(0x1e);
        }
        m_2c80();
        if (g_27fc) {
            g_27f3 = 1;
            return;
        }
        m_24e7(1);
    }
    return;
}

/* FUN_13b2_2c80.  Sets g_27fc on two paths (g_27ec==6&&g_2c61, and when
 * g_27ed wraps to 3), which is the only way main()'s frame loop ever exits. */
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

/* ----------------------------------------------------------------- stubs ---
 * Same-module helpers that are not yet reconstructed.  In the linked build they
 * are genuine no-ops; the byte-exact versions live in their own src/*.c and are
 * verified with check.py, not linked here (they would collide with these names).
 */
void m_38a2(void) { }              /* 1030 B / 219 ins */
void m_2415(void) { }              /* 210 B / 59 ins; clears g_27fc at 0x5F62 */
void m_0dd6(void) { }              /* 821 B / 280 ins */
void m_154f(int v) { (void)v; }    /* 619 B / 215 ins */

/* m_34e5 and m_35dd are only reachable from main's `tail:` label, which the
   frame loop falls out of exactly when g_27fc (or g_27f3, set from g_27fc) is
   set.  main() itself never returns -- it is an unconditional `for(;;)` -- so
   this is the one place the log can report that the frame loop was left, and
   run_game.py's timeout can therefore not be mistaken for a boot that never
   happened.  Reporting it once keeps the line from repeating every spin. */
void emit(char *line);

void m_34e5(void)
{
    static int logged;
    if (!logged) { logged = 1; emit("[boot] frame loop exited; g_27fc is set"); }
}
void m_35dd(void) { }              /* 709 B / 273 ins */

void m_07f7(void) { }
void m_198d(void) { }
void m_202f(void) { }
void m_23f8(void) { }
int  m_4eab(int a) { (void)a; return 0; }

void m_0252(void) { }
void m_02b9(void) { }
void m_1393(int a) { (void)a; }
void m_2f97(void) { }
void m_3153(int a) { (void)a; }

void FUN_13b2_154f(int v) { (void)v; }
void FUN_13b2_0e2f(void) { }
void FUN_13b2_1347(void) { }
void FUN_13b2_1a40(void) { }
void FUN_13b2_3153(int v) { (void)v; }
void FUN_13b2_31b7(void) { }
void FUN_13b2_1a81(void) { }
