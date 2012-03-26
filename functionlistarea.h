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
#ifndef FUNCTIONLISTAREA_H
#define FUNCTIONLISTAREA_H

#include <QDockWidget>
#include <QComboBox>
#include "codeeditor.h"
#include "memorycenter.h"
//#include "structdefine.h"

class FunctionListBox : public QComboBox
{
        Q_OBJECT
    public:
        FunctionListBox(QWidget *parent):QComboBox(parent){}
    signals:
        void updateFunctionList();
        
    protected:
        void mousePressEvent(QMouseEvent *event) {
            emit updateFunctionList();
            QComboBox::mousePressEvent(event);
        }
};

class FunctionListArea : public QDockWidget
{
        Q_OBJECT
    public:
        FunctionListArea(QWidget *parent, CodeEditor *editor);

    private slots:
        void cursorMove(int index);
        void enableArea(bool enable);
        void createFunctionList();

    private:
        void generalSetting();

        CodeEditor *codeEditor;
        FunctionListBox *functionListBox;
        MemoryCenter *memoryCenter;
};


#endif // FUNCTIONLISTAREA_H
