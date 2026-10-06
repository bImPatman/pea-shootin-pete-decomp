/* @target 0x0F21 */
/* @name   flagbits_set */
/* @proto  void flagbits_set(unsigned char v) */
/* @module cross */

/* Target 0x10F21:
     push bp
     mov  bp, sp
     mov  al, byte ptr [g_mask]      ; a global byte
     and  al, 0x70
     mov  dl, byte ptr [bp + 6]      ; the single argument, taken as a byte
     and  dl, 0x8F
     or   al, dl
     mov  byte ptr [g_mask], al
     pop  bp
     retf
   Borland allocates a scratch register (dl) for the argument expression and
   keeps al live across it.  The `retf` comes from the function being called
   from another module: Borland emits `ret`, and Turbo Link patches it to
   `retf` when the call is far.  check.py generates that caller automatically. */

unsigned char g_mask;                 /* @addr 0x23F8 in the target */

void flagbits_set(unsigned char v)
{
    g_mask = (g_mask & 0x70) | (v & 0x8F);
}

/* No main() here on purpose: check.py compiles this together with a generated
   CALLER.C that supplies main() and makes the far call.  Two main()s make
   Turbo Link loop writing the map file, so exactly one module may define it. */