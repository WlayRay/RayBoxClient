#include "contactlistmodel.h"
#include "contactdb.h"
#include "settingsmanager.h"
#include "socketmanager.h"
ContactListModel::ContactListModel(QObject *parent)
    : BaseListModel{parent}
{
    
}

QSharedPointer<ContactModel> ContactListModel::handleContact(const Contact &contact)
{
    QSharedPointer<ContactModel> contactModel = QSharedPointer<ContactModel>(new ContactModel());
    contactModel->setId(contact.id());
    contactModel->setUserId(contact.getUserId());
    contactModel->setIsGroup(contact.getIsGroup());
    contactModel->setName(contact.getName());
    return contactModel;
}

void ContactListModel::resetData()
{
    // 发送socket请求联系人 待后续处理
    QTcpSocket *socket = SocketManager::getInstance()->socket();
    if (socket) {
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Getting contacts.";
        QString msgToSend = QString("{\"msgType\":8,\"FROMID\":%1}")
                                .arg(SettingsManager::getInstance()->getAccount().toInt());
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << __BASE_FILE__
                 << __LINE__ << "Data to send:" << msgToSend;
        socket->write(msgToSend.toUtf8());
        socket->flush();
        socket->waitForBytesWritten(1000);
    }

    QString contactName = SettingsManager::getInstance()->getContactSearchName().toString();
    qDebug() << "search contact:" << contactName;

    ContactDB contactDB;
    QList<Contact> contactList = contactDB.allContacts();

    // filter contact
    QList<Contact> filteredContacts = contactList;
    filteredContacts.erase(std::remove_if(filteredContacts.begin(),
                                          filteredContacts.end(),
                                          [contactName](const Contact &contact) {
                                              return !contact.getName().contains(contactName);
                                          }),
                           filteredContacts.end());

    QList<QSharedPointer<ContactModel>> data;
    for (const Contact &con : filteredContacts) {
        data.append(handleContact(con));
    }

    beginResetModel();
    _datas.clear();
    _datas.append(data);
    endResetModel();
}

void ContactListModel::clear()
{
    beginResetModel();
    _datas.clear();
    endResetModel();
}
