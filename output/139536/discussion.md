# Handle comma operator for implicit int->enum conversions

**Author:** asmok-g
**URL:** https://github.com/llvm/llvm-project/issues/139536

## Body

[PR](https://github.com/llvm/llvm-project/pull/137658) introduces a new diagnostic group to diagnose implicit casts from int to an enumeration type. In C, this is valid, but it is not compatible with C++.

However, in a case like the following
```
void free ();
typedef enum {REG_EBRACE, REG_BADBR} reg_errcode_t;
typedef struct {int *stack;} compile_stack_type;
reg_errcode_t byte_regex_compile () {
  const char *p;
  const char *pend;
  compile_stack_type compile_stack;
  return (free (compile_stack.stack), p == pend ? REG_EBRACE : REG_BADBR));                                                                                                                                
}
```
cmd: `clang -Wimplicit-int-enum-cast -c pre.i`
The warning is emitted.
```
pre.i:8:39: warning: implicit conversion from 'int' to enumeration type 'reg_errcode_t' is invalid in C++ [-Wimplicit-int-enum-cast]
    8 |   return (free (compile_stack.stack), p == pend ? REG_EBRACE : REG_BADBR));                                                                                                                                
      |                                     ~ ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
pre.i:8:74: error: expected ';' after return statement
    8 |   return (free (compile_stack.stack), p == pend ? REG_EBRACE : REG_BADBR));                                                                                                                                
      |                                                                          ^
      |                                                                          ;
1 warning and 1 error generated.
```

While there actually is an implicit conversion happening in C, it's not happening in C++. The warning is emitted for cpp compatibility, so it should not be emitted in this case. 

[A fix](https://github.com/llvm/llvm-project/pull/138752) was introduced for the case: 
```
void free ();
typedef enum {REG_EESCAPE} reg_errcode_t;
typedef struct {int *stack;} compile_stack_type;
reg_errcode_t byte_regex_compile () {
  compile_stack_type compile_stack;
  return (free (compile_stack.stack), REG_EESCAPE);                                                                                                                                
}
```

But we probably need a more generic fix maybe ?

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c

Author: None (asmok-g)

<details>
[PR](https://github.com/llvm/llvm-project/pull/137658) introduces a new diagnostic group to diagnose implicit casts from int to an enumeration type. In C, this is valid, but it is not compatible with C++.

However, in a case like the following
```
void free ();
typedef enum {REG_EBRACE, REG_BADBR} reg_errcode_t;
typedef struct {int *stack;} compile_stack_type;
reg_errcode_t byte_regex_compile () {
  const char *p;
  const char *pend;
  compile_stack_type compile_stack;
  return (free (compile_stack.stack), p == pend ? REG_EBRACE : REG_BADBR));                                                                                                                                
}
```
cmd: `clang -Wimplicit-int-enum-cast -c pre.i`
The warning is emitted.
```
pre.i:8:39: warning: implicit conversion from 'int' to enumeration type 'reg_errcode_t' is invalid in C++ [-Wimplicit-int-enum-cast]
    8 |   return (free (compile_stack.stack), p == pend ? REG_EBRACE : REG_BADBR));                                                                                                                                
      |                                     ~ ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
pre.i:8:74: error: expected ';' after return statement
    8 |   return (free (compile_stack.stack), p == pend ? REG_EBRACE : REG_BADBR));                                                                                                                                
      |                                                                          ^
      |                                                                          ;
1 warning and 1 error generated.
```

While there actually is an implicit conversion happening in C, it's not happening in C++. The warning is emitted for cpp compatibility, so it should not be emitted in this case. 

[A fix](https://github.com/llvm/llvm-project/pull/138752) was introduced for the case: 
```
void free ();
typedef enum {REG_EESCAPE} reg_errcode_t;
typedef struct {int *stack;} compile_stack_type;
reg_errcode_t byte_regex_compile () {
  compile_stack_type compile_stack;
  return (free (compile_stack.stack), REG_EESCAPE);                                                                                                                                
}
```

But we probably need a more generic fix maybe ?
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (asmok-g)

<details>
[PR](https://github.com/llvm/llvm-project/pull/137658) introduces a new diagnostic group to diagnose implicit casts from int to an enumeration type. In C, this is valid, but it is not compatible with C++.

However, in a case like the following
```
void free ();
typedef enum {REG_EBRACE, REG_BADBR} reg_errcode_t;
typedef struct {int *stack;} compile_stack_type;
reg_errcode_t byte_regex_compile () {
  const char *p;
  const char *pend;
  compile_stack_type compile_stack;
  return (free (compile_stack.stack), p == pend ? REG_EBRACE : REG_BADBR));                                                                                                                                
}
```
cmd: `clang -Wimplicit-int-enum-cast -c pre.i`
The warning is emitted.
```
pre.i:8:39: warning: implicit conversion from 'int' to enumeration type 'reg_errcode_t' is invalid in C++ [-Wimplicit-int-enum-cast]
    8 |   return (free (compile_stack.stack), p == pend ? REG_EBRACE : REG_BADBR));                                                                                                                                
      |                                     ~ ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
pre.i:8:74: error: expected ';' after return statement
    8 |   return (free (compile_stack.stack), p == pend ? REG_EBRACE : REG_BADBR));                                                                                                                                
      |                                                                          ^
      |                                                                          ;
1 warning and 1 error generated.
```

While there actually is an implicit conversion happening in C, it's not happening in C++. The warning is emitted for cpp compatibility, so it should not be emitted in this case. 

[A fix](https://github.com/llvm/llvm-project/pull/138752) was introduced for the case: 
```
void free ();
typedef enum {REG_EESCAPE} reg_errcode_t;
typedef struct {int *stack;} compile_stack_type;
reg_errcode_t byte_regex_compile () {
  compile_stack_type compile_stack;
  return (free (compile_stack.stack), REG_EESCAPE);                                                                                                                                
}
```

But we probably need a more generic fix maybe ?
</details>


---

