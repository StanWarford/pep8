#ifndef TERMINALPANE_H
#define TERMINALPANE_H

#include <QtGui/QWidget>

namespace Ui {
    class TerminalPane;
}

class TerminalPane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(TerminalPane)
public:
    explicit TerminalPane(QWidget *parent = 0);
    virtual ~TerminalPane();

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void undo();
    // Post: the last action in the text edit is undone

    void redo();
    // Post: the last undo in the text edit is redone

    bool isUndoable();
    // Returns the undoability of the text edit

    bool isRedoable();
    // Returns the redoability of the text edit

    void cut();
    // Not allowed in this pane, interactive i/o

    void copy();
    // Post: selected text in the text edit is copied to the clipboard

    void paste();
    // Post: selected text in the clipboard is pasted to the end of the text edit

    void setFont();
    // Post: the font used by the text edit is set to a font chosen in a font dialog

private:
    Ui::TerminalPane *m_ui;

signals:
    void undoAvailable(bool);
    void redoAvailable(bool);

};

#endif // TERMINALPANE_H
