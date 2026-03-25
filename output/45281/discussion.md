# clang::ASTStmtWriter::VisitTypoExpr llvm_unreachable("Cannot write TypoExpr nodes");

**Author:** milianw
**URL:** https://github.com/llvm/llvm-project/issues/45281
**Status:** Closed
**Labels:** clang:frontend, bugzilla, crash
**Closed Date:** 2025-06-17T16:27:27Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [45936](https://llvm.org/bz45936) |
| Version | trunk |
| OS | Linux |
| CC | @dwblaikie,@Weverything |

## Extended Description 
when there's a typo in a file that gets parsed with, clang will crash:

```
(gdb) up
#&#8203;1  0x00007ffff306d853 in abort () from /usr/lib/libc.so.6
(gdb) 
#&#8203;2  0x00007fffb6251488 in llvm::llvm_unreachable_internal (msg=0x7fffbaf18423 "Cannot write TypoExpr nodes", 
    file=0x7fffbaf18298 "/home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp", line=2036)
    at /home/milian/projects/src/llvm-project/llvm/lib/Support/ErrorHandling.cpp:210
210       abort();
(gdb) 
#&#8203;3  0x00007fffbadfb521 in clang::ASTStmtWriter::VisitTypoExpr (this=0x7fff7aff66f0, E=0x7fff71fecb38)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2036
2036      llvm_unreachable("Cannot write TypoExpr nodes");
(gdb) list
2031    }
2032
2033    void ASTStmtWriter::VisitTypoExpr(TypoExpr *E) {
2034      VisitExpr(E);
2035      // TODO: Figure out sane writer behavior for a TypoExpr, if necessary
2036      llvm_unreachable("Cannot write TypoExpr nodes");
2037    }
2038
2039    //===----------------------------------------------------------------------===//
2040    // CUDA Expressions and Statements.
(gdb) up
#&#8203;4  0x00007fffbae09bb7 in clang::StmtVisitorBase<std::add_pointer, clang::ASTStmtWriter, void>::Visit (this=0x7fff7aff66f0, 
    S=0x7fff71fecb38) at tools/clang/include/clang/AST/StmtNodes.inc:1355
1355    TYPOEXPR(TypoExpr, Expr)
(gdb) bt
#&#8203;0  0x00007ffff30842e5 in raise () from /usr/lib/libc.so.6
#&#8203;1  0x00007ffff306d853 in abort () from /usr/lib/libc.so.6
#&#8203;2  0x00007fffb6251488 in llvm::llvm_unreachable_internal (msg=0x7fffbaf18423 "Cannot write TypoExpr nodes", 
    file=0x7fffbaf18298 "/home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp", line=2036)
    at /home/milian/projects/src/llvm-project/llvm/lib/Support/ErrorHandling.cpp:210
#&#8203;3  0x00007fffbadfb521 in clang::ASTStmtWriter::VisitTypoExpr (this=0x7fff7aff66f0, E=0x7fff71fecb38)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2036
#&#8203;4  0x00007fffbae09bb7 in clang::StmtVisitorBase<std::add_pointer, clang::ASTStmtWriter, void>::Visit (this=0x7fff7aff66f0, 
    S=0x7fff71fecb38) at tools/clang/include/clang/AST/StmtNodes.inc:1355
#&#8203;5  0x00007fffbadfdfe5 in clang::ASTWriter::WriteSubStmt (this=0x7fff70066650, S=0x7fff71fecb38)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2641
#&#8203;6  0x00007fffbadfe29e in clang::ASTRecordWriter::FlushSubStmts (this=0x7fff7aff6ae8)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2677
#&#8203;7  0x00007fffbae015ce in clang::ASTRecordWriter::EmitStmt (this=0x7fff7aff6ae8, Code=171, Abbrev=0)
    at /home/milian/projects/src/llvm-project/clang/include/clang/Serialization/ASTRecordWriter.h:98
#&#8203;8  0x00007fffbae0633e in clang::ASTStmtWriter::Emit (this=0x7fff7aff6ae0)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:49
#&#8203;9  0x00007fffbadfdff4 in clang::ASTWriter::WriteSubStmt (this=0x7fff70066650, S=0x7fff71fecb88)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2643
#&#8203;10 0x00007fffbadfe29e in clang::ASTRecordWriter::FlushSubStmts (this=0x7fff7aff6ed8)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2677
#&#8203;11 0x00007fffbae015ce in clang::ASTRecordWriter::EmitStmt (this=0x7fff7aff6ed8, Code=270, Abbrev=0)
    at /home/milian/projects/src/llvm-project/clang/include/clang/Serialization/ASTRecordWriter.h:98
#&#8203;12 0x00007fffbae0633e in clang::ASTStmtWriter::Emit (this=0x7fff7aff6ed0)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:49
#&#8203;13 0x00007fffbadfdff4 in clang::ASTWriter::WriteSubStmt (this=0x7fff70066650, S=0x7fff71fecc08)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2643
#&#8203;14 0x00007fffbadfe29e in clang::ASTRecordWriter::FlushSubStmts (this=0x7fff7aff72c8)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2677
#&#8203;15 0x00007fffbae015ce in clang::ASTRecordWriter::EmitStmt (this=0x7fff7aff72c8, Code=138, Abbrev=0)
    at /home/milian/projects/src/llvm-project/clang/include/clang/Serialization/ASTRecordWriter.h:98
#&#8203;16 0x00007fffbae0633e in clang::ASTStmtWriter::Emit (this=0x7fff7aff72c0)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:49
#&#8203;17 0x00007fffbadfdff4 in clang::ASTWriter::WriteSubStmt (this=0x7fff70066650, S=0x7fff71fecc40)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2643
#&#8203;18 0x00007fffbadfe165 in clang::ASTRecordWriter::FlushStmts (this=0x7fff7aff77a0)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2656
#&#8203;19 0x00007fffbad71e73 in clang::ASTRecordWriter::Emit (this=0x7fff7aff77a0, Code=56, Abbrev=0)
    at /home/milian/projects/src/llvm-project/clang/include/clang/Serialization/ASTRecordWriter.h:92
#&#8203;20 0x00007fffbade6791 in clang::ASTDeclWriter::Emit (this=0x7fff7aff7790, D=0x7fff71feb010)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterDecl.cpp:53
#&#8203;21 0x00007fffbade28d2 in clang::ASTWriter::WriteDecl (this=0x7fff70066650, Context=..., D=0x7fff71feb010)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterDecl.cpp:2431
#&#8203;22 0x00007fffbad53665 in clang::ASTWriter::WriteASTCore (this=0x7fff70066650, SemaRef=..., isysroot=..., OutputFile="", 
    WritingModule=0x0) at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriter.cpp:4758
#&#8203;23 0x00007fffbad51316 in clang::ASTWriter::WriteAST (this=0x7fff70066650, SemaRef=..., OutputFile="", WritingModule=0x0, 
    isysroot=..., hasErrors=true, ShouldCacheASTInMemory=false)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriter.cpp:4373
#&#8203;24 0x00007fffbae1149c in clang::PCHGenerator::HandleTranslationUnit (this=0x7fff70066580, Ctx=...)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/GeneratePCH.cpp:72
#&#8203;25 0x00007fffbb60e307 in (anonymous namespace)::PrecompilePreambleConsumer::HandleTranslationUnit (this=0x7fff70066580, 
    Ctx=...) at /home/milian/projects/src/llvm-project/clang/lib/Frontend/PrecompiledPreamble.cpp:177
#&#8203;26 0x00007fffb1512bfc in clang::ParseAST (S=..., PrintStats=false, SkipFunctionBodies=false)
--Type <RET> for more, q to quit, c to continue without paging--
    at /home/milian/projects/src/llvm-project/clang/lib/Parse/ParseAST.cpp:171
#&#8203;27 0x00007fffbb5ccb5b in clang::ASTFrontendAction::ExecuteAction (this=0x7fff7000e210)
    at /home/milian/projects/src/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1060
#&#8203;28 0x00007fffbb5cc4c0 in clang::FrontendAction::Execute (this=0x7fff7000e210)
    at /home/milian/projects/src/llvm-project/clang/lib/Frontend/FrontendAction.cpp:953
#&#8203;29 0x00007fffbb60f25d in clang::PrecompiledPreamble::Build (Invocation=..., MainFileBuffer=0x7fff70014070, Bounds=..., 
    Diagnostics=..., VFS=..., PCHContainerOps=std::shared_ptr<clang::PCHContainerOperations> (empty) = {...}, 
    StoreInMemory=false, Callbacks=...)
    at /home/milian/projects/src/llvm-project/clang/lib/Frontend/PrecompiledPreamble.cpp:357
#&#8203;30 0x00007fffbb4f3ed7 in clang::ASTUnit::getMainBufferWithPrecompiledPreamble (this=0x7fff70004620, 
    PCHContainerOps=std::shared_ptr<clang::PCHContainerOperations> (use count 17, weak count 0) = {...}, 
    PreambleInvocationIn=..., VFS=..., AllowRebuild=true, MaxLines=0)
    at /home/milian/projects/src/llvm-project/clang/lib/Frontend/ASTUnit.cpp:1381
#&#8203;31 0x00007fffbb4f5b45 in clang::ASTUnit::LoadFromCompilerInvocation (this=0x7fff70004620, 
    PCHContainerOps=std::shared_ptr<clang::PCHContainerOperations> (use count 17, weak count 0) = {...}, 
    PrecompilePreambleAfterNParses=1, VFS=...) at /home/milian/projects/src/llvm-project/clang/lib/Frontend/ASTUnit.cpp:1679
#&#8203;32 0x00007fffbb4f6b2f in clang::ASTUnit::LoadFromCommandLine (ArgBegin=0x7fff70001660, ArgEnd=0x7fff70001848, 
    PCHContainerOps=std::shared_ptr<clang::PCHContainerOperations> (empty) = {...}, Diags=..., ResourceFilesPath=..., 
    OnlyLocalDecls=false, CaptureDiagnostics=clang::CaptureDiagsKind::All, RemappedFiles=..., 
    RemappedFilesKeepOriginalName=true, PrecompilePreambleAfterNParses=1, TUKind=clang::TU_Complete, 
    CacheCodeCompletionResults=true, IncludeBriefCommentsInCodeCompletion=false, AllowPCHWithCompilerErrors=true, 
    SkipFunctionBodies=clang::SkipFunctionBodiesScope::None, SingleFileParse=false, UserFilesAreVolatile=true, 
    ForSerialization=false, RetainExcludedConditionalBlocks=false, ModuleFormat=..., ErrAST=0x7fff7affc7e0, VFS=...)
    at /home/milian/projects/src/llvm-project/clang/lib/Frontend/ASTUnit.cpp:1813
#&#8203;33 0x00007fffcd79a539 in clang_parseTranslationUnit_Impl (CIdx=0x5555560757c0, 
    source_filename=0x7fff94007918 "/home/milian/projects/kdab/qitissue/KDAB/features/kmeansclusterdatagenerator.cpp", 
    command_line_args=0x7fff94003230, num_command_line_args=57, unsaved_files=..., options=781, out_TU=0x7fff94006520)
    at /home/milian/projects/src/llvm-project/clang/tools/libclang/CIndex.cpp:3610
#&#8203;34 0x00007fffcd79ab30 in operator() (__closure=0x7fff9fffe2b0)
    at /home/milian/projects/src/llvm-project/clang/tools/libclang/CIndex.cpp:3683
#&#8203;35 0x00007fffcd7b4e75 in llvm::function_ref<void()>::callback_fn<clang_parseTranslationUnit2FullArgv(CXIndex, char const*, char const* const*, int, CXUnsavedFile*, unsigned int, unsigned int, CXTranslationUnitImpl**)::<lambda()> >(intptr_t) (
    callable=140735877735088) at /home/milian/projects/src/llvm-project/llvm/include/llvm/ADT/STLExtras.h:185
#&#8203;36 0x00007fffb621ffa2 in llvm::function_ref<void ()>::operator()() const (this=0x7fff7affc9d0)
    at /home/milian/projects/src/llvm-project/llvm/include/llvm/ADT/STLExtras.h:203
#&#8203;37 0x00007fffb62321f0 in llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (this=0x7fff9fffe290, Fn=...)
    at /home/milian/projects/src/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:418
#&#8203;38 0x00007fffb62322c2 in RunSafelyOnThread_Dispatch (UserData=0x7fff9fffe1c0)
    at /home/milian/projects/src/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:470
#&#8203;39 0x00007fffb63bf206 in threadFuncSync (Arg=0x7fff9fffe150)
    at /home/milian/projects/src/llvm-project/llvm/lib/Support/Unix/Threading.inc:48
#&#8203;40 0x00007ffff21f1422 in start_thread () from /usr/lib/libpthread.so.0
#&#8203;41 0x00007ffff3147b83 in clone () from /usr/lib/libc.so.6
```

## Comments

### Comment 1 - AaronBallman

This issue can be closed now that we've removed delayed typo correction in 9eef4d1c5fa6b1bcbbe675c14ca8301d5d346f7b

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Milian Wolff (milianw)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [45936](https://llvm.org/bz45936) |
| Version | trunk |
| OS | Linux |
| CC | @<!-- -->dwblaikie,@<!-- -->Weverything |

## Extended Description 
when there's a typo in a file that gets parsed with, clang will crash:

```
(gdb) up
#&amp;#<!-- -->8203;1  0x00007ffff306d853 in abort () from /usr/lib/libc.so.6
(gdb) 
#&amp;#<!-- -->8203;2  0x00007fffb6251488 in llvm::llvm_unreachable_internal (msg=0x7fffbaf18423 "Cannot write TypoExpr nodes", 
    file=0x7fffbaf18298 "/home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp", line=2036)
    at /home/milian/projects/src/llvm-project/llvm/lib/Support/ErrorHandling.cpp:210
210       abort();
(gdb) 
#&amp;#<!-- -->8203;3  0x00007fffbadfb521 in clang::ASTStmtWriter::VisitTypoExpr (this=0x7fff7aff66f0, E=0x7fff71fecb38)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2036
2036      llvm_unreachable("Cannot write TypoExpr nodes");
(gdb) list
2031    }
2032
2033    void ASTStmtWriter::VisitTypoExpr(TypoExpr *E) {
2034      VisitExpr(E);
2035      // TODO: Figure out sane writer behavior for a TypoExpr, if necessary
2036      llvm_unreachable("Cannot write TypoExpr nodes");
2037    }
2038
2039    //===----------------------------------------------------------------------===//
2040    // CUDA Expressions and Statements.
(gdb) up
#&amp;#<!-- -->8203;4  0x00007fffbae09bb7 in clang::StmtVisitorBase&lt;std::add_pointer, clang::ASTStmtWriter, void&gt;::Visit (this=0x7fff7aff66f0, 
    S=0x7fff71fecb38) at tools/clang/include/clang/AST/StmtNodes.inc:1355
1355    TYPOEXPR(TypoExpr, Expr)
(gdb) bt
#&amp;#<!-- -->8203;0  0x00007ffff30842e5 in raise () from /usr/lib/libc.so.6
#&amp;#<!-- -->8203;1  0x00007ffff306d853 in abort () from /usr/lib/libc.so.6
#&amp;#<!-- -->8203;2  0x00007fffb6251488 in llvm::llvm_unreachable_internal (msg=0x7fffbaf18423 "Cannot write TypoExpr nodes", 
    file=0x7fffbaf18298 "/home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp", line=2036)
    at /home/milian/projects/src/llvm-project/llvm/lib/Support/ErrorHandling.cpp:210
#&amp;#<!-- -->8203;3  0x00007fffbadfb521 in clang::ASTStmtWriter::VisitTypoExpr (this=0x7fff7aff66f0, E=0x7fff71fecb38)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2036
#&amp;#<!-- -->8203;4  0x00007fffbae09bb7 in clang::StmtVisitorBase&lt;std::add_pointer, clang::ASTStmtWriter, void&gt;::Visit (this=0x7fff7aff66f0, 
    S=0x7fff71fecb38) at tools/clang/include/clang/AST/StmtNodes.inc:1355
#&amp;#<!-- -->8203;5  0x00007fffbadfdfe5 in clang::ASTWriter::WriteSubStmt (this=0x7fff70066650, S=0x7fff71fecb38)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2641
#&amp;#<!-- -->8203;6  0x00007fffbadfe29e in clang::ASTRecordWriter::FlushSubStmts (this=0x7fff7aff6ae8)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2677
#&amp;#<!-- -->8203;7  0x00007fffbae015ce in clang::ASTRecordWriter::EmitStmt (this=0x7fff7aff6ae8, Code=171, Abbrev=0)
    at /home/milian/projects/src/llvm-project/clang/include/clang/Serialization/ASTRecordWriter.h:98
#&amp;#<!-- -->8203;8  0x00007fffbae0633e in clang::ASTStmtWriter::Emit (this=0x7fff7aff6ae0)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:49
#&amp;#<!-- -->8203;9  0x00007fffbadfdff4 in clang::ASTWriter::WriteSubStmt (this=0x7fff70066650, S=0x7fff71fecb88)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2643
#&amp;#<!-- -->8203;10 0x00007fffbadfe29e in clang::ASTRecordWriter::FlushSubStmts (this=0x7fff7aff6ed8)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2677
#&amp;#<!-- -->8203;11 0x00007fffbae015ce in clang::ASTRecordWriter::EmitStmt (this=0x7fff7aff6ed8, Code=270, Abbrev=0)
    at /home/milian/projects/src/llvm-project/clang/include/clang/Serialization/ASTRecordWriter.h:98
#&amp;#<!-- -->8203;12 0x00007fffbae0633e in clang::ASTStmtWriter::Emit (this=0x7fff7aff6ed0)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:49
#&amp;#<!-- -->8203;13 0x00007fffbadfdff4 in clang::ASTWriter::WriteSubStmt (this=0x7fff70066650, S=0x7fff71fecc08)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2643
#&amp;#<!-- -->8203;14 0x00007fffbadfe29e in clang::ASTRecordWriter::FlushSubStmts (this=0x7fff7aff72c8)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2677
#&amp;#<!-- -->8203;15 0x00007fffbae015ce in clang::ASTRecordWriter::EmitStmt (this=0x7fff7aff72c8, Code=138, Abbrev=0)
    at /home/milian/projects/src/llvm-project/clang/include/clang/Serialization/ASTRecordWriter.h:98
#&amp;#<!-- -->8203;16 0x00007fffbae0633e in clang::ASTStmtWriter::Emit (this=0x7fff7aff72c0)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:49
#&amp;#<!-- -->8203;17 0x00007fffbadfdff4 in clang::ASTWriter::WriteSubStmt (this=0x7fff70066650, S=0x7fff71fecc40)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2643
#&amp;#<!-- -->8203;18 0x00007fffbadfe165 in clang::ASTRecordWriter::FlushStmts (this=0x7fff7aff77a0)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterStmt.cpp:2656
#&amp;#<!-- -->8203;19 0x00007fffbad71e73 in clang::ASTRecordWriter::Emit (this=0x7fff7aff77a0, Code=56, Abbrev=0)
    at /home/milian/projects/src/llvm-project/clang/include/clang/Serialization/ASTRecordWriter.h:92
#&amp;#<!-- -->8203;20 0x00007fffbade6791 in clang::ASTDeclWriter::Emit (this=0x7fff7aff7790, D=0x7fff71feb010)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterDecl.cpp:53
#&amp;#<!-- -->8203;21 0x00007fffbade28d2 in clang::ASTWriter::WriteDecl (this=0x7fff70066650, Context=..., D=0x7fff71feb010)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriterDecl.cpp:2431
#&amp;#<!-- -->8203;22 0x00007fffbad53665 in clang::ASTWriter::WriteASTCore (this=0x7fff70066650, SemaRef=..., isysroot=..., OutputFile="", 
    WritingModule=0x0) at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriter.cpp:4758
#&amp;#<!-- -->8203;23 0x00007fffbad51316 in clang::ASTWriter::WriteAST (this=0x7fff70066650, SemaRef=..., OutputFile="", WritingModule=0x0, 
    isysroot=..., hasErrors=true, ShouldCacheASTInMemory=false)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/ASTWriter.cpp:4373
#&amp;#<!-- -->8203;24 0x00007fffbae1149c in clang::PCHGenerator::HandleTranslationUnit (this=0x7fff70066580, Ctx=...)
    at /home/milian/projects/src/llvm-project/clang/lib/Serialization/GeneratePCH.cpp:72
#&amp;#<!-- -->8203;25 0x00007fffbb60e307 in (anonymous namespace)::PrecompilePreambleConsumer::HandleTranslationUnit (this=0x7fff70066580, 
    Ctx=...) at /home/milian/projects/src/llvm-project/clang/lib/Frontend/PrecompiledPreamble.cpp:177
#&amp;#<!-- -->8203;26 0x00007fffb1512bfc in clang::ParseAST (S=..., PrintStats=false, SkipFunctionBodies=false)
--Type &lt;RET&gt; for more, q to quit, c to continue without paging--
    at /home/milian/projects/src/llvm-project/clang/lib/Parse/ParseAST.cpp:171
#&amp;#<!-- -->8203;27 0x00007fffbb5ccb5b in clang::ASTFrontendAction::ExecuteAction (this=0x7fff7000e210)
    at /home/milian/projects/src/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1060
#&amp;#<!-- -->8203;28 0x00007fffbb5cc4c0 in clang::FrontendAction::Execute (this=0x7fff7000e210)
    at /home/milian/projects/src/llvm-project/clang/lib/Frontend/FrontendAction.cpp:953
#&amp;#<!-- -->8203;29 0x00007fffbb60f25d in clang::PrecompiledPreamble::Build (Invocation=..., MainFileBuffer=0x7fff70014070, Bounds=..., 
    Diagnostics=..., VFS=..., PCHContainerOps=std::shared_ptr&lt;clang::PCHContainerOperations&gt; (empty) = {...}, 
    StoreInMemory=false, Callbacks=...)
    at /home/milian/projects/src/llvm-project/clang/lib/Frontend/PrecompiledPreamble.cpp:357
#&amp;#<!-- -->8203;30 0x00007fffbb4f3ed7 in clang::ASTUnit::getMainBufferWithPrecompiledPreamble (this=0x7fff70004620, 
    PCHContainerOps=std::shared_ptr&lt;clang::PCHContainerOperations&gt; (use count 17, weak count 0) = {...}, 
    PreambleInvocationIn=..., VFS=..., AllowRebuild=true, MaxLines=0)
    at /home/milian/projects/src/llvm-project/clang/lib/Frontend/ASTUnit.cpp:1381
#&amp;#<!-- -->8203;31 0x00007fffbb4f5b45 in clang::ASTUnit::LoadFromCompilerInvocation (this=0x7fff70004620, 
    PCHContainerOps=std::shared_ptr&lt;clang::PCHContainerOperations&gt; (use count 17, weak count 0) = {...}, 
    PrecompilePreambleAfterNParses=1, VFS=...) at /home/milian/projects/src/llvm-project/clang/lib/Frontend/ASTUnit.cpp:1679
#&amp;#<!-- -->8203;32 0x00007fffbb4f6b2f in clang::ASTUnit::LoadFromCommandLine (ArgBegin=0x7fff70001660, ArgEnd=0x7fff70001848, 
    PCHContainerOps=std::shared_ptr&lt;clang::PCHContainerOperations&gt; (empty) = {...}, Diags=..., ResourceFilesPath=..., 
    OnlyLocalDecls=false, CaptureDiagnostics=clang::CaptureDiagsKind::All, RemappedFiles=..., 
    RemappedFilesKeepOriginalName=true, PrecompilePreambleAfterNParses=1, TUKind=clang::TU_Complete, 
    CacheCodeCompletionResults=true, IncludeBriefCommentsInCodeCompletion=false, AllowPCHWithCompilerErrors=true, 
    SkipFunctionBodies=clang::SkipFunctionBodiesScope::None, SingleFileParse=false, UserFilesAreVolatile=true, 
    ForSerialization=false, RetainExcludedConditionalBlocks=false, ModuleFormat=..., ErrAST=0x7fff7affc7e0, VFS=...)
    at /home/milian/projects/src/llvm-project/clang/lib/Frontend/ASTUnit.cpp:1813
#&amp;#<!-- -->8203;33 0x00007fffcd79a539 in clang_parseTranslationUnit_Impl (CIdx=0x5555560757c0, 
    source_filename=0x7fff94007918 "/home/milian/projects/kdab/qitissue/KDAB/features/kmeansclusterdatagenerator.cpp", 
    command_line_args=0x7fff94003230, num_command_line_args=57, unsaved_files=..., options=781, out_TU=0x7fff94006520)
    at /home/milian/projects/src/llvm-project/clang/tools/libclang/CIndex.cpp:3610
#&amp;#<!-- -->8203;34 0x00007fffcd79ab30 in operator() (__closure=0x7fff9fffe2b0)
    at /home/milian/projects/src/llvm-project/clang/tools/libclang/CIndex.cpp:3683
#&amp;#<!-- -->8203;35 0x00007fffcd7b4e75 in llvm::function_ref&lt;void()&gt;::callback_fn&lt;clang_parseTranslationUnit2FullArgv(CXIndex, char const*, char const* const*, int, CXUnsavedFile*, unsigned int, unsigned int, CXTranslationUnitImpl**)::&lt;lambda()&gt; &gt;(intptr_t) (
    callable=140735877735088) at /home/milian/projects/src/llvm-project/llvm/include/llvm/ADT/STLExtras.h:185
#&amp;#<!-- -->8203;36 0x00007fffb621ffa2 in llvm::function_ref&lt;void ()&gt;::operator()() const (this=0x7fff7affc9d0)
    at /home/milian/projects/src/llvm-project/llvm/include/llvm/ADT/STLExtras.h:203
#&amp;#<!-- -->8203;37 0x00007fffb62321f0 in llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (this=0x7fff9fffe290, Fn=...)
    at /home/milian/projects/src/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:418
#&amp;#<!-- -->8203;38 0x00007fffb62322c2 in RunSafelyOnThread_Dispatch (UserData=0x7fff9fffe1c0)
    at /home/milian/projects/src/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:470
#&amp;#<!-- -->8203;39 0x00007fffb63bf206 in threadFuncSync (Arg=0x7fff9fffe150)
    at /home/milian/projects/src/llvm-project/llvm/lib/Support/Unix/Threading.inc:48
#&amp;#<!-- -->8203;40 0x00007ffff21f1422 in start_thread () from /usr/lib/libpthread.so.0
#&amp;#<!-- -->8203;41 0x00007ffff3147b83 in clone () from /usr/lib/libc.so.6
```
</details>


---

