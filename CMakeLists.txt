cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME RescueCam)
project(${PROJECT_NAME})
set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/SFML-2.5.1/lib/cmake/SFML")

link_libraries(stdc++fs)

file(GLOB_RECURSE SCRS_FILES "src/*.cpp" "src/*.hpp" "src/*.h" "src/*.c")
file(COPY "data"  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SCRS_FILES} )
find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
find_library(wiringPi_LIB wiringPi)
find_library(bcm2835_lib bcm2835 )
target_link_libraries(${PROJECT_NAME} sfml-audio sfml-graphics sfml-window sfml-system ${wiringPi_LIB} ${bcm2835_lib})