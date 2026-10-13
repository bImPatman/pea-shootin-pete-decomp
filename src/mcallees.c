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
 * Real now: m_1a40, m_1df6, m_24e7, m_013b, m_2c80, m_0dd6, m_34e5, m_b11a,
 * m_154f, m_2415, m_38a2 and m_35dd.  They were reconstructed one function at a
 * time against PETE.EXE with `check.py diff` (each still has its own standalone
 * `src/<name>.c` used for byte-exact verification); this file is the
 * linked-together copy the game actually runs.  m_013b calls m_2c80, and m_2c80
 * is the only writer of g_27fc, so together they are what lets main()'s frame
 * loop terminate.  m_38a2 differs from the target in exactly one instruction
 * (`dec al` vs `add al,0xff`; see notes/compiler.md).
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
extern unsigned int  g_19e4[];
extern unsigned int  g_19e8;
extern int           g_2bf7, g_2bf9, g_2bfb, g_2bfd, g_19ea;
extern unsigned char g_2805, g_27f7, g_349f, g_349e;
extern unsigned char g_2808[];
extern unsigned char g_27f5, g_27f8, g_2801, g_2802;
extern unsigned char g_27e6, g_27e7, g_27ef, g_27fb, g_2803;
extern unsigned char g_2804, g_2807;
extern unsigned char g_27ee;
/* m_2415's header writes (storage lives in src/g13b2.c). */
extern unsigned int  g_168c, g_2c01, g_2bff;
extern unsigned char g_2bf0, g_27f0, g_27f1, g_27f4, g_27fe;
extern unsigned char g_1690, g_18f8;
/* m_e2500a stores a word into g_18fa (FUN_1e25_000a's target does too) even
   though m_2c80 / m_24e7 / m_0dd6 read it as a char; one word of storage, low
   byte is the flag.  Storage lives in src/g13b2.c. */
extern unsigned int  g_18fa;
/* m_e2500a's own header words and the byte buffers it hands to far helpers.
   Storage is defined at the bottom of this file; nothing else references them. */
unsigned int  g_354a, g_3548, g_354e, g_354c;
unsigned char g_18fc, g_1919, g_1932, g_19ec;
/* m_0dd6's input/stage state machine. */
extern unsigned char g_27ea, g_27eb, g_34a0, g_7d6;
extern unsigned char g_386d, g_386e, g_386f, g_3870, g_3871, g_3872, g_3873;
extern unsigned char g_3874, g_3875, g_3876, g_3877;
extern unsigned char g_3888, g_3889, g_388b, g_388c, g_3898, g_389a;
extern unsigned char g_38a4, g_38a5, g_38b4, g_38b7, g_38b9, g_38bc;
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
int  FUN_0ffe_000e();       /* 10 stack words; m_24e7 spells arg 7 as a far
                               pointer, m_b11a as two ints -- same pushed bytes */
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
char far *FUN_1e25_000a(char far *p, char far *s);
int  FUN_1e25_03c6(char far *a, char far *b);
void FUN_1e25_04aa(char far *p, char far *s);
void FUN_1e25_055f(char far *p, char far *s);
void FUN_1e25_0b0f(char far *p);
char far *FUN_1f44_0006(char far *p);
void FUN_1f44_00b7(char far *p, char far *s);   /* same-module: body below */

/* m_e2500a's / m_f4406's far callees that the lists above do not already
   supply (their bodies now live in this file).  FUN_1000_0778, FUN_1b11_0056
   and FUN_1000_2c09_1 are declared/linked for m_1df6 already. */
char far *FUN_1000_0778(int n);
void      FUN_1000_19ee(int n, char far *a, char far *b);
void      FUN_161d_0840(void);
int       FUN_161d_0820(char far *p);
int       FUN_161d_0401(void);
int       FUN_161d_0456(void);
void      FUN_161d_0a58(int v);
void      FUN_161d_024a(char far *p);
void      FUN_161d_0363(int a, int b);
void      FUN_161d_0338(int a, int b);
void      FUN_1fd3_0178(void);
void      FUN_1fd3_01e1(void);
void      FUN_1fd3_0150(int v);
void FUN_1f44_0188(char far *p, unsigned char a, unsigned char b, unsigned char c);
int  FUN_1f44_00fe(char far *p);
void FUN_1f44_0276(char far *p, int a, int b);
void FUN_1f44_029e(char far *p, char v);
void FUN_1f44_03af(char far *p, int a, int b);
int  FUN_1f44_0426(char far *p);
int  FUN_1f44_04a2(char far *p);
int  FUN_1f44_03f1(char far *p);
void FUN_1f44_04b3(char far *p);
int  FUN_1f44_035c(char far *p, char a, char b, char c);

