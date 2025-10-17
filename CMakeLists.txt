cmake_minimum_required(VERSION 3.20)
project(cpp_systems_lab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional demo app
add_executable(app main.cpp)
target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR})

# ---- GoogleTest via FetchContent ----
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
# For MSVC: avoid overriding parent compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ---- Tests ----
enable_testing()

add_executable(lru_tests
  tests/testLRU.cpp
)
target_include_directories(lru_tests PRIVATE
  ${CMAKE_SOURCE_DIR}         # so "lru/LRUCache.hpp" resolves
)
target_link_libraries(lru_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(lru_tests)
