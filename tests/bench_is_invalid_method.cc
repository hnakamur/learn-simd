#include <benchmark/benchmark.h>
#include <emmintrin.h>
#include <smmintrin.h>

static int is_invalid_method1(__m128i bv, int n) {
  // assert(n > 0 && n <= 16);
  int m = (1 << n) - 1;
  return (_mm_movemask_epi8(_mm_andnot_si128(
              _mm_cmpeq_epi8(bv, _mm_set1_epi8('-')),
              _mm_andnot_si128(
                  _mm_cmpeq_epi8(bv, _mm_set1_epi8('_')),
                  _mm_or_si128(_mm_cmplt_epi8(bv, _mm_set1_epi8('A')),
                               _mm_cmpgt_epi8(bv, _mm_set1_epi8('Z')))))) &
          m) != 0;
}

static int is_invalid_method2(__m128i bv, int n) {
  // assert(n > 0 && n <= 16);
  int m = (1 << n) - 1;
  int mask1 =
      _mm_movemask_epi8(_mm_or_si128(_mm_cmplt_epi8(bv, _mm_set1_epi8('A')),
                                     _mm_cmpgt_epi8(bv, _mm_set1_epi8('Z')))) &
      m;
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

static int is_invalid_method3(__m128i bv, int n) {
  // assert(n > 0 && n <= 16);
  int m = (1 << n) - 1;
  __m128i mask1v = _mm_or_si128(_mm_cmplt_epi8(bv, _mm_set1_epi8('A')),
                                _mm_cmpgt_epi8(bv, _mm_set1_epi8('Z')));
  if ((_mm_movemask_epi8(mask1v) & m) == 0) {
    return 0;
  }

  mask1v = _mm_andnot_si128(_mm_cmpeq_epi8(bv, _mm_set1_epi8('_')), mask1v);
  if ((_mm_movemask_epi8(mask1v) & m) == 0) {
    return 0;
  }

  mask1v = _mm_andnot_si128(_mm_cmpeq_epi8(bv, _mm_set1_epi8('-')), mask1v);
  return (_mm_movemask_epi8(mask1v) & m) != 0;
}

alignas(16) uint8_t masks[16][16] = {
    {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF},
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

static int is_invalid_method4(__m128i bv, int n) {
  // assert(n > 0 && n <= 16);
  __m128i m1 = _mm_loadu_si128((__m128i *)masks[n - 1]);
  // debug_print_m128i("m1  ", m1);

  __m128i m2 = _mm_or_si128(_mm_cmplt_epi8(bv, _mm_set1_epi8('A')),
                            _mm_cmpgt_epi8(bv, _mm_set1_epi8('Z')));
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

static int is_invalid_method5(__m128i bv, int n) {
  // assert(n > 0 && n <= 16);
  __m128i m1;
  switch (n) {
  case 1:
    m1 = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                      '\0', '\0', '\0', '\0', '\0', '\0', '\xFF');
    break;
  case 2:
    m1 = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                      '\0', '\0', '\0', '\0', '\0', '\xFF', '\xFF');
    break;
  case 3:
    m1 = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                      '\0', '\0', '\0', '\0', '\xFF', '\xFF', '\xFF');
    break;
  case 4:
    m1 = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                      '\0', '\0', '\0', '\xFF', '\xFF', '\xFF', '\xFF');
    break;
  case 5:
    m1 = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                      '\0', '\0', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
    break;
  case 6:
    m1 = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                      '\0', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
    break;
  case 7:
    m1 = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                      '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
    break;
  case 8:
    m1 = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\xFF',
                      '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
    break;
  case 9:
    m1 = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\xFF', '\xFF',
                      '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
    break;
  case 10:
    m1 =
        _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\xFF', '\xFF', '\xFF',
                     '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF');
    break;
  case 11:
    m1 = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\xFF', '\xFF', '\xFF',
                      '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF',
                      '\xFF');
    break;
  case 12:
    m1 = _mm_set_epi8('\0', '\0', '\0', '\0', '\xFF', '\xFF', '\xFF', '\xFF',
                      '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF',
                      '\xFF');
    break;
  case 13:
    m1 = _mm_set_epi8('\0', '\0', '\0', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF',
                      '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF',
                      '\xFF');
    break;
  case 14:
    m1 = _mm_set_epi8('\0', '\0', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF',
                      '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF',
                      '\xFF', '\xFF');
    break;
  case 15:
    m1 = _mm_set_epi8('\0', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF',
                      '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF',
                      '\xFF', '\xFF');
    break;
  case 16:
    m1 = _mm_set_epi8('\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF',
                      '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF', '\xFF',
                      '\xFF', '\xFF');
    break;
  }
  // debug_print_m128i("m1  ", m1);

  __m128i m2 = _mm_or_si128(_mm_cmplt_epi8(bv, _mm_set1_epi8('A')),
                            _mm_cmpgt_epi8(bv, _mm_set1_epi8('Z')));
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

