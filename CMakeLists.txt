cmake_minimum_required(VERSION 3.20)
project(DistBug)

set(CMAKE_CXX_STANDARD 20)

find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 17)
include_directories( $(OpenCV_INCLUDE_DIRS) )

add_executable(DistBug main.cpp Bug.cpp Bug.h)
target_link_libraries(DistBug ${OPENCV_LIBS})