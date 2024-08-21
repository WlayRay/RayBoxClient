#include "session.h"

Session::Session(long id,
                 long fromId,
                 bool isGroup,
                 const QString &username,
                 const QString &lastMsg,
                 const qint64 &lastMsgTime,
                 int unreadCnt,
                 const QString &draft)
    : m_id(id)
    , m_fromId(fromId)
    , m_isGroup(isGroup)
    , m_username(username)
    , m_lastMsg(lastMsg)
    , m_lastMsgTime(lastMsgTime)
    , m_unreadCnt(unreadCnt)
    , m_draft(draft)
{}

long Session::id() const
{
    return m_id;
}

void Session::setId(long newId)
{
    m_id = newId;
}

long Session::fromId() const
{
    return m_fromId;
}

void Session::setFromId(long newFromId)
{
    m_fromId = newFromId;
}

bool Session::isGroup() const
{
    return m_isGroup;
}

void Session::setIsGroup(bool newIsGroup)
{
    m_isGroup = newIsGroup;
}

QString Session::username() const
{
    return m_username;
}

void Session::setUsername(const QString &newUsername)
{
    m_username = newUsername;
}

QString Session::lastMsg() const
{
    return m_lastMsg;
}

void Session::setLastMsg(const QString &newLastMsg)
{
    m_lastMsg = newLastMsg;
}

qint64 Session::lastMsgTime() const
{
    return m_lastMsgTime;
}

void Session::setLastMsgTime(qint64 newLastMsgTime)
{
    m_lastMsgTime = newLastMsgTime;
}

int Session::unreadCnt() const
{
    return m_unreadCnt;
}

void Session::setUnreadCnt(int newUnreadCnt)
{
    m_unreadCnt = newUnreadCnt;
}

QString Session::draft() const
{
    return m_draft;
}

void Session::setDraft(const QString &newDraft)
{
    m_draft = newDraft;
}
