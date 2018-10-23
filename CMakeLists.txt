cmake_minimum_required(VERSION 3.12)
project(fifteen-puzzle C)

set(CMAKE_C_STANDARD 11)

set(ROOT Programa/src/solver)
set(TEST_ROOT Programa/tests/solver)

include_directories(${ROOT})

add_library(Hashtable ${ROOT}/hashtable.c)
add_library(Utils ${ROOT}/utils.c)
add_library(State ${ROOT}/state.c)

add_executable(main ${ROOT}/main.c)

# test files
add_executable(test-hashtable ${TEST_ROOT}/test_hashtable.c)
target_link_libraries(test-hashtable Hashtable)
target_link_libraries(test-hashtable Utils)
