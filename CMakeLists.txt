cmake_minimum_required(VERSION 3.10)

project(KostebekOyunu CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# SDL2 Yolları
set(SDL2_INCLUDE_DIR "C:/dev/SDL2/include")
set(SDL2_LIB_DIR "C:/dev/SDL2/lib")

include_directories(${SDL2_INCLUDE_DIR} "include")

link_directories(${SDL2_LIB_DIR})

set(SOURCES 
    src/main.cpp 
    src/game.cpp
)

add_executable(main ${SOURCES})

target_link_libraries(main 
    mingw32 
    SDL2main 
    SDL2 
    SDL2_image 
    SDL2_mixer 
    SDL2_ttf
)

file(GLOB DLL_FILES "dlls/*.dll")
file(COPY ${DLL_FILES} DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")