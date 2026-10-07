/* @target 0x3D72 */
/* @name   m_0252 */
/* @proto  void __far m_0252(void) */
/* @module same */
/* @extra  xmod/xb115d.c xmod/xb11056.c xmod/xfd3.c g13b2.c */

/* FUN_13b2_0252, 103 bytes / 35 instructions.  Called by m_013b at 0x3D72, at
   the top of its g_27ff body.
 *
 * Target:
 *   3D72  push bp / mov bp,sp / sub sp,4
 *   3D78  cmp byte ptr [0x27fd], 0
 *   3D7D  jne 0x3DD7
 *   3D7F  mov byte ptr [0x27fd], 1
 *   3D84  push ds / push 0x8d4 / push word ptr [0x34af] / push word ptr [0x34ad]
 *   3D90  lcall 0xb11, 0x15d6      ->  add sp, 8
 *   3D98  mov word ptr [bp-2], dx
 *   3D9B  mov word ptr [bp-4], ax
 *   3D9E  or ax, word ptr [bp-2]
 *   3DA1  jne 0x3DAF
 *   3DA3  push ds / push 0x8de
 *   3DA7  lcall 0xb11, 0x56        ->  add sp, 4
 *   3DAF  push 0x140 / push word ptr [0x19e4] / les bx,[bp-4]
 *   3DB9  push word ptr es:[bx+0x28] / push word ptr es:[bx+0x26]
 *   3DC1  push 0xa6 / push 0xbe / push 8 / push 0x10 / push 0 / push 0
 *   3DCF  lcall 0xfd3, 0x12        ->  add sp, 0x14
 *   3DD7  leave / retf
 *
 * Two readings that pin down the signatures:
 *
 * - `push ds / push 0x8d4` is a far pointer argument (segment, then offset --
 *   Borland's order).  `push word ptr [0x34af] / push word ptr [0x34ad]` is a
 *   second far pointer argument, taken from the global whose offset word lives
 *   at 0x34AD and segment word at 0x34AF, i.e. g_34AD.  Two four-byte arguments
 *   is exactly the `add sp, 8` after the call.
 *
 * - `mov [bp-2], dx / mov [bp-4], ax` is Borland spilling a returned `char far *`
 *   (returned in AX:DX) to the frame, and `les bx, [bp-4]` reads it straight back.
 *   The `sub sp,4 / leave` frame exists purely to hold that far pointer, so the
 *   two arguments of FUN_1fba_0002-style calls at [bx+0x26] and [bx+0x28] are
 *   fields of the structure the callee handed back.
 *
 * `or ax, dx / jne` is the null test on that pointer.  Both arms converge on the
 * 0x3DAF block, so FUN_1b11_0056 is a fallback, not an early return.
 *
 * Not reconstructed: the four far callees.  See src/xmod/.
 */

extern char far *g_34ad;
extern unsigned char g_27fd;
extern unsigned int  g_19e4;

/* Segment 0xb11.  FUN_1b11_15d6 returns a far pointer to a structure whose word
   fields at +0x26 and +0x28 are forwarded to FUN_1fd3_0012.  The return has to
   be a *structure* pointer, not `char far *`: the target pushes
   `word ptr es:[bx+0x28]` and `word ptr es:[bx+0x26]`, so the fields are words. */
struct devinfo { char pad[0x26]; int f26; int f28; };
struct devinfo far *FUN_1b11_15d6(char far *rec, char far *name);
void FUN_1b11_0056(char far *msg);

/* Segment 0xfd3.  Ten word arguments: `add sp, 0x14`. */
void FUN_1fd3_0012(int a, int b, int c, int d, int e,
                   int f, int g, int h, int i, int j);

/* DS:0x8D4 and DS:0x8DE are string literals in this module's data segment.
   Their addresses are masked by diffasm, so only their presence as far pointers
   matters for the byte comparison; the text is filled in from the target dump
   when the 0xb11 call sites are reconstructed.  Borland pushes a near literal
   converted to `char far *` as `push ds / push offset`, which is why the target
   shows `push ds / push 0x8d4` rather than two `push word ptr`. */
static char lit_8d4[] = "vga";
static char lit_8de[] = "vga";

void m_0252(void)
{
    struct devinfo far *r;

    if (g_27fd)
        return;
    g_27fd = 1;

    /* Borland pushes arguments right to left, so the literal (pushed first,
       highest on the stack) is the *second* parameter and g_34AD the first. */
    r = FUN_1b11_15d6(g_34ad, lit_8d4);
    if (!r)
        FUN_1b11_0056(lit_8de);

    FUN_1fd3_0012(0, 0, 0x10, 8, 0xbe, 0xa6,
                  r->f26, r->f28, g_19e4, 0x140);
}
