# return out of __finally fails internal compiler checks (Instruction referencing instruction not embedded in a basic block)

**Author:** jaykrell
**URL:** https://github.com/llvm/llvm-project/issues/116924
**Status:** Open
**Labels:** clang:codegen, miscompilation, extension:microsoft

## Body

Same problem here:
https://github.com/microsoft/compiler-tests/blob/master/seh/xcpt4u.c

(once you clean it up).

Works fine with Visual C++.

```
int main()
{
    int a = 10;
    __try {
        a = 20;
        *(volatile char*)0;
        a = 30;
    }
    __finally
    {
        // Next line is not needed.
        //a = _abnormal_termination() ? 40 : 50;
        
        //goto A;
        
        return a;
    }

A:
    return a;
}

/*
1.c(16,9): warning: jump out of __finally block has undefined behavior [-Wjump-seh-finally]
        return a;
        ^
Instruction referencing instruction not embedded in a basic block!
  %a = call ptr @llvm.localrecover(ptr @main, ptr %frame_pointer, i32 0)
  <badref> = load i32, ptr %a, align 4
in function ?fin$0@0@main@@
fatal error: error in backend: Broken function found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
    0. Program arguments: ...
    1. <eof> parser at end of file
    2. Code generation
    3. Running pass 'Function Pass Manager' on module '1.c'.
    4. Running pass 'Module Verifier' on function '@"?fin$0@0@main@@"'
    clang-cl: error: clang frontend command failed with exit code 70 (use -v to see invocation)
*/
```

## Comments

### Comment 1 - efriedma-quic

See https://reviews.llvm.org/D124642 .  (I never got around to polishing it into something mergeable, and it's not really a priority for me at the moment; if someone else wants to pick it up, feel free.)

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Jay Krell (jaykrell)

<details>
Same problem here:
https://github.com/microsoft/compiler-tests/blob/master/seh/xcpt4u.c

(once you clean it up).

Works fine with Visual C++.

```
int main()
{
    int a = 10;
    __try {
        a = 20;
        *(volatile char*)0;
        a = 30;
    }
    __finally
    {
        // Next line is not needed.
        //a = _abnormal_termination() ? 40 : 50;
        
        //goto A;
        
        return a;
    }

A:
    return a;
}

/*
1.c(16,9): warning: jump out of __finally block has undefined behavior [-Wjump-seh-finally]
        return a;
        ^
Instruction referencing instruction not embedded in a basic block!
  %a = call ptr @<!-- -->llvm.localrecover(ptr @<!-- -->main, ptr %frame_pointer, i32 0)
  &lt;badref&gt; = load i32, ptr %a, align 4
in function ?fin$0@<!-- -->0@<!-- -->main@@
fatal error: error in backend: Broken function found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
    0. Program arguments: ...
    1. &lt;eof&gt; parser at end of file
    2. Code generation
    3. Running pass 'Function Pass Manager' on module '1.c'.
    4. Running pass 'Module Verifier' on function '@"?fin$0@<!-- -->0@<!-- -->main@@"'
    clang-cl: error: clang frontend command failed with exit code 70 (use -v to see invocation)
*/
```
</details>


---

