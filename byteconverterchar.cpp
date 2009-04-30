#include "byteconverterchar.h"
#include "ui_byteconverterchar.h"

ByteConverterChar::ByteConverterChar(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ByteConverterChar)
{
    m_ui->setupUi(this);
    // Regular expression to validate a single character
    QRegExp charRx("(.){0,1}");
    charValidator = new QRegExpValidator(charRx, 0);
    m_ui->lineEdit->setValidator(charValidator);
    // Forward the textEdited() signal from m_ui->lineEdit up to the main window
    QObject::connect(m_ui->lineEdit, SIGNAL(textEdited(const QString &)), this,
                     SIGNAL(textEdited(const QString &)));
}

ByteConverterChar::~ByteConverterChar()
{
    delete m_ui;
}

void ByteConverterChar::setValue(int value)
{
    m_ui->lineEdit->setText(QString(QChar(value)));
}
