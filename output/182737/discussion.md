# Segmentation fault when dumping AST of wrong C doxygen comment

**Author:** cybernetlab
**URL:** https://github.com/llvm/llvm-project/issues/182737
**Status:** Closed
**Labels:** good first issue, clang:frontend, confirmed, crash
**Closed Date:** 2026-03-10T11:40:38Z

## Body

Originally I tried to parse headers of [ESP Zigbee library](https://github.com/espressif/esp-zigbee-sdk) and clang crashes on [this file (line104)](https://github.com/espressif/esp-zigbee-sdk/blob/main/components/esp-zigbee-lib/include/platform/esp_zigbee_platform.h#L104). After some investigation I found that if I remove both `@param` lines from doxygen comment before that line everything is fine. I see that actually this comment is invalid cause it describes typedef for function but not an actual function declaration and `@param` shouldn't be used there. Anyway I believe that AST parser shouldn't crash with segfault on such mistakes and should generate warn or error instead.

## Reproduce

test file `test.h`:

```c
/**
 * @brief A callback
 *
 * @param[in] a param1
 * @return
 *      - true: ok
 *      - false: failure
 */
typedef bool (*func_t)(uint8_t a);
```

command:

```sh
clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics test.h
```

<details>
<summary>output</summary>
```
platform/test.h:9:16: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
    9 | typedef bool (*func_t)(uint8_t a);
      |                ^
      |               int
platform/test.h:9:24: error: unknown type name 'uint8_t'
    9 | typedef bool (*func_t)(uint8_t a);
      |                        ^
platform/test.h:9:9: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
    9 | typedef bool (*func_t)(uint8_t a);
      | ~~~~~~~ ^
      | int
platform/test.h:9:14: error: function cannot return function type 'int (int)'
    9 | typedef bool (*func_t)(uint8_t a);
      |              ^
TranslationUnitDecl 0xcbf156008 <<invalid sloc>> <invalid sloc>
|-TypedefDecl 0xcbf1f2088 <<invalid sloc>> <invalid sloc> implicit __int128_t '__int128'
| `-BuiltinType 0xcbf1565d0 '__int128'
|-TypedefDecl 0xcbf1f20f8 <<invalid sloc>> <invalid sloc> implicit __uint128_t 'unsigned __int128'
| `-BuiltinType 0xcbf1565f0 'unsigned __int128'
|-TypedefDecl 0xcbf1f2400 <<invalid sloc>> <invalid sloc> implicit __NSConstantString 'struct __NSConstantString_tag'
| `-RecordType 0xcbf1f21d0 'struct __NSConstantString_tag'
|   `-Record 0xcbf1f2150 '__NSConstantString_tag'
|-TypedefDecl 0xcbf1f24a8 <<invalid sloc>> <invalid sloc> implicit __Int8x8_t '__attribute__((neon_vector_type(8))) signed char'
| `-VectorType 0xcbf1f2460 '__attribute__((neon_vector_type(8))) signed char' neon 8
|   `-BuiltinType 0xcbf1560d0 'signed char'
|-TypedefDecl 0xcbf1f2548 <<invalid sloc>> <invalid sloc> implicit __Int16x4_t '__attribute__((neon_vector_type(4))) short'
| `-VectorType 0xcbf1f2500 '__attribute__((neon_vector_type(4))) short' neon 4
|   `-BuiltinType 0xcbf1560f0 'short'
|-TypedefDecl 0xcbf1f25e8 <<invalid sloc>> <invalid sloc> implicit __Int32x2_t '__attribute__((neon_vector_type(2))) int'
| `-VectorType 0xcbf1f25a0 '__attribute__((neon_vector_type(2))) int' neon 2
|   `-BuiltinType 0xcbf156110 'int'
|-TypedefDecl 0xcbf1f2688 <<invalid sloc>> <invalid sloc> implicit __Uint8x8_t '__attribute__((neon_vector_type(8))) unsigned char'
| `-VectorType 0xcbf1f2640 '__attribute__((neon_vector_type(8))) unsigned char' neon 8
|   `-BuiltinType 0xcbf156170 'unsigned char'
|-TypedefDecl 0xcbf1f2728 <<invalid sloc>> <invalid sloc> implicit __Uint16x4_t '__attribute__((neon_vector_type(4))) unsigned short'
| `-VectorType 0xcbf1f26e0 '__attribute__((neon_vector_type(4))) unsigned short' neon 4
|   `-BuiltinType 0xcbf156190 'unsigned short'
|-TypedefDecl 0xcbf1f27c8 <<invalid sloc>> <invalid sloc> implicit __Uint32x2_t '__attribute__((neon_vector_type(2))) unsigned int'
| `-VectorType 0xcbf1f2780 '__attribute__((neon_vector_type(2))) unsigned int' neon 2
|   `-BuiltinType 0xcbf1561b0 'unsigned int'
|-TypedefDecl 0xcbf1f2868 <<invalid sloc>> <invalid sloc> implicit __Float16x4_t '__attribute__((neon_vector_type(4))) __fp16'
| `-VectorType 0xcbf1f2820 '__attribute__((neon_vector_type(4))) __fp16' neon 4
|   `-BuiltinType 0xcbf156f20 '__fp16'
|-TypedefDecl 0xcbf1f2908 <<invalid sloc>> <invalid sloc> implicit __Float32x2_t '__attribute__((neon_vector_type(2))) float'
| `-VectorType 0xcbf1f28c0 '__attribute__((neon_vector_type(2))) float' neon 2
|   `-BuiltinType 0xcbf156210 'float'
|-TypedefDecl 0xcbf1f29a8 <<invalid sloc>> <invalid sloc> implicit __Poly8x8_t '__attribute__((neon_polyvector_type(8))) unsigned char'
| `-VectorType 0xcbf1f2960 '__attribute__((neon_polyvector_type(8))) unsigned char' neon poly 8
|   `-BuiltinType 0xcbf156170 'unsigned char'
|-TypedefDecl 0xcbf1f2a48 <<invalid sloc>> <invalid sloc> implicit __Poly16x4_t '__attribute__((neon_polyvector_type(4))) unsigned short'
| `-VectorType 0xcbf1f2a00 '__attribute__((neon_polyvector_type(4))) unsigned short' neon poly 4
|   `-BuiltinType 0xcbf156190 'unsigned short'
|-TypedefDecl 0xcbf1f2ae8 <<invalid sloc>> <invalid sloc> implicit __Bfloat16x4_t '__attribute__((neon_vector_type(4))) __bf16'
| `-VectorType 0xcbf1f2aa0 '__attribute__((neon_vector_type(4))) __bf16' neon 4
|   `-BuiltinType 0xcbf156f40 '__bf16'
|-TypedefDecl 0xcbf1f2b88 <<invalid sloc>> <invalid sloc> implicit __Int8x16_t '__attribute__((neon_vector_type(16))) signed char'
| `-VectorType 0xcbf1f2b40 '__attribute__((neon_vector_type(16))) signed char' neon 16
|   `-BuiltinType 0xcbf1560d0 'signed char'
|-TypedefDecl 0xcbf1f2c28 <<invalid sloc>> <invalid sloc> implicit __Int16x8_t '__attribute__((neon_vector_type(8))) short'
| `-VectorType 0xcbf1f2be0 '__attribute__((neon_vector_type(8))) short' neon 8
|   `-BuiltinType 0xcbf1560f0 'short'
|-TypedefDecl 0xcbf1f2cc8 <<invalid sloc>> <invalid sloc> implicit __Int32x4_t '__attribute__((neon_vector_type(4))) int'
| `-VectorType 0xcbf1f2c80 '__attribute__((neon_vector_type(4))) int' neon 4
|   `-BuiltinType 0xcbf156110 'int'
|-TypedefDecl 0xcbf1f2d68 <<invalid sloc>> <invalid sloc> implicit __Int64x2_t '__attribute__((neon_vector_type(2))) long'
| `-VectorType 0xcbf1f2d20 '__attribute__((neon_vector_type(2))) long' neon 2
|   `-BuiltinType 0xcbf156130 'long'
|-TypedefDecl 0xcbf1f2e08 <<invalid sloc>> <invalid sloc> implicit __Uint8x16_t '__attribute__((neon_vector_type(16))) unsigned char'
| `-VectorType 0xcbf1f2dc0 '__attribute__((neon_vector_type(16))) unsigned char' neon 16
|   `-BuiltinType 0xcbf156170 'unsigned char'
|-TypedefDecl 0xcbf1f2ea8 <<invalid sloc>> <invalid sloc> implicit __Uint16x8_t '__attribute__((neon_vector_type(8))) unsigned short'
| `-VectorType 0xcbf1f2e60 '__attribute__((neon_vector_type(8))) unsigned short' neon 8
|   `-BuiltinType 0xcbf156190 'unsigned short'
|-TypedefDecl 0xcbf1f2f48 <<invalid sloc>> <invalid sloc> implicit __Uint32x4_t '__attribute__((neon_vector_type(4))) unsigned int'
| `-VectorType 0xcbf1f2f00 '__attribute__((neon_vector_type(4))) unsigned int' neon 4
|   `-BuiltinType 0xcbf1561b0 'unsigned int'
|-TypedefDecl 0xcbf1f3000 <<invalid sloc>> <invalid sloc> implicit __Uint64x2_t '__attribute__((neon_vector_type(2))) unsigned long'
| `-VectorType 0xcbf1f2fa0 '__attribute__((neon_vector_type(2))) unsigned long' neon 2
|   `-BuiltinType 0xcbf1561d0 'unsigned long'
|-TypedefDecl 0xcbf1f30a8 <<invalid sloc>> <invalid sloc> implicit __Float16x8_t '__attribute__((neon_vector_type(8))) __fp16'
| `-VectorType 0xcbf1f3060 '__attribute__((neon_vector_type(8))) __fp16' neon 8
|   `-BuiltinType 0xcbf156f20 '__fp16'
|-TypedefDecl 0xcbf1f3148 <<invalid sloc>> <invalid sloc> implicit __Float32x4_t '__attribute__((neon_vector_type(4))) float'
| `-VectorType 0xcbf1f3100 '__attribute__((neon_vector_type(4))) float' neon 4
|   `-BuiltinType 0xcbf156210 'float'
|-TypedefDecl 0xcbf1f31e8 <<invalid sloc>> <invalid sloc> implicit __Float64x2_t '__attribute__((neon_vector_type(2))) double'
| `-VectorType 0xcbf1f31a0 '__attribute__((neon_vector_type(2))) double' neon 2
|   `-BuiltinType 0xcbf156230 'double'
|-TypedefDecl 0xcbf1f3288 <<invalid sloc>> <invalid sloc> implicit __Poly8x16_t '__attribute__((neon_polyvector_type(16))) unsigned char'
| `-VectorType 0xcbf1f3240 '__attribute__((neon_polyvector_type(16))) unsigned char' neon poly 16
|   `-BuiltinType 0xcbf156170 'unsigned char'
|-TypedefDecl 0xcbf1f3328 <<invalid sloc>> <invalid sloc> implicit __Poly16x8_t '__attribute__((neon_polyvector_type(8))) unsigned short'
| `-VectorType 0xcbf1f32e0 '__attribute__((neon_polyvector_type(8))) unsigned short' neon poly 8
|   `-BuiltinType 0xcbf156190 'unsigned short'
|-TypedefDecl 0xcbf1f33c8 <<invalid sloc>> <invalid sloc> implicit __Poly64x2_t '__attribute__((neon_polyvector_type(2))) unsigned long'
| `-VectorType 0xcbf1f3380 '__attribute__((neon_polyvector_type(2))) unsigned long' neon poly 2
|   `-BuiltinType 0xcbf1561d0 'unsigned long'
|-TypedefDecl 0xcbf1f3468 <<invalid sloc>> <invalid sloc> implicit __Bfloat16x8_t '__attribute__((neon_vector_type(8))) __bf16'
| `-VectorType 0xcbf1f3420 '__attribute__((neon_vector_type(8))) __bf16' neon 8
|   `-BuiltinType 0xcbf156f40 '__bf16'
|-TypedefDecl 0xcbf1f3508 <<invalid sloc>> <invalid sloc> implicit __Mfloat8x8_t '__attribute__((neon_vector_type(8))) __mfp8'
| `-VectorType 0xcbf1f34c0 '__attribute__((neon_vector_type(8))) __mfp8' neon 8
|   `-BuiltinType 0xcbf156eb0 '__mfp8'
|-TypedefDecl 0xcbf1f35a8 <<invalid sloc>> <invalid sloc> implicit __Mfloat8x16_t '__attribute__((neon_vector_type(16))) __mfp8'
| `-VectorType 0xcbf1f3560 '__attribute__((neon_vector_type(16))) __mfp8' neon 16
|   `-BuiltinType 0xcbf156eb0 '__mfp8'
|-TypedefDecl 0xcbf1f3610 <<invalid sloc>> <invalid sloc> implicit __SVInt8_t '__SVInt8_t'
| `-BuiltinType 0xcbf1567b0 '__SVInt8_t'
|-TypedefDecl 0xcbf1f3678 <<invalid sloc>> <invalid sloc> implicit __SVInt16_t '__SVInt16_t'
| `-BuiltinType 0xcbf1567d0 '__SVInt16_t'
|-TypedefDecl 0xcbf1f36e0 <<invalid sloc>> <invalid sloc> implicit __SVInt32_t '__SVInt32_t'
| `-BuiltinType 0xcbf1567f0 '__SVInt32_t'
|-TypedefDecl 0xcbf1f3748 <<invalid sloc>> <invalid sloc> implicit __SVInt64_t '__SVInt64_t'
| `-BuiltinType 0xcbf156810 '__SVInt64_t'
|-TypedefDecl 0xcbf1f37b0 <<invalid sloc>> <invalid sloc> implicit __SVUint8_t '__SVUint8_t'
| `-BuiltinType 0xcbf156830 '__SVUint8_t'
|-TypedefDecl 0xcbf1f3818 <<invalid sloc>> <invalid sloc> implicit __SVUint16_t '__SVUint16_t'
| `-BuiltinType 0xcbf156850 '__SVUint16_t'
|-TypedefDecl 0xcbf1f3880 <<invalid sloc>> <invalid sloc> implicit __SVUint32_t '__SVUint32_t'
| `-BuiltinType 0xcbf156870 '__SVUint32_t'
|-TypedefDecl 0xcbf1f38e8 <<invalid sloc>> <invalid sloc> implicit __SVUint64_t '__SVUint64_t'
| `-BuiltinType 0xcbf156890 '__SVUint64_t'
|-TypedefDecl 0xcbf1f3950 <<invalid sloc>> <invalid sloc> implicit __SVFloat16_t '__SVFloat16_t'
| `-BuiltinType 0xcbf1568b0 '__SVFloat16_t'
|-TypedefDecl 0xcbf1f39b8 <<invalid sloc>> <invalid sloc> implicit __SVFloat32_t '__SVFloat32_t'
| `-BuiltinType 0xcbf1568d0 '__SVFloat32_t'
|-TypedefDecl 0xcbf1f3a20 <<invalid sloc>> <invalid sloc> implicit __SVFloat64_t '__SVFloat64_t'
| `-BuiltinType 0xcbf1568f0 '__SVFloat64_t'
|-TypedefDecl 0xcbf1f3a88 <<invalid sloc>> <invalid sloc> implicit __SVBfloat16_t '__SVBfloat16_t'
| `-BuiltinType 0xcbf156910 '__SVBfloat16_t'
|-TypedefDecl 0xcbf1f3af0 <<invalid sloc>> <invalid sloc> implicit __SVMfloat8_t '__SVMfloat8_t'
| `-BuiltinType 0xcbf156930 '__SVMfloat8_t'
|-TypedefDecl 0xcbf1f3b58 <<invalid sloc>> <invalid sloc> implicit __clang_svint8x2_t '__clang_svint8x2_t'
| `-BuiltinType 0xcbf156950 '__clang_svint8x2_t'
|-TypedefDecl 0xcbf1f3bc0 <<invalid sloc>> <invalid sloc> implicit __clang_svint16x2_t '__clang_svint16x2_t'
| `-BuiltinType 0xcbf156970 '__clang_svint16x2_t'
|-TypedefDecl 0xcbf1f3c28 <<invalid sloc>> <invalid sloc> implicit __clang_svint32x2_t '__clang_svint32x2_t'
| `-BuiltinType 0xcbf156990 '__clang_svint32x2_t'
|-TypedefDecl 0xcbf1f3c90 <<invalid sloc>> <invalid sloc> implicit __clang_svint64x2_t '__clang_svint64x2_t'
| `-BuiltinType 0xcbf1569b0 '__clang_svint64x2_t'
|-TypedefDecl 0xcbf1f3cf8 <<invalid sloc>> <invalid sloc> implicit __clang_svuint8x2_t '__clang_svuint8x2_t'
| `-BuiltinType 0xcbf1569d0 '__clang_svuint8x2_t'
|-TypedefDecl 0xcbf1f3d60 <<invalid sloc>> <invalid sloc> implicit __clang_svuint16x2_t '__clang_svuint16x2_t'
| `-BuiltinType 0xcbf1569f0 '__clang_svuint16x2_t'
|-TypedefDecl 0xcbf1f3dc8 <<invalid sloc>> <invalid sloc> implicit __clang_svuint32x2_t '__clang_svuint32x2_t'
| `-BuiltinType 0xcbf156a10 '__clang_svuint32x2_t'
|-TypedefDecl 0xcbf1f3e30 <<invalid sloc>> <invalid sloc> implicit __clang_svuint64x2_t '__clang_svuint64x2_t'
| `-BuiltinType 0xcbf156a30 '__clang_svuint64x2_t'
|-TypedefDecl 0xcbf1f3e98 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat16x2_t '__clang_svfloat16x2_t'
| `-BuiltinType 0xcbf156a50 '__clang_svfloat16x2_t'
|-TypedefDecl 0xcbf1f3f00 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat32x2_t '__clang_svfloat32x2_t'
| `-BuiltinType 0xcbf156a70 '__clang_svfloat32x2_t'
|-TypedefDecl 0xcbf1f3f68 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat64x2_t '__clang_svfloat64x2_t'
| `-BuiltinType 0xcbf156a90 '__clang_svfloat64x2_t'
|-TypedefDecl 0xcbf274000 <<invalid sloc>> <invalid sloc> implicit __clang_svbfloat16x2_t '__clang_svbfloat16x2_t'
| `-BuiltinType 0xcbf156ab0 '__clang_svbfloat16x2_t'
|-TypedefDecl 0xcbf274068 <<invalid sloc>> <invalid sloc> implicit __clang_svmfloat8x2_t '__clang_svmfloat8x2_t'
| `-BuiltinType 0xcbf156ad0 '__clang_svmfloat8x2_t'
|-TypedefDecl 0xcbf2740d0 <<invalid sloc>> <invalid sloc> implicit __clang_svint8x3_t '__clang_svint8x3_t'
| `-BuiltinType 0xcbf156af0 '__clang_svint8x3_t'
|-TypedefDecl 0xcbf274138 <<invalid sloc>> <invalid sloc> implicit __clang_svint16x3_t '__clang_svint16x3_t'
| `-BuiltinType 0xcbf156b10 '__clang_svint16x3_t'
|-TypedefDecl 0xcbf2741a0 <<invalid sloc>> <invalid sloc> implicit __clang_svint32x3_t '__clang_svint32x3_t'
| `-BuiltinType 0xcbf156b30 '__clang_svint32x3_t'
|-TypedefDecl 0xcbf274208 <<invalid sloc>> <invalid sloc> implicit __clang_svint64x3_t '__clang_svint64x3_t'
| `-BuiltinType 0xcbf156b50 '__clang_svint64x3_t'
|-TypedefDecl 0xcbf274270 <<invalid sloc>> <invalid sloc> implicit __clang_svuint8x3_t '__clang_svuint8x3_t'
| `-BuiltinType 0xcbf156b70 '__clang_svuint8x3_t'
|-TypedefDecl 0xcbf2742d8 <<invalid sloc>> <invalid sloc> implicit __clang_svuint16x3_t '__clang_svuint16x3_t'
| `-BuiltinType 0xcbf156b90 '__clang_svuint16x3_t'
|-TypedefDecl 0xcbf274340 <<invalid sloc>> <invalid sloc> implicit __clang_svuint32x3_t '__clang_svuint32x3_t'
| `-BuiltinType 0xcbf156bb0 '__clang_svuint32x3_t'
|-TypedefDecl 0xcbf2743a8 <<invalid sloc>> <invalid sloc> implicit __clang_svuint64x3_t '__clang_svuint64x3_t'
| `-BuiltinType 0xcbf156bd0 '__clang_svuint64x3_t'
|-TypedefDecl 0xcbf274410 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat16x3_t '__clang_svfloat16x3_t'
| `-BuiltinType 0xcbf156bf0 '__clang_svfloat16x3_t'
|-TypedefDecl 0xcbf274478 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat32x3_t '__clang_svfloat32x3_t'
| `-BuiltinType 0xcbf156c10 '__clang_svfloat32x3_t'
|-TypedefDecl 0xcbf2744e0 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat64x3_t '__clang_svfloat64x3_t'
| `-BuiltinType 0xcbf156c30 '__clang_svfloat64x3_t'
|-TypedefDecl 0xcbf274548 <<invalid sloc>> <invalid sloc> implicit __clang_svbfloat16x3_t '__clang_svbfloat16x3_t'
| `-BuiltinType 0xcbf156c50 '__clang_svbfloat16x3_t'
|-TypedefDecl 0xcbf2745b0 <<invalid sloc>> <invalid sloc> implicit __clang_svmfloat8x3_t '__clang_svmfloat8x3_t'
| `-BuiltinType 0xcbf156c70 '__clang_svmfloat8x3_t'
|-TypedefDecl 0xcbf274618 <<invalid sloc>> <invalid sloc> implicit __clang_svint8x4_t '__clang_svint8x4_t'
| `-BuiltinType 0xcbf156c90 '__clang_svint8x4_t'
|-TypedefDecl 0xcbf274680 <<invalid sloc>> <invalid sloc> implicit __clang_svint16x4_t '__clang_svint16x4_t'
| `-BuiltinType 0xcbf156cb0 '__clang_svint16x4_t'
|-TypedefDecl 0xcbf2746e8 <<invalid sloc>> <invalid sloc> implicit __clang_svint32x4_t '__clang_svint32x4_t'
| `-BuiltinType 0xcbf156cd0 '__clang_svint32x4_t'
|-TypedefDecl 0xcbf274750 <<invalid sloc>> <invalid sloc> implicit __clang_svint64x4_t '__clang_svint64x4_t'
| `-BuiltinType 0xcbf156cf0 '__clang_svint64x4_t'
|-TypedefDecl 0xcbf2747b8 <<invalid sloc>> <invalid sloc> implicit __clang_svuint8x4_t '__clang_svuint8x4_t'
| `-BuiltinType 0xcbf156d10 '__clang_svuint8x4_t'
|-TypedefDecl 0xcbf274820 <<invalid sloc>> <invalid sloc> implicit __clang_svuint16x4_t '__clang_svuint16x4_t'
| `-BuiltinType 0xcbf156d30 '__clang_svuint16x4_t'
|-TypedefDecl 0xcbf274888 <<invalid sloc>> <invalid sloc> implicit __clang_svuint32x4_t '__clang_svuint32x4_t'
| `-BuiltinType 0xcbf156d50 '__clang_svuint32x4_t'
|-TypedefDecl 0xcbf2748f0 <<invalid sloc>> <invalid sloc> implicit __clang_svuint64x4_t '__clang_svuint64x4_t'
| `-BuiltinType 0xcbf156d70 '__clang_svuint64x4_t'
|-TypedefDecl 0xcbf274958 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat16x4_t '__clang_svfloat16x4_t'
| `-BuiltinType 0xcbf156d90 '__clang_svfloat16x4_t'
|-TypedefDecl 0xcbf2749c0 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat32x4_t '__clang_svfloat32x4_t'
| `-BuiltinType 0xcbf156db0 '__clang_svfloat32x4_t'
|-TypedefDecl 0xcbf274a28 <<invalid sloc>> <invalid sloc> implicit __clang_svfloat64x4_t '__clang_svfloat64x4_t'
| `-BuiltinType 0xcbf156dd0 '__clang_svfloat64x4_t'
|-TypedefDecl 0xcbf274a90 <<invalid sloc>> <invalid sloc> implicit __clang_svbfloat16x4_t '__clang_svbfloat16x4_t'
| `-BuiltinType 0xcbf156df0 '__clang_svbfloat16x4_t'
|-TypedefDecl 0xcbf274af8 <<invalid sloc>> <invalid sloc> implicit __clang_svmfloat8x4_t '__clang_svmfloat8x4_t'
| `-BuiltinType 0xcbf156e10 '__clang_svmfloat8x4_t'
|-TypedefDecl 0xcbf274b60 <<invalid sloc>> <invalid sloc> implicit __SVBool_t '__SVBool_t'
| `-BuiltinType 0xcbf156e30 '__SVBool_t'
|-TypedefDecl 0xcbf274bc8 <<invalid sloc>> <invalid sloc> implicit __clang_svboolx2_t '__clang_svboolx2_t'
| `-BuiltinType 0xcbf156e50 '__clang_svboolx2_t'
|-TypedefDecl 0xcbf274c30 <<invalid sloc>> <invalid sloc> implicit __clang_svboolx4_t '__clang_svboolx4_t'
| `-BuiltinType 0xcbf156e70 '__clang_svboolx4_t'
|-TypedefDecl 0xcbf274c98 <<invalid sloc>> <invalid sloc> implicit __SVCount_t '__SVCount_t'
| `-BuiltinType 0xcbf156e90 '__SVCount_t'
|-TypedefDecl 0xcbf274d00 <<invalid sloc>> <invalid sloc> implicit __mfp8 '__mfp8'
| `-BuiltinType 0xcbf156eb0 '__mfp8'
|-TypedefDecl 0xcbf156fa8 <<invalid sloc>> <invalid sloc> implicit __builtin_ms_va_list 'char *'
| `-PointerType 0xcbf156f60 'char *'
|   `-BuiltinType 0xcbf1560b0 'char'
|-TypedefDecl 0xcbf1f2018 <<invalid sloc>> <invalid sloc> implicit __builtin_va_list 'char *'
| `-PointerType 0xcbf156f60 'char *'
|   `-BuiltinType 0xcbf1560b0 'char'
`-TypedefDecl 0xcbf2a1398 <platform/test.h:9:1, <invalid sloc>> col:9 invalid bool 'int (int *)'PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/homebrew/opt/llvm/bin/clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics platform/test.h
1.	<eof> parser at end of file
 #0 0x000000011741cb04 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x3e48b04)
 #1 0x00000001136f3f4c llvm::sys::CleanupOnSignal(unsigned long) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x11ff4c)
 #2 0x00000001173f6ee8 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x3e22ee8)
 #3 0x00000001173f6fc4 CrashRecoverySignalHandler(int) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x3e22fc4)
 #4 0x000000018d069764 (/usr/lib/system/libsystem_platform.dylib+0x1804e5764)
 #5 0x0000000106954340 clang::comments::Sema::resolveParamCommandIndexes(clang::comments::FullComment const*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x480340)
 #6 0x0000000106954340 clang::comments::Sema::resolveParamCommandIndexes(clang::comments::FullComment const*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x480340)
 #7 0x00000001069540bc clang::comments::Sema::actOnFullComment(llvm::ArrayRef<clang::comments::BlockContentComment*>) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x4800bc)
 #8 0x0000000106950078 clang::comments::Parser::parseFullComment() (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x47c078)
 #9 0x0000000106cbeebc clang::RawComment::parse(clang::ASTContext const&, clang::Preprocessor const*, clang::Decl const*) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x7eaebc)
#10 0x00000001068d18c4 clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*, bool)::'lambda'()::operator()() const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x3fd8c4)
#11 0x00000001068d5b74 void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*, bool)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*, bool)::'lambda'())::'lambda'(bool)::operator()(bool) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x401b74)
#12 0x00000001068d159c void clang::TextTreeStructure::AddChild<clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*, bool)::'lambda'()>(llvm::StringRef, clang::ASTNodeTraverser<clang::ASTDumper, clang::TextNodeDumper>::Visit(clang::Decl const*, bool)::'lambda'()) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x3fd59c)
#13 0x00000001068d0978 clang::Decl::dump(llvm::raw_ostream&, bool, clang::ASTDumpOutputFormat) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x3fc978)
#14 0x0000000107e6e728 (anonymous namespace)::ASTPrinter::print(clang::Decl*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x199a728)
#15 0x00000001066abd88 clang::ParseAST(clang::Sema&, bool, bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1d7d88)
#16 0x0000000107ef3b44 clang::FrontendAction::Execute() (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1a1fb44)
#17 0x0000000107e8af64 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x19b6f64)
#18 0x0000000107f28dd0 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1a54dd0)
#19 0x00000001021f53a0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x1000053a0)
#20 0x00000001021f3550 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x100003550)
#21 0x0000000107bf4360 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_0>(long) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1720360)
#22 0x0000000113646cfc llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x72cfc)
#23 0x0000000107bf3e98 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x171fe98)
#24 0x0000000107bbde34 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x16e9e34)
#25 0x0000000107bda8b8 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x17068b8)
#26 0x00000001021f2550 clang_main(int, char**, llvm::ToolContext const&) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x100002550)
#27 0x00000001021fd810 main (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x10000d810)
#28 0x000000018cc95d54
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Homebrew clang version 21.1.8
Target: arm64-apple-darwin25.3.0
Thread model: posix
InstalledDir: /opt/homebrew/Cellar/llvm/21.1.8_1/bin
Configuration file: /opt/homebrew/etc/clang/arm64-apple-darwin25.cfg
clang: error: unable to execute command: Segmentation fault: 11
clang: note: diagnostic msg: Error generating preprocessed source(s).
```
</details>

I tried clang `21.1.8` and `17.0.0` (`21.1.8` was installed by brew as a latest available)


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alexey Ovchinnikov (cybernetlab)

<details>
Originally I tried to parse headers of [ESP Zigbee library](https://github.com/espressif/esp-zigbee-sdk) and clang crashes on [this file (line104)](https://github.com/espressif/esp-zigbee-sdk/blob/main/components/esp-zigbee-lib/include/platform/esp_zigbee_platform.h#L104). After some investigation I found that if I remove both `@<!-- -->param` lines from doxygen comment before that line everything is fine. I see that actually this comment is invalid cause it describes typedef for function but not an actual function declaration and `@<!-- -->param` shouldn't be used there. Anyway I believe that AST parser shouldn't crash with segfault on such mistakes and should generate warn or error instead.

## Reproduce

test file `test.h`:

```c
/**
 * @<!-- -->brief A callback
 *
 * @<!-- -->param[in] a param1
 * @<!-- -->return
 *      - true: ok
 *      - false: failure
 */
typedef bool (*func_t)(uint8_t a);
```

command:

```sh
clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics test.h
```

&lt;details&gt;
&lt;summary&gt;output&lt;/summary&gt;
```
platform/test.h:9:16: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
    9 | typedef bool (*func_t)(uint8_t a);
      |                ^
      |               int
platform/test.h:9:24: error: unknown type name 'uint8_t'
    9 | typedef bool (*func_t)(uint8_t a);
      |                        ^
platform/test.h:9:9: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
    9 | typedef bool (*func_t)(uint8_t a);
      | ~~~~~~~ ^
      | int
platform/test.h:9:14: error: function cannot return function type 'int (int)'
    9 | typedef bool (*func_t)(uint8_t a);
      |              ^
TranslationUnitDecl 0xcbf156008 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt;
|-TypedefDecl 0xcbf1f2088 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __int128_t '__int128'
| `-BuiltinType 0xcbf1565d0 '__int128'
|-TypedefDecl 0xcbf1f20f8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __uint128_t 'unsigned __int128'
| `-BuiltinType 0xcbf1565f0 'unsigned __int128'
|-TypedefDecl 0xcbf1f2400 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __NSConstantString 'struct __NSConstantString_tag'
| `-RecordType 0xcbf1f21d0 'struct __NSConstantString_tag'
|   `-Record 0xcbf1f2150 '__NSConstantString_tag'
|-TypedefDecl 0xcbf1f24a8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Int8x8_t '__attribute__((neon_vector_type(8))) signed char'
| `-VectorType 0xcbf1f2460 '__attribute__((neon_vector_type(8))) signed char' neon 8
|   `-BuiltinType 0xcbf1560d0 'signed char'
|-TypedefDecl 0xcbf1f2548 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Int16x4_t '__attribute__((neon_vector_type(4))) short'
| `-VectorType 0xcbf1f2500 '__attribute__((neon_vector_type(4))) short' neon 4
|   `-BuiltinType 0xcbf1560f0 'short'
|-TypedefDecl 0xcbf1f25e8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Int32x2_t '__attribute__((neon_vector_type(2))) int'
| `-VectorType 0xcbf1f25a0 '__attribute__((neon_vector_type(2))) int' neon 2
|   `-BuiltinType 0xcbf156110 'int'
|-TypedefDecl 0xcbf1f2688 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Uint8x8_t '__attribute__((neon_vector_type(8))) unsigned char'
| `-VectorType 0xcbf1f2640 '__attribute__((neon_vector_type(8))) unsigned char' neon 8
|   `-BuiltinType 0xcbf156170 'unsigned char'
|-TypedefDecl 0xcbf1f2728 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Uint16x4_t '__attribute__((neon_vector_type(4))) unsigned short'
| `-VectorType 0xcbf1f26e0 '__attribute__((neon_vector_type(4))) unsigned short' neon 4
|   `-BuiltinType 0xcbf156190 'unsigned short'
|-TypedefDecl 0xcbf1f27c8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Uint32x2_t '__attribute__((neon_vector_type(2))) unsigned int'
| `-VectorType 0xcbf1f2780 '__attribute__((neon_vector_type(2))) unsigned int' neon 2
|   `-BuiltinType 0xcbf1561b0 'unsigned int'
|-TypedefDecl 0xcbf1f2868 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Float16x4_t '__attribute__((neon_vector_type(4))) __fp16'
| `-VectorType 0xcbf1f2820 '__attribute__((neon_vector_type(4))) __fp16' neon 4
|   `-BuiltinType 0xcbf156f20 '__fp16'
|-TypedefDecl 0xcbf1f2908 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Float32x2_t '__attribute__((neon_vector_type(2))) float'
| `-VectorType 0xcbf1f28c0 '__attribute__((neon_vector_type(2))) float' neon 2
|   `-BuiltinType 0xcbf156210 'float'
|-TypedefDecl 0xcbf1f29a8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Poly8x8_t '__attribute__((neon_polyvector_type(8))) unsigned char'
| `-VectorType 0xcbf1f2960 '__attribute__((neon_polyvector_type(8))) unsigned char' neon poly 8
|   `-BuiltinType 0xcbf156170 'unsigned char'
|-TypedefDecl 0xcbf1f2a48 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Poly16x4_t '__attribute__((neon_polyvector_type(4))) unsigned short'
| `-VectorType 0xcbf1f2a00 '__attribute__((neon_polyvector_type(4))) unsigned short' neon poly 4
|   `-BuiltinType 0xcbf156190 'unsigned short'
|-TypedefDecl 0xcbf1f2ae8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Bfloat16x4_t '__attribute__((neon_vector_type(4))) __bf16'
| `-VectorType 0xcbf1f2aa0 '__attribute__((neon_vector_type(4))) __bf16' neon 4
|   `-BuiltinType 0xcbf156f40 '__bf16'
|-TypedefDecl 0xcbf1f2b88 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Int8x16_t '__attribute__((neon_vector_type(16))) signed char'
| `-VectorType 0xcbf1f2b40 '__attribute__((neon_vector_type(16))) signed char' neon 16
|   `-BuiltinType 0xcbf1560d0 'signed char'
|-TypedefDecl 0xcbf1f2c28 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Int16x8_t '__attribute__((neon_vector_type(8))) short'
| `-VectorType 0xcbf1f2be0 '__attribute__((neon_vector_type(8))) short' neon 8
|   `-BuiltinType 0xcbf1560f0 'short'
|-TypedefDecl 0xcbf1f2cc8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Int32x4_t '__attribute__((neon_vector_type(4))) int'
| `-VectorType 0xcbf1f2c80 '__attribute__((neon_vector_type(4))) int' neon 4
|   `-BuiltinType 0xcbf156110 'int'
|-TypedefDecl 0xcbf1f2d68 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Int64x2_t '__attribute__((neon_vector_type(2))) long'
| `-VectorType 0xcbf1f2d20 '__attribute__((neon_vector_type(2))) long' neon 2
|   `-BuiltinType 0xcbf156130 'long'
|-TypedefDecl 0xcbf1f2e08 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Uint8x16_t '__attribute__((neon_vector_type(16))) unsigned char'
| `-VectorType 0xcbf1f2dc0 '__attribute__((neon_vector_type(16))) unsigned char' neon 16
|   `-BuiltinType 0xcbf156170 'unsigned char'
|-TypedefDecl 0xcbf1f2ea8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Uint16x8_t '__attribute__((neon_vector_type(8))) unsigned short'
| `-VectorType 0xcbf1f2e60 '__attribute__((neon_vector_type(8))) unsigned short' neon 8
|   `-BuiltinType 0xcbf156190 'unsigned short'
|-TypedefDecl 0xcbf1f2f48 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Uint32x4_t '__attribute__((neon_vector_type(4))) unsigned int'
| `-VectorType 0xcbf1f2f00 '__attribute__((neon_vector_type(4))) unsigned int' neon 4
|   `-BuiltinType 0xcbf1561b0 'unsigned int'
|-TypedefDecl 0xcbf1f3000 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Uint64x2_t '__attribute__((neon_vector_type(2))) unsigned long'
| `-VectorType 0xcbf1f2fa0 '__attribute__((neon_vector_type(2))) unsigned long' neon 2
|   `-BuiltinType 0xcbf1561d0 'unsigned long'
|-TypedefDecl 0xcbf1f30a8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Float16x8_t '__attribute__((neon_vector_type(8))) __fp16'
| `-VectorType 0xcbf1f3060 '__attribute__((neon_vector_type(8))) __fp16' neon 8
|   `-BuiltinType 0xcbf156f20 '__fp16'
|-TypedefDecl 0xcbf1f3148 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Float32x4_t '__attribute__((neon_vector_type(4))) float'
| `-VectorType 0xcbf1f3100 '__attribute__((neon_vector_type(4))) float' neon 4
|   `-BuiltinType 0xcbf156210 'float'
|-TypedefDecl 0xcbf1f31e8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Float64x2_t '__attribute__((neon_vector_type(2))) double'
| `-VectorType 0xcbf1f31a0 '__attribute__((neon_vector_type(2))) double' neon 2
|   `-BuiltinType 0xcbf156230 'double'
|-TypedefDecl 0xcbf1f3288 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Poly8x16_t '__attribute__((neon_polyvector_type(16))) unsigned char'
| `-VectorType 0xcbf1f3240 '__attribute__((neon_polyvector_type(16))) unsigned char' neon poly 16
|   `-BuiltinType 0xcbf156170 'unsigned char'
|-TypedefDecl 0xcbf1f3328 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Poly16x8_t '__attribute__((neon_polyvector_type(8))) unsigned short'
| `-VectorType 0xcbf1f32e0 '__attribute__((neon_polyvector_type(8))) unsigned short' neon poly 8
|   `-BuiltinType 0xcbf156190 'unsigned short'
|-TypedefDecl 0xcbf1f33c8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Poly64x2_t '__attribute__((neon_polyvector_type(2))) unsigned long'
| `-VectorType 0xcbf1f3380 '__attribute__((neon_polyvector_type(2))) unsigned long' neon poly 2
|   `-BuiltinType 0xcbf1561d0 'unsigned long'
|-TypedefDecl 0xcbf1f3468 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Bfloat16x8_t '__attribute__((neon_vector_type(8))) __bf16'
| `-VectorType 0xcbf1f3420 '__attribute__((neon_vector_type(8))) __bf16' neon 8
|   `-BuiltinType 0xcbf156f40 '__bf16'
|-TypedefDecl 0xcbf1f3508 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Mfloat8x8_t '__attribute__((neon_vector_type(8))) __mfp8'
| `-VectorType 0xcbf1f34c0 '__attribute__((neon_vector_type(8))) __mfp8' neon 8
|   `-BuiltinType 0xcbf156eb0 '__mfp8'
|-TypedefDecl 0xcbf1f35a8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Mfloat8x16_t '__attribute__((neon_vector_type(16))) __mfp8'
| `-VectorType 0xcbf1f3560 '__attribute__((neon_vector_type(16))) __mfp8' neon 16
|   `-BuiltinType 0xcbf156eb0 '__mfp8'
|-TypedefDecl 0xcbf1f3610 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVInt8_t '__SVInt8_t'
| `-BuiltinType 0xcbf1567b0 '__SVInt8_t'
|-TypedefDecl 0xcbf1f3678 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVInt16_t '__SVInt16_t'
| `-BuiltinType 0xcbf1567d0 '__SVInt16_t'
|-TypedefDecl 0xcbf1f36e0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVInt32_t '__SVInt32_t'
| `-BuiltinType 0xcbf1567f0 '__SVInt32_t'
|-TypedefDecl 0xcbf1f3748 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVInt64_t '__SVInt64_t'
| `-BuiltinType 0xcbf156810 '__SVInt64_t'
|-TypedefDecl 0xcbf1f37b0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVUint8_t '__SVUint8_t'
| `-BuiltinType 0xcbf156830 '__SVUint8_t'
|-TypedefDecl 0xcbf1f3818 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVUint16_t '__SVUint16_t'
| `-BuiltinType 0xcbf156850 '__SVUint16_t'
|-TypedefDecl 0xcbf1f3880 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVUint32_t '__SVUint32_t'
| `-BuiltinType 0xcbf156870 '__SVUint32_t'
|-TypedefDecl 0xcbf1f38e8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVUint64_t '__SVUint64_t'
| `-BuiltinType 0xcbf156890 '__SVUint64_t'
|-TypedefDecl 0xcbf1f3950 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVFloat16_t '__SVFloat16_t'
| `-BuiltinType 0xcbf1568b0 '__SVFloat16_t'
|-TypedefDecl 0xcbf1f39b8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVFloat32_t '__SVFloat32_t'
| `-BuiltinType 0xcbf1568d0 '__SVFloat32_t'
|-TypedefDecl 0xcbf1f3a20 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVFloat64_t '__SVFloat64_t'
| `-BuiltinType 0xcbf1568f0 '__SVFloat64_t'
|-TypedefDecl 0xcbf1f3a88 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVBfloat16_t '__SVBfloat16_t'
| `-BuiltinType 0xcbf156910 '__SVBfloat16_t'
|-TypedefDecl 0xcbf1f3af0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVMfloat8_t '__SVMfloat8_t'
| `-BuiltinType 0xcbf156930 '__SVMfloat8_t'
|-TypedefDecl 0xcbf1f3b58 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint8x2_t '__clang_svint8x2_t'
| `-BuiltinType 0xcbf156950 '__clang_svint8x2_t'
|-TypedefDecl 0xcbf1f3bc0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint16x2_t '__clang_svint16x2_t'
| `-BuiltinType 0xcbf156970 '__clang_svint16x2_t'
|-TypedefDecl 0xcbf1f3c28 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint32x2_t '__clang_svint32x2_t'
| `-BuiltinType 0xcbf156990 '__clang_svint32x2_t'
|-TypedefDecl 0xcbf1f3c90 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint64x2_t '__clang_svint64x2_t'
| `-BuiltinType 0xcbf1569b0 '__clang_svint64x2_t'
|-TypedefDecl 0xcbf1f3cf8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint8x2_t '__clang_svuint8x2_t'
| `-BuiltinType 0xcbf1569d0 '__clang_svuint8x2_t'
|-TypedefDecl 0xcbf1f3d60 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint16x2_t '__clang_svuint16x2_t'
| `-BuiltinType 0xcbf1569f0 '__clang_svuint16x2_t'
|-TypedefDecl 0xcbf1f3dc8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint32x2_t '__clang_svuint32x2_t'
| `-BuiltinType 0xcbf156a10 '__clang_svuint32x2_t'
|-TypedefDecl 0xcbf1f3e30 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint64x2_t '__clang_svuint64x2_t'
| `-BuiltinType 0xcbf156a30 '__clang_svuint64x2_t'
|-TypedefDecl 0xcbf1f3e98 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat16x2_t '__clang_svfloat16x2_t'
| `-BuiltinType 0xcbf156a50 '__clang_svfloat16x2_t'
|-TypedefDecl 0xcbf1f3f00 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat32x2_t '__clang_svfloat32x2_t'
| `-BuiltinType 0xcbf156a70 '__clang_svfloat32x2_t'
|-TypedefDecl 0xcbf1f3f68 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat64x2_t '__clang_svfloat64x2_t'
| `-BuiltinType 0xcbf156a90 '__clang_svfloat64x2_t'
|-TypedefDecl 0xcbf274000 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svbfloat16x2_t '__clang_svbfloat16x2_t'
| `-BuiltinType 0xcbf156ab0 '__clang_svbfloat16x2_t'
|-TypedefDecl 0xcbf274068 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svmfloat8x2_t '__clang_svmfloat8x2_t'
| `-BuiltinType 0xcbf156ad0 '__clang_svmfloat8x2_t'
|-TypedefDecl 0xcbf2740d0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint8x3_t '__clang_svint8x3_t'
| `-BuiltinType 0xcbf156af0 '__clang_svint8x3_t'
|-TypedefDecl 0xcbf274138 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint16x3_t '__clang_svint16x3_t'
| `-BuiltinType 0xcbf156b10 '__clang_svint16x3_t'
|-TypedefDecl 0xcbf2741a0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint32x3_t '__clang_svint32x3_t'
| `-BuiltinType 0xcbf156b30 '__clang_svint32x3_t'
|-TypedefDecl 0xcbf274208 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint64x3_t '__clang_svint64x3_t'
| `-BuiltinType 0xcbf156b50 '__clang_svint64x3_t'
|-TypedefDecl 0xcbf274270 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint8x3_t '__clang_svuint8x3_t'
| `-BuiltinType 0xcbf156b70 '__clang_svuint8x3_t'
|-TypedefDecl 0xcbf2742d8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint16x3_t '__clang_svuint16x3_t'
| `-BuiltinType 0xcbf156b90 '__clang_svuint16x3_t'
|-TypedefDecl 0xcbf274340 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint32x3_t '__clang_svuint32x3_t'
| `-BuiltinType 0xcbf156bb0 '__clang_svuint32x3_t'
|-TypedefDecl 0xcbf2743a8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint64x3_t '__clang_svuint64x3_t'
| `-BuiltinType 0xcbf156bd0 '__clang_svuint64x3_t'
|-TypedefDecl 0xcbf274410 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat16x3_t '__clang_svfloat16x3_t'
| `-BuiltinType 0xcbf156bf0 '__clang_svfloat16x3_t'
|-TypedefDecl 0xcbf274478 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat32x3_t '__clang_svfloat32x3_t'
| `-BuiltinType 0xcbf156c10 '__clang_svfloat32x3_t'
|-TypedefDecl 0xcbf2744e0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat64x3_t '__clang_svfloat64x3_t'
| `-BuiltinType 0xcbf156c30 '__clang_svfloat64x3_t'
|-TypedefDecl 0xcbf274548 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svbfloat16x3_t '__clang_svbfloat16x3_t'
| `-BuiltinType 0xcbf156c50 '__clang_svbfloat16x3_t'
|-TypedefDecl 0xcbf2745b0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svmfloat8x3_t '__clang_svmfloat8x3_t'
| `-BuiltinType 0xcbf156c70 '__clang_svmfloat8x3_t'
|-TypedefDecl 0xcbf274618 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint8x4_t '__clang_svint8x4_t'
| `-BuiltinType 0xcbf156c90 '__clang_svint8x4_t'
|-TypedefDecl 0xcbf274680 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint16x4_t '__clang_svint16x4_t'
| `-BuiltinType 0xcbf156cb0 '__clang_svint16x4_t'
|-TypedefDecl 0xcbf2746e8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint32x4_t '__clang_svint32x4_t'
| `-BuiltinType 0xcbf156cd0 '__clang_svint32x4_t'
|-TypedefDecl 0xcbf274750 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint64x4_t '__clang_svint64x4_t'
| `-BuiltinType 0xcbf156cf0 '__clang_svint64x4_t'
|-TypedefDecl 0xcbf2747b8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint8x4_t '__clang_svuint8x4_t'
| `-BuiltinType 0xcbf156d10 '__clang_svuint8x4_t'
|-TypedefDecl 0xcbf274820 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint16x4_t '__clang_svuint16x4_t'
| `-BuiltinType 0xcbf156d30 '__clang_svuint16x4_t'
|-TypedefDecl 0xcbf274888 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint32x4_t '__clang_svuint32x4_t'
| `-BuiltinType 0xcbf156d50 '__clang_svuint32x4_t'
|-TypedefDecl 0xcbf2748f0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint64x4_t '__clang_svuint64x4_t'
| `-BuiltinType 0xcbf156d70 '__clang_svuint64x4_t'
|-TypedefDecl 0xcbf274958 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat16x4_t '__clang_svfloat16x4_t'
| `-BuiltinType 0xcbf156d90 '__clang_svfloat16x4_t'
|-TypedefDecl 0xcbf2749c0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat32x4_t '__clang_svfloat32x4_t'
| `-BuiltinType 0xcbf156db0 '__clang_svfloat32x4_t'
|-TypedefDecl 0xcbf274a28 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat64x4_t '__clang_svfloat64x4_t'
| `-BuiltinType 0xcbf156dd0 '__clang_svfloat64x4_t'
|-TypedefDecl 0xcbf274a90 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svbfloat16x4_t '__clang_svbfloat16x4_t'
| `-BuiltinType 0xcbf156df0 '__clang_svbfloat16x4_t'
|-TypedefDecl 0xcbf274af8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svmfloat8x4_t '__clang_svmfloat8x4_t'
| `-BuiltinType 0xcbf156e10 '__clang_svmfloat8x4_t'
|-TypedefDecl 0xcbf274b60 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVBool_t '__SVBool_t'
| `-BuiltinType 0xcbf156e30 '__SVBool_t'
|-TypedefDecl 0xcbf274bc8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svboolx2_t '__clang_svboolx2_t'
| `-BuiltinType 0xcbf156e50 '__clang_svboolx2_t'
|-TypedefDecl 0xcbf274c30 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svboolx4_t '__clang_svboolx4_t'
| `-BuiltinType 0xcbf156e70 '__clang_svboolx4_t'
|-TypedefDecl 0xcbf274c98 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVCount_t '__SVCount_t'
| `-BuiltinType 0xcbf156e90 '__SVCount_t'
|-TypedefDecl 0xcbf274d00 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __mfp8 '__mfp8'
| `-BuiltinType 0xcbf156eb0 '__mfp8'
|-TypedefDecl 0xcbf156fa8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __builtin_ms_va_list 'char *'
| `-PointerType 0xcbf156f60 'char *'
|   `-BuiltinType 0xcbf1560b0 'char'
|-TypedefDecl 0xcbf1f2018 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __builtin_va_list 'char *'
| `-PointerType 0xcbf156f60 'char *'
|   `-BuiltinType 0xcbf1560b0 'char'
`-TypedefDecl 0xcbf2a1398 &lt;platform/test.h:9:1, &lt;invalid sloc&gt;&gt; col:9 invalid bool 'int (int *)'PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/homebrew/opt/llvm/bin/clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics platform/test.h
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x000000011741cb04 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x3e48b04)
 #<!-- -->1 0x00000001136f3f4c llvm::sys::CleanupOnSignal(unsigned long) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x11ff4c)
 #<!-- -->2 0x00000001173f6ee8 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x3e22ee8)
 #<!-- -->3 0x00000001173f6fc4 CrashRecoverySignalHandler(int) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x3e22fc4)
 #<!-- -->4 0x000000018d069764 (/usr/lib/system/libsystem_platform.dylib+0x1804e5764)
 #<!-- -->5 0x0000000106954340 clang::comments::Sema::resolveParamCommandIndexes(clang::comments::FullComment const*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x480340)
 #<!-- -->6 0x0000000106954340 clang::comments::Sema::resolveParamCommandIndexes(clang::comments::FullComment const*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x480340)
 #<!-- -->7 0x00000001069540bc clang::comments::Sema::actOnFullComment(llvm::ArrayRef&lt;clang::comments::BlockContentComment*&gt;) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x4800bc)
 #<!-- -->8 0x0000000106950078 clang::comments::Parser::parseFullComment() (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x47c078)
 #<!-- -->9 0x0000000106cbeebc clang::RawComment::parse(clang::ASTContext const&amp;, clang::Preprocessor const*, clang::Decl const*) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x7eaebc)
#<!-- -->10 0x00000001068d18c4 clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*, bool)::'lambda'()::operator()() const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x3fd8c4)
#<!-- -->11 0x00000001068d5b74 void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*, bool)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*, bool)::'lambda'())::'lambda'(bool)::operator()(bool) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x401b74)
#<!-- -->12 0x00000001068d159c void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*, bool)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*, bool)::'lambda'()) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x3fd59c)
#<!-- -->13 0x00000001068d0978 clang::Decl::dump(llvm::raw_ostream&amp;, bool, clang::ASTDumpOutputFormat) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x3fc978)
#<!-- -->14 0x0000000107e6e728 (anonymous namespace)::ASTPrinter::print(clang::Decl*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x199a728)
#<!-- -->15 0x00000001066abd88 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1d7d88)
#<!-- -->16 0x0000000107ef3b44 clang::FrontendAction::Execute() (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1a1fb44)
#<!-- -->17 0x0000000107e8af64 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x19b6f64)
#<!-- -->18 0x0000000107f28dd0 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1a54dd0)
#<!-- -->19 0x00000001021f53a0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x1000053a0)
#<!-- -->20 0x00000001021f3550 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x100003550)
#<!-- -->21 0x0000000107bf4360 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1720360)
#<!-- -->22 0x0000000113646cfc llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x72cfc)
#<!-- -->23 0x0000000107bf3e98 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x171fe98)
#<!-- -->24 0x0000000107bbde34 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x16e9e34)
#<!-- -->25 0x0000000107bda8b8 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x17068b8)
#<!-- -->26 0x00000001021f2550 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x100002550)
#<!-- -->27 0x00000001021fd810 main (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x10000d810)
#<!-- -->28 0x000000018cc95d54
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Homebrew clang version 21.1.8
Target: arm64-apple-darwin25.3.0
Thread model: posix
InstalledDir: /opt/homebrew/Cellar/llvm/21.1.8_1/bin
Configuration file: /opt/homebrew/etc/clang/arm64-apple-darwin25.cfg
clang: error: unable to execute command: Segmentation fault: 11
clang: note: diagnostic msg: Error generating preprocessed source(s).
```
&lt;/details&gt;

I tried clang `21.1.8` and `17.0.0` (`21.1.8` was installed by brew as a latest available)

</details>


---

### Comment 2 - shafik

Looks C specific: https://godbolt.org/z/na5x9afYv

---

### Comment 3 - shafik

So `Sema::resolveParmVarReference`

https://github.com/llvm/llvm-project/blob/25f69d7a3feefbf96f3d58d8ca1d8fce45d4543d/clang/lib/AST/CommentSema.cpp#L962-L963

Does not handle if `ParamVars[i]` is `nullptr`, maybe we could return `ParamCommandComment::InvalidParamIndex` but I think maybe we need a new enum. 

and then in `resolveParamCommandIndexes` we would need to handle the case later on and I think maybe a new diagnostic. 

Maybe good first issue @AaronBallman 

---

### Comment 4 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor is working on this issue. If someone is assigned to the issue or claimed to be working on it, ping the person. After one week without a response, the assignee may be changed.
1. Leave a comment indicating that you are working on the issue, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 5 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Alexey Ovchinnikov (cybernetlab)

<details>
Originally I tried to parse headers of [ESP Zigbee library](https://github.com/espressif/esp-zigbee-sdk) and clang crashes on [this file (line104)](https://github.com/espressif/esp-zigbee-sdk/blob/main/components/esp-zigbee-lib/include/platform/esp_zigbee_platform.h#L104). After some investigation I found that if I remove both `@<!-- -->param` lines from doxygen comment before that line everything is fine. I see that actually this comment is invalid cause it describes typedef for function but not an actual function declaration and `@<!-- -->param` shouldn't be used there. Anyway I believe that AST parser shouldn't crash with segfault on such mistakes and should generate warn or error instead.

## Reproduce

test file `test.h`:

```c
/**
 * @<!-- -->brief A callback
 *
 * @<!-- -->param[in] a param1
 * @<!-- -->return
 *      - true: ok
 *      - false: failure
 */
typedef bool (*func_t)(uint8_t a);
```

command:

```sh
clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics test.h
```

&lt;details&gt;
&lt;summary&gt;output&lt;/summary&gt;
```
platform/test.h:9:16: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
    9 | typedef bool (*func_t)(uint8_t a);
      |                ^
      |               int
platform/test.h:9:24: error: unknown type name 'uint8_t'
    9 | typedef bool (*func_t)(uint8_t a);
      |                        ^
platform/test.h:9:9: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
    9 | typedef bool (*func_t)(uint8_t a);
      | ~~~~~~~ ^
      | int
platform/test.h:9:14: error: function cannot return function type 'int (int)'
    9 | typedef bool (*func_t)(uint8_t a);
      |              ^
TranslationUnitDecl 0xcbf156008 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt;
|-TypedefDecl 0xcbf1f2088 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __int128_t '__int128'
| `-BuiltinType 0xcbf1565d0 '__int128'
|-TypedefDecl 0xcbf1f20f8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __uint128_t 'unsigned __int128'
| `-BuiltinType 0xcbf1565f0 'unsigned __int128'
|-TypedefDecl 0xcbf1f2400 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __NSConstantString 'struct __NSConstantString_tag'
| `-RecordType 0xcbf1f21d0 'struct __NSConstantString_tag'
|   `-Record 0xcbf1f2150 '__NSConstantString_tag'
|-TypedefDecl 0xcbf1f24a8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Int8x8_t '__attribute__((neon_vector_type(8))) signed char'
| `-VectorType 0xcbf1f2460 '__attribute__((neon_vector_type(8))) signed char' neon 8
|   `-BuiltinType 0xcbf1560d0 'signed char'
|-TypedefDecl 0xcbf1f2548 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Int16x4_t '__attribute__((neon_vector_type(4))) short'
| `-VectorType 0xcbf1f2500 '__attribute__((neon_vector_type(4))) short' neon 4
|   `-BuiltinType 0xcbf1560f0 'short'
|-TypedefDecl 0xcbf1f25e8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Int32x2_t '__attribute__((neon_vector_type(2))) int'
| `-VectorType 0xcbf1f25a0 '__attribute__((neon_vector_type(2))) int' neon 2
|   `-BuiltinType 0xcbf156110 'int'
|-TypedefDecl 0xcbf1f2688 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Uint8x8_t '__attribute__((neon_vector_type(8))) unsigned char'
| `-VectorType 0xcbf1f2640 '__attribute__((neon_vector_type(8))) unsigned char' neon 8
|   `-BuiltinType 0xcbf156170 'unsigned char'
|-TypedefDecl 0xcbf1f2728 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Uint16x4_t '__attribute__((neon_vector_type(4))) unsigned short'
| `-VectorType 0xcbf1f26e0 '__attribute__((neon_vector_type(4))) unsigned short' neon 4
|   `-BuiltinType 0xcbf156190 'unsigned short'
|-TypedefDecl 0xcbf1f27c8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Uint32x2_t '__attribute__((neon_vector_type(2))) unsigned int'
| `-VectorType 0xcbf1f2780 '__attribute__((neon_vector_type(2))) unsigned int' neon 2
|   `-BuiltinType 0xcbf1561b0 'unsigned int'
|-TypedefDecl 0xcbf1f2868 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Float16x4_t '__attribute__((neon_vector_type(4))) __fp16'
| `-VectorType 0xcbf1f2820 '__attribute__((neon_vector_type(4))) __fp16' neon 4
|   `-BuiltinType 0xcbf156f20 '__fp16'
|-TypedefDecl 0xcbf1f2908 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Float32x2_t '__attribute__((neon_vector_type(2))) float'
| `-VectorType 0xcbf1f28c0 '__attribute__((neon_vector_type(2))) float' neon 2
|   `-BuiltinType 0xcbf156210 'float'
|-TypedefDecl 0xcbf1f29a8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Poly8x8_t '__attribute__((neon_polyvector_type(8))) unsigned char'
| `-VectorType 0xcbf1f2960 '__attribute__((neon_polyvector_type(8))) unsigned char' neon poly 8
|   `-BuiltinType 0xcbf156170 'unsigned char'
|-TypedefDecl 0xcbf1f2a48 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Poly16x4_t '__attribute__((neon_polyvector_type(4))) unsigned short'
| `-VectorType 0xcbf1f2a00 '__attribute__((neon_polyvector_type(4))) unsigned short' neon poly 4
|   `-BuiltinType 0xcbf156190 'unsigned short'
|-TypedefDecl 0xcbf1f2ae8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Bfloat16x4_t '__attribute__((neon_vector_type(4))) __bf16'
| `-VectorType 0xcbf1f2aa0 '__attribute__((neon_vector_type(4))) __bf16' neon 4
|   `-BuiltinType 0xcbf156f40 '__bf16'
|-TypedefDecl 0xcbf1f2b88 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Int8x16_t '__attribute__((neon_vector_type(16))) signed char'
| `-VectorType 0xcbf1f2b40 '__attribute__((neon_vector_type(16))) signed char' neon 16
|   `-BuiltinType 0xcbf1560d0 'signed char'
|-TypedefDecl 0xcbf1f2c28 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Int16x8_t '__attribute__((neon_vector_type(8))) short'
| `-VectorType 0xcbf1f2be0 '__attribute__((neon_vector_type(8))) short' neon 8
|   `-BuiltinType 0xcbf1560f0 'short'
|-TypedefDecl 0xcbf1f2cc8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Int32x4_t '__attribute__((neon_vector_type(4))) int'
| `-VectorType 0xcbf1f2c80 '__attribute__((neon_vector_type(4))) int' neon 4
|   `-BuiltinType 0xcbf156110 'int'
|-TypedefDecl 0xcbf1f2d68 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Int64x2_t '__attribute__((neon_vector_type(2))) long'
| `-VectorType 0xcbf1f2d20 '__attribute__((neon_vector_type(2))) long' neon 2
|   `-BuiltinType 0xcbf156130 'long'
|-TypedefDecl 0xcbf1f2e08 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Uint8x16_t '__attribute__((neon_vector_type(16))) unsigned char'
| `-VectorType 0xcbf1f2dc0 '__attribute__((neon_vector_type(16))) unsigned char' neon 16
|   `-BuiltinType 0xcbf156170 'unsigned char'
|-TypedefDecl 0xcbf1f2ea8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Uint16x8_t '__attribute__((neon_vector_type(8))) unsigned short'
| `-VectorType 0xcbf1f2e60 '__attribute__((neon_vector_type(8))) unsigned short' neon 8
|   `-BuiltinType 0xcbf156190 'unsigned short'
|-TypedefDecl 0xcbf1f2f48 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Uint32x4_t '__attribute__((neon_vector_type(4))) unsigned int'
| `-VectorType 0xcbf1f2f00 '__attribute__((neon_vector_type(4))) unsigned int' neon 4
|   `-BuiltinType 0xcbf1561b0 'unsigned int'
|-TypedefDecl 0xcbf1f3000 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Uint64x2_t '__attribute__((neon_vector_type(2))) unsigned long'
| `-VectorType 0xcbf1f2fa0 '__attribute__((neon_vector_type(2))) unsigned long' neon 2
|   `-BuiltinType 0xcbf1561d0 'unsigned long'
|-TypedefDecl 0xcbf1f30a8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Float16x8_t '__attribute__((neon_vector_type(8))) __fp16'
| `-VectorType 0xcbf1f3060 '__attribute__((neon_vector_type(8))) __fp16' neon 8
|   `-BuiltinType 0xcbf156f20 '__fp16'
|-TypedefDecl 0xcbf1f3148 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Float32x4_t '__attribute__((neon_vector_type(4))) float'
| `-VectorType 0xcbf1f3100 '__attribute__((neon_vector_type(4))) float' neon 4
|   `-BuiltinType 0xcbf156210 'float'
|-TypedefDecl 0xcbf1f31e8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Float64x2_t '__attribute__((neon_vector_type(2))) double'
| `-VectorType 0xcbf1f31a0 '__attribute__((neon_vector_type(2))) double' neon 2
|   `-BuiltinType 0xcbf156230 'double'
|-TypedefDecl 0xcbf1f3288 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Poly8x16_t '__attribute__((neon_polyvector_type(16))) unsigned char'
| `-VectorType 0xcbf1f3240 '__attribute__((neon_polyvector_type(16))) unsigned char' neon poly 16
|   `-BuiltinType 0xcbf156170 'unsigned char'
|-TypedefDecl 0xcbf1f3328 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Poly16x8_t '__attribute__((neon_polyvector_type(8))) unsigned short'
| `-VectorType 0xcbf1f32e0 '__attribute__((neon_polyvector_type(8))) unsigned short' neon poly 8
|   `-BuiltinType 0xcbf156190 'unsigned short'
|-TypedefDecl 0xcbf1f33c8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Poly64x2_t '__attribute__((neon_polyvector_type(2))) unsigned long'
| `-VectorType 0xcbf1f3380 '__attribute__((neon_polyvector_type(2))) unsigned long' neon poly 2
|   `-BuiltinType 0xcbf1561d0 'unsigned long'
|-TypedefDecl 0xcbf1f3468 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Bfloat16x8_t '__attribute__((neon_vector_type(8))) __bf16'
| `-VectorType 0xcbf1f3420 '__attribute__((neon_vector_type(8))) __bf16' neon 8
|   `-BuiltinType 0xcbf156f40 '__bf16'
|-TypedefDecl 0xcbf1f3508 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Mfloat8x8_t '__attribute__((neon_vector_type(8))) __mfp8'
| `-VectorType 0xcbf1f34c0 '__attribute__((neon_vector_type(8))) __mfp8' neon 8
|   `-BuiltinType 0xcbf156eb0 '__mfp8'
|-TypedefDecl 0xcbf1f35a8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __Mfloat8x16_t '__attribute__((neon_vector_type(16))) __mfp8'
| `-VectorType 0xcbf1f3560 '__attribute__((neon_vector_type(16))) __mfp8' neon 16
|   `-BuiltinType 0xcbf156eb0 '__mfp8'
|-TypedefDecl 0xcbf1f3610 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVInt8_t '__SVInt8_t'
| `-BuiltinType 0xcbf1567b0 '__SVInt8_t'
|-TypedefDecl 0xcbf1f3678 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVInt16_t '__SVInt16_t'
| `-BuiltinType 0xcbf1567d0 '__SVInt16_t'
|-TypedefDecl 0xcbf1f36e0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVInt32_t '__SVInt32_t'
| `-BuiltinType 0xcbf1567f0 '__SVInt32_t'
|-TypedefDecl 0xcbf1f3748 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVInt64_t '__SVInt64_t'
| `-BuiltinType 0xcbf156810 '__SVInt64_t'
|-TypedefDecl 0xcbf1f37b0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVUint8_t '__SVUint8_t'
| `-BuiltinType 0xcbf156830 '__SVUint8_t'
|-TypedefDecl 0xcbf1f3818 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVUint16_t '__SVUint16_t'
| `-BuiltinType 0xcbf156850 '__SVUint16_t'
|-TypedefDecl 0xcbf1f3880 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVUint32_t '__SVUint32_t'
| `-BuiltinType 0xcbf156870 '__SVUint32_t'
|-TypedefDecl 0xcbf1f38e8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVUint64_t '__SVUint64_t'
| `-BuiltinType 0xcbf156890 '__SVUint64_t'
|-TypedefDecl 0xcbf1f3950 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVFloat16_t '__SVFloat16_t'
| `-BuiltinType 0xcbf1568b0 '__SVFloat16_t'
|-TypedefDecl 0xcbf1f39b8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVFloat32_t '__SVFloat32_t'
| `-BuiltinType 0xcbf1568d0 '__SVFloat32_t'
|-TypedefDecl 0xcbf1f3a20 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVFloat64_t '__SVFloat64_t'
| `-BuiltinType 0xcbf1568f0 '__SVFloat64_t'
|-TypedefDecl 0xcbf1f3a88 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVBfloat16_t '__SVBfloat16_t'
| `-BuiltinType 0xcbf156910 '__SVBfloat16_t'
|-TypedefDecl 0xcbf1f3af0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVMfloat8_t '__SVMfloat8_t'
| `-BuiltinType 0xcbf156930 '__SVMfloat8_t'
|-TypedefDecl 0xcbf1f3b58 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint8x2_t '__clang_svint8x2_t'
| `-BuiltinType 0xcbf156950 '__clang_svint8x2_t'
|-TypedefDecl 0xcbf1f3bc0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint16x2_t '__clang_svint16x2_t'
| `-BuiltinType 0xcbf156970 '__clang_svint16x2_t'
|-TypedefDecl 0xcbf1f3c28 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint32x2_t '__clang_svint32x2_t'
| `-BuiltinType 0xcbf156990 '__clang_svint32x2_t'
|-TypedefDecl 0xcbf1f3c90 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint64x2_t '__clang_svint64x2_t'
| `-BuiltinType 0xcbf1569b0 '__clang_svint64x2_t'
|-TypedefDecl 0xcbf1f3cf8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint8x2_t '__clang_svuint8x2_t'
| `-BuiltinType 0xcbf1569d0 '__clang_svuint8x2_t'
|-TypedefDecl 0xcbf1f3d60 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint16x2_t '__clang_svuint16x2_t'
| `-BuiltinType 0xcbf1569f0 '__clang_svuint16x2_t'
|-TypedefDecl 0xcbf1f3dc8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint32x2_t '__clang_svuint32x2_t'
| `-BuiltinType 0xcbf156a10 '__clang_svuint32x2_t'
|-TypedefDecl 0xcbf1f3e30 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint64x2_t '__clang_svuint64x2_t'
| `-BuiltinType 0xcbf156a30 '__clang_svuint64x2_t'
|-TypedefDecl 0xcbf1f3e98 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat16x2_t '__clang_svfloat16x2_t'
| `-BuiltinType 0xcbf156a50 '__clang_svfloat16x2_t'
|-TypedefDecl 0xcbf1f3f00 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat32x2_t '__clang_svfloat32x2_t'
| `-BuiltinType 0xcbf156a70 '__clang_svfloat32x2_t'
|-TypedefDecl 0xcbf1f3f68 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat64x2_t '__clang_svfloat64x2_t'
| `-BuiltinType 0xcbf156a90 '__clang_svfloat64x2_t'
|-TypedefDecl 0xcbf274000 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svbfloat16x2_t '__clang_svbfloat16x2_t'
| `-BuiltinType 0xcbf156ab0 '__clang_svbfloat16x2_t'
|-TypedefDecl 0xcbf274068 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svmfloat8x2_t '__clang_svmfloat8x2_t'
| `-BuiltinType 0xcbf156ad0 '__clang_svmfloat8x2_t'
|-TypedefDecl 0xcbf2740d0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint8x3_t '__clang_svint8x3_t'
| `-BuiltinType 0xcbf156af0 '__clang_svint8x3_t'
|-TypedefDecl 0xcbf274138 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint16x3_t '__clang_svint16x3_t'
| `-BuiltinType 0xcbf156b10 '__clang_svint16x3_t'
|-TypedefDecl 0xcbf2741a0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint32x3_t '__clang_svint32x3_t'
| `-BuiltinType 0xcbf156b30 '__clang_svint32x3_t'
|-TypedefDecl 0xcbf274208 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint64x3_t '__clang_svint64x3_t'
| `-BuiltinType 0xcbf156b50 '__clang_svint64x3_t'
|-TypedefDecl 0xcbf274270 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint8x3_t '__clang_svuint8x3_t'
| `-BuiltinType 0xcbf156b70 '__clang_svuint8x3_t'
|-TypedefDecl 0xcbf2742d8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint16x3_t '__clang_svuint16x3_t'
| `-BuiltinType 0xcbf156b90 '__clang_svuint16x3_t'
|-TypedefDecl 0xcbf274340 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint32x3_t '__clang_svuint32x3_t'
| `-BuiltinType 0xcbf156bb0 '__clang_svuint32x3_t'
|-TypedefDecl 0xcbf2743a8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint64x3_t '__clang_svuint64x3_t'
| `-BuiltinType 0xcbf156bd0 '__clang_svuint64x3_t'
|-TypedefDecl 0xcbf274410 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat16x3_t '__clang_svfloat16x3_t'
| `-BuiltinType 0xcbf156bf0 '__clang_svfloat16x3_t'
|-TypedefDecl 0xcbf274478 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat32x3_t '__clang_svfloat32x3_t'
| `-BuiltinType 0xcbf156c10 '__clang_svfloat32x3_t'
|-TypedefDecl 0xcbf2744e0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat64x3_t '__clang_svfloat64x3_t'
| `-BuiltinType 0xcbf156c30 '__clang_svfloat64x3_t'
|-TypedefDecl 0xcbf274548 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svbfloat16x3_t '__clang_svbfloat16x3_t'
| `-BuiltinType 0xcbf156c50 '__clang_svbfloat16x3_t'
|-TypedefDecl 0xcbf2745b0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svmfloat8x3_t '__clang_svmfloat8x3_t'
| `-BuiltinType 0xcbf156c70 '__clang_svmfloat8x3_t'
|-TypedefDecl 0xcbf274618 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint8x4_t '__clang_svint8x4_t'
| `-BuiltinType 0xcbf156c90 '__clang_svint8x4_t'
|-TypedefDecl 0xcbf274680 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint16x4_t '__clang_svint16x4_t'
| `-BuiltinType 0xcbf156cb0 '__clang_svint16x4_t'
|-TypedefDecl 0xcbf2746e8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint32x4_t '__clang_svint32x4_t'
| `-BuiltinType 0xcbf156cd0 '__clang_svint32x4_t'
|-TypedefDecl 0xcbf274750 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svint64x4_t '__clang_svint64x4_t'
| `-BuiltinType 0xcbf156cf0 '__clang_svint64x4_t'
|-TypedefDecl 0xcbf2747b8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint8x4_t '__clang_svuint8x4_t'
| `-BuiltinType 0xcbf156d10 '__clang_svuint8x4_t'
|-TypedefDecl 0xcbf274820 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint16x4_t '__clang_svuint16x4_t'
| `-BuiltinType 0xcbf156d30 '__clang_svuint16x4_t'
|-TypedefDecl 0xcbf274888 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint32x4_t '__clang_svuint32x4_t'
| `-BuiltinType 0xcbf156d50 '__clang_svuint32x4_t'
|-TypedefDecl 0xcbf2748f0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svuint64x4_t '__clang_svuint64x4_t'
| `-BuiltinType 0xcbf156d70 '__clang_svuint64x4_t'
|-TypedefDecl 0xcbf274958 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat16x4_t '__clang_svfloat16x4_t'
| `-BuiltinType 0xcbf156d90 '__clang_svfloat16x4_t'
|-TypedefDecl 0xcbf2749c0 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat32x4_t '__clang_svfloat32x4_t'
| `-BuiltinType 0xcbf156db0 '__clang_svfloat32x4_t'
|-TypedefDecl 0xcbf274a28 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svfloat64x4_t '__clang_svfloat64x4_t'
| `-BuiltinType 0xcbf156dd0 '__clang_svfloat64x4_t'
|-TypedefDecl 0xcbf274a90 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svbfloat16x4_t '__clang_svbfloat16x4_t'
| `-BuiltinType 0xcbf156df0 '__clang_svbfloat16x4_t'
|-TypedefDecl 0xcbf274af8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svmfloat8x4_t '__clang_svmfloat8x4_t'
| `-BuiltinType 0xcbf156e10 '__clang_svmfloat8x4_t'
|-TypedefDecl 0xcbf274b60 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVBool_t '__SVBool_t'
| `-BuiltinType 0xcbf156e30 '__SVBool_t'
|-TypedefDecl 0xcbf274bc8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svboolx2_t '__clang_svboolx2_t'
| `-BuiltinType 0xcbf156e50 '__clang_svboolx2_t'
|-TypedefDecl 0xcbf274c30 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __clang_svboolx4_t '__clang_svboolx4_t'
| `-BuiltinType 0xcbf156e70 '__clang_svboolx4_t'
|-TypedefDecl 0xcbf274c98 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __SVCount_t '__SVCount_t'
| `-BuiltinType 0xcbf156e90 '__SVCount_t'
|-TypedefDecl 0xcbf274d00 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __mfp8 '__mfp8'
| `-BuiltinType 0xcbf156eb0 '__mfp8'
|-TypedefDecl 0xcbf156fa8 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __builtin_ms_va_list 'char *'
| `-PointerType 0xcbf156f60 'char *'
|   `-BuiltinType 0xcbf1560b0 'char'
|-TypedefDecl 0xcbf1f2018 &lt;&lt;invalid sloc&gt;&gt; &lt;invalid sloc&gt; implicit __builtin_va_list 'char *'
| `-PointerType 0xcbf156f60 'char *'
|   `-BuiltinType 0xcbf1560b0 'char'
`-TypedefDecl 0xcbf2a1398 &lt;platform/test.h:9:1, &lt;invalid sloc&gt;&gt; col:9 invalid bool 'int (int *)'PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/homebrew/opt/llvm/bin/clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics platform/test.h
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x000000011741cb04 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x3e48b04)
 #<!-- -->1 0x00000001136f3f4c llvm::sys::CleanupOnSignal(unsigned long) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x11ff4c)
 #<!-- -->2 0x00000001173f6ee8 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x3e22ee8)
 #<!-- -->3 0x00000001173f6fc4 CrashRecoverySignalHandler(int) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x3e22fc4)
 #<!-- -->4 0x000000018d069764 (/usr/lib/system/libsystem_platform.dylib+0x1804e5764)
 #<!-- -->5 0x0000000106954340 clang::comments::Sema::resolveParamCommandIndexes(clang::comments::FullComment const*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x480340)
 #<!-- -->6 0x0000000106954340 clang::comments::Sema::resolveParamCommandIndexes(clang::comments::FullComment const*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x480340)
 #<!-- -->7 0x00000001069540bc clang::comments::Sema::actOnFullComment(llvm::ArrayRef&lt;clang::comments::BlockContentComment*&gt;) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x4800bc)
 #<!-- -->8 0x0000000106950078 clang::comments::Parser::parseFullComment() (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x47c078)
 #<!-- -->9 0x0000000106cbeebc clang::RawComment::parse(clang::ASTContext const&amp;, clang::Preprocessor const*, clang::Decl const*) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x7eaebc)
#<!-- -->10 0x00000001068d18c4 clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*, bool)::'lambda'()::operator()() const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x3fd8c4)
#<!-- -->11 0x00000001068d5b74 void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*, bool)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*, bool)::'lambda'())::'lambda'(bool)::operator()(bool) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x401b74)
#<!-- -->12 0x00000001068d159c void clang::TextTreeStructure::AddChild&lt;clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*, bool)::'lambda'()&gt;(llvm::StringRef, clang::ASTNodeTraverser&lt;clang::ASTDumper, clang::TextNodeDumper&gt;::Visit(clang::Decl const*, bool)::'lambda'()) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x3fd59c)
#<!-- -->13 0x00000001068d0978 clang::Decl::dump(llvm::raw_ostream&amp;, bool, clang::ASTDumpOutputFormat) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x3fc978)
#<!-- -->14 0x0000000107e6e728 (anonymous namespace)::ASTPrinter::print(clang::Decl*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x199a728)
#<!-- -->15 0x00000001066abd88 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1d7d88)
#<!-- -->16 0x0000000107ef3b44 clang::FrontendAction::Execute() (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1a1fb44)
#<!-- -->17 0x0000000107e8af64 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x19b6f64)
#<!-- -->18 0x0000000107f28dd0 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1a54dd0)
#<!-- -->19 0x00000001021f53a0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x1000053a0)
#<!-- -->20 0x00000001021f3550 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x100003550)
#<!-- -->21 0x0000000107bf4360 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x1720360)
#<!-- -->22 0x0000000113646cfc llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libLLVM.dylib+0x72cfc)
#<!-- -->23 0x0000000107bf3e98 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x171fe98)
#<!-- -->24 0x0000000107bbde34 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x16e9e34)
#<!-- -->25 0x0000000107bda8b8 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/homebrew/Cellar/llvm/21.1.8_1/lib/libclang-cpp.dylib+0x17068b8)
#<!-- -->26 0x00000001021f2550 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x100002550)
#<!-- -->27 0x00000001021fd810 main (/opt/homebrew/Cellar/llvm/21.1.8_1/bin/clang-21+0x10000d810)
#<!-- -->28 0x000000018cc95d54
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Homebrew clang version 21.1.8
Target: arm64-apple-darwin25.3.0
Thread model: posix
InstalledDir: /opt/homebrew/Cellar/llvm/21.1.8_1/bin
Configuration file: /opt/homebrew/etc/clang/arm64-apple-darwin25.cfg
clang: error: unable to execute command: Segmentation fault: 11
clang: note: diagnostic msg: Error generating preprocessed source(s).
```
&lt;/details&gt;

I tried clang `21.1.8` and `17.0.0` (`21.1.8` was installed by brew as a latest available)

</details>


---

### Comment 6 - TPPPP72

I'd like to work on it @AaronBallman 

---

### Comment 7 - shafik

> I'd like to work on it [@AaronBallman](https://github.com/AaronBallman)

Please feel free to work on this issue. We don't assign issues until there is a PR and we have checked it is moving in the right direction. 

Please make sure you read: https://www.llvm.org/docs/DeveloperPolicy.html

---

