cmake_minimum_required(VERSION 3.16)

project(GitDiff VERSION 1.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2)

add_executable(GitDiff
    GitDiff.h
    GitDiff.cpp
    main.cpp
    GitDiffResources.qrc
)

set_target_properties(GitDiff PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(GitDiff PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(GitDiff PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(GitDiff PROPERTIES AUTOMOC ON)
set_target_properties(GitDiff PROPERTIES AUTORCC ON)
set_target_properties(GitDiff PROPERTIES AUTOUIC OFF)

target_link_libraries(GitDiff PRIVATE GitCore)
target_link_libraries(GitDiff PRIVATE Qt6::Core Qt6::Gui Qt6::Quick Qt6::QuickControls2)
