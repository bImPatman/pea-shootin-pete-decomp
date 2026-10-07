extern int    far6i(char far *p, char far *s);
extern void fx6(void);
extern void fy6(void);

char far *g_P6;
unsigned char g_a6, g_b6, g_c6;
unsigned char far *g_pc;

/* ---- p61: far call return test (target: add sp,8 / mov ah,0 / or ax,ax) ---- */
int q61a(void) { if ((unsigned char)far6i(g_P6, "XX") == 0) fx6(); return 0; }
int q61b(void) { if (far6i(g_P6, "XX") == 0) fx6(); return 0; }

/* ---- p62: bit test (target: mov al,es:[bx+0x65] / shr ax,2 / and ax,1 / or ax,ax / je) ---- */
struct bf65 { unsigned char pad[0x65]; unsigned char bl : 2; unsigned char b2 : 1; };

int q62a(void) { if ((g_pc[0x65] >> 2) & 1) fx6(); return 0; }
int q62b(void) { if (((struct bf65 far *)g_P6)->b2) fx6(); return 0; }
int q62c(void) { if (((struct bf65 far *)g_P6)->b2 != 0) fx6(); return 0; }
int q62d(void) { ((struct bf65 far *)g_P6)->b2 = 0; return 0; }

/* ---- p7: tail (target: cmp [g_27f9],0 / jne continue / mov al,[g_27f6] / mov ah,0 / or ax,ax / jne return) ---- */
int q7a(void) { if (g_a6 == 0 && g_b6 != 0) return 1; fx6(); return 0; }
int q7b(void) { if (g_a6 == 0 && g_b6) return 1; fx6(); return 0; }
int q7c(void) { if (g_a6 == 0) { if (g_b6 != 0) return 1; } fx6(); return 0; }
int q7d(void) { if (g_a6 == 0 && !g_b6 == 0) return 1; fx6(); return 0; }
int q7e(void) { if (!(g_a6 != 0 || g_b6 != 0)) return 1; fx6(); return 0; }
int q7f(void) { if (!(g_a6 || g_b6)) return 1; fx6(); return 0; }
int q7g(void) { if (g_a6 == 0 && (unsigned char)(g_b6 != 0)) return 1; fx6(); return 0; }
int q7h(void) { if (g_a6 == 0) { if (!g_b6) return 1; } fx6(); return 0; }

int main(void) { return 0; }