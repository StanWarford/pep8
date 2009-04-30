#include "byteconverterdec.h"
#include "ui_byteconverterdec.h"

ByteConverterDec::ByteConverterDec(QWidget *parent) :
        QWidget(parent),
        m_ui(new Ui::ByteConverterDec)
{
    m_ui->setupUi(this);
    // Regular expression to validate 0-255
    QRegExp decRx("^(25[0-5])|(2[0-4][0-9])|([0-1][0-9][0-9])|([0-9]{0,2})$");
    decValidator = new QRegExpValidator(decRx, 0);
    m_ui->lineEdit->setValidator(decValidator);
    // Forward the textEdited() signal from m_ui->lineEdit up to the main window
    QObject::connect(m_ui->lineEdit, SIGNAL(textEdited(const QString &)), this,
                     SIGNAL(textEdited(const QString &)));
}

ByteConverterDec::~ByteConverterDec()
{
    delete m_ui;
}

void ByteConverterDec::setValue(int value)
{
    m_ui->lineEdit->setText(QString("%1").arg(value, 0, 10));
}
