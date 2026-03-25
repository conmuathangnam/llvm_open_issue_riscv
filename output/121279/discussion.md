# MemorySSAUpdater segfaults in deep recursion

**Author:** JonPsson1
**URL:** https://github.com/llvm/llvm-project/issues/121279

## Body

clang -cc1 -triple s390x-unknown-linux-gnu -target-cpu z15 -O3 -w -vectorize-loops -vectorize-slp -mllvm -unroll-allow-remainder=false -mllvm -unroll-count=4 -o /dev/null -emit-llvm -x ir crash1_aftercreduce.bugpoint-reduced-simplified.bc

[tc_memssa.tar.gz](https://github.com/user-attachments/files/18268197/tc_memssa.tar.gz)

```
...
#253 0x000002aa3fa2ac9e llvm::MemorySSAUpdater::getPreviousDefFromEnd(llvm::BasicBlock*, llvm::DenseMap<llvm::BasicBlock*, llvm::TrackingVH<llvm::MemoryAccess>, llvm::DenseMapInfo<llvm::BasicBlock*, void>, llvm::detail::DenseMapPair<llvm::BasicBlock*, llvm::TrackingVH<llvm::MemoryAccess>>>&) (/home/ijonpan/llvm-project/install/bin/clang-20+0x102ac9e)
#254 0x000002aa3fa29552 llvm::MemorySSAUpdater::getPreviousDefRecursive(llvm::BasicBlock*, llvm::DenseMap<llvm::BasicBlock*, llvm::TrackingVH<llvm::MemoryAccess>, llvm::DenseMapInfo<llvm::BasicBlock*, void>, llvm::detail::DenseMapPair<llvm::BasicBlock*, llvm::TrackingVH<llvm::MemoryAccess>>>&) (/home/ijonpan/llvm-project/install/bin/clang-20+0x1029552)
#255 0x000002aa3fa2ac9e llvm::MemorySSAUpdater::getPreviousDefFromEnd(llvm::BasicBlock*, llvm::DenseMap<llvm::BasicBlock*, llvm::TrackingVH<llvm::MemoryAccess>, llvm::DenseMapInfo<llvm::BasicBlock*, void>, llvm::detail::DenseMapPair<llvm::BasicBlock*, llvm::TrackingVH<llvm::MemoryAccess>>>&) (/home/ijonpan/llvm-project/install/bin/clang-20+0x102ac9e)
Segmentation fault (core dumped)
```
@nikic @DianQK @whitneywhtsang 

