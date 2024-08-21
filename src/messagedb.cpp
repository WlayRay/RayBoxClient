#include "messagedb.h"

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
void MessageDB::insertMessage(Message &msg)
{
    QSqlQuery query;
    QString sql = QString("insert into message(groupId, isGroup, fromId, msg, sendTime) "
                          "values(:groupId, :isGroup, :fromId, :msg, :sendTime)");
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "sql:" << sql;

    query.prepare(sql);
    query.bindValue(":groupId", QVariant::fromValue(msg.groupId()));
    query.bindValue(":isGroup", QVariant::fromValue(msg.isGroup()));
    query.bindValue(":fromId", QVariant::fromValue(msg.fromId()));
    query.bindValue(":msg", QVariant::fromValue(msg.msg()));
    query.bindValue(":sendTime", QVariant::fromValue(msg.sendTime()));
    if (query.exec()) {
        qint64 lastInsertedId = query.lastInsertId().toLongLong();
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Last inserted ID:" << lastInsertedId;
        msg.setId(lastInsertedId);
    } else {
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Failed to insert data:" << query.lastError().text();
    }
}

// 查询用户消息
QList<Message> MessageDB::queryAllMessageByUserId(long userId)
{
    QSqlQuery query;
    QString sql = QString("select * from message where groupId = %1 and isGroup = 0").arg(userId);
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "sql:" << sql;
    query.exec(sql);
    QList<Message> result;
    while (query.next()) {
        long id = query.value("id").toLongLong();
        long groupId = query.value("groupId").toLongLong();
        bool isGroup = query.value("isGroup").toBool();
        long fromId = query.value("fromId").toLongLong();
        QString msg = query.value("msg").toString();
        qint64 sendTime = query.value("sendTime").toULongLong();
        Message message(id, groupId, isGroup, fromId, msg, sendTime);
        result.append(message);
    }
    return result;
}

// 查询群消息
QList<Message> MessageDB::queryAllMessageByGroupId(long groupId)
{
    QSqlQuery query;
    QString sql = QString("select * from message where groupId = %1 and isGroup != 0").arg(groupId);
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "sql:" << sql;
    query.exec(sql);
    QList<Message> result;
    while (query.next()) {
        long id = query.value("id").toLongLong();
        long groupId = query.value("groupId").toLongLong();
        bool isGroup = query.value("isGroup").toBool();
        long fromId = query.value("fromId").toLongLong();
        QString msg = query.value("msg").toString();
        qint64 sendTime = query.value("sendTime").toULongLong();
        Message message(id, groupId, isGroup, fromId, msg, sendTime);
        result.append(message);
    }
    return result;
}

QList<Message> MessageDB::queryMessageByUserIdInPage(long userId, int page)
{
    QSqlQuery query;
    QString sql = QString("select * from message where groupId = %1 and isGroup = 0 order by "
                          "sendTime desc, id desc limit 30 offset %2")
                      .arg(userId)
                      .arg((page - 1) * 30);
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "sql:" << sql;
    query.exec(sql);
    QList<Message> result;
    while (query.next()) {
        long id = query.value("id").toLongLong();
        long groupId = query.value("groupId").toLongLong();
        bool isGroup = query.value("isGroup").toBool();
        long fromId = query.value("fromId").toLongLong();
        QString msg = query.value("msg").toString();
        qint64 sendTime = query.value("sendTime").toULongLong();
        Message message(id, groupId, isGroup, fromId, msg, sendTime);
        result.append(message);
    }
    return result;
}

QList<Message> MessageDB::queryMessageByGroupIdInPage(long groupId, int page)
{
    QSqlQuery query;
    QString sql = QString("select * from message where groupId = %1 and isGroup != 0 order by "
                          "sendTime desc, id desc limit 30 offset %2")
                      .arg(groupId)
                      .arg((page - 1) * 30);
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "sql:" << sql;
    query.exec(sql);
    QList<Message> result;
    while (query.next()) {
        long id = query.value("id").toLongLong();
        long groupId = query.value("groupId").toLongLong();
        bool isGroup = query.value("isGroup").toBool();
        long fromId = query.value("fromId").toLongLong();
        QString msg = query.value("msg").toString();
        qint64 sendTime = query.value("sendTime").toULongLong();
        Message message(id, groupId, isGroup, fromId, msg, sendTime);
        result.append(message);
    }
    return result;
}
