# [RISCV][CostModel] opt crashed and triggered assertion `(SrcLT.first == 1) && (DstLT.first == 1) && "Illegal type"'

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/166732
**Status:** Closed
**Labels:** backend:RISC-V, crash-on-valid
**Closed Date:** 2025-11-10T09:01:44Z

## Body

Reproducer: https://godbolt.org/z/fjhevfM8b
Reduced testcase:
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-musl"

define <512 x i2> @backsmith_pure_9(<16 x i64> %BS_ARG_0) #0 {
entry:
  %0 = bitcast <16 x i64> %BS_ARG_0 to <512 x i2>
  ret <512 x i2> %0
}

attributes #0 = { "target-features"="+v" }
```

opt output:
```
opt: /root/llvm-project/llvm/lib/Target/RISCV/RISCVTargetTransformInfo.cpp:1690: virtual llvm::InstructionCost llvm::RISCVTTIImpl::getCastInstrCost(unsigned int, llvm::Type*, llvm::Type*, llvm::TargetTransformInfo::CastContextHint, llvm::TargetTransformInfo::TargetCostKind, const llvm::Instruction*) const: Assertion `(SrcLT.first == 1) && (DstLT.first == 1) && "Illegal type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -O1 <source>
1.	Running pass "ipsccp" on module "<source>"
 #0 0x00000000059aa7e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x59aa7e8)
 #1 0x00000000059a7694 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f300f042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007f300f0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007f300f042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007f300f0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007f300f02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007f300f039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000001db220c llvm::RISCVTTIImpl::getCastInstrCost(unsigned int, llvm::Type*, llvm::Type*, llvm::TargetTransformInfo::CastContextHint, llvm::TargetTransformInfo::TargetCostKind, llvm::Instruction const*) const (.part.0) RISCVTargetTransformInfo.cpp:0:0
 #9 0x0000000001dc818f llvm::TargetTransformInfoImplCRTPBase<llvm::RISCVTTIImpl>::getInstructionCost(llvm::User const*, llvm::ArrayRef<llvm::Value const*>, llvm::TargetTransformInfo::TargetCostKind) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x1dc818f)
#10 0x00000000050409ad llvm::TargetTransformInfo::getInstructionCost(llvm::User const*, llvm::ArrayRef<llvm::Value const*>, llvm::TargetTransformInfo::TargetCostKind) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x50409ad)
#11 0x0000000004d547aa llvm::CodeMetrics::analyzeBasicBlock(llvm::BasicBlock const*, llvm::TargetTransformInfo const&, llvm::SmallPtrSetImpl<llvm::Value const*> const&, bool, llvm::Loop const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4d547aa)
#12 0x0000000003402713 llvm::FunctionSpecializer::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3402713)
#13 0x00000000033ca78b runIPSCCP(llvm::Module&, llvm::DataLayout const&, llvm::AnalysisManager<llvm::Function>*, std::function<llvm::TargetLibraryInfo const& (llvm::Function&)>, std::function<llvm::TargetTransformInfo& (llvm::Function&)>, std::function<llvm::AssumptionCache& (llvm::Function&)>, std::function<llvm::DominatorTree& (llvm::Function&)>, std::function<llvm::BlockFrequencyInfo& (llvm::Function&)>, bool) SCCP.cpp:0:0
#14 0x00000000033cc2bd llvm::IPSCCPPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x33cc2bd)
#15 0x0000000002fc41ee llvm::detail::PassModel<llvm::Module, llvm::IPSCCPPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fc41ee)
#16 0x0000000005781121 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5781121)
#17 0x000000000098745a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x98745a)
#18 0x000000000097b6d1 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97b6d1)
#19 0x00007f300f029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#20 0x00007f300f029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#21 0x0000000000972ae5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x972ae5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Hongyu Chen (XChy)

<details>
Reproducer: https://godbolt.org/z/fjhevfM8b
Reduced testcase:
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-musl"

define &lt;512 x i2&gt; @<!-- -->backsmith_pure_9(&lt;16 x i64&gt; %BS_ARG_0) #<!-- -->0 {
entry:
  %0 = bitcast &lt;16 x i64&gt; %BS_ARG_0 to &lt;512 x i2&gt;
  ret &lt;512 x i2&gt; %0
}

attributes #<!-- -->0 = { "target-features"="+v" }
```

