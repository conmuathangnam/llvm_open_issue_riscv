# 1 "<built-in>"
# 1 "bug.c"
short a[][3][3];
void b(char c, int d, int f, int g, int h, int i[], int j, _Bool k[][3][3],
       short l[][3][3]) {
  for (int m;;)
    for (char n; j ?: k[n][n][n];) {
      for (unsigned o; o < c; o++)
        for (int p = 0; p < 23; p += 4)
          a[o][n][o] = o % 3 == h ? l[o][m][o] >> ({__typeof__(n) e = i[n];e;}) % p : (d ? f : 0) ? : l[o][m][o] ;
      __builtin_assume((_Bool)g + (_Bool)f - 1 == 1);
      for (_Bool q = 0; q < g; q = f)
        ;
    }
}
