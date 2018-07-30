cmake_minimum_required(VERSION 2.8)
project(triangles)

add_subdirectory(source)

##### Sample

add_executable(generic examples/generic.cpp)
target_link_libraries (generic LINK_PUBLIC sample_networks)

add_executable(round_trip_wl examples/round_trip_wl.cpp)
target_link_libraries (round_trip_wl LINK_PUBLIC sample_networks)

add_executable(entropy examples/entropy.cpp)
target_link_libraries (entropy LINK_PUBLIC sample_networks)
