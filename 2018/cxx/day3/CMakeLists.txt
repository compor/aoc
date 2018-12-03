# cmake file

add_executable(day3a
  src/day3a.cpp)

#add_executable(day3b
  #src/day3b.cpp)

set_property(TARGET day2a PROPERTY CXX_STANDARD 14)
#set_property(TARGET day2b PROPERTY CXX_STANDARD 14)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
