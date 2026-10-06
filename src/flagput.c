/* @target 0x0F4F */
/* @name   flagbits_store */
/* @proto  void flagbits_store(unsigned char v) */
/* @module cross */

/* Target 0x10F4F:
     push bp
     mov  bp, sp
     mov  al, byte ptr [bp + 6]
     mov  byte ptr [0x23f8], al
     pop  bp
     retf

   The other half of the flagbits cluster, and the reason it is worth doing
   before anything harder: 11 bytes, 6 instructions, one global, no call, no
   frame.  There is nothing to get wrong, so a failure here would mean the
   harness is broken rather than the source.

   Note the absence of a mask.  flagbits_set (0x10F21) masks, flagbits_shift
   (0x10F36) masks, and this one stores the byte through untouched -- which is
   what pins the cluster's semantics: the caller is responsible for masking, and
   these are the primitives it masks with. */

unsigned char g_mask;                 /* same global as flagbits.c */

void flagbits_store(unsigned char v)
{
    g_mask = v;
}