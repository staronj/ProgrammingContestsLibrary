cmake_minimum_required(VERSION 3.2)
INCLUDE (CheckTypeSize)
project(ProgrammingContestsLibrary)
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

CHECK_TYPE_SIZE("__int128" INT128_TYPE BUILTIN_TYPES_ONLY)
CHECK_TYPE_SIZE("unsigned __int128" UINT128_TYPE BUILTIN_TYPES_ONLY)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            )
endif(DOXYGEN_FOUND)


link_directories(/usr/local/bin)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE HEADERS_LIST
        "*.h"
)

file(GLOB TEST_LIST
        "tests/*_test.cc"
)

foreach(TEST ${TEST_LIST})
    get_filename_component(NAME ${TEST} NAME_WE)
    add_executable(${NAME} ${TEST} ${HEADERS_LIST})
    target_link_libraries(${NAME}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY tests/)
    add_test(${NAME} tests/${NAME})
endforeach(TEST)

file(GLOB BENCHMARK_LIST
        "benchmarks/*_benchmark.cc"
)

foreach(BENCHMARK ${BENCHMARK_LIST})
    get_filename_component(NAME ${BENCHMARK} NAME_WE)
    add_executable(${NAME} ${BENCHMARK} ${HEADERS_LIST})
    target_link_libraries(${NAME}
            celero
    )
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY benchmarks/)
endforeach(BENCHMARK)

function(flatten input output)
    add_custom_command(OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/${output}
            COMMAND
            python include.py --input ${input} --output ${CMAKE_CURRENT_BINARY_DIR}/${output} --remove-comments
            DEPENDS ${input} ${HEADERS_LIST} include.py
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            )
endfunction(flatten)

file(GLOB EXAMPLES_LIST
        "examples/*.cc"
)

foreach(EXAMPLE ${EXAMPLES_LIST})
    get_filename_component(NAME ${EXAMPLE} NAME_WE)
    add_executable(${NAME} ${EXAMPLE} ${HEADERS_LIST})
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY examples/)
endforeach(EXAMPLE)

flatten(main.cc main_flat.cc)

add_executable(main_flat ${CMAKE_CURRENT_BINARY_DIR}/main_flat.cc ${HEADERS_LIST})
add_executable(main main.cc ${HEADERS_LIST})

