/* @target 0x10C1 */
/* @name   farstreq */
/* @proto  int __pascal __near farstreq(char far *s, char far *t) */
/* @module same */

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
   post-increment: `*s++`.

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
   point jumps forward to it; that is the leading `jmp 0x10de`.  The mismatch
   path jumps *past* `mov ax,1` rather than jumping to a shared epilogue, which
   is what the two-separate-`return` form produces.

   Worth doing early for a second reason: this is the first target here with a
   loop, so unlike flagbits_set it can actually discriminate -O from -O1 from
   -O2. */

int __pascal __near farstreq(char far *t, char far *s)
{
    while (*s) {
        if (*s++ != *t++) {
            return 0;
        }
    }
    return 1;
}

void main(void) { farstreq((char far *)0, (char far *)0); }
