if(USE_QT5)
    set(QTX_INCLUDE_DIRS "")
    set(QTX_LIBRARIES Qt5::Widgets Qt5::DBus)
else()
    set(QTX_INCLUDE_DIRS ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTDBUS_DIR})
    set(QTX_LIBRARIES ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTDBUS_LIBRARY})
endif()

add_definitions(-DPROJECT_NAME=\"${PROJECT_NAME}\")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QTX_INCLUDE_DIRS}
)

set(NOTIFICATIONS_SRC
    main.cpp
    notificationarea.cpp
    notification.cpp
    notificationlayout.cpp
    notificationwidgets.cpp
    notifyd.cpp
)

set(NOTIFICATIONS_UI
    notification.ui
)

if(USE_QT5)
    qt5_wrap_ui(NOTIFICATIONS_UI_CPP ${NOTIFICATIONS_UI})
    qt5_add_dbus_adaptor(NOTIFICATIONS_SRC
        org.freedesktop.Notifications.xml
        notifyd.h Notifyd
    )
else()
    qt4_wrap_ui(NOTIFICATIONS_UI_CPP ${NOTIFICATIONS_UI})
    qt4_add_dbus_adaptor(NOTIFICATIONS_SRC
        org.freedesktop.Notifications.xml
        notifyd.h Notifyd
    )
endif()

# Translations **********************************
include(LxQtTranslate)
lxqt_translate_ts(NOTIFICATIONS_QM_FILES
    SOURCES
        ${NOTIFICATIONS_SRC}
        ${NOTIFICATIONS_UI}
    INSTALLATION_DIRECTORY
        ${LXQT_TRANSLATIONS_DIR}/${PROJECT_NAME}
)
#************************************************

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})

add_executable(lxqt-notificationd
    ${NOTIFICATIONS_SRC}
    ${NOTIFICATIONS_UI_CPP}
    ${NOTIFICATIONS_QM_FILES}
)

target_link_libraries(lxqt-notificationd ${QTX_LIBRARIES} ${QTXDG_LIBRARIES} ${LXQT_LIBRARIES})
install(TARGETS lxqt-notificationd RUNTIME DESTINATION bin)
