# [mlir][mlir-tblgen] Duplication on default builder and custom builder on attribute causes segmentation fault.

**Author:** JustinKim98
**URL:** https://github.com/llvm/llvm-project/issues/160227
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2025-09-28T12:16:29Z

## Body

Hi I am working on potential bug on `mlir-tblgen`.
Current upstream version of `mlir-tblgen` emits segmentation fault if

1. User-defined attribute builder's parameters overlap (or-subsumes) with default-generated builder
2. Default builder is not skipped

Here's how to reproduce.
If I try to build `RangeAttr` here with `Example_Dialect` and
its custom builder `AttrBuilder<(ins "std::int64_t" :$lowerBound, "std::int64_t" :$upperBound, "std::int64_t" :$step)`
overlaps with definition that default builder would generate.

```tblgen
#ifndef EXAMPLE_TD
#define EXAMPLE_TD

include "mlir/IR/OpBase.td"

def Example_Dialect : Dialect {
    let name = "example";
    let description = [{
        Example dialect
    }];

    let cppNamespace = "::mlir::example";

    let hasCanonicalizer = true;

    let hasConstantMaterializer = true;
    let useDefaultAttributePrinterParser = true;
    let useDefaultTypePrinterParser = true;
}

class Example_Attr<string attrName, string attrMnemonic, list<Trait> traits = []> :
    AttrDef<Example_Dialect, attrName, traits>{
        let mnemonic = attrMnemonic;
}

def Example_RangeAttr : Example_Attr<"Range", "range">{
    let summary = "Range attribute for representing range";
    let description = [{
        Range attribute represents range.

        Range is used for indexing tensors.
    }];

    let parameters = (ins AttrParameter<"std::int64_t", "lowerBound">:$lowerBound, AttrParameter<"std::int64_t", "upperBound">:$upperBound, AttrParameter<"std::int64_t", "step">:$step);
    let builders = [
        AttrBuilder<(ins "std::int64_t" :$lowerBound, "std::int64_t" :$upperBound, "std::int64_t" :$step) ,[{
            return get(context, lowerBound, upperBound, step);
        }]>
    ];

    let extraClassDeclaration = [{
        std::int64_t getNumIterations(){
            return llvm::divideCeilSigned(getUpperBound() - getLowerBound(), getStep());
        }
    }];

    // If This is set to false, generation of builder fails, and results in segmentation fault.
    let skipDefaultBuilders = false;


    let hasCustomAssemblyFormat = true;
}


class Example_Op<string mnemonic, list<Trait> traits = []> :
    Op<Example_Dialect, mnemonic, traits>;

def Example_CreateRangeFromAttrOp : Example_Op<"create_range_from_attr", []> {
    let summary = "Creates new range from attribute";
    let description = [{
        `example.create_range_from_attr` operation creates new range from attribute.
    }];
    
    let arguments = (ins Example_RangeAttr:$rangeAttr);
    let assemblyFormat = "$rangeAttr attr-dict";

    let hasVerifier = true;

    let builders = [
        OpBuilder<(ins "mlir::example::RangeAttr":$rangeAttr), [{
            build($_builder, $_state,  rangeAttr);
        }]>
    ];
}

#endif

```

Running `mlir-tblgen` on this file with `-gen-attrdef-decls`
```bash
mlir-tblgen -gen-attrdef-decls -attrdefs-dialect=example  -I/root/projects/llvm-project/mlir/include  /root/projects/hyperaccel-sdk/reproduce.td --write-if-changed -o example_output.h.inc
```

