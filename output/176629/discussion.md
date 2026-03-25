# [Analyzer] 21.1.6 crashes (SIGSEGV) in VarTemplateSpecializationDecl during static analyzer on C++23 modules

**Author:** joseph-wardle
**URL:** https://github.com/llvm/llvm-project/issues/176629
**Status:** Open
**Labels:** clang:static analyzer, incomplete, crash

## Body

**Summary**
`clang-tidy` crashes with SIGSEGV while analyzing a C++23 project that uses modules (`.cppm`). Crash occurs during static analyzer (PaddingChecker) and appears related to `VarTemplateSpecializationDecl::getMostRecentDecl()`.

**Steps to reproduce**
1. Configure the project (CMake generates `build/dev` and `compile_commands.json`)
2. Run:
```
rg --files -0 -g '*.{c,cc,cpp,cxx,cppm,ixx}' -g '!build/**' -g '!third_party/**' | \
  xargs -0 clang-tidy -p build/dev
```
In my run, clang-tidy is invoked as:
```
clang-tidy -p build/dev examples/sandbox/main.cpp src/physics/physics_system.cppm src/scene/camera.cppm src/scene/pose_channel.cppm src/scene/render_view.cppm src/scene/shapes.cppm src/scene/physics_view.cppm src/scene/entity.cppm src/scene/scene.cppm src/render/render_system.cppm src/math/mat4.cppm src/render/render_targets.cppm src/math/mat3.cppm src/render/pipeline.cppm src/math/vec4.cppm src/render/render_device.cppm src/math/vec3.cppm src/render/fly_camera.cppm src/math/vec2.cppm src/render/types.cppm src/math/quat.cppm src/math/constants.cppm src/core/app.cppm src/core/logging.cppm src/core/time.cppm src/core/types.cppm src/render/render_context.cppm src/platform/input.cppm src/platform/platform.cppm src/platform/window.cppm src/render/passes/post_pass.cppm src/render/passes/world_grid_pass.cppm src/render/passes/geometry_pass.cppm
```
Crash happens on the first file processed: `examples/sandbox/main.cpp`.

**Backtrace (trimmed)**
```
#4  clang::Redeclarable<clang::VarDecl>::getFirstDecl()
#5  clang::Redeclarable<clang::VarDecl>::getMostRecentDecl()
#6  clang::VarTemplateSpecializationDecl::getMostRecentDecl()
#7  clang::RedeclarableTemplateDecl::SpecIterator<...>::operator*() const
#8  TraverseTemplateInstantiations
#9  clang::DynamicRecursiveASTVisitorBase<false>::TraverseTemplateInstantiations(...)
#10 TraverseVarTemplateDecl
...
#24 _checkDecl<(anonymous namespace)::PaddingChecker>
#25 clang::ento::CheckerManager::runCheckersOnASTDecl(...)
...
#55 clang::tidy::runClangTidy(...)
#56 clang::tidy::clangTidyMain(...)
```

**Environment**
- OS: Arch Linux (rolling)
- clang-tidy version:
```
LLVM (http://llvm.org/):
  LLVM version 21.1.6
  Optimized build.
```

The main file itself is extremely simple:
```cpp
import std;

import javelin.core.types;
import javelin.core.app;

using namespace javelin;

int main() {
    App app{};
    app.run("[hardcoded filepath]");

    return 0;
}
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: None (joseph-wardle)

<details>
**Summary**
`clang-tidy` crashes with SIGSEGV while analyzing a C++23 project that uses modules (`.cppm`). Crash occurs during static analyzer (PaddingChecker) and appears related to `VarTemplateSpecializationDecl::getMostRecentDecl()`.

**Steps to reproduce**
1. Configure the project (CMake generates `build/dev` and `compile_commands.json`)
2. Run:
```
rg --files -0 -g '*.{c,cc,cpp,cxx,cppm,ixx}' -g '!build/**' -g '!third_party/**' | \
  xargs -0 clang-tidy -p build/dev
