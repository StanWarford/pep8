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
    int lineNum = 0;
    bool dotEndDetected = false;

    removeErrorMessages();
    Asm::listOfReferencedSymbols.clear();
    Pep::memAddrssToAssemblerListing.clear();
    Pep::symbolTable.clear();
    while (!codeList.isEmpty()) {
        delete codeList.takeFirst();
    }
    QString sourceCode = m_ui->pepSourceCodeTextEdit->toPlainText();
    sourceCodeList = sourceCode.split('\n');
    Pep::burnCount = 0;
    while (lineNum < sourceCodeList.size() && !dotEndDetected) {
        sourceLine = sourceCodeList[lineNum];
        if (!Asm::processSourceLine(sourceLine, lineNum, code, errorString, byteCount, dotEndDetected)) {
            appendMessageInSourceCodePaneAt(lineNum, errorString, Qt::red);
            return false;
        }
        codeList.append(code);
        lineNum++;
    }
    if (!dotEndDetected) {
        errorString = ";ERROR: Missing .END sentinel.";
        appendMessageInSourceCodePaneAt(0, errorString, Qt::red);
        return false;
    }
    if (byteCount > 65535) {
        errorString = ";ERROR: Object code size too large to fit into memory.";
        appendMessageInSourceCodePaneAt(0, errorString, Qt::red);
        return false;
    }
    for (int i = 0; i < Asm::listOfReferencedSymbols.length(); i++) {
        if (!Pep::symbolTable.contains(Asm::listOfReferencedSymbols[i])) {
            errorString = ";ERROR: Symbol " + Asm::listOfReferencedSymbols[i] + " is used but not defined.";
            appendMessageInSourceCodePaneAt(Asm::listOfReferencedSymbolLineNums[i], errorString, Qt::red);
            return false;
        }
    }
    return true;
}

QList<int> SourceCodePane::getObjectCode()
{
    objectCode.clear();
    for (int i = 0; i < codeList.size(); ++i) {
        codeList.at(i)->appendObjectCode(objectCode);
    }
    return objectCode;
}

QStringList SourceCodePane::getAssemblerListingList()
{
    assemblerListingList.clear();
    listingTraceList.clear();
    hasCheckBox.clear();
    assemblerListingList.append("-------------------------------------------------------------------------------");
    assemblerListingList.append("      Object");
    assemblerListingList.append("Addr  code   Symbol   Mnemon  Operand       Comment");
    assemblerListingList.append("-------------------------------------------------------------------------------");
    for (int i = 0; i < codeList.length(); i++) {
        codeList[i]->appendSourceLine(assemblerListingList, listingTraceList, hasCheckBox);
    }
    assemblerListingList.append("-------------------------------------------------------------------------------");
    if (Pep::symbolTable.size() > 0) {
        assemblerListingList.append("");
        assemblerListingList.append("");
        assemblerListingList.append("Symbol table");
        assemblerListingList.append("--------------------------------------");
        assemblerListingList.append("Symbol    Value        Symbol    Value");
        assemblerListingList.append("--------------------------------------");
        QMapIterator<QString, int> i(Pep::symbolTable);
        QString symbolTableLine = "";
        QString hexString;
        while (i.hasNext()) {
            i.next();
            hexString = QString("%1").arg(i.value(), 4, 16, QLatin1Char('0')).toUpper();
            if (symbolTableLine.length() == 0) {
                symbolTableLine = QString("%1%2").arg(i.key(), -10).arg(hexString, -13);
            }
            else {
                symbolTableLine.append(QString("%1%2").arg(i.key(), -10).arg(hexString, -4));
                assemblerListingList.append(symbolTableLine);
                symbolTableLine = "";
            }
        }
        if (symbolTableLine.length() > 0) {
            assemblerListingList.append(symbolTableLine);
        }
        assemblerListingList.append("--------------------------------------");
    }
    return assemblerListingList;
}

QStringList SourceCodePane::getListingTraceList()
{
    // Compute listingTraceList and hasCheckBox
    return listingTraceList;
}

QList<bool> SourceCodePane::getHasCheckBox()
{
    return hasCheckBox;
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

void SourceCodePane::setSourceCodePaneText(QString string) {
    m_ui->pepSourceCodeTextEdit->setText(string);
}
