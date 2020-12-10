# cmake file

set(DAY_TARGETS day01a)

foreach(DT ${DAY_TARGETS})
  add_executable(${DT} src/${DT}.cpp)
  set_target_properties(${DT} PROPERTIES CMAKE_CXX_STANDARD 17)
endforeach()

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
