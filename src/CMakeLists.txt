add_executable(CTL-model-checker main.cpp)
target_link_libraries(CTL-model-checker args)

install(
    TARGETS CTL-model-checker
    DESTINATION ${PROJECT_SOURCE_DIR}/programs
)
