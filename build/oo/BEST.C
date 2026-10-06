unsigned char  g_find(unsigned int klo, unsigned int khi)
{
    int n = 0;
    unsigned int near *p = (unsigned int near *)0x34c5;
    do {
        if (p[1] <= khi) {
            if (p[1] < khi || p[0] < klo)
                return n + 1;
        }
        p += 7;
        n++;
    } while (p != 0x3551);
    return 0;
}
