cmake_minimum_required(VERSION 3.8)
project(glr)

add_executable(glrt
	src/main.cpp
)
target_link_libraries(glrt xml)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/KHR)

file(DOWNLOAD https://www.khronos.org/registry/EGL/api/KHR/khrplatform.h ${CMAKE_CURRENT_BINARY_DIR}/include/KHR/khrplatform.h)
file(DOWNLOAD https://raw.githubusercontent.com/KhronosGroup/OpenGL-Registry/main/xml/gl.xml ${CMAKE_CURRENT_BINARY_DIR}/gl.xml)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/gl.h
    COMMAND glrt ${CMAKE_CURRENT_BINARY_DIR}/include/gl.h ${CMAKE_CURRENT_BINARY_DIR}/gl.xml
    DEPENDS glrt
)

add_library(glr
	${CMAKE_CURRENT_BINARY_DIR}/include/gl.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/KHR/khrplatform.h
)
target_include_directories(glr PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)
set_target_properties(glr PROPERTIES LINKER_LANGUAGE CXX)
