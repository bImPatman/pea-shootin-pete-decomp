/* Shared data segment for the segment-0x3b2 reconstructions.
 *
 * Borland's large model puts every .c file in its own *code* segment, but all of
 * them share one DGROUP for data, so a global defined here is the same storage
 * whichever module reads it.  In PETE.EXE these live at DS:0x27FC, 0x27FD, ...
 * which is inside main()'s module data, so diffasm masking of absolute DS
 * operands is what lets the addresses differ without breaking the comparison.
 *
 * Why this exists as a module rather than `extern` declarations alone: Turbo Link
 * 5.1 runs away (multi-megabyte .MAP) when a far call's module references an
 * extern symbol that no module defines.  So a checked reconstruction whose body
 * both calls a far function and reads a global needs *two* `@extra` modules: one
 * for the callee, one for the storage.  See tools/py/check.py `extra_sources`.
 */

/* +0x27FC: set by FUN_13b2_2c80 at 0x687D and 0x6AA4; this is the flag main()'s
   frame loop spins on, so nothing exits the loop until 2c80 is reconstructed. */
unsigned char g_27fc;
/* +0x27FD: one-shot latch; FUN_13b2_02b9 tests it, does its far call, clears it. */
unsigned char g_27fd;

/* +0x27FF: m_013b's outer gate; when zero the whole body is skipped. */
unsigned char g_27ff;

/* +0x27F3: set by m_013b once g_27fc is seen, so the tail path can tell the
   difference between "still drawing" and "screen is up". */
unsigned char g_27f3;

/* +0x2800: tested by m_013b alongside the g_34a1->field_65 bit test. */
unsigned char g_2800;

/* +0x27F2, +0x27F6, +0x2C61: compared by m_013b's middle section. */
unsigned char g_27f2;
unsigned char g_27f6;
unsigned char g_2c61;

/* +0x27EC / +0x27ED: m_013b scales g_27ed by 0x34 to index a jump table and
   does `lcall far ptr [bx+0x74e]`, i.e. an indirect call through a table of
   segment:offset pairs living in the record at g_34a1. */
unsigned char g_27ec;
unsigned char g_27ed;

/* +0x27F9 / +0x27FA: written by the same block as g_27f3. */
unsigned char g_27f9;
unsigned char g_27fa;

/* +0x2BF5: m_013b increments this and calls FUN_13b2_02b9 when the old value
   was 0xFF, so it wraps once every 256 frames. */
unsigned int g_2bf5;

/* +0x19E4: fifth argument to FUN_1fba_0002 from m_02b9. */
unsigned int g_19e4;

/* +0x34AD: offset word, with the segment word at +0x34AF.  m_0252 pushes both
   words to pass it as a far pointer to FUN_1b11_15d6, which is why the target
   shows two `push word ptr` rather than `push ds`. */
char far *g_34ad;

/* Owned here rather than in main.c so each shared global has exactly one
   definition; main.c and mcallees.c both refer to them. */

/* +0x349E / +0x349F: frame parity.  main() flips one into the other each frame
   and mcallees' frame body copies g_349E through g_349F. */
unsigned char g_349e, g_349f;

/* +0x27AA: the picture buffer main() hands to m_e256 and buf_to_vga. */
char far *g_27aa;

/* +0x34A9: the node list main() walks and hands to m_b11a and node_free. */
char far *g_34a9;

/* +0x3858: the 0x308-byte record that m_f44b, m_f44c, m_f44d and clrbit3 all
    operate on.  src/run/stubs.c points it at a zeroed copy so the hardware
    register writes stay away from real devices. */
char far *g_3858;

/* m_2415's initialisation block.  FUN_13b2_2415 writes all of these in its
    header (0x5F35..0x5F6B) before the six far calls, so every one has to have
    storage here or TLINK runs away on the undefined reference. */
unsigned int  g_168c;          /* +0x168C: set to 0xBB80, a 48000-hertz-ish divisor */
unsigned int  g_2c01, g_2bff;  /* +0x2C01 / +0x2BFF: cleared, then tested by m_013b */
/* +0x27AE..+0x27B1: one far pointer, segment word at 0x27AE and offset word at
    0x27B0.  FUN_1b11_1106 returns it in DX:AX (segment, offset) and m_2415
    stores DX (segment) at the low word. */
char far *g_27ae;
unsigned char g_2bf0, g_27ee, g_27f0, g_27f1, g_27f4;
unsigned char g_27f7;
unsigned char g_27fe;

/* +0x27E6 / 0x27E7 / 0x27EF / 0x27FB / 0x2803 / 0x2805: cleared by m_24e7 / tested
   by m_2c80.  m_2c80 sets g_27f3 when g_2805 is nonzero. */
unsigned char g_27e6, g_27e7, g_27ef, g_27fb, g_2803, g_2805;
unsigned char g_2804, g_2807;

/* +0x27E2/+0x27E4: copied by m_24e7 from the dispatch row's sub[g_27ec]; the
   else-branch then draws from it.  +0x27F5/+0x27F8: m_24e7 derives g_27f8 from
   the second object byte.  +0x2801/+0x2802: cleared by m_24e7. */
