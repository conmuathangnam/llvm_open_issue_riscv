# [mlir] SEGV `--test-bytecode-roundtrip=test-kind=4`

**Author:** YuanchengJiang
**URL:** https://github.com/llvm/llvm-project/issues/163337
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-13T13:17:21Z

## Body

PoC:
```mlir
spirv.module Logical GLSL450 {
    spirv.func @callee() -> () "None" {
      spirv.Kill
    }
    spirv.func @do_not_inline_kill() -> () "None" {
      spirv.FunctionCall @callee() : () -> ()
      spirv.Return
    }
  }
  func.func @fusion_bridge_698938439() -> i32 {
    %c = arith.constant 0 : i32
    return %c : i32
  }
  %fusion_tmp = func.call @fusion_bridge_698938439() : () -> i32
  "test.versionedC"() <{attribute = #test.attr_params<42, 24>}> : () -> ()
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Yuancheng Jiang (YuanchengJiang)

<details>
PoC:
```
spirv.module Logical GLSL450 {
    spirv.func @<!-- -->callee() -&gt; () "None" {
      spirv.Kill
    }
    spirv.func @<!-- -->do_not_inline_kill() -&gt; () "None" {
      spirv.FunctionCall @<!-- -->callee() : () -&gt; ()
      spirv.Return
    }
  }
  func.func @<!-- -->fusion_bridge_698938439() -&gt; i32 {
    %c = arith.constant 0 : i32
    return %c : i32
  }
  %fusion_tmp = func.call @<!-- -->fusion_bridge_698938439() : () -&gt; i32
  "test.versionedC"() &lt;{attribute = #test.attr_params&lt;42, 24&gt;}&gt; : () -&gt; ()
```
</details>


---

### Comment 2 - YuanchengJiang

stderr:
```
phpfuzz@fd48f8905aed:~/WorkSpace/mlir_out/crashes/CRASH_749bef6c6f2c0fb2$ /home/phpfuzz/WorkSpace/llvm-mlir-build/bin/mlir-opt -split-input-file --test-bytecode-roundtrip=test-kind=4 ./id_000000.mlir 
./id_000000.mlir:276:9: warning: near miss with file split marker
  // ---- FlowFusion bridge ----
        ^
./id_000000.mlir:19:4: error: expected operation name in quotes
  }
   ^
// -----
module {
  spirv.module Logical GLSL450 {
    spirv.func @callee() -> i32 "None" {
      %cst42_i32 = spirv.Constant 42 : i32
      spirv.ReturnValue %cst42_i32 : i32
    }
    spirv.func @calling_single_block_retval_func() -> i32 "None" {
      %0 = spirv.FunctionCall @callee() : () -> i32
      spirv.ReturnValue %0 : i32
    }
  }
}
module {
  spirv.module Logical GLSL450 {
    spirv.func @callee() -> i32 "None" {
      %cst42_i32 = spirv.Constant 42 : i32
      spirv.ReturnValue %cst42_i32 : i32
    }
    spirv.func @calling_single_block_retval_func() -> i32 "None" {
      %0 = spirv.FunctionCall @callee() : () -> i32
      spirv.ReturnValue %0 : i32
    }
  }
}

// -----
module {
  spirv.module Logical GLSL450 {
    spirv.GlobalVariable @data bind(0, 0) : !spirv.ptr<!spirv.struct<(!spirv.rtarray<i32> [0])>, StorageBuffer>
    spirv.func @callee() "None" {
      %data_addr = spirv.mlir.addressof @data : !spirv.ptr<!spirv.struct<(!spirv.rtarray<i32> [0])>, StorageBuffer>
      %cst0_i32 = spirv.Constant 0 : i32
      %0 = spirv.AccessChain %data_addr[%cst0_i32, %cst0_i32] : !spirv.ptr<!spirv.struct<(!spirv.rtarray<i32> [0])>, StorageBuffer>, i32, i32 -> !spirv.ptr<i32, StorageBuffer>
      spirv.Branch ^bb1
    ^bb1:  // pred: ^bb0
      %cst42_i32 = spirv.Constant 42 : i32
      spirv.Store "StorageBuffer" %0, %cst42_i32 : i32
      spirv.Return
    }
    spirv.func @calling_multi_block_ret_func() "None" {
      spirv.FunctionCall @callee() : () -> ()
      spirv.Return
    }
  }
}
module {
  spirv.module Logical GLSL450 {
    spirv.GlobalVariable @data bind(0, 0) : !spirv.ptr<!spirv.struct<(!spirv.rtarray<i32> [0])>, StorageBuffer>
    spirv.func @callee() "None" {
      %data_addr = spirv.mlir.addressof @data : !spirv.ptr<!spirv.struct<(!spirv.rtarray<i32> [0])>, StorageBuffer>
      %cst0_i32 = spirv.Constant 0 : i32
      %0 = spirv.AccessChain %data_addr[%cst0_i32, %cst0_i32] : !spirv.ptr<!spirv.struct<(!spirv.rtarray<i32> [0])>, StorageBuffer>, i32, i32 -> !spirv.ptr<i32, StorageBuffer>
      spirv.Branch ^bb1
    ^bb1:  // pred: ^bb0
      %cst42_i32 = spirv.Constant 42 : i32
      spirv.Store "StorageBuffer" %0, %cst42_i32 : i32
      spirv.Return
    }
    spirv.func @calling_multi_block_ret_func() "None" {
      spirv.FunctionCall @callee() : () -> ()
      spirv.Return
    }
  }
}

