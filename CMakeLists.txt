cmake_minimum_required(VERSION 3.6)

project(UXP1A_linda)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(LIB_NAME UXP1A_linda)
set(APP_NAME LindaApp)
set(TESTS_NAME LindaAppTests)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include(CTest)
enable_testing()

file(GLOB LIB_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/library_src/*.cpp)
file(GLOB APP_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB TESTS_SOURCE_FILES ${PROJECT_SOURCE_DIR}/tests_src/*.cpp ${PROJECT_SOURCE_DIR}/tests_src/*/*.cpp)


message(STATUS "LIB_FILES " ${LIB_SOURCE_FILES})
message(STATUS "APP_FILES " ${APP_SOURCE_FILES})
message(STATUS "TESTS_FILES " ${TESTS_SOURCE_FILES})

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(${LIB_NAME} STATIC ${LIB_SOURCE_FILES})

add_executable(${APP_NAME} ${APP_SOURCE_FILES})
target_link_libraries(${APP_NAME} ${LIB_NAME})

add_executable(${TESTS_NAME} ${TESTS_SOURCE_FILES})
target_link_libraries(${TESTS_NAME} ${LIB_NAME})
add_test(tests ${TESTS_NAME})
