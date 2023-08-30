cmake_minimum_required(VERSION 3.10)

# Definisci il nome del progetto
project(Tesi)


set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

# Imposta la versione del linguaggio C++ utilizzata
set(CMAKE_CXX_STANDARD 11)

# Trova le librerie SDL2 e SDL2image
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Aggiungi i file di inclusione delle librerie
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

file(GLOB SOURCES *.h *.cpp)

# Aggiungi gli eseguibili al progetto
add_executable(Tesi ${SOURCES})

# Collega le librerie utilizzate dall'eseguibile
target_link_libraries(Tesi ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})