would result in following stack trace
```
~/projects/mlir-debug/bin/mlir-tblgen -gen-attrdef-decls -attrdefs-dialect=example  -I/root/projects/llvm-project/mlir/include  /root/projects/hyperaccel-sdk/reproduce.td --write-if-changed -o output.h.inc
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /root/projects/mlir-debug/bin/mlir-tblgen -gen-attrdef-decls -attrdefs-dialect=example -I/root/projects/llvm-project/mlir/include /root/projects/hyperaccel-sdk/reproduce.td --write-if-changed -o output.h.inc
 #0 0x000056033ce2ea89 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /root/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x000056033ce2efab PrintStackTraceSignalHandler(void*) /root/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:917:1
 #2 0x000056033ce2ceff llvm::sys::RunSignalHandlers() /root/projects/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x000056033ce2f669 SignalHandler(int, siginfo_t*, void*) /root/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007f50a86cb520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x000056033cb8f24c mlir::raw_indented_ostream::indent() /root/projects/llvm-project/mlir/include/mlir/Support/IndentedOstream.h:69:19
 #6 0x000056033cb8e8bd mlir::tblgen::MethodBody::indent() /root/projects/llvm-project/mlir/include/mlir/TableGen/Class.h:235:8
 #7 0x000056033cb72c2f (anonymous namespace)::DefGen::emitCustomBuilder(mlir::tblgen::AttrOrTypeBuilder const&) /root/projects/llvm-project/mlir/tools/mlir-tblgen/AttrOrTypeDefGen.cpp:537:13
 #8 0x000056033cb7051b (anonymous namespace)::DefGen::emitBuilders() /root/projects/llvm-project/mlir/tools/mlir-tblgen/AttrOrTypeDefGen.cpp:339:5
 #9 0x000056033cb6d348 (anonymous namespace)::DefGen::DefGen(mlir::tblgen::AttrOrTypeDef const&) /root/projects/llvm-project/mlir/tools/mlir-tblgen/AttrOrTypeDefGen.cpp:0:5
#10 0x000056033cb75ea4 (anonymous namespace)::DefGenerator::emitDecls(llvm::StringRef) /root/projects/llvm-project/mlir/tools/mlir-tblgen/AttrOrTypeDefGen.cpp:830:7
#11 0x000056033cb75a80 $_5::operator()(llvm::RecordKeeper const&, llvm::raw_ostream&) const /root/projects/llvm-project/mlir/tools/mlir-tblgen/AttrOrTypeDefGen.cpp:1202:37
#12 0x000056033cb75a25 bool std::__invoke_impl<bool, $_5&, llvm::RecordKeeper const&, llvm::raw_ostream&>(std::__invoke_other, $_5&, llvm::RecordKeeper const&, llvm::raw_ostream&) /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/invoke.h:61:7
#13 0x000056033cb759c5 std::enable_if<is_invocable_r_v<bool, $_5&, llvm::RecordKeeper const&, llvm::raw_ostream&>, bool>::type std::__invoke_r<bool, $_5&, llvm::RecordKeeper const&, llvm::raw_ostream&>($_5&, llvm::RecordKeeper const&, llvm::raw_ostream&) /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/invoke.h:114:2
#14 0x000056033cb7590d std::_Function_handler<bool (llvm::RecordKeeper const&, llvm::raw_ostream&), $_5>::_M_invoke(std::_Any_data const&, llvm::RecordKeeper const&, llvm::raw_ostream&) /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/std_function.h:290:2
#15 0x000056033ceb4b6e std::function<bool (llvm::RecordKeeper const&, llvm::raw_ostream&)>::operator()(llvm::RecordKeeper const&, llvm::raw_ostream&) const /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/std_function.h:590:2
#16 0x000056033ceb3419 mlir::GenInfo::invoke(llvm::RecordKeeper const&, llvm::raw_ostream&) const /root/projects/llvm-project/mlir/include/mlir/TableGen/GenInfo.h:39:5
#17 0x000056033ceb1613 mlirTableGenMain(llvm::raw_ostream&, llvm::RecordKeeper const&) /root/projects/llvm-project/mlir/lib/Tools/mlir-tblgen/MlirTblgenMain.cpp:136:3
#18 0x000056033ceb8815 bool std::__invoke_impl<bool, bool (*&)(llvm::raw_ostream&, llvm::RecordKeeper const&), llvm::raw_ostream&, llvm::RecordKeeper const&>(std::__invoke_other, bool (*&)(llvm::raw_ostream&, llvm::RecordKeeper const&), llvm::raw_ostream&, llvm::RecordKeeper const&) /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/invoke.h:61:7
#19 0x000056033ceb87b5 std::enable_if<is_invocable_r_v<bool, bool (*&)(llvm::raw_ostream&, llvm::RecordKeeper const&), llvm::raw_ostream&, llvm::RecordKeeper const&>, bool>::type std::__invoke_r<bool, bool (*&)(llvm::raw_ostream&, llvm::RecordKeeper const&), llvm::raw_ostream&, llvm::RecordKeeper const&>(bool (*&)(llvm::raw_ostream&, llvm::RecordKeeper const&), llvm::raw_ostream&, llvm::RecordKeeper const&) /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/invoke.h:114:2
#20 0x000056033ceb86ed std::_Function_handler<bool (llvm::raw_ostream&, llvm::RecordKeeper const&), bool (*)(llvm::raw_ostream&, llvm::RecordKeeper const&)>::_M_invoke(std::_Any_data const&, llvm::raw_ostream&, llvm::RecordKeeper const&) /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/std_function.h:290:2
#21 0x000056033ceb9e9e std::function<bool (llvm::raw_ostream&, llvm::RecordKeeper const&)>::operator()(llvm::raw_ostream&, llvm::RecordKeeper const&) const /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/std_function.h:590:2
#22 0x000056033ceb8e6f llvm::TableGenMain(char const*, std::function<bool (llvm::raw_ostream&, llvm::RecordKeeper const&)>) /root/projects/llvm-project/llvm/lib/TableGen/Main.cpp:153:23
#23 0x000056033ceb1574 mlir::MlirTblgenMain(int, char**) /root/projects/llvm-project/mlir/lib/Tools/mlir-tblgen/MlirTblgenMain.cpp:156:10
#24 0x000056033cc29872 main /root/projects/llvm-project/mlir/tools/mlir-tblgen/mlir-tblgen.cpp:28:35
#25 0x00007f50a86b2d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#26 0x00007f50a86b2e40 call_init ./csu/../csu/libc-start.c:128:20
#27 0x00007f50a86b2e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#28 0x000056033cb6a765 _start (/root/projects/mlir-debug/bin/mlir-tblgen+0x1c765)
Segmentation fault (core dumped)
```

