#ifndef ABOUTPEP_H
#define ABOUTPEP_H

#include <QtGui/QDialog>

namespace Ui {
    class AboutPep;
}

class AboutPep : public QDialog {
    Q_OBJECT
public:
    AboutPep(QWidget *parent = 0);
    ~AboutPep();

private:
    Ui::AboutPep *m_ui;
};

#endif // ABOUTPEP_H
