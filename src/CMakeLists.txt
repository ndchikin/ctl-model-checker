add_executable(ctl-model-checker main.cpp)
target_link_libraries(ctl-model-checker PRIVATE cli checker)
target_link_options(ctl-model-checker PRIVATE -static-libgcc -static-libstdc++)

install(
    TARGETS ctl-model-checker
    DESTINATION ${PROJECT_SOURCE_DIR}/programs
)
