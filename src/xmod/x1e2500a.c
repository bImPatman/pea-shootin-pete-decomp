/* Stand-in for FUN_1e25_000a -- returns a far pointer in DX:AX. */
static char buf[64];
char far *FUN_1e25_000a(int a, int b, char far *s) { return buf; }