# [bug][opt][AMDGPU]At the -O2 optimization level, the division computation result for uint64_t type is incorrect

**Author:** 0oyyo0
**URL:** https://github.com/llvm/llvm-project/issues/151676
**Status:** Closed
**Labels:** backend:AMDGPU, miscompilation, llvm:optimizations
**Closed Date:** 2025-08-18T11:45:06Z

## Body

①I tested the following code on gfx906 and found that the results were not as expected. The core of this code is to check whether the input string can be safely converted to a uint64_t integer.
②When compiled using the following command, the calculation result is incorrect. The cause of the error is located in the incorrect calculation result of the line "a2 / uint64_t{10}".
`hipcc demo.cpp -o demo -std=c++17 -O2`

<img width="1151" height="305" alt="Image" src="https://github.com/user-attachments/assets/dc1689e2-9826-455d-adf1-dc851cb5d6b4" />


③When DivExpand is disabled by adding the "disable-idiv-expansion" option, the calculation result is correct.
`hipcc demo.cpp -o demo -std=c++17 -O2 -mllvm -amdgpu-codegenprepare-disable-idiv-expansion=true`

<img width="1348" height="561" alt="Image" src="https://github.com/user-attachments/assets/8d64022c-1e13-44ca-a13f-761fadc79d31" />


④Current analysis: In the `amdgpu-codegenprepare` pass, `udiv` is converted to `fmul`.
⑤Please advise: What is the specific role of the expandDivRem32 function in the amdgpu-codegenprepare pass? Why does using it lead to incorrect computation results?


```
#include <hip/hip_runtime.h>
#include <iostream>
#include <limits>
#include <string>
#include <type_traits>

__device__ bool check_string(const char* d_str, size_t len) {
    auto iter = d_str;
    auto const iter_end = d_str + len;

    auto const bound_val = std::numeric_limits<uint64_t>::max();
    uint64_t value = 0;

    while (iter != iter_end) {
      auto const chr = *iter++;
      if (chr < '0' || chr > '9') { return false; }

      auto const digit = static_cast<uint64_t>(chr - '0');

      // auto const bound_check = (bound_val - sign * digit) / uint64_t{10} * sign;
      uint64_t const a1 = digit;
      uint64_t const a2 = bound_val - a1;
      uint64_t const bound_check = a2 / uint64_t{10};
      
      printf("bound_val: %llu, digit: %llu, value: %llu, bound_check: %llu\n", bound_val, digit, value, bound_check);

      if (value > bound_check) {
        return false;
      }

      value = value * uint64_t{10} + digit;
    }

    return true;
  
}

__global__ void check_string_kernel(const char* str, size_t len, bool* result) {
    if (threadIdx.x == 0 && blockIdx.x == 0) {
        result[0] = check_string(str, len);
    }
}

int main() {
    std::string input = "9223372036854775806";
    
    char* d_str;
    bool* d_result;
    hipMalloc(&d_str, input.size());
    hipMalloc(&d_result, sizeof(bool));
    
    hipMemcpy(d_str, input.c_str(), input.size(), hipMemcpyHostToDevice);
    
    check_string_kernel<<<1, 256>>>(d_str, input.size(), d_result);
    hipDeviceSynchronize();
    
    bool h_result;
    hipMemcpy(&h_result, d_result, sizeof(bool), hipMemcpyDeviceToHost);
    
    std::cout << (h_result ? "check passed" : "Got 'false' when checking if \"9223372036854775806\" is a uint64_t. Expect 'true'.") << std::endl;
    
    hipFree(d_str);
    hipFree(d_result);
    
    return 0;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-amdgpu

Author: Helium (0oyyo0)

<details>
①I tested the following code on gfx906 and found that the results were not as expected. The core of this code is to check whether the input string can be safely converted to a uint64_t integer.
②When compiled using the following command, the calculation result is incorrect. The cause of the error is located in the incorrect calculation result of the line "a2 / IntegerType{10}".
`hipcc demo.cpp -o demo -std=c++17 -O2`

&lt;img width="1151" height="305" alt="Image" src="https://github.com/user-attachments/assets/dc1689e2-9826-455d-adf1-dc851cb5d6b4" /&gt;


③When DivExpand is disabled by adding the "disable-idiv-expansion" option, the calculation result is correct.
`hipcc demo.cpp -o demo -std=c++17 -O2 -mllvm -amdgpu-codegenprepare-disable-idiv-expansion=true`

&lt;img width="1348" height="561" alt="Image" src="https://github.com/user-attachments/assets/8d64022c-1e13-44ca-a13f-761fadc79d31" /&gt;


④Current analysis: In the `amdgpu-codegenprepare` pass, `udiv` is converted to `fmul`.
⑤Please advise: What is the specific role of the expandDivRem32 function in the amdgpu-codegenprepare pass? Why does using it lead to incorrect computation results?


```
#include &lt;hip/hip_runtime.h&gt;
#include &lt;iostream&gt;
#include &lt;limits&gt;
#include &lt;string&gt;
#include &lt;type_traits&gt;

