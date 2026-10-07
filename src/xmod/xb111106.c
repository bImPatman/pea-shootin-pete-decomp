/* Stand-in for FUN_1B11_1106 -- returns a far pointer in DX:AX. */
static char buf[64];
char far *FUN_1b11_1106(int a, int b, char far *s) { return buf; }
