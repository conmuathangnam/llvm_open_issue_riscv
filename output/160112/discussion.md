# Regression(https://reviews.llvm.org/D143540): llvm-lib writes broken .lib files when output is > 4 GiB

**Author:** nico
**URL:** https://github.com/llvm/llvm-project/issues/160112
**Status:** Closed
**Labels:** miscompilation, llvm, regression:17
**Closed Date:** 2025-09-28T18:17:02Z

## Body

Reduced repro (file names > 15 characters so that the string table is used):

```
% cat foo34567890123456789.cc 
void foo() {}
```

```
% cat bar34567890123456789.cc 
void bar() {}
```

Compile them and make a .lib, using `SYM64_THRESHOLD` to force switch to 64-bit mode without needing many large obj files:

```
% out/gn/bin/clang-cl /c foo34567890123456789.cc                                                                                 
% out/gn/bin/clang-cl /c bar34567890123456789.cc                                                             
% SYM64_THRESHOLD=800 out/gn/bin/lld-link /lib /out:foo.lib foo34567890123456789.obj bar34567890123456789.obj
```

Use it:

```
% cat main.cc   
void foo();
void bar();
int main() {
  foo();
  bar();
}

% out/gn/bin/clang-cl /c main.cc

% out/gn/bin/lld-link /winsysroot:$HOME/src/chrome/src/third_party/depot_tools/win_toolchain/vs_files/e4305f407e main.obj foo.lib
lld-link: error: could not get child name for archive foo.lib while loading symbol void __cdecl foo(void): truncated or malformed archive (string table at long name offset 25 not terminated)
```

The problem is that `writeArchiveToStream` in llvm/lib/Object/ArchiveWriter.cpp dynamically switches `Kind` to `object::Archive::K_GNU64` if the output becomes large enough to require 64-bit offsets, but after https://reviews.llvm.org/D143540 / 4fcbf384200756, several places in ArchiveWriter.cpp check `Kind` and make binding decisions based on it before `Kind` may change.

Options:

1. Short-term, revert the change to LibDriver.cpp to always use `K_GNU` to unbreak things
2. In ArchiveWriter, maybe do a first pass to figure out the final Kind, and then don't change `Kind` while writing in a second, actually-writing pass

@cjacek 

## Comments

### Comment 1 - nico

(downstream: https://crbug.com/445194079)

---

### Comment 2 - nico

I also wonder what lib.exe does when you hand it obj files that need more than 4 GiB total. Does it error out? Something else?

---

### Comment 3 - nico

Another idea, if that matches lib.exe's behavior: make ArchiveWriter.cpp error out instead of changing to GNU64 when writing coff archives, and give LibWriter a flag to force GNU output.

---

### Comment 4 - cjacek

Thanks! I guess we could try harder to make the output work with something like: https://github.com/cjacek/llvm-project/commit/309934a8ab3a25ce18b53e27e69580ddcf01a2b2. This fixes the repro for me.

As far as I remember, lib.exe errors out on overly large archives. I'm not sure whether we should replicate that, large archives seem possible to support (at least in non-EC cases), although less efficiently than for other types.

llvm-ar already has a `--format` argument. I'm not sure how practical that is to use, but I guess we could add it to llvm-lib as well if that would help.

---

### Comment 5 - nico

Thanks!

Abstractly, having an `lld-link /lib` flag that controls if it errors out on large outputs, transparently switches to GNU64 when needed, or just always writes GNU seems useful.

But concretely, nobody really asked for it, so maybe we can YAGNI it for now and just go with your patch – it's better than the current behavior (which is writing corrupt files). And if someone needs more flexibility in the future, we can add that then.

(But if you'd rather do something else, that's fine with me too!)

---

### Comment 6 - nico

@cjacek Were your planning on uploading your commit, or would you prefer if I do it?

---

### Comment 7 - cjacek

I agree, I think we can go ahead with the patch now and see if there’s demand for an explicit option. I’ve created #160606.

(I’m traveling this week, so I may be less responsive than usual.)

---