/* m_38a2's other far callees (grep-confirmed: no other call site). */
unsigned char FUN_1e25_0426(char far *p);
char FUN_1fd3_0123(int a, int b, int c);

/* m_35dd's far callees that the declarations above do not already cover.  The
   first two parameters of FUN_0df0_025f are char in the target (so BCC emits
   `mov al, [bp-N]`); src/m35dd.c keeps that spelling to be byte-exact.  Here the
   spelling only has to link, so FUN_1b11_034c above stays all-int. */
char far *FUN_0df0_025f(char a, int b);
void FUN_1f44_0319(char far *p, int a, int b, int c);

/* m_0dd6's far callees (the body was integrated from src/m0dd6.c). */
unsigned char FUN_0adb_0001(unsigned char v);
unsigned char FUN_0adb_0224(unsigned char v);
void FUN_0adb_02bc(void);
int  FUN_1000_1ae1(void);
void FUN_1000_18e4(void);
void FUN_1b11_009a(void);
void FUN_1e25_03a4(char far *p);
void FUN_1e25_0a23(char far *p);
void FUN_1e25_0a89(char far *p);
int  FUN_161d_03c7(void);
int  FUN_161d_03e4(void);
int  FUN_161d_03a9(int v);

/* m_34e5's far callees. */
unsigned char FUN_1df0_0222(unsigned long v);
void          FUN_1df0_0181(unsigned char a, unsigned long b, char far *s,
                            unsigned char c);
char far     *FUN_1b11_0c4d(char far *p, char far *s, int n);
void          FUN_1b11_079e(char far *p, int flag);

/* m_b11a's far callees.  The two OROS-style draw helpers take a far pointer as
   one parameter (two pushed words); the prototype spelling is only the caller's
   view, the stack words are what matter. */
void FUN_1fd3_0012(int, int, int, int, int, int, char far *, int, int);
void FUN_1fc2_0006(int, int, int, int, int, int, char far *, int, int, int);
void FUN_1d19_068d(char far *p);
void FUN_1d19_073e(char far *p);

/* m_154f / FUN_13b2_17ba's far helpers and shared HUD-number state. */
char far *FUN_1000_2937(unsigned long v, char far *buf, int base);
struct g207 {
    int far *p;                 /* +0x00 */
    char     pad[0x0c];         /* +0x04 */
    int      w10;               /* +0x10 */
    int      w12;               /* +0x12 */
};
extern struct g207 far *g_2c07[];
extern unsigned long    g_num_cur, g_num_prev;
extern unsigned char    g_27e8, g_27e9;
void FUN_13b2_17ba(unsigned char v);

/* The four reconstructed bodies, in call order. */
void m_1df6(void);
void m_24e7(char a);
void m_013b(void);
void m_2c80(void);

/* Same-module callees of main(); all are now reconstructed. */
void m_1a40(void);
void m_38a2(void);
void m_2415(void);
void m_0dd6(void);
void m_154f(unsigned char v);
void m_34e5(void);
void m_35dd(void);
int  m_b11a(char far *p);       /* reconstructed below */

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
void FUN_13b2_1393(int v);
void FUN_13b2_11ac(void);
void FUN_13b2_110b(void);
/* m_38a2's same-module helpers, still stubs. */
void FUN_13b2_1f91(void);
void FUN_13b2_3cb6(int a);
void FUN_13b2_3d7c(void);
void FUN_13b2_3fb4(void);
void FUN_13b2_4134(void);

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
    g_27aa = FUN_1e25_000a(0, "s5");
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
    g_3858 = FUN_1f44_0006(0);
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

