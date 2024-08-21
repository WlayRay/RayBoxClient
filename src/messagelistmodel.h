#pragma once

#include <QSortFilterProxyModel>
#include "baselistmodel.h"
#include "message.h"
#include "messagemodel.h"
#include "sessionmodel.h"

class MessageListModel : public BaseListModel<MessageModel>
{
    Q_OBJECT
public:
    explicit MessageListModel(QObject *parent = nullptr);

    Q_INVOKABLE void loadData(int page);

    SessionModel *session() const;
    void setSession(SessionModel *newSession);

    int loadStatus() const;
    void setLoadStatus(int newLoadStatus);

signals:
    void viewToBottom();
    void viewToPosition(int position);
    void loadCompleted();

    void sessionChanged();
    void loadStatusChanged();

private:
    QSharedPointer<MessageModel> handleMessage(Message val);
    void addOrUpdateData(QSharedPointer<MessageModel> message);
    QString formatMessageTime(qint64 timestamp);

private:
    SessionModel *m_session;
    int m_loadStatus;
    Q_PROPERTY(SessionModel *session READ session WRITE setSession NOTIFY sessionChanged FINAL)
    Q_PROPERTY(int loadStatus READ loadStatus WRITE setLoadStatus NOTIFY loadStatusChanged FINAL)
};

class MessageListSortProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit MessageListSortProxyModel(QObject *parent = nullptr);

    MessageListModel *model() const;
    void setModel(MessageListModel *newModel);

signals:
    void modelChanged();

private:
    MessageListModel *m_model;
    Q_PROPERTY(MessageListModel *model READ model WRITE setModel NOTIFY modelChanged FINAL)

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};
