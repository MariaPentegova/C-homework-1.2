cmake_minimum_required(VERSION 3.10)
project(My_project C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0074 NEW)

add_executable(CSV_tests src/CSV/tests.c)
add_executable(flight_1 src/flight_check-in/main_1.c)
add_executable(flight_2 src/flight_check-in/main_2.c)
add_executable(nations src/nations/main.c)


if(MSVC)
    target_compile_options(CSV_tests PRIVATE /W4 /WX)
    target_compile_options(flight_1 PRIVATE /W4 /WX)
    target_compile_options(flight_2 PRIVATE /W4 /WX)
    target_compile_options(nations PRIVATE /W4 /WX)
else()
    target_compile_options(CSV_tests PRIVATE -Wall -Wextra -Werror)
    target_compile_options(flight_1 PRIVATE -Wall -Wextra -Werror)
    target_compile_options(flight_2 PRIVATE -Wall -Wextra -Werror)
    target_compile_options(nations PRIVATE -Wall -Wextra -Werror)
endif()
