# [Clang-Cl] Wrong destructing code generated in Clang 22

**Author:** QianNangong
**URL:** https://github.com/llvm/llvm-project/issues/183255
**Status:** Closed
**Labels:** clang:frontend, clang:codegen, miscompilation, platform:windows, regression:22
**Closed Date:** 2026-03-17T10:06:40Z

## Body

I'm using Visual Studio 2026 18.3.2 and LLVM 22.1.0.

inc.h
```cpp
#pragma once

class Class1 {};

class Class2 : public Class1 {
public:
	Class2();
	virtual ~Class2();
};
```
impl.cpp
```cpp
#include "inc.h"

Class2::Class2() {}
Class2::~Class2() {}
```
main.cpp
```cpp
#include "inc.h"

int main() {
	Class2* p1 = new Class2;
	delete p1; // <- this is fine
	Class2* p2 = new Class2[1];
	delete [] p2; // <- this will trigger "Invalid address specified to RtlValidateHeap" assertion in Debug mode and crash in Release mode.
	return 0;
}
```

Whether I put the constructor or the destructor of `Class3` inside `inc.h`, it won't crash. Only happen on both constructor and destructor in a standalone `.cpp` file.

This doesn't occur on Clang 21 and Clang 20 and no custom compiler flags being used.

## Comments

### Comment 1 - QianNangong

UPDATE: I can confirm it will only occur on MSVC targets(both x86 and x86_64), neither Linux nor MinGW won't be affected.

---

### Comment 2 - QianNangong

It seems to be related to commit d714a6c21062c6b85beeed1ce09c8761c4f58b71 

---

### Comment 3 - shafik

CC @Fznamznon 

---

### Comment 4 - Fznamznon

Thanks for reporting this, I'll take a look asap

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: 南宫茜 (QianNangong)

<details>
I'm using Visual Studio 2026 18.3.2 and LLVM 22.1.0.

inc.h
```cpp
#pragma once

class Class1 {};

class Class2 : public Class1 {
public:
	Class2();
	virtual ~Class2();
};
```
impl.cpp
```cpp
#include "inc.h"

Class2::Class2() {}
Class2::~Class2() {}
```
main.cpp
```cpp
#include "inc.h"

int main() {
	Class2* p1 = new Class2;
	delete p1; // &lt;- this is fine
	Class2* p2 = new Class2[1];
	delete [] p2; // &lt;- this will trigger "Invalid address specified to RtlValidateHeap" assertion in Debug mode and crash in Release mode.
	return 0;
}
```

Whether I put the constructor or the destructor of `Class3` inside `inc.h`, it won't crash. Only happen on both constructor and destructor in a standalone `.cpp` file.

This doesn't occur on Clang 21 and Clang 20 and no custom compiler flags being used.
</details>


---

### Comment 6 - Fznamznon

Ok, this is slightly problematic because the destructor is not exported, so clang generates scalar deleting destructor definition for impl.cpp because there is no new[] calls, then delete[] in main.cpp expects vector deleting destructor definition which is not there. A simple workaround is to bring destructor definition to .h file or export it via dllexport . I see that MSVC gets away by generating vector deleting destructor definition in object file for main.cpp and it only does so in case new[] is called, clang currently does not generate any destructor definitions for main.cpp and it probably makes sense to replicate this behavior too. https://godbolt.org/z/9YMP3o88Y 

---

### Comment 7 - Fznamznon

I just merged PR that would fix the reported issue. I'm not entirely sure it is a small enough change to cherry-pick to 22.x. I'm going to let it marinate in main for a while first.

---

### Comment 8 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: 南宫茜 (QianNangong)

<details>
I'm using Visual Studio 2026 18.3.2 and LLVM 22.1.0.

inc.h
```cpp
#pragma once

class Class1 {};

class Class2 : public Class1 {
public:
	Class2();
	virtual ~Class2();
};
```
impl.cpp
```cpp
#include "inc.h"

Class2::Class2() {}
Class2::~Class2() {}
```
main.cpp
```cpp
#include "inc.h"

int main() {
	Class2* p1 = new Class2;
	delete p1; // &lt;- this is fine
	Class2* p2 = new Class2[1];
	delete [] p2; // &lt;- this will trigger "Invalid address specified to RtlValidateHeap" assertion in Debug mode and crash in Release mode.
	return 0;
}
```

Whether I put the constructor or the destructor of `Class3` inside `inc.h`, it won't crash. Only happen on both constructor and destructor in a standalone `.cpp` file.

This doesn't occur on Clang 21 and Clang 20 and no custom compiler flags being used.
</details>


---

