cmake_minimum_required(VERSION 3.16)
project(lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(salesman main.cpp)
target_include_directories(salesman PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}        
)

add_custom_command(TARGET salesman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/data
            $<TARGET_FILE_DIR:salesman>/data
)

include(CTest)
enable_testing()

add_executable(salesman_tests
    tests/test_salesman.cpp
)

target_include_directories(salesman_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}        
    ${CMAKE_CURRENT_SOURCE_DIR}/catch2 
)

add_test(NAME salesman_tests COMMAND salesman_tests)

add_custom_command(TARGET salesman_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/data
            $<TARGET_FILE_DIR:salesman_tests>/data
)

add_custom_command(TARGET salesman_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/Example.bat
            $<TARGET_FILE_DIR:salesman_tests>/Example.bat
)
