cmake_minimum_required(VERSION 3.5)
project(CircularLogging LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Test REQUIRED)

# Main application
set(SOURCES
    main.cpp
    CircularLogger.cpp
)
set(HEADERS
    CircularLogger.h
)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)

# Test executable
set(TEST_SOURCES
    test_CircularLogger.cpp
    CircularLogger.cpp
)
set(TEST_HEADERS
    CircularLogger.h
)
add_executable(test_CircularLogger ${TEST_SOURCES} ${TEST_HEADERS})
target_link_libraries(test_CircularLogger PRIVATE Qt6::Core Qt6::Test)
target_include_directories(test_CircularLogger PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
