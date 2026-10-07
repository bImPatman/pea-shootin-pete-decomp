/* @target 0x3DD9 */
/* @name   m_02b9 */
/* @proto  void __far m_02b9(void) */
/* @module same */
/* @extra  xmod/xfba.c g13b2.c */

/* FUN_13b2_02b9, 39 bytes / 12 instructions.  Called by m_013b at 0x3DD9 once
   g_2bf5 wraps past 0xFF, and by FUN_13b2_2c80 at 0x681B unconditionally.
 *
 * Target:
 *   3DD9  cmp byte ptr [0x27fd], 0
 *   3DDE  je 0x3dff
 *   3DE0  push 0
 *   3DE2  push word ptr [0x19e4]
 *   3DE6  push 0xae
 *   3DE9  push 0xce
 *   3DEC  push 0xa6
 *   3DEF  push 0xbe
 *   3DF2  lcall 0xfba, 0x2
 *   3DF7  add sp, 0xc
 *   3DFA  mov byte ptr [0x27fd], 0
 *   3DFF  retf
 *
 * The six `push imm` are 2 bytes each (0x6A is `push imm8` and Borland widens
 * it to a word), so `add sp, 0xc` -- 12 bytes -- is all six words and nothing
 * else is on the stack.  Borland pushes arguments right to left, so the call
 * reads FUN_1fba_0002(0xbe, 0xa6, 0xce, 0xae, g_19e4, 0): the last thing pushed
 * is the first argument.
 *
 * FUN_1fba_0002 is 132 bytes / 66 instructions at 0x0FBA2, frameless with an
 * explicit `push si / push di / cld`, which is the shape of a Borland
 * text-output routine.  The constants 0xbe/0xa6/0xce/0xae and g_19e4 look like
 * an EGA/VGA colour-pair write, and the call is gated on g_27fd and clears it
 * afterwards, so this reads as a one-shot "poke the attribute registers" step.
 * Not reconstructed; see src/xmod/xfba002.c.
 */

extern unsigned char g_27fd;
extern unsigned int  g_19e4;

void FUN_1fba_0002(int a, int b, int c, int d, int e, int f);

void m_02b9(void)
{
    if (g_27fd) {
        FUN_1fba_0002(0xbe, 0xa6, 0xce, 0xae, g_19e4, 0);
        g_27fd = 0;
    }
}
