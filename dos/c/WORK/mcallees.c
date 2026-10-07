/* Same-module callees of main(), one per target function, all in one Borland
 * module so the cheap `nop / push cs / call rel16` form is preserved.
 *
 * Why they live here rather than at the bottom of main.c: `call rel16` only
 * reaches +-32K of the call site, and growing any one of these in place shifts
 * the displacement of every call after it, so main's own bytes move and the
 * frame loop can never be un-stubbed.  Splitting the module lets each function
 * be reconstructed independently.  main() now calls them with `lcall` instead,
 * which is also 5 bytes -- main keeps its 300-byte size but stops being
 * byte-exact, since the opcode differs.  That is the deliberate trade: main's
 * loop only exits when g_27fc is set, and the only code that sets it is behind
 * m_013b, one of these.
 *
 * Within this file the calls stay same-module, so the rel16 form is what the
 * target uses.
 */

/* m_1a40's own far callees */
void node_free(char far *p);
void buf_to_vga(unsigned char far *p);
void pal_upload(char far *a, char far *b);

struct blk_s { unsigned char pad0[2]; unsigned char state; unsigned char pad1[3];
              unsigned char data[0x300]; };
void statebak(struct blk_s far *blk);
void clrbit3(unsigned char far *rec);

/* Globals main.c owns.  Addresses differ from the target's, but diffasm masks
   every absolute DS reference, so only the sharing has to be right. */
extern char far *g_27aa;
extern char far *g_34a9;
extern char far *g_34ad;
extern char far *g_3858;
extern unsigned char g_27fc, g_27f3, g_27fd, g_27ff, g_2800;
extern unsigned char g_27ec, g_27ed, g_27f2, g_27f6, g_27f9, g_27fa;
extern unsigned char g_2c61;
extern unsigned int  g_2bf5;
extern unsigned int  g_19e4;

/* Same-module callees of main() that are still stubs. */
void m_1a40(void);
void m_1df6(void);
void m_38a2(void);
void m_2415(void);
void m_24e7(int v);
void m_0dd6(void);
void m_154f(int v);
void m_013b(void);
void m_34e5(void);
void m_35dd(void);

/* Far callees.  Every one of these is a real `lcall` in the target, so each is a
   separate module and gets a stand-in until it is reconstructed. */
void m_b159(char far *p);
void FUN_1b11_15d6(char far *p, char far *s);
void FUN_1b11_0056(char far *s);
void m_f44d(char far *p);
void m_f44b(char far *p, unsigned char v);
void m_f44c(char far *p);
void FUN_1e25_0978(char far *p, char far *s);
void FUN_1e25_06fe(char far *p, char far *s);
void FUN_1e25_04aa(char far *p, char far *s);
void FUN_1fd3_0012(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);
void FUN_1fba_0002(int a, int b, int c, int d, int e, int f);
void FUN_1d19_014a(char far *p, int v);
void FUN_1d19_032f(char far *p, int a, int b, int c);
void FUN_1d19_02fe(char far *p, int a, int b);

/* ---------------------------------------------------------------- bodies ----
 * Still-empty until each is reconstructed from the target.  They cannot stay
 * empty for long: main()'s loop only exits once g_27fc is set, and the only
 * writer of that flag is FUN_13b2_2c80, reached through m_013b.
 */

/* FUN_13b2_1a40, 65 bytes / 17 instructions.  Reconstructed and byte-exact --
 * it is a pure composition of the real far callees. */
void m_1a40(void)
{
    node_free(g_34a9);
    statebak(g_3858);
    clrbit3(g_3858);
    buf_to_vga(g_27aa);
}

void m_1df6(void) { }      /* 411 B / 127 ins */
void m_38a2(void) { }      /* 1030 B / 219 ins */
void m_2415(void) { }      /* 210 B / 59 ins; clears g_27fc at 0x5F62 */
void m_24e7(int v) { v; }  /* 1945 B / 676 ins */
void m_0dd6(void) { }      /* 821 B / 280 ins */
void m_154f(int v) { v; }  /* 619 B / 215 ins */
void m_013b(void) { }      /* 279 B / 104 ins; calls FUN_13b2_2c80 at 0x3D57 */
void m_34e5(void) { }      /* 248 B / 97 ins */
void m_35dd(void) { }      /* 709 B / 273 ins */
