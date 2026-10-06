struct rec { unsigned int lo, hi; char pad[6]; };
int g_find(unsigned int klo, unsigned int khi)
{
    int n = 0;
    struct rec near *r = (struct rec near *)0x34c5;
    do {
        if (r->hi <= khi) {
            if (r->hi < khi || r->lo < klo)
                return (unsigned char)n + 1;
        }
        r++;
        n++;
    } while (r != (struct rec near *)0x3551);
    return 0;
}

void main(void){ g_find(0,0); }
