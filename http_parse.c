#include "http_parse.h"
#include "bytes.h"
#include <assert.h>
#include <emmintrin.h>
#include <stdio.h>

static inline int
bvec16_index_of_byte(__m128i bv, uint8_t b)
{
    int mask = _mm_movemask_epi8(_mm_cmpeq_epi8(bv, _mm_set1_epi8(b)));
    if (mask == 0) {
        return -1;
    }
    return __builtin_ffs(mask) - 1;
}

static inline int
bvec16_cmp3(__m128i req_vec, uint8_t b0, uint8_t b1, uint8_t b2)
{
    return (_mm_movemask_epi8(_mm_cmpeq_epi8(req_vec, _mm_set_epi8(
        '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
        '\0', '\0', '\0', '\0', '\0', b2, b1, b0))) & 0x7) == 0x7;
}

static inline int
bvec16_cmp4(__m128i req_vec, uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
    return (_mm_movemask_epi8(_mm_cmpeq_epi8(req_vec, _mm_set_epi8(
        '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
        '\0', '\0', '\0', '\0', b3, b2, b1, b0))) & 0xF) == 0xF;
}

static inline int
bvec16_cmp5(__m128i req_vec, uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, 
            uint8_t b4)
{
    return (_mm_movemask_epi8(_mm_cmpeq_epi8(req_vec, _mm_set_epi8(
        '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
        '\0', '\0', '\0', b4, b3, b2, b1, b0))) & 0x1F) == 0x1F;
}

static inline int
bvec16_cmp6(__m128i req_vec, uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, 
            uint8_t b4, uint8_t b5)
{
    return (_mm_movemask_epi8(_mm_cmpeq_epi8(req_vec, _mm_set_epi8(
        '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
        '\0', '\0', b5, b4, b3, b2, b1, b0))) & 0x3F) == 0x3F;
}

static inline int
bvec16_cmp7(__m128i req_vec, uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, 
            uint8_t b4, uint8_t b5, uint8_t b6)
{
    return (_mm_movemask_epi8(_mm_cmpeq_epi8(req_vec, _mm_set_epi8(
        '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
        '\0', b6, b5, b4, b3, b2, b1, b0))) & 0x7F) == 0x7F;
}

static inline int
bvec16_cmp8(__m128i req_vec, uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, 
            uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7)
{
    return (_mm_movemask_epi8(_mm_cmpeq_epi8(req_vec, _mm_set_epi8(
        '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
        b7, b6, b5, b4, b3, b2, b1, b0))) & 0xFF) == 0xFF;
}

static inline int
bvec16_cmp9(__m128i req_vec, uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, 
            uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7, uint8_t b8)
{
    return (_mm_movemask_epi8(_mm_cmpeq_epi8(req_vec, _mm_set_epi8(
        '\0', '\0', '\0', '\0', '\0', '\0', '\0', b8,
        b7, b6, b5, b4, b3, b2, b1, b0))) & 0x1FF) == 0x1FF;
}

static inline int
bvec16_is_valid_method(__m128i bv, int n)
{
    int m, mask1, mask2, mask3, mask4;

    assert(n > 0 && n <= 16);
    m = (1 << n) - 1;
    mask1 = _mm_movemask_epi8(_mm_cmplt_epi8(bv, _mm_set1_epi8('A')));
    mask2 = _mm_movemask_epi8(_mm_cmpgt_epi8(bv, _mm_set1_epi8('Z')));
    mask3 = _mm_movemask_epi8(_mm_cmpeq_epi8(bv, _mm_set1_epi8('_')));
    mask4 = _mm_movemask_epi8(_mm_cmpeq_epi8(bv, _mm_set1_epi8('-')));
    return (((mask1 | mask2) & ~mask3 & ~mask4) & m) == 0;
}

int
http_parse_request_line(http_request *r, const uint8_t b[], int blen)
{
    assert(blen >= 16);

#ifdef __SSE2__
    __m128i req_vec = _mm_loadu_si128((__m128i *)b);
    int method_len = bvec16_index_of_byte(req_vec, ' ');
    if (method_len == -1) {
        return NGX_AGAIN;
    }

    r->method_end = b + method_len - 1;

    switch (method_len) {
    case 3:
        if (bvec16_cmp3(req_vec, 'G', 'E', 'T')) {
            r->method = NGX_HTTP_GET;
            break;
        }
        if (bvec16_cmp3(req_vec, 'P', 'U', 'T')) {
            r->method = NGX_HTTP_PUT;
            break;
        }
        break;

    case 4:
        if (b[1] == 'O') {
            if (bvec16_cmp4(req_vec, 'P', 'O', 'S', 'T')) {
                r->method = NGX_HTTP_POST;
                break;
            }
            if (bvec16_cmp4(req_vec, 'C', 'O', 'P', 'Y')) {
                r->method = NGX_HTTP_COPY;
                break;
            }
            if (bvec16_cmp4(req_vec, 'M', 'O', 'V', 'E')) {
                r->method = NGX_HTTP_MOVE;
                break;
            }
            if (bvec16_cmp4(req_vec, 'L', 'O', 'C', 'K')) {
                r->method = NGX_HTTP_LOCK;
                break;
            }
            
        } else {

            if (bvec16_cmp4(req_vec, 'H', 'E', 'A', 'D')) {
                r->method = NGX_HTTP_HEAD;
                break;
            }
        }
        break;

    case 5:
        if (bvec16_cmp5(req_vec, 'P', 'A', 'T', 'C', 'H')) {
            r->method = NGX_HTTP_PATCH;
            break;
        }
        if (bvec16_cmp5(req_vec, 'T', 'R', 'A', 'C', 'E')) {
            r->method = NGX_HTTP_TRACE;
            break;
        }
        if (bvec16_cmp5(req_vec, 'M', 'K', 'C', 'O', 'L')) {
            r->method = NGX_HTTP_MKCOL;
            break;
        }
        break;

    case 6:
        if (bvec16_cmp6(req_vec, 'D', 'E', 'L', 'E', 'T', 'E')) {
            r->method = NGX_HTTP_DELETE;
            break;
        }
        if (bvec16_cmp6(req_vec, 'U', 'N', 'L', 'O', 'C', 'K')) {
            r->method = NGX_HTTP_UNLOCK;
            break;
        }
        break;

    case 7:
        if (bvec16_cmp7(req_vec, 'O', 'P', 'T', 'I', 'O', 'N', 'S')) {
            r->method = NGX_HTTP_OPTIONS;
            break;
        }
        if (bvec16_cmp7(req_vec, 'C', 'O', 'N', 'N', 'E', 'C', 'T')) {
            r->method = NGX_HTTP_UNLOCK;
            break;
        }
        break;

    case 8:
        if (bvec16_cmp8(req_vec, 'P', 'R', 'O', 'P', 'F', 'I', 'N', 'D')) {
            r->method = NGX_HTTP_PROPFIND;
            break;
        }
        break;

    case 9:
        if (bvec16_cmp9(req_vec, 'P', 'R', 'O', 'P', 'P', 'A', 'T', 'C', 'H')) {
            r->method = NGX_HTTP_PROPPATCH;
            break;
        }
        break;
    }

    if (!bvec16_is_valid_method(req_vec, method_len)) {
        return NGX_HTTP_PARSE_INVALID_METHOD;
    }
#endif

    return NGX_AGAIN;
}
