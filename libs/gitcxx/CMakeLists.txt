cmake_minimum_required(VERSION 3.16)

project(GitCXX VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(PkgConfig REQUIRED)

pkg_check_modules(Git2 REQUIRED IMPORTED_TARGET libgit2)

qt_standard_project_setup()

add_library(gitcxx STATIC

    exception.h
    common.h
    common.cpp
    oid.h
    oid.cpp
    object.h
    object.cpp
    reference.h
    reference.cpp
    signature.h
    blob.h
    blob.cpp
    commit.h
    commit.cpp
    revwalk.h
    revwalk.cpp
    index.h
    index.cpp
    tree.h
    tree.cpp
    diff_file.h
    diff_delta.h
    diff.h
    diff.cpp
    config.h
    config.cpp
    repository.h
    repository.cpp

)
add_library(git::cxx ALIAS gitcxx)

set_target_properties(gitcxx PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(gitcxx PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(gitcxx PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(gitcxx PROPERTIES AUTOMOC OFF)
set_target_properties(gitcxx PROPERTIES AUTOUIC OFF)
set_target_properties(gitcxx PROPERTIES AUTORCC OFF)

target_link_libraries(gitcxx PUBLIC aw::Core Qt6::Core PkgConfig::Git2)