I observed this error, and this is caused by not properly handling case when `addMethod` returns nullptr (It returns nullptr if given custom builder overlaps with one of pre-existing builders).

```cpp
void DefGen::emitCustomBuilder(const AttrOrTypeBuilder &builder) {
  // Don't emit a body if there isn't one.
  auto props = builder.getBody() ? Method::Static : Method::StaticDeclaration;
  StringRef returnType = def.getCppClassName();
  if (std::optional<StringRef> builderReturnType = builder.getReturnType())
    returnType = *builderReturnType;

  // m is nullptr if custom builder is redundant.
  Method *m = defCls.addMethod(returnType, "get", props,
                               getCustomBuilderParams({}, builder));

  if (!builder.getBody())
    return;

  // Format the body and emit it.
  FmtContext ctx;
  ctx.addSubst("_get", "Base::get");
  if (!builder.hasInferredContextParameter())
    ctx.addSubst("_ctxt", "context");
  std::string bodyStr = tgfmt(*builder.getBody(), &ctx);

  // Segmentation fault is occurring here
  m->body().indent().getStream().printReindented(bodyStr);
}
```

I observed similar pattern is used for other places as well, but for other cases, they are used for C++ methods such as
`verify`, `construct` or `parse` and `print` which cannot be duplicated by the user.

However, for builders, users can always generated duplicated form of them. Instead of emitting segmentation fault here, I think I'm refactoring this to check whether `addMethod` was successful, and return without doing anything if given custom builder was redundant.
 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Justin Kim (JustinKim98)

