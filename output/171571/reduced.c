#include <stdint.h>
#include <stdio.h>
#define BS_VEC(type, num) type __attribute__((vector_size(num * sizeof(type))))
uint64_t BS_CHECKSUM_ARR[10];
BS_VEC(uint64_t, 2)
backsmith_snippet_123(uint16_t, uint8_t BS_ARG_1, BS_VEC(uint8_t, 4) BS_ARG_2)
{
    BS_VEC(uint64_t, 32)
    BS_TEMP_0 = __builtin_convertvector(
        __builtin_shufflevector(BS_ARG_2, BS_ARG_2, 0, 4, 0, 6, 5, 7, 6, 3, 5,
                                2, 7, 7, 7, 4, 4, 3, 2, 6, 1, 2, 5, 2, 6, 1, 6,
                                6, 6, 1, 0, 7, 3, 0),
        BS_VEC(uint64_t, 32));
    BS_ARG_2[BS_TEMP_0[9] + BS_TEMP_0[10] + BS_TEMP_0[11] + BS_TEMP_0[12]
                 + BS_TEMP_0[13] + BS_TEMP_0[14] + BS_TEMP_0[15] + BS_TEMP_0[16]
                 + BS_TEMP_0[17] + BS_TEMP_0[18] + BS_TEMP_0[19] + BS_TEMP_0[20]
                 + BS_TEMP_0[21] + BS_TEMP_0[2] + BS_TEMP_0[3] + BS_TEMP_0[4]
                 + BS_TEMP_0[5] + BS_TEMP_0[6] + BS_TEMP_0[7] + BS_TEMP_0[8]
                 + BS_TEMP_0[29] + BS_TEMP_0[0] + BS_TEMP_0[1]
             < 4] = 0;
    BS_ARG_1 = (__builtin_convertvector(
                    __builtin_shufflevector(BS_ARG_2, BS_ARG_2, 0, 5),
                    BS_VEC(int16_t, 2))
                <= 0)[1];
    return __builtin_convertvector((BS_VEC(uint8_t, 2)){ BS_ARG_1, BS_ARG_1 },
                                   BS_VEC(uint64_t, 2));
}
uint8_t backsmith_pure_0(BS_VEC(int32_t, 16), BS_VEC(int64_t, 4) BS_ARG_1,
                         BS_VEC(uint16_t, 2) BS_ARG_2,
                         BS_VEC(uint8_t, 16) BS_ARG_3)
{
    BS_VEC(uint32_t, 2) BS_VAR_0;
    BS_VEC(int64_t, 8) BS_VAR_1[6];
    uint64_t BS_TEMP_2 = BS_ARG_1[3];
    BS_VAR_1[(BS_TEMP_2 ?: 70396191916439) < 6 ? BS_ARG_3[5] << BS_TEMP_2 : 0] =
        __builtin_convertvector(
            __builtin_shufflevector(BS_ARG_3, BS_ARG_3, 5, 6, 1, 1, 7, 2, 8, 2),
            BS_VEC(int64_t, 8));
    BS_VAR_0[BS_ARG_3[4] < 2] = BS_ARG_2[1];
    uint16_t BS_TEMP_3 = (uint16_t)BS_VAR_0[0]
        ? backsmith_snippet_123(
            1, 0, __builtin_convertvector(BS_ARG_1, BS_VEC(uint8_t, 4)))[1]
        : BS_ARG_1[1];
    if (BS_TEMP_3)
    {
        BS_VEC(uint32_t, 2)
        BS_TEMP_4 = __builtin_convertvector(
            __builtin_shufflevector((BS_VEC(uint16_t, 8)){},
                                    (BS_VEC(uint16_t, 8)){}, 3, 7),
            BS_VEC(uint32_t, 2));
        if (BS_ARG_3[0]) return BS_VAR_1[0][4];
    }
    BS_VAR_1[BS_ARG_2[0]] =
        __builtin_convertvector(__builtin_shufflevector((BS_VEC(uint16_t, 4)){},
                                                        (BS_VEC(uint16_t, 4)){},
                                                        0, 7, 1, 3, 7, 7, 2, 0),
                                BS_VEC(int64_t, 8));
}
int16_t g_8;
const int8_t func_2(int32_t, uint16_t, uint64_t, uint64_t, uint32_t);
int8_t func_1()
{
    int32_t l_17 = 5;
    uint16_t l_2263 = func_2(g_8, 0, g_8, 0, l_17);
    return l_2263;
    __builtin_convertvector(
        __builtin_shufflevector(__builtin_convertvector((BS_VEC(uint8_t, 2)){},
                                                        BS_VEC(uint32_t, 2)),
                                __builtin_convertvector((BS_VEC(uint8_t, 2)){},
                                                        BS_VEC(uint32_t, 2)),
                                2, 3, 3, 0, 0, 0, 1, 0, 2, 1, 2, 1, 2, 3, 1, 1),
        BS_VEC(int64_t, 16));
    __builtin_convertvector(
        __builtin_shufflevector(__builtin_convertvector((BS_VEC(uint32_t, 4)){},
                                                        BS_VEC(uint16_t, 4)),
                                __builtin_convertvector((BS_VEC(uint32_t, 4)){},
                                                        BS_VEC(uint16_t, 4)),
                                4, 6),
        BS_VEC(uint32_t, 2));
}
const int8_t func_2(int32_t p_3, uint16_t, uint64_t, uint64_t, uint32_t p_7)
{
    int16_t BS_VAR_0;
    int8_t BS_VAR_1;
    uint64_t *LOCAL_CHECKSUM = &BS_CHECKSUM_ARR[1];
    uint16_t l_1785 = 3;
    __builtin_convertvector((BS_VEC(uint16_t, 4)){}, BS_VEC(int32_t, 4));
    BS_VAR_1 = backsmith_pure_0(
        __builtin_shufflevector(
            __builtin_convertvector((BS_VEC(uint8_t, 4)){}, BS_VEC(int32_t, 4)),
            (BS_VEC(int32_t, 4)){}, 3, 3, 4, 2, 4, 5, 5, 1, 4, 2, 7, 6, 0, 5, 3,
            7),
        __builtin_convertvector((BS_VEC(uint32_t, 4)){}, BS_VEC(int64_t, 4)),
        __builtin_convertvector(
            __builtin_shufflevector(
                __builtin_convertvector((BS_VEC(uint16_t, 8)){ l_1785, l_1785,
                                                               l_1785, l_1785,
                                                               l_1785, l_1785 },
                                        BS_VEC(uint8_t, 8)),
                __builtin_convertvector((BS_VEC(uint16_t, 8)){},
                                        BS_VEC(uint8_t, 8)),
                3, 5),
            BS_VEC(uint16_t, 2)),
        __builtin_convertvector(
            (BS_VEC(uint32_t, 16)){ p_7, p_7, p_7, p_7, p_7, p_7, p_7, p_7 },
            BS_VEC(uint8_t, 16)));
    BS_VAR_0 = BS_VAR_1;
    *LOCAL_CHECKSUM ^= BS_VAR_0;
    return p_3;
}
int main()
{
    func_1();
    uint64_t BS_CHECKSUM = 5;
    for (uint32_t i = 0; i < 10; i++) BS_CHECKSUM ^= BS_CHECKSUM_ARR[i];
    printf("BackSmith Checksum = 0x%016llx\n", BS_CHECKSUM);
}
