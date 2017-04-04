# cmake Laboratory 9 CSI4130
# set minimum required version for cmake
#
# Note: I don't know enough of CMake to be sure if earlier version
# would work as well
cmake_minimum_required(VERSION 2.8)


# add our homegrown package definitions
set(CMAKE_MODULE_PATH ${CMAKE_MODUL_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
message("Module path: ${CMAKE_MODULE_PATH}")

# define the project name
set(project_name "final_project")
project("${project_name}")

# display the project name when cmake is run
message("Project name: ${project_name}")
message("Project source dir: ${PROJECT_SOURCE_DIR}")
# set c++ flags manually -- not needed here
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -O0")

add_executable(${project_name} main.cpp moebius.cpp sphere.cpp common/shader.cpp)

set(CMAKE_BUILD_TYPE Debug)

# include boiler plate
include ("${CMAKE_MODULE_PATH}/../CMakeListsAll.txt")



# make sure common and glm directories are included
include_directories(${PROJECT_SOURCE_DIR}/common)
include_directories(${PROJECT_SOURCE_DIR}/../../glm)
