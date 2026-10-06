/* @target 0xFE80 */
/* @name   setcrtc */
/* @proto  void setcrtc(unsigned short x) */
/* @module cross */

/* Target 0xFE80 (40 bytes / 21 insns):
      push bp / mov bp,sp
      mov bl,0x0d / mov bh,[bp+6]        ; index 0x0d, data = low byte of x
      mov cl,0x0c / mov ch,[bp+7]        ; index 0x0c, data = high byte of x
      mov dx,0x3da / in al,dx / test al,1 / jne back   ; wait out h-retrace
      mov dx,0x3d4 / mov ax,bx / out dx,ax
      mov ax,cx / out dx,ax
      mov dx,0x3da / in al,dx / test al,8 / je back    ; wait for v-retrace
      pop bp / retf

   Two VGA CRTC registers written as words.  A word write to port 0x3d4 puts
   the low byte on the index port first and the high byte on the data port
   second, so each word packed here is (data << 8) | index.  Register 0x0d is the
   vertical display start and 0x0c the vertical display end, which is why the
   two halves of the argument are the two ends of one window rather than one
   value split arbitrarily.  The window is only safe to change while the display
   is off scan, hence the two trace waits bracketing the writes.

   Inline asm rather than C: both waits read the input status register and branch
   on one bit, and C has no way to branch on the result of an `in` without
   spilling a byte to memory between the read and the test that inspects it.

   Three details of this compiler's inline mode shape the source.  It accepts
   neither label definitions inside the asm block nor `$` for the location
   counter, so the two back edges land on C labels placed between blocks and the
   loop bodies open each block themselves.  It will not name the high byte of a
   register, so the argument's bytes are read straight off the frame the
   compiler has already set up.  And given `mov ax, bx` inside an asm block it
   assembles the store-direction form 89 /r, while the same transfer as a C
   assignment to _AX yields the load-direction 8b /r the target has -- hence
   _AX = _BX rather than the obvious `mov ax, bx`.
 */
void setcrtc(unsigned short x)
{
    asm {
        mov     bl, 0x0d
        mov     bh, byte ptr [bp+6]
        mov     cl, 0x0c
        mov     ch, byte ptr [bp+7]
        mov     dx, 03dah
    }
lw_h:
    asm {
        in      al, dx
        test    al, 1
        jne     lw_h
        mov     dx, 03d4h
    }
    _AX = _BX;
    asm {
        out     dx, ax
    }
    _AX = _CX;
    asm {
        out     dx, ax
    }
    _DX = 0x3da;
lw_v:
    asm {
        in      al, dx
        test    al, 8
        je      lw_v
    }
}