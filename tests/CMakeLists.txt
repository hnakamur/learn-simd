set(TEST_FILENAMES sample1_unittest.cc)
list(TRANSFORM TEST_FILENAMES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/
                                      OUTPUT_VARIABLE TEST_FILES)

add_executable(sample1_unittest ${TEST_FILES})
target_link_libraries(sample1_unittest PRIVATE sample1 gtest_main)
include(GoogleTest)
gtest_discover_tests(sample1_unittest)

add_executable(bench_is_invalid_method bench_is_invalid_method.cc)
target_compile_options(bench_is_invalid_method PRIVATE -g -march=x86-64-v3)
target_link_libraries(bench_is_invalid_method PRIVATE benchmark::benchmark)
add_custom_target(
  bench
  COMMAND $<TARGET_FILE:bench_is_invalid_method>
  VERBATIM)
