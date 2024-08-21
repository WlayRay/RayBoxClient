#pragma once
#include <QString>

class Contact
{
public:
    Contact(long id, long userId, bool isGroup, const QString &name);

    long id() const;
    void setId(long newId);
    long getUserId() const;
    void setUserId(long newUserId);
    bool getIsGroup() const;
    void setIsGroup(bool newIsGroup);
    QString getName() const;
    void setName(const QString &newName);

private:
    long m_id;      // 主键
    long m_userId;  // 群id或者好友id
    bool m_isGroup; // 是否为群
    QString m_name; // 群名字或好友名字
};

