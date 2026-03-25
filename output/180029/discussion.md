# [MLIR][AFFINE] mlir-opt affine-loop-fusion `sliceIterationCount > 0' failed

**Author:** naskap
**URL:** https://github.com/llvm/llvm-project/issues/180029
**Status:** Closed
**Labels:** mlir:affine, crash
**Closed Date:** 2026-02-24T02:18:39Z

## Body

LLVM version 21.1.8
Hash: 2078da43e25a4623cab

`getFusionComputeCost` fails with assertion failure: `sliceIterationCount > 0 failed`

Command: `mlir-opt --affine-loop-fusion testfile.mlir`

Test File:
```mlir
module {

  func.func @foo(%arg0: memref<1024x4096xf32>, %arg1: memref<8192x4096xf32>) -> memref<1024x8192xf32> {
    %cst_0 = arith.constant 1.000000e+00 : f32
    %alloc = memref.alloc() : memref<1024x8192xf32>
    affine.for %arg2 = 0 to 16 {
        affine.for %arg4 = 0 to 512 {
          affine.for %arg5 = 0 to 64 {
            affine.for %arg6 = 0 to 16 {
              affine.for %arg7 = 0 to 2048 {
                %0 = affine.load %arg0[%arg5 + %arg2 * 64, %arg7] : memref<1024x4096xf32>
                %1 = affine.load %arg1[%arg6 + %arg4 * 16, %arg7] : memref<8192x4096xf32>
                %2 = affine.load %alloc[%arg5 + %arg2 * 64, %arg6 + %arg4 * 16] : memref<1024x8192xf32>
                %3 = arith.mulf %0, %1 : f32
                %4 = arith.addf %2, %3 : f32
                affine.store %4, %alloc[%arg5 + %arg2 * 64, %arg6 + %arg4 * 16] : memref<1024x8192xf32>
              }
            }
          }
        }

        affine.for %arg4 = 0 to 512 {
          affine.for %arg5 = 0 to 64 {
            affine.for %arg6 = 0 to 16 {
              affine.for %arg7 = 0 to 2048 {
                %0 = affine.load %arg0[%arg5 + %arg2 * 64, %arg7 + 2048] : memref<1024x4096xf32>
                %1 = affine.load %arg1[%arg6 + %arg4 * 16, %arg7 + 2048] : memref<8192x4096xf32>
                %2 = affine.load %alloc[%arg5 + %arg2 * 64, %arg6 + %arg4 * 16] : memref<1024x8192xf32>
                %3 = arith.mulf %0, %1 : f32
                %4 = arith.addf %2, %3 : f32
                affine.store %4, %alloc[%arg5 + %arg2 * 64, %arg6 + %arg4 * 16] : memref<1024x8192xf32>
              }
            }
          }
        }
      
    }
    affine.for %arg2 = 0 to 16 {
      affine.for %arg3 = 0 to 512 {
        affine.for %arg4 = 0 to 64 {
          affine.for %arg5 = 0 to 16 {
            %0 = affine.load %alloc[%arg4 + %arg2 * 64, %arg5 + %arg3 * 16] : memref<1024x8192xf32>
            affine.store %0, %alloc[%arg4 + %arg2 * 64, %arg5 + %arg3 * 16] : memref<1024x8192xf32>
          }
        }
      }
    }
    return %alloc : memref<1024x8192xf32>
  }
}
```
I found it was hard to minimize this example much more and keep the failure. 

