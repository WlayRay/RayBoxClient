#include "loginmanager.h"
#include <QDateTime>
#include <QTcpSocket>
#include "dbmanager.h"
#include "json.h"
#include "sessiondb.h"
#include "settingsmanager.h"
#include "socketmanager.h"
#include <string>
LoginManager *LoginManager::getInstance()
{
    static LoginManager lm;
    return &lm;
}

LoginManager::LoginManager(QObject *parent)
    : QObject(parent)
{
    connect(SocketManager::getInstance(), &SocketManager::serverUnavailable, this, [this]() {
        emit networkUnavailable();
    });
}

void LoginManager::loginUser(const QString& username, const QString& password)
{
    // {"msgType":1, "USERID":6, "password":"6"}
    int userId = username.toInt();

    QCryptographicHash hash(QCryptographicHash::Sha224);
    hash.addData(password.toUtf8());
    QByteArray hashedPassword = hash.result();
    QString hashedPasswordString = QString(hashedPassword.toHex());

    QString message
        = QString("{\"msgType\":1, \"USERID\":%1, \"password\":\"%2\"}").arg(userId).arg(hashedPasswordString);
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "Data to send:" << message;
    QTcpSocket *socket = SocketManager::getInstance()->socket();
    if (socket == nullptr) {
        emit networkUnavailable();
        return;
    }
    socket->write(message.toUtf8());
    socket->flush();
    socket->waitForBytesWritten(1000);

    if (socket->waitForReadyRead(1000)) {
        QByteArray responseData = socket->readAll();
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Received data:" << responseData;

        std::string jsStr = responseData.toStdString();
        try
        {
            nlohmann::json js = nlohmann::json::parse(jsStr);
            int errnum = js["errno"].get<int>();
            int msgType = js["msgType"].get<int>();
            if (errnum == 301)
            {
                // 重复登录
                emit alreadyLogin(userId);
                return;
            }
            if (errnum == 302)
            {
                // 账号密码错误
                emit wrongUser(userId);
                return;
            }

            if (errnum == 200 && msgType == 200) {
                SettingsManager::getInstance()->saveLogin(username, password);

                // 加载数据库
                DBManager::getInstance()->init();
                SessionDB sessionDB;
                sessionDB.syncSession();

                emit loginSuccessful(userId, password);
                return;
            }
        }
        catch (nlohmann::json::exception e)
        {
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                     << __BASE_FILE__ << __LINE__ << "Exception captured, reason:" << e.what();
            emit networkUnavailable();
        }
    } else {
        emit networkUnavailable();
    }
}

void LoginManager::registerUser(const QString &username, const QString &password)
{
    // {"msgType":2, "name":"1", "password":"1"}

    QCryptographicHash hash(QCryptographicHash::Sha224);
    hash.addData(password.toUtf8());
    QByteArray hashedPassword = hash.result();
    QString hashedPasswordString = QString(hashedPassword.toHex());

    QString message = QString("{\"msgType\":2, \"name\":\"%1\", \"password\":\"%2\"}")
                          .arg(username)
                          .arg(hashedPasswordString);
    qDebug() << __FILE__ << ":" << __LINE__ << __FUNCTION__ << " | Data to send:" << message;
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "Data to send:" << message;
    QTcpSocket *socket = SocketManager::getInstance()->socket();
    if (socket == nullptr) {
        emit networkUnavailable();
        return;
    }
    socket->write(message.toUtf8());
    socket->flush();
    socket->waitForBytesWritten(1000);

    if (socket->waitForReadyRead(1000)) {
        QByteArray responseData = socket->readAll();
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Received data:" << responseData;

        std::string jsStr = responseData.toStdString();
        try
        {
            nlohmann::json js = nlohmann::json::parse(jsStr);
            int errnum = js["errno"].get<int>();
            int msgType = js["msgType"].get<int>();
            int userId = js["USERID"].get<int>();

            if (errnum == 200 && msgType == 200)
            {
                emit registerSuccessful(userId);
            }else{
                emit registerFailed();
            }
        }
        catch (nlohmann::json::exception e)
        {
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                     << __BASE_FILE__ << __LINE__ << "Exception captured, reason:" << e.what();
            emit networkUnavailable();
        }
    } else {
        emit networkUnavailable();
    }
}

void LoginManager::logout(const qint64 uId)
{
    // {"msgType":3, "USERID":7}
    QString message = QString("{\"msgType\":3, \"USERID\":%1}").arg(uId);
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "Data to send:" << message;
    QTcpSocket *socket = SocketManager::getInstance()->socket();
    if (socket == nullptr) {
        emit networkUnavailable();
        return;
    }
    socket->write(message.toUtf8());
    socket->flush();
    socket->waitForBytesWritten(1000);

    if (socket->waitForReadyRead(1000)) {
        QByteArray responseData = socket->readAll();
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Received data:" << responseData;
        std::string jsStr = responseData.toStdString();
        try {
            nlohmann::json js = nlohmann::json::parse(jsStr);
            int errnum = js["errno"].get<int>();
            int msgType = js["msgType"].get<int>();

            if (errnum == 200 && msgType == 200) {
                qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                         << __BASE_FILE__ << __LINE__ << "Logout from server successfully.";
                emit logoutSuccessful();
            }
        } catch (nlohmann::json::exception e) {
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                     << __BASE_FILE__ << __LINE__ << "Exception captured, reason:" << e.what();
            emit networkUnavailable();
        }
    } else {
        emit networkUnavailable();
    }
}
