#pragma once

#include <QObject>
class MessageManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(
        bool isConnected READ getIsConnected WRITE setIsConnected NOTIFY isConnectedChanged FINAL)
public:
    static MessageManager *getInstance();
    MessageManager(const MessageManager &) = delete;
    MessageManager(MessageManager &&) = delete;

    bool getIsConnected() const;
    void setIsConnected(bool newIsConnected);

    Q_INVOKABLE void sendTextMessage(const QString &msg, const QString &toId, bool isGroup);
    Q_INVOKABLE void sendCreateGroup(const QString &name, const QString &desc);
    Q_INVOKABLE void sendSearchAccount(const QString &keyword);
    Q_INVOKABLE void sendAddAccount(const QString &userId, bool isGroup);

signals:

    void isConnectedChanged();
    void newMsgSend();
    void newMsgArrived();
    void failedToSendNewMsg(const QString &toId, const QString &msg);
    void newCreatedGroup(int sessionId);
    void newSearchAccount(
        bool isEmpty, const QString &id, const QString &name, bool isGroup, const QString &desc);
    void newAddAccount(int userId, bool isGroup);

private:
    explicit MessageManager(QObject *parent = nullptr);

private:
    bool isConnected;
};
