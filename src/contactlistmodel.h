#pragma once

#include <QAbstractListModel>
#include <QObject>
#include "baselistmodel.h"
#include "contact.h"
#include "contactmodel.h"

class ContactListModel : public BaseListModel<ContactModel>
{
    Q_OBJECT
public:
    explicit ContactListModel(QObject *parent = nullptr);

    Q_INVOKABLE void resetData();
    Q_INVOKABLE void clear();

private:
    QSharedPointer<ContactModel> handleContact(const Contact &contact);
};

