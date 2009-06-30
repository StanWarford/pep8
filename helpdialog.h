#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QtGui/QDialog>
#include <QTreeWidgetItem>
#include "pephighlighter.h" // For syntax highlighting

namespace Ui {
    class HelpDialog;
}

class HelpDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(HelpDialog)
        public:
            explicit HelpDialog(QWidget *parent = 0);
    virtual ~HelpDialog();

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

private:
    Ui::HelpDialog *m_ui;

    void selectItem(QString string);
    PepHighlighter *pepHighlighter;

    enum Row {
        eWRITING = 0,
        eDEBUGGING = 1,
        eINTERRUPT = 2,
        eREFERENCE = 3,
        eEXAMPLES = 4,
        eOS = 5,

        eASSEMBLY = 1,
        eMACHINE = 0,

        eBREAK = 0,
        eSYMTRACE = 1,
        eBYTECONVERTER = 2,

        eINSTRUCTION = 0,
        eDOTCMD = 1,
        eADDRMODE = 2,

        eFIG518 = 0,
        eFIG521 = 1,
        eFIG526 = 2,
        eFIG601 = 3,
        eFIG604 = 4,
        eFIG606 = 5,
        eFIG608 = 6,
        eFIG610 = 7,
        eFIG612 = 8,
        eFIG614 = 9,
        eFIG616 = 10,
        eFIG618 = 11,
        eFIG621 = 12,
        eFIG623 = 13,
        eFIG625 = 14,
        eFIG627 = 15,
        eFIG629 = 16,
        eFIG632 = 17,
        eFIG634 = 18,
        eFIG636 = 19,
        eFIG638 = 20,
        eFIG640 = 21,
        eFIG641 = 22,
        eFIG643 = 23,
        eFIG645 = 24,
        eFIG647 = 25,
    };

private slots:
    void onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);

signals:
    void clicked();
};

#endif // HELPDIALOG_H
