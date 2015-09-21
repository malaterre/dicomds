cmake_minimum_required(VERSION 2.8.7)
project(dicomds)

add_executable(roundtrip roundtrip.cxx)
add_executable(b b.c)
target_link_libraries(b m)
add_executable(d d.c)
target_link_libraries(d m)

enable_testing()

add_test(NAME rt COMMAND roundtrip)
