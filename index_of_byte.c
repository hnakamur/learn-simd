#include <stdint.h>
#include <immintrin.h>
#include <emmintrin.h>
#include <stdio.h>

int index_of_byte(const uint8_t s[], int len, uint8_t b) {
    int pos = 0;
    for (; pos + 32 <= len; pos += 32) {
        __m256i vec = _mm256_loadu_si256((__m256i *)(s + pos));
        __m256i cmp = _mm256_cmpeq_epi8(vec, _mm256_set1_epi8(b));
        int mask = _mm256_movemask_epi8(cmp);
        if (mask != 0) {
            return pos + __builtin_ffs(mask) - 1;
        }
    }
    for (; pos + 16 <= len; pos += 16) {
        __m128i vec = _mm_loadu_si128((__m128i *)(s + pos));
        __m128i cmp = _mm_cmpeq_epi8(vec, _mm_set1_epi8(b));
        int mask = _mm_movemask_epi8(cmp);
        if (mask != 0) {
            return pos + __builtin_ffs(mask) - 1;   
        }
    }
    for (; pos < len; pos++) {
        if (s[pos] == b) {
            return pos;
        }
    }
    return -1;
}

int main() {
    const uint8_t data [] =
        "GET / HTTP/1.1\r\n"
        "Host: example.co"
        "m\r\nUser-Agent: c"
        "url/8.5.0\r\n\r\n";
    #define LEN sizeof(data)

    printf("%d\n", index_of_byte(data, LEN, ' '));
    printf("%d\n", index_of_byte(data, LEN, 'H'));
    printf("%d\n", index_of_byte(data, LEN, '\n'));
    printf("%d\n", index_of_byte(data, LEN, 'c'));
    printf("%d\n", index_of_byte(data, LEN, 'A'));
    printf("%d\n", index_of_byte(data, LEN, '\0'));

    return 0;   
}
