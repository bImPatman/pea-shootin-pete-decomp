/* @target FUN_13b2_3153 */
/* @name   m_3153 */
/* @proto  void m_3153(int n) */
/* @module same */
/* @extra  g13b2.c xmod/xb11a.c xmod/xf44b.c xmod/xf44c.c xmod/xf44d.c */

/* FUN_13b2_3153 -- flat 0x06C73..0x06CD7, 100 bytes / 33 instructions.
 *
 * A counted frame-pump.  The prologue jumps straight to the loop condition and
 * the body is laid out *after* it, which is Borland's `while` loop shape:
 *
 *     6C73  push bp
 *     6C74  mov bp, sp
 *     6C76  jmp 0x6ccb              <- straight to the test
 *     6C78  ...body...
 *     6CCB  mov ax, [bp+6]          <- load n
 *     6CCE  dec word ptr [bp+6]     <- decrement the parameter in memory
 *     6CD1  or ax, ax               <- test the value loaded BEFORE the dec
 *     6CD3  jne 0x6c78
 *     6CD5  pop bp
 *     6CD6  retf
 *
 * `dec word ptr [bp+6]` plus `or ax, ax` on the pre-decrement value is Borland's
 * spelling of post-decrement in `while (n--)`, so the parameter is a word and
 * the parameter is decremented in place.  Because the test runs first, `n == 0`
 * skips the body entirely -- a `do { } while (n--)` compiles to the same code
 * minus the leading `jmp`, since Borland rotates a do-while by knowing the body
 * runs at least once.
 *
 * Each iteration:
 *   1. toggles the frame-parity byte, g_349f = g_349e ^ 1;
 *   2. runs FUN_13b2_0dd6, the same-module near call `push cs / call rel16`
 *      whose target resolves to flat 0x48F6 = module offset 0x0DD6;
 *   3. frees the node list, FUN_1b11_1A94 (g_34a9);
 *   4. runs FUN_1f44_01DE (g_3858);
 *   5. copies the toggled byte back, g_349e = g_349f;
 *   6. runs FUN_1f44_029E (g_3858, g_349f) -- note `push ax` here reuses the AL
 *      loaded at 6CA4, so the second argument is the parity byte itself;
 *   7. runs FUN_1f44_01C3 (g_3858).
 *
 * Steps 3, 4, 6 and 7 are `lcall`s into 0x0B11 and 0x0F44, so they are far
 * callees and need the stand-ins listed on @extra.  m_0dd6 is a same-module
 * near call and therefore has to live in this translation unit.
 */

extern unsigned char g_349e, g_349f;
extern char far *g_34a9;
extern char far *g_3858;

/* Stand-ins for the far callees. */
int  m_b11a(char far *p);
int  m_f44b(char far *p, unsigned char v);
int  m_f44c(char far *p);
int  m_f44d(char far *p);

/* FUN_13b2_0DD6, 821 bytes / 280 instructions.  Still a stub, but it is a
 * same-module near call, so Borland must be able to resolve it locally. */
void m_0dd6(void) { }

void m_3153(int n)
{
    while (n--) {
        g_349f = g_349e ^ 1;
        m_0dd6();
        m_b11a(g_34a9);
        m_f44d(g_3858);
        g_349e = g_349f;
        m_f44b(g_3858, g_349f);
        m_f44c(g_3858);
    }
}
