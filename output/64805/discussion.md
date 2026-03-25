# Source range of deleted or defaulted function outside class body is inaccurate

**Author:** smcpeak
**URL:** https://github.com/llvm/llvm-project/issues/64805

## Body

## Problem

For this translation unit:

```
void delfunc() = delete;
```

the source range of `delfunc` should cover the entire text to the final `e` in `delete`, but when using Clang 16.0.0 (and also a recent-ish trunk build from source), it does not:

```
$ clang -Xclang -ast-dump -fsyntax-only tmp.cc
[...]
`-FunctionDecl 0x5613b9a09b28 <tmp.cc:1:1, col:14> col:6 delfunc 'void ()' delete implicit-inline
                                               ^^ expecting 23 here
```

The same thing happens for methods defined outside their class body:

```
$ cat tmp3.cc
struct S {
  inline S();
};

inline S::S() = default;

$ clang -Xclang -ast-dump -fsyntax-only tmp3.cc
[...]
`-CXXConstructorDecl 0x557217b67278 parent 0x557217b66af0 prev 0x557217b66d28 <line:5:1, col:13> col:11 used S 'void ()' inline default
  `-CompoundStmt 0x557217b67368 <col:13>
```

where `col:13` should be `col:23`.

## Suspected origin

Commit 5f4d76efd365c (2015-03-23) addressed a similar issue for declarations in the class body in `ParseCXXInlineMethods.cpp`, but did not change the handling of those outside class bodies in `Parser.cpp`.

## Rough sketch of a fix

The following diff handles the simple testcases above.  However, it does not handle templates, and I don't know enough about the parser to fix that myself.  I also have not done any testing with real code.  So, this diff is merely evidence that something in this vicinity probably needs to change rather than a serious proposal for exactly how to change it.

```
--- a/clang/lib/Parse/Parser.cpp
+++ b/clang/lib/Parse/Parser.cpp
@@ -1354,6 +1359,10 @@ Decl *Parser::ParseFunctionDefinition(ParsingDeclarator &D,
   ParseScope BodyScope(this, Scope::FnScope | Scope::DeclScope |
                                  Scope::CompoundStmtScope);
 
+  // If this is set, we need to adjust the end location to account for
+  // the '= delete' or '= default'.
+  SourceLocation KWEndLoc_toUpdate;
+
   // Parse function body eagerly if it is either '= delete;' or '= default;' as
   // ActOnStartOfFunctionDef needs to know whether the function is deleted.
   Sema::FnBodyKind BodyKind = Sema::FnBodyKind::Other;
@@ -1361,18 +1370,21 @@ Decl *Parser::ParseFunctionDefinition(ParsingDeclarator &D,
   if (TryConsumeToken(tok::equal)) {
     assert(getLangOpts().CPlusPlus && "Only C++ function definitions have '='");
 
+    SourceLocation KWEndLoc = Tok.getEndLoc().getLocWithOffset(-1);
     if (TryConsumeToken(tok::kw_delete, KWLoc)) {
       Diag(KWLoc, getLangOpts().CPlusPlus11
                       ? diag::warn_cxx98_compat_defaulted_deleted_function
                       : diag::ext_defaulted_deleted_function)
           << 1 /* deleted */;
       BodyKind = Sema::FnBodyKind::Delete;
+      KWEndLoc_toUpdate = KWEndLoc;
     } else if (TryConsumeToken(tok::kw_default, KWLoc)) {
       Diag(KWLoc, getLangOpts().CPlusPlus11
                       ? diag::warn_cxx98_compat_defaulted_deleted_function
                       : diag::ext_defaulted_deleted_function)
           << 0 /* defaulted */;
       BodyKind = Sema::FnBodyKind::Default;
+      KWEndLoc_toUpdate = KWEndLoc;
     } else {
       llvm_unreachable("function definition after = not 'delete' or 'default'");
     }
@@ -1398,6 +1410,13 @@ Decl *Parser::ParseFunctionDefinition(ParsingDeclarator &D,
                                                   : MultiTemplateParamsArg(),
                                               &SkipBody, BodyKind);
 
+  if (KWEndLoc_toUpdate.isValid()) {
+    // TODO: This does not handle templates.
+    if (auto *DeclAsFunction = dyn_cast<FunctionDecl>(Res)) {
+      DeclAsFunction->setRangeEnd(KWEndLoc_toUpdate);
+    }
+  }
+
   if (SkipBody.ShouldSkip) {
     // Do NOT enter SkipFunctionBody if we already consumed the tokens.
     if (BodyKind == Sema::FnBodyKind::Other)

```


## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - smcpeak

This issue started as a question on Stack Overflow: https://stackoverflow.com/questions/76924637/how-to-find-the-clangsourcerange-of-a-deleted-function .

---

### Comment 3 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

  1) Assign the issue to you.
  2) Fix the issue locally.
  3) [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally.
    3.1) Remember that the subdirectories under `test/` create fine-grained testing targets, so you can
         e.g. use `make check-clang-ast` to only run Clang's AST tests.
  4) Create a `git` commit
  5) Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
  6) Submit the patch to [Phabricator](https://reviews.llvm.org/).
    6.1) Detailed instructions can be found [here](https://llvm.org/docs/Phabricator.html#requesting-a-review-via-the-web-interface)

For more instructions on how to submit a patch to LLVM, see our [documentation](https://llvm.org/docs/Contributing.html).

If you have any further questions about this issue, don't hesitate to ask via a comment on this Github issue.

@llvm/issue-subscribers-good-first-issue

---

### Comment 4 - nox1134

Hii @smcpeak ! I'm new to open source and would like to work on this issue. How do i assign this issue to myself?


---

### Comment 5 - smcpeak

@nox1134 As far as I know, you have to have write access to the repository to assign an issue, per [Assigning issues and pull requests to other GitHub users](https://docs.github.com/en/issues/tracking-your-work-with-issues/assigning-issues-and-pull-requests-to-other-github-users).  But I take it from the [Developer Policy](https://llvm.org/docs/DeveloperPolicy.html#obtaining-commit-access) that granting direct write access is unusual (understandably).  So I don't know how to follow the bot's instructions above, sorry!  I haven't made any contributions to LLVM myself.

Perhaps @cor3ntin could advise.

---

### Comment 6 - rajkumarananthu

Hi everyone, if no one is working on this issue, I would like to take up this issue and work on it.
Please assign the issue to me.

Thanks
Rajkumar Ananthu.

---

### Comment 7 - erichkeane

Unassigning as this doesn't seem to have any progress on it.  Feel free to pick it back up if you wish to work on it again. Same offer up to others!

---

### Comment 8 - dipeshs809

HI @erichkeane I would like to work on this issue. Could you please assign this to me ?

---

### Comment 9 - erichkeane

> HI [@erichkeane](https://github.com/erichkeane) I would like to work on this issue. Could you please assign this to me ?

I haven't been assigning people lately since folks tend to not ask to be unassigned when they give up :)  That said, you have your comment so others coming along will see that you're working on it.  So feel free to start working! And let us know if you need help.

---

### Comment 10 - dipeshs809

> > HI [@erichkeane](https://github.com/erichkeane) I would like to work on this issue. Could you please assign this to me ?
> 
> I haven't been assigning people lately since folks tend to not ask to be unassigned when they give up :) That said, you have your comment so others coming along will see that you're working on it. So feel free to start working! And let us know if you need help.

I understand it @erichkeane. Given that you acknowledge that I'm working on this, I should be given priority if someone else raises a PR before me.

---

### Comment 11 - dipeshs809

@smcpeak @erichkeane I had a look at the problem to identify the scope. Please correct me if I'm wrong.
 - Incorrect source range is only for default and delete functions.
 - The problem persists only for functions outside the [Class/Struct](https://godbolt.org/z/nGeWhs5fY) body.

---

### Comment 12 - erichkeane

Yes, that is the bug as reported.

---

### Comment 13 - dipeshs809

> [@smcpeak](https://github.com/smcpeak) [@erichkeane](https://github.com/erichkeane) I had a look at the problem to identify the scope. Please correct me if I'm wrong.
> 
> * Incorrect source range is only for default and delete functions.
> * The problem persists only for functions outside the [Class/Struct](https://godbolt.org/z/nGeWhs5fY) body.

For [templates](https://godbolt.org/z/KhE8hWqc6) also, the behavior is the same. Incorrect source range when declared outside template body.

---

### Comment 14 - dipeshs809

> Rough sketch of a fix
The following diff handles the simple testcases above. However, it does not handle templates, and I don't know enough about the parser to fix that myself. I also have not done any testing with real code. So, this diff is merely evidence that something in this vicinity probably needs to change rather than a serious proposal for exactly how to change it.

```
  if (BodyKind != Sema::FnBodyKind::Other) {
    Actions.SetFunctionBodyKind(Res, KWLoc, BodyKind, DeletedMessage);
    if (auto *FD = dyn_cast_or_null<FunctionDecl>(Res)) {
      FD->setRangeEnd(KWEndLoc_toUpdate);
    + } else if (auto *FTD = dyn_cast_or_null<FunctionTemplateDecl>(Res)) {
    + FTD->getTemplatedDecl()->setRangeEnd(KWEndLoc_toUpdate);
    + }
    Stmt *GeneratedBody = Res ? Res->getBody() : nullptr;
    Actions.ActOnFinishFunctionBody(Res, GeneratedBody, false);
    return Res;
  }
```

@smcpeak  @erichkeane  Correct me if I am wrong, but, I figured out that `FunctionDecl` and `FunctionTemplateDecl` are two different type of nodes in AST hence the two diferent `dyn_cast_or_null` are required to do the job. Also verified this by executing an example.

The issue arise when we encounter out-of-class method/constructor declaration. Though they are sub classes of `FunctionDecl`, hence i expect them to be handled here only but the results are negative. It only handles inline function declaration along with template functional declaration.

Any leads would help on how to go about handeling out-of-class `FunctionalDecl`.



---

### Comment 15 - erichkeane

Yes, `FunctionDecl` and `FunctionTemplateDecl` are two different nodes.  A `FunctionTemplateDecl` contains a primary template (`getTemplatedDecl`) that is a `FunctionDecl` that contains the uninstantiated function that is eventually instantiated.

I'm not sure what you mean by 'the results are negative'.  I too would expect a `CXXMethodDecl`/`CXXConstructorDecl` to be handled by the `FunctionDecl` code as well, are you finding via the debugger that it isnt?  Do you know why?

---

### Comment 16 - dipeshs809

> I'm not sure what you mean by 'the results are negative'.

I mean, for, `CXXMethodDecl` and `CXXConstructoDecl` wrong ranges are reported. But for `FunctionalDecl` and `FunctionalTemplateDecl` ranges are correclty reported by introducing the above stated changes.

> I too would expect a CXXMethodDecl/CXXConstructorDecl to be handled by the FunctionDecl code as well, are you finding via the debugger that it isnt? Do you know why?

For now, I am not using debugger to check if they are handled or not. I am inspecting the logged AST `clang -Xclang -ast-dump -fsyntax-only tmp.cc` to check the ranges.


---

### Comment 17 - erichkeane

> > I'm not sure what you mean by 'the results are negative'.
> 
> I mean, for, `CXXMethodDecl` and `CXXConstructoDecl` wrong ranges are reported. But for `FunctionalDecl` and `FunctionalTemplateDecl` ranges are correclty reported by introducing the above stated changes.
> 
> > I too would expect a CXXMethodDecl/CXXConstructorDecl to be handled by the FunctionDecl code as well, are you finding via the debugger that it isnt? Do you know why?
> 
> For now, I am not using debugger to check if they are handled or not. I am inspecting the logged AST `clang -Xclang -ast-dump -fsyntax-only tmp.cc` to check the ranges.

You might then want to check to make sure that the CXXMethodDecl/CXXConstructorDecl is actually going throug here.  It might actually be handled in a different function.  At least Constructors/Destructors/Operators are often handled separately in code, so might need to be updated elsewhere too

---

### Comment 18 - dipeshs809

@erichkeane I looked at the parser code, meanwhile, also learning how to use debugger to better trace the parsing for out-of-class constructor/method declaration. Everything seems to be fine wrt to parsing.

I just have one doubt, though I have not checked it myself hope I can get directions on it, can `Sema` phase also be involved in the incorrect diagnostics of Range. To be precise, is it possible that `Sema` overwrite the ranges ?

---

### Comment 19 - dipeshs809

>You might then want to check to make sure that the CXXMethodDecl/CXXConstructorDecl is actually going throug here. It might actually be handled in a different function. At least Constructors/Destructors/Operators are often handled separately in code, so might need to be updated elsewhere too

@erichkeane I used debuger to verify weather the flow is going through here or not. Yes the constructor declaration is also handled here, `Parser::ParseFunctionDefinition` for out-of-class function declaration.

<img width="618" alt="Image" src="https://github.com/user-attachments/assets/afce2f6e-ba0c-4118-9b74-1c12b9ed2f22" />

---

### Comment 20 - erichkeane

>I just have one doubt, though I have not checked it myself hope I can get directions on it, can Sema phase also be involved in the incorrect diagnostics of Range. To be precise, is it possible that Sema overwrite the ranges ?

We aren't const correct, so it perhaps happens?  But we typically treat the AST as immutable once created.  So I'd be surprised to see that.

>@erichkeane I used debuger to verify weather the flow is going through here or not. Yes the constructor declaration is also handled here, Parser::ParseFunctionDefinition for out-of-class function declaration.

Hmm.... that is good to know!  I'm glad that it does.  That said, I still wonder why you weren't finding the constructors to be getting the 'correct' range.  Perhaps look at the 'dump' code to see why/where it is getting that wrong?  Otherwise IDK if I have a good idea where to go next besides 'debug more' :(

IF you think it is a value that is being changed in Sema, consider using a 'watchpoint' in your debugger (which is a breakpoint at 'this piece of memory is modified').

---

### Comment 21 - dipeshs809

> Hmm.... that is good to know! I'm glad that it does. That said, I still wonder why you weren't finding the constructors to be getting the 'correct' range. Perhaps look at the 'dump' code to see why/where it is getting that wrong? Otherwise IDK if I have a good idea where to go next besides 'debug more' :(

I accidently tried to parse both `default` and `delete` FunctionDecl. What is more interesting here is that for `delete` the ranges for out-of-order declaration are reported correctly but not for `default`. I have used debuger and verified that the ranges are actually being captured in both the cases but not updated in case of default. No clue why this wierd behaviour? 



---

### Comment 22 - erichkeane

> > Hmm.... that is good to know! I'm glad that it does. That said, I still wonder why you weren't finding the constructors to be getting the 'correct' range. Perhaps look at the 'dump' code to see why/where it is getting that wrong? Otherwise IDK if I have a good idea where to go next besides 'debug more' :(
> 
> I accidently tried to parse both `default` and `delete` FunctionDecl. What is more interesting here is that for `delete` the ranges for out-of-order declaration are reported correctly but not for `default`. I have used debuger and verified that the ranges are actually being captured in both the cases but not updated in case of default. No clue why this wierd behaviour?

Do we perhaps do something strangely while printing/dumping them? Though you said you noticed in the debugger that only 1 is being updated?  how are you telling that it is not updated in the case of default? 

---

