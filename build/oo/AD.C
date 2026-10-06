int g;
int f(int n){ static int a; a=n; g+=a; return a; }
