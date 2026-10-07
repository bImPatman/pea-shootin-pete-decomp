extern void fa(int x);
extern void fc(void);

unsigned char g_A, g_B;      /* if (uc) */
int g_D, g_E;                /* if (int) */
unsigned int g_F, g_G;       /* if ((unsigned char)int) */
unsigned char g_H, g_I;      /* if ((int)uc-ish? via char) */
volatile unsigned char g_J, g_K;   /* if (volatile uc) */
char g_L, g_M;               /* if (char) */
unsigned char g_N;           /* g_27ed++ shape */

int pA(void) { if (g_A) fa(1); else if (g_B) fa(2); return 0; }
int pC(void) { if (g_D) fa(1); else if (g_E) fa(2); return 0; }
int pD(void) { if ((unsigned char)g_F) fa(1); else if ((unsigned char)g_G) fa(2); return 0; }
int pE(void) { if ((unsigned char)g_H) fa(1); else if ((unsigned char)g_I) fa(2); return 0; }
int pF(void) { if (g_J) fa(1); else if (g_K) fa(2); return 0; }
int pG(void) { if (g_L) fa(1); else if (g_M) fa(2); return 0; }
int pB(void) { g_N++; if (g_N == 3) fc(); return 0; }