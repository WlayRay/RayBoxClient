#include "messagemanager.h"
#include <QDateTime>
#include "message.h"
#include "messagedb.h"
#include "sessiondb.h"
#include "settingsmanager.h"
#include "socketmanager.h"
#include <chrono>
#include <functional>
#include <thread>

MessageManager *MessageManager::getInstance()
{
    static MessageManager mm;
    return &mm;
}

MessageManager::MessageManager(QObject *parent)
    : QObject{parent}
{
    isConnected = true;

    connect(SocketManager::getInstance(), &SocketManager::connected, this, [this]() {
        setIsConnected(true);
    });

    connect(SocketManager::getInstance(), &SocketManager::serverUnavailable, this, [this]() {
        setIsConnected(false);
    });

    connect(SocketManager::getInstance(), &SocketManager::newMsgArrived, this, [=]() {
        SessionDB sessionDB;
        sessionDB.syncSession();
        emit newMsgArrived();
    });

    connect(SocketManager::getInstance(), &SocketManager::newCreatedGroup, this, [=](int sessionId) {
        emit newCreatedGroup(sessionId);
    });

    connect(SocketManager::getInstance(),
            &SocketManager::newSearchAccount,
            this,
            [=](bool isEmpty,
                const QString &id,
                const QString &name,
                bool isGroup,
                const QString &desc) { emit newSearchAccount(isEmpty, id, name, isGroup, desc); });

    connect(SocketManager::getInstance(),
            &SocketManager::newAddAccount,
            this,
            [=](int userId, bool isGroup) { emit newAddAccount(userId, isGroup); });
}

bool MessageManager::getIsConnected() const
{
    return isConnected;
}

void MessageManager::setIsConnected(bool newIsConnected)
{
    if (isConnected == newIsConnected)
        return;
    isConnected = newIsConnected;
    emit isConnectedChanged();
}

void MessageManager::sendTextMessage(const QString &msg, const QString &toId, bool isGroup)
{
    qint64 sessionId = SettingsManager::getInstance()->getLastSessionId().toLongLong();
    SessionDB sessionDB;
    Session session = sessionDB.getSessionById(sessionId);
    session.setUnreadCnt(0);
    sessionDB.updateSession(session);

    QString fromId = SettingsManager::getInstance()->getAccount().toString();
    QTcpSocket *socket = SocketManager::getInstance()->socket();
    if (socket == nullptr) {
        isConnected = false;
        emit isConnectedChanged();
        emit failedToSendNewMsg(toId, msg);
        return;
    }
    qint64 sendTime = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
    QString newMsg(msg);
    newMsg.replace("\n", "\\n");
    newMsg.replace("'", "\'");
    newMsg.replace("\"", "\\\"");
    QString msgToSend;
    if (isGroup) {
        // 群消息{"msgType":9,"FROMID":6,"GROUPID":8,"msg":"你好 8号","sendTime":1702556908737}
        msgToSend
            = QString("{\"msgType\":9,\"FROMID\":%1,\"GROUPID\":%2,\"msg\":\"%3\",\"sendTime\":%4}")
                  .arg(fromId)
                  .arg(toId)
                  .arg(newMsg)
                  .arg(sendTime);

    } else {
        // 用户消息 {"msgType":4,"FROMID":6,"TOID":7,"msg":"hello 7号","sendTime":1702556908737}
        msgToSend = QString(
                        "{\"msgType\":4,\"FROMID\":%1,\"TOID\":%2,\"msg\":\"%3\",\"sendTime\":%4}")
                        .arg(fromId)
                        .arg(toId)
                        .arg(newMsg)
                        .arg(sendTime);
    }
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "Data to send:" << msgToSend;
    qint64 bytesWritten = socket->write(msgToSend.toUtf8());
    socket->flush();
    if (bytesWritten == msgToSend.toUtf8().size()) {
        MessageDB messageDB;
        Message message(0, toId.toLong(), isGroup, fromId.toLong(), msg, sendTime);
        messageDB.insertMessage(message);
        SessionDB sessionDB;
        sessionDB.syncSession();
        emit newMsgSend();
    } else {
        isConnected = false;
        emit isConnectedChanged();
        emit failedToSendNewMsg(toId, msg);
    }
}

void MessageManager::sendCreateGroup(const QString &name, const QString &desc)
{
    // 创建群 {"msgType":12,"FROMID":6,"name":"cpp","description":"cpp group."}
    QString fromId = SettingsManager::getInstance()->getAccount().toString();
    QTcpSocket *socket = SocketManager::getInstance()->socket();
    if (socket == nullptr) {
        isConnected = false;
        emit isConnectedChanged();
        return;
    }
    QString msgToSend = QString(
                            "{\"msgType\":12,\"FROMID\":%1,\"name\":\"%2\",\"description\":\"%3\"}")
                            .arg(fromId)
                            .arg(name)
                            .arg(desc);
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "Data to send:" << msgToSend;
    socket->write(msgToSend.toUtf8());
    socket->flush();
    socket->waitForBytesWritten(1000);
}

void MessageManager::sendSearchAccount(const QString &keyword)
{
    // 搜索用户或者群聊 {"msgType":15,"keyword":"1"}
    QTcpSocket *socket = SocketManager::getInstance()->socket();
    if (socket == nullptr) {
        isConnected = false;
        emit isConnectedChanged();
        return;
    }
    QString msgToSend = QString("{\"msgType\":15,\"keyword\":\"%1\"}").arg(keyword);
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "Data to send:" << msgToSend;
    socket->write(msgToSend.toUtf8());
    socket->flush();
    socket->waitForBytesWritten(1000);
}

void MessageManager::sendAddAccount(const QString &userId, bool isGroup)
{
    QString fromId = SettingsManager::getInstance()->getAccount().toString();
    QTcpSocket *socket = SocketManager::getInstance()->socket();
    if (socket == nullptr) {
        isConnected = false;
        emit isConnectedChanged();
        return;
    }
    QString msgToSend;
    if (isGroup) {
        // 加入群聊 {"msgType":10,"FROMID":6,"GROUPID":7}
        msgToSend = QString("{\"msgType\":10,\"FROMID\":%1,\"GROUPID\":%2}").arg(fromId).arg(userId);
    } else {
        // 添加好友 {"msgType":6,"U1":1,"U2":2}
        msgToSend = QString("{\"msgType\":6,\"U1\":%1,\"U2\":%2}").arg(fromId).arg(userId);
    }

    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "Data to send:" << msgToSend;
    socket->write(msgToSend.toUtf8());
    socket->flush();
    socket->waitForBytesWritten(1000);
}
