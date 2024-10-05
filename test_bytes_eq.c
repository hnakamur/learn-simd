#include "bytes.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double get_elapsed_time(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

void test_bytes_eq(const uint8_t s1[], const uint8_t s2 [], int n) {
    int rc = bytes_eq(s1, s2, n);
    printf("rc=%d, s1=%.*s, s2=%.*s, n=%d\n", rc, n, s1, n, s2, n);
}

int bench_memcmp4(const uint8_t s1[], const uint8_t s2 [], int n) {
    int sum = 0;
    for (int i = 0; i < 1000000; i++) {
        sum += (memcmp(s1, s2, n) == 0);
    }
    return sum;
}

int bench_bytes_eq4(const uint8_t s1[], const uint8_t s2 [], int n) {
    int sum = 0;
    for (int i = 0; i < 1000000; i++) {
        sum += bytes_eq(s1, s2, n);
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
    printf("%s %.9fs rc=%d, s1=%s, s2=%s, n=%d\n", name, elapsed_time, rc, s1, s2, n);
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

    bench("bytes_eq4", bench_bytes_eq4, argv[1], argv[2], n);
    bench("memcmp4", bench_memcmp4, argv[1], argv[2], n);

    return 0;   
}
