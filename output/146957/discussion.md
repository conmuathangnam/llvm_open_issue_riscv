# Rust compilation error on some targets

**Author:** SyxtonPrime
**URL:** https://github.com/llvm/llvm-project/issues/146957
**Status:** Closed
**Labels:** backend:X86, miscompilation, release:backport, rust
**Closed Date:** 2025-07-04T10:27:15Z

## Body

Cross post from: https://github.com/rust-lang/rust/issues/143399

Not sure if this is a Rust or LLVM issue but I think there is a decent chance this is an LLVM issue so wanted to post it here too.

Recently I have been running into some strange errors which seem to only occur on some architectures. See

https://github.com/Plonky3/Plonky3/issues/729
https://github.com/Plonky3/Plonky3/issues/905

For more info about the situation where these arose. I managed to make a minimal example in Godbolt so I thought it made sense to post this here.

### Code
Probably easier to understand in the following Godbolt link: https://godbolt.org/z/rrrj3eobd

Basically we define a simple function which adds integers mod a prime `P`. We then use this on vectors of length `4`. What you find is that if you check `(x + y) + z = x + (y + z)` manually, everything works as expected and they agree. If however we define a function which computes `(x + y) + z`, `x + (y + z)` and then checks that they are equal we get an error. I agree this seems totally bizarre, please check out the Godbolt.

```(rust)
  use std::array;
  
  /// The Baby Bear prime: 2^31 - 2^27 + 1.
  const P: u32 = 2013265921;
  // To help read the assembly, note: 2^32 - P = 2281701375
  
  /// Addition modulo P.
  ///
  /// Inputs are asusmed to be < P.
  /// Assuming this, outputs will also be < P.
  #[inline(always)]
  fn add(lhs: u32, rhs: u32) -> u32 {
      let mut sum = lhs + rhs; // Never overflows as inputs are < P < 2^31.
      let (corr_sum, over) = sum.overflowing_sub(P);
       // over is false if sum >= P and true if sum < P.
      if !over { 
          sum = corr_sum;
      }
      sum
  }
  
  /// Addition modulo P in a degree 4 extension.
  ///
  /// Identical to 4 additions in parallel.
  #[unsafe(no_mangle)]
  pub fn add_bb_deg_4_ext(lhs: [u32; 4], rhs: [u32; 4]) -> [u32; 4] {
      array::from_fn(|i| add(lhs[i], rhs[i]))
  }
  
  /// Compute (lhs + mid) + rhs
  #[unsafe(no_mangle)]
  pub fn add_bb_assoc_l(lhs: [u32; 4], mid: [u32; 4], rhs: [u32; 4]) -> [u32; 4] {
      let lhs = add_bb_deg_4_ext(lhs, mid);
      add_bb_deg_4_ext(lhs, rhs)
  }
  
  /// Compute lhs + (mid + rhs)
  #[unsafe(no_mangle)]
  pub fn add_bb_assoc_r(lhs: [u32; 4], mid: [u32; 4], rhs: [u32; 4]) -> [u32; 4] {
      let rhs = add_bb_deg_4_ext(mid, rhs);
      add_bb_deg_4_ext(lhs, rhs)
  }
  
  /// Check that (lhs + mid) + rhs = lhs + (mid + rhs)
  #[unsafe(no_mangle)]
  pub fn check_assoc(lhs: [u32; 4], mid: [u32; 4], rhs: [u32; 4]) {
      let assoc_l = add_bb_assoc_l(lhs, mid, rhs);
      let assoc_r = add_bb_assoc_r(lhs, mid, rhs);
      assert_eq!(assoc_l, assoc_r);
  }
  
  fn main() -> () {
      let lhs = [252551971, 694974649, 213757600, 1325013984];
      let mid = [506156623, 97664653, 1234719014, 1349792299];
      let rhs = [1626423134, 1338438783, 786682629, 1311208151];
  
      // Check all elements are < P
      assert!(lhs.iter().all(|&x| x < P));
      assert!(mid.iter().all(|&x| x < P));
      assert!(rhs.iter().all(|&x| x < P));
  
      // Let's manually check that
      // (lhs + mid) + rhs = lhs + (mid + rhs)
      let assoc_l = add_bb_assoc_l(lhs, mid, rhs);
      let assoc_r = add_bb_assoc_r(lhs, mid, rhs);
  
      // Check that (x + y) + z = x + (y + z)
      println!("{assoc_l:?}");
      println!("{assoc_r:?}");
      assert_eq!(assoc_l, assoc_r);
      println!("The two assoc's are equal");
      // Everything up until here runs.
  
      // Now let's check the same thing using our
      // function check_assoc
      check_assoc(lhs, mid, rhs);
      // This fails with compilation option:
      // -O -C target-cpu=znver4 -C opt-level=3
      // among others.
  } 
```


### Meta
<!--
If you're using the stable version of the compiler, you should also check if the
bug also exists in the beta or nightly versions.
-->
The error occurs when compiling with current rustc 1.880, beta and nightly. It seems to have been introduced in the move to `LLVM 20` as these errors do not appear on `nightly-2025-02-17` but things begin failing on `nightly-2025-02-18`.

The error occurs when using the compiler flags: `-O -C target-cpu=znver4 -C opt-level=3`.
It does not occur with `opt-level=0, 1, 2`, it does however occur with some other target cpus in particular `mic_avx512` and `znver5` but not with others such as `skylake, skylake_avx512, alderlake, raptorlake`. (If you want a complete list I can go through and check them all)



### Error output
See the Godbolt link for more details. Essentially what seems to be happening is that something goes wrong in the vectorization code? It's hard to say exactly what though. The compiled code for `check_assoc` seems to be reasonable? We are pretty lost as to what is going on.

## Comments

