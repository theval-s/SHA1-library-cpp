cmake_minimum_required(VERSION 3.28.3)
project(crypto_lab6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

add_library(val_sha1 STATIC src/library.cpp)
target_include_directories(val_sha1 PUBLIC include)


if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
add_executable(lab_test src/main.cpp)
target_link_libraries(lab_test val_sha1)
endif()