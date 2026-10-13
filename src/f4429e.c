/* @target FUN_1f44_029e */
/* @name   FUN_1f44_029e */
/* @proto  void FUN_1f44_029e(char far *p, unsigned char v) */
/* @module cross */
/* @extra  g13b2.c xmod/xfd3150.c */

extern unsigned int g_19e4[];
extern void FUN_1fd3_0150(int v);
extern int rand(void);

struct node_029e {
    unsigned char a;
    unsigned char cnt;
    unsigned char pad[2];
    unsigned int w4;
};

void FUN_1f44_029e(struct node_029e far *p, unsigned char v)
{
    if (p->a != 0) {
        p->w4 = (rand() % 5) * 80;
        if (p->cnt-- == 0)
            p->a = 0;
        FUN_1fd3_0150(g_19e4[v] + p->w4);
    } else {
        FUN_1fd3_0150(g_19e4[v]);
    }
}
