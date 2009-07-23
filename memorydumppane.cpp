#include <QFontDialog>
#include "memorydumppane.h"
#include "ui_memorydumppane.h"
#include "sim.h"
#include "pep.h"
#include "enu.h"

#include <QTextCharFormat>
#include <QDebug>

MemoryDumpPane::MemoryDumpPane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::MemoryDumpPane)
{
    m_ui->setupUi(this);

    QObject::connect(m_ui->pepMemRefreshButton, SIGNAL(clicked()), this, SLOT(on_pepMemRefreshButton_clicked()));
}

MemoryDumpPane::~MemoryDumpPane()
{
    delete m_ui;
}

void MemoryDumpPane::refreshMemory()
{
    QStringList memoryDump;
    QString memoryDumpLine;
    QChar ch;
    for (int i = 0; i < 65536; i += 8) {
        memoryDumpLine = "";
        memoryDumpLine.append(QString("%1 | ").arg(i, 4, 16, QLatin1Char('0')).toUpper());
        for (int j = 0; j < 8; j++) {
            memoryDumpLine.append(QString("%1 ").arg(Sim::Mem[i + j], 2, 16, QLatin1Char('0')).toUpper());
        }
        memoryDumpLine.append("|");
        for (int j = 0; j < 8; j++) {
            ch = QChar(Sim::Mem[i + j]);
            if (ch.isPrint()) {
                memoryDumpLine.append(ch);
            } else {
                memoryDumpLine.append(".");
            }
        }
        memoryDump.append(memoryDumpLine);
    }
    m_ui->pepMemoryDumpTextEdit->setText(memoryDump.join("\n"));
}

void MemoryDumpPane::refreshMemoryLines(int firstByte, int lastByte)
{
    int vertScrollBarPosition = m_ui->pepMemoryDumpTextEdit->verticalScrollBar()->value();
    int horizScrollBarPosition = m_ui->pepMemoryDumpTextEdit->horizontalScrollBar()->value();

    QTextCursor cursor(m_ui->pepMemoryDumpTextEdit->document());
    cursor.setPosition(0);
    for (int i = 0; i < firstByte / 8; i++) {
        cursor.movePosition(QTextCursor::NextBlock);
    }

    QString memoryDumpLine;
    QChar ch;
    int byteNum = firstByte / 8;
    for (int i = firstByte / 8; i <= lastByte / 8; i++) {
        memoryDumpLine = "";
        memoryDumpLine.append(QString("%1 | ").arg(byteNum, 4, 16, QLatin1Char('0')).toUpper());
        for (int j = 0; j < 8; j++) {
            memoryDumpLine.append(QString("%1 ").arg(Sim::Mem[byteNum + j], 2, 16, QLatin1Char('0')).toUpper());
        }
        memoryDumpLine.append("|");
        for (int j = 0; j < 8; j++) {
            ch = QChar(Sim::Mem[byteNum + j]);
            if (ch.isPrint()) {
                memoryDumpLine.append(ch);
            } else {
                memoryDumpLine.append(".");
            }
        }
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        m_ui->pepMemoryDumpTextEdit->setTextCursor(cursor);
        m_ui->pepMemoryDumpTextEdit->insertPlainText(memoryDumpLine);
        cursor.movePosition(QTextCursor::NextBlock);
        byteNum += 8;
    }

    m_ui->pepMemoryDumpTextEdit->verticalScrollBar()->setValue(vertScrollBarPosition);
    m_ui->pepMemoryDumpTextEdit->horizontalScrollBar()->setValue(horizScrollBarPosition);
}

void MemoryDumpPane::highlightMemory(bool b)
{
    while (!highlightedInstruction.isEmpty()) {
        highlightByte(highlightedInstruction.takeFirst(), Qt::black, Qt::white);
    }
    while (!highlightedData.isEmpty()) {
        highlightByte(highlightedData.takeFirst(), Qt::black, Qt::white);
    }

    if (b) {
        highlightByte(Sim::stackPointer, Qt::white, Qt::darkCyan);
        highlightedInstruction.append(Sim::stackPointer);
        
        if (!Pep::isUnaryMap.value(Pep::decodeMnemonic.value(Sim::readByte(Sim::programCounter)))) {
            highlightByte(Sim::programCounter, Qt::white, Qt::blue);
            highlightedInstruction.append(Sim::programCounter);
            highlightByte(Sim::add(Sim::programCounter, 1), Qt::white, Qt::blue);
            highlightedInstruction.append(Sim::add(Sim::programCounter, 1));
            highlightByte(Sim::add(Sim::programCounter, 2), Qt::white, Qt::blue);
            highlightedInstruction.append(Sim::add(Sim::programCounter, 2));
        }
        else {
            highlightByte(Sim::programCounter, Qt::white, Qt::darkBlue);
            highlightedInstruction.append(Sim::programCounter);
        }

        while (!byteWritten.isEmpty()) {
            highlightByte(byteWritten.at(0), Qt::white, Qt::red);
            highlightedData.append(byteWritten.takeFirst());
        }
    }
}

void MemoryDumpPane::cacheModifiedBytes()
{
    modifiedBytes.unite(Sim::modifiedBytes);
}

void MemoryDumpPane::updateMemory()
{
    modifiedBytes.unite(Sim::modifiedBytes);
    QList<int> modifiedBytesList = modifiedBytes.toList();
    if (!modifiedBytesList.isEmpty()) {
        refreshMemoryLines(modifiedBytesList.first(), modifiedBytesList.last());
        for (int i = 0; i < modifiedBytesList.size(); i++) {
            byteWritten.append(modifiedBytesList.at(i));
        }
    }
    modifiedBytes.clear();
}

void MemoryDumpPane::highlightOnFocus()
{
    if (m_ui->pepMemoryDumpTextEdit->hasFocus()) {
        m_ui->pepMemoryDumpLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->pepMemoryDumpLabel->setAutoFillBackground(false);
    }
}

bool MemoryDumpPane::hasFocus()
{
    return m_ui->pepMemoryDumpTextEdit->hasFocus();
}

void MemoryDumpPane::undo()
{
    // does nothing with our current implementation
}

void MemoryDumpPane::redo()
{
    // does nothing with our current implementation
}

void MemoryDumpPane::cut()
{
    // read only pane, not allowed
}

void MemoryDumpPane::copy()
{
    m_ui->pepMemoryDumpTextEdit->copy();
}

void MemoryDumpPane::paste()
{
    // does nothing with our current implementation
}

void MemoryDumpPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(m_ui->pepMemoryDumpTextEdit->font()), this, "Set Memory Dump Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        m_ui->pepMemoryDumpTextEdit->setFont(font);
    }
}

void MemoryDumpPane::highlightByte(int memAddr, QColor foreground, QColor background)
{
    QTextCursor cursor(m_ui->pepMemoryDumpTextEdit->document());
    cursor.setPosition(0);
    for (int i = 0; i < memAddr / 8; i++) {
        cursor.movePosition(QTextCursor::NextBlock);
    }
    for (int i = 0; i < 7 + 3 * (memAddr % 8); i++) {
        cursor.movePosition(QTextCursor::NextCharacter);
    }
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
    QTextCharFormat format;
    format.setBackground(background);
    format.setForeground(foreground);
    cursor.mergeCharFormat(format);
}

void MemoryDumpPane::on_pepMemRefreshButton_clicked()
{
    int scrollBarPosition = m_ui->pepMemoryDumpTextEdit->verticalScrollBar()->value();
    refreshMemory();
    m_ui->pepMemoryDumpTextEdit->verticalScrollBar()->setValue(scrollBarPosition);
}




