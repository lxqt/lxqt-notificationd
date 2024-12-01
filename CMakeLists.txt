cmake_minimum_required(VERSION 3.18.0 FATAL_ERROR)
# CMP0000: Call the cmake_minimum_required() command at the beginning of the top-level
# CMakeLists.txt file even before calling the project() command.
# The cmake_minimum_required(VERSION) command implicitly invokes the cmake_policy(VERSION)
# command to specify that the current project code is written for the given range of CMake
# versions.
project(lxqt-notificationd)

include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)
option(LXQT_NOTIFICATION_BUILD_TESTS "Build LXQt Notification tests" OFF)

set(KF6_MINIMUM_VERSION "6.0.0")
set(LXQT_MINIMUM_VERSION "2.1.0")
set(QT_MINIMUM_VERSION "6.6.0")
set(SHELLQT_MINIMUM_VERSION "6.0.0")

find_package(Qt6DBus ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt6LinguistTools ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt6Widgets ${QT_MINIMUM_VERSION} REQUIRED)
find_package(KF6WindowSystem ${KF6_MINIMUM_VERSION} REQUIRED)
find_package(lxqt ${LXQT_MINIMUM_VERSION} REQUIRED)
find_package(LayerShellQt ${SHELLQT_MINIMUM_VERSION} REQUIRED)
message(STATUS "Building with Qt${Qt6Core_VERSION}")

# Patch Version
set(LXQT_NOTIFICATIOND_PATCH_VERSION 1)

set(LXQT_NOTIFICATIOND_VERSION ${LXQT_MAJOR_VERSION}.${LXQT_MINOR_VERSION}.${LXQT_NOTIFICATIOND_PATCH_VERSION})
add_definitions(
    "-DLXQT_NOTIFICATIOND_VERSION=\"${LXQT_NOTIFICATIOND_VERSION}\""
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
