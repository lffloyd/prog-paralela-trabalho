set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.c)

set(SOURCES ${SOURCES})

find_package(MPI REQUIRED)
include_directories(${MPI_C_INCLUDE_PATH})

find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

add_executable(${BINARY}_run ${SOURCES})

target_link_libraries(${BINARY}_run ${MPI_LIBRARIES})

add_library(${BINARY}_lib STATIC ${SOURCES})
