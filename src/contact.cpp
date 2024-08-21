#include "contact.h"

Contact::Contact(long id, long userId, bool isGroup, const QString &name)
    : m_id(id)
    , m_userId(userId)
    , m_isGroup(isGroup)
    , m_name(name)
{}

long Contact::id() const
{
    return m_id;
}

void Contact::setId(long newId)
{
    m_id = newId;
}

long Contact::getUserId() const
{
    return m_userId;
}

void Contact::setUserId(long newUserId)
{
    m_userId = newUserId;
}

bool Contact::getIsGroup() const
{
    return m_isGroup;
}

void Contact::setIsGroup(bool newIsGroup)
{
    m_isGroup = newIsGroup;
}

QString Contact::getName() const
{
    return m_name;
}

void Contact::setName(const QString &newName)
{
    m_name = newName;
}
