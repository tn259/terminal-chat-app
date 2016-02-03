add_library(SH SocketHandler.cpp)
add_executable(Client Client.cpp SocketHandler.cpp)
target_link_libraries(Client ${Boost_LIBRARIES})
target_link_libraries(Client ${CMAKE_THREAD_LIBS_INIT})
