void g(void) {}
unsigned char far *P;

struct bi { unsigned char pad[0x65]; unsigned x0 : 5; unsigned b5 : 1; unsigned b6 : 1; unsigned x7 : 1;
            unsigned y0 : 5; unsigned c5 : 1; unsigned c6 : 1; unsigned x15 : 1; unsigned rest : 8; };
struct bi2 { unsigned char pad[0x65]; int x0 : 5; int b5 : 1; int b6 : 1; int x7 : 1;
             int y0 : 5; int c5 : 1; int c6 : 1; int x15 : 1; int rest : 8; };

void c1(struct bi far *o) { if (o->b5 == 0) g(); }
void c2(struct bi far *o) { if (!o->b5) g(); }
void c3(struct bi far *o) { if (o->b5) g(); else g(); }
void d1(struct bi2 far *o) { if (o->b5 == 0) g(); }
void d2(struct bi2 far *o) { if (!o->b5) g(); }
void e1(struct bi far *o) { if (o->c6) g(); else g(); }

int main(void) { return 0; }
