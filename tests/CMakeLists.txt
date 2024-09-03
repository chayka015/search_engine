cmake_minimum_required(VERSION 3.25)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(SrcFiles
        ../include/converterjson.h
        ../src/converterjson.cpp
        ../include/invertedIndex.h
        ../src/invertedIndex.cpp
        ../include/searchServer.h
        ../src/searchServer.cpp

)

set(TestsFiles
        search_engine_test.cc
)

add_executable(GtestTests ${TestsFiles} ${SrcFiles})
target_link_libraries(GtestTests nlohmann_json::nlohmann_json GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(GtestTests)