#include "contactdb.h"
#include <QDateTime>
#include <QSqlQuery>
#include <QVariant>

void ContactDB::insertContact(Contact contact)
{
    QString sql = "insert into contact(userId, isGroup, name) values(:userId, :isGroup, :name)";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":userId", QVariant::fromValue(contact.getUserId()));
    query.bindValue(":isGroup", QVariant::fromValue(contact.getIsGroup()));
    query.bindValue(":name", QVariant::fromValue(contact.getName()));
    query.exec();
}

void ContactDB::insertContacts(QList<Contact> contacts)
{
    QString sql = "insert into contact(userId, isGroup, name) values(:userId, :isGroup, :name)";
    QSqlQuery query;
    query.prepare(sql);
    for (const auto &i : contacts) {
        query.bindValue(":userId", QVariant::fromValue(i.getUserId()));
        query.bindValue(":isGroup", QVariant::fromValue(i.getIsGroup()));
        query.bindValue(":name", QVariant::fromValue(i.getName()));
        query.exec();
    }
}

Contact ContactDB::getContact(long userId, bool isGroup)
{
    QString sql = "select * from contact where userId =:userId and isGroup = :isGroup";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":userId", QVariant::fromValue(userId));
    query.bindValue(":isGroup", isGroup ? 1 : 0);
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "sql:" << sql;
    query.exec();
    if (query.first()) {
        return Contact(query.value("id").toLongLong(),
                       query.value("userId").toLongLong(),
                       query.value("isGroup").toBool(),
                       query.value("name").toString());
    } else {
        return Contact(-1, -1, false, "invalid");
    }
}

QList<Contact> ContactDB::allContacts()
{
    QSqlQuery query;
    QString sql = QString("select * from contact");
    query.exec(sql);
    QList<Contact> result;
    while (query.next()) {
        long id = query.value("id").toLongLong();
        long userId = query.value("userId").toLongLong();
        bool isGroup = query.value("isGroup").toBool();
        QString name = query.value("name").toString();
        Contact contact(id, userId, isGroup, name);
        result.append(contact);
    }
    return result;
}

void ContactDB::clearContacts()
{
    QSqlQuery query;
    QString sql = QString("delete from contact");
    query.exec(sql);
}
