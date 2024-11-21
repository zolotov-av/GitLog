cmake_minimum_required(VERSION 3.16)

project(GitCore VERSION 1.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Quick)

qt_standard_project_setup()

add_library(GitCore STATIC
    GitTools/GitApplication.h
    GitTools/GitApplication.cpp
    GitTools/RefsModel.h
    GitTools/RefsModel.cpp
    GitTools/CommitInfo.h
    GitTools/QuickDialog.h
    GitTools/QuickDialog.cpp
    GitTools/CreateBranchDialog.h
    GitTools/CreateBranchDialog.cpp
    GitTools/GitCommitFiles.h
    GitTools/GitLogModel.h
    GitTools/reference_info.h
    GitTools/GitCommitFiles.cpp
    GitTools/GitLogModel.cpp
    GitTools/DiffModel.h
    GitTools/DiffModel.cpp
)

qt_add_qml_module(GitCore
    URI "GitCore"
    VERSION 1.0
    PLUGIN_TARGET GitCore_plugin
    QML_FILES
        qml/GitTheme.qml
        qml/LineInput.qml
        qml/MultiLineInput.qml
        qml/DiffArea.qml
        qml/RefsComboBox.qml
        qml/CreateBranchDialog.qml
        qml/DeleteBranchDialog.qml
    SOURCES
        GraphItem.h
        GraphItem.cpp
    RESOURCE_PREFIX "/qt/qml"
)

set_target_properties(GitCore PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(GitCore PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(GitCore PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(GitCore PROPERTIES AUTOMOC ON)
set_target_properties(GitCore PROPERTIES AUTORCC ON)
set_target_properties(GitCore PROPERTIES AUTOUIC OFF)

target_include_directories(GitCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GitCore PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/GitCore_autogen/include)

target_link_libraries(GitCore PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Quick aw::Core)
target_link_libraries(GitCore PUBLIC gitcxx)
