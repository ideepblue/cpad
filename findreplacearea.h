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
#ifndef FINDREPLACEAREA_H
#define FINDREPLACEAREA_H

#include <QDockWidget>
#include "codeeditor.h"

QT_BEGIN_NAMESPACE
class QHBoxLayout;
class QLineEdit;
class QPushButton;
class QCheckBox;
class QLabel;
class QGridLayout;
class QAction;
QT_END_NAMESPACE

class FindReplaceArea : public QDockWidget
{
        Q_OBJECT
    public:
        FindReplaceArea(QWidget *parent, CodeEditor *editor,
            QAction *find, QAction *replace);

    protected:
        void closeEvent(QCloseEvent *event);

    private slots:
        void transLayout(bool topLevel);//弹出窗时改变布局
        void openFindArea(bool open);//打开查找
        void openReplaceArea(bool open);//打开替换
        //下面增加搜索函数
        void highlightResultChecked(int state);//勾选高亮时调用
        void checkButtonToggled(int);//勾选匹配大小写和全字匹配时调用
        void runtimeSearchToggled(QString text); //在这里调用 runtimeSearch 进行实时查找与高亮
        void searchNext();
        void searchPrevious();
        void replaceAll();
        void replace();
        /////// 上面的函数是connect时候调用的函数，下面是他们的辅助函数。
        void clearAndHighlightAll(QString text);//被调用的高亮函数
        void clearAll();
        void runtimeSearch(QString text); //实时查找函数
        void userChangedText();//用户输入时候判断函数

    private:
        void createWidget();//创建控件
        void dockedLayout();//附着时布局
        void floatingLayout();//弹出时布局
        void generalSetting();//一般设置
        int inExtraSelection(int cursorPosition ); //判断所给的光标位置在不在 QList<QTextEdit::ExtraSelection> 里面

        QWidget     *findWidget, *replaceWidget;//中央控件
        QLabel      *findLabel;//查找标签
        QLabel      *replaceLabel;//替换标签
        QLineEdit   *findLineEdit;//查找输入栏
        QLineEdit   *replaceLineEdit;//替换输入栏
        QPushButton *previousButton, *nextButton, *closeButton,
                    *replaceButton, *replaceAllButton;
                    //向前、向后、关闭、替换按钮
        QCheckBox   *caseSensitive, *consistentMatch, 
                    *highLightResult;
                   //大小写、全字匹配、高亮复选
        QGridLayout *findLayout, *replaceLayout;//中央控件
        CodeEditor  *codeEditor; 
        QAction     *findAction, 
                    *replaceAction;//菜单栏
        static const int FindMode = 0, ReplaceMode = 1;//模式选择
        int currentMode;//当前模式

        QTextDocument::FindFlags findFlag;  //存放上一次查找时候的配置

};

#endif // FINDREPLACEAREA_H