opt output:
```
opt: /root/llvm-project/llvm/lib/Target/RISCV/RISCVTargetTransformInfo.cpp:1690: virtual llvm::InstructionCost llvm::RISCVTTIImpl::getCastInstrCost(unsigned int, llvm::Type*, llvm::Type*, llvm::TargetTransformInfo::CastContextHint, llvm::TargetTransformInfo::TargetCostKind, const llvm::Instruction*) const: Assertion `(SrcLT.first == 1) &amp;&amp; (DstLT.first == 1) &amp;&amp; "Illegal type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -O1 &lt;source&gt;
1.	Running pass "ipsccp" on module "&lt;source&gt;"
 #<!-- -->0 0x00000000059aa7e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x59aa7e8)
 #<!-- -->1 0x00000000059a7694 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007f300f042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007f300f0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007f300f042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007f300f0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00007f300f02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x00007f300f039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000001db220c llvm::RISCVTTIImpl::getCastInstrCost(unsigned int, llvm::Type*, llvm::Type*, llvm::TargetTransformInfo::CastContextHint, llvm::TargetTransformInfo::TargetCostKind, llvm::Instruction const*) const (.part.0) RISCVTargetTransformInfo.cpp:0:0
 #<!-- -->9 0x0000000001dc818f llvm::TargetTransformInfoImplCRTPBase&lt;llvm::RISCVTTIImpl&gt;::getInstructionCost(llvm::User const*, llvm::ArrayRef&lt;llvm::Value const*&gt;, llvm::TargetTransformInfo::TargetCostKind) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x1dc818f)
#<!-- -->10 0x00000000050409ad llvm::TargetTransformInfo::getInstructionCost(llvm::User const*, llvm::ArrayRef&lt;llvm::Value const*&gt;, llvm::TargetTransformInfo::TargetCostKind) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x50409ad)
#<!-- -->11 0x0000000004d547aa llvm::CodeMetrics::analyzeBasicBlock(llvm::BasicBlock const*, llvm::TargetTransformInfo const&amp;, llvm::SmallPtrSetImpl&lt;llvm::Value const*&gt; const&amp;, bool, llvm::Loop const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4d547aa)
#<!-- -->12 0x0000000003402713 llvm::FunctionSpecializer::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3402713)
#<!-- -->13 0x00000000033ca78b runIPSCCP(llvm::Module&amp;, llvm::DataLayout const&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;*, std::function&lt;llvm::TargetLibraryInfo const&amp; (llvm::Function&amp;)&gt;, std::function&lt;llvm::TargetTransformInfo&amp; (llvm::Function&amp;)&gt;, std::function&lt;llvm::AssumptionCache&amp; (llvm::Function&amp;)&gt;, std::function&lt;llvm::DominatorTree&amp; (llvm::Function&amp;)&gt;, std::function&lt;llvm::BlockFrequencyInfo&amp; (llvm::Function&amp;)&gt;, bool) SCCP.cpp:0:0
#<!-- -->14 0x00000000033cc2bd llvm::IPSCCPPass::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x33cc2bd)
#<!-- -->15 0x0000000002fc41ee llvm::detail::PassModel&lt;llvm::Module, llvm::IPSCCPPass, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fc41ee)
#<!-- -->16 0x0000000005781121 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5781121)
#<!-- -->17 0x000000000098745a llvm::runPassPipeline(llvm::StringRef, llvm::Module&amp;, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef&lt;llvm::PassPlugin&gt;, llvm::ArrayRef&lt;std::function&lt;void (llvm::PassBuilder&amp;)&gt;&gt;, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x98745a)
#<!-- -->18 0x000000000097b6d1 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97b6d1)
#<!-- -->19 0x00007f300f029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->20 0x00007f300f029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->21 0x0000000000972ae5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x972ae5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - topperc

CC: @ElvisWang123 

---

### Comment 3 - XChy

For your information, this testcase is reduced from a valid C program containing `_BitInt`. Maybe the bitwidth is changed after reduction. Let me know if anyone needs an unreduced C testcase.

---

