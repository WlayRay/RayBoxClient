#include "sessionmodel.h"

SessionModel::SessionModel(QObject *parent)
    : QObject{parent}
{
    
}

long SessionModel::id() const
{
    return m_id;
}

void SessionModel::setId(long newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

long SessionModel::fromId() const
{
    return m_fromId;
}

void SessionModel::setFromId(long newFromId)
{
    if (m_fromId == newFromId)
        return;
    m_fromId = newFromId;
    emit fromIdChanged();
}

bool SessionModel::isGroup() const
{
    return m_isGroup;
}

void SessionModel::setIsGroup(bool newIsGroup)
{
    if (m_isGroup == newIsGroup)
        return;
    m_isGroup = newIsGroup;
    emit isGroupChanged();
}

QString SessionModel::username() const
{
    return m_username;
}

void SessionModel::setUsername(const QString &newUsername)
{
    if (m_username == newUsername)
        return;
    m_username = newUsername;
    emit usernameChanged();
}

QString SessionModel::lastMsg() const
{
    return m_lastMsg;
}

void SessionModel::setLastMsg(const QString &newLastMsg)
{
    if (m_lastMsg == newLastMsg)
        return;
    m_lastMsg = newLastMsg;
    emit lastMsgChanged();
}

qint64 SessionModel::lastMsgTime() const
{
    return m_lastMsgTime;
}

void SessionModel::setLastMsgTime(qint64 newLastMsgTime)
{
    if (m_lastMsgTime == newLastMsgTime)
        return;
    m_lastMsgTime = newLastMsgTime;
    emit lastMsgTimeChanged();
}

QString SessionModel::lastMsgTimeStr() const
{
    return m_lastMsgTimeStr;
}

void SessionModel::setLastMsgTimeStr(const QString &newLastMsgTimeStr)
{
    if (m_lastMsgTimeStr == newLastMsgTimeStr)
        return;
    m_lastMsgTimeStr = newLastMsgTimeStr;
    emit lastMsgTimeStrChanged();
}

int SessionModel::unreadCnt() const
{
    return m_unreadCnt;
}

void SessionModel::setUnreadCnt(int newUnreadCnt)
{
    if (m_unreadCnt == newUnreadCnt)
        return;
    m_unreadCnt = newUnreadCnt;
    emit unreadCntChanged();
}

QString SessionModel::draft() const
{
    return m_draft;
}

void SessionModel::setDraft(const QString &newDraft)
{
    if (m_draft == newDraft)
        return;
    m_draft = newDraft;
    emit draftChanged();
}
