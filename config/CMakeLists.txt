project(lxqt-config-notificationd)

if(USE_QT5)
    set(QTX_INCLUDE_DIRS ${Qt5Widgets_INCLUDE_DIR})
    set(QTX_LIBRARIES ${Qt5Widgets_LIBRARY})
else()
    set(QTX_INCLUDE_DIRS ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
    set(QTX_LIBRARIES ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
endif()

include_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

set(NOTIFICATIONS_CONF_MOC
    advancedsettings.h
    basicsettings.h
    mainwindow.h
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
lxqt_translate_ts(NOTIFICATIONS_CONF_QM
    SOURCES
        ${NOTIFICATIONS_CONF_MOC}
        ${NOTIFICATIONS_CONF_SRC}
        ${NOTIFICATIONS_CONF_UI}
)

lxqt_translate_desktop(DESKTOP_FILES
    SOURCES
        lxqt-config-notificationd.desktop.in
)
#************************************************

if(USE_QT5)
    qt5_wrap_cpp(NOTIFICATIONS_CONF_MOC_CPP ${NOTIFICATIONS_CONF_MOC})
    qt5_wrap_ui(NOTIFICATIONS_CONF_UI_CPP ${NOTIFICATIONS_CONF_UI})
else()
    qt4_wrap_cpp(NOTIFICATIONS_CONF_MOC_CPP ${NOTIFICATIONS_CONF_MOC})
    qt4_wrap_ui(NOTIFICATIONS_CONF_UI_CPP ${NOTIFICATIONS_CONF_UI})
endif()

add_executable(${PROJECT_NAME} ${NOTIFICATIONS_CONF_SRC}
                               ${NOTIFICATIONS_CONF_MOC_CPP}
                               ${NOTIFICATIONS_CONF_UI_CPP}
                               ${NOTIFICATIONS_CONF_QM}
                               ${DESKTOP_FILES})

target_link_libraries(${PROJECT_NAME} ${QT_QTCORE_LIBRARY}
                                      ${QT_QTGUI_LIBRARY}
                                      ${QT_QTDBUS_LIBRARY}
                                      ${LXQT_LIBRARIES})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES ${DESKTOP_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
