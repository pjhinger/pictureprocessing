cmake_minimum_required(VERSION 3.12)
project(cpp_pic_proc_pj2017)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(images)

add_executable(cpp_pic_proc_pj2017
        images/ducks1.jpg
        images/ducks2.jpg
        images/ducks3.jpg
        images/keepcalm.png
        images/test.jpg
        Colour.cpp
        Colour.hpp
        example_input.txt
        LockablePicNodeList.cpp
        LockablePicNodeList.hpp
        Main.cpp
        Makefile
        PicLibrary.cpp
        PicLibrary.hpp
        PicNode.cpp
        PicNode.hpp
        Picture.cpp
        Picture.hpp
        Utils.cpp
        Utils.hpp)
