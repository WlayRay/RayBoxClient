#pragma once

#include <QDateTime>
#include <QString>

// 会话封装类
class Session
{
public:
    Session(long id,
            long fromId,
            bool isGroup,
            const QString &username,
            const QString &lastMsg,
            const qint64 &lastMsgTime,
            int unreadCnt,
            const QString &draft);

    long id() const;
    void setId(long newId);

    long fromId() const;
    void setFromId(long newFromId);

    bool isGroup() const;
    void setIsGroup(bool newIsGroup);

    QString username() const;
    void setUsername(const QString &newUsername);

    QString lastMsg() const;
    void setLastMsg(const QString &newLastMsg);

    qint64 lastMsgTime() const;
    void setLastMsgTime(qint64 newLastMsgTime);

    int unreadCnt() const;
    void setUnreadCnt(int newUnreadCnt);

    QString draft() const;
    void setDraft(const QString &newDraft);

private:
    long m_id;            //  主键
    long m_fromId;        //  会话来源 （用户id或群id）
    bool m_isGroup;       //  是否为群会话
    QString m_username;   //  会话名称（对方姓名或群聊名称）
    QString m_lastMsg;    //  最后一条消息
    qint64 m_lastMsgTime; //  最后一条消息的发送事件
    int m_unreadCnt;      //  未读消息数量
    QString m_draft;      //  草稿
};
