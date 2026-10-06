/* @target 0xF61E */
/* @name   m_f44d */
/* @proto  void m_f44d(char far *p) */
/* @module cross */
/* @extra  xmod/xshift.c */

/* Target 0xF61E (152 bytes / 27 insns):
      push bp / mov bp,sp / sub sp,0xa / les bx,[bp+6]
      cmp byte es:[bx+2],0 / jne L_then / jmp L_end
   L_then:
      mov al,[0x3853] / dec byte [0x3853] / mov ah,0 / or ax,ax / jne L_end
      mov word [bp-8],ds / mov word [bp-0xa],0x3850 / les bx,[bp-0xa]
      mov al,es:[bx+2] / mov es:[bx+3],al
      push ds / push 0x3550 / push es:[bx+6] / push es:[bx+4]
      lcall 0xfa6:0x2f / add sp,8
      leave / retf
   L_end: leave / retf

   Two exits, which is the tell for an early `return` inside the taken path:
   Borland emits the fall-through epilogue inline and a second copy at the
   function's own end that both `jne L_end` sites share.

   The decrement lands on 0x3853, which is byte 3 of the record at 0x3850 - the
   very byte the taken path then overwrites with byte 2.  So the gate is a
   countdown that the body then resets from its companion field.

   The callee at 0xfa6:0x2f reads three arguments at [bp+6], [bp+8] and [bp+0xa]
   and pops none of them, so `add sp,8` means Borland was told 8 bytes of
   arguments: 2 + 2 + 4.  The four pushes are two words and one far pointer, and
   the pair that must stay adjacent is `push ds / push 0x3550` - a far pointer to
   g_3550, pushed segment first then offset so the offset lands at the lower
   address.  Reading the callee's slots back off the stack in that order:

      [bp+6]  last pushed word  = rec[+4]
      [bp+8]  second-to-last    = rec[+6]
      [bp+0xa] far pointer      = g_3550

   so the call is a three-argument helper taking the buffer first, and the two
   words come off the 0x3850 record at +6 and +4 in that order.  The callee then
   computes di = segment(g_3550) + [bp+8] and walks backwards from there, i.e.
   it slides the palette bytes in [rec[+4], rec[+6]) down to rec[+4].
 */
struct node {
    unsigned char pad0[2];
    unsigned char a, b;
    unsigned int w4, w6;
};

struct node g_3850;
unsigned char g_3550[0x300];
unsigned char g_3853;         /* byte 3 of the 0x3850 record, counted down */

/* Slides the palette range [to, from) down to `to` inside `buf`. */
void pal_shift(unsigned int to, unsigned int from, char far *buf);

void m_f44d(char far *p)
{
    /* `x` pads the frame out to the ten bytes the target reserves, and `q` is
       kept in a stack slot rather than folded into an immediate segment:offset
       pair: the target reloads it with `les` and then addresses es:[bx+n]. */
    struct { struct node far *q; char x[6]; } v;

    if (p[2]) {
        if (--g_3853 == 0) {
            v.q = &g_3850;
            v.q->b = v.q->a;
            pal_shift(v.q->w4, v.q->w6, g_3550);
            return;
        }
    }
}
