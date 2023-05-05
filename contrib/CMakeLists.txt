set_property(DIRECTORY PROPERTY EXCLUDE_FROM_ALL 1)

add_subdirectory(argparse)
add_subdirectory(PEGTL)
add_subdirectory(sylvan)
add_subdirectory(googletest)

target_compile_options(sylvan PRIVATE -Wno-unused-but-set-parameter)
target_compile_definitions(sylvan PRIVATE SYLVAN_STATS)
