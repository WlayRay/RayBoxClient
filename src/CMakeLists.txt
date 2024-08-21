qt_add_executable(apprayboxclient
    main.cpp res.qrc
)

qt_add_qml_module(apprayboxclient
    URI rayboxclient
    VERSION 1.0
#   all qml files
    QML_FILES res/qml/App.qml res/qml/window/LoginWindow.qml res/qml/window/MainWindow.qml res/qml/window/SettingWindow.qml
    QML_FILES res/qml/component/EmojiPanel.qml res/qml/component/AvatarView.qml
    QML_FILES res/qml/page/ContactsPage.qml res/qml/page/SessionPage.qml
    QML_FILES res/qml/global/MainGlobal.qml res/qml/global/qmldir
#    QML_FILES res/qml/Test.qml

#   all source files
    SOURCES settingsmanager.h settingsmanager.cpp json.h
    SOURCES socketmanager.h socketmanager.cpp
    SOURCES loginmanager.h loginmanager.cpp
    SOURCES messagemanager.h messagemanager.cpp
    SOURCES
    SOURCES message.h message.cpp
    SOURCES session.h session.cpp
    SOURCES contact.h contact.cpp
    SOURCES dbmanager.h dbmanager.cpp
    SOURCES messagedb.h messagedb.cpp
    SOURCES sessiondb.h sessiondb.cpp
    SOURCES contactdb.h contactdb.cpp
    SOURCES baselistmodel.h
    SOURCES messagelistmodel.h messagelistmodel.cpp
    SOURCES EmoticonHelper.h EmoticonHelper.cpp
    SOURCES EmoticonListModel.h EmoticonListModel.cpp
    SOURCES sessionlistmodel.h sessionlistmodel.cpp
    SOURCES messagemodel.h messagemodel.cpp
    SOURCES sessionmodel.h sessionmodel.cpp
    SOURCES contactmodel.h contactmodel.cpp
    SOURCES TextAreaDocument.h TextAreaDocument.cpp
    SOURCES singleinstancechecker.h singleinstancechecker.cpp
    SOURCES contactlistmodel.h contactlistmodel.cpp

#   all resources files
    RESOURCES res/image/favicons/spongebob.ico res/image/favicons/spongebob-transparent.ico res/server.ini
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apprayboxclient PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apprayboxclient
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apprayboxclient PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Quick
    Qt6::Sql
    fluentuiplugin
)
