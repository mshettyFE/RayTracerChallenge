cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

SET(TESTING False CACHE BOOL "Some user-specified option")

project(RayTracer)
file(GLOB SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
include_directories(SYSTEM include)
set(PROG "Tracer")


include(GoogleTest)

if(TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    file(GLOB Tracer_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
    add_executable(
        ${PROG}
        ${Tracer_tests})
    target_link_libraries(
        ${PROG}
    GTest::gtest_main)
    gtest_discover_tests(${PROG})
else()
    add_executable(${PROG} ${SOURCE})
endif()
