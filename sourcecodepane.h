#ifndef SOURCECODEPANE_H
#define SOURCECODEPANE_H

#include <QtGui/QWidget>
#include <QString>
#include <QList>
#include "asm.h"

namespace Ui {
    class SourceCodePane;
}

class SourceCodePane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(SourceCodePane)
public:
    explicit SourceCodePane(QWidget *parent = 0);
    virtual ~SourceCodePane();
    bool assemble();
    QList<int> getObjectCode();
    QStringList getAssemblerListingList();
    QStringList getListingTraceList();
    QList<bool> getHasCheckBox();
    void removeErrorMessages();

private:
    Ui::SourceCodePane *m_ui;
    QList<int> objectCode;
    QStringList assemblerListingList;
    QStringList listingTraceList;
    QList<bool> hasCheckBox;

private:
    void appendMessageInSourceCodePaneAt(int lineNumber, QString message, Qt::GlobalColor color);
};

#endif // SOURCECODEPANE_H
