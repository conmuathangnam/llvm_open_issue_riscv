# DeadArgumentElimination creates invalid code with musttail calls

**Author:** djwatson
**URL:** https://github.com/llvm/llvm-project/issues/126817

## Body

Similar to one with structs #107569, but it appears to happen even without structs:

```
define i64 @A() {
entry:
  %v2660 = musttail call i64 @B()
  ret i64 %v2660
}

define internal i64 @B() {
entry:
  ret i64 0
}

define internal i64 @C() {
entry:
  %v30543 = musttail call i64 @B()
  ret i64 %v30543
}
```

```
 opt -S -passes=deadargelim reduced.ll
cannot guarantee tail call due to mismatched return types
  %v30543 = musttail call i64 @B()
LLVM ERROR: Broken module found, compilation aborted!

opt -version
Ubuntu LLVM version 19.1.1
  Optimized build.
  Default target: x86_64-pc-linux-gnu
  Host CPU: znver3

```

Using llvm 19.1.1, but also appears to still happen in whatever compiler explorer has for trunk (21.0.0git)

https://godbolt.org/z/f6Pb88Kfc


