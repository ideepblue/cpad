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
#include "functionlistarea.h"

FunctionListArea::FunctionListArea(QWidget *parent,
        CodeEditor *editor):
    QDockWidget(parent)
{
    codeEditor = editor;
    functionListBox = new FunctionListBox(this);
    memoryCenter = new MemoryCenter();
    generalSetting();
}

void FunctionListArea::enableArea(bool enable)
{
    if (enable) {
        createFunctionList();
        show();
    }
    else
        close();
}

void FunctionListArea::cursorMove(int index)
{
    if (index == 0) return;
    int line = functionListBox->itemData(index).toInt(); 

    codeEditor->moveCursor(QTextCursor::End);
    QTextCursor tcr(
            codeEditor->document()->findBlockByNumber(line-1));
    codeEditor->setTextCursor(tcr);
}

void FunctionListArea::createFunctionList()
{
    functionListBox->close();
    functionListBox->clear();
    QString str(codeEditor->toPlainText());
    memoryCenter->analysisMain(str);
    memoryCenter->makeFunctionList(str);

    struct FunctionList *list = memoryCenter->getFunctionList();

    functionListBox->addItem(tr("<Please Select>"));
    for ( list = list->nextFunction; list; list = list->nextFunction) {
        functionListBox->addItem(list->functionOutput,
                list->functionLine);
    }
    functionListBox->show();
}

void FunctionListArea::generalSetting()
{
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout();
    QLabel *label = new QLabel(tr("函数列表:"), this);
    widget->setLayout(layout);
    layout->addWidget(label);
    layout->addWidget(functionListBox, 1);
    setTitleBarWidget(widget);
    setWidget(new QWidget());
    setFixedHeight(24);
    layout->setContentsMargins(5, 4, 5, 0);

    connect(functionListBox, 
                SIGNAL(activated(int)),
            this,
                SLOT(cursorMove(int))
           );
    connect(functionListBox, 
                SIGNAL(updateFunctionList()), 
            this, 
                SLOT(createFunctionList())
           );
    QFont afont(font());
    afont.setPointSize(10);
    setFont(afont);
    setWindowTitle(tr(""));
    setAllowedAreas(Qt::TopDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetClosable);
                //QDockWidget::DockWidgetVerticalTitleBar);
    QMainWindow *main = qobject_cast<QMainWindow *>(parent());
    main->addDockWidget(Qt::TopDockWidgetArea, this);
    hide();//默认关闭
}

