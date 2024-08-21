#include "contactmodel.h"

ContactModel::ContactModel(QObject *parent)
    : QObject{parent}
{
    
}

long ContactModel::id() const
{
    return m_id;
}

void ContactModel::setId(long newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

long ContactModel::userId() const
{
    return m_userId;
}

void ContactModel::setUserId(long newUserId)
{
    if (m_userId == newUserId)
        return;
    m_userId = newUserId;
    emit userIdChanged();
}

bool ContactModel::isGroup() const
{
    return m_isGroup;
}

void ContactModel::setIsGroup(bool newIsGroup)
{
    if (m_isGroup == newIsGroup)
        return;
    m_isGroup = newIsGroup;
    emit isGroupChanged();
}

QString ContactModel::name() const
{
    return m_name;
}

void ContactModel::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

bool ContactModel::isFriend() const
{
    return m_isFriend;
}

void ContactModel::setIsFriend(bool newIsFriend)
{
    if (m_isFriend == newIsFriend)
        return;
    m_isFriend = newIsFriend;
    emit isFriendChanged();
}

QString ContactModel::desc() const
{
    return m_desc;
}

void ContactModel::setDesc(const QString &newDesc)
{
    if (m_desc == newDesc)
        return;
    m_desc = newDesc;
    emit descChanged();
}
