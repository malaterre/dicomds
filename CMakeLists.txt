cmake_minimum_required(VERSION 2.8.7)
project(dicomds)

add_executable(roundtrip roundtrip.cxx)
add_executable(b b.c)
target_link_libraries(b m)
add_executable(d d.c)
target_link_libraries(d m)
add_executable(compute compute.c)
target_link_libraries(compute m)

enable_testing()

#add_test(NAME rt COMMAND roundtrip)
add_test(NAME compute COMMAND compute)
