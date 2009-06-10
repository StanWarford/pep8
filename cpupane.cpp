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

void CpuPane::highlightOnFocus()
{
    if (m_ui->pepCpuLabel->hasFocus()) { // Never has focus, which is fine
        m_ui->pepCpuLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->pepCpuLabel->setAutoFillBackground(false);
    }
}
