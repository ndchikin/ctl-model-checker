configure_file(config.h.in config.h)
add_library(config INTERFACE)
target_include_directories(config INTERFACE ${PROJECT_BINARY_DIR}/lib)
