cmake_minimum_required(VERSION 3.1)
project(A4)

set(CMAKE_CXX_STANDARD 11)

# Do not modify these flags other than switching between from the line with -ggdb to -O2 and back.
#set(CMAKE_CXX_FLAGS "-O2 -std=c++11 -Werror -pedantic -Wall -Wuninitialized -Winit-self -Wredundant-decls -Wshadow  -Wno-unused -Wno-sign-compare -Wno-unused-variable")
set(CMAKE_CXX_FLAGS "-O2 -std=c++11 -Werror -pedantic -Wall -Wuninitialized -Winit-self -Wredundant-decls -Wshadow  -Wno-unused -Wno-sign-compare -Wno-unused-variable")

# Set your working directory to the folder containing your project or this main will not function correctly.
add_executable(A4 main.cpp a4.cpp)
