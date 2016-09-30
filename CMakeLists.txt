cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(lxqt-notificationd)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED QUIET)
find_package(Qt5DBus REQUIRED QUIET)
find_package(Qt5LinguistTools REQUIRED QUIET)
find_package(KF5WindowSystem REQUIRED QUIET)

find_package(lxqt REQUIRED QUIET)

include(GNUInstallDirs)
include(LXQtCompilerSettings NO_POLICY_SCOPE)
include(LXQtTranslate)

add_subdirectory(src)
add_subdirectory(config)
