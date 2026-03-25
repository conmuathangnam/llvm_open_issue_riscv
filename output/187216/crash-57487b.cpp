# 1 "<built-in>"
# 1 "crash.cpp"
char e;
extern char q;
long r;
int ac;
extern short ad[];
extern bool ae[][100];
extern char af[];
extern int ag[][100];
template < typename _Tp > _Tp max( _Tp ,  _Tp );
short a, ah;
int *b;
short *c;
char *d;
void u(char f, short g, int i, char ai, int j[][7][7], int k[][7][7],
       short l[][4][4], short m[][0][0], short n[][0][0], int o[][0][10],
       int p[][0]) {
  char h;
  e = ai;
  for (short s; s < 1; s += 3)
    [[assume(ai - 1 == 0)]];
  for (int t = 0; t < 4; t = 7) {
    [[assume(f)]];
    [[assume(!i ? 0 : 7)]];
  }
  if (f)
    if ((f ? g ? i : 6 : 0) ? 8 ? ai ^ ah : 0 : 4073709551615 ? (long)i : 0);
    else
      for (char v = 0; v < 10; v++) {
        for (int w = 1; w; w = h)
          [[assume(g)]];
            for (char x = 0; x < 10; x++) {
          for (int y = 0; y < 10; y += 3) {
            ad[v] = l[3][3][1] ? l[3][1][3] : m[v][3][1];
            ae[v][y] = p[x][x] ?: b[x];
          }
          for (int z = 0; z < 10; z++)
            for (char aa = 2; aa < 8; aa += 3) {
              af[x] |= (7 ? a : 0) ? 0 : n[6][6][z];
              q += 8 ? o[v][x][z] ? j[x][z][5] : 0 : 0;
              ag[v][x * 10 + z] = ah | c[3] ? 0 : d[4];
              r = 0;
            }
        }
      }
  for (char ab(g);; ab = i) {
    max((unsigned)k[ab][ab][ab], 9U);
    ac = ((2 ? f : 0));
  }
}
