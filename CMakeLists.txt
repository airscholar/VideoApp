cmake_minimum_required(VERSION 3.24)
project(VideoApp)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(lib/glfw)
add_subdirectory(lib/FFmpeg)
add_definitions(-DGL_SILENCE_DEPRECATION)

if(APPLE)
    list(APPEND EXTRA_LIBS
        "-framework OpenGL"
    )
elseif (WIN32)
    list(APPEND EXTRA_LIBS
        "lglu32 -lopengl32"
    )
    set (CMAKE_EXE_LINKER_FLAGS "-std=gnu99 -static-libgcc -static-libstdc++ -static -lpthread -lwinpthread -mwindows")
else ()
    list(APPEND EXTRA_LIBS
            "lGL -lGLU -lX11"
    )

endif()

list(APPEND SOURCES
    src/main.cpp
    src/loadFrame.cpp
)

add_executable(VideoApp ${SOURCES})
target_link_libraries(VideoApp FFmpeg glfw ${EXTRA_LIBS})
