/* @target 0x3B2F */
/* @name   main */
/* @proto  int main(int argc, char far *argv[], char far *envp[]) */
/* @module none */
/* @extra  mcallees.c xmod/xb11a.c xmod/xf44d.c xmod/xf44b.c xmod/xf44c.c xmod/xe256.c xmod/xb159.c keyread.c keypoll.c statebak.c clrbit3.c xmod/xnodefr.c xmod/xbuf2vg.c */

/* Target 0x13B2F -- FUN_13b2_000f, the C entry point.
 *
 * Found via the Borland startup rather than the export: `entry` is hand-written
 * RTL assembly, and its last act is a far call whose operand is relocated
 * (relocation [2], flat 0x14B) to segment 0x3B2 offset 0x000F.  Module-relative
 * that is flat 0x3B2F, and the flat rule `seg * 16 + off` is confirmed by
 * `lcall 0xf44,0xf1` in this very function landing on FUN_1f44_00f1 -- the
 * already-exact src/clrbit3.c.
 *
 * Three Borland idioms carry most of the weight here:
 *
* 1. In the large model a call to a function in the *same* .c file is the cheap
 *    `nop / push cs / call rel16` form, because both halves share a segment and
 *    the callee's `retf` pops the pushed CS.  A callee in a *different* .c file
 *    gets its own code segment, so the same source call becomes a 5-byte `lcall`
 *    with a relocated segment:offset.  This function mixes both -- six `lcall`s
 *    to reconstructed modules plus ten more that used to be same-module.
 *
 *    Those ten used to be defined at the bottom of this file, which is why they
 *    were empty: `call rel16` can only reach +-32K of the call site, and every
 *    other function in segment 0x13b2 sits further than that away, so they all
 *    had to be `nop`-shaped placeholders.  They now live in src/mcallees.c.
 *    The price is that this file stops being byte-exact -- the ten calls become
 *    `lcall 5 bytes` instead of `nop / push cs / call rel16`, the same length,
 *    but a different opcode, so `check.py` no longer reports PASS for main.c.
 *    That is the intended trade: main()'s frame loop only exits when g_27fc is
 *    set, and the only code that sets it is behind m_013b, one of those ten.
 *    An empty m_013b cannot ever set it, so byte-exact main.c and a booting
 *    program were mutually exclusive.

 *
 * 2. `les bx,[bp+8]` with `es:[bx+4]`/`es:[bx+6]` is `argv[1]`: argc sits at
 *    [bp+6] and is never read, argv at [bp+8] is a far pointer to 4-byte far
 *    entries, so entry 1 is offset-then-segment at +4/+6.
 *
 * 3. The `repne scasb` / `not cx` / `sub di,cx` / `repe cmpsb` block is
 *    Borland's inlined `strcmp` against a literal in DS; the trailing
 *    `sbb ax,ax / sbb ax,0xffff` normalises the result to 0/1.
 *
 * Absolute DS addresses and lcall operands are masked by diffasm (anything
 * below the image size is an `addr`, big displacements are `mem?`), so the
 * globals below only need to exist, not to land at 0x2C60 and friends.  What
 * must match exactly is the control flow: diffasm normalises branches to
 * `rel:F+N`.
 */

#include <string.h>

/* far callees, one module each */
int  m_b11a(char far *p);
int  m_f44d(char far *p);
int  m_f44b(char far *p, unsigned char v);
int  m_f44c(char far *p);
int  m_e256(char far *p);
int  m_b159(char far *p);
int  key_read(void);
int  key_poll(void);

/* m_1a40's own far callees */
void node_free(char far *p);
void buf_to_vga(unsigned char far *p);
void pal_upload(char far *a, char far *b);

/* already reconstructed, verified exact in their own right */
struct blk_s { unsigned char pad0[2]; unsigned char state; unsigned char pad1[3];
              unsigned char data[0x300]; };
void statebak(struct blk_s far *blk);
void clrbit3(unsigned char far *rec);

/* former same-module callees, now cross-module -- see src/mcallees.c */
void m_1df6(void);
void m_38a2(void);
void m_2415(void);
void m_24e7(int v);
void m_0dd6(void);
void m_154f(int v);
void m_013b(void);
void m_1a40(void);
void m_34e5(void);
void m_35dd(void);

char g_cmd[80];            /* DS:0x8BC, compared against argv[1] */
unsigned char g_2c60;     /* set from the strcmp result */
unsigned char g_349e, g_349f;
unsigned char g_27f3, g_27fc;
unsigned char g_386d;
char far *g_27aa;
char far *g_34a9;
char far *g_34ad;
char far *g_3858;

int main(int argc, char far *argv[], char far *envp[])
{
    if (strcmp(argv[1], g_cmd) == 0)
        g_2c60 = 1;
    else
        g_2c60 = 0;

    m_1df6();

    for (;;) {
init_loop:
    if (g_2c60 == 0)
        m_38a2();
    m_2415();
    m_24e7(1);
    goto check;

frame:
    g_349f = g_349e ^ 1;
    m_0dd6();
    m_b11a(g_34a9);
    m_154f(0);
    m_f44d(g_3858);
    g_349e = g_349f;
    m_f44b(g_3858, g_349f);
    m_f44c(g_3858);
    m_e256(g_27aa);
    m_013b();

    if (g_27f3) {
        m_1a40();
        goto tail;
    }

check:
    if (!g_27fc)
        goto frame;

tail:
    m_b159(g_34ad);
    if (g_386d) {
        while (key_poll())
            key_read();
    } else {
m_34e5();
        m_35dd();
    }
    }
}
