#pragma once

#include <QObject>

class SessionModel : public QObject
{
    Q_OBJECT
public:
    explicit SessionModel(QObject *parent = nullptr);

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

    QString lastMsgTimeStr() const;
    void setLastMsgTimeStr(const QString &newLastMsgTimeStr);

    int unreadCnt() const;
    void setUnreadCnt(int newUnreadCnt);

    QString draft() const;
    void setDraft(const QString &newDraft);

signals:

    void idChanged();

    void fromIdChanged();

    void isGroupChanged();

    void usernameChanged();

    void lastMsgChanged();

    void lastMsgTimeChanged();

    void lastMsgTimeStrChanged();

    void unreadCntChanged();

    void draftChanged();

private:
    long m_id;                //  主键
    long m_fromId;            //  会话来源 （用户id或群id）
    bool m_isGroup;           //  是否为群会话
    QString m_username;       //  会话名称（对方姓名或群聊名称）
    QString m_lastMsg;        //  最后一条消息
    qint64 m_lastMsgTime;     //  最后一条消息的发送时间
    QString m_lastMsgTimeStr; //  最后一条消息的发送时间字符串
    int m_unreadCnt;          //  未读消息数量
    QString m_draft;          //  草稿
    Q_PROPERTY(long id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(long fromId READ fromId WRITE setFromId NOTIFY fromIdChanged FINAL)
    Q_PROPERTY(bool isGroup READ isGroup WRITE setIsGroup NOTIFY isGroupChanged FINAL)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
    Q_PROPERTY(QString lastMsg READ lastMsg WRITE setLastMsg NOTIFY lastMsgChanged FINAL)
    Q_PROPERTY(
        qint64 lastMsgTime READ lastMsgTime WRITE setLastMsgTime NOTIFY lastMsgTimeChanged FINAL)
    Q_PROPERTY(QString lastMsgTimeStr READ lastMsgTimeStr WRITE setLastMsgTimeStr NOTIFY
                   lastMsgTimeStrChanged FINAL)
    Q_PROPERTY(int unreadCnt READ unreadCnt WRITE setUnreadCnt NOTIFY unreadCntChanged FINAL)
    Q_PROPERTY(QString draft READ draft WRITE setDraft NOTIFY draftChanged FINAL)
};
