/* @target 0xC676 */
/* @name   FUN_1b11_1566 */
/* @proto  char far *FUN_1b11_1566(char far *p) */
/* @module cross */
/* @extra  xmod/x0778.c */

/* Target 0xC676 (49 bytes / 18 insns):
      push bp / mov bp,sp
      mov ax,[bp+6] / or ax,[bp+8] / jne already   ; p != 0 ?
      push 0x191 / lcall 0,0x778 / add sp,2
      mov [bp+8],dx / mov [bp+6],ax               ; p = allocate(0x191)
      or ax,dx / je done                          ; still null -> skip
   already:
      les bx,[bp+6] / mov byte es:[bx+0x190],0    ; p->byte_190 = 0
   done:
      mov dx,[bp+8] / mov ax,[bp+6]               ; return p
      pop bp / retf

   Allocate-or-reuse: if the far pointer is null, take 0x191 bytes from the bump
   allocator (FUN_1000_0778, another module) and, if that succeeds, clear the byte
   at +0x190.  The pointer is returned either way.  This is the idiom-2
   return-merge shape from notes/compiler.md: the `or ax,[bp+8]` null test, the
   store-back of the allocation into the parameter slots, and the shared epilogue
   that reloads the pair into DX:AX.

   Linked into the game in place of the src/xmod/xb111566.c stand-in; the caller
   (m_1df6) keeps the same symbol name, so only build_game.py's CORE list changes. */

extern char far *FUN_1000_0778(int n);

char far *FUN_1b11_1566(char far *p)
{
    if (p != 0 || (p = FUN_1000_0778(0x191)) != 0)
        p[0x190] = 0;
    return p;
}
