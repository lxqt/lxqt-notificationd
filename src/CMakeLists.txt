include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
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
lxqt_translate_ts(NOTIFICATIONS_QM_FILES SOURCES
    UPDATE_TRANSLATIONS ${UPDATE_TRANSLATIONS}
    ${NOTIFICATIONS_SRC}
    ${NOTIFICATIONS_UI}
    INSTALL_DIR "${LXQT_TRANSLATIONS_DIR}/${PROJECT_NAME}"
)

lxqt_app_translation_loader(QM_LOADER ${PROJECT_NAME})
#************************************************

add_executable(lxqt-notificationd
    ${NOTIFICATIONS_SRC}
    ${NOTIFICATIONS_UI_CPP}
    ${NOTIFICATIONS_QM_FILES}
    ${QM_LOADER}
)

target_link_libraries(lxqt-notificationd
    Qt5::Widgets
    Qt5::DBus
    ${QTXDG_LIBRARIES}
    ${LXQT_LIBRARIES}
    KF5::WindowSystem
)

install(TARGETS lxqt-notificationd RUNTIME DESTINATION bin)
