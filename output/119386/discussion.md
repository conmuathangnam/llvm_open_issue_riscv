# x264 performance regression since 19.1.5 with rva22u64_v

**Author:** lukel97
**URL:** https://github.com/llvm/llvm-project/issues/119386

## Body

525.x264_r compiled with -O3 -flto -march=rva22u64_v is 15% slower compared to 19.1.5 on the spacemit-x60: https://lnt.lukelau.me/db_default/v4/nts/70?compare_to=69

The regression was introduced in #105858, which caused SLP to start vectorizing the horizontal reductions in `x264_pixel_satd_*`.

These end up producing several m4 vrgather.vvs, which are quadratically expensive. If you view the profiles on LNT, you'll find that `x264_pixel_satd_8x4` gets inlined and is then vectorized in e.g. `x264_pixel_satd_16x16`. You will see spikes in the u_mode_cycle counter around the vrgathers

From what I can tell, #105858 isn't doing anything wrong but just happens to improve the cost model enough such that it triggers the unprofitable vectorization. It's correctly adding the quadratic cost of the several two source permutation shuffles. 

We could try and disable the vectorization again by tweaking the cost of vrgather_vv to be more expensive: At lmul 4 we cost it at `1*4*4=16`, but on the spacemit-x60 this is closer to 64 because at LMUL 1 the reciprocal throughput is 4 according to https://camel-cdr.github.io/rvv-bench-results/bpi_f3/index.html. I'm not sure if this holds for other microarchitectures.

Alternatively, we could try and improve the codegen. These functions seem like they could be eventually profitably vectorized, given the relatively large VF=16. 

One idea is that one of the vrgather.vvs comes from this shuffle:

```llvm
%52 = shufflevector <16 x i32> %51, <16 x i32> poison, <16 x i32> <i32 3, i32 2, i32 1, i32 0, i32 7, i32 6, i32 5, i32 4, i32 11, i32 10, i32 9, i32 8, i32 15, i32 14, i32 13, i32 12>
```

This could be done as an adjacent element swap at e32, and then a second one at e64 i.e.:

```
     ->          ->
0           1              3
1           0              2
2           3              1
3           2              0
4           5	           7
5           4	           6
6           7	           5
7           6	           4
8           9	           11
9           8	           10
10          11	           9
11          10	           8
12          13	           15
13          12	           14
14          15	           13
15          14	           12
```

We can do an adjacent element swap at e32 with vror, and at e64 with a vslide1up + masked vslide1down. 

Finally, it's worth noting that GCC also vectorizes this but as several m1 vrgather.vvs. This is probably profitable because they're not quadratic: https://godbolt.org/z/av4vGqjsG. Is splitting up the reduction into several M1 sized sub-trees something SLP could feasibly do? cc @alexey-bataev 

The extracted kernel is below

```c
#include <stdint.h>
#include <math.h>

// in: a pseudo-simd number of the form x+(y<<16)
// return: abs(x)+(abs(y)<<16)
static inline uint32_t abs2( uint32_t a )
{
    uint32_t s = ((a>>15)&0x10001)*0xffff;
    return (a+s)^s;
}

#define HADAMARD4(d0, d1, d2, d3, s0, s1, s2, s3) {\
    int t0 = s0 + s1;\
    int t1 = s0 - s1;\
    int t2 = s2 + s3;\
    int t3 = s2 - s3;\
    d0 = t0 + t2;\
    d2 = t0 - t2;\
    d1 = t1 + t3;\
    d3 = t1 - t3;\
}

int x264_pixel_satd_8x4( uint8_t *pix1, int i_pix1, uint8_t *pix2, int i_pix2 )
{
    uint32_t tmp[4][4];
    uint32_t a0, a1, a2, a3;
    int sum = 0;
    for( int i = 0; i < 4; i++, pix1 += i_pix1, pix2 += i_pix2 )
    {
        a0 = (pix1[0] - pix2[0]) + ((pix1[4] - pix2[4]) << 16);
        a1 = (pix1[1] - pix2[1]) + ((pix1[5] - pix2[5]) << 16);
        a2 = (pix1[2] - pix2[2]) + ((pix1[6] - pix2[6]) << 16);
        a3 = (pix1[3] - pix2[3]) + ((pix1[7] - pix2[7]) << 16);
        HADAMARD4( tmp[i][0], tmp[i][1], tmp[i][2], tmp[i][3], a0,a1,a2,a3 );
    }
    for( int i = 0; i < 4; i++ )
    {
        HADAMARD4( a0, a1, a2, a3, tmp[0][i], tmp[1][i], tmp[2][i], tmp[3][i] );
        sum += abs2(a0) + abs2(a1) + abs2(a2) + abs2(a3);
    }
    return (((uint16_t)sum) + ((uint32_t)sum>>16)) >> 1;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Luke Lau (lukel97)

<details>
525.x264_r compiled with -O3 -flto -march=rva22u64_v is 15% slower compared to 19.1.5 on the spacemit-x60: https://lnt.lukelau.me/db_default/v4/nts/70?compare_to=69

The regression was introduced in #<!-- -->105858, which caused SLP to start vectorizing the horizontal reductions in `x264_pixel_satd_*`.

These end up producing several m4 vrgather.vvs, which are quadratically expensive. If you view the profiles on LNT, you'll find that `x264_pixel_satd_8x4` gets inlined and is then vectorized in e.g. `x264_pixel_satd_16x16`. You will see spikes in the u_mode_cycle counter around the vrgathers

From what I can tell, #<!-- -->105858 isn't doing anything wrong but just happens to improve the cost model enough such that it triggers the unprofitable vectorization. It's correctly adding the quadratic cost of the several two source permutation shuffles. 

We could try and disable the vectorization again by tweaking the cost of vrgather_vv to be more expensive: At lmul 4 we cost it at `1*4*4=16`, but on the spacemit-x60 this is closer to 64 because at LMUL 1 the reciprocal throughput is 4 according to https://camel-cdr.github.io/rvv-bench-results/bpi_f3/index.html. I'm not sure if this holds for other microarchitectures.

Alternatively, we could try and improve the codegen. These functions seem like they could be eventually profitably vectorized, given the relatively large VF=16. 

One idea is that one of the vrgather.vvs comes from this shuffle:

```llvm
%52 = shufflevector &lt;16 x i32&gt; %51, &lt;16 x i32&gt; poison, &lt;16 x i32&gt; &lt;i32 3, i32 2, i32 1, i32 0, i32 7, i32 6, i32 5, i32 4, i32 11, i32 10, i32 9, i32 8, i32 15, i32 14, i32 13, i32 12&gt;
```

This could be done as an adjacent element swap at e32, and then a second one at e64 i.e.:

```
     -&gt;          -&gt;
