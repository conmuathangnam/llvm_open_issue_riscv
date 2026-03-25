# [MLIR] Crash occurs in mlir-runner when running a GPU program

**Author:** LynnBroe
**URL:** https://github.com/llvm/llvm-project/issues/178581
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-28T14:39:52Z

## Body

I ran a GPU dialect program using mlir-runner, and it crashed. See the code example below.

### Lowered MLIR Program
```mlir
module attributes {gpu.container_module} {
  llvm.func @malloc(i64) -> !llvm.ptr
  llvm.func @main() {
    %0 = llvm.mlir.poison : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)>
    %1 = llvm.mlir.poison : !llvm.struct<(ptr, ptr, i64, array<2 x i64>, array<2 x i64>)>
    %2 = llvm.mlir.zero : !llvm.ptr
    %3 = llvm.mlir.constant(0 : i32) : i32
    %4 = llvm.mlir.constant(1 : i32) : i32
    %5 = llvm.mlir.constant(2 : i32) : i32
    %6 = llvm.mlir.constant(4 : i32) : i32
    %7 = llvm.mlir.constant(8 : i32) : i32
    %8 = llvm.mlir.constant(16 : i32) : i32
    %9 = llvm.mlir.constant(3 : i32) : i32
    %10 = llvm.mlir.constant(6 : i32) : i32
    %11 = llvm.mlir.constant(7 : i32) : i32
    %12 = llvm.mlir.constant(10 : i32) : i32
    %13 = llvm.mlir.constant(11 : i32) : i32
    %14 = llvm.mlir.constant(0 : index) : i64
    %15 = llvm.mlir.constant(1 : index) : i64
    %16 = llvm.mlir.constant(2 : index) : i64
    %17 = llvm.mlir.constant(3 : index) : i64
    %18 = llvm.mlir.constant(4 : index) : i64
    %19 = llvm.mlir.constant(5 : index) : i64
    %20 = llvm.mlir.constant(6 : index) : i64
    %21 = llvm.getelementptr %2[12] : (!llvm.ptr) -> !llvm.ptr, i32
    %22 = llvm.ptrtoint %21 : !llvm.ptr to i64
    %23 = llvm.call @malloc(%22) : (i64) -> !llvm.ptr
    %24 = llvm.insertvalue %23, %1[0] : !llvm.struct<(ptr, ptr, i64, array<2 x i64>, array<2 x i64>)> 
    %25 = llvm.insertvalue %23, %24[1] : !llvm.struct<(ptr, ptr, i64, array<2 x i64>, array<2 x i64>)> 
    %26 = llvm.insertvalue %14, %25[2] : !llvm.struct<(ptr, ptr, i64, array<2 x i64>, array<2 x i64>)> 
    %27 = llvm.insertvalue %16, %26[3, 0] : !llvm.struct<(ptr, ptr, i64, array<2 x i64>, array<2 x i64>)> 
    %28 = llvm.insertvalue %20, %27[3, 1] : !llvm.struct<(ptr, ptr, i64, array<2 x i64>, array<2 x i64>)> 
    %29 = llvm.insertvalue %20, %28[4, 0] : !llvm.struct<(ptr, ptr, i64, array<2 x i64>, array<2 x i64>)> 
    %30 = llvm.insertvalue %15, %29[4, 1] : !llvm.struct<(ptr, ptr, i64, array<2 x i64>, array<2 x i64>)> 
    %31 = llvm.getelementptr %2[2] : (!llvm.ptr) -> !llvm.ptr, i32
    %32 = llvm.ptrtoint %31 : !llvm.ptr to i64
    %33 = llvm.call @malloc(%32) : (i64) -> !llvm.ptr
    %34 = llvm.insertvalue %33, %0[0] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %35 = llvm.insertvalue %33, %34[1] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %36 = llvm.insertvalue %14, %35[2] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %37 = llvm.insertvalue %16, %36[3, 0] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %38 = llvm.insertvalue %15, %37[4, 0] : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> 
    %39 = llvm.alloca %15 x !llvm.struct<(ptr, ptr, i64, array<2 x i64>, array<2 x i64>)> : (i64) -> !llvm.ptr
    llvm.store %30, %39 : !llvm.struct<(ptr, ptr, i64, array<2 x i64>, array<2 x i64>)>, !llvm.ptr
    %40 = llvm.getelementptr %2[1] : (!llvm.ptr) -> !llvm.ptr, i32
    %41 = llvm.ptrtoint %40 : !llvm.ptr to i64
    llvm.call @mgpuMemHostRegisterMemRef(%16, %39, %41) : (i64, !llvm.ptr, i64) -> ()
    %42 = llvm.alloca %15 x !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)> : (i64) -> !llvm.ptr
    llvm.store %38, %42 : !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)>, !llvm.ptr
    llvm.call @mgpuMemHostRegisterMemRef(%15, %42, %41) : (i64, !llvm.ptr, i64) -> ()
    %43 = llvm.mul %14, %20 overflow<nsw, nuw> : i64
    %44 = llvm.add %43, %14 overflow<nsw, nuw> : i64
    %45 = llvm.getelementptr inbounds|nuw %23[%44] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    llvm.store %3, %45 : i32, !llvm.ptr
    %46 = llvm.add %43, %15 overflow<nsw, nuw> : i64
    %47 = llvm.getelementptr inbounds|nuw %23[%46] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    llvm.store %4, %47 : i32, !llvm.ptr
    %48 = llvm.add %43, %16 overflow<nsw, nuw> : i64
    %49 = llvm.getelementptr inbounds|nuw %23[%48] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    llvm.store %5, %49 : i32, !llvm.ptr
    %50 = llvm.add %43, %17 overflow<nsw, nuw> : i64
    %51 = llvm.getelementptr inbounds|nuw %23[%50] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    llvm.store %6, %51 : i32, !llvm.ptr
    %52 = llvm.add %43, %18 overflow<nsw, nuw> : i64
    %53 = llvm.getelementptr inbounds|nuw %23[%52] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    llvm.store %7, %53 : i32, !llvm.ptr
    %54 = llvm.add %43, %19 overflow<nsw, nuw> : i64
    %55 = llvm.getelementptr inbounds|nuw %23[%54] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    llvm.store %8, %55 : i32, !llvm.ptr
    %56 = llvm.mul %15, %20 overflow<nsw, nuw> : i64
    %57 = llvm.add %56, %14 overflow<nsw, nuw> : i64
    %58 = llvm.getelementptr inbounds|nuw %23[%57] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    llvm.store %5, %58 : i32, !llvm.ptr
    %59 = llvm.add %56, %15 overflow<nsw, nuw> : i64
    %60 = llvm.getelementptr inbounds|nuw %23[%59] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    llvm.store %9, %60 : i32, !llvm.ptr
    %61 = llvm.add %56, %16 overflow<nsw, nuw> : i64
    %62 = llvm.getelementptr inbounds|nuw %23[%61] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    llvm.store %10, %62 : i32, !llvm.ptr
    %63 = llvm.add %56, %17 overflow<nsw, nuw> : i64
    %64 = llvm.getelementptr inbounds|nuw %23[%63] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    llvm.store %11, %64 : i32, !llvm.ptr
    %65 = llvm.add %56, %18 overflow<nsw, nuw> : i64
    %66 = llvm.getelementptr inbounds|nuw %23[%65] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    llvm.store %12, %66 : i32, !llvm.ptr
    %67 = llvm.add %56, %19 overflow<nsw, nuw> : i64
    %68 = llvm.getelementptr inbounds|nuw %23[%67] : (!llvm.ptr, i64) -> !llvm.ptr, i32
    llvm.store %13, %68 : i32, !llvm.ptr
    gpu.launch_func  @main_kernel::@main_kernel blocks in (%16, %15, %15) threads in (%20, %15, %15) : i64 args(%23 : !llvm.ptr, %23 : !llvm.ptr, %14 : i64, %16 : i64, %20 : i64, %20 : i64, %15 : i64, %33 : !llvm.ptr, %33 : !llvm.ptr, %14 : i64, %16 : i64, %15 : i64)
    llvm.call @printMemrefI32(%15, %42) : (i64, !llvm.ptr) -> ()
    llvm.return
  }
  gpu.binary @main_kernel  [#gpu.object<#nvvm.target, properties = {ISAToBinaryTimeInMs = 16 : i64, LLVMIRToISATimeInMs = 30 : i64}, "P\EDU\BA\01\00\10\00\90\11\00\00\00\00\00\00\02\00\01\01@\00\00\00\C0\0C\00\00\00\00\00\00\00\00\00\00\00\00\00\00\07\00\01\002\00\00\00\00\00\00\00\00\00\00\00\11\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\7FELF\02\01\013\07\00\00\00\00\00\00\00\02\00\BE\00\80\00\00\00\00\00\00\00\00\00\00\00\E0\0B\00\00\00\00\00\00 \09\00\00\00\00\00\002\052\00@\008\00\04\00@\00\0B\00\01\00\00.shstrtab\00.strtab\00.symtab\00.symtab_shndx\00.nv.info\00.text.main_kernel\00.nv.info.main_kernel\00.nv.shared.main_kernel\00.nv.constant0.main_kernel\00.rel.nv.constant0.main_kernel\00.nv.callgraph\00.nv.prototype\00.nv.rel.action\00\00.shstrtab\00.strtab\00.symtab\00.symtab_shndx\00.nv.info\00.text.main_kernel\00.nv.info.main_kernel\00.nv.shared.main_kernel\00.rel.nv.constant0.main_kernel\00.nv.constant0.main_kernel\00.nv.callgraph\00.nv.prototype\00.nv.rel.action\00main_kernel\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\002\00\00\00\03\00\09\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00Y\00\00\00\03\00\0A\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\8E\00\00\00\03\00\08\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\A8\00\00\00\03\00\06\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\C4\00\00\00\03\00\07\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\D3\00\00\00\12\10\09\00\00\00\00\00\00\00\00\00\00\03\00\00\00\00\00\00\04/\08\00\06\00\00\00\09\00\00\00\04\11\08\00\06\00\00\00\00\00\00\00\04\12\08\00\06\00\00\00\00\00\00\00\047\04\00\80\00\00\00\010\00\00\01*\00\00\04\0A\08\00\03\00\00\00@\01`\00\03\19`\00\04\17\0C\00\00\00\00\00\0B\00X\00\00\F0!\00\04\17\0C\00\00\00\00\00\0A\00P\00\00\F0!\00\04\17\0C\00\00\00\00\00\09\00H\00\00\F0!\00\04\17\0C\00\00\00\00\00\08\00@\00\00\F5!\00\04\17\0C\00\00\00\00\00\07\008\00\00\F5!\00\04\17\0C\00\00\00\00\00\06\000\00\00\F0!\00\04\17\0C\00\00\00\00\00\05\00(\00\00\F0!\00\04\17\0C\00\00\00\00\00\04\00 \00\00\F0!\00\04\17\0C\00\00\00\00\00\03\00\18\00\00\F0!\00\04\17\0C\00\00\00\00\00\02\00\10\00\00\F0!\00\04\17\0C\00\00\00\00\00\01\00\08\00\00\F5!\00\04\17\0C\00\00\00\00\00\00\00\00\00\00\F5!\00\03\1B\FF\00\04)(\00\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\04((\00\E8\00\00\00\08\01\00\00(\01\00\00H\01\00\00p\01\00\00\C8\01\00\00\E8\01\00\00\08\02\00\00(\02\00\00H\02\00\00\04\1D\04\00\18\00\00\00\04\1C\04\00\F0\02\00\00\044 \00\B0\01\00\00\00\00\00\00\01\00\00\00x\02\00\00p\02\00\00\00\00\00\00\01\00\00\00x\02\00\00\04\05\0C\00\06\00\00\00\01\00\00\00\01\00\00\00\04\1E\04\00 \00\00\00\00\00\00\00\FF\FF\FF\FF\00\00\00\00\FE\FF\FF\FF\00\00\00\00\FD\FF\FF\FF\00\00\00\00\FC\FF\FF\FF\00\00\00\00s\00\00\00\00\00\00\00\00\00\00\11%\00\056\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\F6\07\00\FE\00D\1C\00\01\00\87\00\80\07\98L\00\F0\87\01\00\00\00\01\07\00W\02\00\00\C8\F0?\07\00\FE\C1\C4\1F\00\08\00\17\02\00\00\C8\F0\05\07\07\00\80\7F\00[\00\00\80#\00\00\90\E2\F1\07&\FE\C0\CC\1F\18\02\07\07\00\80\7F\00[\04\07\07\00\A8\7F\00[\03\07\07\00\88\7F\00[\E1\07 \FE@\D4\1F\00\05\07\07\00\80\02([\00\07\07\00\88\7F [\02\07G\00\18\010[\F1\07 \FE\00\D4_\00\03\057\00 \00\C0\\\02\02'\05\00\80\10L\00\08\E7\01\00\00(8\E1\07\C0\FE\00\C8\1F\00\03\037\05\00\08\10L\02\08'\00\00\81\18\\\03\007\00\00\08\10\\\B1\07\C0\FE\00\D0\1F\00\02\02\07\00\00 \D4\EE\00\F0\17\00\00\00\00\01\00\00'\00\00\00\12L\1F\07\C1\FC\01\D0\1F\00\04\02\17\D0\00\00\10\EF\04\04\F7\FF\7F\00\A09\05\04'\00\00\00G\\\1F\07\C0\FE\01\D0\1F\00\04\05'\D0\00\00\10\EF\04\04\F7\FF\7F\00\A09\06\05G\00\00\00G\\\1F\07\C0\FE\01\D0\1F\00\04\06G\D0\00\00\10\EF\04\04\F7\FF\7F\00\A09\03\06G\00\00\00G\\\1F\07 \FC\01\D4\1F\00\04\03\87\D0\00\00\10\EF\04\04\F7\FF\7F\00\A09\07\08\F7\0F\80\03j[\F4\07\E0\E3\00\D8?\00\02\03G\00\00\00G\\\04\02\07\D1\00\00\11\EF\04\04\F8\FF\FF\00\A09\F4\07 \FE\00\FC\1F\00\04\02H\00\00\00G\\\04\FF\08\00\00\00\\\EF\00\00\07\00\80\1B\A8\F0\F5\07\A0\FF\00|\1C\00\00\00\07\00\00\00\98\EF\0F\00\00\00\00\00\F8\F0\00\FF\07\00\00\10L\EF\1F\0F\C0\FE\01\D0\1F\00\02\00\17\D0\80\7F\10\EF\02\02\F7\FF\7F\00\A09\02\02\07\00\00\00G\\\1F\07\C0\FE\01\D0\1F\00\03\02'\D0\80\7F\10\EF\03\03\F7\FF\7F\00\A09\03\027\00\00\00G\\\1F\07\C0\FE\01\90\1F\00\04\03G\D0\80\7F\10\EF\04\04\F7\FF\7F\00\A09\04\03G\00\00\00G\\\1F\07\C0\FE\01\D0\1F\00\05\04\87\D0\80\7F\10\EF\05\05\F7\FF\7F\00\A09\05\04W\00\00\00G\\\1F\07\C0\FE\01\D0\1F\00\00\05\07\D1\80\7F\10\EF\00\00\F7\FF\7F\00\A09\00\05\07\00\00\00G\\\F1\00\A0\FF\00\BC\1F\00\00\FF\07\00\00\00\\\EF\0F\00\07\00\00\00\F8\F0\00\0F\07\00\00\00\B0P\EF\07\00\FD\00\FC\1F\00\00\0F\07\00\00\00\B0P\00\0F\07\00\00\00\B0P\00\00\07\00\80\1B\A8\F0\F5\07 \E2\01\B4\1F\00\00\00\07\00\00\00\98\EF\00\FF\07\00\00\10L\EF\02\07\07\06\00\81\D7K\FD\07 \FE\01\BC\1F\00\03\07\17\06\80\7F\17\1A\00\02\07\00\00 \DC\EE\00\0F\07\00\00\00\B0P\E8\07\E0\FF\00\FC\1F\00\00\0F\07\00\00\00\B0P\0F\00\07\00\00\00\00\E3\0F\00\87\FF\FF\0F@\E2\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\01\00\00\00\03\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00@\00\00\00\00\00\00\00\D3\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\01\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\0B\00\00\00\03\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\13\01\00\00\00\00\00\00\DF\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\01\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\13\00\00\00\02\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\F8\01\00\00\00\00\00\00\A8\00\00\00\00\00\00\00\02\00\00\00\06\00\00\00\08\00\00\00\00\00\00\00\18\00\00\00\00\00\00\00)\00\00\00\00\00\00p\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\A0\02\00\00\00\00\00\00$\00\00\00\00\00\00\00\03\00\00\00\00\00\00\00\04\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00D\00\00\00\00\00\00p@\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\C4\02\00\00\00\00\00\00\88\01\00\00\00\00\00\00\03\00\00\00\09\00\00\00\04\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\A8\00\00\00\01\00\00p\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00L\04\00\00\00\00\00\00 \00\00\00\00\00\00\00\03\00\00\00\00\00\00\00\04\00\00\00\00\00\00\00\08\00\00\00\00\00\00\00\C4\00\00\00\0B\00\00p\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00p\04\00\00\00\00\00\00\10\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\08\00\00\00\00\00\00\00\08\00\00\00\00\00\00\00p\00\00\00\01\00\00\00B\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\80\04\00\00\00\00\00\00\A0\01\00\00\00\00\00\00\00\00\00\00\09\00\00\00\04\00\00\00\00\00\00\00\00\00\00\00\00\00\00\002\00\00\00\01\00\00\00\06\00\10\00\00\00\00\00\00\00\00\00\00\00\00\00 \06\00\00\00\00\00\00\00\03\00\00\00\00\00\00\03\00\00\00\06\00\00\09 \00\00\00\00\00\00\00\00\00\00\00\00\00\00\00Y\00\00\00\08\00\00\00C\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00 \09\00\00\00\00\00\00\04\00\00\00\00\00\00\00\00\00\00\00\09\00\00\00\04\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\06\00\00\00\05\00\00\00\E0\0B\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\E0\00\00\00\00\00\00\00\E0\00\00\00\00\00\00\00\08\00\00\00\00\00\00\00\01\00\00\00\05\00\00\00\80\04\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\A0\04\00\00\00\00\00\00\A0\04\00\00\00\00\00\00\08\00\00\00\00\00\00\00\01\00\00\00\06\00\00\00 \09\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\04\00\00\00\00\00\00\00\08\00\00\00\00\00\00\00\01\00\00\00\05\00\00\00\E0\0B\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\E0\00\00\00\00\00\00\00\E0\00\00\00\00\00\00\00\08\00\00\00\00\00\00\00\01\00\01\01P\00\00\00@\04\00\00\00\00\00\00@\04\00\00@\00\00\00\00\00\06\002\00\00\00\00\00\00\00\00\00\00\00\11 \00\00\00\00\00\00\00\00\00\00\00\00\00\00\E0\09\00\00\00\00\00\00H\00\00\00\00\00\00\00\00\00\00\00\00\00\00\000\0A//\03\00\F6\1E\0A.version 6.0\0A.target sm_50\0A.address_size 64\0A4\00\F8%isible .entry main_kernel(\0A.param .u64 .ptr .align 1'\00\11_%\00?_0,/\00\1A\1A1/\00\0E!\00\1F2!\00\0D\1F3!\00\0D\1F4!\00\0D\1F5!\00\0D\1F6\D4\00\1B\1F7/\00\1B\1F8\7F\00\0D\1F9!\00\0D/10\22\00\0E\F3\1A1\0A)\0A.maxntid 6, 1, 1\0A{\0A.reg .pred %p<12>;\13\00\96b32 %r<41\12\00b64 %rd%\00p//\0A.sha8\00\03\DA\00\C84 .u32 __wg_\80\00\920_$_0;\0Ald\A1\00\04G\00O3, [\A7\00\00\F2\04];\0Acvta.to.global.u0\00!4,6\00\0EI\00\1F5I\00\02\1F8I\00\06\111I\00a5;\0Amov\B2\00\D8%r3, %ctaid.x\17\00\00*\00\12t\15\00rsetp.ne\16\012p1,\1E\00\180.\00\00\A1\00\13n/\00\83mul.wide\1B\002d6,f\00\9224;\0Aadd.s\8B\00&7,\DA\00\1E66\00\138g\00\0A5\00&9,;\00\118\0F\01\03\E0\00\02\B2\01\00\0F\01\00&\00\81];\0Asub.s\15\00P6, 322\00\124\B2\00\02+\00\A37, -1;\0Ashrr\00\228,\16\003%r6s\00\01\17\00\01r\00\144-\00\C3fl.sync.bflyI\00r10|%p2,~\00$1,3\00\1088\01$lp%\00\026\01\100H\00\A4, %p2;\0Aand\1D\00%2,#\00\1F5f\00\05c3|%p3,.\00\1F2g\00\05\01O\01$13g\00\193g\00&5,m\00\1F4h\00\05c6|%p4,/\00\1F4h\00\05\01(\01$16h\00\194h\00&8,n\00\1F7h\00\05c9|%p5,/\00\1F8h\00\04120,,\00\04h\00\185h\00$21?\00\00)\00\0Fh\00\04s22|%p6,/\00(16i\00\FA\03@%p1 bra $L__BB0_2}\00\01o\01$22}\00\196}\00\15,\82\00c23;\0Ast\11\04\01\1F\00\1F[\08\04\03\11].\00%;\0Ao\00\10:\A6\03\00\9B\03\13.\F5\02\01V\00a3;\0Abar\CA\00/ 0\A6\00\00\114\F8\02\07o\00_%r24,u\00\06\0F\19\01\05c5|%p7,=\00\00\D0\03:, 1\00\01\01-\01$25\00\01\197\00\01&7,#\00\0FM\02\05s28|%p8,/\00\1F2d\00\01\01\8C\01$28d\00\188d\00630,j\00\1F9d\00\04s31|%p9,/\00\1F4d\00\00\02\FD\03$31d\00\199d\00%3,j\00/32d\00\05@4|%p\BD\03\020\00\1F8e\00\01\01\CC\02$34e\00)10f\00&6,l\00\0F\E3\03\05 37f\00#1,0\00/16g\00\01\01N\01%37g\00\191g\00&9,m\00\1F8\96\02\19&39\97\02)4:\80\02\0Cl\02/40l\02\0B\14l\9C\06\021\01\00\D4\02\192\C4\05\02\E3\00\03\8B\06!10\A7\00\07\C6\05\11[#\00\01\96\00\D040;\0Aret;\0A\0A}\0A\00">]
  llvm.func @printMemrefI32(i64, !llvm.ptr) attributes {sym_visibility = "private"}
  llvm.func @mgpuMemHostRegisterMemRef(i64, !llvm.ptr, i64)
}
```

