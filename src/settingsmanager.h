#pragma once

#include <QDateTime>
#include <QObject>
#include <QSettings>
#include <QVariant>

// 应用程序设置管理
// 存储用户登录信息及其他常用设置项
class SettingsManager : public QObject
{
    Q_OBJECT
public:
    void init(char *argv[]);

    void saveAccount(const QVariant &account) { save("account", account); }
    Q_INVOKABLE QVariant getAccount() { return get("account"); }

    void savePassword(const QVariant &password) { save("password", password); }
    Q_INVOKABLE QVariant getPassword() { return get("password"); }

    void saveState(const QVariant &state) { save("state", state); }
    Q_INVOKABLE QVariant getState() { return get("state"); }

    Q_INVOKABLE void saveHost(const QVariant &host) { save("host", host); }
    Q_INVOKABLE QVariant getHost() { return get("host"); }

    Q_INVOKABLE void savePort(const QVariant &port) { save("port", port); }
    Q_INVOKABLE QVariant getPort() { return get("port"); }

    Q_INVOKABLE void saveKeepPassword(const QVariant &keepPassword)
    {
        save("keepPassword", keepPassword);
    }
    Q_INVOKABLE QVariant getKeepPassword() { return get("keepPassword"); }

    Q_INVOKABLE void saveLogin(const QString &account, const QString &password)
    {
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Saving login user to QSettings, account" << account << ", password"
                 << password;
        saveState("online");
        saveAccount(account);
        savePassword(password);
    }

    Q_INVOKABLE void saveLogout()
    {
        saveState("offline");
        savePassword("");
    }

    Q_INVOKABLE bool isLogin() { return getState().toString() == "online"; }

    Q_INVOKABLE void saveLastSessionId(const QVariant &sessionId);
    Q_INVOKABLE QVariant getLastSessionId();

    void saveLatestMsg(const QString &username, const QString &msg)
    {
        save("latestMsgUserName", username);
        save("latestMsg", msg);
    }
    Q_INVOKABLE QVariant getLatestMsgUserName() { return get("latestMsgUserName"); }
    Q_INVOKABLE QVariant getLatestMsg() { return get("latestMsg"); }

    Q_INVOKABLE void saveFocusState(const QVariant &focused) { save("focused", focused); }
    QVariant getFocusState() { return get("focused"); }

    Q_INVOKABLE void saveSessionSearchName(const QVariant &sessionSearchName)
    {
        save("sessionSearchName", sessionSearchName);
    }
    Q_INVOKABLE QVariant getSessionSearchName() { return get("sessionSearchName"); }

    Q_INVOKABLE void saveContactSearchName(const QVariant &contactSearchName)
    {
        save("contactSearchName", contactSearchName);
    }
    Q_INVOKABLE QVariant getContactSearchName() { return get("contactSearchName"); }

    static SettingsManager *getInstance();
    ~SettingsManager();
    SettingsManager(const SettingsManager &) = delete;
    SettingsManager(SettingsManager &&) = delete;

private:
    explicit SettingsManager(QObject *parent = nullptr);

public:
signals:

private:
    // 保存设置项
    void save(const QString &key, QVariant val);
    // 获取设置项
    QVariant get(const QString &key, QVariant defautVal = {});

private:
    QScopedPointer<QSettings> m_settings; // 设置项
};
