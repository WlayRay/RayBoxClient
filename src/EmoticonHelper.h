#pragma once
#include <QObject>
#include <QRegularExpression>
#include <QSharedPointer>
#include <QTextDocument>

class Emoticon : public QObject
{
    Q_OBJECT
public:
    explicit Emoticon(QObject *parent = nullptr);
    Emoticon(QString file, QString name, QString tag, QObject *parent = nullptr);

    QString file() const;
    void setFile(const QString &newFile);

    QString name() const;
    void setName(const QString &newName);

    QString tag() const;
    void setTag(const QString &newTag);

signals:
    void fileChanged();

    void nameChanged();

    void tagChanged();

private:
    QString m_file;
    QString m_name;
    QString m_tag;
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString tag READ tag WRITE setTag NOTIFY tagChanged FINAL)
};

class EmoticonHelper : public QObject
{
    Q_OBJECT
public:
    static EmoticonHelper *getInstance();
    EmoticonHelper(const EmoticonHelper &) = delete;
    EmoticonHelper(EmoticonHelper &&) = delete;

private:
    explicit EmoticonHelper(QObject *parent = nullptr);

public:
    QList<QSharedPointer<Emoticon>> _datas;
    QString getFileByTag(const QString& tag);
    QString getTagByFile(const QString& file);
    QString getTagByUrl(QString url);
    void addResource(QTextDocument* textDocument);
    Q_INVOKABLE QString toEmoticonString(QString text,int size = 20);
public:
    QRegularExpression  _tagRegular;
    QString _prefix = "qrc:/res/image/emoji/";
    QString _prefix_emoji = "emoji:/";
};