### Crash Log
```bash
Failed to materialize symbols: { (main, { $.LLVMDialectModule.__inits.0, main, __orc_deinit_func.LLVMDialectModule, _mlir_main_kernel_unload, _mlir_main_kernel_load, _mlir_main, __orc_init_func.LLVMDialectModule }) }
UNREACHABLE executed at /llvm-project/llvm/include/llvm/Support/Error.h:779!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-runner lowered.mlir -e main -entry-point-result=void -shared-libs=/llvm-project/build_all/lib/libmlir_c_runner_utils.so,/llvm-project/build_all/lib/libmlir_runner_utils.so
 #0 0x000055c1438272cd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x000055c1438277fb PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x000055c1438256cf llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x000055c143827e99 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007f5fcf245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007f5fcf29eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f5fcf29eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007f5fcf29eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007f5fcf24527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007f5fcf2288ff abort ./stdlib/abort.c:81:7
#10 0x000055c143769c88 /llvm-project/llvm/lib/Support/ErrorHandling.cpp:244:3
#11 0x000055c1433e8601 llvm::cantFail(llvm::Error, char const*) /llvm-project/llvm/include/llvm/Support/Error.h:781:1
#12 0x000055c144348543 mlir::ExecutionEngine::initialize() /llvm-project/mlir/lib/ExecutionEngine/ExecutionEngine.cpp:457:5
#13 0x000055c1442f659f compileAndExecute((anonymous namespace)::Options&, mlir::Operation*, llvm::StringRef, (anonymous namespace)::CompileAndExecuteConfig, void**, std::unique_ptr<llvm::TargetMachine, std::default_delete<llvm::TargetMachine>>) /llvm-project/mlir/lib/ExecutionEngine/JitRunner.cpp:207:23
#14 0x000055c1442f611f compileAndExecuteVoidFunction((anonymous namespace)::Options&, mlir::Operation*, llvm::StringRef, (anonymous namespace)::CompileAndExecuteConfig, std::unique_ptr<llvm::TargetMachine, std::default_delete<llvm::TargetMachine>>) /llvm-project/mlir/lib/ExecutionEngine/JitRunner.cpp:241:3
#15 0x000055c1442f4917 mlir::JitRunnerMain(int, char**, mlir::DialectRegistry const&, mlir::JitRunnerConfig) /llvm-project/mlir/lib/ExecutionEngine/JitRunner.cpp:398:17
#16 0x000055c1433bd0e3 main /llvm-project/mlir/tools/mlir-runner/mlir-runner.cpp:93:3
#17 0x00007f5fcf22a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#18 0x00007f5fcf22a28b call_init ./csu/../csu/libc-start.c:128:20
#19 0x00007f5fcf22a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#20 0x000055c1433bcf15 _start (/mnt/raid/llvm-project/build_all/bin/mlir-runner+0x424f15)
Aborted
```

