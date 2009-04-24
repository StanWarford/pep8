#include "objectcodepane.h"
#include "ui_objectcodepane.h"

ObjectCodePane::ObjectCodePane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ObjectCodePane)
{
    m_ui->setupUi(this);
}

ObjectCodePane::~ObjectCodePane()
{
    delete m_ui;
}
