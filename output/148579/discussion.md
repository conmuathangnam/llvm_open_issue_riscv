# [Clang] Clang crashed when compiling with `-mllvm -print-changed=dot-cfg`

**Author:** cabbaken
**URL:** https://github.com/llvm/llvm-project/issues/148579

## Body

Code:
```c
void do_something();
int main() {
  for(int i = 0; i < 10; ++i)
    do_something();
}
```
compile with
```bash
clang -mllvm -print-changed=dot-cfg t.c 
```
will crash(https://godbolt.org/z/qG6oYsv73).
Below is the crash info:
```plaintext
clang -mllvm -print-changed=dot-cfg ttt.c 
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/ruoyu/llvm_install/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name ttt.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/ruoyu/work/test -fcoverage-compilation-dir=/home/ruoyu/work/test -resource-dir /home/ruoyu/llvm_install/lib/clang/21 -internal-isystem /home/ruoyu/llvm_install/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -mllvm -print-changed=dot-cfg -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/ttt-c61253.o -x c ttt.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function(ee-instrument<>)" on module "ttt.c"
 #0 0x000055f51f538270 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/ruoyu/llvm_install/bin/clang-21+0x3d0d270)
 #1 0x000055f51f535b4c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f3b1eb28520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000055f5205fd9f0 (anonymous namespace)::DotCfgDiff::createDisplayGraph(llvm::StringRef, llvm::StringRef) StandardInstrumentations.cpp:0:0
 #4 0x000055f520617b45 llvm::DotCfgChangeReporter::handleFunctionCompare(llvm::StringRef, llvm::StringRef, llvm::StringRef, llvm::StringRef, bool, unsigned int, llvm::FuncDataT<llvm::DCData> const&, llvm::FuncDataT<llvm::DCData> const&) (/home/ruoyu/llvm_install/bin/clang-21+0x4decb45)
 #5 0x000055f520619669 std::_Function_handler<void (bool, unsigned int, llvm::FuncDataT<llvm::DCData> const&, llvm::FuncDataT<llvm::DCData> const&), llvm::DotCfgChangeReporter::handleInitialIR(llvm::Any)::'lambda'(bool, unsigned int, llvm::FuncDataT<llvm::DCData> const&, llvm::FuncDataT<llvm::DCData> const&)>::_M_invoke(std::_Any_data const&, bool&&, unsigned int&&, llvm::FuncDataT<llvm::DCData> const&, llvm::FuncDataT<llvm::DCData> const&) StandardInstrumentations.cpp:0:0
 #6 0x000055f5205efc37 void llvm::function_ref<void (llvm::FuncDataT<llvm::DCData> const*, llvm::FuncDataT<llvm::DCData> const*)>::callback_fn<llvm::IRComparer<llvm::DCData>::compare(bool, std::function<void (bool, unsigned int, llvm::FuncDataT<llvm::DCData> const&, llvm::FuncDataT<llvm::DCData> const&)>)::'lambda'(llvm::FuncDataT<llvm::DCData> const*, llvm::FuncDataT<llvm::DCData> const*)>(long, llvm::FuncDataT<llvm::DCData> const*, llvm::FuncDataT<llvm::DCData> const*) (/home/ruoyu/llvm_install/bin/clang-21+0x4dc4c37)
 #7 0x000055f52061058f llvm::OrderedChangedData<llvm::FuncDataT<llvm::DCData>>::report(llvm::OrderedChangedData<llvm::FuncDataT<llvm::DCData>> const&, llvm::OrderedChangedData<llvm::FuncDataT<llvm::DCData>> const&, llvm::function_ref<void (llvm::FuncDataT<llvm::DCData> const*, llvm::FuncDataT<llvm::DCData> const*)>) (/home/ruoyu/llvm_install/bin/clang-21+0x4de558f)
 #8 0x000055f520610aa7 llvm::IRComparer<llvm::DCData>::compare(bool, std::function<void (bool, unsigned int, llvm::FuncDataT<llvm::DCData> const&, llvm::FuncDataT<llvm::DCData> const&)>) (/home/ruoyu/llvm_install/bin/clang-21+0x4de5aa7)
 #9 0x000055f520610fe2 llvm::DotCfgChangeReporter::handleInitialIR(llvm::Any) (/home/ruoyu/llvm_install/bin/clang-21+0x4de5fe2)
#10 0x000055f52061297b llvm::ChangeReporter<llvm::IRDataT<llvm::DCData>>::saveIRBeforePass(llvm::Any, llvm::StringRef, llvm::StringRef) (/home/ruoyu/llvm_install/bin/clang-21+0x4de797b)
#11 0x000055f520612b6b void llvm::detail::UniqueFunctionBase<void, llvm::StringRef, llvm::Any>::CallImpl<llvm::ChangeReporter<llvm::IRDataT<llvm::DCData>>::registerRequiredCallbacks(llvm::PassInstrumentationCallbacks&)::'lambda'(llvm::StringRef, llvm::Any)>(void*, llvm::StringRef, llvm::Any&) (/home/ruoyu/llvm_install/bin/clang-21+0x4de7b6b)
#12 0x000055f51eee875a llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/ruoyu/llvm_install/bin/clang-21+0x36bd75a)
#13 0x000055f51f7da39e (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#14 0x000055f51f7de50d clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/ruoyu/llvm_install/bin/clang-21+0x3fb350d)
#15 0x000055f51fe55e73 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/ruoyu/llvm_install/bin/clang-21+0x462ae73)
#16 0x000055f52172cf7c clang::ParseAST(clang::Sema&, bool, bool) (/home/ruoyu/llvm_install/bin/clang-21+0x5f01f7c)
#17 0x000055f520187cde clang::FrontendAction::Execute() (/home/ruoyu/llvm_install/bin/clang-21+0x495ccde)
#18 0x000055f5200f6dcb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/ruoyu/llvm_install/bin/clang-21+0x48cbdcb)
#19 0x000055f520262a88 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/ruoyu/llvm_install/bin/clang-21+0x4a37a88)
#20 0x000055f51c69bb7c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/ruoyu/llvm_install/bin/clang-21+0xe70b7c)
#21 0x000055f51c692db2 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#22 0x000055f51c696bef clang_main(int, char**, llvm::ToolContext const&) (/home/ruoyu/llvm_install/bin/clang-21+0xe6bbef)
#23 0x000055f51c52105b main (/home/ruoyu/llvm_install/bin/clang-21+0xcf605b)
#24 0x00007f3b1eb0fd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#25 0x00007f3b1eb0fe40 call_init ./csu/../csu/libc-start.c:128:20
#26 0x00007f3b1eb0fe40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#27 0x000055f51c692815 _start (/home/ruoyu/llvm_install/bin/clang-21+0xe67815)
clang: error: unable to execute command: Segmentation fault
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.0.0git
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/ruoyu/llvm_install/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/ttt-8e6328.c
clang: note: diagnostic msg: /tmp/ttt-8e6328.sh
clang: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - cabbaken

Does `-print-changed=dot-cfg` not be supported any more or this is a bug?

---

### Comment 2 - zwuis

A crash is always a bug.

---

### Comment 3 - shafik

This looks like a trunk regression, it did not crash in clang-20.

---

### Comment 4 - shafik

I can't reproduce locally.

---

### Comment 5 - cabbaken

I can reproduce this error with latest code with both Release and Debug...

---

### Comment 6 - cabbaken

I think I figured out why it crashed here.
When BB's entry has numberic lable, generateFunctionData() can't create FuncDataT with a correct EntryBlockName.
And DotCfgDiffDisplayGraph may add Nodes (in createDisplayGraph()) which don't have labels, causing the failure of label parse when emitting dot (in DotCfgDiffNode::getBodyContent()).

If we fix this, here will be some other error:
In BlockDataT, the Body should include the Label.
There are two possiblities:
1. Label is the beginning of Body(Most of the time)
  such as Label = "entry:", Body = "entry:\n %a = add i32 2, 3\n ret i32 %a"
2. Label is not the beginning of Body(When the BB has a numberic label)
  such as Label = "1:", Body = "%a = add i32 2, 3\n ret i32 %a"

With the 2nd circumstance, getNodeLabel() will return a BB without label. then the result tends to be:

<img width="216" height="77" alt="Image" src="https://github.com/user-attachments/assets/c0582a36-f962-4612-8810-a838ea601a7d" />

It will lose its label and first instruction.

---

### Comment 7 - cabbaken

Do you have any idea about this?
@shafik 

---

### Comment 8 - cabbaken

> I can't reproduce locally.

Sorry for the late reply. You can refer to the compile options in this [link](https://godbolt.org/z/PvsvTsxax) to reproduce.

---

### Comment 9 - shafik

Looks like this goes back to clang=16: https://godbolt.org/z/4M7TP39qW may be a dup of: https://github.com/llvm/llvm-project/issues/147659 which has a proposed fix: https://github.com/llvm/llvm-project/pull/148582 

---

### Comment 10 - cabbaken

I also submitted a fix for this a few months ago: https://github.com/llvm/llvm-project/pull/148844

The PR has been approved, but I do not have merge access.
Could this be accepted?

---

### Comment 11 - shafik

> I also submitted a fix for this a few months ago: [#148844](https://github.com/llvm/llvm-project/pull/148844)
> 
> The PR has been approved, but I do not have merge access. Could this be accepted?

I don't like that there are fixes, I would like some feedback from @jamieschmeiser om whether some of the changes in the previous fix should be added to yours or not.

---

