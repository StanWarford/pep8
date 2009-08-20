// File: assemblerlistingpane.cpp
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

#include <QScrollBar>
#include <QFontDialog>
#include "assemblerlistingpane.h"
#include "ui_assemblerlistingpane.h"
#include "pep.h"

#include <QMouseEvent>

AssemblerListingPane::AssemblerListingPane(QWidget *parent) :
        QWidget(parent),
        m_ui(new Ui::AssemblerListingPane)
{
    m_ui->setupUi(this);

    pepHighlighter = new PepHighlighter(m_ui->assemblerListingTextEdit->document());

    m_ui->assemblerListingLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));
    m_ui->assemblerListingTextEdit->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
}

AssemblerListingPane::~AssemblerListingPane()
{
    delete m_ui;
}

void AssemblerListingPane::setAssemblerListing(QStringList assemblerListingList) {
    clearAssemblerListing();
    m_ui->assemblerListingTextEdit->append("-------------------------------------------------------------------------------");
    m_ui->assemblerListingTextEdit->append("      Object");
    m_ui->assemblerListingTextEdit->append("Addr  code   Symbol   Mnemon  Operand     Comment");
    m_ui->assemblerListingTextEdit->append("-------------------------------------------------------------------------------");
    m_ui->assemblerListingTextEdit->append(assemblerListingList.join("\n"));
    m_ui->assemblerListingTextEdit->append("-------------------------------------------------------------------------------");
    if (Pep::symbolTable.size() > 0) {
        m_ui->assemblerListingTextEdit->append("");
        m_ui->assemblerListingTextEdit->append("");
        m_ui->assemblerListingTextEdit->append("Symbol table");
        m_ui->assemblerListingTextEdit->append("--------------------------------------");
        m_ui->assemblerListingTextEdit->append("Symbol    Value        Symbol    Value");
        m_ui->assemblerListingTextEdit->append("--------------------------------------");
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
                m_ui->assemblerListingTextEdit->append(symbolTableLine);
                symbolTableLine = "";
            }
        }
        if (symbolTableLine.length() > 0) {
            m_ui->assemblerListingTextEdit->append(symbolTableLine);
        }
        m_ui->assemblerListingTextEdit->append("--------------------------------------");
    }
    m_ui->assemblerListingTextEdit->verticalScrollBar()->setValue(m_ui->assemblerListingTextEdit->verticalScrollBar()->minimum());
}

void AssemblerListingPane::clearAssemblerListing()
{
    m_ui->assemblerListingTextEdit->clear();
}

bool AssemblerListingPane::isModified()
{
    return m_ui->assemblerListingTextEdit->document()->isModified();
}

QString AssemblerListingPane::toPlainText()
{
    return m_ui->assemblerListingTextEdit->toPlainText();
}

void AssemblerListingPane::setCurrentFile(QString string)
{
    if (!string.isEmpty()) {
        m_ui->assemblerListingLabel->setText("Assembler Listing - " + string);
    }
    else {
        m_ui->assemblerListingLabel->setText("Assembler Listing - untitled.pepl");
    }
}

void AssemblerListingPane::highlightOnFocus()
{
    if (m_ui->assemblerListingTextEdit->hasFocus()) {
        m_ui->assemblerListingLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->assemblerListingLabel->setAutoFillBackground(false);
    }
}

bool AssemblerListingPane::hasFocus()
{
    return m_ui->assemblerListingTextEdit->hasFocus();
}

void AssemblerListingPane::copy()
{
    m_ui->assemblerListingTextEdit->copy();
}

void AssemblerListingPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(m_ui->assemblerListingTextEdit->font()), this, "Set Assembler Listing Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        m_ui->assemblerListingTextEdit->setFont(font);
    }
}

void AssemblerListingPane::mouseReleaseEvent(QMouseEvent *)
{
    m_ui->assemblerListingTextEdit->setFocus();
}

void AssemblerListingPane::mouseDoubleClickEvent(QMouseEvent *)
{
    emit labelDoubleClicked(Enu::EListing);
}
