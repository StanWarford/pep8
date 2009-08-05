#include <QFontDialog>
#include <QScrollBar>
#include "terminalpane.h"
#include "ui_terminalpane.h"
#include "sim.h"
#include "pep.h"

TerminalPane::TerminalPane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::TerminalPane)
{
    m_ui->setupUi(this);

    waiting = false;

    connect(m_ui->pepTerminalTextEdit, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
    connect(m_ui->pepTerminalTextEdit, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));

    if (Pep::getSystem() != "Mac") {
        m_ui->pepTerminalLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
        m_ui->pepTerminalTextEdit->setFont(QFont(Pep::codeFont, Pep::ioFontSize));
    }
}

TerminalPane::~TerminalPane()
{
    delete m_ui;
}

void TerminalPane::appendOutput(QString str)
{
    m_ui->pepTerminalTextEdit->setText(m_ui->pepTerminalTextEdit->toPlainText().append(str));
    strokeString.append(str);
}

void TerminalPane::waitingForInput()
{
    waiting = true;
    displayTerminal();
    m_ui->pepTerminalTextEdit->setFocus();
}

void TerminalPane::clearTerminal()
{
    m_ui->pepTerminalTextEdit->clear();
    retString = "";
    strokeString = "";
}

void TerminalPane::highlightOnFocus()
{
    if (m_ui->pepTerminalTextEdit->hasFocus()) {
        m_ui->pepTerminalLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->pepTerminalLabel->setAutoFillBackground(false);
    }
}

bool TerminalPane::hasFocus()
{
    return m_ui->pepTerminalTextEdit->hasFocus();
}

void TerminalPane::copy()
{
    m_ui->pepTerminalTextEdit->copy();
}

void TerminalPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(m_ui->pepTerminalTextEdit->font()), this, "Set Terminal Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        m_ui->pepTerminalTextEdit->setFont(font);
    }
}

void TerminalPane::displayTerminal()
{
    if (waiting) {
        m_ui->pepTerminalTextEdit->setPlainText(strokeString + retString + QString("_"));
    }
    else {
        m_ui->pepTerminalTextEdit->setPlainText(strokeString + retString);
    }
    QScrollBar* sBar = m_ui->pepTerminalTextEdit->verticalScrollBar();
    sBar->setValue(sBar->maximum()); // Scroll to bottom
}

void TerminalPane::keyPressEvent(QKeyEvent* e)
{
    if (m_ui->pepTerminalTextEdit->hasFocus())
    {
        m_ui->pepTerminalTextEdit->grabKeyboard();
        if (waiting)
        {
            if (e->key() == Qt::Key_Shift || e->key() == Qt::Key_Control ||
                e->key() == Qt::Key_Meta || e->key() == Qt::Key_Alt ||
                e->key() == Qt::Key_CapsLock || e->key() == Qt::Key_NumLock ||
                e->key() == Qt::Key_ScrollLock)
            {
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
                return;
            }
            else if (e->key() == Qt::Key_Backspace && !retString.isEmpty()) {
                retString.truncate(retString.length() - 1);
            }
            else {
                retString.append(e->text());
            }
            displayTerminal();
        }

    }
    else
    {
        m_ui->pepTerminalTextEdit->releaseKeyboard();
    }
}
