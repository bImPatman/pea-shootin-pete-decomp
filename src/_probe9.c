extern int    far6i(char far *p, char far *s);
extern unsigned char far6u(char far *p, char far *s);
extern char   far6c(char far *p, char far *s);
extern void fx6(void);

char far *g_P6;

int q61f(void) { if (!far6u(g_P6, "XX")) fx6(); return 0; }
int q61g(void) { if (!far6i(g_P6, "XX")) fx6(); return 0; }
int q61h(void) { if (!far6c(g_P6, "XX")) fx6(); return 0; }
int q61k(void) { if (!(unsigned char)far6i(g_P6, "XX")) fx6(); return 0; }

int main(void) { return 0; }