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

    connect(ui->pepTerminalTextEdit, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
    connect(ui->pepTerminalTextEdit, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));

    ui->pepTerminalLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
    ui->pepTerminalTextEdit->setFont(QFont(Pep::codeFont, Pep::ioFontSize));
    
    qApp->installEventFilter(this);
}

TerminalPane::~TerminalPane()
{
    delete ui;
}

void TerminalPane::appendOutput(QString str)
{
    ui->pepTerminalTextEdit->setText(ui->pepTerminalTextEdit->toPlainText().append(str));
    strokeString.append(str);
    ui->pepTerminalTextEdit->verticalScrollBar()->setValue(ui->pepTerminalTextEdit->verticalScrollBar()->maximum()); // Scroll to bottom
}

void TerminalPane::waitingForInput()
{
    waiting = true;
    displayTerminal();
    ui->pepTerminalTextEdit->setFocus();
}

void TerminalPane::clearTerminal()
{
    ui->pepTerminalTextEdit->clear();
    retString = "";
    strokeString = "";
}

void TerminalPane::highlightOnFocus()
{
    if (ui->pepTerminalTextEdit->hasFocus()) {
        ui->pepTerminalLabel->setAutoFillBackground(true);
    }
    else {
        ui->pepTerminalLabel->setAutoFillBackground(false);
    }
}

bool TerminalPane::hasFocus()
{
    return ui->pepTerminalTextEdit->hasFocus();
}

void TerminalPane::copy()
{
    ui->pepTerminalTextEdit->copy();
}

void TerminalPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(ui->pepTerminalTextEdit->font()), this, "Set Terminal Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        ui->pepTerminalTextEdit->setFont(font);
    }
}

void TerminalPane::displayTerminal()
{
    if (waiting) {
        ui->pepTerminalTextEdit->setPlainText(strokeString + retString + QString("_"));
    }
    else {
        ui->pepTerminalTextEdit->setPlainText(strokeString + retString);
    }
    ui->pepTerminalTextEdit->verticalScrollBar()->setValue(ui->pepTerminalTextEdit->verticalScrollBar()->maximum()); // Scroll to bottom
}

bool TerminalPane::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyPress && ui->pepTerminalTextEdit->hasFocus() && waiting) {
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
    ui->pepTerminalTextEdit->setFocus();
}

