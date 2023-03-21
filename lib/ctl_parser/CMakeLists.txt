add_library(ctl_parser parser.cpp)
target_link_libraries(ctl_parser PUBLIC taocpp::pegtl)
target_include_directories(ctl_parser PUBLIC ${PROJECT_SOURCE_DIR}/lib)
