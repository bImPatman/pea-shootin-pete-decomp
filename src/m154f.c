/* @target FUN_13b2_154f */
/* @name   m_154f */
/* @proto  void m_154f(unsigned char v) */
/* @extra  g13b2.c xmod/x0ffe0e.c xmod/x1002937.c */
#include <string.h>

struct g207 {
    int far *p;                 /* +0x00 */
    char     pad[0x0c];         /* +0x04 */
    int      w10;               /* +0x10 */
    int      w12;               /* +0x12 */
};

extern unsigned char    g_2c33[7];
extern unsigned char    g_2c61;
extern unsigned char    g_349f;
extern unsigned int     g_19e4[];
extern unsigned int     g_19e8;
extern struct g207 far *g_2c07[];
extern unsigned long    g_num_cur;
extern unsigned long    g_num_prev;
extern unsigned char    g_27e8;
extern unsigned char    g_27e9;

void FUN_0ffe_000e(int, int, int, int, int, int, int, int, int, int);
void FUN_1000_2937(unsigned long, char far *, int);
void FUN_13b2_17ba(char v);

void m_154f(unsigned char v)
{
    unsigned char i;
    unsigned char n;
    unsigned char c;
    unsigned char c2;
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

void FUN_13b2_17ba(char v)
{
    unsigned char i;
    unsigned char n;
    unsigned char c;
    unsigned char c2;
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
