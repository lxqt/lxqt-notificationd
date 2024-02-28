project(lxqt-config-notificationd)

set(NOTIFICATIONS_CONF_SRC
    advancedsettings.cpp
    basicsettings.cpp
    mainwindow.cpp
    main.cpp
)

set(NOTIFICATIONS_CONF_UI
    advancedsettings.ui
    basicsettings.ui
)


# Translations **********************************

#************************************************


add_executable(${PROJECT_NAME}
    ${NOTIFICATIONS_CONF_SRC}
    ${NOTIFICATIONS_CONF_QM}
    ${DESKTOP_FILES}
    ${QM_LOADER}
)

target_link_libraries(${PROJECT_NAME}
    KF6::WindowSystem
    Qt6::Widgets
    lxqt
)

install(TARGETS
    ${PROJECT_NAME}
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT Runtime
)
install(FILES
    ${DESKTOP_FILES}
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
    COMPONENT Runtime
)
