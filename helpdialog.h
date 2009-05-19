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
   void on_itemClicked(QTreeWidgetItem*,int);
//   void on_currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);

public slots:
   void on_menuWritingPrograms_clicked();
   void on_menuMachineLanguage_clicked();
   void on_menuAssemblyLanguage_clicked();
   void on_menuDebuggingPrograms_clicked();
   void on_menuBreakPoints_clicked();
   void on_menuSymbolicTrace_clicked();
   void on_menuByteConverter_clicked();
   void on_menuInterruptHandlers_clicked();
   void on_menuReference_clicked();
   void on_menuInstructionSet_clicked();
   void on_menuDotCommands_clicked();
   void on_menuAddressingModes_clicked();
   void on_examples_clicked();
   void on_operatingSystem_clicked();
};

#endif // HELPDIALOG_H
