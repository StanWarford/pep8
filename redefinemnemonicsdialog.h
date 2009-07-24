#ifndef REDEFINEMNEMONICSDIALOG_H
#define REDEFINEMNEMONICSDIALOG_H

#include <QtGui/QDialog>
#include "pep.h"

namespace Ui {
    class RedefineMnemonicsDialog;
}

class RedefineMnemonicsDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(RedefineMnemonicsDialog)
public:
    explicit RedefineMnemonicsDialog(QWidget *parent = 0);
    virtual ~RedefineMnemonicsDialog();

private:
    Ui::RedefineMnemonicsDialog *m_ui;

private:

private slots:
    void restoreDefaults();
    void setNonUnaryAllowedModes();

    void redefineNonUnaryMnemonic0(QString);
    void redefineNonUnaryMnemonic1(QString);
    void redefineNonUnaryMnemonic2(QString);
    void redefineNonUnaryMnemonic3(QString);

    void redefineUnaryMnemonic0(QString);
    void redefineUnaryMnemonic1(QString);
    void redefineUnaryMnemonic2(QString);
    void redefineUnaryMnemonic3(QString);
};

#endif // REDEFINEMNEMONICSDIALOG_H
