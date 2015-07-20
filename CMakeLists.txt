cmake_minimum_required(VERSION 2.8)
project(pychaos)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/source)

##### Sample

add_executable(main source/main.cpp)
target_link_libraries(main ${GSL_LIBRARIES})

add_executable(round_trip_wl examples/round_trip_wl.cpp)
target_link_libraries(round_trip_wl ${GSL_LIBRARIES})

#### Tests

message(STATUS ${CMAKE_MODULE_PATH})

option(build_tests "Build the project's unit tests" OFF)
if(build_tests)
    enable_testing()
    set(GTEST_ROOT ${PROJECT_SOURCE_DIR}/dependencies)

    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    add_executable(runUnitTests test/main.cpp)
    target_link_libraries(runUnitTests ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${GSL_LIBRARIES})
    add_test(runUnitTests runUnitTests)
endif()
