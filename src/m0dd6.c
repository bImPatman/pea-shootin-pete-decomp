/* @target FUN_13b2_0dd6 */
/* @name   m_0dd6 */
/* @proto  void m_0dd6(void) */
/* @module same */
/* @extra  g13b2.c xmod/x0adb1.c xmod/x0adb224.c xmod/x0adb2bc.c xmod/x0adb1ff.c xmod/x0adb324.c xmod/x11ae1.c xmod/x118e4.c xmod/xb11009a.c xmod/xb1100b5.c xmod/xb1134c.c xmod/xb11462.c xmod/x1f4429e.c xmod/xe250a23.c xmod/xe250a89.c xmod/xe2503a4.c xmod/xe25b0f.c xmod/x161d38e.c xmod/x163c7.c xmod/x163e4.c xmod/x163a9.c */

/* Target 0x48F6..0x4C29 (820 bytes / 218 insns): the "small frame" update
   m_0dd6 runs once a frame while g_2c60 == 1.  The interesting part is the
   tail at 0x4AD2, an input/stage disambiguation state machine:

       0x4AD2 cmp [0x2800],0 ; jne next ; jmp end      -> if (g_2800 == 0) goto end;
       0x4ADC cmp [0x7d6],0  ; jne next ; jmp stage2   -> if (g_7d6 == 0) goto stage2;
       0x4AE6 mov al,[0x2805]; mov ah,0; or ax,ax
             je input ; jmp stage2                     -> if (!g_2805) goto input; goto stage2;

   `mov al;mov ah,0;or ax,ax` is BCC's `!x` truthiness test, so the g_2805 gate
   is the `!g_2805` form while 0x2800 / 0x7d6 are `== 0` forms (cmp byte).
   The required source is therefore a goto ladder, exactly as disassembled.

   cli/sti (0x4AF2 / 0x4B08) bracket the keyboard/joystick read.
 */
#include <string.h>

extern unsigned char g_27ec, g_27ea, g_27eb, g_27f2, g_27f3, g_27f9;
extern unsigned char g_2800, g_2805, g_2c60, g_34a0, g_7d6;
extern unsigned char g_18f8, g_18fa;
extern unsigned char g_386d, g_386e, g_386f, g_3870, g_3871, g_3872, g_3873;
extern unsigned char g_3874, g_3875, g_3876, g_3877;
extern unsigned char g_3889, g_388b, g_388c, g_3898, g_389a;
extern unsigned char g_38a4, g_38a5, g_38b4, g_38b7, g_38b9, g_38bc;
extern char far *g_27aa;
extern char far *g_3858;

unsigned char FUN_0adb_0001(unsigned char v);
unsigned char FUN_0adb_0224(unsigned char v);
void FUN_0adb_01ff(void);
void FUN_0adb_02bc(void);
void FUN_0adb_0324(void);
int  FUN_1000_1ae1(void);
void FUN_1000_18e4(void);
void FUN_1b11_009a(void);
void FUN_1b11_00b5(int a, int b);
void FUN_1b11_034c(int a, int b, char far *s, int c, int d, int e, int f);
void FUN_1b11_0462(int a, int b, char far *s);
void FUN_1f44_029e(char far *p, char v);
void FUN_1e25_03a4(char far *p);
void FUN_1e25_0a23(char far *p);
void FUN_1e25_0a89(char far *p);
void FUN_1e25_0b0f(char far *p);
void FUN_161d_038e(void);
int  FUN_161d_03c7(void);
int  FUN_161d_03e4(void);
int  FUN_161d_03a9(int v);

void FUN_13b2_1393(int v);
void FUN_13b2_11ac(void);
void FUN_13b2_110b(void);
void FUN_13b2_1a40(void);

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
        if (g_18f8)
            FUN_1b11_034c(0x64, 0x50, "<< Sound On >>", 0x10, 0x1f, 1, 0x63);
        else
            FUN_1b11_034c(0x64, 0x50, ">> Sound Off <<", 0x10, 0x1f, 1, 0x63);
        FUN_1e25_0b0f(g_27aa);
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

    if (FUN_161d_03a9(1) == 0) {
        g_27eb = 1;
        return;
    }
    if (g_27eb == 0)
        goto end;
    g_27eb = 0;
    FUN_13b2_110b();
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

/* Same-module callees.  In the target FUN_13b2_1393/11ac/110b/1a40 all sit ABOVE
   m_0dd6 in the segment (0x4EB3 / 0x4CCC / 0x4C2B / 0x5560), so defining them
   here makes BCC emit the aligning nop before its `push cs`. */
void FUN_13b2_1393(int v) { (void)v; }
void FUN_13b2_11ac(void) { }
void FUN_13b2_110b(void) { }
void FUN_13b2_1a40(void) { }