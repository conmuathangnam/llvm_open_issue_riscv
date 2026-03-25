# perl 5.40.2 fails to run simple script when compiled with Clang-21.1.0-rc2

**Author:** kraj
**URL:** https://github.com/llvm/llvm-project/issues/152241

## Body

I use yocto to build a system using clang as default system compiler. When using clang-21 RC2, resulting perl is not working correctly. A simple reproducer is

```perl
perl -e 'use Getopt::Std;'
```

instead of executing successfully results in

```
panic: magic_killbackrefs (flags=0) at /usr/lib/perl5/5.40.2/warnings.pm line 620.
Compilation failed in require at /usr/lib/perl5/5.40.2/Getopt/Std.pm line 4.
BEGIN failed--compilation aborted at /usr/lib/perl5/5.40.2/Getopt/Std.pm line 4.
Compilation failed in require at -e line 1.
BEGIN failed--compilation aborted at -e line 1.
```

This happens on x86_64 and also on arm64 targets.

If perl is built with `-Doptimize='-O2 -fno-strict-aliasing'`  added to configure then it works correctly. It also works file when using just `-O1`

I do not have a further narrowed down reproducer than this yet. 

## Comments

### Comment 1 - thesamesam

Note that Perl builds with `-fno-strict-aliasing` in a bunch of environments (looks like it does it for non-ancient GCC, not sure if that branch will fire for Clang or not). It's also easy to accidentally override those *FLAGS that configure sets with Perl's setup.

I wouldn't be surprised if there's aliasing issues there, anyway. There was some interest in fixing those but it's a complex codebase.

---

### Comment 2 - c-rhodes

is this still something we want in LLVM 21?

---

### Comment 3 - kraj

> is this still something we want in LLVM 21?

21.x is already released. But yes it would be good to have this fixed for 22 atleast.

---

### Comment 4 - c-rhodes

Apologies I'm a new release maintainer and was getting up to speed going thru issues quite quickly with Tobias the other day when I commented. Looking at this again, a better reproducer that would be helpful. Also, are we sure this is indeed a regression in 21, did this work in previous releases? Are you able to check if this is a still an issue on main?

---

### Comment 5 - dyung

Hi, we are getting close to the end of the LLVM 21.x release cycle, so if we would like to submit a fix for this issue, it would need to be soon.

---

### Comment 6 - Leont

> looks like it does it for non-ancient GCC, not sure if that branch will fire for Clang or not

Yeah it should. I can't reproduce this issue locally (clang version 21.1.6 from Arch). The issue at hand might be a weird interaction between clang and perl's configuration system.

Could anyone tell me what such a broken perl returns for `perl -V:ccname -V:gccversion`? (the former should be `"gcc"`, the later should be your compiler's value for `__VERSION__`).

---

### Comment 7 - Leont

> I can't reproduce this issue locally

Correction: I can reproduce it locally, but only when using clang++. Indeed it seems like a configuration issue.

The `-fno-strict-aliasing` flag and others like `-pipe` and `-fstack-protector` are only enabled if the code example compiles without warnings. And that code example now generates a warning when compiled with `clang++`:
```
clang++: warning: treating 'c' input as 'c++' when in C++ mode, this behavior is deprecated [-Wdeprecated]
```

---

### Comment 8 - Leont

> If perl is built with -Doptimize='-O2 -fno-strict-aliasing' added to configure then it works correctly.

I would instead recommend something like `-Accflags=-fno-strict-aliasing` when working around an issue like this. `optimize` is much more likely to be overriden for specific CPAN modules, which could break things in "hilarious" ways.

---

