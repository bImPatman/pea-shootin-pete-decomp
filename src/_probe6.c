extern unsigned char far6u(char far *p, char far *s);
extern void fx6(void);
extern void fy6(void);

char far *g_P6;
unsigned char g_a6, g_b6, g_c6;
int g_w6;
unsigned char g_x6;

typedef struct { void (far *fn)(void); unsigned char pad[0x30]; } dp6;
dp6 g_d6[4];

int p61(void) { if (far6u(g_P6, "XX") == 0) fx6(); return 0; }

int p62a(void) { if (((unsigned char)g_P6[0x65] >> 2) & 1) fx6(); return 0; }
int p62b(void) { if (((char)g_P6[0x65] >> 2) & 1) fx6(); return 0; }

int p63(void) { g_d6[g_x6].fn(); return 0; }

int p64(void) { if (g_w6++ == 0xff) g_a6 = 0; return 0; }

int p65a(void) { if (g_a6 == 0 && g_b6 != 0) return 1; fx6(); return 0; }
int p65b(void) { if (g_a6 == 0) { if (g_b6 != 0) return 1; } fx6(); return 0; }
int p65c(void) { if (g_a6 == 0 && g_b6) return 1; fx6(); return 0; }

int p66(void) { if (!g_a6 && !g_b6 && !g_c6) fx6(); else fy6(); return 0; }

int p67(void) { if (g_P6[0x32] != 2) fy6(); return 0; }
int p68(void) { g_P6[0x65] &= (unsigned char)0xfb; return 0; }

int main(void) { return 0; }