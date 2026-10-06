/* @target 0xF583 */
/* @func  FUN_1f44_0143
 *
 * Target (69 bytes / 23 insns):
 *   F583  55             push bp
 *   F584  8bec           mov bp, sp
 *   F586  83ec04         sub sp, 4
 *   F589  c45e06         les bx, ptr [bp + 6]
 *   F58C  26807f0201     cmp byte ptr es:[bx + 2], 1
 *   F591  7533           jne 0xf5c6
 *   F593  c746fe0000     mov word ptr [bp - 2], 0
 *   F598  8b4606         mov ax, word ptr [bp + 6]
 *   F59B  050600         add ax, 6
 *   F59E  8946fc         mov word ptr [bp - 4], ax
 *   F5A1  8e4608         mov es, word ptr [bp + 8]
 *   F5A4  8b5efc         mov bx, word ptr [bp - 4]
 *   F5A7  268a07         mov al, byte ptr es:[bx]
 *   F5AA  8b5efe         mov bx, word ptr [bp - 2]
 *   F5AD  88875035       mov byte ptr [bx + 0x3550], al
 *   F5B1  ff46fc         inc word ptr [bp - 4]
 *   F5B4  ff46fe         inc word ptr [bp - 2]
 *   F5B7  817efe0003     cmp word ptr [bp - 2], 0x300
 *   F5BC  7ce3           jl 0xf5a1
 *   F5BE  c45e06         les bx, ptr [bp + 6]
 *   F5C1  26c6470200     mov byte ptr es:[bx + 2], 0
 *   F5C6  c9             leave
 *   F5C7  cb             retf
 *
 * Mirror image of statesave.c: guards on `state == 1` instead of `state == 0`,
 * copies the other way, and clears the flag.  Same record layout (data at
 * offset 6), same global table at 0x3550, same strength-reduced walking
 * offset in `[bp - 4]`.
 *
 * This is the first pair of reconstructions that are demonstrably two halves
 * of one source file - adjacent in the module, 0x45 bytes apart, sharing both
 * the record and the table - so the record layout recovered here is the real
 * one for module FUN_1f44_*, not a per-function guess.
 */
struct blk_s {
    unsigned char pad0[2];
    unsigned char state;
    unsigned char pad1[3];
    unsigned char data[0x300];
};

unsigned char tbl[0x300];

void statebak(struct blk_s far *blk)
{
    int i;
    if (blk->state == 1) {
        for (i = 0; i < 0x300; i++)
            tbl[i] = blk->data[i];
        blk->state = 0;
    }
}