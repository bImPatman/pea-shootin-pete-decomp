/* @target 0x5560 */
/* @name   m_1a40 */
/* @proto  void m_1a40(void) */
/* @module none */
/* @extra  ../statebak.c ../clrbit3.c xnodefr.c xbuf2vg.c */

/* Target 0x5560 (65 bytes / 17 insns): frameless, four far calls each passing
   one global far pointer, then a bare `retf`.  No branches at all, so the whole
   function is determined by its callee list:
     node_free(g_34a9)  ->  FUN_1b11_2024
     statebak(g_3858)   ->  FUN_1f44_0143   (src/statebak.c, exact)
     clrbit3(g_3858)    ->  FUN_1f44_00f1   (src/clrbit3.c, exact)
     buf_to_vga(g_27aa) ->  FUN_1e25_0b0f
   The middle two are already verified reconstructions, so this one composes
   existing work rather than restating it.

   In the target this lives in main's own module and is reached with the near
   `nop / push cs / call` form; it is a separate file here only so the harness
   can verify it on its own, and main.c keeps its own copy for that reason.
 */
struct blk_s { unsigned char pad0[2]; unsigned char state; unsigned char pad1[3];
              unsigned char data[0x300]; };

void node_free(char far *p);
void buf_to_vga(unsigned char far *p);
void statebak(struct blk_s far *blk);
void clrbit3(unsigned char far *rec);

char far *g_34a9;
char far *g_27aa;
char far *g_3858;

void m_1a40(void)
{
    node_free(g_34a9);
    statebak(g_3858);
    clrbit3(g_3858);
    buf_to_vga(g_27aa);
}