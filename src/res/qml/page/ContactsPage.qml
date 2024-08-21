import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window
import FluentUI
import RAYBOX
import "../component"
import "../global"

FluPage{

    id:control
    launchMode: FluPageType.SingleInstance
    property var currentContact
    signal refreshFriends

    Component.onCompleted: {
        control.refreshFriends()
    }
    ContactListModel{
        id:contact_model
    }
    onRefreshFriends: {
        contact_find_page.visible = false
        contact_model.resetData()
    }

    component ContactItem:Rectangle{
        property bool selected: control.currentContact === display
        id:control_contact
        height: 65
        width: 250
        color:{
            if(control_contact.selected){
                return FluTheme.itemCheckColor
            }
            if(mouse_area_contact.containsMouse)
                return FluTheme.itemPressColor
            return FluTheme.itemNormalColor
        }
        MouseArea{
            id:mouse_area_contact
            acceptedButtons: Qt.LeftButton
            anchors.fill: parent
            hoverEnabled: true
            onClicked:
                (mouse)=>{
                    control.currentContact = display
                }
        }
        AvatarView{
            id:item_avatar
            width: 42
            height: 42
            uId: display.userId
            uisGroup: display.isGroup
            anchors{
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 12
            }
        }
        FluText{
            text:display.name
            anchors{
                verticalCenter: parent.verticalCenter
                left: item_avatar.right
                leftMargin: 10
            }
        }
    }
    Item{
        id:layout_contact
        width: 250
        height: parent.height
        Page{
            background: Item{}
            visible: !contact_find_page.visible
            anchors.fill: parent
            Item{
                id:layout_contact_top_bar
                width: parent.width
                height: 60
                FluTextBox{
                    id: input_contact_search
                    property bool isSearching: false
                    width: 200
                    iconSource: FluentIcons.Search
                    placeholderText: "搜索"
                    anchors{
                        bottom: parent.bottom
                        bottomMargin: 8
                        left: parent.left
                        leftMargin: 10
                    }
                    onCommit: (text) => {
                        input_contact_search.isSearching = true
                        console.log("search contact:", text)
                        SettingsManager.saveContactSearchName(text)
                        control.refreshFriends()
                    }
                }
                FluIconButton{
                    width: 24
                    height: 24
                    iconSize: 14
                    verticalPadding: 0
                    horizontalPadding: 0
                    iconSource: FluentIcons.Add
                    anchors{
                        bottom: parent.bottom
                        bottomMargin: 10
                        right: parent.right
                        rightMargin: 8
                    }
                    onClicked: {
                        textbox_find_contact.clear()
                        contact_find_page.visible = true
                    }
                }
            }
            FluDivider{
                anchors.top: layout_contact_top_bar.bottom
            }
            ListView{
                id:list_contacts
                anchors{
                    left: parent.left
                    right: parent.right
                    top: layout_contact_top_bar.bottom
                    bottom: parent.bottom
                }
                header:Item{
                    width:list_contacts.width
                    height: visible?50:0
                    visible:input_contact_search.isSearching
                    FluButton{
                        text:list_contacts.count===0 ? "未找到联系人或群，点击返回":"点击返回"
                        anchors{
                            left: parent.left
                            right: parent.right
                            centerIn: parent
                        }
                        onClicked: {
                            SettingsManager.saveContactSearchName("")
                            input_contact_search.isSearching = false
                            control.refreshFriends()
                        }
                    }
                }
                boundsBehavior: ListView.StopAtBounds
                model: contact_model
                clip: true
                ScrollBar.vertical: FluScrollBar{}
                delegate: ContactItem{}
            }
        }
        Page{
            id:contact_find_page
            anchors.fill: parent
            visible: false
            background: Item{}
            Item{
                id:layout_contact_find_top_bar
                width: parent.width
                height: 60
                FluTextBox{
                    id:textbox_find_contact
                    width: 200
                    iconSource: FluentIcons.Search
                    placeholderText: "账号/群号/群名"
                    anchors{
                        bottom: parent.bottom
                        bottomMargin: 8
                        left: parent.left
                        leftMargin: 10
                    }
                    onTextChanged: {
                        layout_contact_find_empty.visible = false
                    }
                }
                FluTextButton{
                    anchors{
                        bottom: parent.bottom
                        bottomMargin: 8
                        right: parent.right
                        rightMargin: 2
                    }
                    text:"取消"
                    font.pixelSize: 12
                    onClicked: {
                        contact_find_page.visible = false
                    }
                }
            }
            FluDivider{
                anchors.top: layout_contact_find_top_bar.bottom
            }
            Column{
                spacing: 0
                width: parent.width
                anchors{
                    top: layout_contact_find_top_bar.bottom
                }
                Rectangle{
                    id:layout_contact_find_empty
                    width: parent.width
                    height: 60
                    color: Qt.rgba(247/255,247/255,247/255,1)
                    visible: false
                    FluText{
                        color: FluTheme.fontSecondaryColor
                        text:"无法找到该用户或群聊，请检查你填写的账号是否正确"
                        wrapMode: Text.WrapAnywhere
                        horizontalAlignment: Qt.AlignHCenter
                        anchors{
                            left: parent.left
                            right: parent.right
                            leftMargin: 14
                            rightMargin: 14
                            verticalCenter: parent.verticalCenter
                        }
                    }
                }

                Item{
                    width: parent.width
                    height: 60
                    visible: textbox_find_contact.text !== ""
                    Rectangle{
                        anchors.fill: parent
                        color: {
                            if(mouse_contact_find.containsPress){
                                return FluTheme.itemPressColor
                            }
                            if(mouse_contact_find.containsMouse){
                                return FluTheme.itemHoverColor
                            }
                            return FluTheme.itemNormalColor
                        }
                    }
                    MouseArea{
                        id:mouse_contact_find
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            MessageManager.sendSearchAccount(textbox_find_contact.text)
                        }
                    }
                    RowLayout{
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width
                        spacing: 0
                        Rectangle{
                            Layout.preferredWidth: 36
                            Layout.preferredHeight: 36
                            Layout.alignment: Qt.AlignVCenter
                            Layout.leftMargin: 14
                            color: FluTheme.primaryColor
                            radius: 4
                            FluIcon{
                                iconSize: 20
                                anchors.centerIn: parent
                                iconSource: FluentIcons.Search
                                iconColor: FluColors.White
                            }
                        }
                        FluText{
                            text:"搜索: "
                            Layout.leftMargin: 10
                        }
                        FluText{
                            text:textbox_find_contact.text
                            color: FluTheme.primaryColor
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                        }
                        FluIcon{
                            iconSource: FluentIcons.ChevronRight
                            iconSize: 14
                            Layout.rightMargin: 10
                        }
                    }
                }
            }
        }
    }

    Connections{
        target: MessageManager
        function onNewSearchAccount(isEmpty, id, name, isGroup, desc){
            if(isEmpty){
                layout_contact_find_empty.visible = true
            }else{
                console.log("id",id,"name",name,"isGroup",isGroup,"desc",desc)
                loader_popup_user.userId = id
                loader_popup_user.name = name
                loader_popup_user.desc = desc
                loader_popup_user.pisGroup = isGroup
                loader_popup_user.sourceComponent = com_popup_user
            }
        }
        function onNewAddAccount(userId, isGroup){
            control.refreshFriends()
            MainGlobal.switchSessionEvent(userId, isGroup)
        }
    }

    FluLoader{
        id:loader_popup_user
        property string userId
        property string name
        property string desc
        property bool pisGroup
        sourceComponent: undefined
    }

    Component{
        id:com_popup_user
        FluPopup{
            id: popup_user
            width: 240
            height: 180
            modal: true
            anchors.centerIn: undefined
            x:255
            y:60
            visible: true
            onVisibleChanged: {
                if(!visible){
                    loader_popup_user.sourceComponent = undefined
                }
            }
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
            contentItem: Item{
                AvatarView{
                    id: popup_user_avatar
                    width: 60
                    height: 60
                    uId: userId
                    uisGroup: pisGroup
                    anchors{
                        top: parent.top
                        left: parent.left
                        topMargin: 20
                        leftMargin: 20
                    }
                }

                Column{
                    anchors{
                        left: popup_user_avatar.right
                        top: popup_user_avatar.top
                        leftMargin: 20
                        topMargin: 4
                    }
                    FluText{
                        id: popup_user_name
                        text: name
                    }

                    FluText{
                        id:popup_user_uid
                        color : FluTheme.fontTertiaryColor
                        text: "账号: " + userId
                    }

                    Flickable{
                        id: popup_user_desc
                        width: 120
                        height: 66
                        clip:true
                        flickableDirection: Flickable.VerticalFlick
                        ScrollBar.vertical: FluScrollBar {}
                        boundsBehavior: Flickable.StopAtBounds
                        TextArea.flickable:FluMultilineTextBox{
                            padding:0
                            leftPadding: -1
                            width:parent.width
                            text: desc
                            enabled: false
                            background: Item{}
                            color : FluTheme.fontTertiaryColor
                        }
                    }
                }

                RowLayout{
                    anchors{
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                        bottomMargin: 12
                    }
                    FluFilledButton{
                        text: pisGroup ? "加入群聊" : "添加好友"
                        onClicked:{
                            MessageManager.sendAddAccount(userId,pisGroup)
                            popup_user.visible = false
                        }
                    }
                    FluFilledButton{
                        text: "取消"
                        onClicked:{
                            popup_user.visible = false
                        }
                    }
                }
            }
        }
    }

    Component{
        id:com_contact_info_panne
        Item{
            id:layout_contact_info_panne
            Item{
                height: parent.height
                width: parent.width-40
                anchors.horizontalCenter: parent.horizontalCenter
                AvatarView{
                    id:contact_info_avatar
                    width: 60
                    height: 60
                    radius:[5,5,5,5]
                    anchors{
                        top: parent.top
                        left: parent.left
                        topMargin: 62
                    }
                    uId: currentContact.userId
                    uisGroup: currentContact.isGroup
                }

                ColumnLayout{
                    spacing: 0
                    anchors{
                        top: contact_info_avatar.top
                        left: contact_info_avatar.right
                        leftMargin: 16
                    }

                    FluText{
                        text:currentContact.name

                    }
                    FluText{
                        text: "账号: %1".arg(currentContact.userId)
                        color: FluTheme.fontTertiaryColor
                    }
                }


                FluDivider{
                    id:contact_info_divider_1
                    height: 1
                    width: parent.width
                    anchors{
                        top: contact_info_avatar.bottom
                        topMargin: 20
                    }
                }

                RowLayout{
                    anchors{
                        top: contact_info_divider_1.bottom
                        topMargin: 20
                    }

                    FluIconButton{
                        iconSource:FluentIcons.Message
                        text:"发消息"
                        iconColor: Qt.rgba(123/255,138/255,171/255,1)
                        textColor: iconColor
                        display: Button.TextUnderIcon
                        iconSize: 18
                        font.pixelSize: 12
                        onClicked: {
                            MainGlobal.switchSessionEvent(currentContact.userId, currentContact.isGroup)
                        }
                    }
                }
            }
        }
    }


    FluDivider{
        width: 1
        height: layout_contact.height
        orientation: Qt.Vertical
        anchors{
            left: layout_contact.left
        }
    }
    FluDivider{
        width: 1
        height: layout_contact.height
        orientation: Qt.Vertical
        anchors{
            left: layout_contact.right
        }
    }

    FluLoader{
        anchors{
            left: layout_contact.right
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
        sourceComponent: control.currentContact ? com_contact_info_panne : undefined
    }
}
