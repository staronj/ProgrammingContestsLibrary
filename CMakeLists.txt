cmake_minimum_required(VERSION 3.4)
project(ProgrammingContestsLibrary)
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

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
    add_test(${NAME} ${NAME})
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
endforeach(BENCHMARK)
