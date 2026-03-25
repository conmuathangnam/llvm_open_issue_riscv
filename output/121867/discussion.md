# LLVM ERROR: out of memory

**Author:** ym-xie
**URL:** https://github.com/llvm/llvm-project/issues/121867

## Body

code:
```c
/* Type your code here, or load an example. */
int square(int num) {
    return num * num;
}
extern int sprintf(char *, const char *, ...);
void *sink(void *);
void sprintf_S_a8_an_bn(int n, int i, int j) {
  struct {
    char a8[8], an[n], bn[n];
  } *p = sink(0);
  {
    char *d = p->a8 + i;
    char *s = p->a8;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->a8;
    char *s = p->a8 + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->a8 + i;
    char *s = p->a8 + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->a8 + i;
    char *s = p->an;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->a8;
    char *s = p->an + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->a8 + i;
    char *s = p->an + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->a8 + i;
    char *s = p->bn;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->a8;
    char *s = p->bn + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    static char *name[] = {[0x80000000] = "bar"};
  }
  {
    char *d = p->an + i;
    char *s = p->bn;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->an;
    char *s = p->bn + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->an + i;
    char *s = p->bn + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->an + i;
    char *s = p->a8;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->an;
    char *s = p->a8 + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->an + i;
    char *s = p->a8 + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->bn + i;
    char *s = p->a8;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->bn;
    char *s = p->a8 + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->bn + i;
    char *s = p->a8 + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->bn + i;
    char *s = p->bn;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->bn;
    char *s = p->bn + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p->bn + i;
    char *s = p->bn + j;
    sprintf(d, "%s", s);
    sink(p);
  }
}
```
command:
```shell
clang -x c -std=c2x -c file.c -o /dev/null
```
https://godbolt.org/z/Y3nWKPErE

## Comments

### Comment 1 - EugeneZelenko

This is due `name` definition: https://godbolt.org/z/P61f44vzs

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (wxie7)

<details>
code:
```c
/* Type your code here, or load an example. */
int square(int num) {
    return num * num;
}
extern int sprintf(char *, const char *, ...);
void *sink(void *);
void sprintf_S_a8_an_bn(int n, int i, int j) {
  struct {
    char a8[8], an[n], bn[n];
  } *p = sink(0);
  {
    char *d = p-&gt;a8 + i;
    char *s = p-&gt;a8;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;a8;
    char *s = p-&gt;a8 + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;a8 + i;
    char *s = p-&gt;a8 + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;a8 + i;
    char *s = p-&gt;an;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;a8;
    char *s = p-&gt;an + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;a8 + i;
    char *s = p-&gt;an + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;a8 + i;
    char *s = p-&gt;bn;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;a8;
    char *s = p-&gt;bn + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    static char *name[] = {[0x80000000] = "bar"};
  }
  {
    char *d = p-&gt;an + i;
    char *s = p-&gt;bn;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;an;
    char *s = p-&gt;bn + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;an + i;
    char *s = p-&gt;bn + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;an + i;
    char *s = p-&gt;a8;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;an;
    char *s = p-&gt;a8 + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;an + i;
    char *s = p-&gt;a8 + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;bn + i;
    char *s = p-&gt;a8;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;bn;
    char *s = p-&gt;a8 + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;bn + i;
    char *s = p-&gt;a8 + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;bn + i;
    char *s = p-&gt;bn;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;bn;
    char *s = p-&gt;bn + j;
    sprintf(d, "%s", s);
    sink(p);
  }
  {
    char *d = p-&gt;bn + i;
    char *s = p-&gt;bn + j;
    sprintf(d, "%s", s);
    sink(p);
  }
}
```
command:
```shell
clang -x c -std=c2x -c file.c -o /dev/null
```
https://godbolt.org/z/Y3nWKPErE
</details>


---

