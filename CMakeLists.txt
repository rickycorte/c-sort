cmake_minimum_required (VERSION 3.7)
project(sort C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Default build type is set to DEBUG" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

include_directories(sortlibs tests)

add_executable(sort 
    src/tests/test.c 
    src/sortlibs/sort.c 
    src/main.c)

add_custom_command(
        TARGET sort POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/data/test_data.txt
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_data.txt)

# aggiungi il target run per linux
if (UNIX)
    
    add_custom_target(run COMMAND cd bin && ./sort DEPENDS sort)

endif (UNIX)