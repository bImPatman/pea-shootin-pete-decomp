/* Stub for the palette-slide helper m_f44d calls at 0xfa6:0x2f.

   Still a stand-in: nothing in the run build reaches it with a live record, so
   the body is empty and the symbol just resolves the far call.  The three
   arguments, worked back off the callee's stack slots, are the two range words
   off the 0x3850 record followed by the palette buffer as a far pointer; see the
   comment in src/f44d.c. */
void pal_shift(unsigned int to, unsigned int from, char far *buf)
{
}
