import QtQuick
import FluentUI

Item {
    id: app

    Component.onCompleted: {
        FluApp.init(app)
        // FluApp.vsync = true
        FluApp.routes = {
            "/login":"qrc:/res/qml/window/LoginWindow.qml",
            "/setting":"qrc:/res/qml/window/SettingWindow.qml",
            "/":"qrc:/res/qml/window/MainWindow.qml",
        }
        if(SettingsManager.isLogin()){
            FluApp.initialRoute = "/"
        }else{
            FluApp.initialRoute = "/login"
        }
        FluApp.run()
    }
}
