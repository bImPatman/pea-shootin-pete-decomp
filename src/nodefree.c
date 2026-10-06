/* @target 0xD134 */
/* @name   node_free */
/* @proto  void node_free(struct nodelist far *p) */
/* @module cross */
/* @extra  xmod/xd19dc.c */

/* Target 0xD134 (87 bytes / 28 insns):
      for (i = 0; i < p->count; i++) {
          release(p->ent[i], 3);
          p->ent[i] = 0;
      }
      p->count = 0;

   The record is a count at +0 followed by an array of far pointers starting at
   +2: one spare byte, then 4-byte entries.  Every iteration walks a
   strength-reduced cursor that starts at the record and steps by 4, so entry i
   is at p+2+4i -- which is why the cursor begins at p rather than at p+2.

   The far callee is release() in another module (FUN_1d19_00dc).  It takes a
   far pointer plus a kind word of 3, tests the pair for null, stamps 0x187f
   over the pointer and -- because bit 0 of 3 is set -- hands it to the
   allocator for a real release.  It ignores the kind's other bits.

   The entry is passed as a far pointer, not as an address of one: the target
   loads its offset and segment out of the record with a single `mov es,[bp+8]`
   / `mov bx,[bp-4]` pair and pushes both words, which is what the compiler
   emits for a `char far *` argument and not for `&entry`.

   The loop bound is re-read from the record every iteration rather than
   hoisted, so the count word is loaded through es:bx each time round.
 */
struct nodelist {
    unsigned char count;
    unsigned char pad;
    char far *ent[1];
};

void m_d19_dc(char far *p, unsigned int kind);

void node_free(struct nodelist far *p)
{
    int i;

    for (i = 0; i < p->count; i++) {
        m_d19_dc(p->ent[i], 3);
        p->ent[i] = 0;
    }
    p->count = 0;
}
