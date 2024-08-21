#include "messagemodel.h"

MessageModel::MessageModel(QObject *parent)
    : QObject{parent}
{
    
}

long MessageModel::id() const
{
    return m_id;
}

void MessageModel::setId(long newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

long MessageModel::groupId() const
{
    return m_groupId;
}

void MessageModel::setGroupId(long newGroupId)
{
    if (m_groupId == newGroupId)
        return;
    m_groupId = newGroupId;
    emit groupIdChanged();
}

bool MessageModel::isGroup() const
{
    return m_isGroup;
}

void MessageModel::setIsGroup(bool newIsGroup)
{
    if (m_isGroup == newIsGroup)
        return;
    m_isGroup = newIsGroup;
    emit isGroupChanged();
}

long MessageModel::fromId() const
{
    return m_fromId;
}

void MessageModel::setFromId(long newFromId)
{
    if (m_fromId == newFromId)
        return;
    m_fromId = newFromId;
    emit fromIdChanged();
}

QString MessageModel::msg() const
{
    return m_msg;
}

void MessageModel::setMsg(const QString &newMsg)
{
    if (m_msg == newMsg)
        return;
    m_msg = newMsg;
    emit msgChanged();
}

qint64 MessageModel::sendTime() const
{
    return m_sendTime;
}

void MessageModel::setSendTime(qint64 newSendTime)
{
    if (m_sendTime == newSendTime)
        return;
    m_sendTime = newSendTime;
    emit sendTimeChanged();
}

QString MessageModel::sendTimeStr() const
{
    return m_sendTimeStr;
}

void MessageModel::setSendTimeStr(const QString &newSendTimeStr)
{
    if (m_sendTimeStr == newSendTimeStr)
        return;
    m_sendTimeStr = newSendTimeStr;
    emit sendTimeStrChanged();
}
