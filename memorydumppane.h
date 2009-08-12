/*
    Pep8-1 is a virtual machine for writing machine language and assembly
    language programs.
    
    Copyright (C) 2009  J. Stanley Warford, Pepperdine University

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MEMORYDUMPPANE_H
#define MEMORYDUMPPANE_H

#include <QtGui/QWidget>
#include <QScrollBar>
#include <QSet>

namespace Ui {
    class MemoryDumpPane;
}

class MemoryDumpPane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(MemoryDumpPane)
public:
    explicit MemoryDumpPane(QWidget *parent = 0);
    virtual ~MemoryDumpPane();

    void refreshMemory();
    // Post: the entire memory pane is refreshed

    void refreshMemoryLines(int firstByte, int lastByte);
    // Post: The memory dump is refresed from the line containing startByte to the line
    // containing endByte. Called by load().

    void highlightMemory(bool b);
    // Post: Everything is unhighlighted. If b, current program counter is highlighted.

    void cacheModifiedBytes();
    // Post: Changed bytes from Sim are added to the QSet modifiedBytes

    void updateMemory();
    // Post: Memory displays are updated using the changedMemoryAddrss qlist in sim

    void scrollToTop();
    // Post: Memory dump is scrolled to the top left corner

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void copy();
    // Post: selected text in the text edit is copied to the clipboard

    void setFont();
    // Post: the font used by the text edit is set to a font chosen in a font dialog

    int memoryDumpWidth();
    // Post: the width of the memory dump text edit document is returned

    QSize sizeHint();

private:
    Ui::MemoryDumpPane *m_ui;

    void highlightByte(int memAddr, QColor foreground, QColor background);

    void mouseReleaseEvent(QMouseEvent *);

    QList<int> highlightedData;
    QSet<int> modifiedBytes;
    QList<int> bytesWrittenLastStep;

private slots:
    void on_pepMemRefreshButton_clicked();
};

#endif // MEMORYDUMPPANE_H
