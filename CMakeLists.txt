cmake_minimum_required(VERSION 2.8.12)

project(myProject)


file(GLOB SOURCES
     "*.h"
     "*.cpp"
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)


add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image)
