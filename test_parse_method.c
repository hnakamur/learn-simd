#include "http_parse.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>

#define NGX_HAVE_LITTLE_ENDIAN 1
#define NGX_HAVE_NONALIGNED 1

#if (NGX_HAVE_LITTLE_ENDIAN && NGX_HAVE_NONALIGNED)

#define ngx_str3_cmp(m, c0, c1, c2, c3)                                       \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

#define ngx_str3Ocmp(m, c0, c1, c2, c3)                                       \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

#define ngx_str4cmp(m, c0, c1, c2, c3)                                        \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

#define ngx_str5cmp(m, c0, c1, c2, c3, c4)                                    \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)             \
        && m[4] == c4

#define ngx_str6cmp(m, c0, c1, c2, c3, c4, c5)                                \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)             \
        && (((uint32_t *) m)[1] & 0xffff) == ((c5 << 8) | c4)

#define ngx_str7_cmp(m, c0, c1, c2, c3, c4, c5, c6, c7)                       \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)             \
        && ((uint32_t *) m)[1] == ((c7 << 24) | (c6 << 16) | (c5 << 8) | c4)

#define ngx_str8cmp(m, c0, c1, c2, c3, c4, c5, c6, c7)                        \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)             \
        && ((uint32_t *) m)[1] == ((c7 << 24) | (c6 << 16) | (c5 << 8) | c4)

#define ngx_str9cmp(m, c0, c1, c2, c3, c4, c5, c6, c7, c8)                    \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)             \
        && ((uint32_t *) m)[1] == ((c7 << 24) | (c6 << 16) | (c5 << 8) | c4)  \
        && m[8] == c8

#else /* !(NGX_HAVE_LITTLE_ENDIAN && NGX_HAVE_NONALIGNED) */

#define ngx_str3_cmp(m, c0, c1, c2, c3)                                       \
    m[0] == c0 && m[1] == c1 && m[2] == c2

#define ngx_str3Ocmp(m, c0, c1, c2, c3)                                       \
    m[0] == c0 && m[2] == c2 && m[3] == c3

#define ngx_str4cmp(m, c0, c1, c2, c3)                                        \
    m[0] == c0 && m[1] == c1 && m[2] == c2 && m[3] == c3

#define ngx_str5cmp(m, c0, c1, c2, c3, c4)                                    \
    m[0] == c0 && m[1] == c1 && m[2] == c2 && m[3] == c3 && m[4] == c4

#define ngx_str6cmp(m, c0, c1, c2, c3, c4, c5)                                \
    m[0] == c0 && m[1] == c1 && m[2] == c2 && m[3] == c3                      \
        && m[4] == c4 && m[5] == c5

#define ngx_str7_cmp(m, c0, c1, c2, c3, c4, c5, c6, c7)                       \
    m[0] == c0 && m[1] == c1 && m[2] == c2 && m[3] == c3                      \
        && m[4] == c4 && m[5] == c5 && m[6] == c6

#define ngx_str8cmp(m, c0, c1, c2, c3, c4, c5, c6, c7)                        \
    m[0] == c0 && m[1] == c1 && m[2] == c2 && m[3] == c3                      \
        && m[4] == c4 && m[5] == c5 && m[6] == c6 && m[7] == c7

#define ngx_str9cmp(m, c0, c1, c2, c3, c4, c5, c6, c7, c8)                    \
    m[0] == c0 && m[1] == c1 && m[2] == c2 && m[3] == c3                      \
        && m[4] == c4 && m[5] == c5 && m[6] == c6 && m[7] == c7 && m[8] == c8

#endif


typedef uint8_t u_char;

#define LF     (u_char) '\n'
#define CR     (u_char) '\r'

/* gcc, icc, msvc and others compile these switches as an jump table */

#if 0

