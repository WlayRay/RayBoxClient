#pragma once
#include <QList>
#include "session.h"

class SessionDB
{
public:
    void insertSession(Session &session);
    Session getSessionById(long sessionId);
    bool sessionExist(long id, bool isGroup);
    Session getSessionByGroupId(long groupId);
    Session getSessionByUserId(long userId);
    void deleteSession(long sessionId);
    void updateSession(Session session);
    void syncSession();
    QList<Session> allSessions();
};
