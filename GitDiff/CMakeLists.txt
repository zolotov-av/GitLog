cmake_minimum_required(VERSION 3.16)

project(GitDiff VERSION 1.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Quick Gui)

qt_standard_project_setup(REQUIRES 6.4)

qt_add_executable(GitDiff
    main.cpp
)

qt_add_qml_module(GitDiff
    URI MyDiff
    VERSION 1.0
    QML_FILES
        GitDiff.qml
    SOURCES
        GitDiff.h
        GitDiff.cpp
    RESOURCES
        tmp/left.txt
        tmp/right.txt
    DEPENDENCIES
        GitCore
)

set_target_properties(GitDiff PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(GitDiff PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(GitDiff PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(GitDiff PROPERTIES AUTOMOC ON)
set_target_properties(GitDiff PROPERTIES AUTORCC OFF)
set_target_properties(GitDiff PROPERTIES AUTOUIC OFF)

target_link_libraries(GitDiff PRIVATE Qt6::Quick Qt6::Gui)
target_link_libraries(GitDiff PRIVATE GitCore GitCore_plugin)