I used the command to run mlir-runner: `mlir-runner lowered.mlir -e main -entry-point-result=void -shared-libs=/llvm-project/build_all/lib/libmlir_c_runner_utils.so,/llvm-project/build_all/lib/libmlir_runner_utils.so`.

To reproduce, the original MLIR code is:
```mlir
func.func @main() {
  %data = memref.alloc() : memref<2x6xi32>
  %sum = memref.alloc() : memref<2xi32>
  %cst0 = arith.constant 0 : i32
  %cst1 = arith.constant 1 : i32
  %cst2 = arith.constant 2 : i32
  %cst4 = arith.constant 4 : i32
  %cst8 = arith.constant 8 : i32
  %cst16 = arith.constant 16 : i32

  %cst3 = arith.constant 3 : i32
  %cst6 = arith.constant 6 : i32
  %cst7 = arith.constant 7 : i32
  %cst10 = arith.constant 10 : i32
  %cst11 = arith.constant 11 : i32

  %c0 = arith.constant 0 : index
  %c1 = arith.constant 1 : index
  %c2 = arith.constant 2 : index
  %c3 = arith.constant 3 : index
  %c4 = arith.constant 4 : index
  %c5 = arith.constant 5 : index
  %c6 = arith.constant 6 : index

  %cast_data = memref.cast %data : memref<2x6xi32> to memref<*xi32>
  gpu.host_register %cast_data : memref<*xi32>
  %cast_sum = memref.cast %sum : memref<2xi32> to memref<*xi32>
  gpu.host_register %cast_sum : memref<*xi32>

  memref.store %cst0, %data[%c0, %c0] : memref<2x6xi32>
  memref.store %cst1, %data[%c0, %c1] : memref<2x6xi32>
  memref.store %cst2, %data[%c0, %c2] : memref<2x6xi32>
  memref.store %cst4, %data[%c0, %c3] : memref<2x6xi32>
  memref.store %cst8, %data[%c0, %c4] : memref<2x6xi32>
  memref.store %cst16, %data[%c0, %c5] : memref<2x6xi32>

  memref.store %cst2, %data[%c1, %c0] : memref<2x6xi32>
  memref.store %cst3, %data[%c1, %c1] : memref<2x6xi32>
  memref.store %cst6, %data[%c1, %c2] : memref<2x6xi32>
  memref.store %cst7, %data[%c1, %c3] : memref<2x6xi32>
  memref.store %cst10, %data[%c1, %c4] : memref<2x6xi32>
  memref.store %cst11, %data[%c1, %c5] : memref<2x6xi32>

  gpu.launch blocks(%bx, %by, %bz) in (%grid_x = %c2, %grid_y = %c1, %grid_z = %c1)
             threads(%tx, %ty, %tz) in (%block_x = %c6, %block_y = %c1, %block_z = %c1) {
    %val = memref.load %data[%bx, %tx] : memref<2x6xi32>
    %reduced = gpu.all_reduce and %val uniform {} : (i32) -> (i32)
    memref.store %reduced, %sum[%bx] : memref<2xi32>
    gpu.terminator
  }

  call @printMemrefI32(%cast_sum) : (memref<*xi32>) -> ()

  return
}

func.func private @printMemrefI32(memref<*xi32>)
```

