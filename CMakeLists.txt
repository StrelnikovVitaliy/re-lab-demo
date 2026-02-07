cmake_minimum_required(VERSION 3.15)
project(re_demo C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(re_demo
  src/main.c
  src/util.c
)

if (WIN32)
  # Підключаємо ресурси PE для ПР25 (Version Info / String Table)
  target_sources(re_demo PRIVATE win/resources.rc)
endif()