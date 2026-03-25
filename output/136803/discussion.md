# [RISCV] error: duplicated standard user-level extension 'zifencei' since LLVM 19

**Author:** wangpc-pp
**URL:** https://github.com/llvm/llvm-project/issues/136803
**Status:** Closed
**Labels:** backend:RISC-V
**Closed Date:** 2025-04-27T03:12:48Z

## Body

See: https://godbolt.org/z/cnan5zh41

Before LLVM 19, we can compile successfully with `-march=rv64gc_zifencei`. But since LLVM 19, it complains:
```
error: invalid arch name 'rv64gc_zifencei', duplicated standard user-level extension 'zifencei'
```

The behavior is not compatible.

Is there something I missed? I know there are issues with `zifencei` implication, did we decide to not accept standalone `zifencei`? But I don't see it in release notes...

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Pengcheng Wang (wangpc-pp)

<details>
See: https://godbolt.org/z/cnan5zh41

Before LLVM 19, we can compile successfully with `-march=rv64gc_zifencei`. But since LLVM 19, it complains:
```
error: invalid arch name 'rv64gc_zifencei', duplicated standard user-level extension 'zifencei'
```

The behavior is not compatible.

Is there something I missed? I know there are issues with `zifencei` implication, did we decide to not accept standalone `zifencei`? But I don't see it in release notes...
</details>


---

### Comment 2 - topperc

Probably an unintentional break from dc90af501f00bb0bbbfde2d90360f074922e3e81.

---

### Comment 3 - wangpc-pp

I see, only LLVM 17/18 can accept `-march=rv64gc_zifencei`. Both `zicsr` and `zifencei` have this problem.

---

### Comment 4 - topperc

Possible fix.

```
diff --git a/llvm/lib/TargetParser/RISCVISAInfo.cpp b/llvm/lib/TargetParser/RISCVISAInfo.cpp
index ff0174210f87..e23e161eecfd 100644
--- a/llvm/lib/TargetParser/RISCVISAInfo.cpp
+++ b/llvm/lib/TargetParser/RISCVISAInfo.cpp
@@ -46,7 +46,11 @@ struct RISCVProfile {
 } // end anonymous namespace
 
 static const char *RISCVGImplications[] = {
-  "i", "m", "a", "f", "d", "zicsr", "zifencei"
+  "i", "m", "a", "f", "d"
+};
+
+static const char *RISCVGImplicationsZi[] = {
+  "zicsr", "zifencei"
 };
 
 #define GET_SUPPORTED_EXTENSIONS
@@ -717,6 +721,14 @@ RISCVISAInfo::parseArchString(StringRef Arch, bool EnableExperimentalExtension,
     } while (!Ext.empty());
   }
 
+  if (Baseline == 'g') {
+    for (const char *Ext : RISCVGImplicationsZi) {
+      auto Version = findDefaultVersion(Ext);
+      assert(Version && "Default extension version not found?");
+      ISAInfo->Exts[std::string(Ext)] = {Version->Major, Version->Minor};
+    }
+  }
+
   return RISCVISAInfo::postProcessAndChecking(std::move(ISAInfo));
 }
 
```

---

### Comment 5 - wangpc-pp

@topperc Thanks for the fix! I was hesitating to support this but I checked that GCC can accept specifying `zicsr/zifencei` with `g` all the time, so I think Clang should accept it too. The problem is we accidentally supported it in LLVM 17/18 but removed it since LLVM 19. We can cherry-pick the fix to LLVM 20 since we are still in the release cycle, but is it the same process for previous release LLVM 19? cc @asb @kito-cheng 

---

### Comment 6 - asb

> [@topperc](https://github.com/topperc) Thanks for the fix! I was hesitating to support this but I checked that GCC can accept specifying `zicsr/zifencei` with `g` all the time, so I think Clang should accept it too. The problem is we accidentally supported it in LLVM 17/18 but removed it since LLVM 19. We can cherry-pick the fix to LLVM 20 since we are still in the release cycle, but is it the same process for previous release LLVM 19? cc [@asb](https://github.com/asb) [@kito-cheng](https://github.com/kito-cheng)

I think the answer is basically that there won't be further LLVM 19 releases so this won't get backported to LLVM 19. If there are distros packaging LLVM 19 and supporting it long term, we can file bugs on their issue trackers to suggest they cherry-pick the patch in their downstream package.

---

