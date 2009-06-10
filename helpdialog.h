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

    void selectItem(QString string);

private slots:
   void onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);

public:
   void machineLanguageClicked();
   void assemblyLanguageClicked();
   void breakPointsClicked();
   void symbolicTraceClicked();
   void byteConverterClicked();
   void interruptHandlersClicked();
   void instructionSetClicked();
   void dotCommandsClicked();
   void addressingModesClicked();
   void examplesClicked();
   void operatingSystemClicked();
   QString getLeftTextEditText();

signals:
   void clicked();
};

#endif // HELPDIALOG_H
