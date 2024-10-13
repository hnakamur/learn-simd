#include <assert.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

void bench_escape(void *p) {
    asm volatile("" : : "g"(p) : "memory");
}

void bench_clobber() {
    asm volatile("":::"memory");
}

static int
is_invalid_method_scalar(uint8_t b[], int n)
{
    for (int i = 0; i < n; i++) {
        uint8_t ch = b[i];
        if ((ch < 'A' || ch > 'Z') && ch != '_' && ch != '-') {
            return 1;
        }
    }
    return 0;
}

static int
is_invalid_method1(__m128i bv, int n)
{
    // assert(n > 0 && n <= 16);
    int m = (1 << n) - 1;
    return (_mm_movemask_epi8(
        _mm_andnot_si128(
            _mm_cmpeq_epi8(bv, _mm_set1_epi8('-')),
            _mm_andnot_si128(
                _mm_cmpeq_epi8(bv, _mm_set1_epi8('_')),
                _mm_or_si128(
                    _mm_cmplt_epi8(bv, _mm_set1_epi8('A')),
                    _mm_cmpgt_epi8(bv, _mm_set1_epi8('Z'))
                )
            )
        )
    ) & m) != 0;
}

static int
is_invalid_method2(__m128i bv, int n)
{
    // assert(n > 0 && n <= 16);
    int m = (1 << n) - 1;
    int mask1 = _mm_movemask_epi8(
        _mm_or_si128(
            _mm_cmplt_epi8(bv, _mm_set1_epi8('A')),
            _mm_cmpgt_epi8(bv, _mm_set1_epi8('Z'))
        )
    ) & m;
    if (mask1 == 0) {
        return 0;
    }

    mask1 &= ~(_mm_movemask_epi8(_mm_cmpeq_epi8(bv, _mm_set1_epi8('-'))) & m);
    if (mask1 == 0) {
        return 0;
    }

    mask1 &= ~(_mm_movemask_epi8(_mm_cmpeq_epi8(bv, _mm_set1_epi8('_'))) & m);
    return mask1 != 0;
}

static int
is_invalid_method3(__m128i bv, int n)
{
    // assert(n > 0 && n <= 16);
    int m = (1 << n) - 1;
    __m128i mask1v = _mm_or_si128(
        _mm_cmplt_epi8(bv, _mm_set1_epi8('A')),
        _mm_cmpgt_epi8(bv, _mm_set1_epi8('Z'))
    );
    if ((_mm_movemask_epi8(mask1v) & m) == 0) {
        return 0;
    }

    mask1v = _mm_andnot_si128(
        _mm_cmpeq_epi8(bv, _mm_set1_epi8('_')),
        mask1v);
    if ((_mm_movemask_epi8(mask1v) & m) == 0) {
        return 0;
    }

    mask1v = _mm_andnot_si128(
        _mm_cmpeq_epi8(bv, _mm_set1_epi8('-')),
        mask1v);
    return (_mm_movemask_epi8(mask1v) & m) != 0;
}

