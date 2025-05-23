cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

project(StreamFlix)

include_directories(.)

add_executable(StreamFlix
        Movie.cpp
        Movie.h
        StreamFlix.cpp
        StreamFlix.h
        main.cpp
        MovieDatabase.cpp
        MovieDatabase.h
        TMDBServiceProvider.cpp
        TMDBServiceProvider.h)

target_link_libraries(StreamFlix ws2_32)

target_include_directories(StreamFlix PUBLIC
        ./HttpRequest/include/
        ./json/single_include/
)
