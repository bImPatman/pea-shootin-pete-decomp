/* Stand-in for FUN_1b11_0056 (segment 0xb11, flat 0x0B166, 68 bytes / 25 ins).
   Called by m_0252 when FUN_1b11_15d6 returned null.  The prolog
   (`push bp / sub sp,0x10 / mov word ptr [bp-0x10], 3 / push ss / lea ax,[bp-0x10]`)
   is the shape of a small OROS string-output helper.  Not reconstructed. */
void FUN_1b11_0056(char far *msg)
{
    (void)msg;
}