// -----
module {
  spirv.module Logical GLSL450 {
    spirv.func @callee(%arg0: i1) "None" {
      spirv.mlir.selection {
        spirv.BranchConditional %arg0, ^bb1, ^bb2
      ^bb1:  // pred: ^bb0
        spirv.Return
      ^bb2:  // pred: ^bb0
        spirv.mlir.merge
      }
      spirv.Return
    }
    spirv.func @calling_selection_ret_func() "None" {
      %true = spirv.Constant true
      spirv.FunctionCall @callee(%true) : (i1) -> ()
      spirv.Return
    }
  }
}
module {
  spirv.module Logical GLSL450 {
    spirv.func @callee(%arg0: i1) "None" {
      spirv.mlir.selection {
        spirv.BranchConditional %arg0, ^bb1, ^bb2
      ^bb1:  // pred: ^bb0
        spirv.Return
      ^bb2:  // pred: ^bb0
        spirv.mlir.merge
      }
      spirv.Return
    }
    spirv.func @calling_selection_ret_func() "None" {
      %true = spirv.Constant true
      spirv.FunctionCall @callee(%true) : (i1) -> ()
      spirv.Return
    }
  }
}

// -----
module {
  spirv.module Logical GLSL450 {
    spirv.func @callee(%arg0: i1) "None" {
      spirv.mlir.selection {
        spirv.BranchConditional %arg0, ^bb1, ^bb2
      ^bb1:  // pred: ^bb0
        spirv.Branch ^bb2
      ^bb2:  // 2 preds: ^bb0, ^bb1
        spirv.mlir.merge
      }
      spirv.Return
    }
    spirv.func @calling_selection_no_ret_func() "None" {
      %true = spirv.Constant true
      spirv.FunctionCall @callee(%true) : (i1) -> ()
      spirv.Return
    }
  }
}
module {
  spirv.module Logical GLSL450 {
    spirv.func @callee(%arg0: i1) "None" {
      spirv.mlir.selection {
        spirv.BranchConditional %arg0, ^bb1, ^bb2
      ^bb1:  // pred: ^bb0
        spirv.Branch ^bb2
      ^bb2:  // 2 preds: ^bb0, ^bb1
        spirv.mlir.merge
      }
      spirv.Return
    }
    spirv.func @calling_selection_no_ret_func() "None" {
      %true = spirv.Constant true
      spirv.FunctionCall @callee(%true) : (i1) -> ()
      spirv.Return
    }
  }
}

// -----
module {
  spirv.module Logical GLSL450 {
    spirv.func @callee(%arg0: i1) "None" {
      spirv.mlir.loop {
        spirv.Branch ^bb1
      ^bb1:  // 2 preds: ^bb0, ^bb3
        spirv.BranchConditional %arg0, ^bb2, ^bb4
      ^bb2:  // pred: ^bb1
        spirv.Return
      ^bb3:  // no predecessors
        spirv.Branch ^bb1
      ^bb4:  // pred: ^bb1
        spirv.mlir.merge
      }
      spirv.Return
    }
    spirv.func @calling_loop_ret_func() "None" {
      %true = spirv.Constant true
      spirv.FunctionCall @callee(%true) : (i1) -> ()
      spirv.Return
    }
  }
}
module {
  spirv.module Logical GLSL450 {
    spirv.func @callee(%arg0: i1) "None" {
      spirv.mlir.loop {
        spirv.Branch ^bb1
      ^bb1:  // 2 preds: ^bb0, ^bb3
        spirv.BranchConditional %arg0, ^bb2, ^bb4
      ^bb2:  // pred: ^bb1
        spirv.Return
      ^bb3:  // no predecessors
        spirv.Branch ^bb1
      ^bb4:  // pred: ^bb1
        spirv.mlir.merge
      }
      spirv.Return
    }
    spirv.func @calling_loop_ret_func() "None" {
      %true = spirv.Constant true
      spirv.FunctionCall @callee(%true) : (i1) -> ()
      spirv.Return
    }
  }
}

// -----
module {
  spirv.module Logical GLSL450 {
    spirv.func @callee(%arg0: i1) "None" {
      spirv.mlir.loop {
        spirv.Branch ^bb1
      ^bb1:  // 2 preds: ^bb0, ^bb3
        spirv.BranchConditional %arg0, ^bb2, ^bb4
      ^bb2:  // pred: ^bb1
        spirv.Branch ^bb3
      ^bb3:  // pred: ^bb2
        spirv.Branch ^bb1
      ^bb4:  // pred: ^bb1
        spirv.mlir.merge
      }
      spirv.Return
    }
    spirv.func @calling_loop_no_ret_func() "None" {
      %true = spirv.Constant true
      spirv.FunctionCall @callee(%true) : (i1) -> ()
      spirv.Return
    }
  }
}
module {
  spirv.module Logical GLSL450 {
    spirv.func @callee(%arg0: i1) "None" {
      spirv.mlir.loop {
        spirv.Branch ^bb1
      ^bb1:  // 2 preds: ^bb0, ^bb3
        spirv.BranchConditional %arg0, ^bb2, ^bb4
      ^bb2:  // pred: ^bb1
        spirv.Branch ^bb3
      ^bb3:  // pred: ^bb2
        spirv.Branch ^bb1
      ^bb4:  // pred: ^bb1
        spirv.mlir.merge
      }
      spirv.Return
    }
    spirv.func @calling_loop_no_ret_func() "None" {
      %true = spirv.Constant true
      spirv.FunctionCall @callee(%true) : (i1) -> ()
      spirv.Return
    }
  }
}

