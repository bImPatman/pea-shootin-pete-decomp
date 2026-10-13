/* Functional reconstruction of FUN_1fd3_0178 (module 0xfd3).

   Puts the VGA into "Mode X": BIOS mode 13h, then the Sequencer Memory Mode
   register is set to 06h (chain-4 off, odd/even on, extended memory), video
   memory is cleared, and CRTC registers 14h/17h are set for byte mode.  This
   is the planar 320x200 mode the 0xfd3 glyph blitter (FUN_1fd3_01fa) draws
   into.  The original is assembly; the INT 10h call clobbers BP/SI/DI, so they
   are saved first, exactly as the original does. */
void FUN_1fd3_0178(void)
{
    asm {
        push    bp
        push    si
        push    di
        mov     ax, 0013h
        int     10h
        mov     dx, 03c4h
        mov     ax, 0604h
        out     dx, ax
        mov     ax, 0a000h
        mov     es, ax
        sub     di, di
        sub     ax, ax
        mov     cx, 8000h
        rep     stosw
        mov     dx, 03d4h
        mov     dx, 03d4h
        mov     ax, 0014h
        out     dx, ax
        mov     ax, 0e317h
        out     dx, ax
        pop     di
        pop     si
        pop     bp
    }
}
