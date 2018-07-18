cmake_minimum_required(VERSION 2.8)
project(TomatoClassification)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(OpenCV REQUIRED)

#Config header directory
include_directories(include)
#Find all *.cpp file in src/
file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} include/function.h src/function.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
