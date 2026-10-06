/* @target 0xF6DE */
/* @name   m_f44b */
/* @proto  int m_f44b(struct f44brec far *p, unsigned char v) */
/* @module cross */
/* @extra  xmod/xsetcrtc.c */

/* Target 0xF6DE (107 bytes / 40 insns):
      les bx,[bp+6]
      if (p->live == 0) goto early;
      n = random() % 5;              p->jitter = n * 0x50;
      if (p->countdown-- == 0) p->live = 0;
      setcrtc(g_crtc[v] + p->jitter);
      return;
   early:
      setcrtc(g_crtc[v]);

   So the record is a countdown: while it is live every frame the vertical
   display window is nudged by a random multiple of 0x50 scanlines, and on the
   frame the countdown expires the record is marked dead and no longer jitters.
   The two calls are the same routine with different arguments, so `v` only
   selects the parity word.

   The record layout is read off the offsets the target uses: a flag at +0, a
   countdown byte at +1, a word at +4.  The rest is not touched here.

   rand() is the C library's own generator, reached as a genuine far call into
   file segment 0 (target offset 0x803).  The multiplier is left inline in the
   store rather than hoisted into a local: naming it costs the sub sp,2 in the
   prologue and the pair of spill/fill instructions around the multiply, none of
   which the target has.  The function returns nothing meaningful -- the target
   falls straight out of both paths -- so the declared int is never assigned and
   the compiler warns about it, which is the same code the target shipped.
 */
int rand(void);

struct f44brec {
    unsigned char live;          /* +0 */
    unsigned char countdown;     /* +1 */
    unsigned char pad[2];        /* +2 */
    unsigned int jitter;         /* +4 */
};

/* Packed CRTC (index, data) pairs indexed by the frame parity v. */
unsigned int g_crtc[2];

void setcrtc(unsigned short x);

int m_f44b(struct f44brec far *p, unsigned char v)
{
    if (p->live) {
        p->jitter = (rand() % 5) * 0x50;
        if (p->countdown-- == 0)
            p->live = 0;
        setcrtc((unsigned short)(g_crtc[v] + p->jitter));
    } else {
        setcrtc(g_crtc[v]);
    }
}