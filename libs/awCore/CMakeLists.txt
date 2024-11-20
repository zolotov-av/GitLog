cmake_minimum_required(VERSION 3.16)

project(awCore VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Threads REQUIRED)

qt_standard_project_setup()

add_library(awCore STATIC

    export.h
    string.h
    string.cpp
    exception.h
    exception.cpp
    std_io.h
    std_io.cpp
    ThreadData.h
    thread.h
    thread.cpp
    log.h
    log.cpp
    trace.h

)
add_library(aw::Core ALIAS awCore)

set_target_properties(awCore PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(awCore PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(awCore PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(awCore PROPERTIES AUTOMOC ON)
set_target_properties(awCore PROPERTIES AUTOUIC OFF)
set_target_properties(awCore PROPERTIES AUTORCC OFF)

target_compile_definitions(awCore PRIVATE STATIC_AW_CORE)

target_link_libraries(awCore PUBLIC Qt6::Core)
target_link_libraries(awCore PRIVATE Threads::Threads)
