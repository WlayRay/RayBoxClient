#pragma once
#include <QList>
#include "contact.h"

class ContactDB
{
public:
    void insertContact(Contact contact);
    void insertContacts(QList<Contact> contacts);
    Contact getContact(long userId, bool isGroup);
    QList<Contact> allContacts();
    void clearContacts();
};