int
ngx_http_parse_request_line(http_request *r, uint8_t *b, int blen)
{
    u_char  c, ch;
    u_char *p, *m;
    enum {
        sw_start = 0,
        sw_newline,
        sw_method_start,
        sw_method,
#if 0
        sw_spaces_before_uri,
        sw_schema,
        sw_schema_slash,
        sw_schema_slash_slash,
        sw_host_start,
        sw_host,
        sw_host_end,
        sw_host_ip_literal,
        sw_port,
        sw_after_slash_in_uri,
        sw_check_uri,
        sw_uri,
        sw_http_09,
        sw_http_H,
        sw_http_HT,
        sw_http_HTT,
        sw_http_HTTP,
        sw_first_major_digit,
        sw_major_digit,
        sw_first_minor_digit,
        sw_minor_digit,
        sw_spaces_after_digit,
        sw_almost_done
#endif
    } state;

    state = r->state;

    for (p = b; p < b + blen; p++) {
        ch = *p;

        switch (state) {

        /* HTTP methods: GET, HEAD, POST */
        case sw_start:
            r->request_start = p;

            if (ch == CR) {
                state = sw_newline;
                break;
            }

            if (ch == LF) {
                state = sw_method_start;
                break;
            }

            if ((ch < 'A' || ch > 'Z') && ch != '_' && ch != '-') {
                return NGX_HTTP_PARSE_INVALID_METHOD;
            }

            state = sw_method;
            break;

        case sw_newline:

            if (ch == LF) {
                state = sw_method_start;
                break;
            }

            return NGX_HTTP_PARSE_INVALID_REQUEST;

        case sw_method_start:
            r->request_start = p;

            if ((ch < 'A' || ch > 'Z') && ch != '_' && ch != '-') {
                return NGX_HTTP_PARSE_INVALID_METHOD;
            }

            state = sw_method;
            break;

        case sw_method:
            if (ch == ' ') {
                r->method_end = p - 1;
                m = r->request_start;

                switch (p - m) {

                case 3:
                    if (ngx_str3_cmp(m, 'G', 'E', 'T', ' ')) {
                        r->method = NGX_HTTP_GET;
                        break;
                    }

                    if (ngx_str3_cmp(m, 'P', 'U', 'T', ' ')) {
                        r->method = NGX_HTTP_PUT;
                        break;
                    }

                    break;

                case 4:
                    if (m[1] == 'O') {

                        if (ngx_str3Ocmp(m, 'P', 'O', 'S', 'T')) {
                            r->method = NGX_HTTP_POST;
                            break;
                        }

                        if (ngx_str3Ocmp(m, 'C', 'O', 'P', 'Y')) {
                            r->method = NGX_HTTP_COPY;
                            break;
                        }

                        if (ngx_str3Ocmp(m, 'M', 'O', 'V', 'E')) {
                            r->method = NGX_HTTP_MOVE;
                            break;
                        }

                        if (ngx_str3Ocmp(m, 'L', 'O', 'C', 'K')) {
                            r->method = NGX_HTTP_LOCK;
                            break;
                        }

                    } else {

                        if (ngx_str4cmp(m, 'H', 'E', 'A', 'D')) {
                            r->method = NGX_HTTP_HEAD;
                            break;
                        }
                    }

                    break;

                case 5:
                    if (ngx_str5cmp(m, 'M', 'K', 'C', 'O', 'L')) {
                        r->method = NGX_HTTP_MKCOL;
                        break;
                    }

                    if (ngx_str5cmp(m, 'P', 'A', 'T', 'C', 'H')) {
                        r->method = NGX_HTTP_PATCH;
                        break;
                    }

                    if (ngx_str5cmp(m, 'T', 'R', 'A', 'C', 'E')) {
                        r->method = NGX_HTTP_TRACE;
                        break;
                    }

                    break;

                case 6:
                    if (ngx_str6cmp(m, 'D', 'E', 'L', 'E', 'T', 'E')) {
                        r->method = NGX_HTTP_DELETE;
                        break;
                    }

                    if (ngx_str6cmp(m, 'U', 'N', 'L', 'O', 'C', 'K')) {
                        r->method = NGX_HTTP_UNLOCK;
                        break;
                    }

                    break;

                case 7:
                    if (ngx_str7_cmp(m, 'O', 'P', 'T', 'I', 'O', 'N', 'S', ' '))
                    {
                        r->method = NGX_HTTP_OPTIONS;
                    }

                    if (ngx_str7_cmp(m, 'C', 'O', 'N', 'N', 'E', 'C', 'T', ' '))
                    {
                        r->method = NGX_HTTP_CONNECT;
                    }

                    break;

                case 8:
                    if (ngx_str8cmp(m, 'P', 'R', 'O', 'P', 'F', 'I', 'N', 'D'))
                    {
                        r->method = NGX_HTTP_PROPFIND;
                    }

                    break;

                case 9:
                    if (ngx_str9cmp(m,
                            'P', 'R', 'O', 'P', 'P', 'A', 'T', 'C', 'H'))
                    {
                        r->method = NGX_HTTP_PROPPATCH;
                    }

                    break;
                }

#if 1
                return NGX_AGAIN;
#else
                state = sw_spaces_before_uri;
                break;
#endif
            }

            if ((ch < 'A' || ch > 'Z') && ch != '_' && ch != '-') {
                return NGX_HTTP_PARSE_INVALID_METHOD;
            }

            break;

#if 0
        /* space* before URI */
        case sw_spaces_before_uri:

            if (ch == '/') {
                r->uri_start = p;
                state = sw_after_slash_in_uri;
                break;
            }

            c = (u_char) (ch | 0x20);
            if (c >= 'a' && c <= 'z') {
                r->schema_start = p;
                state = sw_schema;
                break;
            }

            switch (ch) {
            case ' ':
                break;
            default:
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }
            break;

        case sw_schema:

            c = (u_char) (ch | 0x20);
            if (c >= 'a' && c <= 'z') {
                break;
            }

            if ((ch >= '0' && ch <= '9') || ch == '+' || ch == '-' || ch == '.')
            {
                break;
            }

            switch (ch) {
            case ':':
                r->schema_end = p;
                state = sw_schema_slash;
                break;
            default:
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }
            break;

        case sw_schema_slash:
            switch (ch) {
            case '/':
                state = sw_schema_slash_slash;
                break;
            default:
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }
            break;

        case sw_schema_slash_slash:
            switch (ch) {
            case '/':
                state = sw_host_start;
                break;
            default:
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }
            break;

        case sw_host_start:

            r->host_start = p;

            if (ch == '[') {
                state = sw_host_ip_literal;
                break;
            }

            state = sw_host;

            /* fall through */

        case sw_host:

            c = (u_char) (ch | 0x20);
            if (c >= 'a' && c <= 'z') {
                break;
            }

            if ((ch >= '0' && ch <= '9') || ch == '.' || ch == '-') {
                break;
            }

            /* fall through */

        case sw_host_end:

            r->host_end = p;

            switch (ch) {
            case ':':
                state = sw_port;
                break;
            case '/':
                r->uri_start = p;
                state = sw_after_slash_in_uri;
                break;
            case '?':
                r->uri_start = p;
                r->args_start = p + 1;
                r->empty_path_in_uri = 1;
                state = sw_uri;
                break;
            case ' ':
                /*
                 * use single "/" from request line to preserve pointers,
                 * if request line will be copied to large client buffer
                 */
                r->uri_start = r->schema_end + 1;
                r->uri_end = r->schema_end + 2;
                state = sw_http_09;
                break;
            default:
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }
            break;

        case sw_host_ip_literal:

            if (ch >= '0' && ch <= '9') {
                break;
            }

            c = (u_char) (ch | 0x20);
            if (c >= 'a' && c <= 'z') {
                break;
            }

            switch (ch) {
            case ':':
                break;
            case ']':
                state = sw_host_end;
                break;
            case '-':
            case '.':
            case '_':
            case '~':
                /* unreserved */
                break;
            case '!':
            case '$':
            case '&':
            case '\'':
            case '(':
            case ')':
            case '*':
            case '+':
            case ',':
            case ';':
            case '=':
                /* sub-delims */
                break;
            default:
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }
            break;

        case sw_port:
            if (ch >= '0' && ch <= '9') {
                break;
            }

            switch (ch) {
            case '/':
                r->uri_start = p;
                state = sw_after_slash_in_uri;
                break;
            case '?':
                r->uri_start = p;
                r->args_start = p + 1;
                r->empty_path_in_uri = 1;
                state = sw_uri;
                break;
            case ' ':
                /*
                 * use single "/" from request line to preserve pointers,
                 * if request line will be copied to large client buffer
                 */
                r->uri_start = r->schema_end + 1;
                r->uri_end = r->schema_end + 2;
                state = sw_http_09;
                break;
            default:
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }
            break;

        /* check "/.", "//", "%", and "\" (Win32) in URI */
        case sw_after_slash_in_uri:

            if (usual[ch >> 5] & (1U << (ch & 0x1f))) {
                state = sw_check_uri;
                break;
            }

            switch (ch) {
            case ' ':
                r->uri_end = p;
                state = sw_http_09;
                break;
            case CR:
                r->uri_end = p;
                r->http_minor = 9;
                state = sw_almost_done;
                break;
            case LF:
                r->uri_end = p;
                r->http_minor = 9;
                goto done;
            case '.':
                r->complex_uri = 1;
                state = sw_uri;
                break;
            case '%':
                r->quoted_uri = 1;
                state = sw_uri;
                break;
            case '/':
                r->complex_uri = 1;
                state = sw_uri;
                break;
#if (NGX_WIN32)
            case '\\':
                r->complex_uri = 1;
                state = sw_uri;
                break;
#endif
            case '?':
                r->args_start = p + 1;
                state = sw_uri;
                break;
            case '#':
                r->complex_uri = 1;
                state = sw_uri;
                break;
            case '+':
                r->plus_in_uri = 1;
                break;
            default:
                if (ch < 0x20 || ch == 0x7f) {
                    return NGX_HTTP_PARSE_INVALID_REQUEST;
                }
                state = sw_check_uri;
                break;
            }
            break;

        /* check "/", "%" and "\" (Win32) in URI */
        case sw_check_uri:

            if (usual[ch >> 5] & (1U << (ch & 0x1f))) {
                break;
            }

            switch (ch) {
            case '/':
#if (NGX_WIN32)
                if (r->uri_ext == p) {
                    r->complex_uri = 1;
                    state = sw_uri;
                    break;
                }
#endif
                r->uri_ext = NULL;
                state = sw_after_slash_in_uri;
                break;
            case '.':
                r->uri_ext = p + 1;
                break;
            case ' ':
                r->uri_end = p;
                state = sw_http_09;
                break;
            case CR:
                r->uri_end = p;
                r->http_minor = 9;
                state = sw_almost_done;
                break;
            case LF:
                r->uri_end = p;
                r->http_minor = 9;
                goto done;
#if (NGX_WIN32)
            case '\\':
                r->complex_uri = 1;
                state = sw_after_slash_in_uri;
                break;
#endif
            case '%':
                r->quoted_uri = 1;
                state = sw_uri;
                break;
            case '?':
                r->args_start = p + 1;
                state = sw_uri;
                break;
            case '#':
                r->complex_uri = 1;
                state = sw_uri;
                break;
            case '+':
                r->plus_in_uri = 1;
                break;
            default:
                if (ch < 0x20 || ch == 0x7f) {
                    return NGX_HTTP_PARSE_INVALID_REQUEST;
                }
                break;
            }
            break;

        /* URI */
        case sw_uri:

            if (usual[ch >> 5] & (1U << (ch & 0x1f))) {
                break;
            }

            switch (ch) {
            case ' ':
                r->uri_end = p;
                state = sw_http_09;
                break;
            case CR:
                r->uri_end = p;
                r->http_minor = 9;
                state = sw_almost_done;
                break;
            case LF:
                r->uri_end = p;
                r->http_minor = 9;
                goto done;
            case '#':
                r->complex_uri = 1;
                break;
            default:
                if (ch < 0x20 || ch == 0x7f) {
                    return NGX_HTTP_PARSE_INVALID_REQUEST;
                }
                break;
            }
            break;

        /* space+ after URI */
        case sw_http_09:
            switch (ch) {
            case ' ':
                break;
            case CR:
                r->http_minor = 9;
                state = sw_almost_done;
                break;
            case LF:
                r->http_minor = 9;
                goto done;
            case 'H':
                r->http_protocol.data = p;
                state = sw_http_H;
                break;
            default:
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }
            break;

        case sw_http_H:
            switch (ch) {
            case 'T':
                state = sw_http_HT;
                break;
            default:
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }
            break;

        case sw_http_HT:
            switch (ch) {
            case 'T':
                state = sw_http_HTT;
                break;
            default:
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }
            break;

        case sw_http_HTT:
            switch (ch) {
            case 'P':
                state = sw_http_HTTP;
                break;
            default:
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }
            break;

        case sw_http_HTTP:
            switch (ch) {
            case '/':
                state = sw_first_major_digit;
                break;
            default:
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }
            break;

        /* first digit of major HTTP version */
        case sw_first_major_digit:
            if (ch < '1' || ch > '9') {
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }

            r->http_major = ch - '0';

            if (r->http_major > 1) {
                return NGX_HTTP_PARSE_INVALID_VERSION;
            }

            state = sw_major_digit;
            break;

        /* major HTTP version or dot */
        case sw_major_digit:
            if (ch == '.') {
                state = sw_first_minor_digit;
                break;
            }

            if (ch < '0' || ch > '9') {
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }

            r->http_major = r->http_major * 10 + (ch - '0');

            if (r->http_major > 1) {
                return NGX_HTTP_PARSE_INVALID_VERSION;
            }

            break;

        /* first digit of minor HTTP version */
        case sw_first_minor_digit:
            if (ch < '0' || ch > '9') {
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }

            r->http_minor = ch - '0';
            state = sw_minor_digit;
            break;

        /* minor HTTP version or end of request line */
        case sw_minor_digit:
            if (ch == CR) {
                state = sw_almost_done;
                break;
            }

            if (ch == LF) {
                goto done;
            }

            if (ch == ' ') {
                state = sw_spaces_after_digit;
                break;
            }

            if (ch < '0' || ch > '9') {
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }

            if (r->http_minor > 99) {
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }

            r->http_minor = r->http_minor * 10 + (ch - '0');
            break;

        case sw_spaces_after_digit:
            switch (ch) {
            case ' ':
                break;
            case CR:
                state = sw_almost_done;
                break;
            case LF:
                goto done;
            default:
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }
            break;

        /* end of request line */
        case sw_almost_done:
            r->request_end = p - 1;
            switch (ch) {
            case LF:
                goto done;
            default:
                return NGX_HTTP_PARSE_INVALID_REQUEST;
            }
#endif
        }
    }

#if 0
    b->pos = p;
    r->state = state;
#endif

    return NGX_AGAIN;

done:
#if 0
    b->pos = p + 1;

    if (r->request_end == NULL) {
        r->request_end = p;
    }

    r->http_version = r->http_major * 1000 + r->http_minor;
    r->state = sw_start;

    if (r->http_version == 9 && r->method != NGX_HTTP_GET) {
        return NGX_HTTP_PARSE_INVALID_09_METHOD;
    }
#endif
    return NGX_OK;
}

