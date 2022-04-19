cmake_minimum_required(VERSION 3.8)
project(squak-lab)

set(CMAKE_CXX_STANDARD 23)
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

add_subdirectory(packages/compiler)
add_subdirectory(packages/core)
add_subdirectory(packages/engine)
add_subdirectory(packages/gl)
add_subdirectory(packages/gltf)
add_subdirectory(packages/graphics)
add_subdirectory(packages/json)
add_subdirectory(packages/lawless)
add_subdirectory(packages/math)
add_subdirectory(packages/RegularExpression)
add_subdirectory(packages/grammar)
add_subdirectory(packages/utility)
add_subdirectory(packages/compression)
