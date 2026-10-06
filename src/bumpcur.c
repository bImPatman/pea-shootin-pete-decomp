/* @target 0x9040 */
/* @func  FUN_18a2_0620
 *
 * Target (26 bytes / 10 insns):
 *   9040  55                 push bp
 *   9041  8bec               mov bp, sp
 *   9043  c45e06             les bx, ptr [bp + 6]
 *   9046  268b4749           mov ax, word ptr es:[bx + 0x49]
 *   904A  26884749           inc word ptr es:[bx + 0x49]
 *   904E  3d2800             cmp ax, 0x28
 *   9051  7e05               jle 0x9058
 *   9053  26804f6520         or byte ptr es:[bx + 0x65], 0x20
 *   9058  5d                 pop bp
 *   9059  cb                 retf
 *
 * The `jle` (signed 7C) rather than `jbe` (unsigned 76) proves the counter
 * field is a plain `int`, not `unsigned int`; the record layout is a word at
 * +0x49 and a flag byte at +0x65. `r->cur++` in the condition reproduces the
 * read-then-increment pair exactly.
 */
struct rec_s {
    unsigned char pad0[0x49];
    int cur;
    unsigned char pad1[0x1A];
    unsigned char flags;
};

void bumpcur(struct rec_s far *rec)
{
    if (rec->cur++ > 0x28)
        rec->flags |= 0x20;
}