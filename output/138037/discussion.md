# [Debian Linux, LLVMv17.0.6, C++] LLVM crashes when compiling to assembler

**Author:** TheoW03
**URL:** https://github.com/llvm/llvm-project/issues/138037
**Status:** Open
**Labels:** llvm:codegen, crash

## Body

I have ran into this issue on 14 as well.
But I cant generate an assembler file without the LLVM crashing

This is the error I get 

```
terminate called without an active exception
Aborted (core dumped)
```

and here is the code 

```C++
 llvm::LLVMContext context;
    llvm::Module module("MyModule", context);

    llvm::IRBuilder<> builder(context);

// Initialize the target registry etc.llvm::InitializeAllTargets();

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    std::string Error;
    auto TargetTriple = llvm::sys::getDefaultTargetTriple();

    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);
    module.setTargetTriple(TargetTriple);
    if (output.print_llvm == 1)
    {
        module.print(llvm::outs(), nullptr);
    }

    if (!Target)
    {
        std::cout << Error << std::endl;
        return;
    }

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);
    module.setDataLayout(TargetMachine->createDataLayout());

     llvm::legacy::PassManager pass;
    
      llvm::raw_fd_ostream dest("file.s", EC, llvm::sys::fs::OF_None);
            if (EC)
            {
                llvm::errs() << "Could not open file: " << EC.message();
                return;
            }
            auto FileType = llvm::CodeGenFileType::CGFT_AssemblyFile;

            if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType))
            {
                llvm::errs() << "TargetMachine can't emit a file of this type";
                return;
            }
            pass.run(module);

            dest.flush();
```

I know this is a LLVM error because it works with generating an IR file, compiled binary, and an unoptimized IR file. 



## Comments

### Comment 1 - frederick-vs-ja

Could you test this with LLVM 20 or trunk?

---

### Comment 2 - TheoW03


I did test it on LLVM-20.0.6 
I got the same result as i did in LLVM 17.0.0

this error message 

```
pure virtual method called
terminate called without an active exception
Aborted (core dumped)
```

further testing it appears to happen here 

```
    pass.run(module);
```

where module is llvm::module 
and pass is llvvm::legacy::PassManager 
However it only happens when you have it generate an assembler file. 


if i want it to generate an ELF file or an IR file i can do so no problem. 

---

