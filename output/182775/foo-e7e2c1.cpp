# 1 "foo.cc"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 489 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "foo.cc" 2
long* b;
void p();
void q() {
  for (unsigned f;; ++f)
    for (unsigned g = f; g < 4; ++g)
      for (; b[g];)
        for (unsigned h = g; h; ++h)
          for (unsigned i = h; i; ++i)
            for (unsigned a = i; a; ++a)
              for (unsigned j = a; j; ++j)
                for (unsigned k = j; k; ++k)
                  for (unsigned c = k; c < 4; ++c)
                    for (unsigned n = c; n < 4; ++n)
                      for (unsigned l = n; l < 4; ++l) {
                        p();
                        for (unsigned m = l; m < 4; ++m)
                          for (long o = (&b)[0][m]; o; o &= 1)
                            ;
                      }
}
