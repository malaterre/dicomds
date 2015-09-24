cmake_minimum_required(VERSION 2.8.7)
project(dicomds C)

#add_executable(roundtrip roundtrip.cxx)
add_executable(valid valid.c)
add_executable(b b.c)
target_link_libraries(b m)
add_executable(d d.c)
target_link_libraries(d m)
add_executable(compute1 compute1.c)
target_link_libraries(compute1 m)

add_executable(compute2 compute2.c)
target_link_libraries(compute2 m)

add_executable(compute3 compute3.c)
target_link_libraries(compute3 m)

add_executable(compute4 compute4.c)
target_link_libraries(compute4 m)

enable_testing()

#add_test(NAME rt COMMAND roundtrip)
add_test(NAME compute1 COMMAND compute1)
#add_test(NAME compute2 COMMAND compute2)
add_test(NAME compute3 COMMAND compute3)
add_test(NAME compute4 COMMAND compute4)
