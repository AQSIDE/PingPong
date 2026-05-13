cmake_minimum_required(VERSION 3.20)
project(PingPong)

set(CMAKE_CXX_STANDARD 20)

# SHARED
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/libs/raylib/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/engine/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/engine/default)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/libs/raylib/lib)
# CLIENT
file(GLOB_RECURSE CLIENT_SOURCES client/src/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/client/src/game)
# SERVER
# CORE
file(GLOB_RECURSE CORE_SOURCES core/engine/*.cpp)

add_executable(PingPong ${CLIENT_SOURCES} ${CORE_SOURCES})
target_link_libraries(PingPong PRIVATE wininet raylib opengl32 gdi32 winmm)

add_custom_command(TARGET PingPong POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/client/assets"
        "$<TARGET_FILE_DIR:PingPong>/assets"
)