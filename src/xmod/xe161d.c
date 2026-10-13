/* Stand-in for the cross-module callee of m_e256 (FUN_1e25_0256) and of m_24e7.
   Not reconstructed yet.  m_24e7 drains it with `while (FUN_161d_08de());`, so it
   must return 0 once drained; m_e256's `if (!FUN_161d_08de())` therefore runs its
   block.  Returns 0 (input already drained). */
int FUN_161d_08de(void) { return 0; }
