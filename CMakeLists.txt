cmake_minimum_required(VERSION 3.10)
project(common)

# Src and Include folders
file(GLOB_RECURSE SRC_FILES "common/*.cpp" "common/*.h")
include_directories("include")

# Build Library
add_library(common ${SRC_FILES})
target_compile_features(common PUBLIC cxx_std_11)
target_link_libraries(common)

target_include_directories(common
    PUBLIC 
        $<INSTALL_INTERFACE:include>   
    PRIVATE
        ${CMAKE_SOURCE_DIR}/common
)

# Build Thread
option (BUILD_THREAD "Build the thread" ON)
if (BUILD_THREAD)
    add_subdirectory(thread)
endif()