int
ngx_http_parse_method(http_request *r, uint8_t *b, int blen)
{
    r->method = NGX_HTTP_UNKNOWN;
    r->state = 0;
    return ngx_http_parse_request_line(r, b, blen);
}

#define BENCH_REPEAT_COUNT 10000000

uint64_t bench_ngx(uint8_t b[], int blen) {
    uint64_t sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        http_request r;
        r.method = NGX_HTTP_UNKNOWN;
        r.state = 0;
        int rc = ngx_http_parse_request_line(&r, b, blen);
        sum += (i + 1) * r.method;
        // sum += (i + 1) * (r.method * 100 + rc);
    }
    return sum;
}

uint64_t bench_simd(uint8_t b[], int blen) {
    uint64_t sum = 0;
    for (int i = 0; i < BENCH_REPEAT_COUNT; i++) {
        http_request r;
        r.method = NGX_HTTP_UNKNOWN;
        r.state = 0;
        int rc = http_parse_request_line(&r, b, blen);
        sum += (i + 1) * r.method;
        // sum += (i + 1) * (r.method * 100 + rc);
    }
    return sum;
}

double get_elapsed_time(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

void bench(const char *name, uint64_t (*f)(const uint8_t b[], int blen), const uint8_t b[], int blen) {
    struct timespec start, end;
    double elapsed_time;

    clock_gettime(CLOCK_MONOTONIC, &start);
    uint64_t rc = f(b, blen);
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = get_elapsed_time(start, end);
    printf("%s\t%.9fs rc=%lu\n", name, elapsed_time, rc);
}

#include <emmintrin.h>
#endif

int main() {
#if 0
    http_request r;
    #define B1LEN 16
    const uint8_t b1[B1LEN] = {
        'G', 'E', 'T', ' ', '/', ' ', 'H', 'T', 'T', 'P', '/', '1', '.', '1', '\r', '\n'};
    #define B2LEN 22
    const uint8_t b2[B2LEN] = {
        'P', 'R', 'O', 'P', 'P', 'A', 'T', 'C', 'H', ' ', '/', ' ', 'H', 'T', 'T', 'P',
        '/', '1', '.', '1', '\r', '\n'};
    #define B3LEN 17
    const uint8_t b3[B3LEN] = {
        'P', 'O', 'S', 'T', ' ', '/', ' ', 'H', 'T', 'T', 'P', '/', '1', '.', '1', '\r',
        '\n'};
    #define B4LEN 17
    const uint8_t b4[B4LEN] = {
        'H', 'E', 'A', 'D', ' ', '/', ' ', 'H', 'T', 'T', 'P', '/', '1', '.', '1', '\r',
        '\n'};
    #define B5LEN 18
    const uint8_t b5[B5LEN] = {
        'Q', 'U', 'E', 'R', 'Y', ' ', '/', ' ', 'H', 'T', 'T', 'P', '/', '1', '.', '1',
        '\r', '\n'};
    #define B6LEN 16
    const uint8_t b6[B6LEN] = {
        'g', 'e', 't', ' ', '/', ' ', 'H', 'T', 'T', 'P', '/', '1', '.', '1', '\r', '\n'};
    #define B7LEN 16
    const uint8_t b7[B7LEN] = {
        'A', 'Z', 'A', ' ', '/', ' ', 'H', 'T', 'T', 'P', '/', '1', '.', '1', '\r', '\n'};

    {
        int rc = ngx_http_parse_method(&r, b1, B1LEN);
        assert(rc == NGX_AGAIN);
        assert(r.method == NGX_HTTP_GET);
        assert(r.method_end == b1 + sizeof("GET") - 2);

        r.method = NGX_HTTP_UNKNOWN;
        r.state = 0;
        rc = http_parse_request_line(&r, b1, B1LEN);
        assert(rc == NGX_AGAIN);
        assert(r.method == NGX_HTTP_GET);
        assert(r.method_end == b1 + sizeof("GET") - 2);
    }

    {
        int rc = ngx_http_parse_method(&r, b2, B2LEN);
        assert(rc == NGX_AGAIN);
        assert(r.method == NGX_HTTP_PROPPATCH);
        assert(r.method_end == b2 + sizeof("PROPPATCH") - 2);

        r.method = NGX_HTTP_UNKNOWN;
        r.state = 0;
        rc = http_parse_request_line(&r, b2, B2LEN);
        assert(rc == NGX_AGAIN);
        assert(r.method == NGX_HTTP_PROPPATCH);
        assert(r.method_end == b2 + sizeof("PROPPATCH") - 2);
    }

    {
        int rc = ngx_http_parse_method(&r, b3, B3LEN);
        assert(rc == NGX_AGAIN);
        assert(r.method == NGX_HTTP_POST);
        assert(r.method_end == b3 + sizeof("POST") - 2);

        r.method = NGX_HTTP_UNKNOWN;
        r.state = 0;
        rc = http_parse_request_line(&r, b3, B3LEN);
        assert(rc == NGX_AGAIN);
        assert(r.method == NGX_HTTP_POST);
        assert(r.method_end == b3 + sizeof("POST") - 2);
    }

    {
        int rc = ngx_http_parse_method(&r, b4, B4LEN);
        assert(rc == NGX_AGAIN);
        assert(r.method == NGX_HTTP_HEAD);
        assert(r.method_end == b4 + sizeof("HEAD") - 2);

        r.method = NGX_HTTP_UNKNOWN;
        r.state = 0;
        rc = http_parse_request_line(&r, b4, B4LEN);
        assert(rc == NGX_AGAIN);
        assert(r.method == NGX_HTTP_HEAD);
        assert(r.method_end == b4 + sizeof("HEAD") - 2);
    }

    {
        int rc = ngx_http_parse_method(&r, b5, B5LEN);
        assert(rc == NGX_AGAIN);
        assert(r.method == NGX_HTTP_UNKNOWN);
        assert(r.method_end == b5 + sizeof("QUERY") - 2);

        r.method = NGX_HTTP_UNKNOWN;
        r.state = 0;
        rc = http_parse_request_line(&r, b5, B5LEN);
        assert(rc == NGX_AGAIN);
        assert(r.method == NGX_HTTP_UNKNOWN);
        assert(r.method_end == b5 + sizeof("QUERY") - 2);
    }

    {
        int rc = ngx_http_parse_method(&r, b6, B6LEN);
        assert(rc == NGX_HTTP_PARSE_INVALID_METHOD);
        assert(r.method == NGX_HTTP_UNKNOWN);

        r.method = NGX_HTTP_UNKNOWN;
        r.state = 0;
        rc = http_parse_request_line(&r, b6, B6LEN);
        assert(rc == NGX_HTTP_PARSE_INVALID_METHOD);
        assert(r.method == NGX_HTTP_UNKNOWN);
    }

    {
        int rc = ngx_http_parse_method(&r, b7, B7LEN);
        assert(rc == NGX_AGAIN);
        assert(r.method == NGX_HTTP_UNKNOWN);
        assert(r.method_end == b7 + sizeof("AZA") - 2);

        r.method = NGX_HTTP_UNKNOWN;
        r.state = 0;
        rc = http_parse_request_line(&r, b7, B7LEN);
        assert(rc == NGX_AGAIN);
        assert(r.method == NGX_HTTP_UNKNOWN);
        assert(r.method_end == b7 + sizeof("AZA") - 2);
    }

    bench("ngx_get\t", bench_ngx, b1, B1LEN);
    bench("simd_get", bench_simd, b1, B1LEN);
    bench("ngx_proppatch", bench_ngx, b2, B2LEN);
    bench("simd_proppatch", bench_simd, b2, B2LEN);
    bench("ngx_post", bench_ngx, b3, B3LEN);
    bench("simd_post", bench_simd, b3, B3LEN);
    bench("ngx_head", bench_ngx, b4, B4LEN);
    bench("simd_head", bench_simd, b4, B4LEN);
    bench("ngx_query", bench_ngx, b5, B5LEN);
    bench("simd_qury", bench_simd, b5, B5LEN);
#endif
    return 0;
}