char far *g_27e2;
unsigned char g_27f5, g_27f8;
unsigned char g_2801, g_2802;

/* +0x1690 / +0x18F8: m_1df6 clears one, sets the other; m_2c80 branches on g_18f8
   and spins on g_18fa. */
unsigned char g_1690, g_18f8;
unsigned char g_18fa;

/* +0x2C60: mode flag compared against 0 and 1 by m_1df6 / m_2c80 / m_24e7. */
unsigned char g_2c60;

/* +0x2C2F/+0x2C31: far pointer returned by FUN_1b11_15d6 in m_1df6. */
char far *g_2c2f;

/* +0x2BFB / +0x2BFD: words copied from the object behind g_2c2f (m_1df6). */
unsigned int g_2bfb, g_2bfd;

/* +0x34A1/+0x34A3: an object created by FUN_1b11_1dc1 in m_24e7 / m_2c80, holding
   the virtual-call tables those functions dispatch through. */
char far *g_34a1;

/* The DS:0x73E dispatch table that m_013b indexes with g_27ed (stride 0x34).
    The row really starts at DS:0x73A with five far payload pointers, then the
    far function pointer at +0x14 (0x74E), then a far-pointer array `sub[6]` at
    +0x18 (0x752) that m_24e7 strcmps between, then 4 pad bytes.  fn() compiles
    to `imul dx,0x34; lcall [bx+disp]`, and a variable `sub[i]` indexes stride 4
    from +0x752.
 *
    The seventh slot matters.  m_24e7 reads sub[g_27ec] with the counter running
    0..7, and 6 falls past the six declared entries onto the four bytes the
    decompiler recorded as `pad` -- so those four bytes are really one more far
    pointer, and the struct is spelled `sub[7]` here to say so.  mcallees.c and
    src/m24e7.c keep the sub[6]+pad[4] spelling, which lays out identically, so
    their verified bytes do not move.
 */
struct dispatch_entry {
    char far *p_73a;        /* +0x00 */
    char far *p_73e;        /* +0x04 */
    char far *p_742;        /* +0x08 */
    char far *p_746;        /* +0x0c */
    char far *p_74a;        /* +0x10 */
    void (far *fn)(void);   /* +0x14 */
    char far *sub[7];       /* +0x18 */
};

struct obj742 {
    char far *f0, *f4, *f8, *fc, *f10, *f14;   /* +0x00 */
    unsigned char pad[0x1a];                    /* +0x18 */
    void (far *fn32)(void);                     /* +0x32 */
};
struct obj_e2 {
    unsigned char b0f[0xf];                     /* +0x00 */
    char far *f0f;                              /* +0x0f */
    char far *f13;                              /* +0x13 */
    unsigned char b17;                          /* +0x17 */
    void (far *fn18)(void);                     /* +0x18 */
};

/* Nothing reconstructed writes this table yet: the target fills DS:0x73A from
   FUN_13b2_38a2, which is still an empty stand-in, so the table arrives as
   zeroes.  m_013b's `lcall [bx+0x74E]` and m_24e7's `->fn18()` both dereference
   it unconditionally on the first frame, and a null segment lands in the
   interrupt vector table.  The rows below are the smallest shape those two call
   sites accept: readable strings for the strlen/strcpy sources, one obj742 for
   the g_27ec%7 branch, seven obj_e2 for sub[0..6], and a no-op behind every far
   code pointer.  They are stand-ins for data, not a reconstruction of it.
 */
static void dispatch_nop(void) { }

static char d_title[]  = "Scene 00";
static char d_name[]   = "Sub 00";
static char d_key[]    = "k";
static char d_caption[] = "Cap";
static char d_blank[]  = "";
static char d_text[]   = "........";

static struct obj742 d_obj742 = {
    d_text, d_text, d_text, d_text, d_text, d_text, {0}, dispatch_nop
};
static struct obj_e2 d_e2[7] = {
    { {0}, d_text, d_blank, 0, dispatch_nop },
    { {0}, d_text, d_blank, 0, dispatch_nop },
    { {0}, d_text, d_blank, 0, dispatch_nop },
    { {0}, d_text, d_blank, 0, dispatch_nop },
    { {0}, d_text, d_blank, 0, dispatch_nop },
    { {0}, d_text, d_blank, 0, dispatch_nop },
    { {0}, d_text, d_blank, 0, dispatch_nop }
};

#define D_ROW { d_title, d_name, &d_obj742, d_key, d_caption, dispatch_nop, \
                { &d_e2[0], &d_e2[1], &d_e2[2], &d_e2[3], &d_e2[4], \
                  &d_e2[5], &d_e2[6] } }
struct dispatch_entry g_dispatch[8] = {
    D_ROW, D_ROW, D_ROW, D_ROW, D_ROW, D_ROW, D_ROW, D_ROW
};

/* +0x2C33: 7-byte scratch copy area; +0x2C56: string scratch.  m_24e7 and m_2c80
   fill these and pass them by address to FUN_1000_28ef / FUN_1b11_034c. */
unsigned char g_2c33[7];
unsigned char g_2c56[0x20];

/* +0x19E6/+0x19E8: far pointer consumed by the 10-arg FUN_1ffe_000e from 2c80/24e7. */
char far *g_19e6;

