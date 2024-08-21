#include "dbmanager.h"
#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <QtSql/QSqlQuery>
#include "settingsmanager.h"
DBManager *DBManager::getInstance()
{
    static DBManager dbm;
    return &dbm;
}

void DBManager::init()
{
    // 初始化数据库
    QString account = SettingsManager::getInstance()->getAccount().toString();
    QString dbFile = QString("raybox-%1.db").arg(account);
    QFileInfo fileInfo(dbFile);
    if (!fileInfo.exists()) {
        // 数据库文件不存在
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Database doesn't exist, initiating using default sql.";
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(dbFile);
        if (!m_db.open()) {
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                     << __BASE_FILE__ << __LINE__ << "Failed to connect to database.";
        }

        // 创建相应的数据表
        QSqlQuery query;
        bool success = query.exec(R"(
CREATE TABLE "contact" (
  "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "userId" INTEGER,
  "isGroup" integer,
  "name" TEXT
)
)");
        if (success) {
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                     << __BASE_FILE__ << __LINE__ << "Table contact created successfully.";
        }

        success = query.exec(R"(
CREATE TABLE "message" (
  "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "groupId" INTEGER,
  "isGroup" integer,
  "fromId" INTEGER,
  "msg" TEXT,
  "sendTime" integer
)
)");
        if (success) {
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                     << __BASE_FILE__ << __LINE__ << "Table message created successfully.";
        }

        success = query.exec(R"(
CREATE TABLE "session" (
  "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "fromId" INTEGER,
  "isGroup" integer,
  "username" TEXT,
  "lastMsg" TEXT,
  "lastMsgTime" integer,
  "unreadCnt" integer,
  "draft" TEXT
)
)");
        if (success) {
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                     << __BASE_FILE__ << __LINE__ << "Table session created successfully.";
        }
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Database initiated successfully.";
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(dbFile);
        if (!m_db.open()) {
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                     << __BASE_FILE__ << __LINE__ << "Failed to connect to database.";
        }
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Load database successfully.";
    }
}

DBManager::DBManager() {}
