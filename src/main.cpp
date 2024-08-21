#include <QGuiApplication>
#include <QIcon>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "EmoticonListModel.h"
#include "TextAreaDocument.h"
#include "contactlistmodel.h"
#include "loginmanager.h"
#include "messagelistmodel.h"
#include "messagemanager.h"
#include "sessionlistmodel.h"
#include "settingsmanager.h"
#include "singleinstancechecker.h"
#include "socketmanager.h"
int main(int argc, char *argv[])
{
    QGuiApplication::setOrganizationName("WLAYRAY");
    QGuiApplication::setOrganizationDomain("https://github.com/WlayRay/RayBoxClient");
    QGuiApplication::setApplicationName("RAYBOS CLIENT");
    SettingsManager::getInstance()->init(argv);
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/res/image/favicons/spongebob.ico"));

    SingleInstanceChecker instanceChecker;

    SettingsManager::getInstance()->saveState("offline");
    SettingsManager::getInstance()->saveSessionSearchName("");
    SettingsManager::getInstance()->saveContactSearchName("");

    // 在程序即将退出时，修改登录状态
    QObject::connect(&app, &QGuiApplication::aboutToQuit, []() {
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Quitting, save offline state to QSettings.";
        SettingsManager::getInstance()->saveState("offline");
        LoginManager::getInstance()->logout(
            SettingsManager::getInstance()->getAccount().toULongLong());
        SocketManager::getInstance()->quit();
        SocketManager::getInstance()->closeSocket();
    });

    QQmlApplicationEngine engine;

    //========================================
    // 测试数据库
    qDebug() << "时间戳： " << QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
    //========================================

    engine.rootContext()->setContextProperty("SettingsManager", SettingsManager::getInstance());
    engine.rootContext()->setContextProperty("MessageManager", MessageManager::getInstance());
    engine.rootContext()->setContextProperty("LoginManager", LoginManager::getInstance());
    engine.rootContext()->setContextProperty("EmoticonHelper", EmoticonHelper::getInstance());
    engine.rootContext()->setContextProperty("SingleInstanceChecker", &instanceChecker);

    qmlRegisterType<SessionListModel>("RAXBOX", 1, 0, "SessionListModel");
    qmlRegisterType<SessionListSortProxyModel>("RAXBOX", 1, 0, "SessionListSortProxyModel");
    qmlRegisterType<MessageListModel>("RAXBOX", 1, 0, "MessageListModel");
    qmlRegisterType<MessageListSortProxyModel>("RAXBOX", 1, 0, "MessageListSortProxyModel");
    qmlRegisterType<ContactListModel>("RAXBOX", 1, 0, "ContactListModel");
    qmlRegisterType<EmoticonListModel>("RAXBOX", 1, 0, "EmoticonListModel");
    qmlRegisterType<TextAreaDocument>("RAYBOX", 1, 0, "TextAreaDocument");

    //========================================
    // 此文件用于测试
    //    engine.load(QUrl("qrc:/res/qml/Test.qml"));
    //========================================

    const QUrl url(u"qrc:/res/qml/App.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    const int exec = QGuiApplication::exec();
    if (exec == 931) {
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    }
    return exec;
}
