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
