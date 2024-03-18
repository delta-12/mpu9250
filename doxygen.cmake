set(DOXYGEN_NAME doxygen)

find_program(${DOXYGEN_NAME}_BIN
    NAMES ${DOXYGEN_NAME}
)
if(${DOXYGEN_NAME}_BIN)
    message(STATUS "Found ${DOXYGEN_NAME} at: ${${DOXYGEN_NAME}_BIN}")
else()
    message(FATAL_ERROR "${DOXYGEN_NAME} not found.")
endif()

add_custom_target(
        doxygen
        ${${DOXYGEN_NAME}_BIN}
        ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
