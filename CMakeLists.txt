cmake_minimum_required(VERSION 3.18.0)
set(CMAKE_BUILD_TYPE Debug)

if(APPLE OR UNIX)
  set(CMAKE_C_COMPILER "gcc")
  set(CMAKE_CXX_COMPILER "g++")
endif()

set(CMAKE_CXX_STANDARD_REQUIRED 20)

project(chip8 LANGUAGES CXX)

add_executable(chip8 src/main.cpp)
add_library(
  emulator
  src/emulator/processor.cpp src/emulator/memory.cpp src/emulator/display.cpp
  src/emulator/keyboard.cpp src/emulator/runner.cpp)

set(TARGETS chip8 emulator)

set_target_properties(
  ${TARGETS}
  PROPERTIES CXX_STANDARD 20
             ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
             LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
             RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if(WIN32 AND MSVC)
  set_target_properties(chip8 PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)
  set(SDL2_DIR SDL2/cmake)
  set(SDL2_ttf_DIR SDL2_ttf/cmake)
  file(COPY SDL2/lib/x64/SDL2.dll SDL2_ttf/lib/x64/SDL2_ttf.dll fonts/arial.ttf
       DESTINATION ${CMAKE_SOURCE_DIR}/bin/Debug)
  target_include_directories(chip8 PRIVATE SDL2/include SDL2_ttf/include)
elseif(APPLE)
  file(COPY fonts/arial.ttf DESTINATION ${CMAKE_SOURCE_DIR}/bin)
  target_include_directories(
    chip8 PRIVATE /Library/Frameworks/SDL2.framework/Headers
                  /Library/Frameworks/SDL2_ttf.framework/Headers)
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
target_include_directories(chip8 PRIVATE src/emulator)
target_link_libraries(chip8 PRIVATE emulator SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(emulator PRIVATE SDL2::SDL2 SDL2::SDL2main
                                       SDL2_ttf::SDL2_ttf)