#pragma once
#include <QDateTime>
#include <QString>

// 消息封装类
class Message
{
public:
    Message(long id,
            long groupId,
            bool isGroup,
            long fromId,
            const QString &msg,
            const qint64 &sendTime);

    long id() const;
    void setId(long newId);
    long groupId() const;
    void setGroupId(long newGroupId);
    long fromId() const;
    void setFromId(long newFromId);
    QString msg() const;
    void setMsg(const QString &newMsg);
    qint64 sendTime() const;
    void setSendTime(const qint64 &newSendTime);

    bool isGroup() const;
    void setIsGroup(bool newIsGroup);

private:
    long m_id;         // 群消息id
    long m_groupId;    // 所属的群
    bool m_isGroup;    // 是否为群消息
    long m_fromId;     // 哪个用户发送的消息
    QString m_msg;     // 群消息内容
    qint64 m_sendTime; // 发送时间
};

