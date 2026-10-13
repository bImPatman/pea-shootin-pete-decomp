/* Stub for the 0x300-byte DAC uploader m_f44d's dead half calls at 0xfa6:0x6a
   (flat 0x1faca).  Nothing in the run build reaches the dead half, so the body
   is empty and the symbol just resolves the far call.  The single argument is
   the palette buffer as a far pointer, read off the callee's [bp+6] far slot. */
void upload(char far *buf)
{
}