[stack_dump.txt](https://github.com/user-attachments/files/25111339/stack_dump.txt)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Nathan Kaplan (naskap)

<details>
LLVM version 21.1.8
Hash: 2078da43e25a4623cab

getFusionComputeCost fails with assertion failure: sliceIterationCount &gt; 0 failed

Command: `mlir-opt --affine-loop-fusion testfile.mlir`

Test File:
```
module {

  func.func @<!-- -->foo(%arg0: memref&lt;1024x4096xf32&gt;, %arg1: memref&lt;8192x4096xf32&gt;) -&gt; memref&lt;1024x8192xf32&gt; {
    %cst_0 = arith.constant 1.000000e+00 : f32
    %alloc = memref.alloc() : memref&lt;1024x8192xf32&gt;
    affine.for %arg2 = 0 to 16 {
        affine.for %arg4 = 0 to 512 {
          affine.for %arg5 = 0 to 64 {
            affine.for %arg6 = 0 to 16 {
              affine.for %arg7 = 0 to 2048 {
                %0 = affine.load %arg0[%arg5 + %arg2 * 64, %arg7] : memref&lt;1024x4096xf32&gt;
                %1 = affine.load %arg1[%arg6 + %arg4 * 16, %arg7] : memref&lt;8192x4096xf32&gt;
                %2 = affine.load %alloc[%arg5 + %arg2 * 64, %arg6 + %arg4 * 16] : memref&lt;1024x8192xf32&gt;
                %3 = arith.mulf %0, %1 : f32
                %4 = arith.addf %2, %3 : f32
                affine.store %4, %alloc[%arg5 + %arg2 * 64, %arg6 + %arg4 * 16] : memref&lt;1024x8192xf32&gt;
              }
            }
          }
        }

        affine.for %arg4 = 0 to 512 {
          affine.for %arg5 = 0 to 64 {
            affine.for %arg6 = 0 to 16 {
              affine.for %arg7 = 0 to 2048 {
                %0 = affine.load %arg0[%arg5 + %arg2 * 64, %arg7 + 2048] : memref&lt;1024x4096xf32&gt;
                %1 = affine.load %arg1[%arg6 + %arg4 * 16, %arg7 + 2048] : memref&lt;8192x4096xf32&gt;
                %2 = affine.load %alloc[%arg5 + %arg2 * 64, %arg6 + %arg4 * 16] : memref&lt;1024x8192xf32&gt;
                %3 = arith.mulf %0, %1 : f32
                %4 = arith.addf %2, %3 : f32
                affine.store %4, %alloc[%arg5 + %arg2 * 64, %arg6 + %arg4 * 16] : memref&lt;1024x8192xf32&gt;
              }
            }
          }
        }
      
    }
    affine.for %arg2 = 0 to 16 {
      affine.for %arg3 = 0 to 512 {
        affine.for %arg4 = 0 to 64 {
          affine.for %arg5 = 0 to 16 {
            %0 = affine.load %alloc[%arg4 + %arg2 * 64, %arg5 + %arg3 * 16] : memref&lt;1024x8192xf32&gt;
            affine.store %0, %alloc[%arg4 + %arg2 * 64, %arg5 + %arg3 * 16] : memref&lt;1024x8192xf32&gt;
          }
        }
      }
    }
    return %alloc : memref&lt;1024x8192xf32&gt;
  }
}
```
I found it was hard to minimize this example much more and keep the failure. 

[stack_dump.txt](https://github.com/user-attachments/files/25111339/stack_dump.txt)
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-affine

Author: Nathan Kaplan (naskap)

<details>
LLVM version 21.1.8
Hash: 2078da43e25a4623cab

getFusionComputeCost fails with assertion failure: sliceIterationCount &gt; 0 failed

Command: `mlir-opt --affine-loop-fusion testfile.mlir`

Test File:
```
module {

  func.func @<!-- -->foo(%arg0: memref&lt;1024x4096xf32&gt;, %arg1: memref&lt;8192x4096xf32&gt;) -&gt; memref&lt;1024x8192xf32&gt; {
    %cst_0 = arith.constant 1.000000e+00 : f32
    %alloc = memref.alloc() : memref&lt;1024x8192xf32&gt;
    affine.for %arg2 = 0 to 16 {
        affine.for %arg4 = 0 to 512 {
          affine.for %arg5 = 0 to 64 {
            affine.for %arg6 = 0 to 16 {
              affine.for %arg7 = 0 to 2048 {
                %0 = affine.load %arg0[%arg5 + %arg2 * 64, %arg7] : memref&lt;1024x4096xf32&gt;
                %1 = affine.load %arg1[%arg6 + %arg4 * 16, %arg7] : memref&lt;8192x4096xf32&gt;
                %2 = affine.load %alloc[%arg5 + %arg2 * 64, %arg6 + %arg4 * 16] : memref&lt;1024x8192xf32&gt;
                %3 = arith.mulf %0, %1 : f32
                %4 = arith.addf %2, %3 : f32
                affine.store %4, %alloc[%arg5 + %arg2 * 64, %arg6 + %arg4 * 16] : memref&lt;1024x8192xf32&gt;
              }
            }
          }
        }

        affine.for %arg4 = 0 to 512 {
          affine.for %arg5 = 0 to 64 {
            affine.for %arg6 = 0 to 16 {
              affine.for %arg7 = 0 to 2048 {
                %0 = affine.load %arg0[%arg5 + %arg2 * 64, %arg7 + 2048] : memref&lt;1024x4096xf32&gt;
                %1 = affine.load %arg1[%arg6 + %arg4 * 16, %arg7 + 2048] : memref&lt;8192x4096xf32&gt;
                %2 = affine.load %alloc[%arg5 + %arg2 * 64, %arg6 + %arg4 * 16] : memref&lt;1024x8192xf32&gt;
                %3 = arith.mulf %0, %1 : f32
                %4 = arith.addf %2, %3 : f32
                affine.store %4, %alloc[%arg5 + %arg2 * 64, %arg6 + %arg4 * 16] : memref&lt;1024x8192xf32&gt;
              }
            }
          }
        }
      
    }
    affine.for %arg2 = 0 to 16 {
      affine.for %arg3 = 0 to 512 {
        affine.for %arg4 = 0 to 64 {
          affine.for %arg5 = 0 to 16 {
            %0 = affine.load %alloc[%arg4 + %arg2 * 64, %arg5 + %arg3 * 16] : memref&lt;1024x8192xf32&gt;
            affine.store %0, %alloc[%arg4 + %arg2 * 64, %arg5 + %arg3 * 16] : memref&lt;1024x8192xf32&gt;
          }
        }
      }
    }
    return %alloc : memref&lt;1024x8192xf32&gt;
  }
}
```
I found it was hard to minimize this example much more and keep the failure. 

[stack_dump.txt](https://github.com/user-attachments/files/25111339/stack_dump.txt)
</details>


---

### Comment 3 - EugeneZelenko

Could you please try 22 release candidate or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.


---

### Comment 4 - naskap

Ah ok. I didn't see any commits that seemed relevant between  21.1.8 and main. The result from godbolt using a slightly modified program from the one above is interesting: [https://godbolt.org/z/7hz56775e](https://godbolt.org/z/7hz56775e)

It completes without an error, but the result is wrong. It seems to fuse loops but not remove the original. For example, 
`affine.for %arg3 = 0 to 16 {` is a duplicate

Since it's a different program and no longer the same error, I could resubmit as a separate issue if preferred

---

### Comment 5 - bondhugula

Thank you for reporting this. Fixed here: https://github.com/llvm/llvm-project/pull/182712
For the other scenario you are reporting, it's expected behavior that the source nest isn't removed in some cases. In any case, it'd be good to open a separate issue for that.

---