The lowering command is: `mlir-opt -gpu-lower-to-nvvm-pipeline`

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: HKSDev (LynnBroe)

<details>
I ran a GPU dialect program using mlir-runner, and it crashed. See the code example below.

### Lowered MLIR Program
```mlir
module attributes {gpu.container_module} {
  llvm.func @<!-- -->malloc(i64) -&gt; !llvm.ptr
  llvm.func @<!-- -->main() {
    %0 = llvm.mlir.poison : !llvm.struct&lt;(ptr, ptr, i64, array&lt;1 x i64&gt;, array&lt;1 x i64&gt;)&gt;
    %1 = llvm.mlir.poison : !llvm.struct&lt;(ptr, ptr, i64, array&lt;2 x i64&gt;, array&lt;2 x i64&gt;)&gt;
    %2 = llvm.mlir.zero : !llvm.ptr
    %3 = llvm.mlir.constant(0 : i32) : i32
    %4 = llvm.mlir.constant(1 : i32) : i32
    %5 = llvm.mlir.constant(2 : i32) : i32
    %6 = llvm.mlir.constant(4 : i32) : i32
    %7 = llvm.mlir.constant(8 : i32) : i32
    %8 = llvm.mlir.constant(16 : i32) : i32
    %9 = llvm.mlir.constant(3 : i32) : i32
    %10 = llvm.mlir.constant(6 : i32) : i32
    %11 = llvm.mlir.constant(7 : i32) : i32
    %12 = llvm.mlir.constant(10 : i32) : i32
    %13 = llvm.mlir.constant(11 : i32) : i32
    %14 = llvm.mlir.constant(0 : index) : i64
    %15 = llvm.mlir.constant(1 : index) : i64
    %16 = llvm.mlir.constant(2 : index) : i64
    %17 = llvm.mlir.constant(3 : index) : i64
    %18 = llvm.mlir.constant(4 : index) : i64
    %19 = llvm.mlir.constant(5 : index) : i64
    %20 = llvm.mlir.constant(6 : index) : i64
    %21 = llvm.getelementptr %2[12] : (!llvm.ptr) -&gt; !llvm.ptr, i32
    %22 = llvm.ptrtoint %21 : !llvm.ptr to i64
    %23 = llvm.call @<!-- -->malloc(%22) : (i64) -&gt; !llvm.ptr
    %24 = llvm.insertvalue %23, %1[0] : !llvm.struct&lt;(ptr, ptr, i64, array&lt;2 x i64&gt;, array&lt;2 x i64&gt;)&gt; 
    %25 = llvm.insertvalue %23, %24[1] : !llvm.struct&lt;(ptr, ptr, i64, array&lt;2 x i64&gt;, array&lt;2 x i64&gt;)&gt; 
    %26 = llvm.insertvalue %14, %25[2] : !llvm.struct&lt;(ptr, ptr, i64, array&lt;2 x i64&gt;, array&lt;2 x i64&gt;)&gt; 
    %27 = llvm.insertvalue %16, %26[3, 0] : !llvm.struct&lt;(ptr, ptr, i64, array&lt;2 x i64&gt;, array&lt;2 x i64&gt;)&gt; 
    %28 = llvm.insertvalue %20, %27[3, 1] : !llvm.struct&lt;(ptr, ptr, i64, array&lt;2 x i64&gt;, array&lt;2 x i64&gt;)&gt; 
    %29 = llvm.insertvalue %20, %28[4, 0] : !llvm.struct&lt;(ptr, ptr, i64, array&lt;2 x i64&gt;, array&lt;2 x i64&gt;)&gt; 
    %30 = llvm.insertvalue %15, %29[4, 1] : !llvm.struct&lt;(ptr, ptr, i64, array&lt;2 x i64&gt;, array&lt;2 x i64&gt;)&gt; 
    %31 = llvm.getelementptr %2[2] : (!llvm.ptr) -&gt; !llvm.ptr, i32
    %32 = llvm.ptrtoint %31 : !llvm.ptr to i64
    %33 = llvm.call @<!-- -->malloc(%32) : (i64) -&gt; !llvm.ptr
    %34 = llvm.insertvalue %33, %0[0] : !llvm.struct&lt;(ptr, ptr, i64, array&lt;1 x i64&gt;, array&lt;1 x i64&gt;)&gt; 
    %35 = llvm.insertvalue %33, %34[1] : !llvm.struct&lt;(ptr, ptr, i64, array&lt;1 x i64&gt;, array&lt;1 x i64&gt;)&gt; 
    %36 = llvm.insertvalue %14, %35[2] : !llvm.struct&lt;(ptr, ptr, i64, array&lt;1 x i64&gt;, array&lt;1 x i64&gt;)&gt; 
    %37 = llvm.insertvalue %16, %36[3, 0] : !llvm.struct&lt;(ptr, ptr, i64, array&lt;1 x i64&gt;, array&lt;1 x i64&gt;)&gt; 
    %38 = llvm.insertvalue %15, %37[4, 0] : !llvm.struct&lt;(ptr, ptr, i64, array&lt;1 x i64&gt;, array&lt;1 x i64&gt;)&gt; 
    %39 = llvm.alloca %15 x !llvm.struct&lt;(ptr, ptr, i64, array&lt;2 x i64&gt;, array&lt;2 x i64&gt;)&gt; : (i64) -&gt; !llvm.ptr
    llvm.store %30, %39 : !llvm.struct&lt;(ptr, ptr, i64, array&lt;2 x i64&gt;, array&lt;2 x i64&gt;)&gt;, !llvm.ptr
    %40 = llvm.getelementptr %2[1] : (!llvm.ptr) -&gt; !llvm.ptr, i32
    %41 = llvm.ptrtoint %40 : !llvm.ptr to i64
    llvm.call @<!-- -->mgpuMemHostRegisterMemRef(%16, %39, %41) : (i64, !llvm.ptr, i64) -&gt; ()
    %42 = llvm.alloca %15 x !llvm.struct&lt;(ptr, ptr, i64, array&lt;1 x i64&gt;, array&lt;1 x i64&gt;)&gt; : (i64) -&gt; !llvm.ptr
    llvm.store %38, %42 : !llvm.struct&lt;(ptr, ptr, i64, array&lt;1 x i64&gt;, array&lt;1 x i64&gt;)&gt;, !llvm.ptr
    llvm.call @<!-- -->mgpuMemHostRegisterMemRef(%15, %42, %41) : (i64, !llvm.ptr, i64) -&gt; ()
    %43 = llvm.mul %14, %20 overflow&lt;nsw, nuw&gt; : i64
    %44 = llvm.add %43, %14 overflow&lt;nsw, nuw&gt; : i64
    %45 = llvm.getelementptr inbounds|nuw %23[%44] : (!llvm.ptr, i64) -&gt; !llvm.ptr, i32
    llvm.store %3, %45 : i32, !llvm.ptr
    %46 = llvm.add %43, %15 overflow&lt;nsw, nuw&gt; : i64
    %47 = llvm.getelementptr inbounds|nuw %23[%46] : (!llvm.ptr, i64) -&gt; !llvm.ptr, i32
    llvm.store %4, %47 : i32, !llvm.ptr
    %48 = llvm.add %43, %16 overflow&lt;nsw, nuw&gt; : i64
    %49 = llvm.getelementptr inbounds|nuw %23[%48] : (!llvm.ptr, i64) -&gt; !llvm.ptr, i32
    llvm.store %5, %49 : i32, !llvm.ptr
    %50 = llvm.add %43, %17 overflow&lt;nsw, nuw&gt; : i64
    %51 = llvm.getelementptr inbounds|nuw %23[%50] : (!llvm.ptr, i64) -&gt; !llvm.ptr, i32
    llvm.store %6, %51 : i32, !llvm.ptr
    %52 = llvm.add %43, %18 overflow&lt;nsw, nuw&gt; : i64
    %53 = llvm.getelementptr inbounds|nuw %23[%52] : (!llvm.ptr, i64) -&gt; !llvm.ptr, i32
    llvm.store %7, %53 : i32, !llvm.ptr
    %54 = llvm.add %43, %19 overflow&lt;nsw, nuw&gt; : i64
    %55 = llvm.getelementptr inbounds|nuw %23[%54] : (!llvm.ptr, i64) -&gt; !llvm.ptr, i32
    llvm.store %8, %55 : i32, !llvm.ptr
    %56 = llvm.mul %15, %20 overflow&lt;nsw, nuw&gt; : i64
    %57 = llvm.add %56, %14 overflow&lt;nsw, nuw&gt; : i64
    %58 = llvm.getelementptr inbounds|nuw %23[%57] : (!llvm.ptr, i64) -&gt; !llvm.ptr, i32
    llvm.store %5, %58 : i32, !llvm.ptr
    %59 = llvm.add %56, %15 overflow&lt;nsw, nuw&gt; : i64
    %60 = llvm.getelementptr inbounds|nuw %23[%59] : (!llvm.ptr, i64) -&gt; !llvm.ptr, i32
    llvm.store %9, %60 : i32, !llvm.ptr
    %61 = llvm.add %56, %16 overflow&lt;nsw, nuw&gt; : i64
    %62 = llvm.getelementptr inbounds|nuw %23[%61] : (!llvm.ptr, i64) -&gt; !llvm.ptr, i32
    llvm.store %10, %62 : i32, !llvm.ptr
    %63 = llvm.add %56, %17 overflow&lt;nsw, nuw&gt; : i64
    %64 = llvm.getelementptr inbounds|nuw %23[%63] : (!llvm.ptr, i64) -&gt; !llvm.ptr, i32
    llvm.store %11, %64 : i32, !llvm.ptr
    %65 = llvm.add %56, %18 overflow&lt;nsw, nuw&gt; : i64
    %66 = llvm.getelementptr inbounds|nuw %23[%65] : (!llvm.ptr, i64) -&gt; !llvm.ptr, i32
    llvm.store %12, %66 : i32, !llvm.ptr
    %67 = llvm.add %56, %19 overflow&lt;nsw, nuw&gt; : i64
    %68 = llvm.getelementptr inbounds|nuw %23[%67] : (!llvm.ptr, i64) -&gt; !llvm.ptr, i32
    llvm.store %13, %68 : i32, !llvm.ptr
    gpu.launch_func  @<!-- -->main_kernel::@<!-- -->main_kernel blocks in (%16, %15, %15) threads in (%20, %15, %15) : i64 args(%23 : !llvm.ptr, %23 : !llvm.ptr, %14 : i64, %16 : i64, %20 : i64, %20 : i64, %15 : i64, %33 : !llvm.ptr, %33 : !llvm.ptr, %14 : i64, %16 : i64, %15 : i64)
    llvm.call @<!-- -->printMemrefI32(%15, %42) : (i64, !llvm.ptr) -&gt; ()
    llvm.return
  }
  gpu.binary @<!-- -->main_kernel  [#gpu.object&lt;#nvvm.target, properties = {ISAToBinaryTimeInMs = 16 : i64, LLVMIRToISATimeInMs = 30 : i64}, "P\EDU\BA\01\00\10\00\90\11\00\00\00\00\00\00\02\00\01\01@\00\00\00\C0\0C\00\00\00\00\00\00\00\00\00\00\00\00\00\00\07\00\01\002\00\00\00\00\00\00\00\00\00\00\00\11\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\7FELF\02\01\013\07\00\00\00\00\00\00\00\02\00\BE\00\80\00\00\00\00\00\00\00\00\00\00\00\E0\0B\00\00\00\00\00\00 \09\00\00\00\00\00\002\052\00@\008\00\04\00@\00\0B\00\01\00\00.shstrtab\00.strtab\00.symtab\00.symtab_shndx\00.nv.info\00.text.main_kernel\00.nv.info.main_kernel\00.nv.shared.main_kernel\00.nv.constant0.main_kernel\00.rel.nv.constant0.main_kernel\00.nv.callgraph\00.nv.prototype\00.nv.rel.action\00\00.shstrtab\00.strtab\00.symtab\00.symtab_shndx\00.nv.info\00.text.main_kernel\00.nv.info.main_kernel\00.nv.shared.main_kernel\00.rel.nv.constant0.main_kernel\00.nv.constant0.main_kernel\00.nv.callgraph\00.nv.prototype\00.nv.rel.action\00main_kernel\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\002\00\00\00\03\00\09\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00Y\00\00\00\03\00\0A\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\8E\00\00\00\03\00\08\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\A8\00\00\00\03\00\06\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\C4\00\00\00\03\00\07\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\D3\00\00\00\12\10\09\00\00\00\00\00\00\00\00\00\00\03\00\00\00\00\00\00\04/\08\00\06\00\00\00\09\00\00\00\04\11\08\00\06\00\00\00\00\00\00\00\04\12\08\00\06\00\00\00\00\00\00\00\047\04\00\80\00\00\00\010\00\00\01*\00\00\04\0A\08\00\03\00\00\00@\01`\00\03\19`\00\04\17\0C\00\00\00\00\00\0B\00X\00\00\F0!\00\04\17\0C\00\00\00\00\00\0A\00P\00\00\F0!\00\04\17\0C\00\00\00\00\00\09\00H\00\00\F0!\00\04\17\0C\00\00\00\00\00\08\00@\00\00\F5!\00\04\17\0C\00\00\00\00\00\07\008\00\00\F5!\00\04\17\0C\00\00\00\00\00\06\000\00\00\F0!\00\04\17\0C\00\00\00\00\00\05\00(\00\00\F0!\00\04\17\0C\00\00\00\00\00\04\00 \00\00\F0!\00\04\17\0C\00\00\00\00\00\03\00\18\00\00\F0!\00\04\17\0C\00\00\00\00\00\02\00\10\00\00\F0!\00\04\17\0C\00\00\00\00\00\01\00\08\00\00\F5!\00\04\17\0C\00\00\00\00\00\00\00\00\00\00\F5!\00\03\1B\FF\00\04)(\00\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\FF\04((\00\E8\00\00\00\08\01\00\00(\01\00\00H\01\00\00p\01\00\00\C8\01\00\00\E8\01\00\00\08\02\00\00(\02\00\00H\02\00\00\04\1D\04\00\18\00\00\00\04\1C\04\00\F0\02\00\00\044 \00\B0\01\00\00\00\00\00\00\01\00\00\00x\02\00\00p\02\00\00\00\00\00\00\01\00\00\00x\02\00\00\04\05\0C\00\06\00\00\00\01\00\00\00\01\00\00\00\04\1E\04\00 \00\00\00\00\00\00\00\FF\FF\FF\FF\00\00\00\00\FE\FF\FF\FF\00\00\00\00\FD\FF\FF\FF\00\00\00\00\FC\FF\FF\FF\00\00\00\00s\00\00\00\00\00\00\00\00\00\00\11%\00\056\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\F6\07\00\FE\00D\1C\00\01\00\87\00\80\07\98L\00\F0\87\01\00\00\00\01\07\00W\02\00\00\C8\F0?\07\00\FE\C1\C4\1F\00\08\00\17\02\00\00\C8\F0\05\07\07\00\80\7F\00[\00\00\80#\00\00\90\E2\F1\07&amp;\FE\C0\CC\1F\18\02\07\07\00\80\7F\00[\04\07\07\00\A8\7F\00[\03\07\07\00\88\7F\00[\E1\07 \FE@\D4\1F\00\05\07\07\00\80\02([\00\07\07\00\88\7F [\02\07G\00\18\010[\F1\07 \FE\00\D4_\00\03\057\00 \00\C0\\\02\02'\05\00\80\10L\00\08\E7\01\00\00(8\E1\07\C0\FE\00\C8\1F\00\03\037\05\00\08\10L\02\08'\00\00\81\18\\\03\007\00\00\08\10\\\B1\07\C0\FE\00\D0\1F\00\02\02\07\00\00 \D4\EE\00\F0\17\00\00\00\00\01\00\00'\00\00\00\12L\1F\07\C1\FC\01\D0\1F\00\04\02\17\D0\00\00\10\EF\04\04\F7\FF\7F\00\A09\05\04'\00\00\00G\\\1F\07\C0\FE\01\D0\1F\00\04\05'\D0\00\00\10\EF\04\04\F7\FF\7F\00\A09\06\05G\00\00\00G\\\1F\07\C0\FE\01\D0\1F\00\04\06G\D0\00\00\10\EF\04\04\F7\FF\7F\00\A09\03\06G\00\00\00G\\\1F\07 \FC\01\D4\1F\00\04\03\87\D0\00\00\10\EF\04\04\F7\FF\7F\00\A09\07\08\F7\0F\80\03j[\F4\07\E0\E3\00\D8?\00\02\03G\00\00\00G\\\04\02\07\D1\00\00\11\EF\04\04\F8\FF\FF\00\A09\F4\07 \FE\00\FC\1F\00\04\02H\00\00\00G\\\04\FF\08\00\00\00\\\EF\00\00\07\00\80\1B\A8\F0\F5\07\A0\FF\00|\1C\00\00\00\07\00\00\00\98\EF\0F\00\00\00\00\00\F8\F0\00\FF\07\00\00\10L\EF\1F\0F\C0\FE\01\D0\1F\00\02\00\17\D0\80\7F\10\EF\02\02\F7\FF\7F\00\A09\02\02\07\00\00\00G\\\1F\07\C0\FE\01\D0\1F\00\03\02'\D0\80\7F\10\EF\03\03\F7\FF\7F\00\A09\03\027\00\00\00G\\\1F\07\C0\FE\01\90\1F\00\04\03G\D0\80\7F\10\EF\04\04\F7\FF\7F\00\A09\04\03G\00\00\00G\\\1F\07\C0\FE\01\D0\1F\00\05\04\87\D0\80\7F\10\EF\05\05\F7\FF\7F\00\A09\05\04W\00\00\00G\\\1F\07\C0\FE\01\D0\1F\00\00\05\07\D1\80\7F\10\EF\00\00\F7\FF\7F\00\A09\00\05\07\00\00\00G\\\F1\00\A0\FF\00\BC\1F\00\00\FF\07\00\00\00\\\EF\0F\00\07\00\00\00\F8\F0\00\0F\07\00\00\00\B0P\EF\07\00\FD\00\FC\1F\00\00\0F\07\00\00\00\B0P\00\0F\07\00\00\00\B0P\00\00\07\00\80\1B\A8\F0\F5\07 \E2\01\B4\1F\00\00\00\07\00\00\00\98\EF\00\FF\07\00\00\10L\EF\02\07\07\06\00\81\D7K\FD\07 \FE\01\BC\1F\00\03\07\17\06\80\7F\17\1A\00\02\07\00\00 \DC\EE\00\0F\07\00\00\00\B0P\E8\07\E0\FF\00\FC\1F\00\00\0F\07\00\00\00\B0P\0F\00\07\00\00\00\00\E3\0F\00\87\FF\FF\0F@\E2\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\01\00\00\00\03\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00@\00\00\00\00\00\00\00\D3\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\01\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\0B\00\00\00\03\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\13\01\00\00\00\00\00\00\DF\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\01\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\13\00\00\00\02\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\F8\01\00\00\00\00\00\00\A8\00\00\00\00\00\00\00\02\00\00\00\06\00\00\00\08\00\00\00\00\00\00\00\18\00\00\00\00\00\00\00)\00\00\00\00\00\00p\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\A0\02\00\00\00\00\00\00$\00\00\00\00\00\00\00\03\00\00\00\00\00\00\00\04\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00D\00\00\00\00\00\00p@\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\C4\02\00\00\00\00\00\00\88\01\00\00\00\00\00\00\03\00\00\00\09\00\00\00\04\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\A8\00\00\00\01\00\00p\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00L\04\00\00\00\00\00\00 \00\00\00\00\00\00\00\03\00\00\00\00\00\00\00\04\00\00\00\00\00\00\00\08\00\00\00\00\00\00\00\C4\00\00\00\0B\00\00p\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00p\04\00\00\00\00\00\00\10\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\08\00\00\00\00\00\00\00\08\00\00\00\00\00\00\00p\00\00\00\01\00\00\00B\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\80\04\00\00\00\00\00\00\A0\01\00\00\00\00\00\00\00\00\00\00\09\00\00\00\04\00\00\00\00\00\00\00\00\00\00\00\00\00\00\002\00\00\00\01\00\00\00\06\00\10\00\00\00\00\00\00\00\00\00\00\00\00\00 \06\00\00\00\00\00\00\00\03\00\00\00\00\00\00\03\00\00\00\06\00\00\09 \00\00\00\00\00\00\00\00\00\00\00\00\00\00\00Y\00\00\00\08\00\00\00C\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00 \09\00\00\00\00\00\00\04\00\00\00\00\00\00\00\00\00\00\00\09\00\00\00\04\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\06\00\00\00\05\00\00\00\E0\0B\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\E0\00\00\00\00\00\00\00\E0\00\00\00\00\00\00\00\08\00\00\00\00\00\00\00\01\00\00\00\05\00\00\00\80\04\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\A0\04\00\00\00\00\00\00\A0\04\00\00\00\00\00\00\08\00\00\00\00\00\00\00\01\00\00\00\06\00\00\00 \09\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\04\00\00\00\00\00\00\00\08\00\00\00\00\00\00\00\01\00\00\00\05\00\00\00\E0\0B\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\E0\00\00\00\00\00\00\00\E0\00\00\00\00\00\00\00\08\00\00\00\00\00\00\00\01\00\01\01P\00\00\00@\04\00\00\00\00\00\00@\04\00\00@\00\00\00\00\00\06\002\00\00\00\00\00\00\00\00\00\00\00\11 \00\00\00\00\00\00\00\00\00\00\00\00\00\00\E0\09\00\00\00\00\00\00H\00\00\00\00\00\00\00\00\00\00\00\00\00\00\000\0A//\03\00\F6\1E\0A.version 6.0\0A.target sm_50\0A.address_size 64\0A4\00\F8%isible .entry main_kernel(\0A.param .u64 .ptr .align 1'\00\11_%\00?_0,/\00\1A\1A1/\00\0E!\00\1F2!\00\0D\1F3!\00\0D\1F4!\00\0D\1F5!\00\0D\1F6\D4\00\1B\1F7/\00\1B\1F8\7F\00\0D\1F9!\00\0D/10\22\00\0E\F3\1A1\0A)\0A.maxntid 6, 1, 1\0A{\0A.reg .pred %p&lt;12&gt;;\13\00\96b32 %r&lt;41\12\00b64 %rd%\00p//\0A.sha8\00\03\DA\00\C84 .u32 __wg_\80\00\920_$_0;\0Ald\A1\00\04G\00O3, [\A7\00\00\F2\04];\0Acvta.to.global.u0\00!4,6\00\0EI\00\1F5I\00\02\1F8I\00\06\111I\00a5;\0Amov\B2\00\D8%r3, %ctaid.x\17\00\00*\00\12t\15\00rsetp.ne\16\012p1,\1E\00\180.\00\00\A1\00\13n/\00\83mul.wide\1B\002d6,f\00\9224;\0Aadd.s\8B\00&amp;7,\DA\00\1E66\00\138g\00\0A5\00&amp;9,;\00\118\0F\01\03\E0\00\02\B2\01\00\0F\01\00&amp;\00\81];\0Asub.s\15\00P6, 322\00\124\B2\00\02+\00\A37, -1;\0Ashrr\00\228,\16\003%r6s\00\01\17\00\01r\00\144-\00\C3fl.sync.bflyI\00r10|%p2,~\00$1,3\00\1088\01$lp%\00\026\01\100H\00\A4, %p2;\0Aand\1D\00%2,#\00\1F5f\00\05c3|%p3,.\00\1F2g\00\05\01O\01$13g\00\193g\00&amp;5,m\00\1F4h\00\05c6|%p4,/\00\1F4h\00\05\01(\01$16h\00\194h\00&amp;8,n\00\1F7h\00\05c9|%p5,/\00\1F8h\00\04120,,\00\04h\00\185h\00$21?\00\00)\00\0Fh\00\04s22|%p6,/\00(16i\00\FA\03@%p1 bra $L__BB0_2}\00\01o\01$22}\00\196}\00\15,\82\00c23;\0Ast\11\04\01\1F\00\1F[\08\04\03\11].\00%;\0Ao\00\10:\A6\03\00\9B\03\13.\F5\02\01V\00a3;\0Abar\CA\00/ 0\A6\00\00\114\F8\02\07o\00_%r24,u\00\06\0F\19\01\05c5|%p7,=\00\00\D0\03:, 1\00\01\01-\01$25\00\01\197\00\01&amp;7,#\00\0FM\02\05s28|%p8,/\00\1F2d\00\01\01\8C\01$28d\00\188d\00630,j\00\1F9d\00\04s31|%p9,/\00\1F4d\00\00\02\FD\03$31d\00\199d\00%3,j\00/32d\00\05@<!-- -->4|%p\BD\03\020\00\1F8e\00\01\01\CC\02$34e\00)10f\00&amp;6,l\00\0F\E3\03\05 37f\00#<!-- -->1,0\00/16g\00\01\01N\01%37g\00\191g\00&amp;9,m\00\1F8\96\02\19&amp;39\97\02)4:\80\02\0Cl\02/40l\02\0B\14l\9C\06\021\01\00\D4\02\192\C4\05\02\E3\00\03\8B\06!10\A7\00\07\C6\05\11[#\00\01\96\00\D040;\0Aret;\0A\0A}\0A\00"&gt;]
  llvm.func @<!-- -->printMemrefI32(i64, !llvm.ptr) attributes {sym_visibility = "private"}
  llvm.func @<!-- -->mgpuMemHostRegisterMemRef(i64, !llvm.ptr, i64)
}
```