// -----
module {
  spirv.module Logical GLSL450 {
    spirv.GlobalVariable @arg_0 bind(0, 0) : !spirv.ptr<!spirv.struct<(i32 [0])>, StorageBuffer>
    spirv.GlobalVariable @arg_1 bind(0, 1) : !spirv.ptr<!spirv.struct<(i32 [0])>, StorageBuffer>
    spirv.func @inline_into_selection_region() "None" {
      %cst0_i32 = spirv.Constant 0 : i32
      %arg_0_addr = spirv.mlir.addressof @arg_0 : !spirv.ptr<!spirv.struct<(i32 [0])>, StorageBuffer>
      %arg_1_addr = spirv.mlir.addressof @arg_1 : !spirv.ptr<!spirv.struct<(i32 [0])>, StorageBuffer>
      %0 = spirv.AccessChain %arg_0_addr[%cst0_i32] : !spirv.ptr<!spirv.struct<(i32 [0])>, StorageBuffer>, i32 -> !spirv.ptr<i32, StorageBuffer>
      %1 = spirv.Load "StorageBuffer" %0 : i32
      %2 = spirv.SGreaterThan %1, %cst0_i32 : i32
      spirv.mlir.selection {
        spirv.BranchConditional %2, ^bb1, ^bb2
      ^bb1:  // pred: ^bb0
        %3 = spirv.AccessChain %arg_1_addr[%cst0_i32] : !spirv.ptr<!spirv.struct<(i32 [0])>, StorageBuffer>, i32 -> !spirv.ptr<i32, StorageBuffer>
        spirv.FunctionCall @atomic_add(%1, %3) : (i32, !spirv.ptr<i32, StorageBuffer>) -> ()
        spirv.Branch ^bb2
      ^bb2:  // 2 preds: ^bb0, ^bb1
        spirv.mlir.merge
      }
      spirv.Return
    }
    spirv.func @atomic_add(%arg0: i32, %arg1: !spirv.ptr<i32, StorageBuffer>) "None" {
      %0 = spirv.AtomicIAdd <Device> <AcquireRelease> %arg1, %arg0 : !spirv.ptr<i32, StorageBuffer>
      spirv.Return
    }
    spirv.EntryPoint "GLCompute" @inline_into_selection_region
    spirv.ExecutionMode @inline_into_selection_region "LocalSize", 32, 1, 1
  }
}
module {
  spirv.module Logical GLSL450 {
    spirv.GlobalVariable @arg_0 bind(0, 0) : !spirv.ptr<!spirv.struct<(i32 [0])>, StorageBuffer>
    spirv.GlobalVariable @arg_1 bind(0, 1) : !spirv.ptr<!spirv.struct<(i32 [0])>, StorageBuffer>
    spirv.func @inline_into_selection_region() "None" {
      %cst0_i32 = spirv.Constant 0 : i32
      %arg_0_addr = spirv.mlir.addressof @arg_0 : !spirv.ptr<!spirv.struct<(i32 [0])>, StorageBuffer>
      %arg_1_addr = spirv.mlir.addressof @arg_1 : !spirv.ptr<!spirv.struct<(i32 [0])>, StorageBuffer>
      %0 = spirv.AccessChain %arg_0_addr[%cst0_i32] : !spirv.ptr<!spirv.struct<(i32 [0])>, StorageBuffer>, i32 -> !spirv.ptr<i32, StorageBuffer>
      %1 = spirv.Load "StorageBuffer" %0 : i32
      %2 = spirv.SGreaterThan %1, %cst0_i32 : i32
      spirv.mlir.selection {
        spirv.BranchConditional %2, ^bb1, ^bb2
      ^bb1:  // pred: ^bb0
        %3 = spirv.AccessChain %arg_1_addr[%cst0_i32] : !spirv.ptr<!spirv.struct<(i32 [0])>, StorageBuffer>, i32 -> !spirv.ptr<i32, StorageBuffer>
        spirv.FunctionCall @atomic_add(%1, %3) : (i32, !spirv.ptr<i32, StorageBuffer>) -> ()
        spirv.Branch ^bb2
      ^bb2:  // 2 preds: ^bb0, ^bb1
        spirv.mlir.merge
      }
      spirv.Return
    }
    spirv.func @atomic_add(%arg0: i32, %arg1: !spirv.ptr<i32, StorageBuffer>) "None" {
      %0 = spirv.AtomicIAdd <Device> <AcquireRelease> %arg1, %arg0 : !spirv.ptr<i32, StorageBuffer>
      spirv.Return
    }
    spirv.EntryPoint "GLCompute" @inline_into_selection_region
    spirv.ExecutionMode @inline_into_selection_region "LocalSize", 32, 1, 1
  }
}

