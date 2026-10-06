/* main()'s same-module callees, moved into their own modules.
 *
 * main.c reaches these with `nop / push cs / call rel16`, which only works
 * because both halves share a code segment.  Borland gives every .c file its
 * own <MODULE>_TEXT segment, so leaving them in main.c is what forced them to
 * stay empty: growing one of them shifts the rel16 of every call after it and
 * breaks main's own bytes, which is why this file set exists.
 *
 * The cost is that each of these is now an `lcall` in main instead of a
 * same-module call.  Both encodings are 5 bytes, so main stays 300 bytes long,
 * but the opcode differs (9A vs 0E E8 rel16) and main.c therefore stops being
 * byte-exact.  That trade is deliberate: main()'s frame loop only exits when
 * g_27fc is set, and the only code that sets it lives behind m_013b, which was
 * one of these unreachable no-ops.  See the note in src/main.c.
 *
 * Bodies are empty for now.  Each is reconstructed against its target
 * function one at a time; the @target marker is added as that happens, so
 * `check.py all` only reports the ones actually being worked on.
 */

/* m_1a40's own far callees */
void node_free(char far *p);
void buf_to_vga(unsigned char far *p);
void pal_upload(char far *a, char far *b);

struct blk_s { unsigned char pad0[2]; unsigned char state; unsigned char pad1[3];
              unsigned char data[0x300]; };
void statebak(struct blk_s far *blk);
void clrbit3(unsigned char far *rec);

extern char far *g_27aa;
extern char far *g_34a9;
extern char far *g_3858;

/* Target FUN_13b2_1df6, 411 bytes / 127 instructions. */
void m_1df6(void)
{
}

/* Target FUN_13b2_38a2, 1030 bytes / 219 instructions. */
void m_38a2(void)
{
}

/* Target FUN_13b2_2415, 210 bytes / 59 instructions.  Clears g_27fc. */
void m_2415(void)
{
}

/* Target FUN_13b2_24e7, 1945 bytes / 676 instructions -- the largest of the set. */
void m_24e7(int v)
{
    v;
}

/* Target FUN_13b2_0dd6, 821 bytes / 280 instructions. */
void m_0dd6(void)
{
}

/* Target FUN_13b2_154f, 619 bytes / 215 instructions. */
void m_154f(int v)
{
    v;
}

/* Target FUN_13b2_013b, 279 bytes / 104 instructions.  The boot gate: it calls
   FUN_13b2_2c80, which is what sets g_27fc. */
void m_013b(void)
{
}

/* Target 0x5560 (65 bytes / 17 insns): frameless, four far calls each passing
   one global far pointer, then a bare `retf`.  No branches at all, so the whole
   function is determined by its callee list:
     node_free(g_34a9)  ->  FUN_1b11_2024   (src/nodefree.c, exact)
     statebak(g_3858)   ->  FUN_1f44_0143   (src/statebak.c, exact)
     clrbit3(g_3858)    ->  FUN_1f44_00f1   (src/clrbit3.c, exact)
     buf_to_vga(g_27aa) ->  FUN_1e25_0b0f   (src/buftovg.c, exact)
   The middle two are already verified reconstructions, so this one composes
   existing work rather than restating it.
 */
void m_1a40(void)
{
    node_free(g_34a9);
    statebak(g_3858);
    clrbit3(g_3858);
    buf_to_vga(g_27aa);
}

/* Target FUN_13b2_34e5, 248 bytes / 97 instructions. */
void m_34e5(void)
{
}

/* Target FUN_13b2_35dd, 709 bytes / 273 instructions. */
void m_35dd(void)
{
}
