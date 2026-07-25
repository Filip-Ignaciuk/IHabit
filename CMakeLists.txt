
cmake_minimum_required(VERSION 3.20)

project(IHabit)

find_package(raylib 3.0 REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(IErrorManage
        GIT_REPOSITORY https://github.com/Filip-Ignaciuk/IErrorManage.git
        GIT_TAG main  # or specific commit/tag
)

FetchContent_MakeAvailable(IErrorManage)

# IHabit Library
add_library(${PROJECT_NAME} 
    src/core/HabitTracker.cpp 
    src/core/HabitTrackerManager.cpp 
    src/core/HabitTrackerSerializerFileSystem.cpp
    src/core/HabitTrackerSerializerIndexedDB.cpp
    src/core/HabitTrackerSerializerFactory.cpp
        src/core/HabitTrackerSerializer.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
        include/core/
        include/libraries
)

# Website Example
add_executable(IHabitApp
        src/app/Main.cpp
        src/app/GridMaker.cpp
        src/app/App.cpp
)

target_include_directories(IHabitApp PRIVATE
        include/gui
        include/app
        include/libraries
)
target_link_libraries(IHabitApp PRIVATE
        raylib
        IErrorManage
        ${PROJECT_NAME} )

# Compilation for websites.
if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    target_link_options(IHabitApp PRIVATE
            "-lidbfs.js"
    )
endif ()

# Compilation for Apple devices.
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()