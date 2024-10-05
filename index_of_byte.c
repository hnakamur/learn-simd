#include "bytes.h"
#include <stdint.h>
#include <stdio.h>

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
