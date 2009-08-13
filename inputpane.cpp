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
        m_ui(new Ui::InputPane)
{
    m_ui->setupUi(this);

    connect(m_ui->pepInputTextEdit, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
    connect(m_ui->pepInputTextEdit, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));

    if (Pep::getSystem() != "Mac") {
        m_ui->pepInputLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));
        m_ui->pepInputTextEdit->setFont(QFont(Pep::codeFont, Pep::ioFontSize));
    }
}

InputPane::~InputPane()
{
    delete m_ui;
}

void InputPane::highlightOnFocus()
{
    if (m_ui->pepInputTextEdit->hasFocus()) {
        m_ui->pepInputLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->pepInputLabel->setAutoFillBackground(false);
    }
}

QString InputPane::toPlainText()
{
    return m_ui->pepInputTextEdit->toPlainText();
}

void InputPane::setText(QString input)
{
    m_ui->pepInputTextEdit->setText(input);
}

bool InputPane::hasFocus()
{
    return m_ui->pepInputTextEdit->hasFocus();
}

void InputPane::undo()
{
    m_ui->pepInputTextEdit->undo();
}

void InputPane::redo()
{
    m_ui->pepInputTextEdit->redo();
}

bool InputPane::isUndoable()
{
    return m_ui->pepInputTextEdit->document()->isUndoAvailable();
}

bool InputPane::isRedoable()
{
    return m_ui->pepInputTextEdit->document()->isRedoAvailable();
}

void InputPane::cut()
{
    m_ui->pepInputTextEdit->cut();
}

void InputPane::copy()
{
    m_ui->pepInputTextEdit->copy();
}

void InputPane::paste()
{
    m_ui->pepInputTextEdit->paste();
}

void InputPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(m_ui->pepInputTextEdit->font()), this, "Set Input Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        m_ui->pepInputTextEdit->setFont(font);
    }
}

void InputPane::setReadOnly(bool b)
{
    m_ui->pepInputTextEdit->setReadOnly(b);
}

void InputPane::tab()
{
    if (!m_ui->pepInputTextEdit->isReadOnly()) {
        QTextCursor cursor = m_ui->pepInputTextEdit->textCursor();
        cursor.movePosition(QTextCursor::StartOfLine);

        int curLinePos = m_ui->pepInputTextEdit->textCursor().position() - cursor.position();

        QString string;
        int spaces;
        spaces = 4 - (curLinePos % 4);

        for (int i = 0; i < spaces; i++) {
            string.append(" ");
        }

        m_ui->pepInputTextEdit->insertPlainText(string);
    }
}

void InputPane::mouseReleaseEvent(QMouseEvent *)
{
    m_ui->pepInputTextEdit->setFocus();
}
