cmake_minimum_required(VERSION 3.31)
project(Graphite)

set(CMAKE_CXX_STANDARD 17)

add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

add_subdirectory(glfw)

add_executable(Graphite testing/main.cpp)
target_include_directories(Graphite PUBLIC glad/include glfw/include)
target_link_libraries(Graphite PRIVATE glfw glad)

# === Platform-specific system linking ===
if(APPLE)
    target_link_libraries(Graphite PRIVATE
            "-framework Cocoa"
            "-framework OpenGL"
            "-framework IOKit"
            "-framework CoreVideo"
            "-framework QuartzCore"
    )
elseif(UNIX)
    target_link_libraries(Graphite PRIVATE
            GL X11 Xrandr Xi pthread dl
    )
elseif(WIN32)
    target_link_libraries(Graphite PRIVATE
            opengl32 gdi32 user32
    )
endif()