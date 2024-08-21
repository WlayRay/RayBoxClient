#include "socketmanager.h"
#include "contactdb.h"
#include "json.h"
#include "messagedb.h"
#include "sessiondb.h"
#include "settingsmanager.h"
bool SocketManager::connectToServer()
{
    SettingsManager *sm = SettingsManager::getInstance();

    QString host = sm->getHost().toString();
    quint16 port = static_cast<quint16>(sm->getPort().toInt());
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "Connecting to " << host << port;
    m_socket->connectToHost(host, port);

    if (m_socket->waitForConnected(3000) && m_socket->isValid()) {
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Connection established successfully";
        emit connected();
        return true;
    } else {
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Server Unavailable:" << m_socket->errorString();
        emit serverUnavailable();
        return false;
    }
}

void SocketManager::handleUserMsg(const nlohmann::json &js)
{
    // 用户消息 {"msgType":4,"FROMID":6,"TOID":7,"msg":"hello 7号","sendTime":1702556908737}
    int FROMID = js["FROMID"].get<int>();
    std::string msg = js["msg"].get<std::string>();
    qint64 sendTime = js["sendTime"].get<uint64_t>();

    MessageDB messageDB;
    QString msgStr = QString::fromStdString(msg);
    Message message{0, FROMID, false, FROMID, msgStr, sendTime};
    messageDB.insertMessage(message);

    qint64 sessionId = SettingsManager::getInstance()->getLastSessionId().toLongLong();
    SessionDB sessionDB;
    Session session = sessionDB.getSessionById(sessionId);
    int fromIdDB = session.fromId();
    if (fromIdDB != FROMID) {
        // 非当前会话，找到消息所属会话
        if (sessionDB.sessionExist(FROMID, false)) {
            Session s = sessionDB.getSessionByUserId(FROMID);
            s.setUnreadCnt(s.unreadCnt() + 1);
            sessionDB.updateSession(s);
            SettingsManager::getInstance()->saveLatestMsg(s.username(), QString::fromStdString(msg));
        } else {
            // 该session不存在，新建session
            ContactDB contactDB;
            Contact contact = contactDB.getContact(FROMID, false);
            Session newSession(0,
                               FROMID,
                               false,
                               contact.id() == -1 ? QString::number(FROMID) : contact.getName(),
                               msgStr,
                               sendTime,
                               1,
                               QString(""));
            sessionDB.insertSession(newSession);
            SettingsManager::getInstance()->saveLatestMsg(contact.id() == -1
                                                              ? QString::number(FROMID)
                                                              : contact.getName(),
                                                          QString::fromStdString(msg));
        }
    } else {
        // 当前会话， 判断当前窗口是否为焦点窗口
        bool focused = SettingsManager::getInstance()->getFocusState().toBool();
        if (!focused) {
            session.setUnreadCnt(session.unreadCnt() + 1);
            sessionDB.updateSession(session);
        }
        SettingsManager::getInstance()->saveLatestMsg(session.username(),
                                                      QString::fromStdString(msg));
    }
    emit newMsgArrived();
}
void SocketManager::handleGroupMsg(const nlohmann::json &js)
{
    // 群消息{"msgType":9,"FROMID":6,"GROUPID":8,"msg":"你好 8号","sendTime":1702556908737}
    int FROMID = js["FROMID"].get<int>();
    int GROUPID = js["GROUPID"].get<int>();
    std::string msg = js["msg"].get<std::string>();
    qint64 sendTime = js["sendTime"].get<uint64_t>();

    MessageDB messageDB;
    QString msgStr = QString::fromStdString(msg);
    Message message{0, GROUPID, true, FROMID, msgStr, sendTime};
    messageDB.insertMessage(message);

    qint64 sessionId = SettingsManager::getInstance()->getLastSessionId().toLongLong();
    SessionDB sessionDB;
    Session session = sessionDB.getSessionById(sessionId);
    int fromIdDB = session.fromId();
    if (fromIdDB != GROUPID) {
        // 非当前会话，找到消息所属会话
        if (sessionDB.sessionExist(GROUPID, true)) {
            Session s = sessionDB.getSessionByGroupId(GROUPID);
            s.setUnreadCnt(s.unreadCnt() + 1);
            sessionDB.updateSession(s);
            SettingsManager::getInstance()->saveLatestMsg(s.username(), QString::fromStdString(msg));
        } else {
            // 该session不存在
            ContactDB contactDB;
            Contact contact = contactDB.getContact(GROUPID, true);
            Session newSession(0,
                               GROUPID,
                               true,
                               contact.id() == -1 ? QString::number(GROUPID) : contact.getName(),
                               msgStr,
                               sendTime,
                               1,
                               QString(""));
            sessionDB.insertSession(newSession);
            SettingsManager::getInstance()->saveLatestMsg(contact.id() == -1
                                                              ? QString::number(GROUPID)
                                                              : contact.getName(),
                                                          QString::fromStdString(msg));
        }

    } else {
        // 当前会话， 判断当前窗口是否为焦点窗口
        bool focused = SettingsManager::getInstance()->getFocusState().toBool();
        if (!focused) {
            session.setUnreadCnt(session.unreadCnt() + 1);
            sessionDB.updateSession(session);
        }
        SettingsManager::getInstance()->saveLatestMsg(session.username(),
                                                      QString::fromStdString(msg));
    }

    emit newMsgArrived();
}

