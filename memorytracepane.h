// File: memorytracepane.h
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

#ifndef MEMORYTRACEPANE_H
#define MEMORYTRACEPANE_H

#include <QtGui/QWidget>
#include <QGraphicsScene>
#include <QStack>
#include <QSet>
#include "memorycellgraphicsitem.h"
#include "enu.h"

namespace Ui {
    class MemoryTracePane;
}

class MemoryTracePane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(MemoryTracePane)
public:
    explicit MemoryTracePane(QWidget *parent = 0);
    virtual ~MemoryTracePane();

    void setMemoryTrace();
    // The memory trace is populated (on assembly).

    void setDebugState(bool b);
    // Post: if b, the trace pane is populated with globals and the stack frame
    // If !b, the pane is cleared. (yes/no?)

    void updateMemoryTrace();
    // Post: The memory trace is updated

    void cacheStackChanges();
    // Post: Stack changes are cached for the next time the simulation view is updated

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void setFont();
    // Post: the font used by the text edit is set to a font chosen in a font dialog

private:
    Ui::MemoryTracePane *m_ui;

    QGraphicsScene *scene;
    QStack<MemoryCellGraphicsItem *> globalVars;
    QStack<MemoryCellGraphicsItem *> runtimeStack;
    QPointF globalLocation;
    QPointF stackLocation;

    QMap<int, MemoryCellGraphicsItem *> addressToGlobalItemMap;
    QMap<int, MemoryCellGraphicsItem *> addressToStackItemMap;
    QList<int> modifiedBytesToBeUpdated;
    QSet<int> modifiedBytes;
    QList<int> bytesWrittenLastStep;
    bool delayLastStepClear;

    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

    static int cellSize(Enu::ESymbolFormat symbolFormat);

private slots:
    void zoomFactorChanged(int factor);

signals:
    void labelDoubleClicked(Enu::EPane pane);

};

#endif // MEMORYTRACEPANE_H
