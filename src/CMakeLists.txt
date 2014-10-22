set(QTX_INCLUDE_DIRS "")
set(QTX_LIBRARIES Qt5::Widgets Qt5::DBus)

add_definitions(-DPROJECT_NAME=\"${PROJECT_NAME}\")

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
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

qt5_wrap_ui(NOTIFICATIONS_UI_CPP ${NOTIFICATIONS_UI})
qt5_add_dbus_adaptor(NOTIFICATIONS_SRC
    org.freedesktop.Notifications.xml
    notifyd.h Notifyd
)

# Translations **********************************
include(LxQtTranslate)
lxqt_translate_ts(NOTIFICATIONS_QM_FILES SOURCES
    ${NOTIFICATIONS_SRC}
    ${NOTIFICATIONS_UI}
    INSTALLATION_DIRECTORY "${LXQT_TRANSLATIONS_DIR}/${PROJECT_NAME}"
)
#************************************************

add_executable(lxqt-notificationd
    ${NOTIFICATIONS_SRC}
    ${NOTIFICATIONS_UI_CPP}
    ${NOTIFICATIONS_QM_FILES}
)

target_link_libraries(lxqt-notificationd ${QTX_LIBRARIES} ${QTXDG_LIBRARIES} ${LXQT_LIBRARIES} KF5::WindowSystem)
install(TARGETS lxqt-notificationd RUNTIME DESTINATION bin)
