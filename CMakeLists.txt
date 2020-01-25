cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set( PROJECT_NAME Zombieshooter)
project(${PROJECT_NAME})
set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/SFML-2.5.1/lib/cmake/SFML")

file(GLOB REQUIRED_DLLS "libs/required_dll/*.dll")
file(GLOB REQUIRED_SFML_DLL "libs/SFML-2.5.1/bin/*.dll")
link_libraries(stdc++fs)
file(COPY ${REQUIRED_DLLS} ${REQUIRED_SFML_DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


file(GLOB_RECURSE SCRS_FILES "src/*.cpp" "src/*.hpp" "src/*.h" "src/*.c")

add_executable(${PROJECT_NAME} ${SCRS_FILES})
find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)

target_link_libraries(${PROJECT_NAME} sfml-audio sfml-graphics sfml-window sfml-system)