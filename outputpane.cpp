#include "outputpane.h"
#include "ui_outputpane.h"

OutputPane::OutputPane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::OutputPane)
{
    m_ui->setupUi(this);
}

OutputPane::~OutputPane()
{
    delete m_ui;
}

void OutputPane::highlightOnFocus()
{
    if (m_ui->pepOutputTextEdit->hasFocus()) {
        m_ui->pepOutputLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->pepOutputLabel->setAutoFillBackground(false);
    }
}
