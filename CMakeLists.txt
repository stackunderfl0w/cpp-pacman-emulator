cmake_minimum_required(VERSION 3.15)
project(z80)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -O3 -march=native -lmingw32")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ ")


include_directories(${PROJECT_SOURCE_DIR}/include )
link_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories( zip )

#here
link_directories(zip)

add_subdirectory(zip)
add_executable(z80 main.cpp i8080.cpp i8080.h z80.cpp z80.h Screen.cpp Screen.h z80-master.h z80-test.h)

target_link_libraries(z80 SDL2main SDL2 SDL2_ttf zip)


