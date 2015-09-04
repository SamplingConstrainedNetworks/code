cmake_minimum_required(VERSION 2.8)
project(triangles)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# include project's source
include_directories(${PROJECT_SOURCE_DIR}/source)

##### Sample

add_executable(generic examples/generic.cpp)

add_executable(round_trip_wl examples/round_trip_wl.cpp)

add_executable(entropy examples/entropy.cpp)

#### Tests

#option(build_tests "Build the project's unit tests" OFF)
#if(build_tests)
enable_testing()
set(GTEST_ROOT ${PROJECT_SOURCE_DIR}/dependencies)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
message(STATUS ${GTEST_LIBRARIES})

add_executable(tests test/main.cpp)
target_link_libraries(tests ${GTEST_LIBRARIES})
add_test(tests tests)
#endif()
