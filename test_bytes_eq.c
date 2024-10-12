#include "bytes.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <immintrin.h>
#include <emmintrin.h>

#define BENCH_REPEAT_COUNT 10000000

double get_elapsed_time(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

void test_bytes_eq(const uint8_t s1[], const uint8_t s2 [], int n) {
    int rc = bytes_eq(s1, s2, n);
    printf("rc=%d, s1=%.*s, s2=%.*s, n=%d\n", rc, n, s1, n, s2, n);
}

int bench_memcmp4(const uint8_t s1[], const uint8_t s2 [], int n) {
    int sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        sum += (i + 1) * (memcmp(s1, s2, n) == 0 ? 1 : 0);
    }
    return sum;
}

int bench_bytes_eq4(const uint8_t s1[], const uint8_t s2 [], int n) {
    int sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        sum += i * bytes_eq(s1, s2, n);
    }
    return sum;
}


int bytes_eq_scalar(const uint8_t s1[], const uint8_t s2 [], int n) {
    while (n-- > 0) {
        if (*s1++ != *s2++) {
            return 0;
        }
    }
    return 1;
}

int bench_bytes_eq_scalar(const uint8_t s1[], const uint8_t s2 [], int n) {
    int sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        sum += (i + 1) * bytes_eq_scalar(s1, s2, n);
    }
    return sum;
}


int bytes_eq_impl2(const uint8_t s1[], const uint8_t s2 [], int n) {
    if (((uintptr_t)s1 & 0xF) == 0 && ((uintptr_t)s2 & 0xF) == 0 && n >= 4) {
        if (*(uint32_t *)s1 != *(uint32_t *)s2) {
            return 0;
        }
        n -= 4;
        if (n == 0) {
            return 1;
        }
        s1 += 4;
        s2 += 4;
    }

    for (; n > 0; n--) {
        if (*s1++ != *s2++) {
            return 0;
        }
    }
    return 1;
}

int bench_bytes_eq_impl2(const uint8_t s1[], const uint8_t s2 [], int n) {
    int sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        sum += (i + 1) * bytes_eq_impl2(s1, s2, n);
    }
    return sum;
}

#if 0
int bytes_eq_impl3(const uint8_t s1[], const uint8_t s2 [], int n) {
    if (n >= 4) {
        if (_loadbe_i32(s1) != _loadbe_i32(s2)) {
            return 0;
        }
        n -= 4;
        if (n == 0) {
            return 1;
        }
    }

    while (n-- > 0) {
        if (*s1++ != *s2++) {
            return 0;
        }
    }
    return 1;
}

int bench_bytes_eq_impl3(const uint8_t s1[], const uint8_t s2 [], int n) {
    int sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        sum += (i + 1) * bytes_eq_impl3(s1, s2, n);
    }
    return sum;
}
#endif


int bytes_eq_impl4(const uint8_t s1[], const uint8_t s2 [], int n) {
    if (n >= 4) {
        __m128i vec1 = _mm_loadu_si32((__m128i *)(s1));
        __m128i vec2 = _mm_loadu_si32((__m128i *)(s2));
        __m128i cmp = _mm_cmpeq_epi8(vec1, vec2);
        int mask = _mm_movemask_epi8(cmp) & 0x0F;
        if (mask != 0x0F) {
            return 0;
        }
        n -= 4;
        if (n == 0) {
            return 1;
        }
    }

    while (n-- > 0) {
        if (*s1++ != *s2++) {
            return 0;
        }
    }
    return 1;
}

int bench_bytes_eq_impl4(const uint8_t s1[], const uint8_t s2 [], int n) {
    int sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        sum += (i + 1) * bytes_eq_impl4(s1, s2, n);
    }
    return sum;
}


int bytes_eq_impl5(const uint8_t s1[], const uint8_t s2 [], int n) {
    while (n >= 4) {
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

    while (n-- > 0) {
        if (*s1++ != *s2++) {
            return 0;
        }
    }
    return 1;
}

int bench_bytes_eq_impl5(const uint8_t s1[], const uint8_t s2 [], int n) {
    int sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        sum += (i + 1) * bytes_eq_impl5(s1, s2, n);
    }
    return sum;
}

void bench(const char *name, int (*f)(const uint8_t s1[], const uint8_t s2 [], int n), const uint8_t s1[], const uint8_t s2 [], int n) {
    struct timespec start, end;
    double elapsed_time;

    clock_gettime(CLOCK_MONOTONIC, &start);
    int rc = f(s1, s2, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = get_elapsed_time(start, end);
    printf("%s\t%.9fs rc=%d, s1=%s, s2=%s, n=%d\n", name, elapsed_time, rc, s1, s2, n);
}

int main(int argc, char *argv[]) {
#if 0
    test_bytes_eq("0123456789abcdefghijklmnpoqrstvwxyz", "0123456789abcdefghijklmnpoqrstvwxyz", 36);
    test_bytes_eq("0123456789abcdefghijklmnpoqrstvwxyz", "0123456789Abcdefghijklmnpoqrstvwxyz", 36);
    test_bytes_eq("0123456789abcdefghijklmnpoqrstvwxyz", "!123456789abcdefghijklmnpoqrstvwxyz", 36);
    test_bytes_eq("0123456789abcdefghijklmnpoqrstvwxyz", "0123456789abcdefghijklmnpoqrstVwxyz", 36);
    test_bytes_eq("0123456789abcdefghijklmnpoqrstvwxyz", "0123456789abcdefghijklmnpoqrstvWxyz", 36);
    test_bytes_eq("0123456789abcdefghijklmnpoqrstvwxyz", "0123456789abcdefghijklmnpoqrstvwXyz", 36);
    test_bytes_eq("hello", "hello", 5);
    test_bytes_eq("hello", "Hello", 5);
    test_bytes_eq("hello", "hEllo", 5);
    test_bytes_eq("hello", "heLlo", 5);
    test_bytes_eq("hello", "helLo", 5);
#endif
    if (argc != 4) {
        fprintf(stderr, "Usage: $0 s1 s2 n\n");
        return 2;
    }
    int n = atoi(argv[3]);

    printf("argv[1]=0x%lx, aligned=%d\n", (uintptr_t)argv[1], ((uintptr_t)argv[1] & 0x3) == 0);
    printf("argv[2]=0x%lx, aligned=%d\n", (uintptr_t)argv[2], ((uintptr_t)argv[2] & 0x3) == 0);
    bench("memcmp4\t", bench_memcmp4, (uint8_t *)argv[1], (uint8_t *)argv[2], n);
#if 0
    bench("bytes_eq_impl3", bench_bytes_eq_impl3, (uint8_t *)argv[1], (uint8_t *)argv[2], n);
#endif
    bench("bytes_eq_impl4", bench_bytes_eq_impl4, (uint8_t *)argv[1], (uint8_t *)argv[2], n);
    bench("bytes_eq_impl5", bench_bytes_eq_impl5, (uint8_t *)argv[1], (uint8_t *)argv[2], n);
    bench("bytes_eq4", bench_bytes_eq4, (uint8_t *)argv[1], (uint8_t *)argv[2], n);
    bench("bytes_eq_scalar", bench_bytes_eq_scalar, (uint8_t *)argv[1], (uint8_t *)argv[2], n);
    bench("bytes_eq_impl2", bench_bytes_eq_impl2, (uint8_t *)argv[1], (uint8_t *)argv[2], n);

    return 0;   
}
