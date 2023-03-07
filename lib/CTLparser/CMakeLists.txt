add_library(CTLparser parser.cpp)
target_link_libraries(CTLparser taocpp::pegtl)
target_include_directories(CTLparser PUBLIC ${PROJECT_SOURCE_DIR}/lib)
