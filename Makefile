.POSIX:
CC     = cc
CFLAGS = -W -O3 -march=x86-64-v3 -g

C_FILES = index_of_byte.c bytes.c test_bytes_eq.c
H_FILES = bytes.h
EXECUTABLES = index_of_byte test_bytes_eq

all: $(EXECUTABLES)

index_of_byte: index_of_byte.o bytes.o
	$(CC) $^ -o $@

test_bytes_eq: test_bytes_eq.o bytes.o
	$(CC) $^ -o $@

include dependency.mk

dependency.mk: $(C_FILES) $(H_FILES)
	for c_file in $(C_FILES); do $(CC) $(CFLAGS) -MM -MT $$(echo $$c_file | sed 's/\.c$$/.o/') $${c_file}; done > $@

clean:
	@rm -f *.o $(EXECUTABLES)
