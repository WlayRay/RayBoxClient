#include "settingsmanager.h"
#include <QDataStream>
#include <QDebug>
#include <QFileInfo>
#include <QIODevice>
#include <QStandardPaths>
void SettingsManager::init(char *argv[])
{
    auto applicationPath = QString::fromStdString(argv[0]);
    const QFileInfo fileInfo(applicationPath);
    const QString iniFileName = fileInfo.completeBaseName() + ".ini";
    const QString iniFilePath = fileInfo.canonicalPath() + "/" + iniFileName;
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "Application configuration file path:" << iniFilePath;
    m_settings.reset(new QSettings(iniFilePath, QSettings::IniFormat));

    // 读取配置文件
    QSettings settings(":/res/server.ini", QSettings::IniFormat);
    QString ip = settings.value("ip").toString();
    int port = settings.value("port").toInt();
    // ip = "192.168.153.128";
    // port = 8003;
    if (getHost().toString().isEmpty()) {
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Initializing host from server.ini, ip = " << ip ;
        saveHost(ip);
    }
    saveHost(ip);

    if (getPort().toString().isEmpty() || getPort().toString() == "0") {
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Initializing port from server.ini, port = " << port;
        savePort(port);
    }
    savePort(port);

    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "Server addr in QSettings =>" << getHost() << ":" << getPort();
    ;
}

void SettingsManager::saveLastSessionId(const QVariant &sessionId)
{
    save("lastSessionId", sessionId);
}

QVariant SettingsManager::getLastSessionId()
{
    return get("lastSessionId");
}

SettingsManager *SettingsManager::getInstance()
{
    static SettingsManager sm;
    return &sm;
}

SettingsManager::~SettingsManager() = default;

SettingsManager::SettingsManager(QObject *parent)
    : QObject{parent}
{
}

void SettingsManager::save(const QString &key, QVariant val)
{
    QByteArray data = {};
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream.setVersion(QDataStream::Qt_6_2);
    stream << val;
    m_settings->setValue(key, data);
}

QVariant SettingsManager::get(const QString &key, QVariant defautVal)
{
    const QByteArray data = m_settings->value(key).toByteArray();
    if (data.isEmpty()) {
        return defautVal;
    }

    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_6_2);
    QVariant val;
    stream >> val;
    return val;
}