alignas(16) uint8_t masks[16][16] = {
    {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
};

void debug_print_m128i(const char *label, __m128i m) {
    uint8_t b[16];

    printf("%s", label);
    _mm_storeu_si128((__m128i *)b, m);
    for (int i = 0; i < 16; i++) {
        printf(" 0x%02x", b[i]);
    }
    printf("\n");
}

static int
is_invalid_method4(__m128i bv, int n)
{
    // assert(n > 0 && n <= 16);
    __m128i m1 = _mm_loadu_si128((__m128i *)masks[n - 1]);
    // debug_print_m128i("m1  ", m1);

    __m128i m2 = _mm_or_si128(
        _mm_cmplt_epi8(bv, _mm_set1_epi8('A')),
        _mm_cmpgt_epi8(bv, _mm_set1_epi8('Z'))
    );
    // debug_print_m128i("m2#1", m2);
    if (_mm_testz_si128(m1, m2)) {
        return 0;
    }

    // debug_print_m128i("bv_ ", _mm_cmpeq_epi8(bv, _mm_set1_epi8('_')));
    m2 = _mm_andnot_si128(_mm_cmpeq_epi8(bv, _mm_set1_epi8('_')), m2);
    // debug_print_m128i("m2#2", m2);
    if (_mm_testz_si128(m1, m2)) {
        return 0;
    }

    m2 = _mm_andnot_si128(_mm_cmpeq_epi8(bv, _mm_set1_epi8('-')), m2);
    // debug_print_m128i("m2#3", m2);
    return !_mm_testz_si128(m1, m2);
}

static int
is_invalid_method5(__m128i bv, int n)
{
    // assert(n > 0 && n <= 16);
    __m128i m1;
    switch (n) {
    case 1:
        m1 = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\xFF');
        break;
    case 2:
        m1 = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', '\0', '\0', '\0', '\xFF', '\xFF');
        break;
    case 3:
        m1 = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', '\0', '\0', '\xFF', '\xFF', '\xFF');
        break;
    case 4:
        m1 = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', '\0', '\xFF', '\xFF', '\xFF', '\xFF');
        break;
    case 5:
        m1 = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
        break;
    case 6:
        m1 = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\0', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
        break;
    case 7:
        m1 = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
        break;
    case 8:
        m1 = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
        break;
    case 9:
        m1 = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\xFF',
            '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
        break;
    case 10:
        m1 = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\xFF', '\xFF',
            '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
        break;
    case 11:
        m1 = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\xFF', '\xFF', '\xFF',
            '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
        break;
    case 12:
        m1 = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\xFF', '\xFF', '\xFF', '\xFF',
            '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
        break;
    case 13:
        m1 = _mm_set_epi8(
            '\0', '\0', '\0', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF',
            '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
        break;
    case 14:
        m1 = _mm_set_epi8(
            '\0', '\0', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF',
            '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
        break;
    case 15:
        m1 = _mm_set_epi8(
            '\0', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF',
            '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
        break;
    case 16:
        m1 = _mm_set_epi8(
            '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF',
            '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
        break;
    }
    // debug_print_m128i("m1  ", m1);

    __m128i m2 = _mm_or_si128(
        _mm_cmplt_epi8(bv, _mm_set1_epi8('A')),
        _mm_cmpgt_epi8(bv, _mm_set1_epi8('Z'))
    );
    // debug_print_m128i("m2#1", m2);
    if (_mm_testz_si128(m1, m2)) {
        return 0;
    }

    // debug_print_m128i("bv_ ", _mm_cmpeq_epi8(bv, _mm_set1_epi8('_')));
    m2 = _mm_andnot_si128(_mm_cmpeq_epi8(bv, _mm_set1_epi8('_')), m2);
    // debug_print_m128i("m2#2", m2);
    if (_mm_testz_si128(m1, m2)) {
        return 0;
    }

    m2 = _mm_andnot_si128(_mm_cmpeq_epi8(bv, _mm_set1_epi8('-')), m2);
    // debug_print_m128i("m2#3", m2);
    return !_mm_testz_si128(m1, m2);
}

#define BENCH_REPEAT_COUNT (1000 * 1000 * 1000)

uint64_t bench_is_invalid_method_scalar(uint8_t b[], int n) {
    uint64_t sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        bench_escape(b);
        bench_escape(&n);
        int rc = is_invalid_method_scalar(b, n);
        sum = sum * 13 + rc;
        bench_clobber();
    }
    return sum;
}

uint64_t bench_is_invalid_method1(__m128i method, int n) {
    uint64_t sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        bench_escape(&method);
        bench_escape(&n);
        int rc = is_invalid_method1(method, n);
        sum = sum * 13 + rc;
        bench_clobber();
    }
    return sum;
}

uint64_t bench_is_invalid_method2(__m128i method, int n) {
    uint64_t sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        bench_escape(&method);
        bench_escape(&n);
        int rc = is_invalid_method2(method, n);
        sum = sum * 13 + rc;
        bench_clobber();
    }
    return sum;
}

uint64_t bench_is_invalid_method3(__m128i method, int n) {
    uint64_t sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        bench_escape(&method);
        bench_escape(&n);
        int rc = is_invalid_method3(method, n);
        sum = sum * 13 + rc;
        bench_clobber();
    }
    return sum;
}

uint64_t bench_is_invalid_method4(__m128i method, int n) {
    uint64_t sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        bench_escape(&method);
        bench_escape(&n);
        int rc = is_invalid_method4(method, n);
        sum = sum * 13 + rc;
        bench_clobber();
    }
    return sum;
}

uint64_t bench_is_invalid_method5(__m128i method, int n) {
    uint64_t sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        bench_escape(&method);
        bench_escape(&n);
        int rc = is_invalid_method5(method, n);
        sum = sum * 13 + rc;
        bench_clobber();
    }
    return sum;
}

