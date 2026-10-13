/* Owner of the 0x300-byte shadow palette buffer for standalone check.py builds
   of src/f44d.c, which declares it `extern`.

   In the linked game the buffer is defined by src/dacupd.c (m_f44c, the "commit
   the staged palette" half of the same path).  f44d.c and dacupd.c cannot both
   define it, so f44d.c externs it and names this file in `@extra`; this file is
   deliberately *not* in build_game.py's CORE, so the linked image gets the one
   definition from dacupd.c and the harness gets this one.  The address is masked
   by diffasm, so which module owns it never affects the byte comparison. */
unsigned char g_3550[0x300];