/* FUN_1b11_1a94, 813 B / 261 ins.  Byte-exact in src/mb11a.c (shape+exact
 * 261/261); this is the linked copy.  main() calls it once per frame with
 * g_34a9.  The list head is a one byte count at +0 followed by four byte far
 * pointers; the object flags at +0x65/+0x66 are read through an int-based
 * bitfield overlay (BCC emits `shr ax,N / and ax,1 / or ax,ax` for those) and
 * written as plain bytes.  See src/mb11a.c for the full layout notes. */
struct mobj {
    char            pad0[0x13];                 /* +0x00 */
    int             w13;                        /* +0x13 */
    int             w15;                        /* +0x15 */
    int             w17;                        /* +0x17 */
    int             w19;                        /* +0x19 */
    int             w1b;                        /* +0x1b */
    int             w1d;                        /* +0x1d */
    int             w1f;                        /* +0x1f */
    int             w21;                        /* +0x21 */
    char            pad1[0x0a];                 /* +0x23 */
    unsigned char   b2d;                        /* +0x2d */
    char            pad2[2];                    /* +0x2e */
    unsigned char   b30;                        /* +0x30 */
    char            pad3[0x20];                 /* +0x31 */
    void       (far *fn51)(char far *);         /* +0x51 */
    char            pad4[0x10];                 /* +0x55 */
    unsigned char   b65;                        /* +0x65 */
    unsigned char   b66;                        /* +0x66 */
    char far * far *p67;                        /* +0x67 */
    char            pad5[8];                    /* +0x6b */
    char far * far *p73;                        /* +0x73 */
};

struct mbits {
    char            pad[0x65];                  /* +0x00 */
    unsigned        x0 : 5;                     /* +0x65 bits 0..4 */
    unsigned        b5 : 1;                     /* +0x65 bit 5 */
    unsigned        b6 : 1;                     /* +0x65 bit 6 */
    unsigned        x7 : 1;                     /* +0x65 bit 7 */
    unsigned        y0 : 5;                     /* +0x66 bits 0..4 */
    unsigned        c5 : 1;                     /* +0x66 bit 5 */
    unsigned        c6 : 1;                     /* +0x66 bit 6 */
    unsigned        y7 : 1;                     /* +0x66 bit 7 */
};

#define MB(o) ((struct mbits far *)(o))

struct mlist {
    unsigned char       n;                      /* +0x00 */
    char                pad;                    /* +0x01 */
    struct mobj far    *item[1];                /* +0x02 */
};

void FUN_1b11_1ed3(char far *p, char far *o);

int m_b11a(char far *pp)
{
    struct mlist far *p = (struct mlist far *)pp;
    int i;
    int a, b, d, c;

    for (i = 0; i < p->n; i++) {
        if (p->item[i]->fn51 && !MB(p->item[i])->b5)
            p->item[i]->fn51((char far *)p->item[i]);
        if (!MB(p->item[i])->c6) {
            a = p->item[i]->w13 >> 1;
            b = p->item[i]->w15 >> 1;
            c = p->item[i]->w19 >> 1;
            d = p->item[i]->w17 >> 1;
            FUN_0ffe_000e(a, b, a + c, b + d, a, b,
                          g_19e8, g_19e4[g_349f], 0x140, 0x140);
        }
        if (!MB(p->item[i])->c5)
            FUN_1d19_073e((char far *)p->item[i]);
        FUN_1d19_068d((char far *)p->item[i]);
    }

    for (i = 0; i < p->n; i++) {
        if (MB(p->item[i])->b5) {
            if (MB(p->item[i])->b6)
                FUN_1b11_1ed3((char far *)p, (char far *)p->item[i]);
            else
                p->item[i]->b65 |= 0x40;
        }
    }

    for (i = 0; i < p->n; i++) {
        if (!MB(p->item[i])->b5) {
            if (p->item[i]->b30) {
                FUN_1fd3_0012(0, 0, p->item[i]->w21, p->item[i]->w1f,
                              p->item[i]->w1b, p->item[i]->w1d,
                              p->item[i]->p73[p->item[i]->b2d],
                              g_19e4[g_349f], 0x140);
            } else {
                FUN_1fc2_0006(0, 0, p->item[i]->w21, p->item[i]->w1f,
                              p->item[i]->w1b, p->item[i]->w1d,
                              *((char far * far *)p->item[i]->p67[p->item[i]->b2d]),
                              g_19e4[g_349f], 0x140, 0x140);
            }
        }
    }
}

