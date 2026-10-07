/* Stand-in for FUN_1b11_15d6 (segment 0xb11, flat 0x0C6E6, 125 bytes / 55 ins).
   Called by m_0252 and m_013b.  Returns a far pointer to a structure whose word
   fields at +0x26 and +0x28 m_0252 forwards on, or null when it cannot satisfy
   the request.  Not reconstructed: the body is an OROS-style
   `push bp / sub sp,8 / push si / push di` prolog with locals at [bp-2]. */

struct devinfo { char pad[0x26]; int f26; int f28; };

static struct devinfo rec;

struct devinfo far *FUN_1b11_15d6(char far *rec_in, char far *name)
{
    (void)rec_in; (void)name;
    return &rec;
}
