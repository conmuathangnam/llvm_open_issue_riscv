# [clang-tidy] ASTMatchersInternal.cpp getExpansionLocOfMacro() occurs an infinite loop on freecad/src/Mod/Path/App/AreaPyImp.cpp

**Author:** mzyKi
**URL:** https://github.com/llvm/llvm-project/issues/114521
**Status:** Closed
**Labels:** clang:frontend, hang
**Closed Date:** 2024-12-12T08:19:43Z

## Body

I realize one simple clang-tidy ast matcher like this:
```
Finder->addMatcher(expr(isExpandedFromMacro("NULL")).bind("E"), this);
```
when I run ```clang-tidy``` on freecad/src/Mod/Path/App/AreaPyImp.cpp it will result in infinite loop in getExpansionLocOfMacro() in ASTMatchersInternal.cpp.

If I use "testchecker" to refer to the matcher implemented above, the clang-tidy command will be like this:
```
freecad $ clang-tidy src/Mod/Path/App/AreaPyImp.cpp -checks=-*,testchecker -p=build/compile_commands.json
```
The branches for both FreeCAD(https://github.com/FreeCAD/FreeCAD.git) and llvm-project are named "main".

I have tried to simplify the test code.
AreaPyImp.cpp
```
#include "AreaPy.h"

// The rest of the code is all commented out.
```
AreaPy.h
```
#ifndef PATH_AREAPY_H
#define PATH_AREAPY_H
#include <Mod/Path/App/Area.h>
#endif  // PATH_AREAPY_H

// The rest of the code is all commented out.
```

Area.h
```
#ifndef PATH_AREA_H
#define PATH_AREA_H
#include "AreaParams.h"
#include "Path.h"
namespace Path
{

/** Store libarea algorithm configuration */
struct PathExport CAreaParams {
    PARAM_DECLARE(PARAM_FNAME, AREA_PARAMS_CAREA)
    // CAreaParams();
};

/** Store all Area configurations */
struct PathExport AreaParams: CAreaParams {

    PARAM_DECLARE(PARAM_FNAME, AREA_PARAMS_AREA)

    bool operator==(const AreaParams& other) const {
#define AREA_COMPARE(_param) \
         if(PARAM_FIELD(NAME,_param)!=other.PARAM_FIELD(NAME,_param)) return false;
        PARAM_FOREACH(AREA_COMPARE, AREA_PARAMS_CAREA);
        // PARAM_FOREACH will result in infinite loop !!!
        // PARAM_FOREACH(AREA_COMPARE, AREA_PARAMS_AREA);
        return true;
    }
    bool operator!=(const AreaParams& other) const {
        return !(*this == other);
    }
};

} //namespace Path

// The rest of the code is all commented out.
#endif //PATH_AREA_H

```
As shown above,the macro PARAM_FOREACH will let getExpansionLocOfMacro occur an infinite loop.
```
std::optional<SourceLocation>
getExpansionLocOfMacro(StringRef MacroName, SourceLocation Loc,
                       const ASTContext &Context) {
  auto &SM = Context.getSourceManager();
  const LangOptions &LangOpts = Context.getLangOpts();
  Loc.dump(SM);
  while (Loc.isMacroID()) {
    SrcMgr::ExpansionInfo Expansion =
        SM.getSLocEntry(SM.getFileID(Loc)).getExpansion();
    if (Expansion.isMacroArgExpansion())
      // Check macro argument for an expansion of the given macro. For example,
      // `F(G(3))`, where `MacroName` is `G`.
      if (std::optional<SourceLocation> ArgLoc = getExpansionLocOfMacro(
              MacroName, Expansion.getSpellingLoc(), Context))
        return ArgLoc;
    Loc = Expansion.getExpansionLocStart();
    if (isTokenAtLoc(SM, LangOpts, MacroName, Loc))
      return Loc;
  }
  return std::nullopt;
}
```
I try to delete code 
``` 
if (Expansion.isMacroArgExpansion())
      // Check macro argument for an expansion of the given macro. For example,
      // `F(G(3))`, where `MacroName` is `G`.
      if (std::optional<SourceLocation> ArgLoc = getExpansionLocOfMacro(
              MacroName, Expansion.getSpellingLoc(), Context))
        return ArgLoc;
``` 
and it seems to be able to terminate normally. But it also result in failure in check-clang-unit.
I don't know the exact reason about this. Thanks for your suggestion and help! I'm willing to fix this bug!




## Comments

### Comment 1 - mandymimi

I found the root cause:     
Root Cause: The ASTMatcher "isExpandedFromMacro" calls getExpansionLocOfMacro (in llvm-project/clang/lib/ASTMatchers/ASTMatchersInternal.cpp). This function is designed to find the location of macro expansions. When the current expansion is a macro argument, it recursively calls getExpansionLocOfMacro to check the macro argument's expansion. The recursive logic checks previous macro expansions at each level, leading to redundant checks. When there are many nested macros, these redundant checks can significantly increase processing time.    

Solution: Track already processed macros and check if a macro has been processed before during recursion.

---

### Comment 2 - mzyKi

fixed in https://github.com/llvm/llvm-project/pull/117143 closed

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Exile (mzyKi)

<details>
I realize one simple clang-tidy ast matcher like this:
```
Finder-&gt;addMatcher(expr(isExpandedFromMacro("NULL")).bind("E"), this);
```
when I run ```clang-tidy``` on freecad/src/Mod/Path/App/AreaPyImp.cpp it will result in infinite loop in getExpansionLocOfMacro() in ASTMatchersInternal.cpp.

If I use "testchecker" to refer to the matcher implemented above, the clang-tidy command will be like this:
```
freecad $ clang-tidy src/Mod/Path/App/AreaPyImp.cpp -checks=-*,testchecker -p=build/compile_commands.json
```
The branches for both FreeCAD(https://github.com/FreeCAD/FreeCAD.git) and llvm-project are named "main".

I have tried to simplify the test code.
AreaPyImp.cpp
```
#include "AreaPy.h"

// The rest of the code is all commented out.
```
AreaPy.h
```
#ifndef PATH_AREAPY_H
#define PATH_AREAPY_H
#include &lt;Mod/Path/App/Area.h&gt;
#endif  // PATH_AREAPY_H

// The rest of the code is all commented out.
```

Area.h
```
#ifndef PATH_AREA_H
#define PATH_AREA_H
#include "AreaParams.h"
#include "Path.h"
namespace Path
{

/** Store libarea algorithm configuration */
struct PathExport CAreaParams {
    PARAM_DECLARE(PARAM_FNAME, AREA_PARAMS_CAREA)
    // CAreaParams();
};

/** Store all Area configurations */
struct PathExport AreaParams: CAreaParams {

    PARAM_DECLARE(PARAM_FNAME, AREA_PARAMS_AREA)

    bool operator==(const AreaParams&amp; other) const {
#define AREA_COMPARE(_param) \
         if(PARAM_FIELD(NAME,_param)!=other.PARAM_FIELD(NAME,_param)) return false;
        PARAM_FOREACH(AREA_COMPARE, AREA_PARAMS_CAREA);
        // PARAM_FOREACH will result in infinite loop !!!
        // PARAM_FOREACH(AREA_COMPARE, AREA_PARAMS_AREA);
        return true;
    }
    bool operator!=(const AreaParams&amp; other) const {
        return !(*this == other);
    }
};

} //namespace Path

// The rest of the code is all commented out.
#endif //PATH_AREA_H

```
As shown above,the macro PARAM_FOREACH will let getExpansionLocOfMacro occur an infinite loop.
```
std::optional&lt;SourceLocation&gt;
getExpansionLocOfMacro(StringRef MacroName, SourceLocation Loc,
                       const ASTContext &amp;Context) {
  auto &amp;SM = Context.getSourceManager();
  const LangOptions &amp;LangOpts = Context.getLangOpts();
  Loc.dump(SM);
  while (Loc.isMacroID()) {
    SrcMgr::ExpansionInfo Expansion =
        SM.getSLocEntry(SM.getFileID(Loc)).getExpansion();
    if (Expansion.isMacroArgExpansion())
      // Check macro argument for an expansion of the given macro. For example,
      // `F(G(3))`, where `MacroName` is `G`.
      if (std::optional&lt;SourceLocation&gt; ArgLoc = getExpansionLocOfMacro(
              MacroName, Expansion.getSpellingLoc(), Context))
        return ArgLoc;
    Loc = Expansion.getExpansionLocStart();
    if (isTokenAtLoc(SM, LangOpts, MacroName, Loc))
      return Loc;
  }
  return std::nullopt;
}
```
I try to delete code 
``` 
if (Expansion.isMacroArgExpansion())
      // Check macro argument for an expansion of the given macro. For example,
      // `F(G(3))`, where `MacroName` is `G`.
      if (std::optional&lt;SourceLocation&gt; ArgLoc = getExpansionLocOfMacro(
              MacroName, Expansion.getSpellingLoc(), Context))
        return ArgLoc;
``` 
and it seems to be able to terminate normally. But it also result in failure in check-clang-unit.
I don't know the exact reason about this. Thanks for your suggestion and help! I'm willing to fix this bug!



</details>


---

