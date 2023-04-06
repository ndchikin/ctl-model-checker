add_library(ast ast.cpp)
target_link_libraries(ast PUBLIC taocpp::pegtl)
target_include_directories(ast PUBLIC ${PROJECT_SOURCE_DIR}/lib)
