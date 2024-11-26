# version 3.22

cmake_minimum_required(VERSION 3.22)

project(hot-reload-raylib VERSION 0.1
                          DESCRIPTION "Hot reload enabled c++17 raylib template"
                          LANGUAGES CXX)

option(HOT_RELOAD "Build binary with hot-reload (do not use for full release)" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# default to debug build
if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
endif()

include(FetchContent)

# do not build raylib extras
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

if(HOT_RELOAD)
    # hot reload requires raylib to be a shared library
    set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
else()
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
endif()

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG        "5.5"
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(raylib)

set(PROJ_NAME "demo")
set(SRC src/main.cpp)

add_library(common INTERFACE)
target_compile_features(common INTERFACE cxx_std_17)
target_link_libraries(common INTERFACE raylib)
target_include_directories(common SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(HOT_RELOAD)
    # to enable hot reload split the application into a shared library and executable
    FetchContent_Declare(
        dylib
        GIT_REPOSITORY "https://github.com/martin-olivier/dylib"
        GIT_TAG        "v2.2.1"
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(dylib)

    add_library(lib${PROJ_NAME} SHARED ${SRC})
    target_link_libraries(lib${PROJ_NAME} PUBLIC common)
    set_target_properties(lib${PROJ_NAME}
        PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON
        PREFIX "")
    target_compile_definitions(lib${PROJ_NAME} PUBLIC HR_LIB)

    add_executable(${PROJ_NAME} src/main.cpp)
    target_link_libraries(${PROJ_NAME}
        PUBLIC common
        PUBLIC dylib)
    target_compile_definitions(${PROJ_NAME} PRIVATE HR_EXE HR_LIB_NAME="${PROJ_NAME}")
else()
    add_executable(${PROJ_NAME} ${SRC})
    target_link_libraries(${PROJ_NAME} PUBLIC common)
endif()

