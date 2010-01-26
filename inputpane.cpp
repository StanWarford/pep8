// File: inputpane.cpp
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
#include <QKeyEvent>
#include "inputpane.h"
#include "ui_inputpane.h"
#include "pep.h"

InputPane::InputPane(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::InputPane)
{
    ui->setupUi(this);

    connect(ui->textEdit, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
    connect(ui->textEdit, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));

    ui->label->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
    ui->textEdit->setFont(QFont(Pep::codeFont, Pep::ioFontSize));
}

InputPane::~InputPane()
{
    delete ui;
}

void InputPane::highlightOnFocus()
{
    if (ui->textEdit->hasFocus()) {
        ui->label->setAutoFillBackground(true);
    }
    else {
        ui->label->setAutoFillBackground(false);
    }
}

QString InputPane::toPlainText()
{
    return ui->textEdit->toPlainText();
}

void InputPane::setText(QString input)
{
    ui->textEdit->setText(input);
}

bool InputPane::hasFocus()
{
    return ui->textEdit->hasFocus();
}

void InputPane::undo()
{
    ui->textEdit->undo();
}

void InputPane::redo()
{
    ui->textEdit->redo();
}

bool InputPane::isUndoable()
{
    return ui->textEdit->document()->isUndoAvailable();
}

bool InputPane::isRedoable()
{
    return ui->textEdit->document()->isRedoAvailable();
}

void InputPane::cut()
{
    ui->textEdit->cut();
}

void InputPane::copy()
{
    ui->textEdit->copy();
}

void InputPane::paste()
{
    ui->textEdit->paste();
}

void InputPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(ui->textEdit->font()), this, "Set Input Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        ui->textEdit->setFont(font);
    }
}

void InputPane::setReadOnly(bool b)
{
    ui->textEdit->setReadOnly(b);
}

void InputPane::tab()
{
    if (!ui->textEdit->isReadOnly()) {
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.movePosition(QTextCursor::StartOfLine);

        int curLinePos = ui->textEdit->textCursor().position() - cursor.position();

        QString string;
        int spaces;
        spaces = 4 - (curLinePos % 4);

        for (int i = 0; i < spaces; i++) {
            string.append(" ");
        }

        ui->textEdit->insertPlainText(string);
    }
}

void InputPane::mouseReleaseEvent(QMouseEvent *)
{
    ui->textEdit->setFocus();
}
