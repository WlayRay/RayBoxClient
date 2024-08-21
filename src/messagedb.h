#pragma once
#include <QList>
#include "message.h"

class MessageDB
{
public:
    void insertMessage(Message &msg);
    QList<Message> queryAllMessageByUserId(long userId);
    QList<Message> queryAllMessageByGroupId(long groupId);

    QList<Message> queryMessageByUserIdInPage(long userId, int page);
    QList<Message> queryMessageByGroupIdInPage(long groupId, int page);
};

