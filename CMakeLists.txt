cmake_minimum_required(VERSION 3.24)
project(VideoApp)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/glfw)

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


add_executable(VideoApp src/main.cpp)
target_link_libraries(VideoApp glfw ${EXTRA_LIBS})
