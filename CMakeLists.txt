cmake_minimum_required(VERSION 3.5)

project(UXP1A_linda)
include(CTest)
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DBOOST_LOG_DYN_LINK")
set(LIB_NAME LindaLib)
set(APP_NAME LindaApp)
set(TESTS_NAME LindaTests)
set(INTEGRATION_TESTS_NAME LindaIntegrationTests)
set(LINDA_OUTPUT_NAME LindaOutput)
set(LINDA_READ_NAME LindaRead)
set(LINDA_INPUT_NAME LindaInput)

SET(BOOST_LIBRARYDIR "~/boost_1_65_1")

find_package(Boost COMPONENTS unit_test_framework log system filesystem program_options REQUIRED)

file(GLOB APP_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE LIB_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/library_src/*.cpp)
file(GLOB_RECURSE TESTS_SOURCE_FILES ${PROJECT_SOURCE_DIR}/tests_src/unit_tests/*.cpp)
file(GLOB_RECURSE INTEGRATION_TESTS_SOURCE_FILES ${PROJECT_SOURCE_DIR}/tests_src/integration_tests/*.cpp)
file(GLOB_RECURSE LINDA_OUTPUT_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/linda_output/*.cpp)
file(GLOB_RECURSE LINDA_READ_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/linda_read/*.cpp)
file(GLOB_RECURSE LINDA_INPUT_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/linda_input/*.cpp)

message(STATUS "APP_FILES " ${APP_SOURCE_FILES})
message(STATUS "LIB_FILES " ${LIB_SOURCE_FILES})
message(STATUS "TESTS_FILES " ${TESTS_SOURCE_FILES})
message(STATUS "INTEGRATION_TESTS_FILES " ${INTEGRATION_TESTS_SOURCE_FILES})
message(STATUS "LINDA_OUTPUT_FILES " ${LINDA_OUTPUT_SOURCE_FILES})
message(STATUS "LINDA_READ_FILES " ${LINDA_READ_SOURCE_FILES})
message(STATUS "LINDA_INPUT_FILES " ${LINDA_INPUT_SOURCE_FILES})

include_directories(${PROJECT_SOURCE_DIR}/include)
set(LIBS ${Boost_LIBRARIES} pthread rt)

add_library(${LIB_NAME} STATIC ${LIB_SOURCE_FILES} include/linda_api/ExclusiveFileAccessor.h)
target_link_libraries(${LIB_NAME} ${LIBS})

add_executable(${APP_NAME} ${APP_SOURCE_FILES})
target_link_libraries(${APP_NAME} ${LIB_NAME} ${LIBS})

add_executable(${LINDA_OUTPUT_NAME} ${LINDA_OUTPUT_SOURCE_FILES})
target_link_libraries(${LINDA_OUTPUT_NAME} ${LIB_NAME} ${LIBS})

add_executable(${LINDA_READ_NAME} ${LINDA_READ_SOURCE_FILES})
target_link_libraries(${LINDA_READ_NAME} ${LIB_NAME} ${LIBS})

add_executable(${LINDA_INPUT_NAME} ${LINDA_INPUT_SOURCE_FILES})
target_link_libraries(${LINDA_INPUT_NAME} ${LIB_NAME} ${LIBS})

add_executable(${TESTS_NAME} ${TESTS_SOURCE_FILES})
target_link_libraries(${TESTS_NAME} ${LIB_NAME} ${LIBS})
add_test(tests ${TESTS_NAME})

add_executable(${INTEGRATION_TESTS_NAME} ${INTEGRATION_TESTS_SOURCE_FILES})
target_link_libraries(${INTEGRATION_TESTS_NAME} ${LIB_NAME} ${LIBS})
add_test(integration_tests ${INTEGRATION_TESTS_NAME})
