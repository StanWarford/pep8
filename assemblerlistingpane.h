#ifndef ASSEMBLERLISTINGPANE_H
#define ASSEMBLERLISTINGPANE_H

#include <QtGui/QWidget>
#include "pephighlighter.h"

namespace Ui {
    class AssemblerListingPane;
}

class AssemblerListingPane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(AssemblerListingPane)
public:
    explicit AssemblerListingPane(QWidget *parent = 0);
    virtual ~AssemblerListingPane();
    void setAssemblerListing(QStringList assemblerListingList);
    void clearAssemblerListing();

    bool isModified();
    // Post: Returns true if the assembler listing pane has been modified

    QString toPlainText();
    // Post: Contents of the source code pane are returned

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void undo();
    // Post: does nothing in this pane

    void redo();
    // Post: does nothing in this pane

    void cut();
    // Does nothing in our implementation of the assembler listing

    void copy();
    // Does nothing in our implementation of the assembler listing

    void paste();
    // Does nothing in our implementation of the assembler listing

    void setFont();
    // Post: the font used by the text edit is set to a font chosen in a font dialog

private:
    Ui::AssemblerListingPane *m_ui;

    PepHighlighter *pepHighlighter;
};

#endif // ASSEMBLERLISTINGPANE_H
