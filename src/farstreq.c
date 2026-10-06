/* @target 0x10C1 */
/* @name   farstreq */
/* @proto  int __pascal __near farstreq(char far *t, char far *s) */
/* @module same */
/* @flags  -O1 */

/* Target 0x110C1:
     push bp
     mov  bp, sp
     jmp  0x10de                <- rotated loop: test at the bottom
     les  bx, ptr [bp + 4]
     inc  word ptr [bp + 4]
     mov  al, byte ptr es:[bx]
     les  bx, ptr [bp + 8]
     inc  word ptr [bp + 8]
     cmp  al, byte ptr es:[bx]
     je   0x10de
     xor  ax, ax
     jmp  0x10ea
     les  bx, ptr [bp + 4]
     cmp  byte ptr es:[bx], 0
     jne  0x10c6
     mov  ax, 1
     pop  bp
     ret  8

   Three things worth reading off this.

   Borland lays a far pointer out offset-then-segment in the argument slot, so
   `inc word ptr [bp + 4]` walking the *argument itself* is just `s++`.  The
   slot is loaded and incremented before the byte is read, so the source is
   post-increment: `*s++`.  `__pascal` pushes arguments right to left, so the
   *last* declared parameter lands in the lowest slot -- hence `s` is declared
   second.

   `ret 8` is the callee popping 8 bytes -- two 4-byte far pointers -- so this is
   `__pascal`, not cdecl.  With cdecl Borland emits a bare `ret`.

   `__near` is what puts the arguments at [bp+4] rather than [bp+6].  Borland
   normally reserves a word at [bp+4] for the segment half of a *far* return
   address, which shifts the first argument to [bp+6]; declaring the function
   `__near` drops the reservation.  That single keyword accounts for the whole
   split in the binary -- 27 of 29 near-returning functions read their first
   argument at [bp+4], against 256 of 266 far-returning ones at [bp+6].
   See the frame-layout section of notes/compiler.md.

   Borland rotates `while` loops so the test sits at the bottom and the entry
   point jumps forward to it; that is the leading `jmp 0x10de`.

   WHY @flags -O1 INSTEAD OF THE GLOBAL -O2
   -----------------------------------------
   The `xor ax,ax` / `jmp 0x10ea` pair is a *merged epilogue*: both exits jump
   to one shared `pop bp / ret 8`.  Under 3.1's -O2 the compiler instead emits
   `pop bp / ret 8` twice, giving 47 bytes instead of 45.  -O and -O1 both merge
   it, so this function alone cannot tell those two apart.

   That matters because -O2 also cannot be the answer for the binary as a
   whole: -O2 is the *only* 3.1 setting that allocates frames with
   `sub sp,N` / `leave`, and PETE.EXE has 100 such functions and not one `enter`.
   -O and -O1 emit `enter` for every framed function, with no source-level
   workaround.  So the two halves of the evidence are mutually exclusive under
   3.1: 100 functions want -O2's frame idiom, 30 frameless multi-return
   functions want -O/-O1's epilogue merging, and no single 3.1 flag set does
   both.  Together with the constant-shift divergence in flagshft.c that is
   strong evidence PETE.EXE was not built with this exact compiler.  -O2 stays
   the global default because it wins on function count (100 > 30); this file
   overrides it via @flags so the reconstruction can still be checked. */

int __pascal __near farstreq(char far *t, char far *s)
{
    while (*s) {
        if (*s++ != *t++) {
            return 0;
        }
    }
    return 1;
}