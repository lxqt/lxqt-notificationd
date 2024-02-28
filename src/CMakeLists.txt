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

Qt6_add_dbus_adaptor(NOTIFICATIONS_DBUS_SRC
    org.freedesktop.Notifications.xml
    notifyd.h Notifyd
)

set_property(SOURCE ${NOTIFICATIONS_DBUS_SRC} PROPERTY SKIP_AUTOGEN ON)
list(APPEND NOTIFICATIONS_SRC "${NOTIFICATIONS_DBUS_SRC}")

# Translations **********************************

#************************************************

add_executable(lxqt-notificationd
    ${NOTIFICATIONS_SRC}
    ${NOTIFICATIONS_QM_FILES}
    ${QM_LOADER}
)

target_link_libraries(lxqt-notificationd
    Qt6::Widgets
    Qt6::DBus
    lxqt
    KF6::WindowSystem
)

install(TARGETS
    lxqt-notificationd
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT Runtime
)
