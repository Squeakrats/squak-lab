cmake_minimum_required(VERSION 3.8)
project(squak-lab)

set(CMAKE_CXX_STANDARD 23)

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

add_subdirectory(packages/compiler)
add_subdirectory(packages/json)
add_subdirectory(packages/lawless)
add_subdirectory(packages/RegularExpression)
add_subdirectory(packages/sgrammar)
