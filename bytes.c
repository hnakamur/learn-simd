#include "bytes.h"
#include <immintrin.h>
#include <emmintrin.h>

#include <stdio.h>

int index_of_byte(const uint8_t s[], int len, uint8_t b) {
    int pos = 0;

#ifdef __AVX2__
    for (; pos + 32 <= len; pos += 32) {
        __m256i vec = _mm256_loadu_si256((__m256i *)(s + pos));
        __m256i cmp = _mm256_cmpeq_epi8(vec, _mm256_set1_epi8(b));
        int mask = _mm256_movemask_epi8(cmp);
        if (mask != 0) {
            // printf("found with avx2: %d\n", pos + __builtin_ffs(mask) - 1);
            return pos + __builtin_ffs(mask) - 1;
        }
    }
#endif

#ifdef __SSE2__
    for (; pos + 16 <= len; pos += 16) {
        __m128i vec = _mm_loadu_si128((__m128i *)(s + pos));
        __m128i cmp = _mm_cmpeq_epi8(vec, _mm_set1_epi8(b));
        int mask = _mm_movemask_epi8(cmp);
        if (mask != 0) {
            // printf("found with sse2: %d\n", pos + __builtin_ffs(mask) - 1);
            return pos + __builtin_ffs(mask) - 1;   
        }
    }
#endif

    for (; pos < len; pos++) {
        if (s[pos] == b) {
            return pos;
        }
    }
    return -1;
}

int bytes_eq(const uint8_t s1[], const uint8_t s2 [], int n) {
#ifdef __AVX2__
    while (n >= 32) {
        __m256i vec1 = _mm256_loadu_si256((__m256i *)s1);
        __m256i vec2 = _mm256_loadu_si256((__m256i *)s2);
        __m256i cmp = _mm256_cmpeq_epi8(vec1, vec2);
        int mask = _mm256_movemask_epi8(cmp);
        if ((unsigned int)mask != 0xFFFFFFFF) {
            return 0;
        }
        n -= 32;
        if (n == 0) {
            return 1;
        }
        s1 += 32;
        s2 += 32;
    }
#endif

#ifdef __SSE2__
    if (n >= 16) {
        __m128i vec1 = _mm_loadu_si128((__m128i *)s1);
        __m128i vec2 = _mm_loadu_si128((__m128i *)s2);
        __m128i cmp = _mm_cmpeq_epi8(vec1, vec2);
        int mask = _mm_movemask_epi8(cmp);
        if (mask != 0xFFFF) {
            return 0;
        }
        n -= 16;
        if (n == 0) {
            return 1;
        }
        s1 += 16;
        s2 += 16;
    }

    if (n >= 8) {
        __m128i vec1 = _mm_loadu_si64((__m128i *)s1);
        __m128i vec2 = _mm_loadu_si64((__m128i *)s2);
        __m128i cmp = _mm_cmpeq_epi8(vec1, vec2);
        int mask = _mm_movemask_epi8(cmp) & 0xFF;
        if (mask != 0xFF) {
            return 0;
        }
        n -= 8;
        if (n == 0) {
            return 1;
        }
        s1 += 8;
        s2 += 8;
    }

    if (n >= 4) {
        __m128i vec1 = _mm_loadu_si32((__m128i *)s1);
        __m128i vec2 = _mm_loadu_si32((__m128i *)s2);
        __m128i cmp = _mm_cmpeq_epi8(vec1, vec2);
        int mask = _mm_movemask_epi8(cmp) & 0x0F;
        if (mask != 0x0F) {
            return 0;
        }
        n -= 4;
        if (n == 0) {
            return 1;
        }
        s1 += 4;
        s2 += 4;
    }
#endif

    while (n-- > 0) {
        if (*s1++ != *s2++) {
            return 0;
        }
    }
    return 1;
}
#if 0
int bytes_eq(const uint8_t s1[], const uint8_t s2 [], int n) {
    int pos = 0;

#ifdef __AVX2__
    for (; pos + 32 <= n; pos += 32) {
        __m256i vec1 = _mm256_loadu_si256((__m256i *)(s1 + pos));
        __m256i vec2 = _mm256_loadu_si256((__m256i *)(s2 + pos));
        __m256i cmp = _mm256_cmpeq_epi8(vec1, vec2);
        int mask = _mm256_movemask_epi8(cmp);
        if ((unsigned int)mask != 0xFFFFFFFF) {
            return 0;
        }
    }
#endif

#ifdef __SSE2__
    for (; pos + 16 <= n; pos += 16) {
        __m128i vec1 = _mm_loadu_si128((__m128i *)(s1 + pos));
        __m128i vec2 = _mm_loadu_si128((__m128i *)(s2 + pos));
        __m128i cmp = _mm_cmpeq_epi8(vec1, vec2);
        int mask = _mm_movemask_epi8(cmp);
        if (mask != 0xFFFF) {
            return 0;
        }
    }

    for (; pos + 8 <= n; pos += 8) {
        __m128i vec1 = _mm_loadu_si64((__m128i *)(s1 + pos));
        __m128i vec2 = _mm_loadu_si64((__m128i *)(s2 + pos));
        __m128i cmp = _mm_cmpeq_epi8(vec1, vec2);
        int mask = _mm_movemask_epi8(cmp) & 0xFF;
        if (mask != 0xFF) {
            return 0;
        }
    }

    for (; pos + 4 <= n; pos += 4) {
        __m128i vec1 = _mm_loadu_si32((__m128i *)(s1 + pos));
        __m128i vec2 = _mm_loadu_si32((__m128i *)(s2 + pos));
        __m128i cmp = _mm_cmpeq_epi8(vec1, vec2);
        int mask = _mm_movemask_epi8(cmp) & 0x0F;
        if (mask != 0x0F) {
            return 0;
        }
    }
#endif

    for (; pos < n; pos++) {
        if (s1[pos] != s2[pos]) {
            return 0;
        }
    }
    return 1;
}
#endif
