
/* Codegen probes.  Deliberately cover the constructs whose lowering changes
   most between Borland's optimisation settings. */
int p_add(int a, int b) { return a + b; }
int p_mix(int a, int b, int c) { return a * b + c - (a >> 2); }
int p_max(int a, int b) { return a > b ? a : b; }
int p_clampb(int v) { if (v > 255) v = 255; if (v < 0) v = 0; return v; }
int p_sign(int v) { return v < 0 ? -1 : (v > 0 ? 1 : 0); }
long p_widen(int a) { return (long)a * 3; }
void p_bump(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i]++; }
void p_fill(unsigned char *p, int n, unsigned char v) { while (n-- > 0) *p++ = v; }
int p_switch(int x) { switch (x) { case 0: return 10; case 1: return 20; case 5: return 30; default: return -1; } }
long p_lmul(int a, int b) { return (long)a * b; }
int p_deref(long *q) { return (int)(*q + 1); }
void p_call(void (*f)(void)) { f(); }
int p_div(int a, int b) { return a / b; }
unsigned p_udiv(unsigned a, unsigned b) { return a / b; }
int p_shift(long v) { return (int)(v << 3) >> 2; }

/* main() must touch every probe, otherwise -O2 dead-code-eliminates them and
   there is nothing left to compare. */
static unsigned char buf[64];
static long slot;
static void sink(void) { }
void main(void)
{
    slot = 0;
    p_add(1, 2); p_mix(3, 4, 5); p_max(1, 2); p_clampb(300); p_sign(-1);
    p_widen(7); p_bump(buf, 8); p_fill(buf, 8, 3); p_switch(5);
    p_lmul(6, 7); p_deref(&slot); p_call(sink); p_div(9, 3); p_udiv(9u, 3u);
    p_shift(16);
}
