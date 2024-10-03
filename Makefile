.POSIX:
CC     = cc
CFLAGS = -W -O -march=x86-64-v3 -g

C_FILES = index_of_byte.c
H_FILES =
EXECUTABLES = index_of_byte

all: $(EXECUTABLES)

index_of_byte: index_of_byte.o
	$(CC) $^ -o $@

include dependency.mk

dependency.mk: $(C_FILES) $(H_FILES)
	for c_file in $(C_FILES); do $(CC) $(CFLAGS) -MM -MT $$(echo $$c_file | sed 's/\.c$$/.o/') $${c_file}; done > $@

clean:
	@rm -f *.o $(EXECUTABLES)