### Crash Log
```bash
Failed to materialize symbols: { (main, { $.LLVMDialectModule.__inits.0, main, __orc_deinit_func.LLVMDialectModule, _mlir_main_kernel_unload, _mlir_main_kernel_load, _mlir_main, __orc_init_func.LLVMDialectModule }) }
UNREACHABLE executed at /llvm-project/llvm/include/llvm/Support/Error.h:779!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-runner lowered.mlir -e main -entry-point-result=void -shared-libs=/llvm-project/build_all/lib/libmlir_c_runner_utils.so,/llvm-project/build_all/lib/libmlir_runner_utils.so
 #<!-- -->0 0x000055c1438272cd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x000055c1438277fb PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x000055c1438256cf llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x000055c143827e99 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f5fcf245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007f5fcf29eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f5fcf29eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007f5fcf29eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007f5fcf24527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007f5fcf2288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000055c143769c88 /llvm-project/llvm/lib/Support/ErrorHandling.cpp:244:3
#<!-- -->11 0x000055c1433e8601 llvm::cantFail(llvm::Error, char const*) /llvm-project/llvm/include/llvm/Support/Error.h:781:1
#<!-- -->12 0x000055c144348543 mlir::ExecutionEngine::initialize() /llvm-project/mlir/lib/ExecutionEngine/ExecutionEngine.cpp:457:5
#<!-- -->13 0x000055c1442f659f compileAndExecute((anonymous namespace)::Options&amp;, mlir::Operation*, llvm::StringRef, (anonymous namespace)::CompileAndExecuteConfig, void**, std::unique_ptr&lt;llvm::TargetMachine, std::default_delete&lt;llvm::TargetMachine&gt;&gt;) /llvm-project/mlir/lib/ExecutionEngine/JitRunner.cpp:207:23
#<!-- -->14 0x000055c1442f611f compileAndExecuteVoidFunction((anonymous namespace)::Options&amp;, mlir::Operation*, llvm::StringRef, (anonymous namespace)::CompileAndExecuteConfig, std::unique_ptr&lt;llvm::TargetMachine, std::default_delete&lt;llvm::TargetMachine&gt;&gt;) /llvm-project/mlir/lib/ExecutionEngine/JitRunner.cpp:241:3
#<!-- -->15 0x000055c1442f4917 mlir::JitRunnerMain(int, char**, mlir::DialectRegistry const&amp;, mlir::JitRunnerConfig) /llvm-project/mlir/lib/ExecutionEngine/JitRunner.cpp:398:17
#<!-- -->16 0x000055c1433bd0e3 main /llvm-project/mlir/tools/mlir-runner/mlir-runner.cpp:93:3
#<!-- -->17 0x00007f5fcf22a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->18 0x00007f5fcf22a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->19 0x00007f5fcf22a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->20 0x000055c1433bcf15 _start (/mnt/raid/llvm-project/build_all/bin/mlir-runner+0x424f15)
Aborted
```

