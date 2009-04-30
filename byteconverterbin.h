#ifndef BYTECONVERTERBIN_H
#define BYTECONVERTERBIN_H

#include <QtGui/QWidget>
#include <QRegExpValidator>

namespace Ui {
    class ByteConverterBin;
}

class ByteConverterBin : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(ByteConverterBin)
        public:
            explicit ByteConverterBin(QWidget *parent = 0);
    virtual ~ByteConverterBin();
    void setValue(int);

private:
    Ui::ByteConverterBin *m_ui;
    QRegExpValidator* binValidator;

signals:
    void textEdited(const QString &);
};

#endif // BYTECONVERTERBIN_H
