/* Stand-in for FUN_1b11_1566 -- returns a far pointer in DX:AX. */
static char buf[64];
char far *FUN_1b11_1566(int a, int b) { return buf; }