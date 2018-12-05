# cmake file

add_executable(day5a
  src/day5a.cpp)

add_executable(day5b
  src/day5b.cpp)

set_property(TARGET day5a PROPERTY CXX_STANDARD 14)
set_property(TARGET day5b PROPERTY CXX_STANDARD 14)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
