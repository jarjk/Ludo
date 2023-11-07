cmake_minimum_required(VERSION 3.2)

project(kineveteavegen LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 20)

add_executable(ludo main.cxx)

install(TARGETS ludo DESTINATION "~/.local/bin")

add_custom_target(uninstall COMMAND "${CMAKE_COMMAND}" -E remove "~/.local/bin/ludo")


if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
  target_compile_options(ludo PRIVATE -Wall -Wextra -Wunreachable-code
                                      -Wpedantic)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(ludo PRIVATE -Wweak-vtables -Wexit-time-destructors
                                      -Wglobal-constructors -Wmissing-noreturn)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options(ludo PRIVATE /W4 /w44265 /w44061 /w44062)
endif()
