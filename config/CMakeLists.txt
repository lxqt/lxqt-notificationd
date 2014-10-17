project(lxqt-config-notificationd)

set(QTX_INCLUDE_DIRS "")
set(QTX_LIBRARIES Qt5::Widgets)

add_definitions(-DPROJECT_NAME=\"${PROJECT_NAME}\")

include_directories (
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    ${QTX_INCLUDE_DIRS}
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
include(LxQtTranslate)
lxqt_translate_ts(NOTIFICATIONS_CONF_QM SOURCES
    ${NOTIFICATIONS_CONF_MOC}
    ${NOTIFICATIONS_CONF_SRC}
    ${NOTIFICATIONS_CONF_UI}
    INSTALLATION_DIR "${LXQT_TRANSLATIONS_DIR}/${PROJECT_NAME}"
)

lxqt_translate_desktop(DESKTOP_FILES SOURCES lxqt-config-notificationd.desktop.in)
#************************************************

qt5_wrap_ui(NOTIFICATIONS_CONF_UI_CPP ${NOTIFICATIONS_CONF_UI})

add_executable(${PROJECT_NAME}
    ${NOTIFICATIONS_CONF_SRC}
    ${NOTIFICATIONS_CONF_UI_CPP}
    ${NOTIFICATIONS_CONF_QM}
    ${DESKTOP_FILES}
)

target_link_libraries(${PROJECT_NAME} ${QTX_LIBRARIES} ${LXQT_LIBRARIES})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES ${DESKTOP_FILES} DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
