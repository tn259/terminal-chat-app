file( GLOB SRCS *.cpp *.hpp )
add_executable(Client ${SRCS})
target_link_libraries(Client ${Boost_LIBRARIES})
