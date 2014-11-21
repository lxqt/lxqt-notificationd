project(lxqt-config-notificationd)

include_directories (
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

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
lxqt_translate_ts(NOTIFICATIONS_CONF_QM SOURCES
    UPDATE_TRANSLATIONS ${UPDATE_TRANSLATIONS}
    ${NOTIFICATIONS_CONF_MOC}
    ${NOTIFICATIONS_CONF_SRC}
    ${NOTIFICATIONS_CONF_UI}
    INSTALL_DIR "${LXQT_TRANSLATIONS_DIR}/${PROJECT_NAME}"
)

lxqt_app_translation_loader(QM_LOADER ${PROJECT_NAME})
lxqt_translate_desktop(DESKTOP_FILES SOURCES lxqt-config-notificationd.desktop.in)
#************************************************

qt5_wrap_ui(NOTIFICATIONS_CONF_UI_CPP ${NOTIFICATIONS_CONF_UI})

add_executable(${PROJECT_NAME}
    ${NOTIFICATIONS_CONF_SRC}
    ${NOTIFICATIONS_CONF_UI_CPP}
    ${NOTIFICATIONS_CONF_QM}
    ${DESKTOP_FILES}
    ${QM_LOADER}
)

target_link_libraries(${PROJECT_NAME}
    KF5::WindowSystem
    Qt5::Widgets
    ${LXQT_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES ${DESKTOP_FILES} DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
