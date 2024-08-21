import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Shapes
import FluentUI
import RAYBOX
import "../component"
import "../global"

FluPage{

    id:control
    launchMode: FluPageType.SingleInstance
    property var currentSession : null
    SessionListSortProxyModel{
        id:session_sort_model
        model:session_model
    }

    SessionListModel{
        id:session_model
    }

    Component.onCompleted: {
        refreshSession()
    }
    Connections{
        target: MainGlobal
        function onSwitchSessionEvent(userId, isGroup){
            control.currentSession = session_model.getSessionByUserId(userId, isGroup)
            console.log("control.currentSession.id", control.currentSession.id)
            SettingsManager.saveLastSessionId(control.currentSession.id)
        }
    }
    Connections{
        target: MessageManager
        function onNewMsgArrived(){
            refreshSession()
        }
        function onNewMsgSend(){
            refreshSession()
        }
        function onFailedToSendNewMsg(toId, msg){
            console.log("failed to send msg, toId:", toId,", msg:", msg)
            showError("消息发送失败，请重新运行软件后重试")
        }
        function onNewCreatedGroup(sessionId){
            session_model.resetData()
            switchSession(sessionId)
        }
    }

    function switchSession(sessionId){
        control.currentSession = session_model.getSessionBySessionId(sessionId)
        SettingsManager.saveLastSessionId(sessionId)

    }

    function refreshSession(){
        session_model.resetData()
        var lastSessionId = SettingsManager.getLastSessionId();
        console.log("lastSessionId", lastSessionId)
        if(lastSessionId){
            switchSession(lastSessionId)
        }else{
            switchSession(session_model.data(session_model.index(0,0)).id)
        }
    }

    onCurrentSessionChanged: {
        if(currentSession){
            loader_session.sourceComponent = undefined
            loader_session.sourceComponent = com_message_panne
        }else{
            loader_session.sourceComponent = undefined
        }
    }

    // 单个会话项
    component SessionItem:Rectangle{
        property bool selected: control.currentSession === display
        id:control_session
        height: 65
        width: 250
        color:{
            if(control_session.selected){
                return FluTheme.itemCheckColor
            }
            if(mouse_area_session.containsMouse)
                return FluTheme.itemPressColor
            return FluTheme.itemNormalColor
        }
        AvatarView{
            id:item_avatar
            width: 45
            height: 45
            uId: display.fromId
            uisGroup: display.isGroup
            anchors{
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: (parent.height-height)/2
            }
        }
        FluText{
            id:item_session_name
            text: display.username
            maximumLineCount: 1
            elide: Text.ElideRight
            anchors{
                top: item_avatar.top
                topMargin: 2
                left: item_avatar.right
                leftMargin: 10
                right: item_session_time.left
                rightMargin: 10
            }
        }
        FluText{
            id:item_session_time
            text: display.lastMsgTimeStr
            anchors{
                verticalCenter: item_session_name.verticalCenter
                right: parent.right
                rightMargin: 10
            }
            color:FluTheme.fontTertiaryColor
        }
        Row{
            id:item_layout_text
            spacing: 5
            anchors{
                bottom: item_avatar.bottom
                left: item_avatar.right
                leftMargin: 10
                right: item_session_unreadcount.left
                rightMargin: 10
            }
            clip: true
            height: 20
            Text{
                id:item_session_text
                color:FluTheme.fontTertiaryColor
                width: Math.min(implicitWidth,control_session.width-100)
                font.pixelSize: 13
                wrapMode: Text.NoWrap
                // padding: 0
                textFormat: Text.RichText
                verticalAlignment: Qt.AlignBottom
                text:EmoticonHelper.toEmoticonString(display.lastMsg, 13)
            }
        }
        Rectangle{
            id:item_session_unreadcount
            width: visible ? 18 : 0
            height: 18
            radius: 10
            visible: display.unreadCnt !== 0
            color:Qt.rgba(255/255,77/255,79/255,1)
            anchors{
                verticalCenter:item_layout_text.verticalCenter
                right: parent.right
                rightMargin: 10
            }
            Text{
                text: Math.min(display.unreadCnt,99)
                font.pixelSize: 10
                color: FluColors.White
                anchors.centerIn: parent
            }
        }
        MouseArea{
            id:mouse_area_session
            acceptedButtons: Qt.LeftButton
            anchors.fill: parent
            hoverEnabled: true
            onClicked:
                (event)=>{
                    if(event.button === Qt.LeftButton){
                        SettingsManager.saveLastSessionId(display.id)
                        SettingsManager.saveSessionSearchName("")
                        session_model.clearUnreadCnt(display.id)

                        if(textbox_search.isSearching){
                            textbox_search.isSearching = false
                            refreshSession()
                        }else{
                            control.currentSession = session_model.getSessionBySessionId(display.id)
                        }
                    }
                }
        }
    }
    Component{
        id:com_text_message
        Rectangle{
            width: item_message_content.contentWidth + 18
            height: item_message_content.contentHeight + 18
            radius: 4
            color: {
                if(SettingsManager.getAccount() === modelData.fromId.toString()){
                    return Qt.rgba(149/255,231/255,105/255,1)
                }
                return FluTheme.dark ? Qt.rgba(38/255,44/255,54/255,1) : Qt.rgba(253/255,253/255,253/255,1)
            }
            border.width: SettingsManager.getAccount() === modelData.fromId.toString() ? 0 : 1
            border.color: FluTheme.dark ? Window.active ? Qt.rgba(55/255,55/255,55/255,1):Qt.rgba(45/255,45/255,45/255,1) : Qt.rgba(226/255,229/255,234/255,1)
            FluText{
                id:item_message_content
                text: modelData.msg
                color: {
                    if(SettingsManager.getAccount() === modelData.fromId.toString()){
                        return FluColors.Black
                    }
                    return FluTheme.dark ? Qt.rgba(233/255,233/255,233/255,1) : FluColors.Black
                }
                textFormat: Text.RichText
                width: Math.min(implicitWidth,viewMessage.width/2+36)
                height: implicitHeight
                wrapMode: Text.WrapAnywhere
                x: 9
                y: 9
            }
        }
    }

    // 文本消息显示面板
    Component{
        id:com_message_panne
        Item{
            id:layout_message_panne
            MessageListModel{
                id:message_model
                session:currentSession
                onViewToBottom:{
                    listview_message.positionViewAtBeginning()
                }
                onViewToPosition:
                    (positon)=>{

                    }
                onLoadCompleted: {
                    rect_divider_bottom.y = Qt.binding(function(){
                        return layout_message_panne.height-150
                    })
                }
            }
            MessageListSortProxyModel{
                id:message_sort_model
                model:message_model
            }

            Item{
                id:layout_message_top_bar
                height: 60

                FluText{
                    text: currentSession.username
                    anchors{
                        top: parent.top
                        topMargin: 24
                        left: parent.left
                        leftMargin: 20
                    }
                    font.pixelSize: 18
                }
            }

            FluDivider{
                id:rect_divider_top
                height: 1
                width: parent.width
                orientation: Qt.Horizontal
                anchors.top: layout_message_top_bar.bottom
            }

            ListView{
                id:listview_message
                property int page: 1
                boundsBehavior: ListView.StopAtBounds
                clip: true
                // verticalLayoutDirection: ListView.BottomToTop
                // verticalCenter: ListView.BottomToTop
                model: message_sort_model
                anchors{
                    top: rect_divider_top.bottom
                    left: parent.left
                    right: parent.right
                }
                height:rect_divider_bottom.y - rect_divider_top.y
                Binding on height {
                    when: rect_divider_bottom.y - rect_divider_top.y > listview_message.contentHeight
                    value: listview_message.contentHeight
                }
                cacheBuffer: 500
                footer: Item{
                    height: visible ? 30 : 0
                    width: listview_message.width
                    visible: message_model.loadStatus === 0
                    RowLayout{
                        anchors.centerIn: parent
                        FluText{
                            text: "正在加载"
                            Layout.alignment: Qt.AlignVCenter
                        }
                        FluProgressRing{
                            Layout.preferredWidth: 20
                            Layout.preferredHeight: 20
                            Layout.alignment: Qt.AlignVCenter
                            strokeWidth:4
                        }
                    }
                }
                onContentYChanged: {
                    if(contentY == originY && contentY!==0){
                        page = page + 1;
                        message_model.loadData(page)
                    }
                }
                ScrollBar.vertical: FluScrollBar {}
                Component.onCompleted: {
                    message_model.loadData(page)
                }
                delegate: Column{
                    id:item_message_control
                    Item{
                        width: 1
                        height: 10
                    }
                    Rectangle{
                        width: item_message_time.width+8
                        height: item_message_time.height+8
                        radius: 3
                        color: FluTheme.dark ? Qt.rgba(255,255,255,0.1) : Qt.rgba(0,0,0,0.1)
                        anchors.horizontalCenter: parent.horizontalCenter
                        visible: {
                            if(index === listview_message.count-1){
                                return true
                            }
                            return checkTimestampDiff(message_model.data(message_model.index(Math.min(index+1,listview_message.count-1),0),0).sendTime,display.sendTime)
                        }
                        FluText {
                            id: item_message_time
                            text: display.sendTimeStr
                            color: Qt.rgba(255/255,255/255,255/255,1)
                            anchors.centerIn: parent
                        }
                    }
                    Item{
                        width: 1
                        height: 10
                    }
                    Row{
                        width: listview_message.width
                        layoutDirection: SettingsManager.getAccount() === display.fromId.toString() ? Qt.RightToLeft : Qt.LeftToRight
                        Item{
                            width: 30
                            height: 1
                        }
                        AvatarView{
                            width: 36
                            height: 36
                            uId: display.fromId
                            uisGroup: false
                        }
                        Item{
                            width: 12
                            height: 1
                        }
                        FluLoader{
                            property var modelData: display
                            property var viewMessage: listview_message
                            sourceComponent: com_text_message
                        }
                        Item{
                            width: 8
                            height: 1
                        }
                    }
                    Item{
                        width: 1
                        height: 10
                    }
                }
            }

            Component.onCompleted: {
                textbox_message_input.forceActiveFocus()
            }

            FluDivider{
                id:rect_divider_bottom
                height: 1
                orientation: Qt.Horizontal
                onYChanged: {
                    listview_message.positionViewAtBeginning()
                }
                MouseArea {
                    height: 6
                    width: parent.width
                    anchors.centerIn: parent
                    drag.target: rect_divider_bottom
                    drag.axis: Drag.YAxis
                    drag.minimumY: 80
                    // cursorShape: Qt.SizeVerCursor
                    drag.maximumY: layout_message_panne.height-130
                    onReleased: {
                        FluTools.restoreOverrideCursor()
                    }
                    onCanceled: {
                        FluTools.restoreOverrideCursor()
                    }
                    onPressed: {
                        FluTools.setOverrideCursor(Qt.SizeVerCursor)
                    }
                }
            }

            TextAreaDocument{
                id:text_area_doc
                document: textbox_message_input.textDocument
                cursorPosition: textbox_message_input.cursorPosition
                selectionStart: textbox_message_input.selectionStart
                selectionEnd: textbox_message_input.selectionEnd
                emoticonSize: 24
                onInsertTextChanged:
                    (text)=>{
                        textbox_message_input.insert(textbox_message_input.cursorPosition,EmoticonHelper.toEmoticonString(text))
                    }
            }

            Flickable{
                id:flickable_message_input
                anchors{
                    top: rect_divider_bottom.bottom
                    topMargin: 36
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                    bottomMargin: 40
                }
                clip: true
                flickableDirection: Flickable.VerticalFlick
                ScrollBar.vertical: FluScrollBar{}
                boundsBehavior: Flickable.StopAtBounds
                TextArea.flickable : FluMultilineTextBox{
                    id:textbox_message_input
                    padding: 0
                    leftPadding: 12
                    rightPadding: 12
                    width: parent.width
                    textFormat: Text.RichText
                    background: Item{}
                    onTextChanged: {
                        if (length > 2048) {
                            remove(2048, length)
                            return
                        }
                    }
                    Keys.onEnterPressed: (event)=> {
                        event.accepted = false
                    }
                    Keys.onReturnPressed:(event)=> {
                         event.accepted = true
                         var text =  text_area_doc.rawText()
                         if(text === ""){
                             showError("不能发送空白信息")
                             return
                         }

                         MessageManager.sendTextMessage(text,control.currentSession.fromId, control.currentSession.isGroup)
                         textbox_message_input.clear()
                    }
                }
            }

            RowLayout{
                anchors{
                    top: rect_divider_bottom.bottom
                    topMargin: 5
                    left: parent.left
                    leftMargin: 8
                }

                FluIconButton{
                    id:btn_emoji
                    verticalPadding: 0
                    horizontalPadding: 0
                    iconSize: 18
                    Layout.preferredWidth: 26
                    Layout.preferredHeight: 26
                    iconSource: FluentIcons.Emoji2
                    onClicked: {
                        var pos = mapToGlobal(0,0)
                        emoji_panel.x = pos.x - (emoji_panel.width-btn_emoji.width) / 2
                        emoji_panel.y = Math.max(pos.y - emoji_panel.height,0)
                        emoji_panel.show()
                    }
                }
            }

            FluFilledButton{
                text:"发送"
                width: 80
                anchors{
                    bottom: parent.bottom
                    right: parent.right
                    rightMargin: 12
                    bottomMargin: 12
                }
                onClicked:{
                    var text =  text_area_doc.rawText()
                    if(text === ""){
                        showError("不能发送空白信息")
                        return
                    }
                    MessageManager.sendTextMessage(text,control.currentSession.fromId, control.currentSession.isGroup);
                    textbox_message_input.clear()
                }
            }

            FluText{
                text:"%1/%2".arg(textbox_message_input.length).arg(2048)
                anchors{
                    left: parent.left
                    bottom: parent.bottom
                    bottomMargin: 5
                    leftMargin: 14
                }
                color:FluTheme.fontTertiaryColor
            }

            EmojiPanel{
                id:emoji_panel
                onEmojiClicked:
                    (tag)=>{
                        textbox_message_input.insert(textbox_message_input.cursorPosition,EmoticonHelper.toEmoticonString(tag))
                    }
            }
        }
    }

    // 左边的session会话一列
    Item{
        id:layout_session
        width: 250
        height: parent.height
        Item{
            id:layout_session_top_bar
            width: parent.width
            height: 60
            FluTextBox{
                id:textbox_search
                property bool isSearching: false
                width: 200
                iconSource: FluentIcons.Search
                placeholderText: "搜索会话"
                anchors{
                    bottom: parent.bottom
                    bottomMargin: 8
                    left: parent.left
                    leftMargin: 10
                }
                cleanEnabled: false
                rightPadding: 60
                FluIconButton{
                    iconSource: FluentIcons.Cancel
                    iconSize: 12
                    anchors{
                        right: parent.right
                        rightMargin: 30
                        verticalCenter: parent.verticalCenter
                    }
                    iconColor: FluTheme.dark ? Qt.rgba(222/255,222/255,222/255,1) : Qt.rgba(97/255,97/255,97/255,1)
                    width: 30
                    height: 20
                    verticalPadding: 0
                    horizontalPadding: 0
                    visible: textbox_search.activeFocus
                    onVisibleChanged: {
                        if(!visible){
                            textbox_search.clear()
                        }
                    }
                    onClicked:{
                        textbox_search.focus = false
                    }
                }
            }
            FluIconButton{
                width: 24
                height: 24
                iconSize: 14
                text:"创建群聊"
                verticalPadding: 0
                horizontalPadding: 0
                iconSource: FluentIcons.Add
                anchors{
                    bottom: parent.bottom
                    bottomMargin: 10
                    right: parent.right
                    rightMargin: 8
                }
                onClicked:{
                    popup_create_group.visible = true
                }

                FluPopup{
                    id: popup_create_group
                    width: 320
                    height: 220
                    modal: true
                    visible: false
                    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
                    onVisibleChanged: {
                        if(visible){
                            input_group_name.text = ""
                            input_group_desc.text = ""
                            input_group_name.forceActiveFocus()
                        }
                    }

                    contentItem: Item{
                        ColumnLayout{
                            spacing: 6
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.top: parent.top
                            anchors.topMargin: 12
                            FluText{
                                text: "创建群聊"
                            }

                            FluTextBox{
                                id: input_group_name
                                Layout.preferredWidth: 250
                                placeholderText:"群名"

                            }
                            FluMultilineTextBox{
                                id:input_group_desc
                                Layout.preferredWidth: 250
                                Layout.preferredHeight: 100
                                placeholderText:"描述信息"
                            }

                        }

                        RowLayout{
                            spacing: 16
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 12

                            FluFilledButton{
                                text: "创建群聊"
                                onClicked:{
                                    var group_name = input_group_name.text
                                    var group_desc = input_group_desc.text
                                    popup_create_group.visible = false
                                    MessageManager.sendCreateGroup(group_name,group_desc)
                                }
                            }
                            FluFilledButton{
                                text: "取消"
                                onClicked:{
                                    popup_create_group.visible = false
                                }
                            }
                        }
                    }
                }
            }
        }

        FluDivider{
            anchors.top: layout_session_top_bar.bottom
        }
        // 会话显示
        ListView{
            id:list_session
            anchors{
                left: parent.left
                right: parent.right
                top: layout_session_top_bar.bottom
                bottom: parent.bottom
            }
            header:Item{
                width:layout_session.width
                height: visible?50:0
                visible: list_session.count===0 && textbox_search.isSearching
                FluButton{
                    text:"未找到会话，点击返回"
                    anchors{
                        left: parent.left
                        right: parent.right
                        centerIn: parent
                    }
                    onClicked: {
                        SettingsManager.saveSessionSearchName("")
                        textbox_search.isSearching = false
                        refreshSession()
                    }
                }
            }
            boundsBehavior: ListView.StopAtBounds
            model: session_sort_model
            clip: true
            ScrollBar.vertical: FluScrollBar{}
            delegate: SessionItem{
            }
        }
    }
    FluDivider{
        width: 1
        orientation: Qt.Vertical
        anchors{
            top: parent.top
            bottom: parent.bottom
            left: layout_session.left
        }
    }
    FluDivider{
        width: 1
        orientation: Qt.Vertical
        anchors{
            top: parent.top
            bottom: parent.bottom
            left: layout_session.right
        }
    }
    FluLoader{
        id:loader_session
        anchors{
            left: layout_session.right
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
    }
    Connections{
        target: textbox_search
        function onCommit(text){
            textbox_search.isSearching = true
            console.log("search session:", text)
            SettingsManager.saveSessionSearchName(text)
            refreshSession()
        }
    }

    function checkTimestampDiff(timestamp1, timestamp2) {
        const time1 = timestamp1;
        const time2 = timestamp2;
        const diff = Math.abs(time1 - time2);
        const diffMinutes = diff / 60000;
        if (diffMinutes <= 5) {
            return false;
        } else {
            return true;
        }
    }

}
