/* @target 0xC6A7 */
/* @name   m_b159 */
/* @proto  int m_b159(char far *p) */

/* p points at a dispatch table: 100 far function pointers from offset 0,
   followed by a one byte entry count at 0x190. Every entry is invoked once.

   FUN_1b11_1257 lives in the same module as m_b159 in the original image, so
   it has to be defined in this file: a cross module reference would compile to
   lcall instead of push cs / call rel16.

   It must also be defined *before* m_b159. With a forward reference bcc emits
   the five byte form nop / push cs / call rel16, but the original image uses
   the four byte form. */

struct table160 {
    char far *fn[100];
    unsigned char count;
};

void FUN_1b11_1257(char far *fn);

/* not reconstructed yet: the target body is 168 bytes / 52 instructions and
   ends up calling through the node's vtable at +0x26 */
void FUN_1b11_1257(char far *fn)
{
    fn = fn;
}

int m_b159(struct table160 far *p)
{
    int i;

    i = 0;
    while (p->count > i) {
        FUN_1b11_1257(p->fn[i]);
        i++;
    }
}