# Goto out of __finally fails internal compiler checks (Referring to a basic block in another function)

**Author:** jaykrell
**URL:** https://github.com/llvm/llvm-project/issues/116923
**Status:** Open
**Labels:** clang:codegen, llvm:codegen, miscompilation, extension:microsoft

## Body

see also

https://github.com/microsoft/compiler-tests/blob/master/seh/seh0015.c


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
        
        goto A;
        
        // Similar will file separately.
        //return a;
    }

A:
    return a;
}

/*
        goto A;
        ^
Referring to a basic block in another function!
  br label %A
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

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Jay Krell (jaykrell)

<details>
see also

https://github.com/microsoft/compiler-tests/blob/master/seh/seh0015.c


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
        
        goto A;
        
        // Similar will file separately.
        //return a;
    }

A:
    return a;
}

/*
        goto A;
        ^
Referring to a basic block in another function!
  br label %A
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

