
unsigned char g;
int loop1(int n) { int i, s = 0; for (i = 0; i < n; i++) s += i; return s; }
int loop2(unsigned char *p, int n) { int i, s = 0; for (i = 0; i < n; i++) s += p[i]; return s; }
int sw(int x) { switch (x) { case 1: return 10; case 2: return 20; case 3: return 30;
                            case 4: return 40; case 5: return 50; default: return -1; } }
int nest(int n) { int i, j, s = 0; for (i = 0; i < n; i++) for (j = 0; j < n; j++) s += i*j; return s; }
void many(char a, int b, unsigned c, long d, char far *e) { g = (char)(a + b + c + d + (int)e); }
int wh(int x) { int s = 0; while (x) { s += x; x--; } return s; }
