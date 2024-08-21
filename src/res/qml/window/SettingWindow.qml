import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window
import FluentUI
import "../component"

FluWindow {
    id:window
    width: 560
    height: 480
    showStayTop: false
    fixSize: true
    title: "设置"
    launchMode: FluWindowType.SingleTask

    component TabItem:Item{
        property bool selected: listview_tab.currentIndex === model.index
        width: listview_tab.width
        height: 30
        FluText{
            anchors.centerIn: parent
            text:model.title
            color: selected ? FluTheme.primaryColor : textColor
        }
        TapHandler{
            onTapped: {
                listview_tab.currentIndex = model.index
            }
        }
    }

    ListModel{
        id:listmodel_tab
        ListElement{
            title:"账号设置"
            comId: function(){return com_account_settings}
        }
        ListElement{
            title:"通用设置"
            comId: function(){return com_account_settings}
        }
        ListElement{
            title:"快捷键"
            comId: function(){return com_account_settings}
        }
        ListElement{
            title:"关于"
            comId: function(){return com_about}
        }
    }

    ListView{
        id:listview_tab
        currentIndex: 0
        width: 100
        anchors{
            top: parent.top
            topMargin: 50
            left: parent.left
            bottom: parent.bottom
        }
        model:listmodel_tab
        delegate: TabItem{}
    }

    Rectangle{
        width: 1
        anchors{
            top: listview_tab.top
            right: listview_tab.right
            bottom: parent.bottom
        }
        color: Qt.rgba(214/255,214/255,214/255,1)
    }

    Component{
        id:com_account_settings
        Item{
            Component{
                id:com_profile_panne
                FluArea{
                    AvatarView{
                        id: profile_avatar
                        width: 60
                        height: 60
                        uId: SettingsManager.getAccount()
                        uisGroup: false
                        anchors{
                            left: parent.left
                            leftMargin: 30
                            verticalCenter: parent.verticalCenter
                        }
                    }

                    FluText{
                        text:"账号：%1".arg(SettingsManager.getAccount())
                        font.pixelSize: 20
                        anchors{
                            left: profile_avatar.right
                            top: profile_avatar.top
                            leftMargin: 15
                        }
                    }
                }
            }

            FluLoader{
                height: 140
                width: parent.width
                sourceComponent: com_profile_panne
            }


            FluFilledButton{
                text:"退出登录"
                anchors{
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                    bottomMargin: 20
                }
                onClicked: {
                    dialog_logout.open()
                }
            }
        }
    }

    Component{
        id:com_about
        Item{
            FluArea{
                height: 120
                width: parent.width
                FluText{
                    text:"Developed by WlayRay, see [raybox server](https://github.com/WlayRay/RayBoxServer) for the source code of the RayBox server, see [raybox client](https://github.com/WlayRay/RayBoxClient) for the source code of this RayBox client."
                    textFormat: Text.RichText
                    width: parent.width
                    // padding: 12
                    color: FluColors.Black
                    wrapMode: Text.WrapAnywhere
                }
            }
        }
    }

    FluLoader{
        sourceComponent: listmodel_tab.get(listview_tab.currentIndex).comId()
        anchors{
            left: listview_tab.right
            leftMargin: 48
            right: parent.right
            rightMargin: 48
            top: listview_tab.top
            bottom: parent.bottom
            bottomMargin: 40
        }
    }

    FluContentDialog{
        id:dialog_logout
        title:"退出登录"
        message:"退出登录后将无法收到新消息，确定退出登录？"
        buttonFlags: FluContentDialogType.NegativeButton | FluContentDialogType.PositiveButton
        negativeText: "取消"
        positiveText:"确定"
        onPositiveClicked:{
            showLoading()
            SettingsManager.saveLogout()
            delay_restart.restart()
        }
    }

    Timer{
        id:delay_restart
        interval: 300
        onTriggered: {
            FluApp.exit(931)
        }
    }

}
