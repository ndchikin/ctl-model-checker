add_library(cli cli.cpp)
target_link_libraries(cli PRIVATE argparse::argparse config)
target_include_directories(cli PUBLIC ${PROJECT_SOURCE_DIR}/lib)
