#ifndef REDEFINEMNEMONICSDIALOG_H
#define REDEFINEMNEMONICSDIALOG_H

#include <QtGui/QDialog>

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
};

#endif // REDEFINEMNEMONICSDIALOG_H
