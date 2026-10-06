/* @target 0x0F36 */
/* @name   flagbits_shift */
/* @proto  void flagbits_shift(unsigned char v) */
/* @module cross */

/* Target 0x10F36:
     push bp
     mov  bp, sp
     mov  al, byte ptr [0x23f8]
     and  al, 0x8f
     mov  dl, byte ptr [bp + 6]
     mov  cl, 4
     shl  dl, cl
     and  dl, 0x7f
     or   al, dl
     mov  byte ptr [0x23f8], al
     pop  bp
     retf

   Sibling of flagbits_set at 0x10F21.

   KNOWN DIVERGENCE -- read tools/py/shifts.py output before "fixing" this.

   The target routes the shift through CL:

       mov  cl, 4
       shl  dl, cl

   whereas `v << 4` compiles to the short form `shl dl,4` (2 bytes), which is
   what this file produces at every optimisation level.

   `shl r/m8,cl` is the only 8-bit shift x86 offers, so a *variable* count must
   be materialised in CL -- but a count the optimiser already knows is 4 it
   normally folds straight into the immediate form.  Getting the long form means
   the count reached the shift as a variable whose value Borland had already
   constant-propagated into CL and then declined to re-fold.

   Reproducing that needs the count to live in memory, which `static const
   unsigned char n = 4` does -- and that variant matches every other instruction
   in the function exactly:

       mov cl, byte ptr [n]      <->  mov cl, 4

   So only the count's storage differs (27 vs 25 bytes), and the shipped source
   below prefers the readable form.  `tools/py/search.py 0x0F36` sweeps ~30
   candidate C shapes x 13 flag sets and finds no combination that matches,
   which puts this one idiom outside BCC 3.1's reach rather than pointing at a
   wrong flag set.  It is a codegen difference, not a decompilation error: see
   the "constant shifts via CL" section of notes/compiler.md. */

unsigned char g_mask;                 /* same global as flagbits.c */

void flagbits_shift(unsigned char v)
{
    g_mask = (g_mask & 0x8F) | ((v << 4) & 0x7F);
}