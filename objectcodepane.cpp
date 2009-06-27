#include "objectcodepane.h"
#include "ui_objectcodepane.h"
#include "pep.h"
#include <QDebug>

ObjectCodePane::ObjectCodePane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ObjectCodePane)
{
    m_ui->setupUi(this);

    connect(m_ui->pepObjectCodeTextEdit->document(), SIGNAL(modificationChanged(bool)), this, SLOT(setLabelToModified(bool)));

    isUndoable = false;
    isRedoable = false;

    connect(m_ui->pepObjectCodeTextEdit, SIGNAL(undoAvailable(bool)), this, SLOT(setUndoability(bool)));
    connect(m_ui->pepObjectCodeTextEdit, SIGNAL(redoAvailable(bool)), this, SLOT(setRedoability(bool)));

//    connect(m_ui->pepObjectCodeTextEdit, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
//    connect(m_ui->pepObjectCodeTextEdit, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));
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

bool ObjectCodePane::getObjectCode(QList<int> &objectCodeList)
{
    QString objectString = m_ui->pepObjectCodeTextEdit->toPlainText();
    while (objectString.length() > 0) {
        if (objectString.at(1) == QChar('z')) {
            return true;
        }
        if (objectString.length() < 3) {
            return false;
        }
        QString s = objectString.left(2); // Get the two-char hex number
        objectString.remove(0, 3); // Removes the number and trailing whitespace
        bool ok;
        objectCodeList.append(s.toInt(&ok, 16));
        if (!ok) {
            return false;
        }
    }
    return false;
}

void ObjectCodePane::ObjectCodePane::clearObjectCode()
{
    m_ui->pepObjectCodeTextEdit->clear();
}


bool ObjectCodePane::isModified()
{
    return m_ui->pepObjectCodeTextEdit->document()->isModified();
}

void ObjectCodePane::setModifiedFalse()
{
    m_ui->pepObjectCodeTextEdit->document()->setModified(false);
}

QString ObjectCodePane::toPlainText()
{
    return m_ui->pepObjectCodeTextEdit->toPlainText();
}

void ObjectCodePane::setCurrentFile(QString string)
{
    m_ui->pepObjectCodeLabel->setText("Object Code - " + string);
}

void ObjectCodePane::highlightOnFocus()
{
    if (m_ui->pepObjectCodeTextEdit->hasFocus()) {
        m_ui->pepObjectCodeLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->pepObjectCodeLabel->setAutoFillBackground(false);
    }
}

bool ObjectCodePane::hasFocus()
{
    return m_ui->pepObjectCodeTextEdit->hasFocus();
}

void ObjectCodePane::undo()
{
    m_ui->pepObjectCodeTextEdit->undo();
}

void ObjectCodePane::redo()
{
    m_ui->pepObjectCodeTextEdit->redo();
}

void ObjectCodePane::cut()
{
    m_ui->pepObjectCodeTextEdit->cut();
}

void ObjectCodePane::copy()
{
    m_ui->pepObjectCodeTextEdit->copy();
}

void ObjectCodePane::paste()
{
    m_ui->pepObjectCodeTextEdit->paste();
}


void ObjectCodePane::setLabelToModified(bool modified)
{
    QString temp = m_ui->pepObjectCodeLabel->text();
    if (modified) {
        m_ui->pepObjectCodeLabel->setText(temp.append(temp.endsWith(QChar('*')) ? "" : "*"));
    }
    else if (temp.endsWith(QChar('*'))) {
        temp.chop(1);
        m_ui->pepObjectCodeLabel->setText(temp);
    }
}

void ObjectCodePane::setUndoability(bool b)
{
    isUndoable = b;
}

void ObjectCodePane::setRedoability(bool b)
{
    isRedoable = b;
}


