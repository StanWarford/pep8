#include "sourcecodepane.h"
#include "ui_sourcecodepane.h"

SourceCodePane::SourceCodePane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::SourceCodePane)
{
    m_ui->setupUi(this);
}

SourceCodePane::~SourceCodePane()
{
    delete m_ui;
}
