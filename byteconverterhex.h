#ifndef BYTECONVERTERHEX_H
#define BYTECONVERTERHEX_H

#include <QtGui/QWidget>
#include <QRegExpValidator>

namespace Ui {
    class ByteConverterHex;
}

class ByteConverterHex : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(ByteConverterHex)
public:
    explicit ByteConverterHex(QWidget *parent = 0);
    virtual ~ByteConverterHex();
    void setValue(int);

private:
    Ui::ByteConverterHex *m_ui;
    QRegExpValidator* hexValidator;

private slots:
    void moveCursorAwayFromPrefix(int old, int current);

signals:
    void textEdited(const QString &);
};

#endif // BYTECONVERTERHEX_H
