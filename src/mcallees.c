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
