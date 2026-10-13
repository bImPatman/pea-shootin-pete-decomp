/* @target FUN_1e25_000a */
/* @name   m_e2500a */
/* @proto  char far *m_e2500a(char far *p, char far *s) */
/* @module cross */
/* @extra  xmod/x0778.c xmod/x161d840.c xmod/x161d820.c xmod/xb110056.c xmod/x161d401.c xmod/x161d456.c xmod/x1000c91.c xmod/x161da58.c xmod/x161d24a.c xmod/x161d363.c xmod/x161d338.c */

/* Target 0xE25A (290 bytes / 86 insns): initialise the picture buffer g_27aa.
   Allocate 0x10b bytes if p is null; clear p+0x105..0x107 and p+0x64..0x67 plus
   four DS words at 0x3548..0x354e; run the 0x161d init helper; if a name was
   supplied and the 0x161d probe returns -1 report it via FUN_1b11_0056; read a
   status word twice into g_18fa and branch on -1 to arm (and otherwise disarm)
   the 0x161d sound/channel helpers; stash g_18fa into p+0x10a; finally zero the
   0x14-byte block at p+0x50.  Returns p in DX:AX. */

unsigned int  g_354a, g_3548, g_354e, g_354c;
unsigned int  g_18fa;
unsigned char g_18fc, g_1919, g_1932;

char far *FUN_1000_0778(int n);
void      FUN_161d_0840(void);
int       FUN_161d_0820(char far *p);
int       FUN_1b11_0056(char far *s);
int       FUN_161d_0401(void);
int       FUN_161d_0456(void);
int       FUN_1000_2c09_1(char far *a);
void      FUN_161d_0a58(int v);
void      FUN_161d_024a(char far *p);
void      FUN_161d_0363(int a, int b);
void      FUN_161d_0338(int a, int b);

char far *m_e2500a(char far *p, char far *s)
{
    int i;

    if (p != 0 || (p = FUN_1000_0778(0x10b)) != 0) {
        p[0x109] = p[0x108] = 0;
        g_354a = 0;
        g_3548 = 0;
        g_354e = 0;
        g_354c = 0;
        *(int *)(p + 0x66) = 0;
        *(int *)(p + 0x64) = 0;

        FUN_161d_0840();

        if (s != 0) {
            if (FUN_161d_0820(s) == -1)
                FUN_1b11_0056((char far *)&g_18fc);
        }

        g_18fa = FUN_161d_0401();
        g_18fa = FUN_161d_0456();
        if (g_18fa != -1) {
            g_18fa = 1;
            FUN_1000_2c09_1((char far *)&g_1919);
            FUN_161d_0a58(1);
        } else {
            g_18fa = 0;
        }

        if (g_18fa != 0) {
            FUN_161d_024a((char far *)&g_1932);
            FUN_161d_0363(0xf, 0xf);
            FUN_161d_0338(0xf, 0xf);
        }

        p[0x10a] = (char)g_18fa;

        for (i = 0; i < 0x14; i++)
            p[0x50 + i] = 0;
    }
    return p;
}
