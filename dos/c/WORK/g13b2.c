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
/* +0x27AE..+0x27B1: one far pointer, offset word at 0x27AE and segment word at
    0x27B0.  FUN_1b11_1106 returns it in DX:AX and m_2415 stores DX first. */
char far *g_27ae;
unsigned char g_2bf0, g_27ee, g_27f0, g_27f1, g_27f4;
unsigned char g_27f7;
unsigned char g_27fe;

/* +0x27E6 / 0x27E7 / 0x27EF / 0x27FB / 0x2803 / 0x2805: cleared by m_24e7 / tested
   by m_2c80.  m_2c80 sets g_27f3 when g_2805 is nonzero. */
unsigned char g_27e6, g_27e7, g_27ef, g_27fb, g_2803, g_2805;
unsigned char g_2804, g_2807;

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

/* The DS:0x74E dispatch table that m_013b indexes with g_27ed (stride 0x34):
   each entry is a far function pointer followed by 0x30 bytes of payload, so
   `g_dispatch[g_27ed].fn()` compiles to `imul dx,0x34; lcall [bx+disp]`. */
struct dispatch_entry { void (far *fn)(void); unsigned char pad[0x30]; };
struct dispatch_entry g_dispatch[8];

/* +0x2C33: 7-byte scratch copy area; +0x2C56: string scratch.  m_24e7 and m_2c80
   fill these and pass them by address to FUN_1000_28ef / FUN_1b11_034c. */
unsigned char g_2c33[7];
unsigned char g_2c56[0x20];

/* +0x19E6/+0x19E8: far pointer consumed by the 10-arg FUN_1ffe_000e from 2c80/24e7. */
char far *g_19e6;

