#pragma once
#include <QtSql/QSqlDatabase>

// 数据库连接管理类 单例
class DBManager
{
public:
    static DBManager *getInstance();
    void init();
    DBManager(const DBManager &) = delete;
    DBManager(DBManager &&) = delete;

private:
    DBManager();

private:
    QSqlDatabase m_db;
};

