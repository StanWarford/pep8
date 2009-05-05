#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class HelpDialog;
}

class HelpDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(HelpDialog)
public:
    explicit HelpDialog(QWidget *parent = 0);
    virtual ~HelpDialog();

private:
    Ui::HelpDialog *m_ui;
};

#endif // HELPDIALOG_H
