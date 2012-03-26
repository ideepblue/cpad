/*
    This file is part of cPad Project.

    cPad is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cPad is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cPad.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QtGui>
#include "functionfoldarea.h"

/*
    鼠标移动事件：
        当鼠标移入本区域时，触发此事件。实时记录下当前鼠标的位置，并用repaint()函数强制触发PaintEvent事件
*/
void FunctionFoldArea::mouseMoveEvent(QMouseEvent *event)
{
    codeEditor->mousePosition.setX(event->x());
    codeEditor->mousePosition.setY(event->y());

    repaint();
}

void FunctionFoldArea::mousePressEvent(QMouseEvent *event)
{
    QTextBlock block = codeEditor->getMousePositionBlock(event->y());
    if(block.isValid())
    {
        while(block.isVisible() == false && block.blockNumber() != -1)
        {
            block = block.next();
        }
        if(block.blockNumber() == -1) return;

        if(block.next().isVisible()) codeEditor->functionFold(event->y());
        else codeEditor->functionUnfold(event->y());
    }
}
