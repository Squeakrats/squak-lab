cmake_minimum_required(VERSION 3.8)
project(squak-lab)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include_directories(${CMAKE_SOURCE_DIR}/lib/include)

set(CMAKE_CXX_STANDARD 23)
if (NOT EMSCRIPTEN)
  set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*;header-filter=.;")
endif()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Follow google test framework setup instructions
enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES FOLDER extern)

add_subdirectory(packages/core)
add_subdirectory(packages/engine)
add_subdirectory(packages/gl)
add_subdirectory(packages/glr)
add_subdirectory(packages/gltf)
add_subdirectory(packages/graphics)
add_subdirectory(packages/json)
add_subdirectory(packages/lawless)
add_subdirectory(packages/math)
add_subdirectory(packages/regex)
add_subdirectory(packages/grammar)
add_subdirectory(packages/utility)
add_subdirectory(packages/compression)
add_subdirectory(packages/png)
add_subdirectory(packages/window)
add_subdirectory(packages/xml)
add_subdirectory(packages/http)
add_subdirectory(packages/net)
add_subdirectory(packages/websocket)
add_subdirectory(packages/headertool)
