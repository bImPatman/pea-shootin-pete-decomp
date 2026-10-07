extern void fa(int x);
extern void fb(char far *p);
extern void fc(void);

unsigned char g_A, g_B;
char far *g_P;

int pG1(void) { if (!g_A) { if (!g_B) fb(g_P); } else { g_A = 1; fc(); } return 0; }
int pG2(void) { if (g_A == 0) { if (g_B == 0) fb(g_P); } else { g_A = 1; fc(); } return 0; }
int pG3(void) { if (!g_A) fb(g_P); return 0; }
int pG4(void) { unsigned char t = (unsigned char)(!g_A); if (t) fb(g_P); return 0; }