### Comment 1 - nikic

/cherry-pick 3d631914677b58a5479b310f480ac76e27d41e7e

---

### Comment 2 - llvmbot

/pull-request llvm/llvm-project#147034

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: AngusG (SyxtonPrime)

<details>
Cross post from: https://github.com/rust-lang/rust/issues/143399

Not sure if this is a Rust or LLVM issue but I think there is a decent chance this is an LLVM issue so wanted to post it here too.

Recently I have been running into some strange errors which seem to only occur on some architectures. See

https://github.com/Plonky3/Plonky3/issues/729
https://github.com/Plonky3/Plonky3/issues/905

For more info about the situation where these arose. I managed to make a minimal example in Godbolt so I thought it made sense to post this here.

### Code
Probably easier to understand in the following Godbolt link: https://godbolt.org/z/rrrj3eobd

Basically we define a simple function which adds integers mod a prime `P`. We then use this on vectors of length `4`. What you find is that if you check `(x + y) + z = x + (y + z)` manually, everything works as expected and they agree. If however we define a function which computes `(x + y) + z`, `x + (y + z)` and then checks that they are equal we get an error. I agree this seems totally bizarre, please check out the Godbolt.

```(rust)
  use std::array;
  
  /// The Baby Bear prime: 2^31 - 2^27 + 1.
  const P: u32 = 2013265921;
  // To help read the assembly, note: 2^32 - P = 2281701375
  
  /// Addition modulo P.
  ///
  /// Inputs are asusmed to be &lt; P.
  /// Assuming this, outputs will also be &lt; P.
  #[inline(always)]
  fn add(lhs: u32, rhs: u32) -&gt; u32 {
      let mut sum = lhs + rhs; // Never overflows as inputs are &lt; P &lt; 2^31.
      let (corr_sum, over) = sum.overflowing_sub(P);
       // over is false if sum &gt;= P and true if sum &lt; P.
      if !over { 
          sum = corr_sum;
      }
      sum
  }
  
  /// Addition modulo P in a degree 4 extension.
  ///
  /// Identical to 4 additions in parallel.
  #[unsafe(no_mangle)]
  pub fn add_bb_deg_4_ext(lhs: [u32; 4], rhs: [u32; 4]) -&gt; [u32; 4] {
      array::from_fn(|i| add(lhs[i], rhs[i]))
  }
  
  /// Compute (lhs + mid) + rhs
  #[unsafe(no_mangle)]
  pub fn add_bb_assoc_l(lhs: [u32; 4], mid: [u32; 4], rhs: [u32; 4]) -&gt; [u32; 4] {
      let lhs = add_bb_deg_4_ext(lhs, mid);
      add_bb_deg_4_ext(lhs, rhs)
  }
  
  /// Compute lhs + (mid + rhs)
  #[unsafe(no_mangle)]
  pub fn add_bb_assoc_r(lhs: [u32; 4], mid: [u32; 4], rhs: [u32; 4]) -&gt; [u32; 4] {
      let rhs = add_bb_deg_4_ext(mid, rhs);
      add_bb_deg_4_ext(lhs, rhs)
  }
  
  /// Check that (lhs + mid) + rhs = lhs + (mid + rhs)
  #[unsafe(no_mangle)]
  pub fn check_assoc(lhs: [u32; 4], mid: [u32; 4], rhs: [u32; 4]) {
      let assoc_l = add_bb_assoc_l(lhs, mid, rhs);
      let assoc_r = add_bb_assoc_r(lhs, mid, rhs);
      assert_eq!(assoc_l, assoc_r);
  }
  
  fn main() -&gt; () {
      let lhs = [252551971, 694974649, 213757600, 1325013984];
      let mid = [506156623, 97664653, 1234719014, 1349792299];
      let rhs = [1626423134, 1338438783, 786682629, 1311208151];
  
      // Check all elements are &lt; P
      assert!(lhs.iter().all(|&amp;x| x &lt; P));
      assert!(mid.iter().all(|&amp;x| x &lt; P));
      assert!(rhs.iter().all(|&amp;x| x &lt; P));
  
      // Let's manually check that
      // (lhs + mid) + rhs = lhs + (mid + rhs)
      let assoc_l = add_bb_assoc_l(lhs, mid, rhs);
      let assoc_r = add_bb_assoc_r(lhs, mid, rhs);
  
      // Check that (x + y) + z = x + (y + z)
      println!("{assoc_l:?}");
      println!("{assoc_r:?}");
      assert_eq!(assoc_l, assoc_r);
      println!("The two assoc's are equal");
      // Everything up until here runs.
  
      // Now let's check the same thing using our
      // function check_assoc
      check_assoc(lhs, mid, rhs);
      // This fails with compilation option:
      // -O -C target-cpu=znver4 -C opt-level=3
      // among others.
  } 
```


### Meta
&lt;!--
If you're using the stable version of the compiler, you should also check if the
bug also exists in the beta or nightly versions.
--&gt;
The error occurs when compiling with current rustc 1.880, beta and nightly. It seems to have been introduced in the move to `LLVM 20` as these errors do not appear on `nightly-2025-02-17` but things begin failing on `nightly-2025-02-18`.

The error occurs when using the compiler flags: `-O -C target-cpu=znver4 -C opt-level=3`.
It does not occur with `opt-level=0, 1, 2`, it does however occur with some other target cpus in particular `mic_avx512` and `znver5` but not with others such as `skylake, skylake_avx512, alderlake, raptorlake`. (If you want a complete list I can go through and check them all)



### Error output
See the Godbolt link for more details. Essentially what seems to be happening is that something goes wrong in the vectorization code? It's hard to say exactly what though. The compiled code for `check_assoc` seems to be reasonable? We are pretty lost as to what is going on.
</details>


---

