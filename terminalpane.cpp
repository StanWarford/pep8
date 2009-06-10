#include "terminalpane.h"
#include "ui_terminalpane.h"

TerminalPane::TerminalPane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::TerminalPane)
{
    m_ui->setupUi(this);
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
