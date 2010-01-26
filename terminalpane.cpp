//File: terminalpane.cpp
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
#include "terminalpane.h"
#include "ui_terminalpane.h"
#include "sim.h"
#include "pep.h"

TerminalPane::TerminalPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TerminalPane)
{
    ui->setupUi(this);

    waiting = false;

    connect(ui->textEdit, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
    connect(ui->textEdit, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));

    ui->label->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
    ui->textEdit->setFont(QFont(Pep::codeFont, Pep::ioFontSize));
    
    qApp->installEventFilter(this);
}

TerminalPane::~TerminalPane()
{
    delete ui;
}

void TerminalPane::appendOutput(QString str)
{
    ui->textEdit->setText(ui->textEdit->toPlainText().append(str));
    strokeString.append(str);
    ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->maximum()); // Scroll to bottom
}

void TerminalPane::waitingForInput()
{
    waiting = true;
    displayTerminal();
    ui->textEdit->setFocus();
}

void TerminalPane::clearTerminal()
{
    ui->textEdit->clear();
    retString = "";
    strokeString = "";
}

void TerminalPane::highlightOnFocus()
{
    if (ui->textEdit->hasFocus()) {
        ui->label->setAutoFillBackground(true);
    }
    else {
        ui->label->setAutoFillBackground(false);
    }
}

bool TerminalPane::hasFocus()
{
    return ui->textEdit->hasFocus();
}

void TerminalPane::copy()
{
    ui->textEdit->copy();
}

void TerminalPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(ui->textEdit->font()), this, "Set Terminal Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        ui->textEdit->setFont(font);
    }
}

void TerminalPane::displayTerminal()
{
    if (waiting) {
        ui->textEdit->setPlainText(strokeString + retString + QString("_"));
    }
    else {
        ui->textEdit->setPlainText(strokeString + retString);
    }
    ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->maximum()); // Scroll to bottom
}

bool TerminalPane::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyPress && ui->textEdit->hasFocus() && waiting) {
        QKeyEvent *e = static_cast<QKeyEvent *>(event);
        if (e->key() == Qt::Key_Shift || e->key() == Qt::Key_Control ||
            e->key() == Qt::Key_Meta || e->key() == Qt::Key_Alt ||
            e->key() == Qt::Key_CapsLock || e->key() == Qt::Key_NumLock ||
            e->key() == Qt::Key_ScrollLock || e->key() == Qt::Key_Tab) {
            // skip
        }
        else if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
            retString.append('\n');
            strokeString.append(retString);
            waiting = false;
            Sim::inputBuffer = retString;
            retString = "";
            displayTerminal();
            emit inputReceived();
            return true;
        }
        else if (e->key() == Qt::Key_Backspace && !retString.isEmpty()) {
            retString.truncate(retString.length() - 1);
        }
        else {
            retString.append(e->text());
        }
        displayTerminal();
        return true;
    }
    return false;
}

void TerminalPane::mouseReleaseEvent(QMouseEvent *)
{
    ui->textEdit->setFocus();
}

