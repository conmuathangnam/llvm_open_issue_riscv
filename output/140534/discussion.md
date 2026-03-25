# Clang 19 regression: SIGFPE on -O2

**Author:** roe85
**URL:** https://github.com/llvm/llvm-project/issues/140534
**Status:** Closed
**Labels:** backend:X86, miscompilation
**Closed Date:** 2025-05-20T08:33:29Z

## Body

I tried compiling our codebase and I get SIGFPE in an innocent looking function. 
I've added `noinline`, to isolate it further. When called deep in the call tree it crashes with SIGFPE (which should be impossible with the given datatypes) 
If I call it in an isolated unit-test with the very same parameters `(3,0, 200)` it works as expected

```
[[clang::noinline]]
bool checkIfObjTooBig(uint size_radial, uint size_azimuthal, uint max)
{
    return (size_azimuthal > sqrt(2) * max ||
            size_radial > sqrt(2) * max);
}
```
The generated Assembly with the supposed crash location.

```
mov    %esi,%eax
cvtsi2sd %rax,%xmm0
mov    %edx,%eax
cvtsi2sd %rax,%xmm1
mulsd  0x2eb12(%rip),%xmm1        # 0x57bfb8
mov    %edi,%eax
cvtsi2sd %rax,%xmm2
movapd %xmm1,%xmm3
cmpltpd %xmm2,%xmm3       #<- Supposedly SIGFPE happens here
cmpltpd %xmm0,%xmm1
orpd   %xmm3,%xmm1
movd   %xmm1,%eax
and    $0x1,%al
retq
```
Command line parameters:
```
-D_REENTRANT -m64 -march=x86-64 -pipe -fno-strict-aliasing  -O2 -g -DNDEBUG -std=c++14 
```

Could it be that some (parts of) registers are polluted and not really cleared beforehand? 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Richard (roe85)

<details>
I tried compiling our codebase and I get SIGFPE in an innocent looking function. 
I've added `noinline`, to isolate it further. When called deep in the call tree it crashes with SIGFPE (which should be impossible with the given datatypes) 
If I call it in an isolated unit-test with the very same parameters `(3,0, 200)` it works as expected

```
[[clang::noinline]]
bool checkIfObjTooBig(uint size_radial, uint size_azimuthal, uint max)
{
    return (size_azimuthal &gt; sqrt(2) * max ||
            size_radial &gt; sqrt(2) * max);
}
```
The generated Assembly with the supposed crash location.

```
mov    %esi,%eax
cvtsi2sd %rax,%xmm0
mov    %edx,%eax
cvtsi2sd %rax,%xmm1
mulsd  0x2eb12(%rip),%xmm1        # 0x57bfb8
mov    %edi,%eax
cvtsi2sd %rax,%xmm2
movapd %xmm1,%xmm3
cmpltpd %xmm2,%xmm3       #&lt;- Supposedly SIGFPE happens here
cmpltpd %xmm0,%xmm1
orpd   %xmm3,%xmm1
movd   %xmm1,%eax
and    $0x1,%al
retq
```
Command line parameters:
```
-D_REENTRANT -m64 -march=x86-64 -pipe -fno-strict-aliasing  -O2 -g -DNDEBUG -std=c++14 
```

Could it be that some (parts of) registers are polluted and not really cleared beforehand? 
</details>


---

### Comment 2 - RKSimon

I'd expect cmpltsd not cmpltpd 

---

### Comment 3 - RKSimon

convertIntLogicToFPLogic is doing some unnecessary vectorization - not sure why its only regressing now

---

### Comment 4 - efriedma-quic

If you're going to enable floating-point traps, you need to pass `-ftrapping-math` or equivalent, to disable optimization that might trigger a spurious floating-point exception.  By default, LLVM will speculate operations that trigger an exception.

(In this particular case, it looks like the compiler can avoid the spurious FP exception without any performance cost, but we don't make any promises.)

---

### Comment 5 - SiliconA-Z

> If you're going to enable floating-point traps, you need to pass `-ftrapping-math` or equivalent, to disable optimization that might trigger a spurious floating-point exception. By default, LLVM will speculate operations that trigger an exception.
> 
> (In this particular case, it looks like the compiler can avoid the spurious FP exception without any performance cost, but we don't make any promises.)

well, yes, because it was comparing random trash in the upper half

and that random trash might've been a NaN

Thanks for fixing this!

---

### Comment 6 - SiliconA-Z

an operation involving a signalling NaN will raise an exception if not masked
if one of those upper halves had a NaN in it, this happens


---

### Comment 7 - SiliconA-Z

and it's not entirely unusual for a half of an XMM register to have a NaN, because 0xffff'ffff'ffff'ffff (which is a signalling NaN) is one of the possible results of cmpltpd itself!

---

### Comment 8 - RKSimon

Apologies, I should have mentioned that we don't normally protect against fp traps, but given in this case it was an easy performance tweak I just focussed on that.

---

### Comment 9 - SiliconA-Z

I would actually argue that this bug was a miscompile because again it was comparing random part of the register because floating point operations use these registers.

---

### Comment 10 - phoebewang

Masking for used elements for each operation has its cost, which should not be taken when user doesn't enable FP exception. The cost should be paid only when trapping-math enabled.

---

### Comment 11 - roe85

@RKSimon It happend again with clang version 20.1.8

---

### Comment 12 - RKSimon

> [@RKSimon](https://github.com/RKSimon) It happend again with clang version 20.1.8

Development has now moved to 22.x and a 20.x back fix is not going to happen. Is it the same code snippet or a new one? Can you replicate the failure on 21.x or trunk (maybe with godbolt)?

---

### Comment 13 - roe85

It seems clang 20 didn't get this fix, but clang 21 seems to have it: https://godbolt.org/z/K6q6435a5

---

