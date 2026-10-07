/* Stand-in for FUN_1fd3_0012 (segment 0xfd3, flat 0x0FD42, 224 bytes / 106 ins).
   Called by m_0252 with ten word arguments (`add sp, 0x14` after the lcall), so
   the signature must keep all ten or the pushed byte count changes.  The prolog
   is `push bp / sub sp,0xa / push si / push di / cld`, an OROS-style helper.
   Not reconstructed. */
void FUN_1fd3_0012(int a, int b, int c, int d, int e,
                   int f, int g, int h, int i, int j)
{
    (void)a; (void)b; (void)c; (void)d; (void)e;
    (void)f; (void)g; (void)h; (void)i; (void)j;
}
