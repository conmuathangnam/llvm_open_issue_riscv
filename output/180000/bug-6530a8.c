# 1 "<built-in>"
# 1 "bug.c"
int a;
_Bool b[][3];
void c(_Bool d, char e, short g, int h, int i[][4][4], _Bool j,
       _Bool k[][0][20]) {
  for (short l = 0; l < 3; l += d) {
    for (short m = 0; m < g - 7; m += 3) {
      a = h ? ({
        __typeof__(-k[2][m][m]) f = -k[2][m][m];
        f;
      })
            : 0;
      b[l][m] = (e ? g : 0) ? j ? 2 : d : i[l][m][m];
      for (int n; n < 4; n += 050)
        if (!e)
          __builtin_unreachable();
    }
    for (short o; o; o += 3)
      ;
  }
}
