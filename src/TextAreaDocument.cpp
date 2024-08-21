#include "TextAreaDocument.h"

#include <QClipboard>
#include <QDebug>
#include <QGuiApplication>
#include <QMimeData>
#include <QPainter>
#include <QTextBlock>
#include <QTextCursor>
#include "EmoticonHelper.h"

TextAreaDocument::TextAreaDocument(QObject *parent)
    : QObject{parent}
{
    m_document = nullptr;
}

TextAreaDocument::~TextAreaDocument(){

}

QTextDocument* TextAreaDocument::textDocument() const
{
    if (m_document)
        return m_document->textDocument();
    else return nullptr;
}

QTextCursor TextAreaDocument::textCursor() const
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return QTextCursor();
    QTextCursor cursor = QTextCursor(doc);
    if (m_selectionStart != m_selectionEnd) {
        cursor.setPosition(m_selectionStart);
        cursor.setPosition(m_selectionEnd, QTextCursor::KeepAnchor);
    } else {
        cursor.setPosition(m_cursorPosition);
    }
    return cursor;
}

void TextAreaDocument::insertImage(const QString& url){
    QTextImageFormat format;
    format.setName(url);
    format.setWidth(20);
    format.setHeight(20);
    textCursor().insertImage(format, QTextFrameFormat::InFlow);
}

QString TextAreaDocument::rawText(){
    auto doc = textDocument();
    return toRawText(0,doc->characterCount()-1);
}

QString TextAreaDocument::toRawText(int start,int end){
    auto doc = textDocument();
    QString text;
    QTextBlock block = doc->begin();
    int index = 0;
    while (block.isValid()) {
        for (QTextBlock::iterator it = block.begin(); !it.atEnd(); ++it) {
            QTextFragment fragment = it.fragment();
            QTextCharFormat format = fragment.charFormat();
            if(format.isImageFormat()){
                QTextImageFormat imageFormat = format.toImageFormat();
                for (int var = 0; var < fragment.length(); var++) {
                    if(index>=start && index < end){
                        text.append(EmoticonHelper::getInstance()->getTagByUrl(imageFormat.name()));
                    }
                    index++;
                }
            }else{
                auto s = fragment.text();
                for (int var = 0; var < fragment.length(); var++) {
                    if(index>=start && index < end){
                        text.append(s.at(var));
                    }
                    index++;
                }
            }
        }
        if(doc->lastBlock() != block){
            text.append("\n");
        }
        block = block.next();
    }
    return text;
}

QQuickTextDocument *TextAreaDocument::document() const
{
    return m_document;
}

void TextAreaDocument::setDocument(QQuickTextDocument *newDocument)
{
    if (m_document == newDocument)
        return;
    m_document = newDocument;
    emit documentChanged();
}

int TextAreaDocument::getCursorPosition() const
{
    return m_cursorPosition;
}

void TextAreaDocument::setCursorPosition(int newCursorPosition)
{
    if (m_cursorPosition == newCursorPosition)
        return;
    m_cursorPosition = newCursorPosition;
    emit cursorPositionChanged();
}

int TextAreaDocument::getSelectionStart() const
{
    return m_selectionStart;
}

void TextAreaDocument::setSelectionStart(int newSelectionStart)
{
    if (m_selectionStart == newSelectionStart)
        return;
    m_selectionStart = newSelectionStart;
    emit selectionStartChanged();
}

int TextAreaDocument::getSelectionEnd() const
{
    return m_selectionEnd;
}

void TextAreaDocument::setSelectionEnd(int newSelectionEnd)
{
    if (m_selectionEnd == newSelectionEnd)
        return;
    m_selectionEnd = newSelectionEnd;
    emit selectionEndChanged();
}

int TextAreaDocument::getEmoticonSize() const
{
    return m_emoticonSize;
}

void TextAreaDocument::setEmoticonSize(int newEmoticonSize)
{
    if (m_emoticonSize == newEmoticonSize)
        return;
    m_emoticonSize = newEmoticonSize;
    emit emoticonSizeChanged();
}
