/* @target FUN_1b11_034c */
/* @name   FUN_1b11_034c */
/* @proto  void FUN_1b11_034c(int a, int b, char far *s, unsigned char c, unsigned char d, unsigned char e, unsigned char f) */
/* @module cross */
/* @extra  g13b2.c xmod/xfd301fa.c */

extern void FUN_1fd3_01fa(int x, int y, int font, int color, char far *s, int flag);
extern unsigned int g_19e4[];

void FUN_1b11_034c(int a, int b, char far *s, unsigned char c, unsigned char d, unsigned char e, unsigned char f)
{
    switch (f) {
    case 0:
    case 1:
    case 2:
    case 3:
        FUN_1fd3_01fa(a + 1, b + 1, g_19e4[f], d, s, 0);
        FUN_1fd3_01fa(a, b, g_19e4[f], c, s, e);
        break;
    case 0x63:
        FUN_1fd3_01fa(a + 1, b + 1, g_19e4[0], d, s, 0);
        FUN_1fd3_01fa(a, b, g_19e4[0], c, s, e);
        FUN_1fd3_01fa(a + 1, b + 1, g_19e4[1], d, s, 0);
        FUN_1fd3_01fa(a, b, g_19e4[1], c, s, e);
        break;
    }
}
