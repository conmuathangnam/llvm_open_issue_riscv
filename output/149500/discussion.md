# Initialization of global pointers is accepted by clang++ but rejected by clang

**Author:** benshi001
**URL:** https://github.com/llvm/llvm-project/issues/149500
**Status:** Closed
**Labels:** c, clang:frontend, regression, diverges-from:gcc, diverges-from:msvc
**Closed Date:** 2025-07-19T17:30:17Z

## Body

For the following code,

```c++
unsigned int * p = &(*(unsigned int *)0x400);
```

clang++ accepts it, but clang rejects with error 
```bash
a.c:1:20: error: initializer element is not a compile-time constant
    1 | unsigned int * p = &(*(unsigned int *)0x400);
      |  
```



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ben Shi (benshi001)

<details>
For the following code,

```c++
unsigned int * p = &amp;(*(unsigned int *)0x400);
```

clang++ accepts it, but clang rejects with error 
```bash
a.c:1:20: error: initializer element is not a compile-time constant
    1 | unsigned int * p = &amp;(*(unsigned int *)0x400);
      |  
```


</details>


---

### Comment 2 - benshi001

This code looks strange but is used common in embedded devices, such as AVR.

```c++
#define PORT (*(unsigned int *)0x400)
unsigned int *p = &PORT;
```

---

### Comment 3 - frederick-vs-ja

I think such initialization should be valid.

`&(*(unsigned int *)0x400)` seems to be an address constant in C per C23/[N3220](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf) 6.6/11, /12:
> 11. An address constant is a null pointer,<sup>118)</sup> a pointer to an lvalue designating an object of static storage duration, or a pointer to a function designator; it shall be created explicitly using the unary `&` operator or an integer constant cast to pointer type, or implicitly using an expression of array or function type.
> 12. The array-subscript `[]` and member-access `->` operator, the address `&` and indirection `*` unary operators, and pointer casts may be used in the creation of an address constant, but the value of an object shall not be accessed by use of these operators.<sup>119)</sup>

Note that  6.5.4.1/13:
> The unary `&` operator yields the address of its operand. If the operand has type "type", the result has type "pointer to type". If the operand is the result of a unary * operator, neither that operator nor the `&` operator is evaluated and the result is as if both were omitted, except that the constraints on the operators still apply and the result is not an lvalue. [...]

