cmake_minimum_required(VERSION 3.27)
project(WindowsCode01 C)

set(CMAKE_C_STANDARD 11)

add_definitions(-DUNICODE -D_UNICODE)

if(MSVC)
    add_compile_options(/utf-8)
else()
    add_compile_options(-finput-charset=utf-8)
    add_compile_options(-fexec-charset=utf-8)
endif()

add_executable(code01 WIN32 code01/main.c)
add_executable(code02 WIN32 code02/main.c)
add_executable(code03 WIN32 code03/main.c)
add_executable(code04 WIN32 code04/main.c)
add_executable(code05 WIN32 code05/main.c)
add_executable(code06_2 WIN32 code06_2/main.c)
add_executable(code06_3 WIN32 code06_3/main.c)
add_executable(code06_4 WIN32 code06_4/main.c)
add_executable(code07 WIN32 code07_01/main.c)
add_executable(code08_01 WIN32 code08_01/main.c)
add_executable(code08_02 WIN32 code08_02/main.c)
add_subdirectory(code10)
add_subdirectory(code11)
add_subdirectory(code12)
add_subdirectory(code13)
add_subdirectory(code14)
add_subdirectory(code15)




