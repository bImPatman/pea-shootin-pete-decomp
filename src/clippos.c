/* @target 0x99BA */
/* @func  FUN_18a2_0f9a
 *
 * Target (89 bytes / 33 insns):
 *   99BA  55             push bp
 *   99BB  8bec           mov bp, sp
 *   99BD  56             push si
 *   99BE  c41ea134       les bx, ptr [0x34a1]
 *   99C2  268b4702       mov ax, word ptr es:[bx + 2]
 *   99C6  48             dec ax
 *   99C7  c45e0a         les bx, ptr [bp + 0xa]
 *   99CA  268907         mov word ptr es:[bx], ax
 *   99CD  c45e06         les bx, ptr [bp + 6]
 *   99D0  268b4704       mov ax, word ptr es:[bx + 4]
 *   99D4  2603470d       add ax, word ptr es:[bx + 0xd]
 *   99D8  c45e0e         les bx, ptr [bp + 0xe]
 *   99DB  268907         mov word ptr es:[bx], ax
 *   99DE  c45e0a         les bx, ptr [bp + 0xa]
 *   99E1  26833f00       cmp word ptr es:[bx], 0
 *   99E5  7d05           jge 0x99ec
 *   99E7  26c7070000     mov word ptr es:[bx], 0
 *   99EC  c45e06         les bx, ptr [bp + 6]
 *   99EF  06             push es
 *   99F0  c4760a         les si, ptr [bp + 0xa]
 *   99F3  268b04         mov ax, word ptr es:[si]
 *   99F6  07             pop es
 *   99F7  26034708       add ax, word ptr es:[bx + 8]
 *   99FB  3d8002         cmp ax, 0x280
 *   99FE  7e10           jle 0x9a10
 *   9A00  8e4608         mov es, word ptr [bp + 8]
 *   9A03  b88002         mov ax, 0x280
 *   9A06  262b4708       sub ax, word ptr es:[bx + 8]
 *   9A0A  c45e0a         les bx, ptr [bp + 0xa]
 *   9A0D  268907         mov word ptr es:[bx], ax
 *   9A10  5e             pop si
 *   9A11  5d             pop bp
 *   9A12  cb             retf
 *
 * Two out-params plus a bounds check.  `*p` is seeded from a global record's
 * cursor, `*q` from this record's origin, and then `*p` is clamped into
 * [0, 0x280 - rec->f08].
 *
 * Two idioms worth noting, both of which the struct/`far *` reading had to get
 * right for this to land on the first attempt:
 *
 *   - The seed is `g->f02 - 1` and the target says `dec ax`, not a full
 *     subtract, so the source really is a decrement of a loaded field.
 *   - The clamp reads through the argument rather than caching it in a local,
 *     which is why the target reloads `es` from [bp+8] and dereferences the
 *     [bp+0xa] pointer with `si` (`push es / les si / ... / pop es`) while `bx`
 *     is still holding `a`.  Writing the conditions as `if (*p < 0)` and
 *     `if (*p + a->f08 > 0x280)` reproduces that register split exactly; an
 *     equivalent local-caching form does not.
 */
struct rec_s {
    unsigned char pad0[2];
    int f02, f04;
    unsigned char pad1[2];
    int f08;
    unsigned char pad2[3];
    int f0d;
};
struct rec_s far *g;
void clippos(struct rec_s far *a, int far *p, int far *q)
{
    *p = g->f02 - 1;
    *q = a->f04 + a->f0d;
    if (*p < 0)
        *p = 0;
    if (*p + a->f08 > 0x280)
        *p = 0x280 - a->f08;
}