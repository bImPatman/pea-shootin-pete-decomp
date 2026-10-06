/* @target 0xD4BF */
/* @func  FUN_1d19_032f
 *
 * Target (58 bytes / 20 insns):
 *   D4BF  55             push bp
 *   D4C0  8bec           mov bp, sp
 *   D4C2  c45e06         les bx, ptr [bp + 6]
 *   D4C5  8b460a         mov ax, word ptr [bp + 0xa]
 *   D4C8  2689470b       mov word ptr es:[bx + 0xb], ax
 *   D4CC  8b460c         mov ax, word ptr [bp + 0xc]
 *   D4CF  2689470d       mov word ptr es:[bx + 0xd], ax
 *   D4D3  8a460e         mov al, byte ptr [bp + 0xe]
 *   D4D6  2688472f       mov byte ptr es:[bx + 0x2f], al
 *   D4DA  26837f0b00     cmp word ptr es:[bx + 0xb], 0
 *   D4DF  7416           je 0xd4f7
 *   D4E1  26837f0b00     cmp word ptr es:[bx + 0xb], 0
 *   D4E6  7d07           jge 0xd4ef
 *   D4E8  26c6473301     mov byte ptr es:[bx + 0x33], 1
 *   D4ED  5d             pop bp
 *   D4EE  cb             retf
 *   D4EF  c45e06         les bx, ptr [bp + 6]
 *   D4F2  26c6473302     mov byte ptr es:[bx + 0x33], 2
 *   D4F7  5d             pop bp
 *   D4F8  cb             retf
 *
 * Three things are load-bearing here:
 *
 *  - The field is compared *twice*, both times re-read from memory, so both
 *    conditions must be written against `rec->a` rather than a cached local.
 *  - `jge` (7D) not `jae` (77), so +0x0b is a signed `int`.
 *  - The `return` inside the negative arm is what gives that arm its own
 *    `pop bp / retf`, leaving the positive arm to fall through to the shared
 *    exit.  Dropping it changes the instruction count.
 *
 * Sibling of setfields.c; same record, same module.
 */
struct cls_s {
    unsigned char pad0[0x0B];
    int a;
    int b;
    unsigned char pad1[0x20];
    unsigned char c;
    unsigned char pad2[3];
    unsigned char d;
};

void setclass(struct cls_s far *rec, int u, int v, unsigned char w)
{
    rec->a = u;
    rec->b = v;
    rec->c = w;
    if (rec->a != 0) {
        if (rec->a < 0) {
            rec->d = 1;
            return;
        }
        rec->d = 2;
    }
}