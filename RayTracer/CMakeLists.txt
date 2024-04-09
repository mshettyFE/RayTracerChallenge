cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

SET(TESTING TRUE CACHE BOOL "Some user-specified option")
SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall")
SET(GCC_COVERAGE_LINK_FLAGS    "")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

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
    list(REMOVE_ITEM SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    add_executable(
        ${PROG}
        ${Tracer_tests} ${SOURCE})
    target_link_libraries(
        ${PROG}
    GTest::gtest_main)
    gtest_discover_tests(${PROG})
else()
    add_executable(${PROG} ${SOURCE})
endif()
