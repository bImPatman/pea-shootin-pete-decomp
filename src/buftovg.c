/* @target 0xED5F */
/* @name   buf_to_vga */
/* @proto  void buf_to_vga(struct bvg far *p) */
/* @module cross */
/* @extra  xmod/xb2vcb.c */

/* Target 0xED5F -- FUN_1e25_0b0f (224 bytes / 72 insns).
 *
 * Same record as m_e256: a far-pointer array at +0x00, a byte array at +0x50
 * and the entry count as a byte at +0x109.  One pass clears the byte array and
 * resets four fields of every entry it points at -- three bytes, then copies
 * the word at +0x10 back over the word at +0x0C.
 *
 * Both array cursors are strength reduced off the loop counter, one stepped by
 * one and the other by four.  Every write through an entry re-derives es:bx
 * from the cursor with `les`, so nothing about the entry pointer is cached
 * across stores; the constant zero stays live in al for all three byte stores.
 *
 * After the pass, each of the two module-wide far pointers is flushed: if it is
 * non-null it is handed to the same-module flush at 0xEFE5 (offset first, then
 * segment), then both are cleared to zero along with the flag byte at 0x18F9.
 *
 * The two same-module callees differ in width, which pins their order in the
 * file: 0xE68E is called with the four byte push cs / call rel16 form and sits
 * before this function, while 0xEFE5 is called with the five byte
 * nop / push cs / call rel16 form and sits after it.  A cross-module call would
 * be an lcall instead.
 */
struct bvg_ent {
    unsigned char b0;              /* +0x00 */
    unsigned char pad1[3];
    unsigned char live;            /* +0x04 */
    unsigned char b5;              /* +0x05 */
    unsigned char pad2[6];
    unsigned int  w12;             /* +0x0C */
    unsigned int  pad3;
    unsigned int  w16;             /* +0x10 */
};

struct bvg {
    struct bvg_ent far *ent[20];   /* +0x00 */
    unsigned char used[0xB9];      /* +0x50 */
    unsigned char count;           /* +0x109 */
};

/* cross module: the runtime hook at segment 0, offset 0x20CB */
void FUN_1000_20cb(void);

/* The two module-wide cursors, and the flag byte at 0x18F9.
 *
 * These are three distinct globals in PETE.EXE -- one data segment -- but Borland
 * in the large model gives every .c file its own data segment, so the same
 * spelling in two reconstructions becomes two symbols and the link fails on the
 * duplicate.  They are renamed here to keep both modules linkable; src/e256.c
 * keeps the original names for the two it shares with this file.  Only the
 * operand addresses change, and those are masked as `addr` by diffasm, so both
 * reconstructions still compare exact. */
char far *g_cur_a;          /* +0x3548 offset, +0x354A segment */
char far *g_cur_b;          /* +0x354C offset, +0x354E segment */
unsigned char g_flag_flush; /* +0x18F9 */

/* same module, and after buf_to_vga: five byte call form */
void FUN_1e25_0d95(char far *p);

/* Same module, defined *before* buf_to_vga so the call stays four bytes
   wide -- the width is what distinguishes the two forms. */
void FUN_1e25_043e(struct bvg far *p) { p; }

void buf_to_vga(struct bvg far *p)
{
    int i;

    FUN_1000_20cb();

    for (i = 0; i < p->count; i++) {
        /* Chained assignment, right to left: the shared zero is evaluated once
           into al and reused by all four byte stores. */
        p->ent[i]->live = p->ent[i]->b0 = p->ent[i]->b5 = p->used[i] = 0;
        p->ent[i]->w12 = p->ent[i]->w16;
    }

    if (g_cur_a)
        FUN_1e25_0d95(g_cur_a);
    if (g_cur_b)
        FUN_1e25_0d95(g_cur_b);

    g_cur_a = 0;
    g_cur_b = 0;
    g_flag_flush = 0;
    FUN_1e25_043e(p);
}

/* ---- same-module callee placed after buf_to_vga: five byte call form ---- */

void FUN_1e25_0d95(char far *p) { p; }
