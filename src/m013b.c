/* @target 0x3C5B */
/* @name m_013b */
/* @proto void __far m_013b(void) */
/* @module same */
/* @extra g13b2.c xmod/xe25978.c xmod/xe256fe.c xmod/xe25b0f.c */
#include <string.h>

extern unsigned char g_27ff;
extern int g_2bf5;
extern char far *g_27aa;
extern char far *g_34a1;
extern unsigned char g_2800, g_27f2, g_27f6, g_2c61, g_27ed, g_27f9, g_27fa, g_27fc, g_27f3;

struct bfi65 { unsigned char pad[0x65]; unsigned int : 2; unsigned int b2 : 1; };
struct dispatch_entry { void (far *fn)(void); unsigned char pad[0x30]; };
extern struct dispatch_entry g_dispatch[];

unsigned char FUN_0e25_0978(char far *p, char far *lit);
void FUN_0e25_06fe(char far *p, char far *lit);
void FUN_0e25_0b0f(char far *p);

void m_0252(void);
void m_02b9(void);
void m_2f97(void);
void m_24e7(int a);
void m_1393(int a);
void m_2c80(void);
void m_3153(int a);

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

void m_0252(void) { }
void m_02b9(void) { }
void m_1393(int a) { }
void m_2f97(void) { }
void m_2c80(void) { }
void m_24e7(int a) { }
void m_3153(int a) { }