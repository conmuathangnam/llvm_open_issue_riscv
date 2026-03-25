# Emit pass - LLVM ERROR - 64-bit code requested on a subtarget that doesn't support it

**Author:** aldrinmathew
**URL:** https://github.com/llvm/llvm-project/issues/172254
**Status:** Open
**Labels:** backend:X86, crash

## Body

LLVM Version: `20.1.5`

In my compiler, in case the user hasn't provided a target triple or CPU name or CPU features, I ask LLVM for the host's CPU name and CPU features.

In my case, the values are as follows:

CPU Name: `znver3`
CPU Features: `prfchw`, `avx`, `aes`, `sahf`, `pclmul`, `crc32`, `xsaves`, `sse4.1`, `xsave`, `sse4.2`, `invpcid`, `64bit`, `xsavec`, `cmov`, `movbe`, `adx`, `avx2`, `vpclmulqdq`, `clflushopt`, `bmi`, `sse`, `xsaveopt`, `rdrnd`, `cx8`, `sse3`, `pku`, `fsgsbase`, `clzero`, `mwaitx`, `lzcnt`, `sha`, `wbnoinvd`, `ssse3`, `cx16`, `bmi2`, `fma`, `popcnt`, `f16c`, `rdpru`, `clwb`, `mmx`, `sse2`, `rdseed`, `rdpid`, `shstk`, `vaes`, `fxsr`, `sse4a`

I am using `llvm::sys::getHostCPUName` and `llvm::sys::getHostCPUFeatures` to retrieve the above information.

When I run the emit pass, the following error occurs:

```log
LLVM ERROR: 64-bit code requested on a subtarget that doesn't support it!
```

I tried skipping the `64bit` CPU feature to check what happens. I get  the following error in one of the IR functions:
```log
error: <unknown>:0:0:
in function std:float_to_string:[f64] %"std::String" (double):
SSE register return with SSE disabled
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Aldrin Mathew (aldrinmathew)

<details>
LLVM Version: `20.1.5`

In my compiler, in case the user hasn't provided a target triple or CPU name or CPU features, I ask LLVM for the host's CPU name and CPU features.

In my case, the values are as follows:

CPU Name: `znver3`
CPU Features: `prfchw`, `avx`, `aes`, `sahf`, `pclmul`, `crc32`, `xsaves`, `sse4.1`, `xsave`, `sse4.2`, `invpcid`, `64bit`, `xsavec`, `cmov`, `movbe`, `adx`, `avx2`, `vpclmulqdq`, `clflushopt`, `bmi`, `sse`, `xsaveopt`, `rdrnd`, `cx8`, `sse3`, `pku`, `fsgsbase`, `clzero`, `mwaitx`, `lzcnt`, `sha`, `wbnoinvd`, `ssse3`, `cx16`, `bmi2`, `fma`, `popcnt`, `f16c`, `rdpru`, `clwb`, `mmx`, `sse2`, `rdseed`, `rdpid`, `shstk`, `vaes`, `fxsr`, `sse4a`

I am using `llvm::sys::getHostCPUName` and `llvm::sys::getHostCPUFeatures` to retrieve the above information.

When I run the emit pass, the following error occurs:

```log
LLVM ERROR: 64-bit code requested on a subtarget that doesn't support it!
```

I tried skipping the `64bit` CPU feature to check what happens. I get  the following error in one of the IR functions:
```log
error: &lt;unknown&gt;:0:0:
in function std:float_to_string:[f64] %"std::String" (double):
SSE register return with SSE disabled
```
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 21 or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.


---

### Comment 3 - nikic

Can you please provide the full IR / bitcode you are feeding to LLVM?

---

### Comment 4 - aldrinmathew

@nikic 

Sure. I am attaching the LLVM files.
You might have a hard time going through them as the name mangling for my language is a mess for now.

[file-main.txt](https://github.com/user-attachments/files/24162356/file-main.txt)
[file-std.txt](https://github.com/user-attachments/files/24162358/file-std.txt)
[file-std_file-fs.txt](https://github.com/user-attachments/files/24162357/file-std_file-fs.txt)
[file-std_file-matrix.txt](https://github.com/user-attachments/files/24162361/file-std_file-matrix.txt)
[file-std_file-string.txt](https://github.com/user-attachments/files/24162360/file-std_file-string.txt)
[file-std_file-sys.txt](https://github.com/user-attachments/files/24162359/file-std_file-sys.txt)

---

### Comment 5 - aldrinmathew

@EugeneZelenko 
Hi, I just built the latest version 21.1.7 of LLVM from the source archive in the releases section.
The issue persists. The exact same error.
When I remove the `64bit` feature, the other error also pops up.

---

### Comment 6 - aldrinmathew

@nikic 
I enabled logging before compiling each IR file, it seems the error happens when emit pass is done for `file-main.ll`

```
Compiling module `main.qat` from file /mnt/main/dev/qatlang/qat/test/main.qat
```

---

### Comment 7 - phoebewang

> the error happens when emit pass is done for `file-main.ll`

Do you mean the error happens when you skipping the 64bit CPU feature? You can add `+sse2` to aviod it. It seems you did not only skip 64bit, but all also all the rest features from what I see in the attachment.

---

### Comment 8 - nikic

I don't see any mentioned of `"target-features"` inside file-main.ll, so it's not really clear to me how you are feeding the computed target features to LLVM / how you are invoking codegen. Just running `llc` on that file works fine (well, if I run it through `opt -passes='default<O0>' -enable-matrix` to lower the matrix intrinsic first).

---

### Comment 9 - aldrinmathew

@phoebewang 
No the main error `LLVM ERROR: 64-bit code requested on a subtarget that doesn't support it!` happens when all of the features listed in the description are enabled. The error occurs while emit pass is run on the `file-main.ll` file
When I remove `64bit` from the list of features, then the error changes to
```
error: <unknown>:0:0:
in function std:float_to_string:[f64] %"std::String" (double):
SSE register return with SSE disabled
```
Do note that the second error happens in a different file: `file-std_file-string.ll`

@nikic 
Apologies for not making it clear. I use the LLVM C++ API.
The specific code I use to feed the features to LLVM is:
```cpp
auto target = llvm::TargetRegistry::lookupTarget(cfg->get_target_triple(), errStr);
// Err is handled here
llvm::TargetOptions targetOptions;
String              cpuName = "generic";
String              cpuFeatures;
if (not cfg->has_target_triple() && not cfg->has_cpu_name()) {
	cpuName = llvm::sys::getHostCPUName().str();
	auto features = llvm::sys::getHostCPUFeatures();
	for (auto& item : features) {
		if (item.getValue()) {
			if (not cpuFeatures.empty()) {
				cpuFeatures += ",";
			}
			cpuFeatures += item.getKey();
		}
	}
}
if (cfg->has_cpu_name()) { // This is false for this test case
	cpuName = cfg->get_cpu_name();
}
if (cfg->has_cpu_features()) { // This is also false for this test case
	cpuFeatures = cfg->get_cpu_features();
}
ctx->targetMachine = target->createTargetMachine(cfg->get_target_triple(), cpuName,
					cpuFeatures, targetOptions, llvm::Reloc::PIC_);
```

---

### Comment 10 - aldrinmathew

@phoebewang 
My bad, after seeing your above comment, I tried prepending `+` to the feature keys in the above code, and now the compilation works without issues.

@EugeneZelenko @nikic 

Besides my blunder, this is a horrible error message for not having a `+` before the feature key.
1. Please either change `createTargetMachine` to accept just feature keys as a list
or
2. Document the format required for the features in `createTargetMachine`

---

