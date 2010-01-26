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

    connect(ui->pepInputTextEdit, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
    connect(ui->pepInputTextEdit, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));

    ui->pepInputLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
    ui->pepInputTextEdit->setFont(QFont(Pep::codeFont, Pep::ioFontSize));
}

InputPane::~InputPane()
{
    delete ui;
}

void InputPane::highlightOnFocus()
{
    if (ui->pepInputTextEdit->hasFocus()) {
        ui->pepInputLabel->setAutoFillBackground(true);
    }
    else {
        ui->pepInputLabel->setAutoFillBackground(false);
    }
}

QString InputPane::toPlainText()
{
    return ui->pepInputTextEdit->toPlainText();
}

void InputPane::setText(QString input)
{
    ui->pepInputTextEdit->setText(input);
}

bool InputPane::hasFocus()
{
    return ui->pepInputTextEdit->hasFocus();
}

void InputPane::undo()
{
    ui->pepInputTextEdit->undo();
}

void InputPane::redo()
{
    ui->pepInputTextEdit->redo();
}

bool InputPane::isUndoable()
{
    return ui->pepInputTextEdit->document()->isUndoAvailable();
}

bool InputPane::isRedoable()
{
    return ui->pepInputTextEdit->document()->isRedoAvailable();
}

void InputPane::cut()
{
    ui->pepInputTextEdit->cut();
}

void InputPane::copy()
{
    ui->pepInputTextEdit->copy();
}

void InputPane::paste()
{
    ui->pepInputTextEdit->paste();
}

void InputPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(ui->pepInputTextEdit->font()), this, "Set Input Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        ui->pepInputTextEdit->setFont(font);
    }
}

void InputPane::setReadOnly(bool b)
{
    ui->pepInputTextEdit->setReadOnly(b);
}

void InputPane::tab()
{
    if (!ui->pepInputTextEdit->isReadOnly()) {
        QTextCursor cursor = ui->pepInputTextEdit->textCursor();
        cursor.movePosition(QTextCursor::StartOfLine);

        int curLinePos = ui->pepInputTextEdit->textCursor().position() - cursor.position();

        QString string;
        int spaces;
        spaces = 4 - (curLinePos % 4);

        for (int i = 0; i < spaces; i++) {
            string.append(" ");
        }

        ui->pepInputTextEdit->insertPlainText(string);
    }
}

void InputPane::mouseReleaseEvent(QMouseEvent *)
{
    ui->pepInputTextEdit->setFocus();
}
