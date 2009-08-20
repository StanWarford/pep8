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
#include "pep.h"
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
    return QRectF(-32, -16, 64, 32) ;
}

void MemoryCellGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(boxColor);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(boxBgColor);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRoundedRect(QRectF(-32, -16, 64, 32), 2, 2, Qt::RelativeSize);

    painter->setPen(textColor);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setFont(QFont(Pep::codeFont, Pep::codeFontSize));

    painter->drawText(QRectF(-110, -16, 64, 32), Qt::AlignVCenter | Qt::AlignRight, QString("0x") + QString("%1").arg(address, 4, 16, QLatin1Char('0')).toUpper());
    painter->drawText(QRectF(-32, -16, 64, 32), Qt::AlignCenter, QString("%1").arg(value));
    painter->drawText(QRectF(48, -16, 64, 32), Qt::AlignVCenter | Qt::AlignLeft, QString("%1").arg(symbol));

}

