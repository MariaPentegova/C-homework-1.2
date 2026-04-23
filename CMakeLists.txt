cmake_minimum_required(VERSION 3.10)
project(My_project C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0074 NEW)

add_executable(CSV src/CSV/main.c)
add_executable(CSV_tests src/CSV/tests.c)
add_executable(flight src/flight_check-in/main.c)
add_executable(nations src/nations/main.c)


if(MSVC)
    target_compile_options(CSV PRIVATE /W4 /WX)
else()
    target_compile_options(CSV PRIVATE -Wall -Wextra -Werror)
endif()
