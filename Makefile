.POSIX:
CC     = clang
CFLAGS = -W -O3 -march=x86-64-v3 -g
LDFLAGS =

C_FILES = index_of_byte.c bytes.c http_parse.c test_bytes_eq.c test_parse_method.c
H_FILES = bytes.h http_parse.h
EXECUTABLES = index_of_byte test_bytes_eq test_parse_method

all: $(EXECUTABLES)

test_parse_method: test_parse_method.o http_parse.o
	$(CC) $^ -o $@ $(LDFLAGS)

test_bytes_eq: test_bytes_eq.o bytes.o
	$(CC) $^ -o $@ $(LDFLAGS)

index_of_byte: index_of_byte.o bytes.o
	$(CC) $^ -o $@ $(LDFLAGS)

include dependency.mk

dependency.mk: $(C_FILES) $(H_FILES)
	for c_file in $(C_FILES); do $(CC) $(CFLAGS) -MM -MT $$(echo $$c_file | sed 's/\.c$$/.o/') $${c_file}; done > $@

clean:
	@rm -f *.o $(EXECUTABLES)