0           1              3
1           0              2
2           3              1
3           2              0
4           5	           7
5           4	           6
6           7	           5
7           6	           4
8           9	           11
9           8	           10
10          11	           9
11          10	           8
12          13	           15
13          12	           14
14          15	           13
15          14	           12
```

We can do an adjacent element swap at e32 with vror, and at e64 with a vslide1up + masked vslide1down. 

Finally, it's worth noting that GCC also vectorizes this but as several m1 vrgather.vvs. This is probably profitable because they're not quadratic: https://godbolt.org/z/av4vGqjsG. Is splitting up the reduction into several M1 sized sub-trees something SLP could feasibly do? cc @<!-- -->alexey-bataev 

The extracted kernel is below

```c
#include &lt;stdint.h&gt;
#include &lt;math.h&gt;

// in: a pseudo-simd number of the form x+(y&lt;&lt;16)
// return: abs(x)+(abs(y)&lt;&lt;16)
static inline uint32_t abs2( uint32_t a )
{
    uint32_t s = ((a&gt;&gt;15)&amp;0x10001)*0xffff;
    return (a+s)^s;
}

#define HADAMARD4(d0, d1, d2, d3, s0, s1, s2, s3) {\
    int t0 = s0 + s1;\
    int t1 = s0 - s1;\
    int t2 = s2 + s3;\
    int t3 = s2 - s3;\
    d0 = t0 + t2;\
    d2 = t0 - t2;\
    d1 = t1 + t3;\
    d3 = t1 - t3;\
}

int x264_pixel_satd_8x4( uint8_t *pix1, int i_pix1, uint8_t *pix2, int i_pix2 )
{
    uint32_t tmp[4][4];
    uint32_t a0, a1, a2, a3;
    int sum = 0;
    for( int i = 0; i &lt; 4; i++, pix1 += i_pix1, pix2 += i_pix2 )
    {
        a0 = (pix1[0] - pix2[0]) + ((pix1[4] - pix2[4]) &lt;&lt; 16);
        a1 = (pix1[1] - pix2[1]) + ((pix1[5] - pix2[5]) &lt;&lt; 16);
        a2 = (pix1[2] - pix2[2]) + ((pix1[6] - pix2[6]) &lt;&lt; 16);
        a3 = (pix1[3] - pix2[3]) + ((pix1[7] - pix2[7]) &lt;&lt; 16);
        HADAMARD4( tmp[i][0], tmp[i][1], tmp[i][2], tmp[i][3], a0,a1,a2,a3 );
    }
    for( int i = 0; i &lt; 4; i++ )
    {
        HADAMARD4( a0, a1, a2, a3, tmp[0][i], tmp[1][i], tmp[2][i], tmp[3][i] );
        sum += abs2(a0) + abs2(a1) + abs2(a2) + abs2(a3);
    }
    return (((uint16_t)sum) + ((uint32_t)sum&gt;&gt;16)) &gt;&gt; 1;
}
```
</details>


---

### Comment 2 - alexey-bataev

Somethink like https://github.com/llvm/llvm-project/pull/118293 may help, I assume. It is not ready yet for x264, the follow-up patch may try to improve.

---