/* FUN_1b11_1ed3, 337 B.  Not reconstructed; reached only when an object has
   both flags at +0x65 set, which the zeroed run record never does. */
void FUN_1b11_1ed3(char far *p, char far *o) { (void)p; (void)o; }

/* FUN_13b2_38a2, 1030 B / 316 ins.  Reconstructed in src/m38a2.c; verified with
 * check.py at 1030/1030 size, shape+exact matching through instruction 238.  The
 * single residual difference is index 239, `add al,0xff` in the target vs the
 * `dec al` BCC 3.1 emits for `k - 1` (see notes/compiler.md, "8-bit +/-1 is
 * always dec/inc").  Title/menu screen: fills g_2808, runs the menu loop and
 * dispatches on the selected item. */
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

/* ----------- the two init allocators m_1df6 calls, reconstructed ---------- */

/* FUN_1e25_000a (src/m_e2500a.c, byte-exact standalone), 290 B / 86 ins.
 * Creates g_27aa's 0x10b-byte picture buffer and arms the 0x161d sound/channel
 * state through g_18fa. */
char far *FUN_1e25_000a(char far *p, char far *s)
{
    int i;

    if (p != 0 || (p = FUN_1000_0778(0x10b)) != 0) {
        p[0x109] = p[0x108] = 0;
        g_354a = 0;
        g_3548 = 0;
        g_354e = 0;
        g_354c = 0;
        *(int *)(p + 0x66) = 0;
        *(int *)(p + 0x64) = 0;

        FUN_161d_0840();

        if (s != 0) {
            if (FUN_161d_0820(s) == -1)
                FUN_1b11_0056((char far *)&g_18fc);
        }

        g_18fa = FUN_161d_0401();
        g_18fa = FUN_161d_0456();
        if (g_18fa != -1) {
            g_18fa = 1;
            FUN_1000_2c09_1((char far *)&g_1919);
            FUN_161d_0a58(1);
        } else {
            g_18fa = 0;
        }

        if (g_18fa != 0) {
            FUN_161d_024a((char far *)&g_1932);
            FUN_161d_0363(0xf, 0xf);
            FUN_161d_0338(0xf, 0xf);
        }

        p[0x10a] = (char)g_18fa;

        for (i = 0; i < 0x14; i++)
            p[0x50 + i] = 0;
    }
    return p;
}

/* FUN_1f44_0006 (src/m_f4406.c, byte-exact standalone), 125 B / 46 ins.
 * Creates g_3858's 0x306-byte record. */
char far *FUN_1f44_0006(char far *p)
{
    char local[0x10];

    if (p != 0 || (p = FUN_1000_0778(0x306)) != 0) {
        *(int *)local = 0x1a00;
        FUN_1000_19ee(0x10, (char far *)local, (char far *)local);

        if (local[2] != 8)
            FUN_1f44_00b7(p, (char far *)&g_19ec);

        FUN_1fd3_0178();
        FUN_1fd3_01e1();

        p[0] = p[2] = p[3] = 0;

        FUN_1fd3_0150(0);
    }
    return p;
}

/* FUN_1f44_00b7: the same-module helper FUN_1f44_0006 reaches with
 * `push cs; call rel16`.  Still a stand-in -- it registers p and returns. */
void FUN_1f44_00b7(char far *p, char far *s) { (void)p; (void)s; }

/* ----------------------------------------------------------------- stubs ---
 * Same-module helpers that are not yet reconstructed.  In the linked build they
 * are genuine no-ops; the byte-exact versions live in their own src/*.c and are
 * verified with check.py, not linked here (they would collide with these names).
 */
