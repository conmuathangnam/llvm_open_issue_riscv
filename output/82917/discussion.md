# Clang crashes when having destructor called inside `__except`  block with`--async-exceptions`

**Author:** m4arhz
**URL:** https://github.com/llvm/llvm-project/issues/82917

## Body

Here is the code and options I used to reproduce the problem: https://godbolt.org/z/8r8TYsbq7
While the code crashes at `calculateSEHStateForAsynchEH` I believe the actual problem is at `EHScopeStack`. There are two type of `SEH` scopes, `llvm.seh.try` and `llvm.seh.scope`, the problem seem to be that the scope of the temporary starts with `llvm.seh.scope.begin` and ends with `llvm.seh.try.end` which causes the crash.
Anyone knows which type of `SEH` scope should the temporary have?

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (m4arhz)

<details>
Here is the code and options I used to reproduce the problem: https://godbolt.org/z/8r8TYsbq7
While the code crashes at `calculateSEHStateForAsynchEH` I believe the actual problem is at `EHScopeStack`. There are two type of `SEH` scopes, `llvm.seh.try` and `llvm.seh.scope`, the problem seem to be that the scope of the temporary starts with `llvm.seh.scope.begin` and ends with `llvm.seh.try.end` which causes the crash.
Anyone knows which type of `SEH` scope should the temporary have?
</details>


---

### Comment 2 - EugeneZelenko

Still crashes in `main`: https://godbolt.org/z/3dvf6T1jx

---

### Comment 3 - m4arhz

Even simpler example: https://godbolt.org/z/z6TWY6r4q

---

### Comment 4 - phoebewang

> Anyone knows which type of `SEH` scope should the temporary have?

I think it's `llvm.seh.scope.begin/end`, see https://godbolt.org/z/M7M3sMhY8
It looks like a front end bug. CC @jyu2-git 

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (m4arhz)

<details>
Here is the code and options I used to reproduce the problem: https://godbolt.org/z/8r8TYsbq7
While the code crashes at `calculateSEHStateForAsynchEH` I believe the actual problem is at `EHScopeStack`. There are two type of `SEH` scopes, `llvm.seh.try` and `llvm.seh.scope`, the problem seem to be that the scope of the temporary starts with `llvm.seh.scope.begin` and ends with `llvm.seh.try.end` which causes the crash.
Anyone knows which type of `SEH` scope should the temporary have?
</details>


---

### Comment 6 - m4arhz

Looks like it should be an early failure instead of a crash: https://godbolt.org/z/TYKcM47K5
Where is the right place to put this kind of check?

---

### Comment 7 - phoebewang

Thanks for the info! I think these check should be done in the front end SemaCheck part.

---

### Comment 8 - Qwinci

I also ran into this, it shouldn't be an error if c++ exceptions are disabled (it's not on msvc either if you pass `/EHs-` and my understanding is that msvc always supports catching async exceptions within __try regardless if the support for catching them in c++'s standard try statement is enabled). There are two places in https://github.com/llvm/llvm-project/blob/main/clang/lib/CodeGen/CGCleanup.cpp where the condition for choosing between `llvm.seh.try.end` and `llvm.seh.scope.end` is different from what is used to generate the corresponding begin intrinsic.
Here is where the scope begin is generated: https://github.com/llvm/llvm-project/blob/main/clang/lib/CodeGen/CGCleanup.cpp#L195
and as far as I understand the try end gets generated in here: https://github.com/llvm/llvm-project/blob/main/clang/lib/CodeGen/CGCleanup.cpp#L814 (and possibly in https://github.com/llvm/llvm-project/blob/main/clang/lib/CodeGen/CGCleanup.cpp#L856 too). I tried to change the logic of those two places to match the logic that's used to generate the scope begin and it does make it not crash but also makes https://github.com/llvm/llvm-project/blob/main/clang/test/CodeGen/windows-seh-EHa-TryInFinally.cpp test fail (maybe the test should also be changed but I am not sure, the changes that I made might also not be fully correct).

---

### Comment 9 - phoebewang

> it's not on msvc either if you pass `/EHs-` and my understanding is that msvc always supports catching async exceptions within __try regardless if the support for catching them in c++'s standard try statement is enabled

That's the currect difference between Clang and MSVC and we should improve it, see #62606 
Did you check if it works if we don't emit llvm.seh.scope* when c++ exceptions disabled?
I'm not sure to what extent the current async exceptions implementation rely on the c++ exceptions and how easy we can decouple them, but would be a good direction for exploration.

---

### Comment 10 - Qwinci

> > it's not on msvc either if you pass `/EHs-` and my understanding is that msvc always supports catching async exceptions within __try regardless if the support for catching them in c++'s standard try statement is enabled
> 
> That's the currect difference between Clang and MSVC and we should improve it, see [#62606](https://github.com/llvm/llvm-project/issues/62606) Did you check if it works if we don't emit llvm.seh.scope* when c++ exceptions disabled? I'm not sure to what extent the current async exceptions implementation rely on the c++ exceptions and how easy we can decouple them, but would be a good direction for exploration.

Looks like it does work if it is changed to not emit them at all when c++ exceptions are disabled, that's probably a decent solution along with a sema check for the other case when c++ exceptions are enabled.

---

