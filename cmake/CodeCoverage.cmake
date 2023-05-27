find_program(LCOV_PATH NAMES lcov)
find_program(GENHTML_PATH NAMES genhtml)

set(GCOV_TOOL "${PROJECT_SOURCE_DIR}/scripts/llvm-gcov.sh")

function(setup_target_for_coverage_lcov)
    set(oneValueArgs NAME)
    set(multiValueArgs EXCLUDE EXECUTABLE)
    cmake_parse_arguments(Coverage "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(LCOV_EXCLUDES "")
    foreach(EXCLUDE ${Coverage_EXCLUDE})
        get_filename_component(EXCLUDE ${EXCLUDE} ABSOLUTE BASE_DIR ${PROJECT_SOURCE_DIR})
        list(APPEND LCOV_EXCLUDES "${EXCLUDE}")
    endforeach()

    add_custom_target(${Coverage_NAME}
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_TOOL} -d . -b ${PROJECT_SOURCE_DIR} -z
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_TOOL} -d . -b ${PROJECT_SOURCE_DIR} -c -i -o ${Coverage_NAME}.base
        COMMAND ${Coverage_EXECUTABLE}
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_TOOL} -d . -b ${PROJECT_SOURCE_DIR} -c -o ${Coverage_NAME}.capture
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_TOOL} -a ${Coverage_NAME}.base -a ${Coverage_NAME}.capture -o ${Coverage_NAME}.total
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_TOOL} -r ${Coverage_NAME}.total ${LCOV_EXCLUDES} -o ${Coverage_NAME}.info
        COMMAND ${GENHTML_PATH} --demangle-cpp -p ${PROJECT_SOURCE_DIR} -o ${PROJECT_SOURCE_DIR}/${Coverage_NAME} ${Coverage_NAME}.info

        BYPRODUCTS
            ${Coverage_NAME}.base
            ${Coverage_NAME}.capture
            ${Coverage_NAME}.total
            ${Coverage_NAME}.info
            ${Coverage_NAME}/index.html
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        VERBATIM
    )
endfunction()
