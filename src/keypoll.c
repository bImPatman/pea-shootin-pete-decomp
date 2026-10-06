/* @target 0x1AE1 */
/* @name   key_poll */
/* @proto  int key_poll(void) */
/* @module cross */

/* Target 0x1AE1:
     cmp byte ptr [0x2444], 0
     je   0x1aed
     mov  ax, 1
     jmp  0x1af2
   0x1aed:
     mov  ah, 0x0b        ; DOS: check whether a keystroke is waiting
     int  0x21            ;   AL = 0xFF if one is, 0 otherwise
     cbw                 ; sign-extend AL, so 0xFF becomes 0xFFFF
   0x1af2:
     retf

   Once the flag at 0x2444 has been set the answer is simply 1 and the BIOS is
   never consulted again.  The flag is only ever cleared by key_read(), which
   is what makes this a one-shot latch rather than a poll.
 */
unsigned char g_2444;

int key_poll(void)
{
    if (g_2444)
        _AX = 1;
    else
        asm {
            mov ah, 0bh
            int 21h
            cbw
        }
    return _AX;
}