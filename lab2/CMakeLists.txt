cmake_minimum_required(VERSION 3.16)
project(lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Основное приложение
add_executable(minimal_team
    main.cpp
)

target_include_directories(minimal_team PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Копируем папку data рядом с бинарником
add_custom_command(TARGET minimal_team POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/data
            $<TARGET_FILE_DIR:minimal_team>/data
)

# Тесты (Catch2)
include(CTest)
enable_testing()

add_executable(team_tests
    tests/test_team.cpp
)

target_include_directories(team_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/catch2
)

add_test(NAME team_tests COMMAND team_tests)

# Копируем data рядом с тестовым бинарником
add_custom_command(TARGET team_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/data
            $<TARGET_FILE_DIR:team_tests>/data
)
