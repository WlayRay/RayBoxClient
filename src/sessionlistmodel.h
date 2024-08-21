#pragma once
#include <QAbstractListModel>
#include <QObject>
#include <QSortFilterProxyModel>
#include "baselistmodel.h"
#include "session.h"
#include "sessionmodel.h"
#include <memory>

class SessionListModel : public BaseListModel<SessionModel>
{
    Q_OBJECT
public:
    explicit SessionListModel(QObject *parent = nullptr);
    Q_INVOKABLE void resetData();
    Q_INVOKABLE void clear();
    Q_INVOKABLE SessionModel *getSessionByUserId(long userId, bool isGroup);
    Q_INVOKABLE SessionModel *getSessionBySessionId(long sessionId);
    Q_INVOKABLE void clearUnreadCnt(long sessionId);

private:
    void addOrUpdateData(QSharedPointer<SessionModel> session);
    QSharedPointer<SessionModel> handleSession(Session val);
    int getIndexById(const QString &id);
    QString handleContent(int type, const QString &content);
    QString handleDraft(const QString &draft);
    QString formatSessiontime(qint64 timestamp);
};

class SessionListSortProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SessionListSortProxyModel(QSortFilterProxyModel *parent = nullptr);

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
    SessionListModel *getModel() const;
    void setModel(SessionListModel *newModel);

signals:
    void modelChanged();

private:
    SessionListModel *model;
    Q_PROPERTY(SessionListModel *model READ getModel WRITE setModel NOTIFY modelChanged FINAL)
};
