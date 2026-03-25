# [Clad + Enzyme][Apple M1] Assertion failed: (Loc.isValid()) in DeclTemplate.h:1937 when using clad::gradient<opts::use_enzyme>

**Author:** Vedant2005goyal
**URL:** https://github.com/llvm/llvm-project/issues/162427
**Status:** Closed
**Labels:** clang:frontend, crash, needs-reduction
**Closed Date:** 2025-10-13T16:10:13Z

## Body

When compiling and running a Clad + Enzyme test on Apple M1 with Clang 18.1.8,  a segmentation fault occurs due to an assertion  in DeclTemplate.h.

**Error Message:**
Assertion failed: (Loc.isValid() && "point of instantiation must be valid!"), 
function setPointOfInstantiation, file DeclTemplate.h, line 1937.

**System Info:**
- macOS 15 (Darwin 25.0.0)
- Apple Silicon (M1)
- Clang version: 18.1.8 (built from llvm-project)
- Clad build: latest main branch
- Enzyme enabled

**Steps to Reproduce:**
**1. Test File (test/Enzyme/DifferentCladEnzymeDerivatives.C):**

// RUN: %cladclang %s -I%S/../../include -oDifferentCladEnzymeDerivatives.out | %filecheck %s
// RUN: ./DifferentCladEnzymeDerivatives.out
// REQUIRES: Enzyme
#include "clad/Differentiator/Differentiator.h"
#include <iostream>

double foo(double x, double y) {
    return x * y;
}

int main() {
    auto grad = clad::gradient(foo);
    auto gradEnzyme = clad::gradient<clad::opts::use_enzyme>(foo);
    double x = 3.0, y = 4.0;
    double grads[2];
    grad.execute(x, y, grads);
    std::cout << grads[0] << " " << grads[1] << std::endl;
}

**2. Command:**

bash./runclad.sh test/Enzyme/DifferentCladEnzymeDerivatives.C

**3. Full Output:**

