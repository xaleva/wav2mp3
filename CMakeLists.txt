cmake_minimum_required (VERSION 2.6)
project (MP3Converter)
set(CMAKE_CXX_STANDARD 14)


include_directories(3rdparty/lame-3.100/include)
include_directories(include)

add_library(lame STATIC IMPORTED)
set_property(TARGET lame PROPERTY
    IMPORTED_LOCATION ../3rdparty/lame-3.100/libmp3lame/.libs/libmp3lame.a)


set(SOURCE_FILES  src/FileConverter.cpp
                    src/ThreadPool.cpp
                    src/MemoryManager.cpp
                    src/Packet.cpp
                    src/MP3Encoder.cpp
                    src/FileWriter.cpp
                    src/SystemOutput.cpp)

add_executable(mp3Converter main.cpp ${SOURCE_FILES})

target_link_libraries(mp3Converter pthread lame)
