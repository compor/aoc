# cmake file

add_executable(a
  src/a.cpp)

add_executable(b
  src/b.cpp)

set_property(TARGET a PROPERTY CXX_STANDARD 14)
set_property(TARGET b PROPERTY CXX_STANDARD 14)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
