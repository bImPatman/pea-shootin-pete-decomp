/* Functional reconstruction of the module-0xfd3 menu text renderer
   FUN_1fd3_01fa and its planar-VGA putpixel helper FUN_1fd3_01a7.

   The originals are hand-written assembly (a CS-relative DS-scratch idiom and
   a frame-less helper) that BCC 3.1 cannot reproduce byte-for-byte, so these
   are functional equivalents: enough to make the boot menu actually draw. */

unsigned int g_2030;   /* text cursor x         */
unsigned int g_2032;   /* text cursor y         */
unsigned int g_2034;   /* text colour           */
unsigned int g_2036;   /* framebuffer base off  */
unsigned int g_2038;   /* colour-increment flag */
unsigned int g_203a;   /* putpixel x            */
unsigned int g_203c;   /* putpixel y            */
unsigned int g_203e;   /* putpixel colour       */
char far *g_2044;      /* 8x8 font pointer, set by FUN_1fd3_01e1 */

/* Plane-masked write of g_203e at (g_203a, g_203c) in VGA planar mode:
   offset = g_2036 + g_203c*80 + g_203a/4, plane = g_203a & 3. */
void FUN_1fd3_01a7(void)
{
    asm {
        push    es
        mov     si, word ptr g_203a
        mov     bx, word ptr g_203c
        mov     cx, word ptr g_203e
        push    cx
        mov     ax, 0050h
        mul     bx
        mov     bx, si
        shr     bx, 1
        shr     bx, 1
        add     bx, ax
        mov     dx, word ptr g_2036
        add     bx, dx
        mov     ax, 0a000h
        mov     es, ax
        mov     cx, si
        and     cl, 3
        mov     ax, 0102h
        shl     ah, cl
        mov     dx, 03c4h
        out     dx, ax
        pop     cx
        mov     al, cl
        mov     es:[bx], al
        pop     es
    }
}

/* Draw the far string s as 8x8 glyphs starting at (x, y).  Each glyph is
   indexed by the character value into the font at g_2044; the cursor advances
   8 px per character.  colour is incremented per scanline when flag is set. */
void FUN_1fd3_01fa(int x, int y, int base, int color, char far *s, int flag)
{
    unsigned char c;
    unsigned char bits;
    char far *fp;
    int row;
    int bit;

    g_2030 = x;
    g_2032 = y;
    g_2036 = base;
    g_2034 = color;
    g_2038 = flag;

    while ((c = *s) != 0) {
        g_203a = g_2030;
        g_203c = g_2032;
        g_203e = g_2034;
        fp = g_2044 + (unsigned int)c * 8;
        for (row = 0; row < 8; row++) {
            bits = (unsigned char)fp[row];
            for (bit = 0; bit < 8; bit++) {
                if (bits & 0x80)
                    FUN_1fd3_01a7();
                bits = (unsigned char)(bits << 1);
                g_203a++;
            }
            g_203a = g_2030;
            g_203c++;
            if (g_2038)
                g_203e++;
        }
        s++;
        g_2030 += 8;
    }
}
