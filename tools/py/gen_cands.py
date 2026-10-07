import os
root = r'C:\Users\patri\pea-shoot\cand'
os.makedirs(root, exist_ok=True)
tpl = '''/* @target FUN_13b2_2415 */
/* @name   m_2415 */
/* @proto  void m_2415(void) */
/* @module same */
/* @extra  ../src/g13b2.c ../src/xmod/xb111106.c ../src/xmod/xb1111cc.c ../src/xmod/xf02bf.c ../src/xmod/xf0276.c ../src/xmod/xb111653.c */

extern unsigned int  g_168c, g_2c01, g_2bff;
extern unsigned char g_349f, g_27f2, g_27f3, g_27f7;
extern unsigned char g_27ec, g_27ed, g_27fc, g_2bf0;
extern unsigned char g_27ee, g_27f0, g_27f1, g_27f4;
extern unsigned char g_27fd, g_27fe;
extern char far *g_27ae;
extern char far *g_34ad;
extern char far *g_3858;

char far *FUN_1b11_1106(int a, int b, char far *s);
void      FUN_1b11_11cc(char far *p, int v);
void      FUN_1000_02bf(char far *p);
void      FUN_1f44_0276(char far *p, int a, int b);
void      FUN_1b11_1653(char far *a, char far *b, int c);

void FUN_13b2_2018(void)
{
    FUN_1b11_1653(g_34ad, "x", 0);
}

void m_2415(void)
{
    g_168c = 0xBB80;
    g_349f = 0;
    g_27f2 = 0;
    g_27f3 = g_27f7 = 0;
    %GROUP%;
    g_27ec = 0;
    g_27ed = 0;
    g_27fc = 0;
    g_2bf0 = 0;
    g_27ee = 2;
    g_27f0 = 0;
    g_27f1 = 1;
    g_27f4 = 6;
    g_27fd = 0;
    g_27fe = 0;

    g_27ae = FUN_1b11_1106(0, 0, "s");
    g_27ae[0x0a] = 2;
    FUN_1b11_11cc(g_27ae, 0);
    FUN_1000_02bf(g_27ae);
    FUN_1f44_0276(g_3858, 0, 0);
    FUN_1f44_0276(g_3858, 0, 1);
    FUN_1f44_0276(g_3858, 0, 3);
    FUN_13b2_2018();
}
'''
groups = {
 'c0': 'g_2c01 = 0; g_2bff = 0;',
 'c1': 'g_2c01 = g_27f3; g_2bff = g_27f3;',
 'c2': 'g_2c01 = 0; g_2bff = g_27f3;',
 'c3': 'g_2c01 = 0; g_2bff = g_27f7;',
 'c4': 'g_2c01 = 0; g_2bff = g_349f;',
 'c5': 'g_2c01 = g_27f3; g_2bff = 0;',
 'c6': 'g_2bff = g_2c01 = g_27f3;',
 'c7': 'g_2c01 = 0; g_2bff = g_27f2;',
 'c8': 'g_2c01 = g_27f7; g_2bff = g_27f7;',
 'd0': 'g_2bff = g_2c01 = g_27f3 = g_27f7 = 0;',
 'd1': 'g_2bff = g_2c01 = (g_27f3 = g_27f7 = 0);',
 'd2': 'g_2c01 = (char)0; g_2bff = g_27f3;',
 'd3': 'g_2c01 = (g_27f3, 0); g_2bff = g_27f3;',
 'd4': 'g_2c01 = (g_27f3 & 0); g_2bff = g_27f3;',
 'd5': 'g_2c01 = 0 * g_27f3; g_2bff = g_27f3;',
 'd6': 'g_2bff = g_27f3; g_2c01 = 0;',
 'd7': 'g_2c01 = (int)g_27f3 >> 0; g_2bff = g_27f3;',
 'd8': 'g_2c01 = !g_27f3; g_2bff = g_27f3;',
}
for k, g in groups.items():
    open(os.path.join(root, k + '.c'), 'w').write(tpl.replace('%GROUP%', g))
print('wrote', len(groups))
