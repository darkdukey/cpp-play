file(GLOB SRC_FILES "*.h" "*.cpp" )

add_executable(thread ${SRC_FILES})
target_link_libraries(thread PRIVATE common)
