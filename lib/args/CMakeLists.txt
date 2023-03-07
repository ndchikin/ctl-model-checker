add_library(args args.cpp)
target_link_libraries(args argparse::argparse config)
target_include_directories(args PUBLIC ${PROJECT_SOURCE_DIR}/lib)
