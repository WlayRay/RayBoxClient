#include "message.h"

Message::Message(
    long id, long groupId, bool isGroup, long fromId, const QString &msg, const qint64 &sendTime)
    : m_id(id)
    , m_groupId(groupId)
    , m_isGroup(isGroup)
    , m_fromId(fromId)
    , m_msg(msg)
    , m_sendTime(sendTime)
{}

long Message::id() const
{
    return m_id;
}

void Message::setId(long newId)
{
    m_id = newId;
}

long Message::groupId() const
{
    return m_groupId;
}

void Message::setGroupId(long newGroupId)
{
    m_groupId = newGroupId;
}

long Message::fromId() const
{
    return m_fromId;
}

void Message::setFromId(long newFromId)
{
    m_fromId = newFromId;
}

QString Message::msg() const
{
    return m_msg;
}

void Message::setMsg(const QString &newMsg)
{
    m_msg = newMsg;
}

qint64 Message::sendTime() const
{
    return m_sendTime;
}

void Message::setSendTime(const qint64 &newSendTime)
{
    m_sendTime = newSendTime;
}

bool Message::isGroup() const
{
    return m_isGroup;
}

void Message::setIsGroup(bool newIsGroup)
{
    m_isGroup = newIsGroup;
}
