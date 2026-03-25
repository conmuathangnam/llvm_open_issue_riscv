# Clang Segmentation fault: clang::Lexer::getSpelling()

**Author:** bi6c
**URL:** https://github.com/llvm/llvm-project/issues/128967

## Body

Compiler explorer (from clang 18.1.0): https://godbolt.org/z/z1K5r4jEx
code:
```c
#define LIM1(x)                                                                \
  x##0b0 : x##1 : x##2 : x##3 : x##4 : x##5 : x##6 : x##7 : x##8 : x##9:
#define LIM2(x)                                                                \
  LIM1(x##0b0)                                                                   \
  LIM1(x##1) LIM1(x##2) LIM1(x##3) LIM1(x##4) LIM1(x##5) LIM1(x##6) LIM1(x##7) \
      LIM1(x##8) LIM1(x##9)
#define LIM3(x)                                                                \
  LIM2(x##0b0)                                                                   \
  LIM2(x##1) LIM2(x##2) LIM2(x##3) LIM2(x##4) LIM2(x##5) LIM2(x##6) LIM2(x##7) \
      LIM2(x##8) LIM2(x##9)
#define LIM4(x)                                                                \
  LIM3(x##0b0)                                                                   \
  LIM3(x##1) LIM3(x##2) LIM3(x##3) LIM3(x##4) LIM3(x##5) LIM3(x##6) LIM3(x##7) \
      LIM3(x##8) LIM3(x##9)
#define LIM5(x)                                                                \
  LIM4(x##0b0)                                                                   \
  LIM4(x##1) LIM4(x##2) LIM4(x##3) LIM4(x##4) LIM4(x##5) LIM4(x##6) LIM4(x##7) \
      LIM4(x##8) LIM4(x##9)
#define LIM6(x)                                                                \
  LIM5(x##0b0)                                                                   \
  LIM5(x##1) LIM5(x##2) LIM5(x##3) LIM5(x##4) LIM5(x##5) LIM5(x##6) LIM5(x##7) \
      LIM5(x##8) LIM5(x##9)
#define LIM7(x)                                                                \
  LIM6(x##0b0)                                                                   \
  LIM6(x##1) LIM6(x##2) LIM6(x##3) LIM6(x##4) LIM6(x##5) LIM6(x##6) LIM6(x##7) \
      LIM6(x##8) LIM6(x##9)

void q19_func(char i) {
  switch (i) {
    LIM5(case 1)
    break;
  }
}
``` 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (bi6c)

<details>
Compiler explorer (from clang 18.1.0): https://godbolt.org/z/z1K5r4jEx
code:
```c
#define LIM1(x)                                                                \
  x##<!-- -->0b0 : x##<!-- -->1 : x##<!-- -->2 : x##<!-- -->3 : x##<!-- -->4 : x##<!-- -->5 : x##<!-- -->6 : x##<!-- -->7 : x##<!-- -->8 : x##<!-- -->9:
#define LIM2(x)                                                                \
  LIM1(x##<!-- -->0b0)                                                                   \
  LIM1(x##<!-- -->1) LIM1(x##<!-- -->2) LIM1(x##<!-- -->3) LIM1(x##<!-- -->4) LIM1(x##<!-- -->5) LIM1(x##<!-- -->6) LIM1(x##<!-- -->7) \
      LIM1(x##<!-- -->8) LIM1(x##<!-- -->9)
#define LIM3(x)                                                                \
  LIM2(x##<!-- -->0b0)                                                                   \
  LIM2(x##<!-- -->1) LIM2(x##<!-- -->2) LIM2(x##<!-- -->3) LIM2(x##<!-- -->4) LIM2(x##<!-- -->5) LIM2(x##<!-- -->6) LIM2(x##<!-- -->7) \
      LIM2(x##<!-- -->8) LIM2(x##<!-- -->9)
#define LIM4(x)                                                                \
  LIM3(x##<!-- -->0b0)                                                                   \
  LIM3(x##<!-- -->1) LIM3(x##<!-- -->2) LIM3(x##<!-- -->3) LIM3(x##<!-- -->4) LIM3(x##<!-- -->5) LIM3(x##<!-- -->6) LIM3(x##<!-- -->7) \
      LIM3(x##<!-- -->8) LIM3(x##<!-- -->9)
#define LIM5(x)                                                                \
  LIM4(x##<!-- -->0b0)                                                                   \
  LIM4(x##<!-- -->1) LIM4(x##<!-- -->2) LIM4(x##<!-- -->3) LIM4(x##<!-- -->4) LIM4(x##<!-- -->5) LIM4(x##<!-- -->6) LIM4(x##<!-- -->7) \
      LIM4(x##<!-- -->8) LIM4(x##<!-- -->9)
#define LIM6(x)                                                                \
  LIM5(x##<!-- -->0b0)                                                                   \
  LIM5(x##<!-- -->1) LIM5(x##<!-- -->2) LIM5(x##<!-- -->3) LIM5(x##<!-- -->4) LIM5(x##<!-- -->5) LIM5(x##<!-- -->6) LIM5(x##<!-- -->7) \
      LIM5(x##<!-- -->8) LIM5(x##<!-- -->9)
#define LIM7(x)                                                                \
  LIM6(x##<!-- -->0b0)                                                                   \
  LIM6(x##<!-- -->1) LIM6(x##<!-- -->2) LIM6(x##<!-- -->3) LIM6(x##<!-- -->4) LIM6(x##<!-- -->5) LIM6(x##<!-- -->6) LIM6(x##<!-- -->7) \
      LIM6(x##<!-- -->8) LIM6(x##<!-- -->9)

void q19_func(char i) {
  switch (i) {
    LIM5(case 1)
    break;
  }
}
``` 
</details>


---

### Comment 2 - zyn0217

Is this from a fuzzer?

---

### Comment 3 - MacDue

Given the input (and the crash log), this is very likely just a stack overflow in parsing (due to the exponential expansion). 

---

### Comment 4 - bi6c

> Is this from a fuzzer?

yes

---

### Comment 5 - zyn0217

> > Is this from a fuzzer?
> 
> yes

We prefer using the distinct label ‘fuzzer generated’ to mark fuzzer-based issues. If you don’t have access to label the issue yourself, please clarify the origin of your examples. Thanks.

---

### Comment 6 - bi6c

I see. Thanks.

> > > Is this from a fuzzer?
> > 
> > 
> > yes
> 
> We prefer using the distinct label ‘fuzzer generated’ to mark fuzzer-based issues. If you don’t have access to label the issue yourself, please clarify the origin of your examples. Thanks.



---

### Comment 7 - ecnelises

This is likely a stack overflow due to deep level of recursions (ParseStatementOrDeclarationAfterAttributes -> ParseCaseStatement -> ParseStatementOrDeclaration and then again).

In my environment cc1 crashes at `clang::Lexer::getSpelling`.

A more intuitive way to get reproducer is generate many case lines with invalid integer literal:

```python
with open('pr128967.c', 'w', encoding='utf-8') as f:
    # the number 6676 may vary depending on machines
    case_lines = ''.join([f'case {i}b:\n' for i in range(1, 6676)])
    c_code = f'void q19_func(int i) {{\nswitch (i) {{\n{case_lines}break; }} }}'
    f.write(c_code)
```

---

