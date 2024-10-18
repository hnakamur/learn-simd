#define NGX_HAVE_LITTLE_ENDIAN 1
#define NGX_HAVE_NONALIGNED 1

#include "http_parse.h"
#include "greatest.h"

SUITE(simd);
SUITE(ngx);

#define BUFLEN 1024

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

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(simd);
    RUN_SUITE(ngx);

    GREATEST_MAIN_END();
}
