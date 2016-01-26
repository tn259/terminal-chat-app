find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(runTests ClientTests.cpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
