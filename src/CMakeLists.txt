include(hardening)
add_compile_options(-Wunused-parameter)
# add_compile_options(-flto -Wunused-parameter) add_link_options(-flto)

set(SRC_FILENAMES sample1.cc)
list(TRANSFORM SRC_FILENAMES PREPEND ${PROJECT_SOURCE_DIR}/src/ OUTPUT_VARIABLE
                                                                SRC_FILES)

add_library(sample1 STATIC ${SRC_FILES})
target_include_directories(sample1 PUBLIC .)
install(TARGETS sample1)
