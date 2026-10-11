/* Stand-in for FUN_1000_2937 (segment 0x1000, ghidra 0x12937, 40 bytes).

   The target is the run-time's number -> ASCII conversion (ultoa-shaped: it
   takes (value, buf, base) and writes the digits, NUL-terminated, into buf).
   m_154f / FUN_13b2_17ba depend on that write: they call it to fill g_2c33 and
   then take strlen(g_2c33) to size the digit loop, so an empty buffer makes
   n = strlen - 1 wrap to 0xff and the loop runs 256 times instead of once.
   Delegate to the linked run-time so the buffer really receives the number.
   Not reconstructed byte-for-byte. */
#include <stdlib.h>

void FUN_1000_2937(unsigned long v, char far *buf, int base)
{
    ultoa(v, buf, base);
}
