#include <QDebug>
#include "byteconverterhex.h"
#include "ui_byteconverterhex.h"

ByteConverterHex::ByteConverterHex(QWidget *parent) :
        QWidget(parent),
        m_ui(new Ui::ByteConverterHex)
{
    m_ui->setupUi(this);
    // Regular expression to validate 0x00 - 0xff
    QRegExp hexRx("0x([0-9]|[a-f]|[A-F])([0-9]|[a-f]|[A-F])");
    hexValidator = new QRegExpValidator(hexRx, 0);
    m_ui->lineEdit->setValidator(hexValidator);
    // Forward the textEdited() signal from m_ui->lineEdit up to the main window
    QObject::connect(m_ui->lineEdit, SIGNAL(textEdited(const QString &)), this,
                     SIGNAL(textEdited(const QString &)));


}

ByteConverterHex::~ByteConverterHex()
{
    delete m_ui;
}

void ByteConverterHex::setValue(int value)
{
    if (value == -1) {
        m_ui->lineEdit->setText("0x");
    }
    else {
        m_ui->lineEdit->setText(QString("0x%1").arg(value, 2, 16, QLatin1Char('0')));
    }
}
