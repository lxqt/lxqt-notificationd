cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(lxqt-notificationd)

include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)
option(LXQT_NOTIFICATION_BUILD_TESTS "Build LXQt Notification tests" OFF)

find_package(Qt5DBus REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5Widgets REQUIRED)
message(STATUS "Building with Qt${Qt5Core_VERSION}")

find_package(KF5WindowSystem REQUIRED)

find_package(lxqt REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Patch Version
set(LXQT_NOTIFICATIOND_PATCH_VERSION 0)

set(LXQT_NOTIFICATIOND_VERSION ${LXQT_MAJOR_VERSION}.${LXQT_MINOR_VERSION}.${LXQT_NOTIFICATIOND_PATCH_VERSION})
add_definitions(
    "-DLXQT_NOTIFICATIOND_VERSION=\"${LXQT_NOTIFICATIOND_VERSION}\""
    "-DQT_USE_QSTRINGBUILDER"
    "-DQT_NO_CAST_FROM_ASCII"
    "-DQT_NO_CAST_TO_ASCII"
    "-DQT_NO_URL_CAST_FROM_STRING"
    "-DQT_NO_CAST_FROM_BYTEARRAY"
)

include(LXQtPreventInSourceBuilds)
include(LXQtCompilerSettings NO_POLICY_SCOPE)

# Translations **********************************
include(LXQtTranslate)

add_subdirectory(config)
add_subdirectory(src)
if (LXQT_NOTIFICATION_BUILD_TESTS)
    add_subdirectory(test)
endif()

## merged from lxqt-common
add_subdirectory(autostart)