<details>
Hi I am working on potential bug on `mlir-tblgen`.
Current upstream version of `mlir-tblgen` emits segmentation fault if

1. User-defined attribute builder's parameters overlap (or-subsumes) with default-generated builder
2. Default builder is not skipped

Here's how to reproduce.
If I try to build `RangeAttr` here with `Example_Dialect` and
its custom builder `AttrBuilder&lt;(ins "std::int64_t" :$lowerBound, "std::int64_t" :$upperBound, "std::int64_t" :$step)`
Overlaps with definition that default builder would generate.

```tblgen
#ifndef EXAMPLE_TD
#define EXAMPLE_TD

include "mlir/IR/OpBase.td"

def Example_Dialect : Dialect {
    let name = "example";
    let description = [{
        Example dialect
    }];

    let cppNamespace = "::mlir::example";

    let hasCanonicalizer = true;

    let hasConstantMaterializer = true;
    let useDefaultAttributePrinterParser = true;
    let useDefaultTypePrinterParser = true;
}

class Example_Attr&lt;string attrName, string attrMnemonic, list&lt;Trait&gt; traits = []&gt; :
    AttrDef&lt;Example_Dialect, attrName, traits&gt;{
        let mnemonic = attrMnemonic;
}

def Example_RangeAttr : Example_Attr&lt;"Range", "range"&gt;{
    let summary = "Range attribute for representing range";
    let description = [{
        Range attribute represents range.

        Range is used for indexing tensors.
    }];

    let parameters = (ins AttrParameter&lt;"std::int64_t", "lowerBound"&gt;:$lowerBound, AttrParameter&lt;"std::int64_t", "upperBound"&gt;:$upperBound, AttrParameter&lt;"std::int64_t", "step"&gt;:$step);
    let builders = [
        AttrBuilder&lt;(ins "std::int64_t" :$lowerBound, "std::int64_t" :$upperBound, "std::int64_t" :$step) ,[{
            return get(context, lowerBound, upperBound, step);
        }]&gt;
    ];

    let extraClassDeclaration = [{
        std::int64_t getNumIterations(){
            return llvm::divideCeilSigned(getUpperBound() - getLowerBound(), getStep());
        }
    }];

    // If This is set to false, generation of builder fails, and results in segmentation fault.
    let skipDefaultBuilders = false;


    let hasCustomAssemblyFormat = true;
}


class Example_Op&lt;string mnemonic, list&lt;Trait&gt; traits = []&gt; :
    Op&lt;Example_Dialect, mnemonic, traits&gt;;

def Example_CreateRangeFromAttrOp : Example_Op&lt;"create_range_from_attr", []&gt; {
    let summary = "Creates new range from attribute";
    let description = [{
        `example.create_range_from_attr` operation creates new range from attribute.
    }];
    
    let arguments = (ins Example_RangeAttr:$rangeAttr);
    let assemblyFormat = "$rangeAttr attr-dict";

    let hasVerifier = true;

    let builders = [
        OpBuilder&lt;(ins "mlir::example::RangeAttr":$rangeAttr), [{
            build($_builder, $_state,  rangeAttr);
        }]&gt;
    ];
}

#endif

```

Running `mlir-tblgen` on this file
```bash
mlir-tblgen -gen-attrdef-decls -attrdefs-dialect=example  -I/root/projects/llvm-project/mlir/include  /root/projects/hyperaccel-sdk/reproduce.td --write-if-changed -o example_output.h.inc
```

