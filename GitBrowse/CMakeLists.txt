cmake_minimum_required(VERSION 3.16)

project(GitBrowse VERSION 1.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Quick Gui)

qt_standard_project_setup(REQUIRES 6.4)

qt_add_executable(GitBrowse
    main.cpp
)

qt_add_qml_module(GitBrowse
    URI Browse
    VERSION 1.0
    QML_FILES
        GitBrowse.qml
    DEPENDENCIES
        GitCore
)

set_target_properties(GitBrowse PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(GitBrowse PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(GitBrowse PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(GitBrowse PROPERTIES AUTOMOC ON)
set_target_properties(GitBrowse PROPERTIES AUTORCC OFF)
set_target_properties(GitBrowse PROPERTIES AUTOUIC OFF)

target_link_libraries(GitBrowse PRIVATE Qt6::Quick Qt6::Gui)
target_link_libraries(GitBrowse PRIVATE GitCore GitCore_plugin)
