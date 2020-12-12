# cmake file

set(DAY_TARGETS day12)

foreach(DT ${DAY_TARGETS})
  add_executable(${DT} src/${DT}.cpp)
  set_target_properties(${DT} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED
                                                         ON)
endforeach()

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