// -----
module {
  spirv.module Logical GLSL450 {
    spirv.func @foo(%arg0: i32) -> i32 "None" {
      %0 = spirv.FunctionCall @bar(%arg0) : (i32) -> i32
      spirv.ReturnValue %0 : i32
    }
    spirv.func @bar(%arg0: i32) -> i32 "None" attributes {sym_visibility = "private"} {
      %cst1_i32 = spirv.Constant 1 : i32
      %0 = spirv.IEqual %arg0, %cst1_i32 : i32
      spirv.BranchConditional %0, ^bb1(%cst1_i32 : i32), ^bb2
    ^bb1(%1: i32):  // pred: ^bb0
      spirv.ReturnValue %1 : i32
    ^bb2:  // pred: ^bb0
      spirv.ReturnValue %cst1_i32 : i32
    }
  }
}
module {
  spirv.module Logical GLSL450 {
    spirv.func @foo(%arg0: i32) -> i32 "None" {
      %0 = spirv.FunctionCall @bar(%arg0) : (i32) -> i32
      spirv.ReturnValue %0 : i32
    }
    spirv.func @bar(%arg0: i32) -> i32 "None" attributes {sym_visibility = "private"} {
      %cst1_i32 = spirv.Constant 1 : i32
      %0 = spirv.IEqual %arg0, %cst1_i32 : i32
      spirv.BranchConditional %0, ^bb1(%cst1_i32 : i32), ^bb2
    ^bb1(%1: i32):  // pred: ^bb0
      spirv.ReturnValue %1 : i32
    ^bb2:  // pred: ^bb0
      spirv.ReturnValue %cst1_i32 : i32
    }
  }
}