(Such wording was added in C99 in order to resolve [DR076](https://www.open-std.org/jtc1/sc22/wg14/www/docs/dr_076.html), so I think it should also apply to C89 mode.)

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-c

Author: Ben Shi (benshi001)

<details>
For the following code,

```c++
unsigned int * p = &amp;(*(unsigned int *)0x400);
```

clang++ accepts it, but clang rejects with error 
```bash
a.c:1:20: error: initializer element is not a compile-time constant
    1 | unsigned int * p = &amp;(*(unsigned int *)0x400);
      |  
```


</details>


---

### Comment 5 - AaronBallman

> More latitude is permitted for constant expressions in initializers. Such a constant expression shall
> be, or evaluate to, one of the following:
> — a named constant,

The macro does not expand to a named constant.

> — a compound literal constant,

Nor a compound literal constant

> — an arithmetic constant expression,

Nor an arithmetic constant expression

> — a null pointer constant,

Nor a null pointer constant

> — an address constant, or
> — an address constant for a complete object type plus or minus an integer constant expression.

Nor an address constant:

> An address constant is a null pointer,117) a pointer to an lvalue designating an object of static storage duration, or a pointer to a function designator; it shall be created explicitly using the unary & operator or an integer constant cast to pointer type, or implicitly using an expression of array or function type.

It's not an address constant because the pointer isn't pointing to either an object of static storage duration or function designator.

So I believe Clang is correct to reject this code, at least per the standard. I think the most practically portable way to write that code is:
```
#define PORT ((unsigned int *)0x400)
unsigned int *p = PORT;
```
https://godbolt.org/z/doYs1h4oc

It's not fully portable because it relies on this:
> An implementation may accept other forms of constant expressions; however, it is implementationdefined whether they are an integer constant expression.


---

### Comment 6 - benshi001

https://www.godbolt.org/z/63oe59rs6

clang 20 accepts it ,but current trunk clang rejects it.

---

### Comment 7 - benshi001

cc @beakthoven, this issue would affect compiling of the arduino core libraries.

---

### Comment 8 - AaronBallman

> https://www.godbolt.org/z/63oe59rs6
> 
> clang 20 accepts it ,but current trunk clang rejects it.

Good point! I hadn't realized this behavior changed recently, so thank you for pointing this out. Nothing jumps out at me in the releases notes as a cause, so we should probably bisect to see which commit changed the behavior. CC @shafik for help with bisection (though anyone is welcome to help out with that)

Temporarily adding the regression label just so it gets more attention as we work on the release. This may be an intentional change that didn't get release noted properly, it may be an unintentional change we want to address.

---

### Comment 9 - benshi001

> > https://www.godbolt.org/z/63oe59rs6
> > clang 20 accepts it ,but current trunk clang rejects it.
> 
> Good point! I hadn't realized this behavior changed recently, so thank you for pointing this out. Nothing jumps out at me in the releases notes as a cause, so we should probably bisect to see which commit changed the behavior. CC [@shafik](https://github.com/shafik) for help with bisection (though anyone is welcome to help out with that)
> 
> Temporarily adding the regression label just so it gets more attention as we work on the release. This may be an intentional change that didn't get release noted properly, it may be an unintentional change we want to address.

I will appreciate if this issue can be fixed. Because many popular AVR libraries are written in this way. I am work on substituting avr-gcc with clang-llvm in embedded systems.

---

### Comment 10 - AaronBallman

Good to know! Given the latitude we have for accepting other forms of constant expressions and the disruption a change in behavior can cause for users, I think we probably should do something. Perhaps that something is "downgrade to an extension warning", but let's see what a bisect tells us caused the issue in the first place.

---

### Comment 11 - shafik

Looks like this was introduced sometimes in the past couple of days maybe one of @efriedma-quic recent changes inadvertently changed this. I will bisect soon.

---

### Comment 12 - efriedma-quic

I suspect #143667 .

---

### Comment 13 - cor3ntin

I will look into it in the next few days.

---

### Comment 14 - shafik

> I suspect [#143667](https://github.com/llvm/llvm-project/pull/143667) .

Bingo, git bisect says you have a winner 9e5470e7d6ea1ad4fe25a9416706d769e41a03c1

CC @cor3ntin 

---

### Comment 15 - beakthoven

> cc [@beakthoven](https://github.com/beakthoven), this issue would affect compiling of the arduino core libraries.

Yup, the HEAD build seems to be failing to compile the Arduino core libs 
```
ClangBuiltArduino/hardware/avr/1.0.1/variants/mega/pins_arduino.h:116:2: error: initializer element is not a compile-time constant
  116 |         (uint16_t) &DDRA,
      |         ^~~~~~~~~~~~~~~~
/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega/pins_arduino.h:132:2: error: initializer element is not a compile-time constant
  132 |         (uint16_t) &PORTA,
      |         ^~~~~~~~~~~~~~~~~
/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega/pins_arduino.h:148:2: error: initializer element is not a compile-time constant
  148 |         (uint16_t) &PINA,
      |         ^~~~~~~~~~~~~~~~
3 errors generated.
```

Tracing the issue shows that several `avr-libc` headers follow the same pattern:
https://github.com/avrdudes/avr-libc/blob/156172515d32f8a3db1b390681bebf052e9b3d59/include/avr/sfr_defs.h#L126-L128

Arduino framework code that relies on these headers fails to compile.
https://github.com/ClangBuiltArduino/core_arduino_avr/blob/master/variants/mega/pins_arduino.h#L114-L128

---

### Comment 16 - frederick-vs-ja

> Nor an address constant:
> 
> > An address constant is a null pointer,117) a pointer to an lvalue designating an object of static storage duration, or a pointer to a function designator; it shall be created explicitly using the unary & operator or an integer constant cast to pointer type, or implicitly using an expression of array or function type.
> 
> It's not an address constant because the pointer isn't pointing to either an object of static storage duration or function designator.

Thanks for correction! So, the allowance for "an integer constant cast to pointer type" looks quite superfluous (unless the result is a null pointer), because the same numeric address can be either qualified as an address constant or not in different executions.

---

