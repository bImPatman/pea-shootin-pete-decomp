/* Stand-in for FUN_1b11_15d6 -- returns a far pointer in DX:AX. */
static char buf[64];
char far *FUN_1b11_15d6(char far *p, char far *s) { return buf; }