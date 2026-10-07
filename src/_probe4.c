extern void fa(int x);
extern void fc(void);

unsigned char g_A, g_B;
unsigned char g_t;
unsigned char vEc, vEd, vF, vG;

int pT(void) { unsigned char t = g_A; if (t) fa(1); else if (g_B) fa(2); return 0; }
int pU(void) { if (g_A) fa(1); else if (g_B == 0) fa(2); return 0; }
int pW(void) { if (g_A != 0) fa(1); else if (g_B != 0) fa(2); return 0; }
int pX(void) { int v = g_t; if (v) fa(1); return 0; }
int pY(void) { if ((int)g_t) fa(1); return 0; }
int pZ(void) { vEc++; if (vF) { unsigned char t = (unsigned char)(vEd + 1); vEd = t; if (t == 3) { vG = 1; fc(); } vEc = 0; } return 0; }
int pAA(void) { vEc++; if (vF) { vEd++; if (vEd == 3) { vG = 1; fc(); } vEc = 0; } return 0; }
int pBB(void) { vEc++; if (vF) { vEd = (unsigned char)(vEd + 1); if (vEd == 3) { vG = 1; fc(); } vEc = 0; } return 0; }