static void BM_is_invalid_method1_case1(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'Y', 'R', 'E', 'U', 'Q');
  int n = 5;

  for (auto _ : state) {
    benchmark::ClobberMemory();
    auto rc = is_invalid_method1(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method1_case1);

static void BM_is_invalid_method2_case1(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'Y', 'R', 'E', 'U', 'Q');
  int n = 5;

  for (auto _ : state) {
    benchmark::ClobberMemory();
    auto rc = is_invalid_method2(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method2_case1);

static void BM_is_invalid_method3_case1(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'Y', 'R', 'E', 'U', 'Q');
  int n = 5;

  for (auto _ : state) {
    benchmark::ClobberMemory();
    auto rc = is_invalid_method3(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method3_case1);

static void BM_is_invalid_method4_case1(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'Y', 'R', 'E', 'U', 'Q');
  int n = 5;

  for (auto _ : state) {
    benchmark::ClobberMemory();
    auto rc = is_invalid_method4(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method4_case1);

static void BM_is_invalid_method5_case1(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'Y', 'R', 'E', 'U', 'Q');
  int n = 5;

  for (auto _ : state) {
    benchmark::ClobberMemory();
    auto rc = is_invalid_method5(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method5_case1);

static void BM_is_invalid_method1_case2(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'C', '-', 'B', '_', 'A');
  int n = 5;

  for (auto _ : state) {
    auto rc = is_invalid_method1(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method1_case2);

static void BM_is_invalid_method2_case2(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'C', '-', 'B', '_', 'A');
  int n = 5;

  for (auto _ : state) {
    auto rc = is_invalid_method2(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method2_case2);

static void BM_is_invalid_method3_case2(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'C', '-', 'B', '_', 'A');
  int n = 5;

  for (auto _ : state) {
    auto rc = is_invalid_method3(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method3_case2);

static void BM_is_invalid_method4_case2(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'C', '-', 'B', '_', 'A');
  int n = 5;

  for (auto _ : state) {
    auto rc = is_invalid_method4(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method4_case2);

static void BM_is_invalid_method5_case2(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'C', '-', 'B', '_', 'A');
  int n = 5;

  for (auto _ : state) {
    auto rc = is_invalid_method5(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method5_case2);

static void BM_is_invalid_method1_case3(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '!', 'C', '-', 'B', '_', 'A');
  int n = 6;

  for (auto _ : state) {
    auto rc = is_invalid_method1(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method1_case3);

static void BM_is_invalid_method2_case3(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '!', 'C', '-', 'B', '_', 'A');
  int n = 6;

  for (auto _ : state) {
    auto rc = is_invalid_method2(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method2_case3);

static void BM_is_invalid_method3_case3(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '!', 'C', '-', 'B', '_', 'A');
  int n = 6;

  for (auto _ : state) {
    auto rc = is_invalid_method3(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method3_case3);

static void BM_is_invalid_method4_case3(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '!', 'C', '-', 'B', '_', 'A');
  int n = 6;

  for (auto _ : state) {
    auto rc = is_invalid_method4(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method4_case3);

static void BM_is_invalid_method5_case3(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '!', 'C', '-', 'B', '_', 'A');
  int n = 6;

  for (auto _ : state) {
    auto rc = is_invalid_method5(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method5_case3);

static void BM_is_invalid_method1_case4(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'C', '-', 'B', '_', 'a');
  int n = 5;

  for (auto _ : state) {
    auto rc = is_invalid_method1(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method1_case4);

static void BM_is_invalid_method2_case4(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'C', '-', 'B', '_', 'a');
  int n = 5;

  for (auto _ : state) {
    auto rc = is_invalid_method2(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method2_case4);

static void BM_is_invalid_method3_case4(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'C', '-', 'B', '_', 'a');
  int n = 5;

  for (auto _ : state) {
    auto rc = is_invalid_method3(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method3_case4);

static void BM_is_invalid_method4_case4(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'C', '-', 'B', '_', 'a');
  int n = 5;

  for (auto _ : state) {
    auto rc = is_invalid_method4(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method4_case4);

static void BM_is_invalid_method5_case4(benchmark::State &state) {
  __m128i method = _mm_set_epi8('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                                '\0', '\0', '\0', 'C', '-', 'B', '_', 'a');
  int n = 5;

  for (auto _ : state) {
    auto rc = is_invalid_method5(method, n);
    benchmark::DoNotOptimize(rc);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_is_invalid_method5_case4);

BENCHMARK_MAIN();
