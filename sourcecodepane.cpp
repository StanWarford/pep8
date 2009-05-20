#include <QStringList>
#include <QTextCursor>
#include "sourcecodepane.h"
#include "ui_sourcecodepane.h"

#include <QDebug>

SourceCodePane::SourceCodePane(QWidget *parent) :
        QWidget(parent),
        m_ui(new Ui::SourceCodePane)
{
    m_ui->setupUi(this);
}

SourceCodePane::~SourceCodePane()
{
    delete m_ui;
}

bool SourceCodePane::assemble()
{
    QString sourceLine;
    Asm::ELexicalToken token;
    QString tokenString;

    removeErrorMessages();
    QString sourceCode = m_ui->pepSourceCodeTextEdit->toPlainText();
    assemblerListingList = sourceCode.split('\n');
    for (int i = 0; i < assemblerListingList.size(); i++) {
        sourceLine = assemblerListingList[i];
        do {
            Asm::getToken(sourceLine, token, tokenString);
            qDebug() << "Token == " << tokenString;
            if (token == Asm::LT_ERROR) {
                appendMessageInSourceCodePaneAt(i, tokenString, Qt::red);
                return false;
            }
        } while (token != Asm::LT_EMPTY);
        qDebug() << "---------------------------";
    }
    return false; // For now, while developing assembler.

    /*
    // UI test, if first letter on a line is 'y', insert text box in listing
    hasCheckBox.clear();
    for (int i = 0; i < assemblerListingList.size(); i++) {
        if (assemblerListingList[i].length() > 0 && assemblerListingList[i][0] == 'y') {
            hasCheckBox.append(true);
        }
        else {
            hasCheckBox.append(false);
        }
    }
    // Just some random numbers for now
    objectCode.clear();
    for (int i = 0; i < 40; i++) {
        objectCode << (10 * i) % 256;
    }

*/
}

QList<int> SourceCodePane::getObjectCode() { return objectCode; }
QStringList SourceCodePane::getAssemblerListingList() { return assemblerListingList; }
QStringList SourceCodePane::getListingTraceList() { return listingTraceList; }
QList<bool> SourceCodePane::getHasCheckBox() { return hasCheckBox; }

void SourceCodePane::appendMessageInSourceCodePaneAt(int lineNumber, QString message, Qt::GlobalColor color)
{
    QTextCursor cursor(m_ui->pepSourceCodeTextEdit->document());
    cursor.setPosition(0);
    for (int i = 0; i < lineNumber; i++) {
        cursor.movePosition(QTextCursor::NextBlock);
    }
    cursor.movePosition(QTextCursor::EndOfLine);
    m_ui->pepSourceCodeTextEdit->setTextCursor(cursor);
    m_ui->pepSourceCodeTextEdit->setTextColor(color);
    m_ui->pepSourceCodeTextEdit->textCursor().insertText(message);
    m_ui->pepSourceCodeTextEdit->setTextColor(Qt::black);
}

void SourceCodePane::removeErrorMessages()
{
    QTextCursor cursor(m_ui->pepSourceCodeTextEdit->document()->find(";ERROR:"));
    while (!cursor.isNull()) {
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        cursor = m_ui->pepSourceCodeTextEdit->document()->find(";ERROR:", cursor);
    }
    cursor = m_ui->pepSourceCodeTextEdit->document()->find(";WARNING:");
    while (!cursor.isNull()) {
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        cursor = m_ui->pepSourceCodeTextEdit->document()->find(";WARNING:", cursor);
    }
}
