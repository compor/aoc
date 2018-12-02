# cmake file

add_executable(day1a
  src/day1a.cpp)

add_executable(day1b
  src/day1b.cpp)

set_property(TARGET day1a PROPERTY CXX_STANDARD 14)
set_property(TARGET day1b PROPERTY CXX_STANDARD 14)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