void FUN_13b2_1f91(void) { }
void FUN_13b2_3cb6(int a) { (void)a; }
void FUN_13b2_3d7c(void) { }
void FUN_13b2_3fb4(void) { }
void FUN_13b2_4134(void) { }

/* FUN_13b2_2018, 24 B.  Same-module helper called at m_2415's tail (the target
 * reaches it with `push cs; call rel16`); labels the "s" cel object with a far
 * string. */
void FUN_13b2_2018(void)
{
    FUN_1b11_1653(g_34ad, "x", 0);
}

/* FUN_13b2_2415, 210 B / 59 ins.  Byte-size exact in src/m2415.c (210/210, all
 * 59 instructions present); the only divergence is that the game compiler hoists
 * the char->int extension of g_27f3 one instruction earlier than BCC 3.1 can --
 * see notes/compiler.md, "Zext placed at the value's def".  Clears the score (the
 * words overlapping g_num_cur), the frame flags, builds the "s" object and draws
 * menu rows 0/1/3. */
void m_2415(void)
{
    g_168c = 0xBB80;
    g_349f = 0;
    g_27f2 = 0;
    g_27f3 = g_27f7 = 0;
    g_2c01 = 0;
    g_2bff = g_27f3;
    g_27ec = 0;
    g_27ed = 0;
    g_27fc = 0;
    g_2bf0 = 0;
    g_27ee = 2;
    g_27f0 = 0;
    g_27f1 = 1;
    g_27f4 = 6;
    g_27fd = 0;
    g_27fe = 0;

    g_27ae = FUN_1b11_1106(0, 0, "s");
    g_27ae[0x0a] = 2;
    FUN_1b11_11cc(g_27ae, 0);
    FUN_1000_02bf(g_27ae);
    FUN_1f44_0276(g_3858, 0, 0);
    FUN_1f44_0276(g_3858, 0, 1);
    FUN_1f44_0276(g_3858, 0, 3);
    FUN_13b2_2018();
}
/* FUN_13b2_0dd6, 821 B / 280 ins.  Byte-exact in src/m0dd6.c (shape+exact
 * 280/280); this is the linked copy.  Its same-module callees are below. */
