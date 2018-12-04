# cmake file

add_executable(day4a
  src/day4a.cpp)

#add_executable(day4b
  #src/day4b.cpp)

set_property(TARGET day4a PROPERTY CXX_STANDARD 14)
#set_property(TARGET day4b PROPERTY CXX_STANDARD 14)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
