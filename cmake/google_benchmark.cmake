include(FetchContent)

FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark
  GIT_TAG 12235e24652fc7f809373e7c11a5f73c5763fc4c # v1.9.0
)

set(BENCHMARK_ENABLE_TESTING
    OFF
    CACHE BOOL "Enable testing of the benchmark library.")
set(BENCHMARK_DOWNLOAD_DEPENDENCIES
    ON
    CACHE BOOL
          "Allow the downloading and in-tree building of unmet dependencies")

# This command requires CMake 3.14 or later
FetchContent_MakeAvailable(googlebenchmark)
