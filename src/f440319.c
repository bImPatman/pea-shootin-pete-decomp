/* @target FUN_1f44_0319 */
/* @name   FUN_1f44_0319 */
/* @proto  void FUN_1f44_0319(char far *p, unsigned char a, unsigned char b, unsigned char c) */
/* @module cross */
/* @extra  xmod/xfd3150.c */

extern void FUN_1fd3_0150(int v);

void FUN_1f44_0309(char far *p, int v)
{
    FUN_1fd3_0150(v);
}

void FUN_1f44_0319(char far *p, unsigned char a, unsigned char b, unsigned char c)
{
    unsigned int cur;

    for (cur = a * 16000; cur <= b * 16000; cur += c * 320)
        FUN_1f44_0309(p, cur);
}
