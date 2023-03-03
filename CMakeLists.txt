cmake_minimum_required(VERSION 3.24)
project(irc)

set(CMAKE_CXX_STANDARD 14)

add_executable(irc
        main.cpp differentIPClient.cpp server.cpp)
