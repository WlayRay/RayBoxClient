#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "json.h"
// 管理与服务器的socket连接，线程安全的单例
class SocketManager : public QObject
{
    Q_OBJECT
public:
    QTcpSocket *socket();
    ~SocketManager();
    void closeSocket();
    void quit();
    static SocketManager *getInstance();
    SocketManager(const SocketManager &) = delete;
    SocketManager(SocketManager &&) = delete;
signals:
    void serverUnavailable();
    void connected();
    void newMsgArrived();
    void newCreatedGroup(int sessionId);
    void newSearchAccount(
        bool isEmpty, const QString &id, const QString &name, bool isGroup, const QString &desc);
    void newAddAccount(int userId, bool isGroup);

private:
    bool connectToServer();
    void handleUserMsg(const nlohmann::json &js);
    void handleGroupMsg(const nlohmann::json &js);
    void handleNewCreatedGroup(const nlohmann::json &js);
    void handleContactSync(const nlohmann::json &js);
    void handleSearchAccount(const nlohmann::json &js);
    void handleAddAccount(const nlohmann::json &js, bool isGroup);
    explicit SocketManager(QObject *parent = nullptr);

private:
    QTcpSocket *m_socket;
    QTimer *m_retryTimer;
    bool m_isAppRunning;
};
