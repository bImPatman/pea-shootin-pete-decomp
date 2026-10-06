/* @target 0xFACA */
/* @name   dac_write */
/* @proto  void dac_write(char far *p) */
/* @module cross */

/* Target 0xFACA:
     push bp
     mov  bp, sp
     mov  si, word ptr [bp + 6]    ; only the offset is wanted
     mov  dx, 0x3c8                ; VGA DAC write index
     mov  al, 0
     out  dx, al                   ; start at colour 0
     inc  dx                       ; DAC data port
     mov  cx, 0x300                ; 768 entries: 256 colours x RGB
     cld
     rep outsb                     ; straight out of the buffer
     pop  bp
     retf

   A raw VGA palette upload.  The whole 768-byte triple-buffer is pushed at the
   DAC data port in one go, so the caller guarantees the DAC is free first.
 */
void dac_write(char far *p)
{
    asm {
        mov si, word ptr p
        mov dx, 03c8h
        mov al, 0
        out dx, al
        inc dx
        mov cx, 0300h
        cld
        rep outsb
    }
}