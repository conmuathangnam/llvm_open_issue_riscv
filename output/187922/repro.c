#include <stdio.h>
#include <assert.h>

#ifndef VALIDATION
    #define ASSUME(cond) do{if (!(cond)) __builtin_unreachable();}while(0)
#else
    #define ASSUME(cond) do{assert(cond); }while(0)
#endif

#define max(a,b) ({ __typeof__(a) _a=(a); __typeof__(b) _b=(b); _a>_b?_a:_b; })
#define min(a,b) ({ __typeof__(a) _a=(a); __typeof__(b) _b=(b); _a<_b?_a:_b; })

#define E0 (((int)(signed char)(var_15 ? (unsigned int)min(1, max(0, (int)var_17)) : 2147483648U)) - 1)
#define EB ((signed char)var_0 - 70)
#define E1 (((int)(unsigned short)(max((long long)var_7, 1LL - var_0) ? var_3 : var_4)) - 211)
#define E2 (((int)(_Bool)(var_3 ? (int)arr_4[i_1 - 1][i_1 - 1][i_0] : (int)var_7)) - 1)

long long var_0 = 1675129639484229466LL;
unsigned int var_3 = 898901400U;
unsigned char var_4 = 213;
_Bool var_7 = 0;
int var_15 = -230299294;
unsigned short var_17 = 18302;

short arr_4[20][20][20];
signed char arr_8[20];
int arr_11[20][20][20];
unsigned long long seed = 0;

void hash(unsigned long long v) { seed ^= v + 0x9e3779b9 + (seed<<6) + (seed>>2); }

int main() {
    for(int i=0; i<20; i++) for(int j=0; j<20; j++) for(int k=0; k<20; k++) arr_4[i][j][k] = -14263;

    ASSUME((signed char)E0 == 0);
    ASSUME((signed char)EB == 20);
    ASSUME((EB - E0 + 2ULL) / 3ULL == 7ULL);

    for (signed char i_0 = E0; i_0 < EB; i_0 += 3)
        for (unsigned short i_1 = E1; i_1 < 18; i_1 += 4) {
            ASSUME(1 - E2 == 1ULL);
            for (_Bool i_2 = E2; i_2 < 1; i_2 += 1) {
                arr_8[i_2] = (signed char)arr_4[i_1][i_1][i_0];
                arr_11[i_0][i_0][i_2] = 0;
            }
        }

    for(int i=0; i<20; i++) hash(arr_8[i]);
    for(int i=0; i<20; i++) for(int j=0; j<20; j++) for(int k=0; k<20; k++) hash(arr_11[i][j][k]);

    printf("%llu\n", seed);
    return 0;
}