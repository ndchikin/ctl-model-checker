add_library(ctl ctl.cpp)
target_link_libraries(ctl PUBLIC taocpp::pegtl)
target_include_directories(ctl PUBLIC ${PROJECT_SOURCE_DIR}/lib)
