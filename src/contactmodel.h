#pragma once

#include <QObject>

class ContactModel : public QObject
{
    Q_OBJECT
public:
    explicit ContactModel(QObject *parent = nullptr);

    long id() const;
    void setId(long newId);

    long userId() const;
    void setUserId(long newUserId);

    bool isGroup() const;
    void setIsGroup(bool newIsGroup);

    QString name() const;
    void setName(const QString &newName);

    bool isFriend() const;
    void setIsFriend(bool newIsFriend);

    QString desc() const;
    void setDesc(const QString &newDesc);

signals:

    void idChanged();

    void userIdChanged();

    void isGroupChanged();

    void nameChanged();

    void isFriendChanged();

    void descChanged();

private:
    long m_id;      // 主键
    long m_userId;  // 群id或者好友id
    bool m_isGroup; // 是否为群
    QString m_name; // 群名字或好友名字

    // 额外添加字段
    bool m_isFriend; // 是否为好友关系 或者 是否在群中
    QString m_desc;  // 群描述
    Q_PROPERTY(long id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(long userId READ userId WRITE setUserId NOTIFY userIdChanged FINAL)
    Q_PROPERTY(bool isGroup READ isGroup WRITE setIsGroup NOTIFY isGroupChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(bool isFriend READ isFriend WRITE setIsFriend NOTIFY isFriendChanged FINAL)
    Q_PROPERTY(QString desc READ desc WRITE setDesc NOTIFY descChanged FINAL)
};
