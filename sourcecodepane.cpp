#include <QList>
#include <QStringList>
#include <QTextCursor>
#include "sourcecodepane.h"
#include "ui_sourcecodepane.h"
#include "code.h"

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
    QString errorString;
    QStringList sourceCodeList;
    Code *code;
    int byteCount = 0;
    bool dotEndDetected;
    QList<Code *> codeList;

    removeErrorMessages();
    Asm::listOfReferencedSymbols.clear();
    QString sourceCode = m_ui->pepSourceCodeTextEdit->toPlainText();
    sourceCodeList = sourceCode.split('\n');
    for (int i = 0; i < sourceCodeList.size(); i++) {
        sourceLine = sourceCodeList[i];
        if (!Asm::processSourceLine(sourceLine, code, errorString, byteCount, dotEndDetected)) {
            appendMessageInSourceCodePaneAt(i, errorString, Qt::red);
            return false;
        }
        if (dotEndDetected) {
            if (byteCount > 65535) {
                errorString = ";ERROR: Program to long. Object code will not fit into memory.";
                appendMessageInSourceCodePaneAt(i, errorString, Qt::red);
                return false;
            }
            qDebug() << "byteCount == " << byteCount;
            return true;
        }
    }
    errorString = ";ERROR: Missing .END sentinel.";
    appendMessageInSourceCodePaneAt(0, errorString, Qt::red);
    return false;
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
