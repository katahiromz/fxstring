# CMakeLists.txt --- CMake project settings
##############################################################################

# CMake minimum version
cmake_minimum_required(VERSION 3.6)

# project name and languages
project(fxstring CXX)

##############################################################################

option(FXSTRING_TEST "Create a test program for fxstring" ON)

##############################################################################

# libfxstring.a
add_library(fxstring STATIC fxstring_test.cpp)

if(FXSTRING_TEST)
    # fxstring_test.exe
    add_executable(fxstring_test fxstring_test.cpp)
endif()

##############################################################################
