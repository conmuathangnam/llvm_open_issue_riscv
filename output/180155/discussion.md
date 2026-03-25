# [clang-tidy] `llvm-header-guard` emits wrong warning in case of `-std=c++20`

**Author:** thorsten-klein
**URL:** https://github.com/llvm/llvm-project/issues/180155
**Status:** Closed
**Labels:** clang:frontend, regression:21
**Closed Date:** 2026-03-04T14:11:36Z

## Body

Header File `include/test.hpp`:
```
#ifndef TEST_HPP
#define TEST_HPP
#endif // TEST_HPP
```

Run:
`clang-tidy '--checks=-*,llvm-header-guard' include/test.hpp -- -std=c++20`

Emits warning:
```
1 warning generated.
/home/user/work/include/test.hpp:2:9: warning: code/includes outside of area guarded by header guard; consider moving it [llvm-header-guard]
    2 | #define TEST_HPP
      |   
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: Thorsten Klein (thorsten-klein)

<details>
Header File `include/test.hpp`:
```
#ifndef TEST_HPP
#define TEST_HPP
#endif // TEST_HPP
```

Run:
`clang-tidy '--checks=-*,llvm-header-guard' include/test.hpp -- -std=c++20`

Emits warning:
```
1 warning generated.
/home/user/work/include/test.hpp:2:9: warning: code/includes outside of area guarded by header guard; consider moving it [llvm-header-guard]
    2 | #define TEST_HPP
      |   
