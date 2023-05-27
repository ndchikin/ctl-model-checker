set(GCOV_TOOL "${PROJECT_SOURCE_DIR}/scripts/llvm-gcov.sh")

function(setup_target_for_coverage_lcov)
    set(oneValueArgs NAME)
    set(multiValueArgs INCLUDE EXECUTABLE)
    cmake_parse_arguments(Coverage "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(LCOV_INCLUDES "")
    foreach(INCLUDE ${Coverage_INCLUDE})
        get_filename_component(INCLUDE ${INCLUDE} ABSOLUTE BASE_DIR ${PROJECT_SOURCE_DIR})
        list(APPEND LCOV_INCLUDES "${INCLUDE}")
    endforeach()

    add_custom_target(${Coverage_NAME}
        COMMAND lcov --gcov-tool ${GCOV_TOOL} -d . -b ${PROJECT_SOURCE_DIR} -z
        COMMAND lcov --gcov-tool ${GCOV_TOOL} -d . -b ${PROJECT_SOURCE_DIR} -c -i -o ${Coverage_NAME}.base
        COMMAND ${Coverage_EXECUTABLE}
        COMMAND lcov --gcov-tool ${GCOV_TOOL} -d . -b ${PROJECT_SOURCE_DIR} -c -o ${Coverage_NAME}.capture
        COMMAND lcov --gcov-tool ${GCOV_TOOL} -a ${Coverage_NAME}.base -a ${Coverage_NAME}.capture -o ${Coverage_NAME}.total
        COMMAND lcov --gcov-tool ${GCOV_TOOL} -e ${Coverage_NAME}.total ${LCOV_INCLUDES} -o ${Coverage_NAME}.info
        COMMAND genhtml --demangle-cpp -p ${PROJECT_SOURCE_DIR} -o ${PROJECT_SOURCE_DIR}/${Coverage_NAME} ${Coverage_NAME}.info
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        VERBATIM
    )
endfunction()
