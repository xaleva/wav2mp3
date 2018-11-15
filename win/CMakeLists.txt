cmake_minimum_required (VERSION 2.6)
project (MP3Converter)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER i686-w64-mingw32-c++)
set(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(BUILD_SHARED_LIBS OFF)
set(DLL_TOOL i686-w64-mingw32-dlltool)
set( CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "" )
set( CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-static-libgcc -static-libstdc++ -Wl,-Bstatic" )

include_directories(../3rdparty/lame-3.100/include)
include_directories(../include)

add_library(lame STATIC IMPORTED)
set_property(TARGET lame PROPERTY
    IMPORTED_LOCATION ../win/libmp3lame.a)


set(SOURCE_FILES  ../src/FileConverter.cpp
                    ../src/ThreadPool.cpp
                    ../src/MemoryManager.cpp
                    ../src/Packet.cpp
                    ../src/MP3Encoder.cpp
                    ../src/FileWriter.cpp
                    ../src/SystemOutput.cpp)

add_executable(mp3Converter ../main.cpp ${SOURCE_FILES})

target_link_libraries(mp3Converter pthread lame)
