# specify minimum required version of CMake
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# Specify project and identify languages used
project(cpp_project LANGUAGES CXX)

option(ENABLE_UBSAN false)
option (ENABLE_ASAN false)

set (CMAKE_CXX_STANDARD 20)

find_package(CGAL)
find_package(OpenGL)
find_package(GLUT)

if(ENABLE_UBSAN) 
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
	set(CMAKE_EXE:LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
elseif(ENABLE_ASAN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
	set(CMAKE_EXE:LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

add_executable(chess app/Chess.cpp lib/Piece.cpp lib/DrawUtils.cpp)
target_include_directories(chess PUBLIC include ${GLUT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
target_link_libraries(chess ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

install (TARGETS chess DESTINATION bin)

install (PROGRAMS demo DESTINATION bin)





