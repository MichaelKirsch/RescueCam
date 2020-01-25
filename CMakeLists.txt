cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME RescueCam)
project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp") #glob all the source files intp one place
file(COPY ${REQUIRED_DLLS_FOR_SFML} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(SFML COMPONENTS system window graphics network audio REQUIRED)

include_directories("libs")

add_executable(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME} sfml-audio sfml-graphics sfml-window sfml-system )