#pragma once

#include <QApplication>
#include <QDataStream>
#include <QDebug>
#include <QLocalServer>
#include <QLocalSocket>
#include <QMainWindow>
#include <QSharedMemory>

class SingleInstanceChecker : public QObject
{
    Q_OBJECT

public:
    explicit SingleInstanceChecker(QObject *parent = nullptr);
signals:
    void activate();
private slots:
    void handleNewConnection();

private:
    QSharedMemory sharedMemory_;
    QLocalServer localServer_;
};
