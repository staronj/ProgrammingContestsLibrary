cmake_minimum_required(VERSION 3.4)
INCLUDE (CheckTypeSize)
project(ProgrammingContestsLibrary)
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

CHECK_TYPE_SIZE("__int128" INT128_TYPE BUILTIN_TYPES_ONLY)
CHECK_TYPE_SIZE("unsigned __int128" UINT128_TYPE BUILTIN_TYPES_ONLY)

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

add_custom_command(OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/main_flat.cc
        COMMAND
        python include.py --input main.cc --output ${CMAKE_CURRENT_BINARY_DIR}/main_flat.cc
        DEPENDS main.cc ${HEADERS_LIST} include.py
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(main ${CMAKE_CURRENT_BINARY_DIR}/main_flat.cc ${HEADERS_LIST})
