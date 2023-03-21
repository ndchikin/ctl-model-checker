add_executable(ctl-model-checker main.cpp)
target_link_libraries(ctl-model-checker PRIVATE args ctl_parser)

install(
    TARGETS ctl-model-checker
    DESTINATION ${PROJECT_SOURCE_DIR}/programs
)
