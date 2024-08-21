#include "sessiondb.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
void SessionDB::insertSession(Session &session)
{
    QString sql
        = "insert into session(fromId,isGroup,username,lastMsg,lastMsgTime,unreadCnt,draft) "
          "values(:fromId, :isGroup, :username, :lastMsg, :lastMsgTime, :unreadCnt, :draft)";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":fromId", QVariant::fromValue(session.fromId()));
    query.bindValue(":isGroup", QVariant::fromValue(session.isGroup()));
    query.bindValue(":username", QVariant::fromValue(session.username()));
    query.bindValue(":lastMsg", QVariant::fromValue(session.lastMsg()));
    query.bindValue(":lastMsgTime", QVariant::fromValue(session.lastMsgTime()));
    query.bindValue(":unreadCnt", QVariant::fromValue(session.unreadCnt()));
    query.bindValue(":draft", QVariant::fromValue(session.draft()));
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "sql:" << sql;

    if (query.exec()) {
        qint64 lastInsertedId = query.lastInsertId().toLongLong();
        session.setId(lastInsertedId);
    } else {
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Failed to insert data:" << query.lastError().text();
    }
}

Session SessionDB::getSessionById(long sessionId)
{
    QString sql = "select * from session where id = :sessionId";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":sessionId", QVariant::fromValue(sessionId));
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "sql:" << sql;
    query.exec();
    query.first();
    long id = query.value("id").toLongLong();
    long fromId = query.value("fromId").toLongLong();
    bool isGroup = query.value("isGroup").toBool();
    QString username = query.value("username").toString();
    QString lastMsg = query.value("lastMsg").toString();
    qint64 lastMsgTime = query.value("lastMsgTime").toULongLong();
    long unreadCnt = query.value("unreadCnt").toLongLong();
    QString draft = query.value("draft").toString();
    return Session(id, fromId, isGroup, username, lastMsg, lastMsgTime, unreadCnt, draft);
}

bool SessionDB::sessionExist(long id, bool isGroup)
{
    QString sql
        = "select count(*) as count from session where fromId =:groupId and isGroup = :isGroup";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":groupId", QVariant::fromValue(id));
    query.bindValue(":isGroup", isGroup ? 1 : 0);
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "sql:" << sql;
    query.exec();
    query.first();
    return query.value("count").toBool();
}

Session SessionDB::getSessionByGroupId(long groupId)
{
    QString sql = "select * from session where fromId =:groupId and isGroup = 1";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":groupId", QVariant::fromValue(groupId));
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "sql:" << sql;
    query.exec();
    query.first();
    long id = query.value("id").toLongLong();
    long fromId = query.value("fromId").toLongLong();
    bool isGroup = query.value("isGroup").toBool();
    QString username = query.value("username").toString();
    QString lastMsg = query.value("lastMsg").toString();
    qint64 lastMsgTime = query.value("lastMsgTime").toULongLong();
    long unreadCnt = query.value("unreadCnt").toLongLong();
    QString draft = query.value("draft").toString();
    return Session(id, fromId, isGroup, username, lastMsg, lastMsgTime, unreadCnt, draft);
}

Session SessionDB::getSessionByUserId(long userId)
{
    QString sql = "select * from session where fromId =:userId and isGroup = 0";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":userId", QVariant::fromValue(userId));
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "sql:" << sql;
    query.exec();
    query.first();
    long id = query.value("id").toLongLong();
    long fromId = query.value("fromId").toLongLong();
    bool isGroup = query.value("isGroup").toBool();
    QString username = query.value("username").toString();
    QString lastMsg = query.value("lastMsg").toString();
    qint64 lastMsgTime = query.value("lastMsgTime").toULongLong();
    long unreadCnt = query.value("unreadCnt").toLongLong();
    QString draft = query.value("draft").toString();
    return Session(id, fromId, isGroup, username, lastMsg, lastMsgTime, unreadCnt, draft);
}

void SessionDB::deleteSession(long sessionId)
{
    QString sql = "delete from session where id = :sessionId";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":sessionId", QVariant::fromValue(sessionId));
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "sql:" << sql;
    query.exec();
}

void SessionDB::updateSession(Session session)
{
    QString sql = "update session set username=:username, lastMsg=:lastMsg, "
                  "lastMsgTime=:lastMsgTime, unreadCnt=:unreadCnt, "
                  "draft = :draft where id = :id";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":username", QVariant::fromValue(session.username()));
    query.bindValue(":lastMsg", QVariant::fromValue(session.lastMsg()));
    query.bindValue(":lastMsgTime", QVariant::fromValue(session.lastMsgTime()));
    query.bindValue(":unreadCnt", QVariant::fromValue(session.unreadCnt()));
    query.bindValue(":draft", QVariant::fromValue(session.draft()));
    query.bindValue(":id", QVariant::fromValue(session.id()));
    query.exec();
}

void SessionDB::syncSession()
{
    QString sql = R"(
UPDATE session
SET lastMsg = (
    SELECT msg
    FROM message
    WHERE message.groupId = session.fromId and message.isGroup = session.isGroup
    ORDER BY sendTime DESC
    LIMIT 1
),
lastMsgTime = (
    SELECT sendTime
    FROM message
    WHERE message.groupId = session.fromId and message.isGroup = session.isGroup
    ORDER BY sendTime DESC
    LIMIT 1
)
)";
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "Syncing messages into session table.";
    QSqlQuery query;
    query.exec(sql);
}

QList<Session> SessionDB::allSessions()
{
    QSqlQuery query;
    QString sql = "select * from session order by lastMsgTime desc";
    query.exec(sql);
    QList<Session> result;
    while (query.next()) {
        long id = query.value("id").toLongLong();
        long fromId = query.value("fromId").toLongLong();
        bool isGroup = query.value("isGroup").toBool();
        QString username = query.value("username").toString();
        QString lastMsg = query.value("lastMsg").toString();
        qint64 lastMsgTime = query.value("lastMsgTime").toULongLong();
        long unreadCnt = query.value("unreadCnt").toLongLong();
        QString draft = query.value("draft").toString();
        result.append(
            Session(id, fromId, isGroup, username, lastMsg, lastMsgTime, unreadCnt, draft));
    }
    return result;
}
