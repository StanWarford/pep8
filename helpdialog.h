#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QtGui/QDialog>

#include <QTreeWidgetItem>

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

private slots:
// void QTreeWidget::itemClicked ( QTreeWidgetItem * item, int column )   [signal]
   void on_itemClicked(QTreeWidgetItem *, int);

};

#endif // HELPDIALOG_H