```
In my run, clang-tidy is invoked as:
```
clang-tidy -p build/dev examples/sandbox/main.cpp src/physics/physics_system.cppm src/scene/camera.cppm src/scene/pose_channel.cppm src/scene/render_view.cppm src/scene/shapes.cppm src/scene/physics_view.cppm src/scene/entity.cppm src/scene/scene.cppm src/render/render_system.cppm src/math/mat4.cppm src/render/render_targets.cppm src/math/mat3.cppm src/render/pipeline.cppm src/math/vec4.cppm src/render/render_device.cppm src/math/vec3.cppm src/render/fly_camera.cppm src/math/vec2.cppm src/render/types.cppm src/math/quat.cppm src/math/constants.cppm src/core/app.cppm src/core/logging.cppm src/core/time.cppm src/core/types.cppm src/render/render_context.cppm src/platform/input.cppm src/platform/platform.cppm src/platform/window.cppm src/render/passes/post_pass.cppm src/render/passes/world_grid_pass.cppm src/render/passes/geometry_pass.cppm
```
Crash happens on the first file processed: `examples/sandbox/main.cpp`.

**Backtrace (trimmed)**
```
#<!-- -->4  clang::Redeclarable&lt;clang::VarDecl&gt;::getFirstDecl()
#<!-- -->5  clang::Redeclarable&lt;clang::VarDecl&gt;::getMostRecentDecl()
#<!-- -->6  clang::VarTemplateSpecializationDecl::getMostRecentDecl()
#<!-- -->7  clang::RedeclarableTemplateDecl::SpecIterator&lt;...&gt;::operator*() const
#<!-- -->8  TraverseTemplateInstantiations
#<!-- -->9  clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseTemplateInstantiations(...)
#<!-- -->10 TraverseVarTemplateDecl
...
#<!-- -->24 _checkDecl&lt;(anonymous namespace)::PaddingChecker&gt;
#<!-- -->25 clang::ento::CheckerManager::runCheckersOnASTDecl(...)
...
#<!-- -->55 clang::tidy::runClangTidy(...)
#<!-- -->56 clang::tidy::clangTidyMain(...)
```

**Environment**
- OS: Arch Linux (rolling)
- clang-tidy version:
```
LLVM (http://llvm.org/):
  LLVM version 21.1.6
  Optimized build.
```

The main file itself is extremely simple:
```cpp
import std;

import javelin.core.types;
import javelin.core.app;

using namespace javelin;

int main() {
    App app{};
    app.run("[hardcoded filepath]");

    return 0;
}
```

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-static-analyzer

Author: None (joseph-wardle)

<details>
**Summary**
`clang-tidy` crashes with SIGSEGV while analyzing a C++23 project that uses modules (`.cppm`). Crash occurs during static analyzer (PaddingChecker) and appears related to `VarTemplateSpecializationDecl::getMostRecentDecl()`.

**Steps to reproduce**
1. Configure the project (CMake generates `build/dev` and `compile_commands.json`)
2. Run:
```
rg --files -0 -g '*.{c,cc,cpp,cxx,cppm,ixx}' -g '!build/**' -g '!third_party/**' | \
  xargs -0 clang-tidy -p build/dev
```
In my run, clang-tidy is invoked as:
```
clang-tidy -p build/dev examples/sandbox/main.cpp src/physics/physics_system.cppm src/scene/camera.cppm src/scene/pose_channel.cppm src/scene/render_view.cppm src/scene/shapes.cppm src/scene/physics_view.cppm src/scene/entity.cppm src/scene/scene.cppm src/render/render_system.cppm src/math/mat4.cppm src/render/render_targets.cppm src/math/mat3.cppm src/render/pipeline.cppm src/math/vec4.cppm src/render/render_device.cppm src/math/vec3.cppm src/render/fly_camera.cppm src/math/vec2.cppm src/render/types.cppm src/math/quat.cppm src/math/constants.cppm src/core/app.cppm src/core/logging.cppm src/core/time.cppm src/core/types.cppm src/render/render_context.cppm src/platform/input.cppm src/platform/platform.cppm src/platform/window.cppm src/render/passes/post_pass.cppm src/render/passes/world_grid_pass.cppm src/render/passes/geometry_pass.cppm
```
Crash happens on the first file processed: `examples/sandbox/main.cpp`.

**Backtrace (trimmed)**
```
#<!-- -->4  clang::Redeclarable&lt;clang::VarDecl&gt;::getFirstDecl()
#<!-- -->5  clang::Redeclarable&lt;clang::VarDecl&gt;::getMostRecentDecl()
#<!-- -->6  clang::VarTemplateSpecializationDecl::getMostRecentDecl()
#<!-- -->7  clang::RedeclarableTemplateDecl::SpecIterator&lt;...&gt;::operator*() const
#<!-- -->8  TraverseTemplateInstantiations
#<!-- -->9  clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseTemplateInstantiations(...)
#<!-- -->10 TraverseVarTemplateDecl
...
#<!-- -->24 _checkDecl&lt;(anonymous namespace)::PaddingChecker&gt;
#<!-- -->25 clang::ento::CheckerManager::runCheckersOnASTDecl(...)
...
#<!-- -->55 clang::tidy::runClangTidy(...)
#<!-- -->56 clang::tidy::clangTidyMain(...)
```

**Environment**
- OS: Arch Linux (rolling)
- clang-tidy version:
```
LLVM (http://llvm.org/):
  LLVM version 21.1.6
  Optimized build.
```

The main file itself is extremely simple:
```cpp
import std;

import javelin.core.types;
import javelin.core.app;

using namespace javelin;

int main() {
    App app{};
    app.run("[hardcoded filepath]");

    return 0;
}
```

</details>


---

### Comment 3 - steakhal

Hey, thanks for the report. This looks interesting, and I have the impression that this has not much to do with the Static Analyzer, and this closely resembles a past issue I saw.
See #169302, which is caused by #110899.

The Padding checker doesn't do anything fancy, but grabs some decls and queries the `getASTRecordLayout`.

We could confirm this by the following actions:
 - try to reduce the example (the fewer and smaller files the better)
 - try to reproduce the issue without using modules
 - try to move away from clang-tidy and use clang directly, by using the `clang --analyze --analyzer-no-default-checks file.cpp -Xclang -analyzer-checker=optin.performance.Padding`
 - Once you have some files, share the preprocessed version of the input files using the `-E -o file.i` so that I could reproduce and investigate the issue myself.

Thanks again for the report.

---

