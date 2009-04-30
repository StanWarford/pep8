#include "byteconverterbin.h"
#include "ui_byteconverterbin.h"

ByteConverterBin::ByteConverterBin(QWidget *parent) :
        QWidget(parent),
        m_ui(new Ui::ByteConverterBin)
{
    m_ui->setupUi(this);
    // Regular expression to validate a binary byte
    QRegExp binRx("([0-1]{0,8})");
    binValidator = new QRegExpValidator(binRx, 0);
    m_ui->lineEdit->setValidator(binValidator);
    // Forward the textEdited() signal from m_ui->lineEdit up to the main window
    QObject::connect(m_ui->lineEdit, SIGNAL(textEdited(const QString &)), this,
                     SIGNAL(textEdited(const QString &)));
}

ByteConverterBin::~ByteConverterBin()
{
    delete m_ui;
}

void ByteConverterBin::setValue(int value)
{
    m_ui->lineEdit->setText(QString("%1").arg(value, 8, 2, QLatin1Char('0')));
}
