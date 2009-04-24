#include "cpupane.h"
#include "ui_cpupane.h"

CpuPane::CpuPane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::CpuPane)
{
    m_ui->setupUi(this);
}

CpuPane::~CpuPane()
{
    delete m_ui;
}