void m_0dd6(void)
{
    int loc1;
    int loc2;

    FUN_0adb_0001(g_27ec);

    if (g_2805 && FUN_1000_1ae1())
        g_27f3++;

    if (g_386d) {
        if (!g_27f3) {
            FUN_13b2_1a40();
            g_27f3++;
        }
    }

    if (g_2c60 == 1) {
        if (g_386e)
            FUN_1b11_00b5(0x14, 0);
        if (g_386f)
            FUN_1b11_00b5(0x3c, 0);
        if (g_3898)
            g_27f9 = 1;
        if (g_389a) {
        if (g_18fa)
            FUN_1e25_0a23(g_27aa);
        else
            FUN_1e25_0a89(g_27aa);
            FUN_1000_18e4();
            FUN_1b11_009a();
            FUN_1b11_00b5(0x1e, 0);
        }
        if (g_3870) {
            FUN_1f44_029e(g_3858, 2);
            FUN_1000_18e4();
        }
        if (g_3871) {
            FUN_1f44_029e(g_3858, 3);
            FUN_1000_18e4();
        }
        if (g_3872)
            g_27f2 = 1;
        if (g_3873)
            g_27f2 = 0;
        if (g_3874)
            FUN_0adb_0224(g_27ec);
        if (g_3875)
            FUN_0adb_01ff();
        if (g_3876)
            FUN_0adb_02bc();
        if (g_3877)
            FUN_0adb_0324();
        if (g_388c) {
            if (!g_34a0)
                g_34a0++;
        }
    }

    if (g_388b) {
        FUN_1e25_03a4(g_27aa);
        if (g_18f8) {
            FUN_1b11_034c(0x64, 0x50, "<< Sound On >>", 0x10, 0x1f, 1, 0x63);
        } else {
            FUN_1b11_034c(0x64, 0x50, ">> Sound Off <<", 0x10, 0x1f, 1, 0x63);
            FUN_1e25_0b0f(g_27aa);
        }
        FUN_1b11_00b5(0x3c, 0);
        FUN_1b11_0462(0x64, 0x50, ">> Sound Off <<");
    }

    if (g_2800 == 0)
        goto end;
    if (g_7d6 == 0)
        goto stage2;
    if (!g_2805)
        goto input;
    goto stage2;

input:
    asm {
        cli
    }
    FUN_161d_038e();
    loc1 = FUN_161d_03c7();
    loc2 = FUN_161d_03e4();
    asm {
        sti
    }

    if (loc1 <= 0x1d)
        FUN_13b2_1393(1);
    else if (loc1 >= 0x63)
        FUN_13b2_1393(2);
    else if (loc2 >= 0x54)
        FUN_13b2_1393(4);
    else
        FUN_13b2_1393(5);

    if (FUN_161d_03a9(0)) {
        if (g_27ea) {
            g_27ea = 0;
            FUN_13b2_11ac();
        }
    } else {
        g_27ea = 1;
    }

    if (FUN_161d_03a9(1)) {
        if (g_27eb == 0)
            goto end;
        g_27eb = 0;
        FUN_13b2_110b();
        return;
    }
    g_27eb = 1;
    return;

stage2:
    if (g_38b7)
        FUN_13b2_1393(1);
    else if (g_38b9)
        FUN_13b2_1393(2);
    else if (g_38bc)
        FUN_13b2_1393(4);
    else
        FUN_13b2_1393(5);

    if (g_38a4 == 0 && g_3889 == 0 && g_38b4 == 0)
        goto l_then;
    if (g_27eb) {
        g_27eb = 0;
        FUN_13b2_110b();
    }
    goto l_after;
l_then:
    g_27eb = 1;
l_after:
    if (g_38a5) {
        if (g_27ea) {
            g_27ea = 0;
            FUN_13b2_11ac();
            return;
        }
        return;
    }
    g_27ea = 1;
    return;

end:
    return;
}
/* FUN_13b2_154f, 619 B / 215 ins.  Byte-exact in src/m154f.c (shape+exact
 * 215/215).  The HUD number-draw: formats the score (FUN_1000_2937), then
 * blits a background band and one 8px glyph per digit via FUN_0ffe_000e, then
 * tail-calls FUN_13b2_17ba through the same-module rel16 form. */
void m_154f(unsigned char v)
{
    unsigned char i, n, c, c2;
    int y;

    if (g_27e8++ < 0x32 && g_27e9 != 0 && !v)
        return;
    if (g_2c61)
        g_num_cur = 0;

    FUN_1000_2937(g_num_cur, (char far *)g_2c33, 0xa);
    n = strlen(g_2c33) - 1;
    y = 0xae;
    FUN_0ffe_000e(0xae - (n * 9 + 9), 0xa5, 0xae, 0xb0, 0xae - (n * 9 + 9), 0xa5,
                  g_19e8, g_19e4[g_349f], 0x140, 0x140);
    for (i = 0; i <= n; i++) {
        c = g_2c33[n - i];
        c2 = c + 0xd0;
        y -= 8;
        FUN_0ffe_000e(0, 0, g_2c07[c2]->w10, g_2c07[c2]->w12, y, 0xa5,
                      g_2c07[c2]->p[1], g_19e4[g_349f], 8, 0x140);
    }

    if (g_27e8 >= 0x32)
        g_27e9 = 0;
    else
        g_27e9 = 1;
    g_27e8 = 0;

    if (v != 0) {
        FUN_0ffe_000e(0xae - (n * 9 + 9), 0xa5, 0xae, 0xb0,
                      0xae - (n * 9 + 9), 0xa5,
                      g_19e8, g_19e4[g_349f ^ 1], 0x140, 0x140);
        y = 0xae;
        for (i = 0; i <= n; i++) {
            c = g_2c33[n - i];
            c2 = c + 0xd0;
            y -= 8;
            FUN_0ffe_000e(0, 0, g_2c07[c2]->w10, g_2c07[c2]->w12, y, 0xa5,
                          g_2c07[c2]->p[1], g_19e4[g_349f ^ 1], 8, 0x140);
        }
        g_27e9 = 1;
    }

    if (g_num_cur < g_num_prev && v == 0)
        return;
    if (g_num_cur >= g_num_prev)
        g_num_prev = g_num_cur;

    FUN_13b2_17ba(v);
}