template &lt;typename IntegerType&gt;
struct string_to_integer_check {
  const char* d_str;
  std::size_t n;

  __device__ bool operator()(int)
  {
    if (d_str[0] == '-' &amp;&amp; std::is_unsigned_v&lt;IntegerType&gt;) { return false; }

    auto iter           = d_str + static_cast&lt;int&gt;((d_str[0] == '-' || d_str[0] == '+'));
    auto const iter_end = d_str + n;
    if (iter == iter_end) { return false; }

    // printf("hhh\n");

    auto const sign = d_str[0] == '-' ? static_cast&lt;IntegerType&gt;(-1) : IntegerType{1};
    auto const bound_val =
      sign &gt; 0 ? std::numeric_limits&lt;IntegerType&gt;::max() : std::numeric_limits&lt;IntegerType&gt;::min();

    IntegerType value = 0;
    while (iter != iter_end) {
      auto const chr = *iter++;
      if (chr &lt; '0' || chr &gt; '9') { return false; }

      auto const digit       = static_cast&lt;IntegerType&gt;(chr - '0');

      // auto const bound_check = (bound_val - sign * digit) / IntegerType{10} * sign;
      IntegerType const a1 = sign * digit;
      IntegerType const a2 = bound_val - a1;
      IntegerType const a3 = a2 / IntegerType{10};

      IntegerType const bound_check = a3 * sign;
      // printf("value: %llu, bound_check: %llu, digit: %llu,bound_val: %llu, sign: %llu\n", value,bound_check,digit, bound_val, sign);

      if (value &gt; bound_check) {
        return false;
      }

      value = value * IntegerType{10} + digit;
    }

    return true;
  }
};

template&lt;typename IntegerType&gt;
__global__ void check_string_kernel(const char* d_str, std::size_t str_len, bool* result)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (idx == 0) {
        string_to_integer_check&lt;IntegerType&gt; checker{d_str, str_len};
        result[0] = checker(0);
    }
}

