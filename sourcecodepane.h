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
    // Pre: The source code pane contains a Pep/8 source program.
    // Post: If the program assembles correctly, true is returned, and codeList is populated
    // with the code objects. Otherwise false is returned and codeList is partially populated.

    QList<int> getObjectCode();
    // Pre: codeList is populated with code from a complete correct Pep/8 source program.
    // Post: objectCode is populated with the object code, one byte per entry, and returned.

    QStringList getAssemblerListingList();
    // Pre: codeList is populated with code from a complete correct Pep/8 source program.
    // Post: assemlberListingList is populated with the assemlber listing, including headers
    // and a text printout of the symbol table at the bottom, listingTraceList is populated with
    // the object code, and hasCheckBox is populated with the checkBox list that specifies whether
    // a trace line can have a break point.
    // Post: assemblerListingList is returned.

    QStringList getListingTraceList();
    // Pre: listingTraceList is populated.
    // Post: ListingTraceList is returned.

    QList<bool> getHasCheckBox();
    // Pre: hasCheckBox is populated.
    // Post: hasCheckBox is returned.

    void removeErrorMessages();
    //
    void appendMessageInSourceCodePaneAt(int lineNumber, QString message, Qt::GlobalColor color);
    //
    void setSourceCodePaneText(QString string);
    // Post: Sets text in source code pane to string.
    // Called from help dialog by helpCopyToSourceButton clicked.
    void clearSourceCodePane();
    // Post: Clears the source code pane
    // Called from main window on file new
    bool isModified();
    // Post: Returns true if the source code pane has been modified
    // Called from main window on file new
    void setModified(bool modified);
    // Post: Sets isModified of the source code pane to modified
    // Called from main window on file new
    QString toPlainText();
    // Post: Contents of the source code pane are returned
    // Called from main window on file save

private:
    Ui::SourceCodePane *m_ui;
    QList<Code *> codeList;
    QList<int> objectCode;
    QStringList assemblerListingList;
    QStringList listingTraceList;
    QList<bool> hasCheckBox;

};

#endif // SOURCECODEPANE_H
