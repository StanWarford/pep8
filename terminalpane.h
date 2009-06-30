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

    bool isUndoable;
    // The undoability of the text edit is stored here

    bool isRedoable;
    // The redoability of the text edit is stored here

    void cut();
    // Not allowed in this pane, interactive i/o

    void copy();
    // Post: selected text in the text edit is copied to the clipboard

    void paste();
    // Post: selected text in the clipboard is pasted to the end of the text edit

private:
    Ui::TerminalPane *m_ui;

private slots:
    void setUndoability(bool b);
    void setRedoability(bool b);

signals:
    void undoAvailable(bool);
    void redoAvailable(bool);

};

#endif // TERMINALPANE_H
