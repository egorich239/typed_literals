cmake_minimum_required(VERSION 3.20)
project(typed_literals)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/2d924d7a971e9667d76ad09727fb2402b4f8a1e3.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(typed_literals INTERFACE)
target_include_directories(typed_literals INTERFACE .)

enable_testing()
add_executable(typed_literals_test typed_literals_test.cc)
target_link_libraries(typed_literals_test gmock_main typed_literals)

include(GoogleTest)
gtest_discover_tests(typed_literals_test)