🔧 Compiling test/Enzyme/DifferentCladEnzymeDerivatives.C...
Assertion failed: (Loc.isValid() && "point of instantiation must be valid!"), function setPointOfInstantiation, file DeclTemplate.h, line 1937.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
clang++: error: unable to execute command: Segmentation fault: 11
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 18.1.8 (https://github.com/llvm/llvm-project.git 3b5b5c1ec4a3095ab096dd780e84d7ab81f3d7ff)
Target: arm64-apple-darwin25.0.0
Thread model: posix
InstalledDir: /Users/vedantgoyal/Desktop/CODING/clad/./llvm-project/build/bin

**Diagnostic Files:**
[clang-18-2025-10-08-104848.txt](https://github.com/user-attachments/files/22759338/clang-18-2025-10-08-104848.txt)
[DifferentCladEnzymeDerivatives-4f9ddd.cpp](https://github.com/user-attachments/files/22759312/DifferentCladEnzymeDerivatives-4f9ddd.cpp)
[DifferentCladEnzymeDerivatives-4f9ddd.sh](https://github.com/user-attachments/files/22759311/DifferentCladEnzymeDerivatives-4f9ddd.sh)
**Note:** .crash file is renamed to .txt in order to upload it on github
**Shell script:** I have also attached the shell script that i used to compile and execute the code
#!/bin/zsh
set -e

CLANG=(ccache ./llvm-project/build/bin/clang++)
CLAD_LIB=./llvm-project/build/lib/clad.dylib
INCLUDE1=./llvm-project/llvm/include
INCLUDE2=./include
SRC=$1
BASENAME=$(basename ${SRC%.*})
OUT=build_exe/${BASENAME}

mkdir -p build_exe

echo "🔧 Compiling $SRC..."
"${CLANG[@]}" -O0 -isysroot $(xcrun --show-sdk-path) \
  -I$INCLUDE1 -I$INCLUDE2 \
  -L./llvm-project/build/lib \
  -fplugin=$CLAD_LIB \
  -Xclang -load -Xclang $CLAD_LIB \
  -o $OUT $SRC

echo "🚀 Running $OUT..."
./$OUT

**Expected Behavior:**
The code should compile successfully, allowing both gradient computation methods (standard Clad and Enzyme-based) to coexist in the same program.

**Additional Context:**
This appears to be related to template instantiation when mixing different Clad gradient backends in the same translation unit. The assertion occurs during compilation when `setPointOfInstantiation` is called with an invalid location.






## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Vedant Goyal (Vedant2005goyal)

<details>
When compiling and running a Clad + Enzyme test on Apple M1 with Clang 18.1.8,  a segmentation fault occurs due to an assertion  in DeclTemplate.h.

**Error Message:**
Assertion failed: (Loc.isValid() &amp;&amp; "point of instantiation must be valid!"), 
function setPointOfInstantiation, file DeclTemplate.h, line 1937.

**System Info:**
- macOS 15 (Darwin 25.0.0)
- Apple Silicon (M1)
- Clang version: 18.1.8 (built from llvm-project)
- Clad build: latest main branch
- Enzyme enabled

**Steps to Reproduce:**
**1. Test File (test/Enzyme/DifferentCladEnzymeDerivatives.C):**

// RUN: %cladclang %s -I%S/../../include -oDifferentCladEnzymeDerivatives.out | %filecheck %s
// RUN: ./DifferentCladEnzymeDerivatives.out
// REQUIRES: Enzyme
#include "clad/Differentiator/Differentiator.h"
#include &lt;iostream&gt;

double foo(double x, double y) {
    return x * y;
}

int main() {
    auto grad = clad::gradient(foo);
    auto gradEnzyme = clad::gradient&lt;clad::opts::use_enzyme&gt;(foo);
    double x = 3.0, y = 4.0;
    double grads[2];
    grad.execute(x, y, grads);
    std::cout &lt;&lt; grads[0] &lt;&lt; " " &lt;&lt; grads[1] &lt;&lt; std::endl;
}

**2. Command:**

bash./runclad.sh test/Enzyme/DifferentCladEnzymeDerivatives.C

**3. Full Output:**

🔧 Compiling test/Enzyme/DifferentCladEnzymeDerivatives.C...
Assertion failed: (Loc.isValid() &amp;&amp; "point of instantiation must be valid!"), function setPointOfInstantiation, file DeclTemplate.h, line 1937.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
clang++: error: unable to execute command: Segmentation fault: 11
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 18.1.8 (https://github.com/llvm/llvm-project.git 3b5b5c1ec4a3095ab096dd780e84d7ab81f3d7ff)
Target: arm64-apple-darwin25.0.0
Thread model: posix
InstalledDir: /Users/vedantgoyal/Desktop/CODING/clad/./llvm-project/build/bin

**Diagnostic Files:**
[clang-18-2025-10-08-104848.txt](https://github.com/user-attachments/files/22759338/clang-18-2025-10-08-104848.txt)
[DifferentCladEnzymeDerivatives-4f9ddd.cpp](https://github.com/user-attachments/files/22759312/DifferentCladEnzymeDerivatives-4f9ddd.cpp)
[DifferentCladEnzymeDerivatives-4f9ddd.sh](https://github.com/user-attachments/files/22759311/DifferentCladEnzymeDerivatives-4f9ddd.sh)
**Note:** .crash file is renamed to .txt in order to upload it on github
**Shell script:** I have also attached the shell script that i used to compile and execute the code
#!/bin/zsh
set -e

CLANG=(ccache ./llvm-project/build/bin/clang++)
CLAD_LIB=./llvm-project/build/lib/clad.dylib
INCLUDE1=./llvm-project/llvm/include
INCLUDE2=./include
SRC=$1
BASENAME=$(basename ${SRC%.*})
OUT=build_exe/${BASENAME}

mkdir -p build_exe

echo "🔧 Compiling $SRC..."
"${CLANG[@]}" -O0 -isysroot $(xcrun --show-sdk-path) \
  -I$INCLUDE1 -I$INCLUDE2 \
  -L./llvm-project/build/lib \
  -fplugin=$CLAD_LIB \
  -Xclang -load -Xclang $CLAD_LIB \
  -o $OUT $SRC

echo "🚀 Running $OUT..."
./$OUT

**Expected Behavior:**
The code should compile successfully, allowing both gradient computation methods (standard Clad and Enzyme-based) to coexist in the same program.

**Additional Context:**
This appears to be related to template instantiation when mixing different Clad gradient backends in the same translation unit. The assertion occurs during compilation when `setPointOfInstantiation` is called with an invalid location.





</details>


---

### Comment 2 - boomanaiden154

Can you post the full stack trace?

This seems like it might be an issue with clad rather than clang.

---

### Comment 3 - EugeneZelenko

Could you please try 21 or `main` branch? Only most recent release is maintained.

---

### Comment 4 - Vedant2005goyal

> Can you post the full stack trace?
> 
> This seems like it might be an issue with clad rather than clang.

Hi @boomanaiden154 
Thanks for giving you valuable time in looking into this issue
I have rerun the code and captured the full stack trace as you requested. Please find it attached here
[crash-backtrace.txt](https://github.com/user-attachments/files/22888548/crash-backtrace.txt)
I think you are right to say that the issue is with clad rather than clang because most of the upper frames are because of clad::Plugin before the call to clang.
Please let me know if you’d like me to share any additional artifacts
Thanks for your consideration
-Vedant Goyal

---

### Comment 5 - boomanaiden154

Yeah, given `clad` shows up in the stack trace, this definitely seems like a `clad` issue, and you're going to have better luck reporting it there.

Because it seems like a `clad` issue, I'm going to close this. Feel free to reopen if it does turn out to be an issue on the clang side.

---

### Comment 6 - Vedant2005goyal

> Could you please try 21 or `main` branch? Only most recent release is maintained.

Hi @EugeneZelenko,
Thanks for the suggestion! I’ll try reproducing the crash using Clang 21 or the main branch to see if the issue persists
I’ll report back with the results and attach any relevant artifacts if the crash still occurs.
Thanks again for your guidance!
— Vedant Goyal

---

