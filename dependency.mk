index_of_byte.o: index_of_byte.c bytes.h
bytes.o: bytes.c bytes.h
http_parse.o: http_parse.c http_parse.h bytes.h
test_bytes_eq.o: test_bytes_eq.c bytes.h
test_parse_method.o: test_parse_method.c http_parse.h
test_invalid_method.o: test_invalid_method.c
test_parse_request.o: test_parse_request.c http_parse.h \
  greatest/greatest.h
ngx_http_parse.o: ngx_http_parse.c http_parse.h
