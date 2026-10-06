/* @target 0x18E4 */
/* @name   key_read */
/* @proto  int key_read(void) */
/* @module cross */
/* @extra  keypoll.c */

/* Target 0x18E4:
     cmp byte ptr [0x2444], 0
     je   0x18f5
     mov  byte ptr [0x2444], 0     ; consume the latch
     mov  al, byte ptr [0x2445]     ; hand over what was stashed
     jmp  0x18fa
   0x18f5:
     mov  ax, 0x700                 ; DOS: read a character, no echo
     int  0x21
   0x18fa:
     mov  ah, 0                     ; widen the char to a word
     retf

   This is the other half of key_poll(): reading clears the latch, so a key
   seen by key_poll() is reported exactly once.  g_2444 and g_2445 are the
   same two globals key_poll() tests.
 */
/* In the target both halves live in one module and share these two bytes.
   Here each is its own module, so key_poll() owns the latch and this file
   only borrows it. */
extern unsigned char g_2444;
unsigned char g_2445;

unsigned char key_read(void)
{
    if (g_2444) {
        g_2444 = 0;
        _AL = g_2445;
    } else {
        asm {
            mov ax, 0700h
            int 21h
        }
    }
    _AH = 0;
    return _AX;
}