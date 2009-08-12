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
#include <QScrollBar>
#include "outputpane.h"
#include "ui_outputpane.h"
#include "pep.h"

OutputPane::OutputPane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::OutputPane)
{
    m_ui->setupUi(this);

    if (Pep::getSystem() != "Mac") {
        m_ui->pepOutputLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));
        m_ui->pepOutputTextEdit->setFont(QFont(Pep::codeFont, Pep::ioFontSize));
    }
}

OutputPane::~OutputPane()
{
    delete m_ui;
}

void OutputPane::appendOutput(QString str)
{
    m_ui->pepOutputTextEdit->setText(m_ui->pepOutputTextEdit->toPlainText().append(str));
    m_ui->pepOutputTextEdit->verticalScrollBar()->setValue(m_ui->pepOutputTextEdit->verticalScrollBar()->maximum());
}

void OutputPane::clearOutput()
{
    m_ui->pepOutputTextEdit->clear();
}

void OutputPane::highlightOnFocus()
{
    if (m_ui->pepOutputTextEdit->hasFocus()) {
        m_ui->pepOutputLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->pepOutputLabel->setAutoFillBackground(false);
    }
}

bool OutputPane::hasFocus()
{
    return m_ui->pepOutputTextEdit->hasFocus();
}

void OutputPane::copy()
{
    m_ui->pepOutputTextEdit->copy();
}

void OutputPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(m_ui->pepOutputTextEdit->font()), this, "Set Output Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        m_ui->pepOutputTextEdit->setFont(font);
    }
}

void OutputPane::mouseReleaseEvent(QMouseEvent *)
{
    m_ui->pepOutputTextEdit->setFocus();
}
