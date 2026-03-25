# 1 "<built-in>"
# 1 "red.c"
int a, e;
long **b;
static char c[7][9];
char d, f;
void g() {
  char *h[] = {&c[6][7], &f};
  long i;
  a = 0;
  for (; a <= 8; a++) {
    e = 6;
    for (; e; e--) {
      c[e][1] = c[0][8];
      i = **b > 0;
      d |= i;
    }
  }
}
