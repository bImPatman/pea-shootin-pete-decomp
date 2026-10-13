/* Functional reconstruction of FUN_1fd3_01e1 (module 0xfd3).

   BIOS INT 10h, AH=11h, AL=30h returns the address of the current 8x8
   character font in ES:BP; stash it for the glyph blitter.  The original is
   hand-written assembly: it saves BP/SI/DI/DS around the call because the BIOS
   does not preserve them, and it deliberately does not save ES. */
extern char far *g_2044;

void FUN_1fd3_01e1(void)
{
    asm {
        push    bp
        push    si
        push    di
        push    ds
        mov     ah, 11h
        mov     al, 30h
        mov     bh, 3
        int     10h
        mov     word ptr [g_2044], bp
        mov     word ptr [g_2044+2], es
        pop     ds
        pop     di
        pop     si
        pop     bp
    }
}