would result in following stack trace
```
~/projects/mlir-debug/bin/mlir-tblgen -gen-attrdef-decls -attrdefs-dialect=example  -I/root/projects/llvm-project/mlir/include  /root/projects/hyperaccel-sdk/reproduce.td --write-if-changed -o output.h.inc
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /root/projects/mlir-debug/bin/mlir-tblgen -gen-attrdef-decls -attrdefs-dialect=example -I/root/projects/llvm-project/mlir/include /root/projects/hyperaccel-sdk/reproduce.td --write-if-changed -o output.h.inc
 #<!-- -->0 0x000056033ce2ea89 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /root/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x000056033ce2efab PrintStackTraceSignalHandler(void*) /root/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:917:1
 #<!-- -->2 0x000056033ce2ceff llvm::sys::RunSignalHandlers() /root/projects/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x000056033ce2f669 SignalHandler(int, siginfo_t*, void*) /root/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f50a86cb520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x000056033cb8f24c mlir::raw_indented_ostream::indent() /root/projects/llvm-project/mlir/include/mlir/Support/IndentedOstream.h:69:19
 #<!-- -->6 0x000056033cb8e8bd mlir::tblgen::MethodBody::indent() /root/projects/llvm-project/mlir/include/mlir/TableGen/Class.h:235:8
 #<!-- -->7 0x000056033cb72c2f (anonymous namespace)::DefGen::emitCustomBuilder(mlir::tblgen::AttrOrTypeBuilder const&amp;) /root/projects/llvm-project/mlir/tools/mlir-tblgen/AttrOrTypeDefGen.cpp:537:13
 #<!-- -->8 0x000056033cb7051b (anonymous namespace)::DefGen::emitBuilders() /root/projects/llvm-project/mlir/tools/mlir-tblgen/AttrOrTypeDefGen.cpp:339:5
 #<!-- -->9 0x000056033cb6d348 (anonymous namespace)::DefGen::DefGen(mlir::tblgen::AttrOrTypeDef const&amp;) /root/projects/llvm-project/mlir/tools/mlir-tblgen/AttrOrTypeDefGen.cpp:0:5
#<!-- -->10 0x000056033cb75ea4 (anonymous namespace)::DefGenerator::emitDecls(llvm::StringRef) /root/projects/llvm-project/mlir/tools/mlir-tblgen/AttrOrTypeDefGen.cpp:830:7
#<!-- -->11 0x000056033cb75a80 $_5::operator()(llvm::RecordKeeper const&amp;, llvm::raw_ostream&amp;) const /root/projects/llvm-project/mlir/tools/mlir-tblgen/AttrOrTypeDefGen.cpp:1202:37
#<!-- -->12 0x000056033cb75a25 bool std::__invoke_impl&lt;bool, $_5&amp;, llvm::RecordKeeper const&amp;, llvm::raw_ostream&amp;&gt;(std::__invoke_other, $_5&amp;, llvm::RecordKeeper const&amp;, llvm::raw_ostream&amp;) /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/invoke.h:61:7
#<!-- -->13 0x000056033cb759c5 std::enable_if&lt;is_invocable_r_v&lt;bool, $_5&amp;, llvm::RecordKeeper const&amp;, llvm::raw_ostream&amp;&gt;, bool&gt;::type std::__invoke_r&lt;bool, $_5&amp;, llvm::RecordKeeper const&amp;, llvm::raw_ostream&amp;&gt;($_5&amp;, llvm::RecordKeeper const&amp;, llvm::raw_ostream&amp;) /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/invoke.h:114:2
#<!-- -->14 0x000056033cb7590d std::_Function_handler&lt;bool (llvm::RecordKeeper const&amp;, llvm::raw_ostream&amp;), $_5&gt;::_M_invoke(std::_Any_data const&amp;, llvm::RecordKeeper const&amp;, llvm::raw_ostream&amp;) /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/std_function.h:290:2
#<!-- -->15 0x000056033ceb4b6e std::function&lt;bool (llvm::RecordKeeper const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(llvm::RecordKeeper const&amp;, llvm::raw_ostream&amp;) const /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/std_function.h:590:2
#<!-- -->16 0x000056033ceb3419 mlir::GenInfo::invoke(llvm::RecordKeeper const&amp;, llvm::raw_ostream&amp;) const /root/projects/llvm-project/mlir/include/mlir/TableGen/GenInfo.h:39:5
#<!-- -->17 0x000056033ceb1613 mlirTableGenMain(llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;) /root/projects/llvm-project/mlir/lib/Tools/mlir-tblgen/MlirTblgenMain.cpp:136:3
#<!-- -->18 0x000056033ceb8815 bool std::__invoke_impl&lt;bool, bool (*&amp;)(llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;), llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;&gt;(std::__invoke_other, bool (*&amp;)(llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;), llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;) /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/invoke.h:61:7
#<!-- -->19 0x000056033ceb87b5 std::enable_if&lt;is_invocable_r_v&lt;bool, bool (*&amp;)(llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;), llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;&gt;, bool&gt;::type std::__invoke_r&lt;bool, bool (*&amp;)(llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;), llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;&gt;(bool (*&amp;)(llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;), llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;) /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/invoke.h:114:2
#<!-- -->20 0x000056033ceb86ed std::_Function_handler&lt;bool (llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;), bool (*)(llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;)&gt;::_M_invoke(std::_Any_data const&amp;, llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;) /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/std_function.h:290:2
#<!-- -->21 0x000056033ceb9e9e std::function&lt;bool (llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;)&gt;::operator()(llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;) const /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/std_function.h:590:2
#<!-- -->22 0x000056033ceb8e6f llvm::TableGenMain(char const*, std::function&lt;bool (llvm::raw_ostream&amp;, llvm::RecordKeeper const&amp;)&gt;) /root/projects/llvm-project/llvm/lib/TableGen/Main.cpp:153:23
#<!-- -->23 0x000056033ceb1574 mlir::MlirTblgenMain(int, char**) /root/projects/llvm-project/mlir/lib/Tools/mlir-tblgen/MlirTblgenMain.cpp:156:10
#<!-- -->24 0x000056033cc29872 main /root/projects/llvm-project/mlir/tools/mlir-tblgen/mlir-tblgen.cpp:28:35
#<!-- -->25 0x00007f50a86b2d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->26 0x00007f50a86b2e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->27 0x00007f50a86b2e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->28 0x000056033cb6a765 _start (/root/projects/mlir-debug/bin/mlir-tblgen+0x1c765)
Segmentation fault (core dumped)
```

