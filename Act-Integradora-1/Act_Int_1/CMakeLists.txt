cmake_minimum_required(VERSION 3.30)
project(Tests)

set(CMAKE_CXX_STANDARD 20)

add_executable(Tests main.cpp
        src/LineRead.cpp
        hdr/LineRead.h
        src/fileReader.cpp
        hdr/fileReader.h
        hdr/DateToNum.h
        src/DateToNum.cpp
        hdr/Registro.h
        src/Registro.cpp
        hdr/Sort.h
        hdr/Search.h
)
