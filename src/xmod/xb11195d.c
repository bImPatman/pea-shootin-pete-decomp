/* Stand-in for FUN_1b11_195d -- returns a far pointer in DX:AX. */
static char buf[64];
char far *FUN_1b11_195d(int a, int b) { return buf; }