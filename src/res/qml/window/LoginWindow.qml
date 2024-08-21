import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window
import FluentUI


FluWindow {
    id:window
    width: 300
    height: 380
    stayTop: true
    fixSize: true
    title: qsTr("RAYBOX CLIENT")

    // 0 登录页面; 1 注册页面; 2 设置页面
    property int pageIndex: 0

    // 登录页面
    component LoginPage:Page{
        background: Rectangle{
            color:window.backgroundColor
        }

        Connections{
            target: LoginManager
            function onLoginSuccessful(userId, password){
                // 登录成功 打开主窗口
                FluApp.navigate("/")
                window.close()
            }
            function onRegisterSuccessful(userId)
            {
                // 注册成功后 自动填充账号
                console.log("register successful, userId: " + userId)
                textbox_login_account.text = userId
                textbox_login_password.forceActiveFocus()
                window.pageIndex = 0
                loginErrorLabel.text = "注册成功，您的账号为 " + userId
                loginErrorLabel.visible = true
            }
            function onAlreadyLogin(userId){
                // 重复登录的用户
                console.log("user ", userId, " already login")
                loginErrorLabel.text = "该账号已登录"
                loginErrorLabel.visible = true
            }
            function onWrongUser(userId){
                // 账号或密码错误
                console.log("user ", userId, " wrong user")
                loginErrorLabel.text = "账号或密码错误"
                loginErrorLabel.visible = true
            }
            function onNetworkUnavailable(){
                loginErrorLabel.text = "无法连接到服务器"
                loginErrorLabel.visible = true
            }
        }

        Component.onCompleted: {
            textbox_login_account.forceActiveFocus()
            if(checkbox_login_rememberPassword.checked){
                textbox_login_password.text = SettingsManager.getPassword()
            }
        }

        ColumnLayout{
            anchors.centerIn: parent
            spacing: 0

            FluTextBox{
                id: textbox_login_account
                text: SettingsManager.getAccount()
                Layout.preferredWidth: 240
                placeholderText: "请输入账号"
                validator:  IntValidator {
                }
            }

            FluPasswordBox{
                id: textbox_login_password
                Layout.topMargin: 10
                Layout.preferredWidth: 240
                placeholderText: "请输入密码"
            }

            FluCheckBox{
                id: checkbox_login_rememberPassword
                Layout.topMargin: 10
                checked: SettingsManager.getKeepPassword()
                text: "记住密码"
            }

            FluFilledButton{
                text: "登录"
                Layout.topMargin: 30
                Layout.preferredWidth: 240
                Layout.preferredHeight: 34
                onClicked: {
                    var account = textbox_login_account.text
                    var password = textbox_login_password.text
                    if(checkbox_login_rememberPassword.checked) {
                        SettingsManager.saveKeepPassword(true)
                    }else{
                        SettingsManager.saveKeepPassword(false)
                    }

                    LoginManager.loginUser(account, password);
                }
            }
            FluButton{
                text: "注册"
                Layout.topMargin: 10
                Layout.preferredWidth: 240
                Layout.preferredHeight: 34
                onClicked: {
                    window.pageIndex = 1
                }
            }
            FluText {
                id: loginErrorLabel
                visible: false
                color: "red"
                text: ""
                Layout.fillWidth: true
                Layout.topMargin: 20
                Layout.preferredHeight: 20
            }
        }
        FluTextButton{
            text:"设置"
            anchors{ right: parent.right; rightMargin: 10; bottom: parent.bottom; bottomMargin: 5 }
            onClicked: { window.pageIndex = 2 }
        }
    }

    component RegisterPage:Page{
        background: Rectangle{
            color:window.backgroundColor
        }

        Connections{
            target: LoginManager

            function onNetworkUnavailable(){
                registerErrorLabel.text = "无法连接到服务器"
                registerErrorLabel.visible = true
            }
        }

        Component.onCompleted: {
            textbox_register_username.forceActiveFocus()
        }

        ColumnLayout{
            anchors.centerIn: parent
            spacing: 0

            FluTextBox{
                id: textbox_register_username
                Layout.preferredWidth: 240
                placeholderText: "请输入昵称"
            }

            FluPasswordBox{
                id: textbox_register_password
                Layout.topMargin: 20
                Layout.preferredWidth: 240
                placeholderText: "请输入密码"
            }

            FluFilledButton{
                text: "注册"
                Layout.topMargin: 30
                Layout.preferredWidth: 240
                Layout.preferredHeight: 34
                onClicked: {
                    var username = textbox_register_username.text
                    var password = textbox_register_password.text
                    LoginManager.registerUser(username, password);
                }
            }
            FluButton{
                text: "登录"
                Layout.topMargin: 10
                Layout.preferredWidth: 240
                Layout.preferredHeight: 34
                onClicked: {
                    window.pageIndex = 0
                }
            }
            Label {
                id: registerErrorLabel
                visible: false
                color: "red"
                text: ""
                Layout.fillWidth: true
                Layout.topMargin: 20
                Layout.preferredHeight: 20
            }
        }
        FluTextButton{
            text:"设置"
            anchors{ right: parent.right; rightMargin: 10; bottom: parent.bottom; bottomMargin: 5 }
            onClicked: { window.pageIndex = 2 }
        }
    }

    component SettingPage:Page{
        background: Rectangle{
            color:window.backgroundColor
        }
        ColumnLayout{
            anchors.centerIn: parent
            spacing: 20

            FluTextBox{
                id: textbox_server_host
                Layout.preferredWidth: 240
                placeholderText: "服务器IP地址"
                text: SettingsManager.getHost()
                onTextChanged: {
                    SettingsManager.saveHost(text)
                }
            }

            FluTextBox{
                id:textbox_server_port
                Layout.preferredWidth: 240
                placeholderText: "服务器端口号"
                text: SettingsManager.getPort()
                onTextChanged: {
                    SettingsManager.savePort(text)
                }
            }

        }
        FluTextButton{
            text:"返回登录"
            anchors{
                right: parent.right
                rightMargin: 10
                bottom: parent.bottom
                bottomMargin: 5
            }
            onClicked: {
                window.pageIndex = 0
            }
        }
    }

    SwipeView{
        id:swipe_view
        anchors.fill: parent
        interactive: false
        currentIndex:  window.pageIndex
        LoginPage{}
        RegisterPage{}
        SettingPage{}
    }
}
