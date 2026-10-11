/* @target 0xCBA4 */
/* @name   m_b11a */
/* @proto  int m_b11a(char far *p) */
/* @extra  g13b2.c xmod/x0ffe0e.c xmod/xfd3.c xmod/xfc2006.c xmod/xd1968d.c xmod/xd1973e.c */

/* FUN_1b11_1a94, 813 bytes / 261 instructions.  Segment 0xb11.  Called by
   main() once per frame with g_34a9 and by FUN_1b11_1dc1.
 *
 * p is a far pointer to a list head: a one byte count at +0 and an array of
 * four byte far pointers to objects starting at +2 (word aligned).  Three
 * passes over the list:
 *
 *   1. per-object draw: optional vtable call at obj+0x51, an 0xffe:000e blit
 *      from the coords at +0x13..+0x19, then two calls into segment 0xd19.
 *   2. objects flagged at bit 5 of +0x65 either fall into a same-module helper
 *      (FUN_1b11_1ed3, defined below) when bit 6 is also set, or just get bit 6
 *      set.
 *   3. objects with bit 5 of +0x65 clear are drawn either via a table behind
 *      +0x73 (when +0x30 is set) or a table behind +0x67.
 *
 * BCC 3.1 compiles each `p->item[i]` read as `<es>=[bp+8]; bx=[walker];
 * les bx, es:[bx+2]` with a per-loop walker that starts at p and advances by 4,
 * so the source must read `p->item[i]` at every use rather than caching it.
 *
 * The flag tests at +0x65/+0x66 are expressed as a bitfield overlay: BCC emits
 * `mov al,[x] / shr ax,N / and ax,1 / or ax,ax` for a one bit field, which is
 * exactly the shape in the target (see src/_probe7.c).  The `b65 |= 0x40`
 * write stays a plain unsigned char field so it compiles to `or byte`.
 *
 * FUN_1b11_1ed3 is a forward reference, hence the five byte `nop / push cs /
 * call rel16` form; it is defined after m_b11a below.
 */

typedef unsigned int u16;

struct mobj {
    char            pad0[0x13];                 /* +0x00 */
    int             w13;                        /* +0x13 */
    int             w15;                        /* +0x15 */
    int             w17;                        /* +0x17 */
    int             w19;                        /* +0x19 */
    int             w1b;                        /* +0x1b */
    int             w1d;                        /* +0x1d */
    int             w1f;                        /* +0x1f */
    int             w21;                        /* +0x21 */
    char            pad1[0x0a];                 /* +0x23 */
    unsigned char   b2d;                        /* +0x2d */
    char            pad2[2];                    /* +0x2e */
    unsigned char   b30;                        /* +0x30 */
    char            pad3[0x20];                 /* +0x31 */
    void       (far *fn51)(char far *);         /* +0x51 */
    char            pad4[0x10];                 /* +0x55 */
    unsigned char   b65;                        /* +0x65 */
    unsigned char   b66;                        /* +0x66 */
    char far * far *p67;                        /* +0x67 */
    char            pad5[8];                    /* +0x6b */
    char far * far *p73;                        /* +0x73 */
};

struct mbits {
    char            pad[0x65];                  /* +0x00 */
    unsigned        x0 : 5;                     /* +0x65 bits 0..4 */
    unsigned        b5 : 1;                     /* +0x65 bit 5 */
    unsigned        b6 : 1;                     /* +0x65 bit 6 */
    unsigned        x7 : 1;                     /* +0x65 bit 7 */
    unsigned        y0 : 5;                     /* +0x66 bits 0..4 */
    unsigned        c5 : 1;                     /* +0x66 bit 5 */
    unsigned        c6 : 1;                     /* +0x66 bit 6 */
    unsigned        y7 : 1;                     /* +0x66 bit 7 */
};

#define MB(o) ((struct mbits far *)(o))

struct mlist {
    unsigned char       n;                      /* +0x00 */
    char                pad;                    /* +0x01 */
    struct mobj far    *item[1];                /* +0x02 */
};

extern unsigned char g_349f;
extern u16  g_19e4[];
extern u16  g_19e8;

void FUN_0ffe_000e(int, int, int, int, int, int, int, int, int, int);
void FUN_1fd3_0012(int, int, int, int, int, int, char far *, int, int);
void FUN_1fc2_0006(int, int, int, int, int, int, char far *, int, int, int);
void FUN_1d19_068d(char far *);
void FUN_1d19_073e(char far *);

void FUN_1b11_1ed3(char far *p, char far *o);

int m_b11a(struct mlist far *p)
{
    int i;
    int a, b, d, c;

    for (i = 0; i < p->n; i++) {
        if (p->item[i]->fn51 && !MB(p->item[i])->b5)
            p->item[i]->fn51((char far *)p->item[i]);
        if (!MB(p->item[i])->c6) {
            a = p->item[i]->w13 >> 1;
            b = p->item[i]->w15 >> 1;
            c = p->item[i]->w19 >> 1;
            d = p->item[i]->w17 >> 1;
            FUN_0ffe_000e(a, b, a + c, b + d, a, b,
                          g_19e8, g_19e4[g_349f], 0x140, 0x140);
        }
        if (!MB(p->item[i])->c5)
            FUN_1d19_073e((char far *)p->item[i]);
        FUN_1d19_068d((char far *)p->item[i]);
    }

    for (i = 0; i < p->n; i++) {
        if (MB(p->item[i])->b5) {
            if (MB(p->item[i])->b6)
                FUN_1b11_1ed3((char far *)p, (char far *)p->item[i]);
            else
                p->item[i]->b65 |= 0x40;
        }
    }

    for (i = 0; i < p->n; i++) {
        if (!MB(p->item[i])->b5) {
            if (p->item[i]->b30) {
                FUN_1fd3_0012(0, 0, p->item[i]->w21, p->item[i]->w1f,
                              p->item[i]->w1b, p->item[i]->w1d,
                              p->item[i]->p73[p->item[i]->b2d],
                              g_19e4[g_349f], 0x140);
            } else {
                FUN_1fc2_0006(0, 0, p->item[i]->w21, p->item[i]->w1f,
                              p->item[i]->w1b, p->item[i]->w1d,
                              *((char far * far *)p->item[i]->p67[p->item[i]->b2d]),
                              g_19e4[g_349f], 0x140, 0x140);
            }
        }
    }
}

void FUN_1b11_1ed3(char far *p, char far *o)
{
    (void)p; (void)o;
}
