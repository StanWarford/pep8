// File: outputpane.cpp
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
    ui(new Ui::OutputPane)
{
    ui->setupUi(this);

    ui->label->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
    ui->textEdit->setFont(QFont(Pep::codeFont, Pep::ioFontSize));
}

OutputPane::~OutputPane()
{
    delete ui;
}

void OutputPane::appendOutput(QString str)
{
    ui->textEdit->setText(ui->textEdit->toPlainText().append(str));
    ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->maximum());
}

void OutputPane::clearOutput()
{
    ui->textEdit->clear();
}

void OutputPane::highlightOnFocus()
{
    if (ui->textEdit->hasFocus()) {
        ui->label->setAutoFillBackground(true);
    }
    else {
        ui->label->setAutoFillBackground(false);
    }
}

bool OutputPane::hasFocus()
{
    return ui->textEdit->hasFocus();
}

void OutputPane::copy()
{
    ui->textEdit->copy();
}

void OutputPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(ui->textEdit->font()), this, "Set Output Font");
    if (ok) {
        ui->textEdit->setFont(font);
    }
}

void OutputPane::mouseReleaseEvent(QMouseEvent *)
{
    ui->textEdit->setFocus();
}
