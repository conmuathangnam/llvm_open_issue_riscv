# Clang crashes generating IR for variable of type auto

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/41373
**Status:** Closed
**Labels:** clang:frontend, bugzilla, confirmed, crash
**Closed Date:** 2025-03-08T14:57:48Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [42028](https://llvm.org/bz42028) |
| Version | unspecified |
| OS | MacOS X |
| Attachments | [Crash backtrace](https://user-images.githubusercontent.com/60944935/143759271-e48383aa-5c65-4f62-813b-748b048cec3f.gz), [Testcase](https://user-images.githubusercontent.com/60944935/143759272-c6eeae96-de58-46d9-b60d-60bc4f84e9cf.gz), [Testcase(C++)](https://user-images.githubusercontent.com/60944935/143759273-81ebe39e-0c81-4795-8aab-db47ead33bcc.gz) |
| Reporter | LLVM Bugzilla Contributor |
| CC | @hfinkel,@rnk |

## Extended Description 


Compile the Code and metal compiler crash
```cpp
#include <metal_stdlib>
using namespace metal;

struct VertexOutput
{
    float4 gl_Position [[position]];
};

struct VertexContext
{
    constant float4x4& projection;
    constant float3& position;
    union
    {
        VertexOutput out;
        struct
        {
            float4 gl_Position;
        };
    };
    
    VertexOutput main()
    {
        out.gl_Position = float4(position, 1.0);
        return out;
    }
};

vertex VertexOutput vertexShader(constant float4x4& projection [[buffer(0)]],
                                 constant float3& position [[buffer(1)]])
{
    auto b = VertexContext
    {
        .position = position,
        .projection = projection,
    }.main();
    VertexOutput d = b;
    return d;
}
```

## Comments

### Comment 1 - llvmbot

> I confirmed the test case crashes upstream clang. Upstream LLVM doesn't
> support Metal, normally you would report such bugs to Apple, but since it
> affects clang, we can take a look.

clang(C++) crash while compiling the C++ code 
```cpp
struct foo
{
    float& a;
    float& b;
    
    float bar()
    {
        return b;
    }
};

int main(int argc, const char * argv[]) {
    float x = 0, y = 0;
    auto s = foo
    {
        .b = x,
        .a = y,
    }.bar();
    float t = s;
    return 0;
}
```

---

### Comment 2 - llvmbot

clang(C++) crash while compiling the C++ code 
```cpp
struct foo
{
    float& a;
    float& b;
    
    float bar()
    {
        return b;
    }
};

int main(int argc, const char * argv[]) {
    float x = 0, y = 0;
    auto s = foo
    {
        .b = x,
        .a = y,
    }.bar();
    float t = s;
    return 0;
}
```

---

### Comment 3 - rnk

I confirmed the test case crashes upstream clang. Upstream LLVM doesn't support Metal, normally you would report such bugs to Apple, but since it affects clang, we can take a look.

---

### Comment 4 - llvmbot

```cpp
struct foo
{
    float& a;
    float& b;
    
    float bar()
    {
        return b;
    }
};

int main(int argc, const char * argv[]) {
    float x = 0, y = 0;
    auto s = foo
    {
        .b = x,
        .a = y,
    }.bar();
    float t = s;
    return 0;
}
```

---

### Comment 5 - Endilll

Appears to be fixed in Clang 10: https://godbolt.org/z/fKdGT7hnq

---

### Comment 6 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (llvmbot)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [42028](https://llvm.org/bz42028) |
| Version | unspecified |
| OS | MacOS X |
| Attachments | [Crash backtrace](https://user-images.githubusercontent.com/60944935/143759271-e48383aa-5c65-4f62-813b-748b048cec3f.gz), [Testcase](https://user-images.githubusercontent.com/60944935/143759272-c6eeae96-de58-46d9-b60d-60bc4f84e9cf.gz), [Testcase(C++)](https://user-images.githubusercontent.com/60944935/143759273-81ebe39e-0c81-4795-8aab-db47ead33bcc.gz) |
| Reporter | LLVM Bugzilla Contributor |
| CC | @<!-- -->hfinkel,@<!-- -->rnk |

## Extended Description 


Compile the Code and metal compiler crash
```cpp
#include &lt;metal_stdlib&gt;
using namespace metal;

struct VertexOutput
{
    float4 gl_Position [[position]];
};

struct VertexContext
{
    constant float4x4&amp; projection;
    constant float3&amp; position;
    union
    {
        VertexOutput out;
        struct
        {
            float4 gl_Position;
        };
    };
    
    VertexOutput main()
    {
        out.gl_Position = float4(position, 1.0);
        return out;
    }
};

vertex VertexOutput vertexShader(constant float4x4&amp; projection [[buffer(0)]],
                                 constant float3&amp; position [[buffer(1)]])
{
    auto b = VertexContext
    {
        .position = position,
        .projection = projection,
    }.main();
    VertexOutput d = b;
    return d;
}
```
</details>


---

