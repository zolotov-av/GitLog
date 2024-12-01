cmake_minimum_required(VERSION 3.16)

project(GitStatus VERSION 1.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Quick Gui)

qt_standard_project_setup(REQUIRES 6.4)

qt_add_executable(GitStatus
    main.cpp
)

qt_add_qml_module(GitStatus
    URI Status
    VERSION 1.0
    QML_FILES
        GitStatusPage.qml
        GitDiffPage.qml
        GitStatus.qml
    DEPENDENCIES
        GitCore
)

set_target_properties(GitStatus PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(GitStatus PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(GitStatus PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(GitStatus PROPERTIES AUTOMOC ON)
set_target_properties(GitStatus PROPERTIES AUTORCC OFF)
set_target_properties(GitStatus PROPERTIES AUTOUIC OFF)

target_link_libraries(GitStatus PRIVATE Qt6::Quick Qt6::Gui)
target_link_libraries(GitStatus PRIVATE GitCore GitCore_plugin)
