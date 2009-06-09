#include "objectcodepane.h"
#include "ui_objectcodepane.h"
#include <QDebug>

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

void ObjectCodePane::setObjectCode(QList<int> objectCode)
{
    QString objectCodeString = "";
    for (int i = 0; i < objectCode.length(); i++) {
        objectCodeString.append(QString("%1").arg(objectCode[i], 2, 16, QLatin1Char('0')).toUpper());
        objectCodeString.append((i % 16) == 15 ? '\n' : ' ');
    }
    objectCodeString.append("zz");
    m_ui->pepObjectCodeTextEdit->clear();
    m_ui->pepObjectCodeTextEdit->setText(objectCodeString);
}

void ObjectCodePane::setObjectCodePaneText(QString string)
{
	m_ui->pepObjectCodeTextEdit->setText(string);
}

void ObjectCodePane::ObjectCodePane::clearObjectCode()
{
    m_ui->pepObjectCodeTextEdit->clear();
}


bool ObjectCodePane::isModified()
{
	return m_ui->pepObjectCodeTextEdit->document()->isModified();
}

void ObjectCodePane::setModified(bool modified)
{
	m_ui->pepObjectCodeTextEdit->document()->setModified(modified);
}

QString ObjectCodePane::toPlainText()
{
	return m_ui->pepObjectCodeTextEdit->toPlainText();
}

void ObjectCodePane::setCurrentFile(QString string)
{
    m_ui->pepObjectCodeLabel->setText("Object Code - " + string);
}
