#include "aboutpep.h"
#include "ui_aboutpep.h"

AboutPep::AboutPep(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AboutPep)
{
    m_ui->setupUi(this);
}

AboutPep::~AboutPep()
{
    delete m_ui;
}
