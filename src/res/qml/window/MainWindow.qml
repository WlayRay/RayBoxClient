import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window
import Qt.labs.platform
import FluentUI
import "../component"
import "../global"

FluWindow {
    id:window
    width: 800
    height: 600
    minimumWidth: 600
    minimumHeight: 400
    launchMode: FluWindowType.SingleTask
    appBar: null

    onActiveChanged: {
        timer_trayIcon_flash.running = false;
        trayIcon.icon.source = "qrc:/res/image/favicons/spongebob.ico"
        if (active) {
            SettingsManager.saveFocusState(true)
        } else {
            SettingsManager.saveFocusState(false)
        }
    }
    Connections{
        target: MainGlobal
        function onSwitchSessionEvent(uid){
            nav_view.startPageByItem(pane_item_session)
        }
    }
    Connections{
        target: SingleInstanceChecker
        function onActivate(){
            window.show()
            window.raise()
            window.requestActivate()
        }
    }
    Connections{
        target: MessageManager
        function onNewMsgArrived(){
            window.show()
            window.raise()
            window.requestActivate()
            trayIcon.showMessage(SettingsManager.getLatestMsgUserName(), SettingsManager.getLatestMsg())
            timer_trayIcon_flash.running = true;
        }
    }


    AvatarView{
        width: 48
        height: 48
        uId: SettingsManager.getAccount()
        uisGroup: false
        anchors{
            left: parent.left
            top: parent.top
            topMargin: 8
            leftMargin: (nav_view.navCompactWidth - width) / 2
        }
    }

    FluObject{
        id:items_original
        FluPaneItem{
            id:pane_item_session
            count: 99
            title: "聊天"
            icon:FluentIcons.Message
            url:"qrc:/res/qml/page/SessionPage.qml"
            onTap:{
                SettingsManager.saveSessionSearchName("")
                nav_view.push(url)
            }
        }
        FluPaneItem{
            id:pane_item_contacts
            count: 99
            title: "联系人"
            icon:FluentIcons.ContactPresence
            url:"qrc:/res/qml/page/ContactsPage.qml"
            onTap:{
                nav_view.push(url)
            }
        }
    }

    FluObject{
        id:items_footer
        FluPaneItem{
            title:"设置"
            icon:FluentIcons.Settings
            onTapListener:function(){
                FluApp.navigate("/setting")
            }
        }
    }

    FluAppBar {
        id:app_bar
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        z:7
        closeClickListener: () => {window.hide()}
    }

    FluNavigationView{
        id:nav_view
        displayMode:FluNavigationViewType.Compact
        navCompactWidth: 60
        width: parent.width
        height: parent.height
        pageMode: FluNavigationViewType.Stack
        items: items_original
        footerItems:items_footer
        hideNavAppBar: true
        navTopMargin: 54
        Component.onCompleted: {
            setCurrentIndex(0)
        }
    }



    Timer {
        id: timer_trayIcon_flash
        interval: 500; running: false; repeat: true
        onTriggered: {
            if(trayIcon.transparent){
                trayIcon.icon.source = "qrc:/res/image/favicons/spongebob.ico"
                trayIcon.transparent = false;
            }else{
                trayIcon.icon.source = "qrc:/res/image/favicons/spongebob-transparent.ico"
                trayIcon.transparent = true;
            }
        }
    }

    Connections {
        target: trayIcon
        function onActivated(reason) {
            timer_trayIcon_flash.running = false;
            trayIcon.icon.source = "qrc:/res/image/favicons/spongebob.ico"
            window.show()
            window.raise()
            window.requestActivate()
        }
    }

    Window{
        flags: Qt.FramelessWindowHint
        width: 330
        height: 34
        visible: !MessageManager.isConnected
        color: Qt.rgba(204/255,83/255,83/255,1)
        x:window.x + (window.width - width)/2
        y:Math.max(window.y - height - 5,0)
        Row{
            anchors.centerIn: parent
            Text{
                text:"网络不可用, 请检查你的网络设置"
                color: FluColors.White
                font.pixelSize: 15
            }
        }
    }
}
