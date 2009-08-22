// File: memorydumppane.cpp
/*
    Pep8-1 is a virtual machine for writing machine language and assembly
    language programs.
    
    Copyright (C) 2009  J. Stanley Warford, Pepperdine University

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QFontDialog>
#include <QTextCharFormat>
#include <QAbstractTextDocumentLayout>
#include "memorydumppane.h"
#include "ui_memorydumppane.h"
#include "sim.h"
#include "pep.h"
#include "enu.h"

#include <QDebug>

MemoryDumpPane::MemoryDumpPane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::MemoryDumpPane)
{
    m_ui->setupUi(this);

    m_ui->pepMemRefreshButton->hide();
    m_ui->refreshBlock->setParent(0);
    // We no longer need the 'refresh' button, but we'll leave it there for debugging purposes.
    // Comment the previous two lines to show the refresh button.

    QObject::connect(m_ui->pepMemRefreshButton, SIGNAL(clicked()), this, SLOT(on_pepMemRefreshButton_clicked()));

    m_ui->pepMemoryDumpLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
    m_ui->pepMemoryDumpTextEdit->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
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

    int firstLine = firstByte / 8;
    int lastLine = lastByte / 8;

    QTextCursor cursor(m_ui->pepMemoryDumpTextEdit->document());
    cursor.setPosition(0);
    for (int i = 0; i < firstLine; i++) {
        cursor.movePosition(QTextCursor::NextBlock);
    }

    QString memoryDumpLine;
    QChar ch;
    int byteNum;
    for (int i = firstLine; i <= lastLine; i++) {
        memoryDumpLine = "";
        byteNum = i * 8;
        memoryDumpLine.append(QString("%1 | ").arg(byteNum, 4, 16, QLatin1Char('0')).toUpper());
        for (int j = 0; j < 8; j++) {
            memoryDumpLine.append(QString("%1 ").arg(Sim::Mem[byteNum++], 2, 16, QLatin1Char('0')).toUpper());
        }
        memoryDumpLine.append("|");
        byteNum = i * 8;
        for (int j = 0; j < 8; j++) {
            ch = QChar(Sim::Mem[byteNum++]);
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
    }

    m_ui->pepMemoryDumpTextEdit->verticalScrollBar()->setValue(vertScrollBarPosition);
    m_ui->pepMemoryDumpTextEdit->horizontalScrollBar()->setValue(horizScrollBarPosition);
}

void MemoryDumpPane::highlightMemory(bool b)
{
    while (!highlightedData.isEmpty()) {
        highlightByte(highlightedData.takeFirst(), Qt::black, Qt::white);
    }

    if (b) {
        highlightByte(Sim::stackPointer, Qt::white, Qt::darkMagenta);
        highlightedData.append(Sim::stackPointer);
        
        if (!Pep::isUnaryMap.value(Pep::decodeMnemonic.value(Sim::readByte(Sim::programCounter)))) {
            QTextCursor cursor(m_ui->pepMemoryDumpTextEdit->document());
            QTextCharFormat format;
            format.setBackground(Qt::blue);
            format.setForeground(Qt::white);
            cursor.setPosition(0);
            for (int i = 0; i < Sim::programCounter / 8; i++) {
                cursor.movePosition(QTextCursor::NextBlock);
            }
            for (int i = 0; i < 7 + 3 * (Sim::programCounter % 8); i++) {
                cursor.movePosition(QTextCursor::NextCharacter);
            }
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
            cursor.mergeCharFormat(format);
            highlightedData.append(Sim::programCounter);
            if (Sim::programCounter / 8 == (Sim::programCounter + 1) / 8) {
                cursor.clearSelection();
                cursor.movePosition(QTextCursor::NextCharacter);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
                cursor.mergeCharFormat(format);
            }
            else {
                cursor.clearSelection();
                cursor.movePosition(QTextCursor::NextBlock);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, 7);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
                cursor.mergeCharFormat(format);
            }
            highlightedData.append(Sim::add(Sim::programCounter, 1));
            if ((Sim::programCounter + 1) / 8 == (Sim::programCounter + 2) / 8) {
                cursor.clearSelection();
                cursor.movePosition(QTextCursor::NextCharacter);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
                cursor.mergeCharFormat(format);
            }
            else {
                cursor.clearSelection();
                cursor.movePosition(QTextCursor::NextBlock);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, 7);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
                cursor.mergeCharFormat(format);
            }
            highlightedData.append(Sim::add(Sim::programCounter, 2));
        }
        else { // unary.
            highlightByte(Sim::programCounter, Qt::white, Qt::blue);
            highlightedData.append(Sim::programCounter);
        }

        while (!bytesWrittenLastStep.isEmpty()) {
            highlightByte(bytesWrittenLastStep.at(0), Qt::white, Qt::red);
            highlightedData.append(bytesWrittenLastStep.takeFirst());
        }
    }
}

void MemoryDumpPane::cacheModifiedBytes()
{
    modifiedBytes.unite(Sim::modifiedBytes);
    if (Sim::tracingTraps) {
        bytesWrittenLastStep.clear();
        bytesWrittenLastStep = Sim::modifiedBytes.toList();
    }
    else if (Sim::trapped) {
        delayLastStepClear = true;
        bytesWrittenLastStep.append(Sim::modifiedBytes.toList());
    }
    else if (delayLastStepClear) {
        delayLastStepClear = false;
    }
    else {
        bytesWrittenLastStep.clear();
        bytesWrittenLastStep = Sim::modifiedBytes.toList();
    }
}

void MemoryDumpPane::updateMemory()
{
    int vertScrollBarPosition = m_ui->pepMemoryDumpTextEdit->verticalScrollBar()->value();
    int horizScrollBarPosition = m_ui->pepMemoryDumpTextEdit->horizontalScrollBar()->value();

    QList<int> list;
    QSet<int> linesToBeUpdated;
    QString memoryDumpLine;
    QChar ch;
    int byteNum;
    int lineNum;

    modifiedBytes.unite(Sim::modifiedBytes);
    list = modifiedBytes.toList();
    while(!list.isEmpty()) {
        linesToBeUpdated.insert(list.takeFirst() / 8);
    }
    list = linesToBeUpdated.toList();
    qSort(list.begin(), list.end());
    QTextCursor cursor(m_ui->pepMemoryDumpTextEdit->document());
    cursor.setPosition(0);
    lineNum = 0;
    while (!list.isEmpty()) {
        while (lineNum < list.first()) {
            cursor.movePosition(QTextCursor::NextBlock);
            lineNum++;
        }

        memoryDumpLine = "";
        byteNum = lineNum * 8;
        memoryDumpLine.append(QString("%1 | ").arg(byteNum, 4, 16, QLatin1Char('0')).toUpper());
        for (int j = 0; j < 8; j++) {
            memoryDumpLine.append(QString("%1 ").arg(Sim::Mem[byteNum++], 2, 16, QLatin1Char('0')).toUpper());
        }
        memoryDumpLine.append("|");
        byteNum = lineNum * 8;
        for (int j = 0; j < 8; j++) {
            ch = QChar(Sim::Mem[byteNum++]);
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
        lineNum++;
        list.removeFirst();
    }
    modifiedBytes.clear();

    m_ui->pepMemoryDumpTextEdit->verticalScrollBar()->setValue(vertScrollBarPosition);
    m_ui->pepMemoryDumpTextEdit->horizontalScrollBar()->setValue(horizScrollBarPosition);

}

void MemoryDumpPane::scrollToTop()
{
    m_ui->pepMemoryDumpTextEdit->verticalScrollBar()->setValue(0);
    m_ui->pepMemoryDumpTextEdit->horizontalScrollBar()->setValue(0);
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

void MemoryDumpPane::copy()
{
    m_ui->pepMemoryDumpTextEdit->copy();
}

void MemoryDumpPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(m_ui->pepMemoryDumpTextEdit->font()), this, "Set Memory Dump Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        m_ui->pepMemoryDumpTextEdit->setFont(font);
    }
}

int MemoryDumpPane::memoryDumpWidth()
{
    return m_ui->pepMemoryDumpTextEdit->document()->documentLayout()->documentSize().toSize().width() +
            m_ui->pepMemoryDumpTextEdit->verticalScrollBar()->width() + 6;
}

QSize MemoryDumpPane::sizeHint()
{
//    qDebug() << QSize(m_ui->pepMemoryDumpTextEdit->document()->documentLayout()->documentSize().toSize().width() +
//            m_ui->pepMemoryDumpTextEdit->verticalScrollBar()->width() + 5, 600);
    return QSize(m_ui->pepMemoryDumpTextEdit->document()->documentLayout()->documentSize().toSize().width() +
            m_ui->pepMemoryDumpTextEdit->verticalScrollBar()->width() + 6, 600);
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

void MemoryDumpPane::mouseReleaseEvent(QMouseEvent *)
{
    m_ui->pepMemoryDumpTextEdit->setFocus();
}

void MemoryDumpPane::on_pepMemRefreshButton_clicked()
{
    int scrollBarPosition = m_ui->pepMemoryDumpTextEdit->verticalScrollBar()->value();
    refreshMemory();
    m_ui->pepMemoryDumpTextEdit->verticalScrollBar()->setValue(scrollBarPosition);
}

