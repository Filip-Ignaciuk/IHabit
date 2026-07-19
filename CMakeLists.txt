
cmake_minimum_required(VERSION 3.20)

project(HabitTracker)

find_package(raylib 3.0 REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



add_executable(${PROJECT_NAME}  src/website/main.cpp src/core/HabitTracker.cpp src/core/HabitTrackerSerializer.cpp src/core/HabitTrackerManager.cpp)

target_include_directories(${PROJECT_NAME}  PRIVATE include)

target_link_libraries(${PROJECT_NAME} raylib)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()