void SocketManager::handleNewCreatedGroup(const nlohmann::json &js)
{
    // 创建群聊成功 {\"GROUPID\":2,\"errno\":200,\"msgType\":12}
    int groupId = js["GROUPID"].get<int>();
    std::string name = js["name"].get<std::string>();
    Session s(0,
              groupId,
              true,
              QString::fromStdString(name),
              "",
              QDateTime::currentDateTimeUtc().toMSecsSinceEpoch(),
              0,
              "");
    SessionDB sessionDB;
    sessionDB.insertSession(s);

    Contact contact(0, groupId, true, QString::fromStdString(name));
    ContactDB contactDB;
    contactDB.insertContact(contact);

    emit newCreatedGroup(s.id());
}

void SocketManager::handleContactSync(const nlohmann::json &js)
{
    ContactDB contactDB;
    contactDB.clearContacts();
    // 解析联系人，插入到数据库中
    std::vector<std::unordered_map<std::string, std::string>> friends
        = js["friends"].get<std::vector<std::unordered_map<std::string, std::string>>>();
    QList<Contact> contactList;
    for (auto &f : friends) {
        std::string id = f["id"];
        std::string name = f["name"];
        bool isGroup = "1" == f["isGroup"];
        contactList.append(Contact(0, std::stol(id), isGroup, QString::fromStdString(name)));
    }
    contactDB.insertContacts(contactList);
}

void SocketManager::handleSearchAccount(const nlohmann::json &js)
{
    std::vector<std::unordered_map<std::string, std::string>> results
        = js["results"].get<std::vector<std::unordered_map<std::string, std::string>>>();
    ContactDB contactDB;
    for (auto &i : results) {
        std::string id = i["id"];
        std::string name = i["name"];
        bool isGroup = "1" == i["isGroup"];
        std::string desc;
        if (isGroup)
            desc = i["desc"];
        Contact c = contactDB.getContact(std::stoi(id), isGroup);
        if (c.id() == -1) {
            emit newSearchAccount(false,
                                  QString::fromStdString(id),
                                  QString::fromStdString(name),
                                  isGroup,
                                  QString::fromStdString(desc));
            return;
        }
    }
    emit newSearchAccount(true, QString(""), QString(""), false, QString(""));
}

void SocketManager::handleAddAccount(const nlohmann::json &js, bool isGroup)
{
    int errnum = js["errno"].get<int>();
    if (errnum == 200) {
        if (isGroup) {
            // 加入群聊成功
            int groupId = js["groupId"].get<int>();
            std::string name = js["name"].get<std::string>();

            Session s(0,
                      groupId,
                      true,
                      QString::fromStdString(name),
                      "",
                      QDateTime::currentDateTimeUtc().toMSecsSinceEpoch(),
                      0,
                      "");
            SessionDB sessionDB;
            sessionDB.insertSession(s);

            Contact contact(0, groupId, true, QString::fromStdString(name));
            ContactDB contactDB;
            contactDB.insertContact(contact);

            emit newAddAccount(groupId, true);
        } else {
            // 添加好友成功
            int userId = js["userId"].get<int>();
            std::string name = js["name"].get<std::string>();
            bool flag = js.contains("new");
            if (flag) {
                // 仅添加联系人即可
                Contact contact(0, userId, false, QString::fromStdString(name));
                ContactDB contactDB;
                contactDB.insertContact(contact);
                return;
            }

            Session s(0,
                      userId,
                      false,
                      QString::fromStdString(name),
                      "",
                      QDateTime::currentDateTimeUtc().toMSecsSinceEpoch(),
                      0,
                      "");
            SessionDB sessionDB;
            sessionDB.insertSession(s);

            Contact contact(0, userId, false, QString::fromStdString(name));
            ContactDB contactDB;
            contactDB.insertContact(contact);

            emit newAddAccount(userId, false);
        }
    }
}

