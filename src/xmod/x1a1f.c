/* Stand-in for FUN_1000_1a1f (segment 0x1000, flat 0x1a1f, 153 bytes), the
   int86x-style RTL helper FUN_1f44_0426 calls to load the DAC palette.  Not
   reconstructed; only the symbol is needed. */
int FUN_1000_1a1f(int no, void far *in, void far *out, void far *seg)
{
    (void)no; (void)in; (void)out; (void)seg;
    return 0;
}
