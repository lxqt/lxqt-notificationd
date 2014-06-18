cmake_minimum_required(VERSION 2.8.9)
# additional cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(PROJECT lxqt-notificationd)
project(${PROJECT})

# Support different versions of Qt
option(USE_QT5 "Build with Qt5." $ENV{USE_QT5})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)
if(USE_QT5)
    cmake_minimum_required(VERSION 2.8.11)
    find_package(Qt5Widgets REQUIRED QUIET)
    find_package(Qt5DBus REQUIRED QUIET)
    find_package(Qt5LinguistTools REQUIRED QUIET)

    find_package(lxqt-qt5 REQUIRED QUIET)
    find_package(qt5xdg REQUIRED)
else()
    set(QT_USE_QTDBUS 1)

    find_package(Qt4 REQUIRED)
    include(${QT_USE_FILE})

    find_package(LXQT REQUIRED)
    find_package(QTXDG REQUIRED)
endif()

include(${LXQT_USE_FILE})
include(${QTXDG_USE_FILE})

add_subdirectory(src)
add_subdirectory(config)

# building tarball with CPack -------------------------------------------------
include (InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_VERSION_MAJOR ${LXQT_MAJOR_VERSION})
set (CPACK_PACKAGE_VERSION_MINOR ${LXQT_MINOR_VERSION})
set (CPACK_PACKAGE_VERSION_PATCH ${LXQT_PATCH_VERSION})
set (CPACK_GENERATOR TBZ2)
set (CPACK_SOURCE_GENERATOR TBZ2)
set (CPACK_SOURCE_IGNORE_FILES /build/;.gitignore;.*~;.git;.kdev4;temp)
include (CPack)
