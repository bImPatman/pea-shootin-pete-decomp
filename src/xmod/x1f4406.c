/* Stand-in for FUN_1f44_0006 -- returns a far pointer in DX:AX. */
static char buf[64];
char far *FUN_1f44_0006(int a, int b) { return buf; }