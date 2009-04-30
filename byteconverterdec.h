#ifndef BYTECONVERTERDEC_H
#define BYTECONVERTERDEC_H

#include <QtGui/QWidget>
#include <QRegExpValidator>

namespace Ui {
    class ByteConverterDec;
}

class ByteConverterDec : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(ByteConverterDec)
public:
    explicit ByteConverterDec(QWidget *parent = 0);
    virtual ~ByteConverterDec();
    void setValue(int);

private:
    Ui::ByteConverterDec *m_ui;
    QRegExpValidator* decValidator;

signals:
    void textEdited(const QString &);
};

#endif // BYTECONVERTERDEC_H
