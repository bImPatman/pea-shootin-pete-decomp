/* @target FUN_1f44_0006 */
/* @name   m_f4406 */
/* @proto  char far *m_f4406(char far *p) */
/* @module cross */
/* @extra  xmod/x0778.c xmod/x19ee.c xmod/xfd3178.c xmod/xfd31e1.c xmod/xfd3150.c */

/* Target 0xF446 (125 bytes / 46 insns): initialise a "Pete" record.  If p is
   null allocate 0x306 bytes; fill the 16-byte local descriptor with the value
   0x1a00 and pass it twice (with length 0x10) to the RTL helper at 0:0x19ee;
   if the byte at +2 comes back != 8 register p via the module-local helper
   FUN_1f44_00b7 with the far string at ds:0x19ec; then run the two 0xfd3
   helpers, clear the record's bytes 0/2/3 and tear down with FUN_1fd3_0150(0).
   Returns p in DX:AX. */

unsigned char g_19ec;

void      FUN_1f44_00b7(char far *p, char far *s);
char far *FUN_1000_0778(int n);
void      FUN_1000_19ee(int n, char far *a, char far *b);
void      FUN_1fd3_0178(void);
void      FUN_1fd3_01e1(void);
void      FUN_1fd3_0150(int v);

char far *m_f4406(char far *p)
{
    char local[0x10];

    if (p != 0 || (p = FUN_1000_0778(0x306)) != 0) {
        *(int *)local = 0x1a00;
        FUN_1000_19ee(0x10, (char far *)local, (char far *)local);

        if (local[2] != 8)
            FUN_1f44_00b7(p, (char far *)&g_19ec);

        FUN_1fd3_0178();
        FUN_1fd3_01e1();

        p[0] = p[2] = p[3] = 0;

        FUN_1fd3_0150(0);
    }
    return p;
}

void FUN_1f44_00b7(char far *p, char far *s)
{
    (void)p; (void)s;
}