I used the command to run mlir-runner: `mlir-runner lowered.mlir -e main -entry-point-result=void -shared-libs=/llvm-project/build_all/lib/libmlir_c_runner_utils.so,/llvm-project/build_all/lib/libmlir_runner_utils.so`.

To reproduce, the original MLIR code is:
```mlir
func.func @<!-- -->main() {
  %data = memref.alloc() : memref&lt;2x6xi32&gt;
  %sum = memref.alloc() : memref&lt;2xi32&gt;
  %cst0 = arith.constant 0 : i32
  %cst1 = arith.constant 1 : i32
  %cst2 = arith.constant 2 : i32
  %cst4 = arith.constant 4 : i32
  %cst8 = arith.constant 8 : i32
  %cst16 = arith.constant 16 : i32

  %cst3 = arith.constant 3 : i32
  %cst6 = arith.constant 6 : i32
  %cst7 = arith.constant 7 : i32
  %cst10 = arith.constant 10 : i32
  %cst11 = arith.constant 11 : i32

  %c0 = arith.constant 0 : index
  %c1 = arith.constant 1 : index
  %c2 = arith.constant 2 : index
  %c3 = arith.constant 3 : index
  %c4 = arith.constant 4 : index
  %c5 = arith.constant 5 : index
  %c6 = arith.constant 6 : index

  %cast_data = memref.cast %data : memref&lt;2x6xi32&gt; to memref&lt;*xi32&gt;
  gpu.host_register %cast_data : memref&lt;*xi32&gt;
  %cast_sum = memref.cast %sum : memref&lt;2xi32&gt; to memref&lt;*xi32&gt;
  gpu.host_register %cast_sum : memref&lt;*xi32&gt;

  memref.store %cst0, %data[%c0, %c0] : memref&lt;2x6xi32&gt;
  memref.store %cst1, %data[%c0, %c1] : memref&lt;2x6xi32&gt;
  memref.store %cst2, %data[%c0, %c2] : memref&lt;2x6xi32&gt;
  memref.store %cst4, %data[%c0, %c3] : memref&lt;2x6xi32&gt;
  memref.store %cst8, %data[%c0, %c4] : memref&lt;2x6xi32&gt;
  memref.store %cst16, %data[%c0, %c5] : memref&lt;2x6xi32&gt;

  memref.store %cst2, %data[%c1, %c0] : memref&lt;2x6xi32&gt;
  memref.store %cst3, %data[%c1, %c1] : memref&lt;2x6xi32&gt;
  memref.store %cst6, %data[%c1, %c2] : memref&lt;2x6xi32&gt;
  memref.store %cst7, %data[%c1, %c3] : memref&lt;2x6xi32&gt;
  memref.store %cst10, %data[%c1, %c4] : memref&lt;2x6xi32&gt;
  memref.store %cst11, %data[%c1, %c5] : memref&lt;2x6xi32&gt;

  gpu.launch blocks(%bx, %by, %bz) in (%grid_x = %c2, %grid_y = %c1, %grid_z = %c1)
             threads(%tx, %ty, %tz) in (%block_x = %c6, %block_y = %c1, %block_z = %c1) {
    %val = memref.load %data[%bx, %tx] : memref&lt;2x6xi32&gt;
    %reduced = gpu.all_reduce and %val uniform {} : (i32) -&gt; (i32)
    memref.store %reduced, %sum[%bx] : memref&lt;2xi32&gt;
    gpu.terminator
  }

  call @<!-- -->printMemrefI32(%cast_sum) : (memref&lt;*xi32&gt;) -&gt; ()

  return
}

func.func private @<!-- -->printMemrefI32(memref&lt;*xi32&gt;)
```

