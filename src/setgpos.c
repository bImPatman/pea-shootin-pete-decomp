/* @target 0x107F2 */
/* @name   setgpos */
/* @proto  void setgpos(unsigned int v) */

/* Target 0x107F2:
     push bp
     mov  bp, sp
     mov  ax, word ptr [bp + 6]      <- the argument
     mov  word ptr [0x2386], 0       <- zero the HIGH half first
     mov  word ptr [0x2384], ax      <- then store the low half
     pop  bp
     retf

   A 17-byte setter, and a useful contrast with flagput.c and flagbits.c, which
   write single bytes.  The two absolute addresses are 2 apart, so this is one
   32-bit object at 0x2384 being assigned a 16-bit value: Borland widens the
   right-hand side and stores it high word first, then low.

   That store order is the whole signature of the construct.  Three near-misses
   are worth recording so they are not retried:

     long g; f(unsigned v)  { g = v; }        exact  (this file)
     long g; f(int v)       { g = v; }        16 bytes -- `int` narrows the
                                               stored high word instead of
                                               zeroing it unconditionally
     unsigned long g; f(unsigned long v) { g = v; }   18 bytes -- the argument
                                               is read as a dword

   and writing two separate globals by hand (`g_hi = 0; g_lo = v;`) is 17 bytes
   but the wrong instruction order.

   Note `retf` from a module-crossing call: Borland emits a bare `ret` in the
   listing and Turbo Link patches it.  See notes/compiler.md. */

long g_field;

void setgpos(unsigned int v)
{
    g_field = v;
}