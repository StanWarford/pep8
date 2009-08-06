#include "aboutpep.h"
#include "ui_aboutpep.h"
#include "pep.h"

AboutPep::AboutPep(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AboutPep)
{
    m_ui->setupUi(this);
    if (Pep::getSystem() != "Mac") {
        m_ui->aboutLabel->hide();
    }
    else {
        m_ui->aboutLabelWin->hide();
    }
}

AboutPep::~AboutPep()
{
    delete m_ui;
}
