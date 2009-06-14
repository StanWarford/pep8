#include "inputpane.h"
#include "ui_inputpane.h"

InputPane::InputPane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::InputPane)
{
    m_ui->setupUi(this);
}

InputPane::~InputPane()
{
    delete m_ui;
}

void InputPane::highlightOnFocus()
{
    if (m_ui->_textEdit->hasFocus()) {
        m_ui->_label->setAutoFillBackground(true);
    }
    else {
        m_ui->_label->setAutoFillBackground(false);
    }
}

bool InputPane::hasFocus()
{
    return m_ui->_textEdit->hasFocus();
}

void InputPane::undo()
{
    m_ui->_textEdit->undo();
}

void InputPane::redo()
{
    m_ui->_textEdit->redo();
}

void InputPane::cut()
{
    m_ui->_textEdit->cut();
}

void InputPane::copy()
{
    m_ui->_textEdit->copy();
}

void InputPane::paste()
{
    m_ui->_textEdit->paste();
}


