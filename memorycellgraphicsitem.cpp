// File: memorycellgraphicsitem.cpp
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

#include "memorycellgraphicsitem.h"
#include <QPainter>

MemoryCellGraphicsItem::MemoryCellGraphicsItem(int addr, int val, QString sym)
{
    address = addr;
    value = val;
    symbol = sym;
    boxColor = Qt::black;
    boxBgColor = Qt::white;
    textColor = Qt::black;
}

QRectF MemoryCellGraphicsItem::boundingRect() const
{
    const int Margin = 1;
    return QRectF(-32, -16, 64, 32) ;
}

void MemoryCellGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(boxColor);
    painter->setPen(pen);
    painter->setBrush(boxBgColor);

    painter->drawRect(-32, -16, 64, 32);
    painter->setPen(textColor);
    painter->drawText(QRectF(-32, -16, 64, 32), Qt::AlignCenter, QString("%1").arg(value));
}

