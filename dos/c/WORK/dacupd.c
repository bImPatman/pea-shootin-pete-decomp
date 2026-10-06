/* @target 0xF603 */
/* @name   m_f44c */
/* @proto  void m_f44c(char far *p) */
/* @module cross */
/* @extra  dacwrite.c */

/* Target 0xF603 (27 bytes / 11 insns):
      push bp / mov bp,sp / les bx,[bp+6]
      cmp byte es:[bx+2],0
      je  end
      push ds / push 0x3550 / lcall 0xfa6:0x6a / add sp,4
   end: pop bp / retf

   Flush the shadow palette if the record is live.  0x3550 is the same 0x300
   byte buffer statebak.c fills from the record, so this is the "commit the
   staged palette" half of the palette load path: statebak stages it into the
   buffer, m_f44c decides whether to push it at the DAC.

   Only the offset of 0x3550 is encoded in the push because the target pushed
   the segment separately; dac_write() ignores both and runs `rep outsb` off
   the segment:offset pair the loader put in place.
 */
unsigned char g_3550[0x300];

void dac_write(char far *p);

void m_f44c(char far *p)
{
    if (p[2])
        dac_write(g_3550);
}