I observed this error, and this is caused by not properly handling case when `addMethod` returns nullptr (It returns nullptr if given custom builder overlaps with one of pre-existing builders).

```cpp
void DefGen::emitCustomBuilder(const AttrOrTypeBuilder &amp;builder) {
  // Don't emit a body if there isn't one.
  auto props = builder.getBody() ? Method::Static : Method::StaticDeclaration;
  StringRef returnType = def.getCppClassName();
  if (std::optional&lt;StringRef&gt; builderReturnType = builder.getReturnType())
    returnType = *builderReturnType;

  // m is nullptr if custom builder is redundant.
  Method *m = defCls.addMethod(returnType, "get", props,
                               getCustomBuilderParams({}, builder));

  if (!builder.getBody())
    return;

  // Format the body and emit it.
  FmtContext ctx;
  ctx.addSubst("_get", "Base::get");
  if (!builder.hasInferredContextParameter())
    ctx.addSubst("_ctxt", "context");
  std::string bodyStr = tgfmt(*builder.getBody(), &amp;ctx);

  // Segmentation fault is occurring here
  m-&gt;body().indent().getStream().printReindented(bodyStr);
}
```

I observed similar pattern is used for other places as well, but for other cases, they are used for C++ methods such as
`verify`, `construct` or `parse` and `print` which cannot be duplicated by the user.

However, for builders, users can always generated duplicated form of them. Instead of emitting segmentation fault here, I think I'm refactoring this to check whether `addMethod` was successful, and return without doing anything if given custom builder was redundant.
 
</details>


---

### Comment 2 - JustinKim98

I am currently working on it to fix this. I'm also checking if there are other cases that would cause similar issue.

---

