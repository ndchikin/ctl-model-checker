add_library(fsm fsm.cpp)
target_link_libraries(fsm PUBLIC sylvan::sylvan)
target_include_directories(fsm PUBLIC ${PROJECT_SOURCE_DIR}/lib)
