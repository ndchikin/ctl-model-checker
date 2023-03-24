# add_library(bdd bdd.cpp)
add_executable(bdd bdd.cpp)
target_link_libraries(bdd PRIVATE sylvan::sylvan)
# target_include_directories(bdd PUBLIC ${PROJECT_SOURCE_DIR}/lib)

install(
    TARGETS bdd
    DESTINATION ${PROJECT_SOURCE_DIR}/programs
)
