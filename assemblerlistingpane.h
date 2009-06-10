#ifndef ASSEMBLERLISTINGPANE_H
#define ASSEMBLERLISTINGPANE_H

#include <QtGui/QWidget>

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

    void setModified(bool modified);
    // Post: Sets isModified of the assembler listing pane to modified

    QString toPlainText();
    // Post: Contents of the source code pane are returned

    void setCurrentFile(QString string);
    // Post: Title bar of the window is set to "Assembler Listing - 'string'"

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

private:
    Ui::AssemblerListingPane *m_ui;
};

#endif // ASSEMBLERLISTINGPANE_H
