# Assertion failure "Assertion `isa<LabelDecl>(D) && "declaration not instantiated in this scope"' failed"

**Author:** hokein
**URL:** https://github.com/llvm/llvm-project/issues/140898

## Body

https://godbolt.org/z/qGcnPe635

```
template <typename T>
void s() {
  if (Unknown tr) tr;
  switch (Unknown tr)
    tr;
}

void abc() {
  s<int>();
}
```

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:4558: llvm::PointerUnion<clang::Decl*, llvm::SmallVector<clang::ValueDecl*, 4>*>* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*): Assertion `isa<LabelDecl>(D) && "declaration not instantiated in this scope"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:2:6: instantiating function definition 's<int>'
 #0 0x0000000003fa47b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fa47b8)
 #1 0x0000000003fa2444 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fa2444)
 #2 0x0000000003ee7078 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000072d9b1242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000072d9b12969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000072d9b1242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000072d9b12287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000072d9b122871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000072d9b1239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000071edce5 clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71edce5)
#10 0x00000000072974f6 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72974f6)
#11 0x00000000071dbd71 (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000007223550 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000007223a3b (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#14 0x00
```

## Comments

### Comment 1 - AaronBallman

Slightly reduced reproducer:
```
template <typename T>
void s() {
  switch (Unknown tr)
    tr;
}

void abc() {
  s<int>();
}
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Haojian Wu (hokein)

<details>
https://godbolt.org/z/qGcnPe635

```
template &lt;typename T&gt;
void s() {
  if (Unknown tr) tr;
  switch (Unknown tr)
    tr;
}

void abc() {
  s&lt;int&gt;();
}
```

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:4558: llvm::PointerUnion&lt;clang::Decl*, llvm::SmallVector&lt;clang::ValueDecl*, 4&gt;*&gt;* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*): Assertion `isa&lt;LabelDecl&gt;(D) &amp;&amp; "declaration not instantiated in this scope"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:2:6: instantiating function definition 's&lt;int&gt;'
 #<!-- -->0 0x0000000003fa47b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fa47b8)
 #<!-- -->1 0x0000000003fa2444 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fa2444)
 #<!-- -->2 0x0000000003ee7078 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000072d9b1242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000072d9b12969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000072d9b1242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000072d9b12287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000072d9b122871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000072d9b1239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000071edce5 clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71edce5)
#<!-- -->10 0x00000000072974f6 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72974f6)
#<!-- -->11 0x00000000071dbd71 (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x0000000007223550 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x0000000007223a3b (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x00
```
</details>


---