The lowering command is: `mlir-opt -gpu-lower-to-nvvm-pipeline`
</details>


---

### Comment 2 - mugiwaraluffy56

Hey @LynnBroe , this isn't a bug , you're missing the CUDA runtime library in your mlir-runner command.

The lowered code contains calls to `mgpuMemHostRegisterMemRef` and other GPU runtime functions. These are provided by `libmlir_cuda_runtime.so`, which you need to load via the `--shared-libs` flag.

Update your command to include the CUDA runtime library:

```bash
mlir-runner lowered.mlir -e main -entry-point-result=void \
  -shared-libs=/llvm-project/build_all/lib/libmlir_cuda_runtime.so,/llvm-project/build_all/lib/libmlir_c_runner_utils.so,/llvm-project/build_all/lib/libmlir_runner_utils.so
```

If you don't have that library, build MLIR with CUDA support:

```bash
cd /llvm-project/build_all
cmake -DMLIR_ENABLE_CUDA_RUNNER=ON ..
cmake --build . --target mlir_cuda_runtime
```
Check out the integration tests in ```mlir/test/Integration/GPU/CUDA/ ``` for examples of the proper usage pattern.




---

### Comment 3 - LynnBroe

Hi @mugiwaraluffy56 , I understand what you mean. To clarify, I don’t think a crash should occur here. Under certain conditions, a crash could lead to unsafe situations. We should let MLIR terminate gracefully and exit.

---

### Comment 4 - joker-eph

We shouldn't use `cantFail` here:

```c++
void ExecutionEngine::initialize() {
  if (isInitialized)
    return;
  cantFail(jit->initialize(jit->getMainJITDylib()));
  isInitialized = true;
}
```


because it will only abort in assert builds, we should check for the error and kill the process (or propagate the error).

---

### Comment 5 - Men-cotton

I believe https://github.com/llvm/llvm-project/pull/171115 resolved this crash.

---

