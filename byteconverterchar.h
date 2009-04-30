#ifndef BYTECONVERTERCHAR_H
#define BYTECONVERTERCHAR_H

#include <QtGui/QWidget>
#include <QRegExpValidator>

namespace Ui {
    class ByteConverterChar;
}

class ByteConverterChar : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(ByteConverterChar)
public:
    explicit ByteConverterChar(QWidget *parent = 0);
    virtual ~ByteConverterChar();
    void setValue(int);

private:
    Ui::ByteConverterChar *m_ui;
    QRegExpValidator* charValidator;

signals:
    void textEdited(const QString &);
};

#endif // BYTECONVERTERCHAR_H
