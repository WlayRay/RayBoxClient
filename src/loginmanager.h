#pragma once

#include <QObject>
#include <QString>
#include <QCryptographicHash>

// 发送用户登录注册请求
class LoginManager : public QObject
{
    Q_OBJECT

public:
    static LoginManager *getInstance();
    LoginManager(const LoginManager &) = delete;
    LoginManager(LoginManager &&) = delete;
signals:
    void loginSuccessful(int userId, QString password);
    void registerSuccessful(int userId);
    void logoutSuccessful();

    void alreadyLogin(int userId);
    void wrongUser(int userId);
    void registerFailed();
    void networkUnavailable();
public slots:
    // 发送用户登录请求
    void loginUser(const QString& username, const QString& password);
    // 发送用户注册请求
    void registerUser(const QString &username, const QString &password);
    // 退出登录请求
    void logout(const qint64 uId);

private:
    explicit LoginManager(QObject *parent = nullptr);
};
