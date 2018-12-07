# cmake file

add_executable(day7a
  src/day7a.cpp)

#add_executable(day7b
  #src/day7b.cpp)

set_property(TARGET day7a PROPERTY CXX_STANDARD 14)
#set_property(TARGET day7b PROPERTY CXX_STANDARD 14)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
