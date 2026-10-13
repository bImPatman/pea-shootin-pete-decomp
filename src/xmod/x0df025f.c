/* Stand-in for FUN_0df0_025f -- two ints in, far pointer out (DX:AX). */
static char buf[64];
char far *FUN_0df0_025f(char a, int b) { (void)a; (void)b; return buf; }
