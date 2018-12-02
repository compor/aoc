# cmake file

add_executable(day2a
  src/day2a.cpp)

add_executable(day2b
  src/day2b.cpp)

set_property(TARGET day2a PROPERTY CXX_STANDARD 14)
set_property(TARGET day2b PROPERTY CXX_STANDARD 14)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
