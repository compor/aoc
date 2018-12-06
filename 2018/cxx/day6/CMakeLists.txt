# cmake file

add_executable(day6a
  src/day6a.cpp)

#add_executable(day6b
  #src/day6b.cpp)

set_property(TARGET day6a PROPERTY CXX_STANDARD 14)
#set_property(TARGET day6b PROPERTY CXX_STANDARD 14)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
