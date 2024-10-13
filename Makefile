CXX = clang++
CLANG_FORMAT = clang-format

build: setup
	cmake --build build --config Release -v

test: build
	cmake --build build --config Release --target test -v

bench: build
	cmake --build build --config Release --target bench -v

install: test
	sudo cmake --build build --config Release --target install -v

debug_build: setup
	cmake --build build --config Debug -v

debug_test: debug_build
	cmake --build build --config Debug --target test -v

format: setup
	cmake --build build --config Release --target format -v

setup:
	if [ ! -d build ]; then \
	CXX=$(CXX) cmake -B build -G "Ninja Multi-Config" -DCLANG_FORMAT=$(CLANG_FORMAT); \
	fi

clean:
	@rm -rf build

.PHONY: build test bench install debug_build debug_test format setup clean