```
</details>


---

### Comment 2 - vbvictor

Assigning to me for now to not lose this.
@thorsten-klein, if you want to look into this - feel free to do so:)

---

### Comment 3 - vbvictor

I run newest `claude opus4.6` on this and here what it suggested:

```diff
--- a/clang/lib/Lex/Lexer.cpp
+++ b/clang/lib/Lex/Lexer.cpp
@@ -3233,6 +3233,7 @@ std::optional<Token> Lexer::peekNextPPToken() {
   bool atStartOfLine = IsAtStartOfLine;
   bool atPhysicalStartOfLine = IsAtPhysicalStartOfLine;
   bool leadingSpace = HasLeadingSpace;
+  MultipleIncludeOpt SavedMIOpt = MIOpt;
 
   Token Tok;
   Lex(Tok);
@@ -3243,6 +3244,7 @@ std::optional<Token> Lexer::peekNextPPToken() {
   HasLeadingSpace = leadingSpace;
   IsAtStartOfLine = atStartOfLine;
   IsAtPhysicalStartOfLine = atPhysicalStartOfLine;
+  MIOpt = SavedMIOpt;
   // Restore the lexer back to non-skipping mode.
   LexingRawMode = false;
```

Unfortunately I don't have knowledge of Lexer to thoughtfully verify it's correctness but I verified locally you case doesn't produce FP anymore.

<details>

<summary>Here is whole output of LLM explaining this if anyone interested:</summary>

The Problem                                                                                                                                                                                             
                                                                                                          
  When running llvm-header-guard on a perfectly valid header file with -std=c++20, clang-tidy emits a false positive warning:                                                                             
                                                                                                          
  $ cat include/test.hpp
  #ifndef TEST_HPP
  #define TEST_HPP
  #endif // TEST_HPP

  $ clang-tidy '--checks=-*,llvm-header-guard' include/test.hpp -- -std=c++17
  # No warning (correct)

  $ clang-tidy '--checks=-*,llvm-header-guard' include/test.hpp -- -std=c++20
  warning: code/includes outside of area guarded by header guard; consider moving it

  The issue affects C++20, C++23, and C++26 — any standard where C++ modules syntax is enabled.

  ---
  Background: How Header Guard Detection Works

  Clang has a Multiple Include Optimization (MIOpt) — a state machine inside each Lexer instance that tracks whether a file is fully protected by a header guard. This is the same mechanism the
  llvm-header-guard check relies on.

  The MultipleIncludeOpt State Machine

  The state machine lives in clang/include/clang/Lex/MultipleIncludeOpt.h and has these key fields:

  ReadAnyTokens          — have any non-directive tokens been seen?
  ImmediatelyAfterIfndef — was the last thing a top-level #ifndef?
  TheMacro               — the candidate controlling macro (e.g., TEST_HPP)

  Normal Flow (C++17, working correctly)

  For a file like:
  #ifndef TEST_HPP       // line 1
  #define TEST_HPP       // line 2
  #endif // TEST_HPP     // line 3

  The state machine transitions are:

  State at file entry:
    ReadAnyTokens = false
    TheMacro = null

  Step 1: Lexer hits '#' on line 1 → HandleDirective()
    Saves: ReadAnyTokensBeforeDirective = false  ← KEY VALUE
    Processes: #ifndef TEST_HPP
    Since ReadAnyTokensBeforeDirective=false AND TEST_HPP is not defined:
      → calls EnterTopLevelIfndef(TEST_HPP)
      → TheMacro = TEST_HPP
      → ReadAnyTokens = true  (the #ifndef itself counts)

  Step 2: Lexer hits '#' on line 2 → HandleDirective()
    Processes: #define TEST_HPP
    (Normal macro definition, no MIOpt impact)

  Step 3: Lexer hits '#' on line 3 → HandleDirective()
    Processes: #endif
    Since conditional stack depth is now 0:
      → calls ExitTopLevelConditional()
      → TheMacro is set, so: ReadAnyTokens = false  ← RESET

  Step 4: End of file → HandleEndOfFile()
    Calls GetControllingMacroAtEndOfFile():
      ReadAnyTokens == false → returns TheMacro (TEST_HPP)
    → MI->setUsedForHeaderGuard(true)  ← SUCCESS

  Later, when the HeaderGuardPPCallbacks::EndOfMainFile() callback fires, it iterates over all macros and checks MI->isUsedForHeaderGuard(). Since it's true, the file is removed from the "unguarded
  files" list. No false positive.

  Broken Flow (C++20)

  In C++20, a single extra function call corrupts the state machine before step 1 even begins:

  State at file entry:
    ReadAnyTokens = false
    TheMacro = null

  *** NEW in C++20: Preprocessor::EnterMainSourceFile() ***
    Calls CurLexer->peekNextPPToken() to record FirstPPTokenLoc
    (used for better "module declaration must be at start" diagnostics)

    peekNextPPToken() internally calls Lex() which hits the '#' character.
    The '#' falls through to the default case in LexTokenInternal(),
    which calls MIOpt.ReadToken().

    peekNextPPToken() restores BufferPtr, ParsingPreprocessorDirective,
    IsAtStartOfLine, IsAtPhysicalStartOfLine, HasLeadingSpace...
    ...but does NOT restore MIOpt!

    → ReadAnyTokens = true  ← CORRUPTED (should still be false)
    → TheMacro = null

  Step 1: Lexer hits '#' on line 1 → HandleDirective()
    Saves: ReadAnyTokensBeforeDirective = true  ← WRONG! Should be false
    Processes: #ifndef TEST_HPP
    Since ReadAnyTokensBeforeDirective=true:
      → calls EnterTopLevelConditional()  ← WRONG PATH
      → calls Invalidate()
      → TheMacro = null  (permanently invalidated)

  Step 3: #endif → ExitTopLevelConditional()
    TheMacro is null → calls Invalidate() again
    ReadAnyTokens stays true

  Step 4: End of file → HandleEndOfFile()
    GetControllingMacroAtEndOfFile():
      ReadAnyTokens == true → returns null
    → setUsedForHeaderGuard() is NEVER called  ← FAILURE

  Now when HeaderGuardPPCallbacks::EndOfMainFile() runs, isUsedForHeaderGuard() returns false. The file stays in the "unguarded files" list. The check then finds the TEST_HPP macro (it matches the
  expected guard name) but since it wasn't recognized by the preprocessor as a valid header guard, it emits the false warning.

  ---
  The Culprit Code

  In clang/lib/Lex/Preprocessor.cpp, lines 584-596:

  // Record the first PP token in the main file. This is used to generate
  // better diagnostics for C++ modules.
  if (getLangOpts().CPlusPlusModules) {       // ← Only in C++20+
      // ... setup code ...
      std::optional<Token> FirstPPTok = CurLexer->peekNextPPToken();  // ← THIS
      if (FirstPPTok)
          FirstPPTokenLoc = FirstPPTok->getLocation();
  }

  And peekNextPPToken() in clang/lib/Lex/Lexer.cpp:

  std::optional<Token> Lexer::peekNextPPToken() {
      LexingRawMode = true;

      // Save state...
      const char *TmpBufferPtr = BufferPtr;
      bool inPPDirectiveMode = ParsingPreprocessorDirective;
      bool atStartOfLine = IsAtStartOfLine;
      bool atPhysicalStartOfLine = IsAtPhysicalStartOfLine;
      bool leadingSpace = HasLeadingSpace;
      // ← MIOpt is NOT saved!

      Token Tok;
      Lex(Tok);    // ← This calls MIOpt.ReadToken() as a side effect!

      // Restore state...
      BufferPtr = TmpBufferPtr;
      ParsingPreprocessorDirective = inPPDirectiveMode;
      HasLeadingSpace = leadingSpace;
      IsAtStartOfLine = atStartOfLine;
      IsAtPhysicalStartOfLine = atPhysicalStartOfLine;
      // ← MIOpt is NOT restored!

      LexingRawMode = false;
      // ...
  }

  ---
  The Fix

  Save and restore MIOpt in peekNextPPToken(), just like all other lexer state:

  const char *TmpBufferPtr = BufferPtr;
  bool inPPDirectiveMode = ParsingPreprocessorDirective;
  bool atStartOfLine = IsAtStartOfLine;
  bool atPhysicalStartOfLine = IsAtPhysicalStartOfLine;
  bool leadingSpace = HasLeadingSpace;
  MultipleIncludeOpt SavedMIOpt = MIOpt;       // ← ADDED

  Token Tok;
  Lex(Tok);

  BufferPtr = TmpBufferPtr;
  ParsingPreprocessorDirective = inPPDirectiveMode;
  HasLeadingSpace = leadingSpace;
  IsAtStartOfLine = atStartOfLine;
  IsAtPhysicalStartOfLine = atPhysicalStartOfLine;
  MIOpt = SavedMIOpt;                           // ← ADDED

  This ensures peeking at the next token is truly side-effect-free with respect to header guard detection.

  ---
  How I Found It

  I added debug prints at each state transition in MultipleIncludeOpt and in HandleEndOfFile, then compared C++17 vs C++20:

  === C++17 ===
  EnterTopLevelIfndef called          ← correct path
  ExitTopLevelConditional, TheMacro=set
  HandleEndOfFile: ReadAnyTokens=0    ← guard detected

  === C++20 ===
  EnterTopLevelConditional called     ← WRONG path (Invalidate!)
  ExitTopLevelConditional, TheMacro=null
  HandleEndOfFile: ReadAnyTokens=1    ← guard NOT detected

  This showed EnterTopLevelConditional (the wrong branch) was taken instead of EnterTopLevelIfndef. Adding a check showed ReadAnyTokensBeforeDirective=true — tokens were supposedly read before the first
   #ifndef. A stack trace on the first MIOpt.ReadToken() call revealed the peekNextPPToken() → EnterMainSourceFile() chain as the source of corruption.

</details>

---

### Comment 4 - vbvictor

This is probably a regression after https://github.com/llvm/llvm-project/pull/143898 / https://github.com/llvm/llvm-project/pull/145244

Confirmed that this is not happening in `clang-tidy-20` and started to fire in `clang-tidy-21` (which was released after those patches were landed)

---

### Comment 5 - vbvictor

CC @yronglin as author of those patches.
This seems a regression when we don't restore state of lexer after calling `peekNextToken` here:

https://github.com/llvm/llvm-project/blob/763761888f644615cde7db1d896e46daf2b85db3/clang/lib/Lex/Preprocessor.cpp#L584-L597

---

### Comment 6 - thorsten-klein

I have applied this fix in as a separate commit in PR https://github.com/llvm/llvm-project/pull/177315

Update: PR [#180700](https://github.com/llvm/llvm-project/pull/180700) is created where this fix is contributed separately.

---

### Comment 7 - vbvictor

> I have applied this fix in as a separate commit in PR [#177315](https://github.com/llvm/llvm-project/pull/177315)

This should not go into clang-tidy PR.
This need proper review from clang maintainers and must go separately.

The only reason I didn't post this myself before because I don't clearly understand what is going on in Lexer and that is violation of https://llvm.org/docs/AIToolPolicy.html.

So if you clearly understand this fix and can reason about it on review, please post this in a separate PR for clang review.
This fix will likely need some standalone unit/lit test in Lexer component.

---

### Comment 8 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Thorsten Klein (thorsten-klein)

<details>
Header File `include/test.hpp`:
```
#ifndef TEST_HPP
#define TEST_HPP
#endif // TEST_HPP
```

Run:
`clang-tidy '--checks=-*,llvm-header-guard' include/test.hpp -- -std=c++20`

Emits warning:
```
1 warning generated.
/home/user/work/include/test.hpp:2:9: warning: code/includes outside of area guarded by header guard; consider moving it [llvm-header-guard]
    2 | #define TEST_HPP
      |   
```
</details>


---

