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
