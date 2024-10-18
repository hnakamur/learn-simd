#include "http_parse.h"
#include "greatest.h"
#include <time.h>

SUITE(simd);
SUITE(ngx);
SUITE(bench_simd);
SUITE(bench_ngx);

#define BUFLEN 1024

#define BENCH_REPEAT_COUNT (1000 * 1000 * 1000)

void bench_escape(void *p) {
    asm volatile("" : : "g"(p) : "memory");
}

void bench_clobber() {
    asm volatile("":::"memory");
}

double get_elapsed_time(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

TEST test_simd_get(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "GET / HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    ngx_http_request_t req = {0};
    int rc = http_parse_request_line(&req, &buf);
    ASSERT_EQ(NGX_OK, rc);
    ASSERT_EQ(NGX_HTTP_GET, req.method);
    ASSERT_EQ(NGX_HTTP_VERSION_11, req.http_version);
    ASSERT_EQ(b, req.request_start);
    ASSERT_EQ(b + sizeof("GET") - 2, req.method_end);
    ASSERT_EQ(b + sizeof("GET ") - 1, req.uri_start);
    ASSERT_EQ(b + sizeof("GET /") - 1, req.uri_end);
    ASSERT_EQ(b + strlen(src) - 2, req.request_end);
    PASS();
}

TEST test_ngx_get(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "GET / HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    ngx_http_request_t req = {0};
    int rc = ngx_http_parse_request_line(&req, &buf);
    ASSERT_EQ(NGX_OK, rc);
    ASSERT_EQ(NGX_HTTP_GET, req.method);
    ASSERT_EQ(NGX_HTTP_VERSION_11, req.http_version);
    ASSERT_EQ(b, req.request_start);
    ASSERT_EQ(b + sizeof("GET") - 2, req.method_end);
    ASSERT_EQ(b + sizeof("GET ") - 1, req.uri_start);
    ASSERT_EQ(b + sizeof("GET /") - 1, req.uri_end);
    ASSERT_EQ(b + strlen(src) - 2, req.request_end);
    PASS();
}

TEST test_simd_post(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "POST /hello HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    ngx_http_request_t req = {0};
    int rc = http_parse_request_line(&req, &buf);
    ASSERT_EQ(NGX_OK, rc);
    ASSERT_EQ(NGX_HTTP_POST, req.method);
    ASSERT_EQ(NGX_HTTP_VERSION_11, req.http_version);
    ASSERT_EQ(b, req.request_start);
    ASSERT_EQ(b + sizeof("POST") - 2, req.method_end);
    ASSERT_EQ(b + sizeof("POST ") - 1, req.uri_start);
    ASSERT_EQ(b + sizeof("POST /hello") - 1, req.uri_end);
    ASSERT_EQ(b + strlen(src) - 2, req.request_end);
    PASS();
}

TEST test_ngx_post(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "POST /hello HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    ngx_http_request_t req = {0};
    int rc = ngx_http_parse_request_line(&req, &buf);
    ASSERT_EQ(NGX_OK, rc);
    ASSERT_EQ(NGX_HTTP_POST, req.method);
    ASSERT_EQ(NGX_HTTP_VERSION_11, req.http_version);
    ASSERT_EQ(b, req.request_start);
    ASSERT_EQ(b + sizeof("POST") - 2, req.method_end);
    ASSERT_EQ(b + sizeof("POST ") - 1, req.uri_start);
    ASSERT_EQ(b + sizeof("POST /hello") - 1, req.uri_end);
    ASSERT_EQ(b + strlen(src) - 2, req.request_end);
    PASS();
}

TEST test_simd_query(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "QUERY /hello HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    ngx_http_request_t req = {0};
    req.method = NGX_HTTP_UNKNOWN;
    int rc = http_parse_request_line(&req, &buf);
    ASSERT_EQ(NGX_OK, rc);
    ASSERT_EQ(NGX_HTTP_UNKNOWN, req.method);
    ASSERT_EQ(NGX_HTTP_VERSION_11, req.http_version);
    ASSERT_EQ(b, req.request_start);
    ASSERT_EQ(b + sizeof("QUERY") - 2, req.method_end);
    ASSERT_EQ(b + sizeof("QUERY ") - 1, req.uri_start);
    ASSERT_EQ(b + sizeof("QUERY /hello") - 1, req.uri_end);
    ASSERT_EQ(b + strlen(src) - 2, req.request_end);
    PASS();
}

TEST test_ngx_query(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "QUERY /hello HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    ngx_http_request_t req = {0};
    req.method = NGX_HTTP_UNKNOWN;
    int rc = ngx_http_parse_request_line(&req, &buf);
    ASSERT_EQ(NGX_OK, rc);
    ASSERT_EQ(NGX_HTTP_UNKNOWN, req.method);
    ASSERT_EQ(NGX_HTTP_VERSION_11, req.http_version);
    ASSERT_EQ(b, req.request_start);
    ASSERT_EQ(b + sizeof("QUERY") - 2, req.method_end);
    ASSERT_EQ(b + sizeof("QUERY ") - 1, req.uri_start);
    ASSERT_EQ(b + sizeof("QUERY /hello") - 1, req.uri_end);
    ASSERT_EQ(b + strlen(src) - 2, req.request_end);
    PASS();
}

TEST test_simd_long16(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "LONGLONGLONGLONG /hello HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    ngx_http_request_t req = {0};
    req.method = NGX_HTTP_UNKNOWN;
    int rc = http_parse_request_line(&req, &buf);
    ASSERT_EQ(NGX_OK, rc);
    ASSERT_EQ(NGX_HTTP_UNKNOWN, req.method);
    ASSERT_EQ(NGX_HTTP_VERSION_11, req.http_version);
    ASSERT_EQ(b, req.request_start);
    ASSERT_EQ(b + sizeof("LONGLONGLONGLONG") - 2, req.method_end);
    ASSERT_EQ(b + sizeof("LONGLONGLONGLONG ") - 1, req.uri_start);
    ASSERT_EQ(b + sizeof("LONGLONGLONGLONG /hello") - 1, req.uri_end);
    ASSERT_EQ(b + strlen(src) - 2, req.request_end);
    PASS();
}

TEST test_ngx_long16(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "LONGLONGLONGLONG /hello HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    ngx_http_request_t req = {0};
    req.method = NGX_HTTP_UNKNOWN;
    int rc = ngx_http_parse_request_line(&req, &buf);
    ASSERT_EQ(NGX_OK, rc);
    ASSERT_EQ(NGX_HTTP_UNKNOWN, req.method);
    ASSERT_EQ(NGX_HTTP_VERSION_11, req.http_version);
    ASSERT_EQ(b, req.request_start);
    ASSERT_EQ(b + sizeof("LONGLONGLONGLONG") - 2, req.method_end);
    ASSERT_EQ(b + sizeof("LONGLONGLONGLONG ") - 1, req.uri_start);
    ASSERT_EQ(b + sizeof("LONGLONGLONGLONG /hello") - 1, req.uri_end);
    ASSERT_EQ(b + strlen(src) - 2, req.request_end);
    PASS();
}

TEST test_simd_long17(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "LOONGLONGLONGLONG /hello HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    ngx_http_request_t req = {0};
    req.method = NGX_HTTP_UNKNOWN;
    int rc = http_parse_request_line(&req, &buf);
    ASSERT_EQ(NGX_OK, rc);
    ASSERT_EQ(NGX_HTTP_UNKNOWN, req.method);
    ASSERT_EQ(NGX_HTTP_VERSION_11, req.http_version);
    ASSERT_EQ(b, req.request_start);
    ASSERT_EQ(b + sizeof("LOONGLONGLONGLONG") - 2, req.method_end);
    ASSERT_EQ(b + sizeof("LOONGLONGLONGLONG ") - 1, req.uri_start);
    ASSERT_EQ(b + sizeof("LOONGLONGLONGLONG /hello") - 1, req.uri_end);
    ASSERT_EQ(b + strlen(src) - 2, req.request_end);
    PASS();
}

TEST test_ngx_long17(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "LOONGLONGLONGLONG /hello HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    ngx_http_request_t req = {0};
    req.method = NGX_HTTP_UNKNOWN;
    int rc = ngx_http_parse_request_line(&req, &buf);
    ASSERT_EQ(NGX_OK, rc);
    ASSERT_EQ(NGX_HTTP_UNKNOWN, req.method);
    ASSERT_EQ(NGX_HTTP_VERSION_11, req.http_version);
    ASSERT_EQ(b, req.request_start);
    ASSERT_EQ(b + sizeof("LOONGLONGLONGLONG") - 2, req.method_end);
    ASSERT_EQ(b + sizeof("LOONGLONGLONGLONG ") - 1, req.uri_start);
    ASSERT_EQ(b + sizeof("LOONGLONGLONGLONG /hello") - 1, req.uri_end);
    ASSERT_EQ(b + strlen(src) - 2, req.request_end);
    PASS();
}

TEST test_simd_long33(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "LOONGLONGLONGLONGLONGLONGLONGLONG /hello HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    ngx_http_request_t req = {0};
    req.method = NGX_HTTP_UNKNOWN;
    int rc = http_parse_request_line(&req, &buf);
    ASSERT_EQ(NGX_OK, rc);
    ASSERT_EQ(NGX_HTTP_UNKNOWN, req.method);
    ASSERT_EQ(NGX_HTTP_VERSION_11, req.http_version);
    ASSERT_EQ(b, req.request_start);
    ASSERT_EQ(b + sizeof("LOONGLONGLONGLONGLONGLONGLONGLONG") - 2, req.method_end);
    ASSERT_EQ(b + sizeof("LOONGLONGLONGLONGLONGLONGLONGLONG ") - 1, req.uri_start);
    ASSERT_EQ(b + sizeof("LOONGLONGLONGLONGLONGLONGLONGLONG /hello") - 1, req.uri_end);
    ASSERT_EQ(b + strlen(src) - 2, req.request_end);
    PASS();
}

TEST test_ngx_long33(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "LOONGLONGLONGLONGLONGLONGLONGLONG /hello HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    ngx_http_request_t req = {0};
    req.method = NGX_HTTP_UNKNOWN;
    int rc = ngx_http_parse_request_line(&req, &buf);
    ASSERT_EQ(NGX_OK, rc);
    ASSERT_EQ(NGX_HTTP_UNKNOWN, req.method);
    ASSERT_EQ(NGX_HTTP_VERSION_11, req.http_version);
    ASSERT_EQ(b, req.request_start);
    ASSERT_EQ(b + sizeof("LOONGLONGLONGLONGLONGLONGLONGLONG") - 2, req.method_end);
    ASSERT_EQ(b + sizeof("LOONGLONGLONGLONGLONGLONGLONGLONG ") - 1, req.uri_start);
    ASSERT_EQ(b + sizeof("LOONGLONGLONGLONGLONGLONGLONGLONG /hello") - 1, req.uri_end);
    ASSERT_EQ(b + strlen(src) - 2, req.request_end);
    PASS();
}

SUITE(simd) {
    RUN_TEST(test_simd_get);
    RUN_TEST(test_simd_post);
    RUN_TEST(test_simd_query);
    RUN_TEST(test_simd_long16);
    RUN_TEST(test_simd_long17);
    RUN_TEST(test_simd_long33);
}

SUITE(ngx) {
    RUN_TEST(test_ngx_get);
    RUN_TEST(test_ngx_post);
    RUN_TEST(test_ngx_query);
    RUN_TEST(test_ngx_long16);
    RUN_TEST(test_ngx_long17);
    RUN_TEST(test_ngx_long33);
}

TEST bench_simd_get(void) {
    _Alignas(16) uint8_t b[BUFLEN] = {0};
    const char *src = "GET / HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    struct timespec start, end;
    double elapsed_time;
    clock_gettime(CLOCK_MONOTONIC, &start);
    uint64_t sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        ngx_http_request_t req = {0};
        bench_escape(&req);
        bench_escape(&buf);
        int rc = http_parse_request_line(&req, &buf);
        sum = sum * 13 + rc;
        bench_clobber();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = get_elapsed_time(start, end);
    printf("%s\t%.9fs rc=%lu\n", "bench_simd_get", elapsed_time, sum);
    PASS();
}

TEST bench_ngx_get(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "GET / HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    struct timespec start, end;
    double elapsed_time;
    clock_gettime(CLOCK_MONOTONIC, &start);
    uint64_t sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        ngx_http_request_t req = {0};
        bench_escape(&req);
        bench_escape(&buf);
        int rc = ngx_http_parse_request_line(&req, &buf);
        sum = sum * 13 + rc;
        bench_clobber();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = get_elapsed_time(start, end);
    printf("%s\t%.9fs rc=%lu\n", "bench_ngx_get", elapsed_time, sum);
    PASS();
}

TEST bench_simd_post(void) {
    _Alignas(16) uint8_t b[BUFLEN] = {0};
    const char *src = "POST / HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    struct timespec start, end;
    double elapsed_time;
    clock_gettime(CLOCK_MONOTONIC, &start);
    uint64_t sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        ngx_http_request_t req = {0};
        bench_escape(&req);
        bench_escape(&buf);
        int rc = http_parse_request_line(&req, &buf);
        sum = sum * 13 + rc;
        bench_clobber();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = get_elapsed_time(start, end);
    printf("%s\t%.9fs rc=%lu\n", "bench_simd_post", elapsed_time, sum);
    PASS();
}

TEST bench_ngx_post(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "POST / HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    struct timespec start, end;
    double elapsed_time;
    clock_gettime(CLOCK_MONOTONIC, &start);
    uint64_t sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        ngx_http_request_t req = {0};
        bench_escape(&req);
        bench_escape(&buf);
        int rc = ngx_http_parse_request_line(&req, &buf);
        sum = sum * 13 + rc;
        bench_clobber();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = get_elapsed_time(start, end);
    printf("%s\t%.9fs rc=%lu\n", "bench_ngx_post", elapsed_time, sum);
    PASS();
}

TEST bench_simd_query(void) {
    _Alignas(16) uint8_t b[BUFLEN] = {0};
    const char *src = "QUERY / HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    struct timespec start, end;
    double elapsed_time;
    clock_gettime(CLOCK_MONOTONIC, &start);
    uint64_t sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        ngx_http_request_t req = {0};
        bench_escape(&req);
        bench_escape(&buf);
        int rc = http_parse_request_line(&req, &buf);
        sum = sum * 13 + rc;
        bench_clobber();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = get_elapsed_time(start, end);
    printf("%s\t%.9fs rc=%lu\n", "bench_simd_query", elapsed_time, sum);
    PASS();
}

TEST bench_ngx_query(void) {
    uint8_t b[BUFLEN] = {0};
    const char *src = "QUERY / HTTP/1.1\r\n";
    memcpy(b, src, strlen(src));

    ngx_buf_t buf = {0};
    buf.start = b;
    buf.end = b + BUFLEN;
    buf.pos = buf.start;
    buf.last = buf.pos + strlen(src);

    struct timespec start, end;
    double elapsed_time;
    clock_gettime(CLOCK_MONOTONIC, &start);
    uint64_t sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        ngx_http_request_t req = {0};
        bench_escape(&req);
        bench_escape(&buf);
        int rc = ngx_http_parse_request_line(&req, &buf);
        sum = sum * 13 + rc;
        bench_clobber();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = get_elapsed_time(start, end);
    printf("%s\t%.9fs rc=%lu\n", "bench_ngx_query", elapsed_time, sum);
    PASS();
}

SUITE(bench_simd) {
    RUN_TEST(bench_simd_get);
    RUN_TEST(bench_simd_post);
    RUN_TEST(bench_simd_query);
}

SUITE(bench_ngx) {
    RUN_TEST(bench_ngx_get);
    RUN_TEST(bench_ngx_post);
    RUN_TEST(bench_ngx_query);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(simd);
    RUN_SUITE(ngx);
    RUN_SUITE(bench_simd);
    RUN_SUITE(bench_ngx);

    GREATEST_MAIN_END();
}
