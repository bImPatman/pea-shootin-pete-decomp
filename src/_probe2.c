extern unsigned char g_349f;
extern char far *g_3858;

void f02_iv(char far *p, int v);
void f02_ucv(char far *p, unsigned char v);
void f02_cv(char far *p, char v);

void probe_i(void)
{
    f02_iv(g_3858, g_349f);
}

void probe_uc(void)
{
    f02_ucv(g_3858, g_349f);
}

void probe_c(void)
{
    f02_cv(g_3858, g_349f);
}