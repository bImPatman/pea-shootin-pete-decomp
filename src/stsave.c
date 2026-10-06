/* @target 0xF53E */
/* @func  FUN_1f44_00fe
 *
 * Target (69 bytes / 23 insns):
 *   F53E  55             push bp
 *   F53F  8bec           mov bp, sp
 *   F541  83ec04         sub sp, 4
 *   F544  c45e06         les bx, ptr [bp + 6]
 *   F547  26807f0200     cmp byte ptr es:[bx + 2], 0
 *   F54C  7533           jne 0xf581
 *   F54E  c746fe0000     mov word ptr [bp - 2], 0
 *   F553  8b4606         mov ax, word ptr [bp + 6]
 *   F556  050600         add ax, 6
 *   F559  8946fc         mov word ptr [bp - 4], ax
 *   F55C  8b5efe         mov bx, word ptr [bp - 2]
 *   F55F  8a875035       mov al, byte ptr [bx + 0x3550]
 *   F563  8e4608         mov es, word ptr [bp + 8]
 *   F566  8b5efc         mov bx, word ptr [bp - 4]
 *   F569  268807         mov byte ptr es:[bx], al
 *   F56C  ff46fc         inc word ptr [bp - 4]
 *   F56F  ff46fe         inc word ptr [bp - 2]
 *   F572  817efe0003     cmp word ptr [bp - 2], 0x300
 *   F577  7ce3           jl 0xf55c
 *   F579  c45e06         les bx, ptr [bp + 6]
 *   F57C  26c6470201     mov byte ptr es:[bx + 2], 1
 *   F581  c9             leave
 *   F582  cb             retf
 *
 * Borland strength-reduces `blk->data[i]` into a walking *offset* held in one
 * local (`[bp - 4]`, initialised from the far pointer's offset word at
 * `[bp + 6]`) while reloading `es` from `[bp + 8]` each iteration, so the
 * record's data array starts at offset 6 - not 3.  Getting the header size
 * wrong shows up as `add ax, 3` vs `add ax, 6` and costs the exact tier while
 * shape still passes at 100%.
 *
 * The source table is a *global* array (absolute `mov al,[bx + 0x3550]`), not a
 * pointer parameter, which is why the table's address shows up as a relocation
 * here but does not affect the verdict.
 *
 * Sibling of statebak.c (FUN_1f44_0143), 0x45 bytes away in the same module
 * and writing the same two globals - almost certainly one source file.
 */
struct blk_s {
    unsigned char pad0[2];
    unsigned char state;
    unsigned char pad1[3];
    unsigned char data[0x300];
};

unsigned char tbl[0x300];

void stsave(struct blk_s far *blk)
{
    int i;
    if (blk->state == 0) {
        for (i = 0; i < 0x300; i++)
            blk->data[i] = tbl[i];
        blk->state = 1;
    }
}