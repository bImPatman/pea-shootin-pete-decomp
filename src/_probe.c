#include <string.h>

unsigned int g_2bff, g_2c01;
char far g_2c33[16];

char far *FUN_1000_28ef(int a, char far *b, int c) { return b; }

void probe(int delta)
{
    char far *s1 = "abc";
    char far *s2 = FUN_1000_28ef(delta, g_2c33, 10);
    strcat(g_2c33, s2);
    strcat(g_2c33, s1);
    {
        long q = delta;
        g_2bff += (unsigned int)q;
        g_2c01 += (unsigned int)(q >> 16);
    }
}