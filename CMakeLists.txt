cmake_minimum_required(VERSION 2.8.7)
project(dicomds C)

#add_executable(roundtrip roundtrip.cxx)
add_executable(b b.c)
target_link_libraries(b m)
add_executable(d d.c)
target_link_libraries(d m)
add_executable(compute1 compute1.c)
target_link_libraries(compute1 m)

add_executable(compute3 compute3.c)
target_link_libraries(compute3 m)

enable_testing()

#add_test(NAME rt COMMAND roundtrip)
add_test(NAME compute1 COMMAND compute1)
add_test(NAME compute3 COMMAND compute3)
