add_library(checker checker.cpp)
target_link_libraries(checker PUBLIC ast fsm)
target_include_directories(checker PUBLIC ${PROJECT_SOURCE_DIR}/lib)
