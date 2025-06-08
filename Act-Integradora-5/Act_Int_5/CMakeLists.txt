cmake_minimum_required(VERSION 3.31)
project(Act_Int_5)

set(CMAKE_CXX_STANDARD 20)

add_executable(Act_Int_5 main.cpp
        HashTable.h
        ALDWGraph.h
        Heap.h
        LineRead.cpp
        LineRead.h
        fileReader.cpp
        fileReader.h
        IP.cpp
        IP.h
        HashNode.h)
