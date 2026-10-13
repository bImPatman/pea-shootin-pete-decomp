/* @target 0xF61E */
/* @name   m_f44d */
/* @proto  void m_f44d(char far *p) */
/* @module cross */
/* @extra  xmod/xshift.c xmod/xupload.c xmod/xg3550.c */

/* Target 0xF61E (152 bytes / 50 insns).  The whole reachable body lives in the
   first 72 bytes and ends in an inline `leave / retf`; everything after it up to
   the shared epilogue at 0xF6B4 is a *dead* copy of the palette-slide + DAC
   upload that BCC 3.1 still emits because the source has statements after the
   taken path's `return`.  The dead half has no prologue of its own: it reuses
   the live half's frame (the loop index is [bp-2], the three RGB bytes are
   [bp-6..-4]) and is entered only by falling out of the live half's epilogue,
   which is impossible, so nothing ever executes it.  It is reproduced here
   purely so the 152 bytes line up.

      push bp / mov bp,sp / sub sp,0xa / les bx,[bp+6]
      cmp byte es:[bx+2],0 / jne L_then / jmp L_end
   L_then:
      mov al,[0x3853] / dec byte [0x3853] / mov ah,0 / or ax,ax / jne L_end
      mov word [bp-8],ds / mov word [bp-0xa],0x3850 / les bx,[bp-0xa]
      mov al,es:[bx+2] / mov es:[bx+3],al
      push ds / push 0x3550 / push es:[bx+6] / push es:[bx+4]
      lcall 0xfa6:0x2f / add sp,8
      leave / retf
   L_dead:
      mov bx,[bp-2]
      mov al,[bx+0x354d] / mov [bx+0x3550],al / dec [bp-2]
      les bx,[bp-0xa] / mov ax,es:[bx+4] / cmp ax,[bp-2] / jne L_dead
      mov bx,es:[bx+4]
      mov al,[bp-6] / mov [bx+0x3550],al
      mov bx,[bp-0xa] / mov bx,es:[bx+4] / mov al,[bp-5] / mov [bx+0x3551],al
      mov bx,[bp-0xa] / mov bx,es:[bx+4] / mov al,[bp-4] / mov [bx+0x3552],al
      push ds / push 0x3550 / lcall 0xfa6:0x6a / add sp,4
   L_end:
      leave / retf

   `q` has to be a *plain* far-pointer local, not a member of a struct: BCC
   folds a struct member's pointer to its static offset and emits `mov es,[bp-n]`
   plus `es:g_3850+n`, whereas a bare `struct node far *q` is reloaded whole with
   `les bx,[bp-0xa]` and addressed `es:[bx+n]`, which is what the target does.
   `x[4]`, `i` and `t` exist only to reserve the frame; declaration order fixes
   the slots: i at [bp-2], x at [bp-6..-3], q at [bp-0xa..-7].  The guard has to
   be the assignment form `(t = g_3853--) != 0`, not the bare `g_3853-- != 0`:
   only the assignment forces BCC to materialise the `unsigned char` value as a
   16-bit int, which is the `mov ah,0 / or ax,ax` pair the target has.  It is
   still just a `!= 0` test, so `t` itself is dead and never gets its own slot. */

struct node_f44d {
    unsigned char pad0[2];
    unsigned char a, b;
    unsigned int w4, w6;
};

struct node_f44d g_3850;
extern unsigned char g_3550[0x300];   /* owned by dacupd.c in the linked game */
unsigned char g_3853;

void pal_shift(unsigned int to, unsigned int from, char far *buf);
void upload(char far *buf);

void m_f44d(char far *p)
{
    int i, t;
    char x[4];
    struct node_f44d far *q;

    if (p[2] == 0)
        return;
    if ((t = g_3853--) != 0)
        return;
    q = &g_3850;
    q->b = q->a;
    pal_shift(q->w4, q->w6, (char far *)g_3550);
    return;

    for (i = q->w6; i != q->w4; i--)
        g_3550[i] = g_3550[i - 3];
    g_3550[q->w4] = x[0];
    g_3550[q->w4 + 1] = x[1];
    g_3550[q->w4 + 2] = x[2];
    upload((char far *)g_3550);
}
