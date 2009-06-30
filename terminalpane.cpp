#include "terminalpane.h"
#include "ui_terminalpane.h"

TerminalPane::TerminalPane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::TerminalPane)
{
    m_ui->setupUi(this);

    isUndoable = false;
    isRedoable = false;

    connect(m_ui->pepTerminalTextEdit, SIGNAL(undoAvailable(bool)), this, SLOT(setUndoability(bool)));
    connect(m_ui->pepTerminalTextEdit, SIGNAL(redoAvailable(bool)), this, SLOT(setRedoability(bool)));

    connect(m_ui->pepTerminalTextEdit, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
    connect(m_ui->pepTerminalTextEdit, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));
}

TerminalPane::~TerminalPane()
{
    delete m_ui;
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

void TerminalPane::undo()
{
    m_ui->pepTerminalTextEdit->undo();
}

void TerminalPane::redo()
{
    m_ui->pepTerminalTextEdit->redo();
}

void TerminalPane::cut()
{
    // not allowed in interactive i/o
}

void TerminalPane::copy()
{
    m_ui->pepTerminalTextEdit->copy();
}

void TerminalPane::paste()
{
    m_ui->pepTerminalTextEdit->moveCursor(QTextCursor::End);
    m_ui->pepTerminalTextEdit->paste();
}

void TerminalPane::setUndoability(bool b)
{
    isUndoable = b;
}

void TerminalPane::setRedoability(bool b)
{
    isRedoable = b;
}

