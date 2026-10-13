/* Functional reconstruction of FUN_1fd3_0150 (module 0xfd3).

   Sets the VGA CRTC display start address.  The argument word v is written as:
   register 0Dh = low byte of v, register 0Ch = high byte, exactly the mapping
   the original assembly uses.  The original also polls port 3DAh to avoid a
   mid-frame update; that is a cosmetic detail, omitted here. */
void FUN_1fd3_0150(int v)
{
    asm {
        mov     dx, 03d4h
        mov     bx, word ptr v
        mov     al, 0dh
        mov     ah, bh
        out     dx, ax
        mov     al, 0ch
        mov     ah, bl
        out     dx, ax
    }
}
