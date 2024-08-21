#include "messagelistmodel.h"
#include "EmoticonHelper.h"
#include "messagedb.h"
MessageListModel::MessageListModel(QObject *parent)
    : BaseListModel<MessageModel>(parent)
{
    m_loadStatus = 0;
    m_session = nullptr;
    //    connect()
}

void MessageListModel::loadData(int page)
{
    if (!m_session)
        return;
    if (m_loadStatus != 0) {
        return;
    }
    setLoadStatus(1);
    MessageDB mdb;
    QList<Message> list;
    QList<QSharedPointer<MessageModel>> data;
    if (m_session->isGroup()) {
        list = mdb.queryMessageByGroupIdInPage(m_session->fromId(), page);
    } else {
        list = mdb.queryMessageByUserIdInPage(m_session->fromId(), page);
    }
    for (const auto &i : list) {
        data.append(handleMessage(i));
    }
    beginInsertRows(QModelIndex(), _datas.count(), _datas.count() + data.count() - 1);
    _datas.append(data);
    endInsertRows();
    setLoadStatus(0);
    if (data.count() < 30) {
        setLoadStatus(2);
    }
    emit loadCompleted();
}

SessionModel *MessageListModel::session() const
{
    return m_session;
}

void MessageListModel::setSession(SessionModel *newSession)
{
    if (m_session == newSession)
        return;
    m_session = newSession;
    emit sessionChanged();
}

int MessageListModel::loadStatus() const
{
    return m_loadStatus;
}

void MessageListModel::setLoadStatus(int newLoadStatus)
{
    if (m_loadStatus == newLoadStatus)
        return;
    m_loadStatus = newLoadStatus;
    emit loadStatusChanged();
}

QSharedPointer<MessageModel> MessageListModel::handleMessage(Message val)
{
    QSharedPointer<MessageModel> model = QSharedPointer<MessageModel>(new MessageModel(this));
    model->setId(val.id());
    model->setGroupId(val.groupId());
    model->setIsGroup(val.isGroup());
    model->setFromId(val.fromId());
    model->setMsg(EmoticonHelper::getInstance()->toEmoticonString(val.msg()));
    model->setSendTime(val.sendTime());
    model->setSendTimeStr(formatMessageTime(val.sendTime()));
    return model;
}

QString MessageListModel::formatMessageTime(qint64 timestamp)
{
    QDateTime dateTime;
    dateTime.setMSecsSinceEpoch(timestamp);
    QDateTime currentTime = QDateTime::currentDateTime();
    qint64 days = dateTime.daysTo(currentTime);
    if (days == 0) {
        return dateTime.toString("hh:mm");
    } else if (days == 1) {
        return dateTime.toString("昨天　hh:mm");
    }
    {
        if (dateTime.date().year() == currentTime.date().year()) {
            return dateTime.toString("M月dd日　hh:mm");
        } else {
            return dateTime.toString("yyyy年M月dd日　hh:mm");
        }
    }
}

void MessageListModel::addOrUpdateData(QSharedPointer<MessageModel> message)
{
    // update
    for (int i = 0; i < _datas.size(); ++i) {
        auto item = _datas.at(i);
        if (item.get()->id() == message.get()->id()) {
            _datas[i] = message;
            emit dataChanged(this->index(i, 0), this->index(i, 0));
            return;
        }
    }
    // add if doesn't exist
    beginInsertRows(QModelIndex(), 0, 0);
    _datas.insert(0, message);
    endInsertRows();
    Q_EMIT viewToBottom();
}

MessageListModel *MessageListSortProxyModel::model() const
{
    return m_model;
}

void MessageListSortProxyModel::setModel(MessageListModel *newModel)
{
    if (m_model == newModel)
        return;
    m_model = newModel;
    emit modelChanged();
}

MessageListSortProxyModel::MessageListSortProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_model = nullptr;
    connect(this, &MessageListSortProxyModel::modelChanged, this, [=] {
        setSourceModel(this->model());
        sort(0, Qt::AscendingOrder);
    });
}

bool MessageListSortProxyModel::filterAcceptsRow(int source_row,
                                                 const QModelIndex &source_parent) const
{
    return true;
}

bool MessageListSortProxyModel::filterAcceptsColumn(int source_column,
                                                    const QModelIndex &source_parent) const
{
    return true;
}

bool MessageListSortProxyModel::lessThan(const QModelIndex &source_left,
                                         const QModelIndex &source_right) const
{
    QSharedPointer<MessageModel> left = m_model->_datas.at(source_left.row());
    QSharedPointer<MessageModel> right = m_model->_datas.at(source_right.row());
    return left.get()->sendTime() > right.get()->sendTime();
}