int main()
{
    std::string int_str("9223372036854775806");
    
    char* d_str;
    bool* d_result;
    hipMalloc(&amp;d_str, int_str.size());
    hipMalloc(&amp;d_result, sizeof(bool));
    
    hipMemcpy(d_str, int_str.c_str(), int_str.size(), hipMemcpyHostToDevice);
    
    dim3 blockSize(256);
    dim3 gridSize(1);

    check_string_kernel&lt;uint64_t&gt;&lt;&lt;&lt;gridSize, blockSize&gt;&gt;&gt;(d_str, int_str.size(), d_result);
    
    hipDeviceSynchronize();
    
    bool h_result;
    hipMemcpy(&amp;h_result, d_result, sizeof(bool), hipMemcpyDeviceToHost);
    
    if (!h_result) {
        std::cout &lt;&lt; "Got 'false' when check if \"9223372036854775806\" is a uint64_t. Expect 'true'." &lt;&lt; std::endl;
    } else {
        std::cout &lt;&lt; "check passed" &lt;&lt; std::endl;
    }
    
    hipFree(d_str);
    hipFree(d_result);
    
    return 0;
}
```
</details>


---

### Comment 2 - frederik-h

> ①I tested the following code on gfx906 and found that the results were not as expected. The core of this code is to check whether the input string can be safely converted to a uint64_t integer. ②When compiled using the following command, the calculation result is incorrect. The cause of the error is located in the incorrect calculation result of the line "a2 / IntegerType{10}". `hipcc demo.cpp -o demo -std=c++17 -O2`

It would be helpful to reduce the example as far as possible. There seems to be a lot of code than cannot be related to your problem (e.g. handling '-' although your input does not contain it; and why does your `operator()` not use its argument?). I suggest to strip it away. This will help others to figure out if this is a compiler issue.

---

### Comment 3 - arsenm

Was this the issue fixed by 8d2e611802d5f3bdd681d308ceb293e5ace8a894? 

---

### Comment 4 - 0oyyo0

> > ①I tested the following code on gfx906 and found that the results were not as expected. The core of this code is to check whether the input string can be safely converted to a uint64_t integer. ②When compiled using the following command, the calculation result is incorrect. The cause of the error is located in the incorrect calculation result of the line "a2 / IntegerType{10}". `hipcc demo.cpp -o demo -std=c++17 -O2`
> 
> It would be helpful to reduce the example as far as possible. There seems to be a lot of code than cannot be related to your problem (e.g. handling '-' although your input does not contain it; and why does your `operator()` not use its argument?). I suggest to strip it away. This will help others to figure out if this is a compiler issue.

Redundant code has been deleted

---

### Comment 5 - 0oyyo0

> Was this the issue fixed by [8d2e611](https://github.com/llvm/llvm-project/commit/8d2e611802d5f3bdd681d308ceb293e5ace8a894)?

It doesn't look like the same issue. I tried to modify the code, but the problem still exists

---

### Comment 6 - frederik-h

> > > ①I tested the following code on gfx906 and found that the results were not as expected. The core of this code is to check whether the input string can be safely converted to a uint64_t integer. ②When compiled using the following command, the calculation result is incorrect. The cause of the error is located in the incorrect calculation result of the line "a2 / IntegerType{10}". `hipcc demo.cpp -o demo -std=c++17 -O2`
> > 
> > 
> > It would be helpful to reduce the example as far as possible. There seems to be a lot of code than cannot be related to your problem (e.g. handling '-' although your input does not contain it; and why does your `operator()` not use its argument?). I suggest to strip it away. This will help others to figure out if this is a compiler issue.
> 
> Redundant code has been deleted

Thank you! And thanks for the reporting issue. I can reproduce it on rocm 6.3. @arsenm On rocm 6.4, I don't get the error.

---

### Comment 7 - 0oyyo0

> > > > ①I tested the following code on gfx906 and found that the results were not as expected. The core of this code is to check whether the input string can be safely converted to a uint64_t integer. ②When compiled using the following command, the calculation result is incorrect. The cause of the error is located in the incorrect calculation result of the line "a2 / IntegerType{10}". `hipcc demo.cpp -o demo -std=c++17 -O2`
> > > 
> > > 
> > > It would be helpful to reduce the example as far as possible. There seems to be a lot of code than cannot be related to your problem (e.g. handling '-' although your input does not contain it; and why does your `operator()` not use its argument?). I suggest to strip it away. This will help others to figure out if this is a compiler issue.
> > 
> > 
> > Redundant code has been deleted
> 
> Thank you! And thanks for the reporting issue. I can reproduce it on rocm 6.3. [@arsenm](https://github.com/arsenm) On rocm 6.4, I don't get the error.

Thanks. The commit that fixed this issue was found in rocm6.4.2, https://github.com/ROCm/llvm-project/commit/e5638c5a00682243b1ee012d7dd8292aa221dff8#diff-bd14c07380de930aa1b564d8403ff604929e667e23aa803e0ae09071b93e7037  and  https://github.com/ROCm/llvm-project/commit/edf2376bca3ac3409b61bded1d231a0112e3eeea#diff-f41381955a38b43c81e4f973016801d8fa5cfb1800f1e6427dd6d2f062e5e932

---