// -----
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /home/phpfuzz/WorkSpace/llvm-mlir-build/bin/mlir-opt -split-input-file --test-bytecode-roundtrip=test-kind=4 ./id_000000.mlir
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  mlir-opt  0x00005d13baf0469b __interceptor_backtrace + 91
1  mlir-opt  0x00005d13bafd1ad7 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 343
2  mlir-opt  0x00005d13bafcbdbf llvm::sys::RunSignalHandlers() + 207
3  mlir-opt  0x00005d13bafd4e16
4  libc.so.6 0x0000782760f65520
5  mlir-opt  0x00005d13cdf4952a
6  mlir-opt  0x00005d13bc3189e6
7  mlir-opt  0x00005d13c9ed26a4
8  mlir-opt  0x00005d13c9ed1e17
9  mlir-opt  0x00005d13cdf5596a
10 mlir-opt  0x00005d13cdf498ee
11 mlir-opt  0x00005d13bc3189e6
12 mlir-opt  0x00005d13c9ed26a4
13 mlir-opt  0x00005d13c9ed1e17
14 mlir-opt  0x00005d13cdf5596a
15 mlir-opt  0x00005d13cdf498ee
16 mlir-opt  0x00005d13bc3189e6
17 mlir-opt  0x00005d13c9ed26a4
18 mlir-opt  0x00005d13c9ed1e17
19 mlir-opt  0x00005d13cdf5596a
20 mlir-opt  0x00005d13cdf498ee
21 mlir-opt  0x00005d13bc3189e6
22 mlir-opt  0x00005d13c9ed26a4
23 mlir-opt  0x00005d13c9ed1e17
24 mlir-opt  0x00005d13cdf5596a
25 mlir-opt  0x00005d13cdf498ee
26 mlir-opt  0x00005d13bc3189e6
27 mlir-opt  0x00005d13c9ed26a4
28 mlir-opt  0x00005d13c9ed1e17
29 mlir-opt  0x00005d13cdf5596a
30 mlir-opt  0x00005d13cdf498ee
31 mlir-opt  0x00005d13bc3189e6
32 mlir-opt  0x00005d13c9ed26a4
33 mlir-opt  0x00005d13c9ed1e17
34 mlir-opt  0x00005d13cdf5596a
35 mlir-opt  0x00005d13cdf498ee
36 mlir-opt  0x00005d13bc3189e6
37 mlir-opt  0x00005d13c9ed26a4
38 mlir-opt  0x00005d13c9ed1e17
39 mlir-opt  0x00005d13cdf5596a
40 mlir-opt  0x00005d13cdf498ee
41 mlir-opt  0x00005d13bc3189e6
42 mlir-opt  0x00005d13c9ed26a4
43 mlir-opt  0x00005d13c9ed1e17
44 mlir-opt  0x00005d13cdf5596a
45 mlir-opt  0x00005d13cdf498ee
46 mlir-opt  0x00005d13bc3189e6
47 mlir-opt  0x00005d13c9ed26a4
48 mlir-opt  0x00005d13c9ed1e17
49 mlir-opt  0x00005d13cdf5596a
50 mlir-opt  0x00005d13cdf498ee
51 mlir-opt  0x00005d13bc3189e6
52 mlir-opt  0x00005d13c9ed26a4
53 mlir-opt  0x00005d13c9ed1e17
54 mlir-opt  0x00005d13cdf5596a
55 mlir-opt  0x00005d13cdf498ee
56 mlir-opt  0x00005d13bc3189e6
57 mlir-opt  0x00005d13c9ed26a4
58 mlir-opt  0x00005d13c9ed1e17
59 mlir-opt  0x00005d13cdf5596a
60 mlir-opt  0x00005d13cdf498ee
61 mlir-opt  0x00005d13bc3189e6
62 mlir-opt  0x00005d13c9ed26a4
63 mlir-opt  0x00005d13c9ed1e17
64 mlir-opt  0x00005d13cdf5596a
65 mlir-opt  0x00005d13cdf498ee
66 mlir-opt  0x00005d13bc3189e6
67 mlir-opt  0x00005d13c9ed26a4
68 mlir-opt  0x00005d13c9ed1e17
69 mlir-opt  0x00005d13cdf5596a
70 mlir-opt  0x00005d13cdf498ee
71 mlir-opt  0x00005d13bc3189e6
72 mlir-opt  0x00005d13c9ed26a4
73 mlir-opt  0x00005d13c9ed1e17
74 mlir-opt  0x00005d13cdf5596a
75 mlir-opt  0x00005d13cdf498ee
76 mlir-opt  0x00005d13bc3189e6
77 mlir-opt  0x00005d13c9ed26a4
78 mlir-opt  0x00005d13c9ed1e17
79 mlir-opt  0x00005d13cdf5596a
80 mlir-opt  0x00005d13cdf498ee
81 mlir-opt  0x00005d13bc3189e6
82 mlir-opt  0x00005d13c9ed26a4
83 mlir-opt  0x00005d13c9ed1e17
84 mlir-opt  0x00005d13cdf5596a
85 mlir-opt  0x00005d13cdf498ee
86 mlir-opt  0x00005d13bc3189e6
87 mlir-opt  0x00005d13c9ed26a4
88 mlir-opt  0x00005d13c9ed1e17
89 mlir-opt  0x00005d13cdf5596a
90 mlir-opt  0x00005d13cdf498ee
91 mlir-opt  0x00005d13bc3189e6
92 mlir-opt  0x00005d13c9ed26a4
93 mlir-opt  0x00005d13c9ed1e17
94 mlir-opt  0x00005d13cdf5596a
95 mlir-opt  0x00005d13cdf498ee
96 mlir-opt  0x00005d13bc3189e6
97 mlir-opt  0x00005d13c9ed26a4
98 mlir-opt  0x00005d13c9ed1e17
99 mlir-opt  0x00005d13cdf5596a
100 mlir-opt  0x00005d13cdf498ee
101 mlir-opt  0x00005d13bc3189e6
102 mlir-opt  0x00005d13c9ed26a4
103 mlir-opt  0x00005d13c9ed1e17
104 mlir-opt  0x00005d13cdf5596a
105 mlir-opt  0x00005d13cdf498ee
106 mlir-opt  0x00005d13bc3189e6
107 mlir-opt  0x00005d13c9ed26a4
108 mlir-opt  0x00005d13c9ed1e17
109 mlir-opt  0x00005d13cdf5596a
110 mlir-opt  0x00005d13cdf498ee
111 mlir-opt  0x00005d13bc3189e6
112 mlir-opt  0x00005d13c9ed26a4
113 mlir-opt  0x00005d13c9ed1e17
114 mlir-opt  0x00005d13cdf5596a
115 mlir-opt  0x00005d13cdf498ee
116 mlir-opt  0x00005d13bc3189e6
117 mlir-opt  0x00005d13c9ed26a4
118 mlir-opt  0x00005d13c9ed1e17
119 mlir-opt  0x00005d13cdf5596a
120 mlir-opt  0x00005d13cdf498ee
121 mlir-opt  0x00005d13bc3189e6
122 mlir-opt  0x00005d13c9ed26a4
123 mlir-opt  0x00005d13c9ed1e17
124 mlir-opt  0x00005d13cdf5596a
125 mlir-opt  0x00005d13cdf498ee
126 mlir-opt  0x00005d13bc3189e6
127 mlir-opt  0x00005d13c9ed26a4
128 mlir-opt  0x00005d13c9ed1e17
129 mlir-opt  0x00005d13cdf5596a
130 mlir-opt  0x00005d13cdf498ee
131 mlir-opt  0x00005d13bc3189e6
132 mlir-opt  0x00005d13c9ed26a4
133 mlir-opt  0x00005d13c9ed1e17
134 mlir-opt  0x00005d13cdf5596a
135 mlir-opt  0x00005d13cdf498ee
136 mlir-opt  0x00005d13bc3189e6
137 mlir-opt  0x00005d13c9ed26a4
138 mlir-opt  0x00005d13c9ed1e17
139 mlir-opt  0x00005d13cdf5596a
140 mlir-opt  0x00005d13cdf498ee
141 mlir-opt  0x00005d13bc3189e6
142 mlir-opt  0x00005d13c9ed26a4
143 mlir-opt  0x00005d13c9ed1e17
144 mlir-opt  0x00005d13cdf5596a
145 mlir-opt  0x00005d13cdf498ee
146 mlir-opt  0x00005d13bc3189e6
147 mlir-opt  0x00005d13c9ed26a4
148 mlir-opt  0x00005d13c9ed1e17
149 mlir-opt  0x00005d13cdf5596a
150 mlir-opt  0x00005d13cdf498ee
151 mlir-opt  0x00005d13bc3189e6
152 mlir-opt  0x00005d13c9ed26a4
153 mlir-opt  0x00005d13c9ed1e17
154 mlir-opt  0x00005d13cdf5596a
155 mlir-opt  0x00005d13cdf498ee
156 mlir-opt  0x00005d13bc3189e6
157 mlir-opt  0x00005d13c9ed26a4
158 mlir-opt  0x00005d13c9ed1e17
159 mlir-opt  0x00005d13cdf5596a
160 mlir-opt  0x00005d13cdf498ee
161 mlir-opt  0x00005d13bc3189e6
162 mlir-opt  0x00005d13c9ed26a4
163 mlir-opt  0x00005d13c9ed1e17
164 mlir-opt  0x00005d13cdf5596a
165 mlir-opt  0x00005d13cdf498ee
166 mlir-opt  0x00005d13bc3189e6
167 mlir-opt  0x00005d13c9ed26a4
168 mlir-opt  0x00005d13c9ed1e17
169 mlir-opt  0x00005d13cdf5596a
170 mlir-opt  0x00005d13cdf498ee
171 mlir-opt  0x00005d13bc3189e6
172 mlir-opt  0x00005d13c9ed26a4
173 mlir-opt  0x00005d13c9ed1e17
174 mlir-opt  0x00005d13cdf5596a
175 mlir-opt  0x00005d13cdf498ee
176 mlir-opt  0x00005d13bc3189e6
177 mlir-opt  0x00005d13c9ed26a4
178 mlir-opt  0x00005d13c9ed1e17
179 mlir-opt  0x00005d13cdf5596a
180 mlir-opt  0x00005d13cdf498ee
181 mlir-opt  0x00005d13bc3189e6
182 mlir-opt  0x00005d13c9ed26a4
183 mlir-opt  0x00005d13c9ed1e17
184 mlir-opt  0x00005d13cdf5596a
185 mlir-opt  0x00005d13cdf498ee
186 mlir-opt  0x00005d13bc3189e6
187 mlir-opt  0x00005d13c9ed26a4
188 mlir-opt  0x00005d13c9ed1e17
189 mlir-opt  0x00005d13cdf5596a
190 mlir-opt  0x00005d13cdf498ee
191 mlir-opt  0x00005d13bc3189e6
192 mlir-opt  0x00005d13c9ed26a4
193 mlir-opt  0x00005d13c9ed1e17
194 mlir-opt  0x00005d13cdf5596a
195 mlir-opt  0x00005d13cdf498ee
196 mlir-opt  0x00005d13bc3189e6
197 mlir-opt  0x00005d13c9ed26a4
198 mlir-opt  0x00005d13c9ed1e17
199 mlir-opt  0x00005d13cdf5596a
200 mlir-opt  0x00005d13cdf498ee
201 mlir-opt  0x00005d13bc3189e6
202 mlir-opt  0x00005d13c9ed26a4
203 mlir-opt  0x00005d13c9ed1e17
204 mlir-opt  0x00005d13cdf5596a
205 mlir-opt  0x00005d13cdf498ee
206 mlir-opt  0x00005d13bc3189e6
207 mlir-opt  0x00005d13c9ed26a4
208 mlir-opt  0x00005d13c9ed1e17
209 mlir-opt  0x00005d13cdf5596a
210 mlir-opt  0x00005d13cdf498ee
211 mlir-opt  0x00005d13bc3189e6
212 mlir-opt  0x00005d13c9ed26a4
213 mlir-opt  0x00005d13c9ed1e17
214 mlir-opt  0x00005d13cdf5596a
215 mlir-opt  0x00005d13cdf498ee
216 mlir-opt  0x00005d13bc3189e6
217 mlir-opt  0x00005d13c9ed26a4
218 mlir-opt  0x00005d13c9ed1e17
219 mlir-opt  0x00005d13cdf5596a
220 mlir-opt  0x00005d13cdf498ee
221 mlir-opt  0x00005d13bc3189e6
222 mlir-opt  0x00005d13c9ed26a4
223 mlir-opt  0x00005d13c9ed1e17
224 mlir-opt  0x00005d13cdf5596a
225 mlir-opt  0x00005d13cdf498ee
226 mlir-opt  0x00005d13bc3189e6
227 mlir-opt  0x00005d13c9ed26a4
228 mlir-opt  0x00005d13c9ed1e17
229 mlir-opt  0x00005d13cdf5596a
230 mlir-opt  0x00005d13cdf498ee
231 mlir-opt  0x00005d13bc3189e6
232 mlir-opt  0x00005d13c9ed26a4
233 mlir-opt  0x00005d13c9ed1e17
234 mlir-opt  0x00005d13cdf5596a
235 mlir-opt  0x00005d13cdf498ee
236 mlir-opt  0x00005d13bc3189e6
237 mlir-opt  0x00005d13c9ed26a4
238 mlir-opt  0x00005d13c9ed1e17
239 mlir-opt  0x00005d13cdf5596a
240 mlir-opt  0x00005d13cdf498ee
241 mlir-opt  0x00005d13bc3189e6
242 mlir-opt  0x00005d13c9ed26a4
243 mlir-opt  0x00005d13c9ed1e17
244 mlir-opt  0x00005d13cdf5596a
245 mlir-opt  0x00005d13cdf498ee
246 mlir-opt  0x00005d13bc3189e6
247 mlir-opt  0x00005d13c9ed26a4
248 mlir-opt  0x00005d13c9ed1e17
249 mlir-opt  0x00005d13cdf5596a
250 mlir-opt  0x00005d13cdf498ee
251 mlir-opt  0x00005d13bc3189e6
252 mlir-opt  0x00005d13c9ed26a4
253 mlir-opt  0x00005d13c9ed1e17
254 mlir-opt  0x00005d13cdf5596a
255 mlir-opt  0x00005d13cdf498ee
AddressSanitizer:DEADLYSIGNAL
=================================================================
==1067579==ERROR: AddressSanitizer: SEGV on unknown address 0x7ffc7a63dda0 (pc 0x78276104188d bp 0x782760b85770 sp 0x782760b855f8 T0)
==1067579==The signal is caused by a WRITE memory access.
    #0 0x78276104188d in syscall misc/../sysdeps/unix/sysv/linux/x86_64/syscall.S:38
    #1 0x5d13bafd4e57 in SignalHandler(int, siginfo_t*, void*) /home/phpfuzz/WorkSpace/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:7
    #2 0x782760f6551f  (/lib/x86_64-linux-gnu/libc.so.6+0x4251f) (BuildId: cd410b710f0f094c6832edd95931006d883af48e)
    #3 0x5d13cdf49529 in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:161
    #4 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #5 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #6 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #7 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #8 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #9 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #10 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #11 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #12 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #13 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #14 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #15 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #16 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #17 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #18 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #19 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #20 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #21 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #22 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #23 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #24 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #25 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #26 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #27 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #28 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #29 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #30 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #31 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #32 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #33 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #34 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #35 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #36 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #37 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #38 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #39 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #40 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #41 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #42 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #43 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #44 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #45 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #46 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #47 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #48 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #49 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #50 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #51 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #52 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #53 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #54 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #55 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #56 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #57 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #58 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #59 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #60 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #61 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #62 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #63 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #64 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #65 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #66 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #67 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #68 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #69 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #70 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #71 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #72 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #73 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #74 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #75 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #76 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #77 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #78 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #79 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #80 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #81 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #82 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #83 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #84 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #85 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #86 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #87 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #88 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #89 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #90 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #91 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #92 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #93 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #94 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #95 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #96 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #97 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #98 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #99 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #100 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #101 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #102 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #103 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #104 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #105 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #106 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #107 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #108 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #109 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #110 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #111 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #112 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #113 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #114 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #115 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #116 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #117 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #118 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #119 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #120 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #121 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #122 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #123 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #124 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #125 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #126 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #127 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #128 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #129 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #130 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #131 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #132 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #133 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #134 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #135 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #136 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #137 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #138 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #139 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #140 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #141 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #142 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #143 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #144 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #145 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #146 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #147 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #148 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #149 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #150 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #151 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #152 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #153 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #154 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #155 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #156 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #157 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #158 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #159 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #160 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #161 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #162 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #163 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #164 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #165 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #166 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #167 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #168 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #169 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #170 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #171 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #172 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #173 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #174 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #175 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #176 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #177 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #178 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #179 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #180 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #181 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #182 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #183 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #184 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #185 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #186 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #187 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #188 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #189 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #190 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #191 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #192 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #193 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #194 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #195 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #196 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #197 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #198 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #199 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #200 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #201 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #202 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #203 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #204 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #205 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #206 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #207 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #208 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #209 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #210 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #211 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #212 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #213 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #214 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #215 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #216 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #217 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #218 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #219 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #220 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #221 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #222 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #223 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #224 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #225 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #226 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #227 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #228 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #229 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #230 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #231 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #232 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #233 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #234 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #235 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #236 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #237 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #238 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #239 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #240 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #241 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #242 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #243 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #244 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #245 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #246 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #247 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #248 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #249 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #250 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #251 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #252 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #253 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #254 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #255 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #256 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #257 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #258 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #259 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #260 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #261 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #262 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #263 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #264 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #265 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #266 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #267 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #268 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #269 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #270 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #271 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #272 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #273 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #274 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #275 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #276 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #277 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #278 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #279 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #280 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #281 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #282 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #283 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #284 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #285 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #286 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #287 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #288 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #289 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #290 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #291 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #292 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #293 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #294 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #295 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #296 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #297 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #298 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #299 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #300 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #301 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #302 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #303 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #304 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #305 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #306 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #307 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #308 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #309 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #310 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #311 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #312 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #313 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #314 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #315 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #316 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #317 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #318 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #319 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #320 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #321 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #322 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #323 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #324 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #325 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #326 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #327 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #328 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #329 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #330 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #331 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #332 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #333 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #334 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #335 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #336 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #337 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #338 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #339 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #340 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #341 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #342 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #343 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #344 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #345 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #346 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #347 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #348 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #349 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #350 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #351 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #352 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #353 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #354 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #355 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #356 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #357 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #358 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #359 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #360 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #361 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #362 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #363 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #364 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #365 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #366 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #367 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #368 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #369 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #370 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #371 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #372 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #373 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #374 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #375 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #376 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #377 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #378 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #379 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #380 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #381 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #382 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #383 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #384 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #385 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #386 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #387 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #388 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #389 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #390 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #391 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #392 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #393 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #394 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #395 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #396 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #397 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #398 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #399 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #400 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #401 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #402 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #403 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #404 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #405 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #406 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #407 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #408 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #409 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #410 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #411 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #412 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #413 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #414 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #415 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #416 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #417 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #418 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #419 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #420 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #421 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #422 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #423 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #424 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #425 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #426 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #427 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #428 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #429 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #430 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #431 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #432 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #433 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #434 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #435 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #436 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #437 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #438 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #439 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #440 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #441 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #442 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #443 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #444 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #445 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #446 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #447 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #448 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #449 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #450 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #451 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #452 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #453 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #454 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #455 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #456 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #457 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #458 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #459 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #460 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #461 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #462 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #463 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #464 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #465 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #466 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #467 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #468 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #469 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #470 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #471 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #472 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #473 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #474 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #475 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #476 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #477 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #478 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #479 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #480 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #481 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #482 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #483 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #484 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #485 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #486 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #487 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #488 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #489 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #490 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #491 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #492 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #493 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #494 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #495 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #496 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #497 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #498 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #499 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #500 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #501 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #502 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #503 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #504 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #505 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #506 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #507 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #508 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #509 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #510 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #511 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #512 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #513 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #514 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #515 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #516 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #517 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #518 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #519 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #520 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #521 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #522 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #523 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #524 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #525 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #526 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #527 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #528 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #529 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #530 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #531 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #532 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #533 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #534 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #535 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #536 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #537 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #538 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #539 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #540 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #541 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #542 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #543 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #544 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #545 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #546 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #547 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #548 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #549 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #550 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #551 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #552 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #553 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #554 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #555 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #556 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #557 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #558 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #559 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #560 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #561 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #562 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #563 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #564 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #565 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #566 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #567 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #568 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #569 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #570 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #571 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #572 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #573 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14
    #574 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)::operator()(mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:53
    #575 0x5d13cdf55969 in llvm::LogicalResult mlir::DialectBytecodeReader::readList<mlir::Attribute, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)>(llvm::SmallVectorImpl<mlir::Attribute>&, llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&)::'lambda'(mlir::Attribute&)&&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:83:20
    #576 0x5d13cdf498ed in llvm::LogicalResult mlir::DialectBytecodeReader::readAttributes<mlir::Attribute>(llvm::SmallVectorImpl<mlir::Attribute>&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeImplementation.h:108:12
    #577 0x5d13cdf498ed in (anonymous namespace)::readArrayAttr(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:24
    #578 0x5d13cdf498ed in (anonymous namespace)::readAttribute(mlir::MLIRContext*, mlir::DialectBytecodeReader&) /home/phpfuzz/WorkSpace/llvm-mlir-build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:381:14
    #579 0x5d13cdf498ed in (anonymous namespace)::BuiltinDialectBytecodeInterface::readAttribute(mlir::DialectBytecodeReader&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/IR/BuiltinDialectBytecode.cpp:162:12
    #580 0x5d13bc3189e5 in (anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)::operator()(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) const /home/phpfuzz/WorkSpace/llvm-project/mlir/test/lib/IR/TestBytecodeRoundtrip.cpp:312:42
    #581 0x5d13bc3189e5 in std::unique_ptr<mlir::AttrTypeBytecodeReader<mlir::Attribute>, std::default_delete<mlir::AttrTypeBytecodeReader<mlir::Attribute> > > mlir::AttrTypeBytecodeReader<mlir::Attribute>::fromCallable<(anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&), true>((anonymous namespace)::TestBytecodeRoundtripPass::runTest4(mlir::Operation*)::'lambda'(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&)&&)::Processor::read(mlir::DialectBytecodeReader&, llvm::StringRef, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/include/mlir/Bytecode/BytecodeReaderConfig.h:52:16
    #582 0x5d13c9ed26a3 in llvm::LogicalResult (anonymous namespace)::AttrTypeReader::parseCustomEntry<mlir::Attribute>((anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute>&, (anonymous namespace)::EncodingReader&, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1327:25
    #583 0x5d13c9ed26a3 in mlir::Attribute (anonymous namespace)::AttrTypeReader::resolveEntry<mlir::Attribute>(llvm::SmallVectorImpl<(anonymous namespace)::AttrTypeReader::Entry<mlir::Attribute> >&, unsigned long, llvm::StringRef) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:1258:16
    #584 0x5d13c9ed26a3 in (anonymous namespace)::AttrTypeReader::resolveAttribute(unsigned long) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:869:12
    #585 0x5d13c9ed1e16 in (anonymous namespace)::AttrTypeReader::parseAttribute((anonymous namespace)::EncodingReader&, mlir::Attribute&) /home/phpfuzz/WorkSpace/llvm-project/mlir/lib/Bytecode/Reader/BytecodeReader.cpp:878:14

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV misc/../sysdeps/unix/sysv/linux/x86_64/syscall.S:38 in syscall
==1067579==ABORTING
phpfuzz@fd48f8905aed:~/WorkSpace/mlir_out/crashes/CRASH_749bef6c6f2c0fb2$ 

```

---

