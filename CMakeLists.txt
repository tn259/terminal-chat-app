cmake_minimum_required (VERSION 2.6)
project (terminal-chat-app)
ADD_DEFINITIONS(-std=c++11)
find_package(Boost 1.54.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_subdirectory(src)

