int __pascal __near farstreq(char far *t, char far *s) {
    while (*s) {
        s++;
        if (*(s-1) != *(t-1)) return 0;
        t++;
    }
    return 1;
}

void main(void){ farstreq((char far*)0,(char far*)0); }
