#include "sessionlistmodel.h"
#include "contactdb.h"
#include "sessiondb.h"
#include "settingsmanager.h"
SessionListModel::SessionListModel(QObject *parent)
    : BaseListModel(parent)
{
}

void SessionListModel::addOrUpdateData(QSharedPointer<SessionModel> session)
{
    // update
    for (int i = 0; i < _datas.size(); ++i) {
        auto item = _datas.at(i);
        if (item.get()->id() == session.get()->id()) {
            _datas[i] = session;
            Q_EMIT dataChanged(this->index(i, 0), this->index(i, 0));
            return;
        }
    }
    // add if doesn't exist
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _datas.append(session);
    endInsertRows();
}

void SessionListModel::resetData()
{
    QString searchName = SettingsManager::getInstance()->getSessionSearchName().toString();
    qDebug() << "search session:" << searchName;

    SessionDB sessionDB;
    QList<Session> sessionList = sessionDB.allSessions();

    // filter session
    QList<Session> filteredSessions = sessionList;
    filteredSessions.erase(std::remove_if(filteredSessions.begin(),
                                          filteredSessions.end(),
                                          [searchName](const Session &session) {
                                              return !session.username().contains(searchName);
                                          }),
                           filteredSessions.end());

    QList<QSharedPointer<SessionModel>> data;
    for (const Session &i : filteredSessions) {
        data.append(handleSession(i));
    }

    beginResetModel();
    _datas.clear();
    _datas.append(data);
    endResetModel();
}

void SessionListModel::clear()
{
    beginResetModel();
    _datas.clear();
    endResetModel();
}

QString SessionListModel::formatSessiontime(qint64 timestamp)
{
    if (0 == timestamp)
        return "";
    QDateTime dateTime;
    dateTime.setMSecsSinceEpoch(timestamp);
    QDateTime currentTime = QDateTime::currentDateTime();
    qint64 days = dateTime.daysTo(currentTime);
    if (days == 0) {
        return dateTime.toString("hh:mm");
    } else if (days == 1) {
        return dateTime.toString("昨天");
    }
    {
        return dateTime.toString("yy/M/dd");
    }
}

SessionModel *SessionListModel::getSessionByUserId(long userId, bool isGroup)
{
    // 先判断模型中是否存在
    for (int i = 0; i < _datas.size(); ++i) {
        auto item = _datas.at(i);
        if (item->fromId() == userId && item->isGroup() == isGroup) {
            return item.get();
        }
    }
    // 再判断数据库中是否存在
    SessionDB sessionDB;
    if (sessionDB.sessionExist(userId, isGroup)) {
        Session sessionInDB = isGroup ? sessionDB.getSessionByGroupId(userId)
                                      : sessionDB.getSessionByUserId(userId);
        QSharedPointer<SessionModel> sessionModel = handleSession(sessionInDB);
        addOrUpdateData(sessionModel);
        return sessionModel.get();
    }

    // 都不存在，则新建session
    ContactDB contactDB;
    Contact contact = contactDB.getContact(userId, isGroup);
    Session session(0,
                    userId,
                    isGroup,
                    contact.getName(),
                    QString(""),
                    QDateTime::currentDateTimeUtc().toMSecsSinceEpoch(),
                    0,
                    "");
    sessionDB.insertSession(session);
    QSharedPointer<SessionModel> sessionModel = handleSession(session);
    addOrUpdateData(sessionModel);
    return sessionModel.get();
}

SessionModel *SessionListModel::getSessionBySessionId(long sessionId)
{
    for (int i = 0; i < _datas.size(); ++i) {
        auto item = _datas.at(i);
        if (item->id() == sessionId) {
            return item.get();
        }
    }
    return nullptr;
}

void SessionListModel::clearUnreadCnt(long sessionId)
{
    SessionDB sessionDB;
    Session session = sessionDB.getSessionById(sessionId);
    session.setUnreadCnt(0);
    sessionDB.updateSession(session);
    QSharedPointer<SessionModel> sm = handleSession(session);
    addOrUpdateData(sm);
}

QSharedPointer<SessionModel> SessionListModel::handleSession(Session val)
{
    QSharedPointer<SessionModel> model = QSharedPointer<SessionModel>(new SessionModel(this));
    model->setId(val.id());
    model->setFromId(val.fromId());
    model->setIsGroup(val.isGroup());
    model->setUsername(val.username());
    model->setLastMsg(val.lastMsg());
    model->setLastMsgTime(val.lastMsgTime());
    model->setLastMsgTimeStr(formatSessiontime(val.lastMsgTime()));
    model->setUnreadCnt(val.unreadCnt());
    model->setDraft(val.draft());
    return model;
}

SessionListSortProxyModel::SessionListSortProxyModel(QSortFilterProxyModel *parent)
    : QSortFilterProxyModel{parent}
{
    model = nullptr;
    connect(this, &SessionListSortProxyModel::modelChanged, this, [=] {
        setSourceModel(this->getModel());
        sort(0, Qt::AscendingOrder);
    });
}

bool SessionListSortProxyModel::filterAcceptsRow(int source_row,
                                                 const QModelIndex &source_parent) const
{
    return true;
}

bool SessionListSortProxyModel::filterAcceptsColumn(int source_column,
                                                    const QModelIndex &source_parent) const
{
    return true;
}

bool SessionListSortProxyModel::lessThan(const QModelIndex &source_left,
                                         const QModelIndex &source_right) const
{
    QSharedPointer<SessionModel> left = model->_datas.at(source_left.row());
    QSharedPointer<SessionModel> right = model->_datas.at(source_right.row());

    return left.get()->lastMsgTime() > right.get()->lastMsgTime();
}

SessionListModel *SessionListSortProxyModel::getModel() const
{
    return model;
}

void SessionListSortProxyModel::setModel(SessionListModel *newModel)
{
    if (model == newModel)
        return;
    model = newModel;
    emit modelChanged();
}