SocketManager::SocketManager(QObject *parent)
    : QObject(parent)
{
    m_socket = new QTcpSocket();
    m_isAppRunning = true;
    QTimer *t = new QTimer(this);
    connect(t, &QTimer::timeout, this, [=]() {
        m_socket->write(QString("keep this socket alive").toUtf8());
        m_socket->flush();
        m_socket->waitForBytesWritten(100);
    });
    t->start(5 * 60 * 1000);
    connect(m_socket, &QTcpSocket::connected, this, [=]() {
        // ==============stop qtimer=================
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Socket connected, deleting qtimer";
        if (m_retryTimer) {
            m_retryTimer->stop();
            m_retryTimer->deleteLater();
            m_retryTimer = nullptr;
        }

        // ====================relogin==============================
        if (SettingsManager::getInstance()->isLogin()) {
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                     << __BASE_FILE__ << __LINE__ << "Relogining transparently.";
            QString message = QString("{\"msgType\":1, \"USERID\":%1, \"password\":\"%2\"}")
                                  .arg(SettingsManager::getInstance()->getAccount().toInt())
                                  .arg(SettingsManager::getInstance()->getPassword().toString());
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                     << __BASE_FILE__ << __LINE__ << "Data to send:" << message;

            m_socket->write(message.toUtf8());
            m_socket->flush();
            m_socket->waitForBytesWritten(1000);
        }

        // ====================schedule for getting offline msg==================
        QTimer::singleShot(5000, this, [=]() {
            if (SettingsManager::getInstance()->isLogin()) {
                qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                         << __BASE_FILE__ << __LINE__ << "Getting offline message.";
                QString msgToSend = QString("{\"msgType\":14,\"FROMID\":%1}")
                                        .arg(SettingsManager::getInstance()->getAccount().toInt());
                qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                         << __BASE_FILE__ << __LINE__ << "Data to send:" << msgToSend;
                m_socket->write(msgToSend.toUtf8());
                m_socket->flush();
                m_socket->waitForBytesWritten(1000);
            }
        });

        // =======================schedule for getting contacts======================
        QTimer::singleShot(10000, this, [=]() {
            if (SettingsManager::getInstance()->isLogin()) {
                qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                         << __BASE_FILE__ << __LINE__ << "Getting contacts.";
                QString msgToSend = QString("{\"msgType\":8,\"FROMID\":%1}")
                                        .arg(SettingsManager::getInstance()->getAccount().toInt());
                qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                         << __BASE_FILE__ << __LINE__ << "Data to send:" << msgToSend;
                m_socket->write(msgToSend.toUtf8());
                m_socket->flush();
                m_socket->waitForBytesWritten(1000);
            }
        });
    });
    connect(m_socket, &QTcpSocket::disconnected, this, [=]() {
        if (m_isAppRunning && !m_retryTimer) {
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                     << __BASE_FILE__ << __LINE__
                     << "Socket disconnected, setup qtimer to retry connecting to the server";
            m_retryTimer = new QTimer();
            connect(m_retryTimer, &QTimer::timeout, this, [=]() { connectToServer(); });
            m_retryTimer->start(1000);
        }
    });
    connect(m_socket, &QTcpSocket::readyRead, this, [=]() {
        if (SettingsManager::getInstance()->isLogin()) {
            QByteArray responseData = m_socket->readAll();
            qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                     << __BASE_FILE__ << __LINE__ << "New data arrived:" << responseData;
            std::string jsStr = responseData.toStdString();
            try {
                nlohmann::json js = nlohmann::json::parse(jsStr);
                int msgType = js["msgType"].get<int>(); // 仅处理用户消息和群消息
                if (msgType == 9) {
                    handleGroupMsg(js);
                }
                if (msgType == 4) {
                    handleUserMsg(js);
                }
                if (msgType == 14) {
                    std::vector<std::string> msgs = js["msgs"].get<std::vector<std::string>>();
                    for (auto msgJsonStr : msgs) {
                        nlohmann::json js = nlohmann::json::parse(msgJsonStr);
                        int offlineMsgType = js["msgType"].get<int>();
                        if (offlineMsgType == 9) {
                            handleGroupMsg(js);
                        }
                        if (offlineMsgType == 4) {
                            handleUserMsg(js);
                        }
                    }
                }
                if (msgType == 12) {
                    handleNewCreatedGroup(js);
                }
                if (msgType == 8) {
                    handleContactSync(js);
                }
                if (msgType == 15) {
                    handleSearchAccount(js);
                }
                if (msgType == 10) {
                    // 加入群聊
                    handleAddAccount(js, true);
                }
                if (msgType == 6) {
                    // 添加好友
                    handleAddAccount(js, false);
                }
            } catch (nlohmann::json::exception e) {
                qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                         << __BASE_FILE__ << __LINE__ << "Exception captured, reason:" << e.what();
            }
        }
    });
}

QTcpSocket *SocketManager::socket()
{
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "Attempting to get socket, socket isOpen()" << m_socket->isOpen()
             << "isWritable()" << m_socket->isWritable();

    if (m_socket->state() == QAbstractSocket::ConnectedState)
        return m_socket;
    else {
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Socket is in unconnected state, connecting...";
        bool result = connectToServer();
        if (result)
            return m_socket;
        else
            return nullptr;
    }
}

SocketManager::~SocketManager()
{
    m_socket->disconnectFromHost();
}

void SocketManager::closeSocket()
{
    qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
             << __LINE__ << "Closing socket";
    m_socket->disconnectFromHost();
}

void SocketManager::quit()
{
    m_isAppRunning = false;
}

SocketManager *SocketManager::getInstance()
{
    static SocketManager sm;
    return &sm;
}
