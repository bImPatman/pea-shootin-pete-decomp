/* Stand-in for FUN_1e25_0426 (segment 0xe25): one far pointer in, unsigned
   char out -- the caller immediately does `mov ah,0 / or ax,ax`. */
unsigned char FUN_1e25_0426(char far *p) { (void)p; return 0; }
