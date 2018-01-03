cmake_minimum_required(VERSION 3.6)

project(UXP1A_linda)
include(CTest)
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DBOOST_LOG_DYN_LINK")
set(LIB_NAME LindaLib)
set(APP_NAME LindaApp)
set(TESTS_NAME LindaTests)
set(INTEGRATION_TESTS_NAME LindaIntegrationTests)

SET(BOOST_LIBRARYDIR "~/boost_1_65_1")

find_package(Boost COMPONENTS unit_test_framework log system filesystem REQUIRED)

file(GLOB APP_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE LIB_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/library_src/*.cpp)
file(GLOB_RECURSE TESTS_SOURCE_FILES ${PROJECT_SOURCE_DIR}/tests_src/unit_tests/*.cpp)
file(GLOB_RECURSE INTEGRATION_TESTS_SOURCE_FILES ${PROJECT_SOURCE_DIR}/tests_src/integration_tests/*.cpp)

message(STATUS "APP_FILES " ${APP_SOURCE_FILES})
message(STATUS "LIB_FILES " ${LIB_SOURCE_FILES})
message(STATUS "TESTS_FILES " ${TESTS_SOURCE_FILES})
message(STATUS "INTEGRATION_TESTS_FILES " ${INTEGRATION_TESTS_SOURCE_FILES})

include_directories(${PROJECT_SOURCE_DIR}/include)
set(LIBS ${Boost_LIBRARIES} pthread rt)

add_library(${LIB_NAME} STATIC ${LIB_SOURCE_FILES})
target_link_libraries(${LIB_NAME} ${LIBS})

add_executable(${APP_NAME} ${APP_SOURCE_FILES})
target_link_libraries(${APP_NAME} ${LIB_NAME} ${LIBS})

add_executable(${TESTS_NAME} ${TESTS_SOURCE_FILES})
target_link_libraries(${TESTS_NAME} ${LIB_NAME} ${LIBS})
add_test(tests ${TESTS_NAME})

add_executable(${INTEGRATION_TESTS_NAME} ${INTEGRATION_TESTS_SOURCE_FILES})
target_link_libraries(${INTEGRATION_TESTS_NAME} ${LIB_NAME} ${LIBS})
add_test(integration_tests ${INTEGRATION_TESTS_NAME})