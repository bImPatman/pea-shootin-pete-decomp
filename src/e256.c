/* @target 0xE4A6 */
/* @name   m_e256 */
/* @proto  int m_e256(char far *p) */
/* @module cross */
/* @extra  xmod/xe161d.c */

/* Target 0xE4A6 -- FUN_1e25_0256 (334 bytes / 110 insns).
 *
 * Liveness scan over the record at p.  The record is a parallel pair of
 * arrays that share one index: a far-pointer array of 20 entries at +0x00 and
 * a byte "still live" flag array at +0x50, with the entry count as a byte at
 * +0x109 (0x50 + 0xB9).  Both cursors are strength reduced off `i` and stepped
 * at the bottom of the body -- one byte, four bytes -- which is why the two
 * locals are kept as running offsets rather than re-indexed.
 *
 * The outer pass only touches live entries.  Each live entry is handed to the
 * same-module routine at 0xF2AB, and if that leaves a non-zero byte at +4 of
 * the entry the same-module routine at 0xF02E runs too, the flag is cleared,
 * and -- only when the entry is the one currently pointed at by g_cur -- the
 * table is rescanned for the live entry with the highest byte at +3.  If that
 * rescan finds nothing better than zero, g_cur is dropped instead.
 *
 * Both same-module callees are declared before use but defined after m_e256:
 * a forward reference gets the five byte nop / push cs / call rel16 form,
 * which is what the image uses.  A cross-module call would be an lcall.
 */
struct ent {
    unsigned char pad[3];
    unsigned char score;         /* +3, the ranking key */
    unsigned char live;          /* +4, set by the 0xF2AB pass */
};

struct e256 {
    struct ent far *ent[20];      /* +0x00 */
    unsigned char used[0xB9];     /* +0x50 */
    unsigned char count;          /* +0x109 */
};

/* DS:0x18F8 enables the whole pass, 0x18F9 is the retry flag, and 0x3548 is
   the far cursor the table is searched against.  Note that 0x3548/0x354C are
   the *same* two globals buf_to_vga flushes; Borland's large model puts every
   .c file in its own data segment, so src/buftovg.c renames its copies to
   g_cur_a / g_cur_b to keep both modules linkable. */
extern unsigned char g_18f8;   /* owned by src/g13b2.c (shared with m_1df6/m_2c80) */
unsigned char g_18f9;
char far *g_cur;

/* cross module: FUN_161d_08de */
int FUN_161d_08de(void);

/* same module, defined after m_e256 so the calls stay five bytes wide */
void FUN_1e25_105b(char far *p);
void FUN_1e25_0dde(char far *p);

void m_e256(struct e256 far *p)
{
    int i, j, best;

    if (!g_18f8)
        return;

    if (g_18f9) {
        if (!FUN_161d_08de())
            g_18f9 = 0;
    }

    for (i = 0; i < p->count; i++) {
        if (p->used[i]) {
            FUN_1e25_105b(p->ent[i]);

            if (p->ent[i]->live) {
                FUN_1e25_0dde(p->ent[i]);
                p->used[i] = 0;

                if (p->ent[i] == g_cur) {
                    best = 0;
                    for (j = 0; j < p->count; j++) {
                        if (p->used[j]) {
                            if (p->ent[j]->score > best) {
                                g_cur = p->ent[j];
                                best = p->ent[j]->score;
                            }
                        }
                    }

                    if (best == 0)
                        g_cur = 0;
                }
            }
        }
    }
}

/* ---- same-module callees, forward references on purpose ---- */

void FUN_1e25_105b(char far *p) { p; }
void FUN_1e25_0dde(char far *p) { p; }
