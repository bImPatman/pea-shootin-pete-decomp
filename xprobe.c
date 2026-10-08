#include <string.h>

char far *g_27ae, *g_34a1, *g_34a9, *g_27e2, *g_34ad;

unsigned char g_27ec, g_27ed;
unsigned char g_27f5, g_27f8;

struct obj_e2 {
    unsigned char b[0xf];
    char far *f0f;
    unsigned char b2[5];
    void (far *fn18)(void);
};

struct obj742 {
    char far *f0;
    char far *f4;
    char far *f8;
    char far *fc;
    char far *f10;
    char far *f14;
    unsigned char pad[0x32 - 0x18];
    void (far *fn32)(void);
};

struct disp_row {
    char far *p_73a;
    char far *p_73e;
    char far *p_742;
    char far *p_746;
    char far *p_74a;
    void (far *fn)(void);
    char far *sub[6];
    unsigned char pad[4];
};
struct disp_row g_dispatch[8];

char far *FUN_1b11_1106(int a, int b, char far *s) { return g_34ad; }
void FUN_1b11_11cc(char far *p, int v) { g_27ae = p; }
void FUN_1000_02bf(char far *p) { g_27ae = p; }
int FUN_1b11_1653(char far *a, char far *b, int c) { return a != b ? c : 0; }
void FUN_1f44_0276(char far *p, int a, int b) { g_34ad = p; }
void FUN_1f44_029e(char far *p, char a) { g_34ad = p; }
void FUN_1f44_0426(char far *p) { g_34ad = p; }
void FUN_0ffe_000e(int a, int b, int c, int d, char far *e, int f, int g, int h, int i) { g_34ad = e; }
void FUN_0d19_0104(char far *p, int a, int b, int c) { g_34ad = p; }
char far *FUN_1b11_1dc1(char far *p, int a, int b, int c, int d, int e) { return g_34a9; }

void probe(void)
{
    struct disp_row *row;
    struct obj742 far *obj;
    struct obj_e2 far *ob;
    int i;

    row = &g_dispatch[g_27ed];
    obj = (struct obj742 far *)row->p_742;
    g_27ae = FUN_1b11_1106(0, 0, row->p_746);
    g_27ae[0x0a] = 1;
    FUN_1b11_11cc(g_27ae, 0);
    FUN_1000_02bf(g_27ae);
    FUN_0ffe_000e(0, 0, 0x140, 0x9b, g_27ae, 0, 0, 0x140, 0x140);

    if (g_27ed) {
        FUN_1b11_1653(g_34ad, obj->f4, 1);
        FUN_1b11_1653(g_34ad, obj->f10, 1);
    }
    for (i = 0; i < 3; i++) {
        FUN_1b11_1653(g_34ad, obj->f0, 1);
    }
    ob = (struct obj_e2 far *)g_27e2;
    g_27f5 = ob->b[0x17];
    g_27f8 = (g_27f5 > 0x96U) ? 1 : 2;
    FUN_1b11_1653(g_34ad, ob->f0f, 1);
    FUN_1b11_1653(g_34ad, ob->f0f + 0xf, 1);
    FUN_1b11_1653(g_34ad, ob->f0f + 0x1e, 1);
    FUN_1b11_1653(g_34ad, ob->f0f + 0x2d, 1);
    g_34a1 = FUN_1b11_1dc1(g_34a9, 0xe4, 0x8a2, 0x154, 0x8a2, 0);
    FUN_0d19_0104(g_34a1, 0x136 - obj->f0[6], 0x140, 0);
    ob->b[0x17] = 2;
    if (strcmp(row->sub[2], g_27ae) != 0) {
        FUN_1f44_0276(g_34ad, 0, 0);
        FUN_1f44_029e(g_34ad, 0);
    }
    ob->fn18();
}

int main(void)
{
    probe();
    return 0;
}