#ifndef HTTP_PARSE_H_
#define HTTP_PARSE_H_

#include <stdint.h>

#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6

#define NGX_HTTP_UNKNOWN                   0x00000001
#define NGX_HTTP_GET                       0x00000002
#define NGX_HTTP_HEAD                      0x00000004
#define NGX_HTTP_POST                      0x00000008
#define NGX_HTTP_PUT                       0x00000010
#define NGX_HTTP_DELETE                    0x00000020
#define NGX_HTTP_MKCOL                     0x00000040
#define NGX_HTTP_COPY                      0x00000080
#define NGX_HTTP_MOVE                      0x00000100
#define NGX_HTTP_OPTIONS                   0x00000200
#define NGX_HTTP_PROPFIND                  0x00000400
#define NGX_HTTP_PROPPATCH                 0x00000800
#define NGX_HTTP_LOCK                      0x00001000
#define NGX_HTTP_UNLOCK                    0x00002000
#define NGX_HTTP_PATCH                     0x00004000
#define NGX_HTTP_TRACE                     0x00008000
#define NGX_HTTP_CONNECT                   0x00010000

#define NGX_HTTP_PARSE_HEADER_DONE         1

#define NGX_HTTP_CLIENT_ERROR              10
#define NGX_HTTP_PARSE_INVALID_METHOD      10
#define NGX_HTTP_PARSE_INVALID_REQUEST     11
#define NGX_HTTP_PARSE_INVALID_VERSION     12
#define NGX_HTTP_PARSE_INVALID_09_METHOD   13

#define NGX_HTTP_PARSE_INVALID_HEADER      14

typedef uintptr_t       ngx_uint_t;
typedef uint8_t         u_char;

typedef struct {
    size_t      len;
    u_char     *data;
} ngx_str_t;

typedef struct {
    int                               method;
    ngx_uint_t                        http_version;

    const u_char                     *request_start;
    const u_char                     *method_end;

    ngx_str_t                         http_protocol;

    int state;

    /* URI with "/." and on Win32 with "//" */
    unsigned                          complex_uri:1;

    /* URI with "%" */
    unsigned                          quoted_uri:1;

    /* URI with "+" */
    unsigned                          plus_in_uri:1;

    /* URI with empty path */
    unsigned                          empty_path_in_uri:1;

    u_char                           *uri_start;
    u_char                           *uri_end;
    u_char                           *uri_ext;
    u_char                           *args_start;
    u_char                           *request_start;
    u_char                           *request_end;
    u_char                           *method_end;
    u_char                           *schema_start;
    u_char                           *schema_end;
    u_char                           *host_start;
    u_char                           *host_end;

    unsigned                          http_minor:16;
    unsigned                          http_major:16;
} http_request;

int http_parse_request_line(http_request *r, ngx_buf_t *b);

#endif /* HTTP_PARSE_H_ */
