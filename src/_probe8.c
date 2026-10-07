extern int    far6i(char far *p, char far *s);
extern unsigned char far6u(char far *p, char far *s);
extern void fx6(void);
extern void fy6(void);

char far *g_P6;
unsigned char g_a6, g_b6, g_c6;

/* tail: target = cmp [f9],0 / jne cont / mov al,[f6] / mov ah,0 / or ax,ax / jne ret */
int q9a(void) { if (g_a6 != 0 || !g_b6) fx6(); else return 7; return 0; }
int q9b(void) { if (g_a6 != 0 || g_b6 == 0) fx6(); else return 7; return 0; }
int q9c(void) { if (g_a6 == 0 && g_b6) return 7; fx6(); return 0; }
int q9d(void) { if ((unsigned char)g_b6) fx6(); return 0; }

/* far return test: target = add sp,8 / mov ah,0 / or ax,ax / jne */
int q61c(void) { if (far6i(g_P6, "XX") & 0xff) fx6(); return 0; }
int q61d(void) { if (far6u(g_P6, "XX") != 0) fx6(); return 0; }
int q61e(void) { if (far6i(g_P6, "XX") != 0) fx6(); return 0; }

/* bit test: want shr ax,2 / and ax,1 / or ax,ax (16-bit) */
struct bfc { unsigned char pad[0x65]; unsigned char : 2; unsigned char b2 : 1; };
struct bfi { unsigned char pad[0x65]; unsigned int  : 2; unsigned int  b2 : 1; };
struct bfw { unsigned char pad[0x65]; unsigned      : 2; unsigned      b2 : 1; };
int q62b2(void) { if (((struct bfi far *)g_P6)->b2) fx6(); return 0; }
int q62b3(void) { if (((struct bfw far *)g_P6)->b2) fx6(); return 0; }
int q62b4(void) { if (((struct bfc far *)g_P6)->b2 == 1) fx6(); return 0; }

int main(void) { return 0; }