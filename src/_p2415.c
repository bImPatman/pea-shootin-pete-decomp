extern unsigned int  g_168c, g_2c01, g_2bff;
extern unsigned char g_349f, g_27f2, g_27f3, g_27f7, g_27ec;

void q1(void) {
    g_168c = 0xBB80; g_349f = 0; g_27f2 = 0;
    g_27f3 = g_27f7 = 0;
    g_2bff = g_2c01 = g_27f3;
    g_27ec = 0;
}

void q2(void) {
    g_168c = 0xBB80; g_349f = 0; g_27f2 = 0;
    g_27f3 = g_27f7 = 0;
    g_2c01 = 0; g_2bff = g_27f3;
    g_27ec = 0;
}

void q3(void) {
    g_168c = 0xBB80; g_349f = 0; g_27f2 = 0;
    g_27f3 = g_27f7 = 0;
    g_2bff = (g_2c01 = 0, g_27f3);
    g_27ec = 0;
}

void q4(void) {
    g_168c = 0xBB80; g_349f = 0; g_27f2 = 0;
    g_27f3 = g_27f7 = 0;
    g_2c01 = (g_2bff = g_27f3);
    g_27ec = 0;
}

void q5(void) {
    g_168c = 0xBB80; g_349f = 0; g_27f2 = 0;
    g_27f3 = g_27f7 = 0;
    g_2bff = (unsigned char)(g_2c01 = 0);
    g_27ec = 0;
}

void q6(void) {
    g_168c = 0xBB80; g_349f = 0; g_27f2 = 0;
    g_27f3 = g_27f7 = 0;
    g_2bff = g_2c01 = (unsigned char)0;
    g_27ec = 0;
}

void q7(void) {
    g_168c = 0xBB80; g_349f = 0; g_27f2 = 0;
    g_27f3 = g_27f7 = 0;
    g_2c01 = (unsigned char)(g_2bff = g_27f3);
    g_27ec = 0;
}
