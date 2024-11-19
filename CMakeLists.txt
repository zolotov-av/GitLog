cmake_minimum_required(VERSION 3.16)

project(GitTools VERSION 1.0 LANGUAGES C CXX)

find_package(Qt6 REQUIRED COMPONENTS Quick Gui)
find_package(Doxygen REQUIRED)

qt_standard_project_setup()

add_custom_target(GitToolsInfo SOURCES
    README.md
    mainpage.dox
)

add_subdirectory(GitCore)
add_subdirectory(GitLog)
add_subdirectory(GitDiff)

doxygen_add_docs(docs
    ${CMAKE_CURRENT_SOURCE_DIR}
    #ALL
    COMMENT "Generating docs"
)
