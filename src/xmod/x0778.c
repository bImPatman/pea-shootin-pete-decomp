/* Stand-in for FUN_1000_0778 (segment 0x1000, flat 0x0778, 66 bytes / ~26 ins),
   the heap allocator m_f4406 and m_e2500a call to carve their records (0x306 and
   0x10b bytes).  Returns a far pointer in DX:AX.  Not reconstructed.

   This is a bump allocator rather than one shared static buffer: m_1df6 calls
   m_e2500a first (for g_27aa) and m_f4406 later (for g_3858), and if both landed
   on the same storage the second initialiser would stomp the first record.
   Successive blocks are distinct, even-sized and zero on first use (BSS). */
static char arena[0x2000];
static unsigned used;

char far *FUN_1000_0778(int n)
{
    char far *p = arena + used;
    used = (used + (unsigned)n + 1u) & ~1u;
    return p;
}
