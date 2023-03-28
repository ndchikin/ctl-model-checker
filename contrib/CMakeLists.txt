set_property(DIRECTORY PROPERTY EXCLUDE_FROM_ALL 1)

add_subdirectory(argparse)

add_subdirectory(PEGTL)

add_subdirectory(sylvan)
target_compile_options(sylvan PRIVATE -Wno-unused-but-set-parameter)

# include(FetchContent)
# FetchContent_Declare(
#   nusmv
#   URL https://nusmv.fbk.eu/distrib/NuSMV-2.6.0.tar.gz
# )
# FetchContent_Populate(nusmv)
# add_subdirectory(${nusmv_SOURCE_DIR}/NuSMV ${nusmv_BINARY_DIR})
