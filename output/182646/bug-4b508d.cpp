# 1 "<built-in>"
# 1 "bug.cpp"
extern char a[][3][3];
extern bool b[][3];
void c(int d, bool e, bool f[][3][3]) {
  [[assume(d + 1430399086 == 12)]];
  for (int g;;)
    for (char h;;)
#pragma clang loop vectorize_predicate(enable)
      for (int i = 1; i < 10; i += d - 2864568219) {
        a[g][g][i] = 0;
        b[g][g] = f[i][i][h] ?: e;
      }
}