double get_elapsed_time(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

void bench_scalar(const char *name, uint8_t b[], int n) {
    struct timespec start, end;
    double elapsed_time;

    clock_gettime(CLOCK_MONOTONIC, &start);
    uint64_t rc = bench_is_invalid_method_scalar(b, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = get_elapsed_time(start, end);
    printf("%s\t%.9fs rc=%lu\n", name, elapsed_time, rc);
}

void bench(const char *name, uint64_t (*f)(__m128i method, int n), __m128i method, int n) {
    struct timespec start, end;
    double elapsed_time;

    clock_gettime(CLOCK_MONOTONIC, &start);
    uint64_t rc = f(method, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = get_elapsed_time(start, end);
    printf("%s\t%.9fs rc=%lu\n", name, elapsed_time, rc);
}

void test_all() {
    {
        __m128i method = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', 'Y', 'R', 'E', 'U', 'Q');
        int n = 5;
        assert(!is_invalid_method1(method, n));
        assert(!is_invalid_method2(method, n));
        assert(!is_invalid_method3(method, n));
        assert(!is_invalid_method4(method, n));
        assert(!is_invalid_method5(method, n));
    }
    {
        __m128i method = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', 'C', '-', 'B', '_', 'A');
        int n = 5;
        assert(!is_invalid_method1(method, n));
        assert(!is_invalid_method2(method, n));
        assert(!is_invalid_method3(method, n));
        assert(!is_invalid_method4(method, n));
        assert(!is_invalid_method5(method, n));
    }
    {
        __m128i method = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\0', '!', 'C', '-', 'B', '_', 'A');
        int n = 6;
        assert(is_invalid_method1(method, n));
        assert(is_invalid_method2(method, n));
        assert(is_invalid_method3(method, n));
        assert(is_invalid_method4(method, n));
        assert(is_invalid_method5(method, n));
    }
    {
        __m128i method = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', 'C', '-', 'B', '_', 'a');
        int n = 5;
        assert(is_invalid_method1(method, n));
        assert(is_invalid_method2(method, n));
        assert(is_invalid_method3(method, n));
        assert(is_invalid_method4(method, n));
        assert(is_invalid_method5(method, n));
    }
}

void bench_all() {
    {
        __m128i method = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', 'Y', 'R', 'E', 'U', 'Q');
        int n = 5;
        uint8_t b[5] = {'Q', 'U', 'E', 'R', 'Y'};

        printf("case1\n");
        bench_scalar("is_invalid_method_scalar", b, n);
        bench("is_invalid_method1\t", bench_is_invalid_method1, method, n);
        bench("is_invalid_method2\t", bench_is_invalid_method2, method, n);
        bench("is_invalid_method3\t", bench_is_invalid_method3, method, n);
        bench("is_invalid_method4\t", bench_is_invalid_method4, method, n);
        bench("is_invalid_method5\t", bench_is_invalid_method5, method, n);
    }
    {
        __m128i method = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', 'C', '-', 'B', '_', 'A');
        int n = 5;
        uint8_t b[5] = {'A', '_', 'B', '-', 'C'};

        printf("case2\n");
        bench_scalar("is_invalid_method_scalar", b, n);
        bench("is_invalid_method1\t", bench_is_invalid_method1, method, n);
        bench("is_invalid_method2\t", bench_is_invalid_method2, method, n);
        bench("is_invalid_method3\t", bench_is_invalid_method3, method, n);
        bench("is_invalid_method4\t", bench_is_invalid_method4, method, n);
        bench("is_invalid_method5\t", bench_is_invalid_method5, method, n);
    }
    {
        __m128i method = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\0', '!', 'C', '-', 'B', '_', 'A');
        int n = 6;
        uint8_t b[6] = {'A', '_', 'B', '-', 'C', '!'};

        printf("case3\n");
        bench_scalar("is_invalid_method_scalar", b, n);
        bench("is_invalid_method1\t", bench_is_invalid_method1, method, n);
        bench("is_invalid_method2\t", bench_is_invalid_method2, method, n);
        bench("is_invalid_method3\t", bench_is_invalid_method3, method, n);
        bench("is_invalid_method4\t", bench_is_invalid_method4, method, n);
        bench("is_invalid_method5\t", bench_is_invalid_method5, method, n);
    }
    {
        __m128i method = _mm_set_epi8(
            '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
            '\0', '\0', '\0', 'C', '-', 'B', '_', 'a');
        int n = 5;
        uint8_t b[5] = {'a', '_', 'B', '-', 'C'};
        printf("case4\n");
        bench_scalar("is_invalid_method_scalar", b, n);
        bench("is_invalid_method1\t", bench_is_invalid_method1, method, n);
        bench("is_invalid_method2\t", bench_is_invalid_method2, method, n);
        bench("is_invalid_method3\t", bench_is_invalid_method3, method, n);
        bench("is_invalid_method4\t", bench_is_invalid_method4, method, n);
        bench("is_invalid_method5\t", bench_is_invalid_method5, method, n);
    }
}

int main() {
    test_all();
    bench_all();
}
