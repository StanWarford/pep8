#include <QFontDialog>
#include "inputpane.h"
#include "ui_inputpane.h"
#include "pep.h"

InputPane::InputPane(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::InputPane)
{
    m_ui->setupUi(this);

    connect(m_ui->pepInputTextEdit, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
    connect(m_ui->pepInputTextEdit, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));

    if (Pep::getSystem() != "Mac") {
        m_ui->pepInputLabel->setFont(QFont("Verdana"));
        m_ui->pepInputTextEdit->setFont(QFont("Dark Courier"));
    }
}

InputPane::~InputPane()
{
    delete m_ui;
}

void InputPane::highlightOnFocus()
{
    if (m_ui->pepInputTextEdit->hasFocus()) {
        m_ui->pepInputLabel->setAutoFillBackground(true);
    }
    else {
        m_ui->pepInputLabel->setAutoFillBackground(false);
    }
}

QString InputPane::toPlainText()
{
    return m_ui->pepInputTextEdit->toPlainText();
}

void InputPane::setText(QString input)
{
    m_ui->pepInputTextEdit->setText(input);
}

bool InputPane::hasFocus()
{
    return m_ui->pepInputTextEdit->hasFocus();
}

void InputPane::undo()
{
    m_ui->pepInputTextEdit->undo();
}

void InputPane::redo()
{
    m_ui->pepInputTextEdit->redo();
}

bool InputPane::isUndoable()
{
    return m_ui->pepInputTextEdit->document()->isUndoAvailable();
}

bool InputPane::isRedoable()
{
    return m_ui->pepInputTextEdit->document()->isRedoAvailable();
}

void InputPane::cut()
{
    m_ui->pepInputTextEdit->cut();
}

void InputPane::copy()
{
    m_ui->pepInputTextEdit->copy();
}

void InputPane::paste()
{
    m_ui->pepInputTextEdit->paste();
}

void InputPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(m_ui->pepInputTextEdit->font()), this, "Set Input Font", QFontDialog::DontUseNativeDialog);
    if (ok) {
        m_ui->pepInputTextEdit->setFont(font);
    }
}

void InputPane::setReadOnly(bool b)
{
    m_ui->pepInputTextEdit->setReadOnly(b);
}