/* FUN_13b2_17ba, 467 B / 167 ins.  Byte-exact in src/m17ba.c.  Same-module
 * forward reference from m_154f, hence it is defined here after the caller.
 * It is m_154f's draw with the other field-parity bit and the score-parity
 * constants 0xb4/0xbf in place of 0xa5/0xb0. */
void FUN_13b2_17ba(unsigned char v)
{
    unsigned char i, n, c, c2;
    int y;

    FUN_1000_2937(g_num_prev, (char far *)g_2c33, 0xa);
    n = strlen(g_2c33) - 1;
    y = 0xae;
    FUN_0ffe_000e(0xae - (n * 9 + 9), 0xb4, 0xae, 0xbf, 0xae - (n * 9 + 9), 0xb4,
                  g_19e8, g_19e4[g_349f], 0x140, 0x140);
    for (i = 0; i <= n; i++) {
        c = g_2c33[n - i];
        c2 = c + 0xd0;
        y -= 8;
        FUN_0ffe_000e(0, 0, g_2c07[c2]->w10, g_2c07[c2]->w12, y, 0xb4,
                      g_2c07[c2]->p[1], g_19e4[g_349f], 8, 0x140);
    }
    if (v == 0)
        return;
    FUN_0ffe_000e(0xae - (n * 9 + 9), 0xb4, 0xae, 0xbf, 0xae - (n * 9 + 9), 0xb4,
                  g_19e8, g_19e4[g_349f ^ 1], 0x140, 0x140);
    y = 0xae;
    for (i = 0; i <= n; i++) {
        c = g_2c33[n - i];
        c2 = c + 0xd0;
        y -= 8;
        FUN_0ffe_000e(0, 0, g_2c07[c2]->w10, g_2c07[c2]->w12, y, 0xb4,
                      g_2c07[c2]->p[1], g_19e4[g_349f ^ 1], 8, 0x140);
    }
}

/* m_34e5 and m_35dd are only reachable from main's `tail:` label, which the
   frame loop falls out of exactly when g_27fc (or g_27f3, set from g_27fc) is
   set.  main() itself never returns -- it is an unconditional `for(;;)` -- so
   this is the one place the log can report that the frame loop was left, and
   run_game.py's timeout can therefore not be mistaken for a boot that never
   happened.  Reporting it once keeps the line from repeating every spin.

   The body below is FUN_13b2_34e5, reconstructed as src/m34e5.c to 244 of the
   target's 248 bytes (93/97 instructions).  The four missing bytes are a
   redundant adjacent duplicate epilogue that BCC 3.1 -O2 coalesces; see the
   "Adjacent duplicate epilogues" section in notes/compiler.md.  It is inert
   here because FUN_1df0_0222's stand-in returns 0, so the second guard returns
   before any of the calls that would touch a real pointer. */

char far *FUN_13b2_4ecf(char far *p)
{
    return p;
}

void m_34e5(void)
{
    static int logged;
    int v;
    char far *p;

    if (!logged) { logged = 1; emit("[boot] frame loop exited; g_27fc is set"); }

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
/* FUN_13b2_35dd, 709 B / 273 ins.  Byte-exact in src/m35dd.c (709/709 bytes,
 * all 273 instructions).  The title/menu screen main() falls into once g_27fc
 * is set: clears the panel, lays out the option rows (each row a label cell and
 * three number cells), prints the score, then spins until the confirm gate
 * (g_7d6) fires or the retry counter (0x258) runs out. */
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

void FUN_13b2_154f(int v) { m_154f((unsigned char)v); }  /* alias of m_154f */
void FUN_13b2_0e2f(void) { }
void FUN_13b2_1347(void) { }
void FUN_13b2_1a40(void) { }
void FUN_13b2_1393(int v) { (void)v; }
void FUN_13b2_11ac(void) { }
void FUN_13b2_110b(void) { }
void FUN_13b2_3153(int v) { (void)v; }
void FUN_13b2_31b7(void) { }
void FUN_13b2_1a81(void) { }
