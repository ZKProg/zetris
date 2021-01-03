cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

project(zetris VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CSS_FLAGS "-Wall -fopenmp")

include_directories(
  ${CMAKE_SOURCE_DIR}/.
  ${CMAKE_SOURCE_DIR}/include)

file(GLOB
  zetris_SRC
  "*.hpp"
  "*.cpp")

add_executable(
  zetris
  ${zetris_SRC})

target_precompile_headers(
  zetris
  PUBLIC
  pch.h)

target_link_libraries(
  zetris
  SDL2
  SDL2main
  SDL2_image
  SDL2_ttf
  pthread
  sqlite3)
