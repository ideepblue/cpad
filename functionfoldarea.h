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
#ifndef FUNCTIONFOLDAREA_H
#define FUNCTIONFOLDAREA_H

#include <QWidget>
#include "codeeditor.h"
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class FunctionFoldArea : public QWidget
{
        Q_OBJECT
    public:
        FunctionFoldArea(CodeEditor *editor) : QWidget(editor) {
            codeEditor = editor;//与edit框通信
            setMouseTracking(true);//讲mouseMoveEvent事件变为移动鼠标就触发（默认为按下左键并移动才触发）
        }

        void mouseMoveEvent(QMouseEvent *event);//鼠标移动事件

    protected:
        void paintEvent(QPaintEvent *event) {//控件重画事件
            codeEditor->functionFoldAreaPaintEvent(event);
        }

        void leaveEvent(QEvent *event)//鼠标离开控件事件
        {
            codeEditor->mousePosition.setX(-1);
            repaint();
        }

    private:
        CodeEditor *codeEditor;
        void mousePressEvent(QMouseEvent *event);//点击鼠标事件
};

#endif // FUNCTIONFOLDAREA_H
