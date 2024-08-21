#pragma once

#include <QObject>

class MessageModel : public QObject
{
    Q_OBJECT
public:
    explicit MessageModel(QObject *parent = nullptr);

    long id() const;
    void setId(long newId);

    long groupId() const;
    void setGroupId(long newGroupId);

    bool isGroup() const;
    void setIsGroup(bool newIsGroup);

    long fromId() const;
    void setFromId(long newFromId);

    QString msg() const;
    void setMsg(const QString &newMsg);

    qint64 sendTime() const;
    void setSendTime(qint64 newSendTime);

    QString sendTimeStr() const;
    void setSendTimeStr(const QString &newSendTimeStr);

signals:

    void idChanged();

    void groupIdChanged();

    void isGroupChanged();

    void fromIdChanged();

    void msgChanged();

    void sendTimeChanged();

    void sendTimeStrChanged();

private:
    long m_id;         // 群消息id
    long m_groupId;    // 所属的群
    bool m_isGroup;    // 是否为群消息
    long m_fromId;     // 哪个用户发送的消息
    QString m_msg;     // 群消息内容
    qint64 m_sendTime; // 发送时间
    QString m_sendTimeStr; // 发送时间显示字符串
    Q_PROPERTY(long id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(long groupId READ groupId WRITE setGroupId NOTIFY groupIdChanged FINAL)
    Q_PROPERTY(bool isGroup READ isGroup WRITE setIsGroup NOTIFY isGroupChanged FINAL)
    Q_PROPERTY(long fromId READ fromId WRITE setFromId NOTIFY fromIdChanged FINAL)
    Q_PROPERTY(QString msg READ msg WRITE setMsg NOTIFY msgChanged FINAL)
    Q_PROPERTY(qint64 sendTime READ sendTime WRITE setSendTime NOTIFY sendTimeChanged FINAL)
    Q_PROPERTY(
        QString sendTimeStr READ sendTimeStr WRITE setSendTimeStr NOTIFY sendTimeStrChanged FINAL)
};
