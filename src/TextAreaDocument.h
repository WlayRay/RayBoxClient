#ifndef TEXTAREADOCUMENT_H
#define TEXTAREADOCUMENT_H

#include <QObject>
#include <QQuickTextDocument>

class TextAreaDocument : public QObject
{
    Q_OBJECT
public:
    explicit TextAreaDocument(QObject *parent = nullptr);
    ~TextAreaDocument();
    QTextDocument *textDocument() const;
    QTextCursor textCursor() const;
    Q_INVOKABLE void insertImage(const QString& url);
    Q_INVOKABLE QString rawText();
    Q_SIGNAL void insertTextChanged(QString text);
    QString toRawText(int start, int end);

    QQuickTextDocument *document() const;
    void setDocument(QQuickTextDocument *newDocument);

    int getCursorPosition() const;
    void setCursorPosition(int newCursorPosition);

    int getSelectionStart() const;
    void setSelectionStart(int newSelectionStart);

    int getSelectionEnd() const;
    void setSelectionEnd(int newSelectionEnd);

    int getEmoticonSize() const;
    void setEmoticonSize(int newEmoticonSize);

signals:
    void documentChanged();

    void cursorPositionChanged();

    void selectionStartChanged();

    void selectionEndChanged();

    void emoticonSizeChanged();

private:
    QQuickTextDocument *m_document;
    int m_cursorPosition;
    int m_selectionStart;
    int m_selectionEnd;
    int m_emoticonSize;
    Q_PROPERTY(
        QQuickTextDocument *document READ document WRITE setDocument NOTIFY documentChanged FINAL)
    Q_PROPERTY(int cursorPosition READ getCursorPosition WRITE setCursorPosition NOTIFY
                   cursorPositionChanged FINAL)
    Q_PROPERTY(int selectionStart READ getSelectionStart WRITE setSelectionStart NOTIFY
                   selectionStartChanged FINAL)
    Q_PROPERTY(int selectionEnd READ getSelectionEnd WRITE setSelectionEnd NOTIFY
                   selectionEndChanged FINAL)
    Q_PROPERTY(int emoticonSize READ getEmoticonSize WRITE setEmoticonSize NOTIFY
                   emoticonSizeChanged FINAL)
};

#endif // TEXTAREADOCUMENT_H
