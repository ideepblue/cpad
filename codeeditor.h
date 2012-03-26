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
#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include "textblockdata.h"
#include "memorycenter.h"
#include "cpadpalette.h"

QT_BEGIN_NAMESPACE
class QWidget;
class QListWidget;
QT_END_NAMESPACE

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
    public:
        CodeEditor(QWidget *parent = 0);
        ~CodeEditor();

        void lineNumberAreaPaintEvent(QPaintEvent *event);//自定义行号区画图函数
        void functionFoldAreaPaintEvent(QPaintEvent *event);//自定义函数折叠区画图函数
        int lineNumberAreaWidth();//计算行号区宽度
        //获取语句块的顶部和底部block号
        void getTopAndBottom(int mouseY,int& topBlockNum,int& bottomBlockNum,bool& isNeededPaint);
        QPoint mousePosition;//当鼠标进入函数折叠区时的鼠标位置
        QWidget *lineNumberArea;//定义行标区域
        QWidget *functionFoldArea;//定义函数折叠区域
        void functionFold(int y);//函数折叠
        void functionUnfold(int y);//函数打开
        QTextBlock getMousePositionBlock(int mouseY);//获取鼠标所在block
        void keyPressEvent(QKeyEvent *event);//键盘按下事件
        QPoint getCompleXY();//得到blockY

    signals:
        void needAutoComple(int);//需要显示自动补全
        void fontChanged(QFont f);//改变的字体
        
    protected:
        void contextMenuEvent(QContextMenuEvent *event);//右键菜单 
        void resizeEvent(QResizeEvent *event);//为区域内的控件重新分配大小
        //void keyPressEvent(QKeyEvent *event);//键盘按下事件
        void paintEvent(QPaintEvent *event);

    private slots:
        void updateLineNumberAreaWidth(int newBlockCount);//更新行号区的宽度
        void highlightCurrentLine();//高亮当前行
        void updateLineNumberArea(const QRect &, int);//更新行号区
        //光标移动时候匹配括号
        void matchParentheses();
        void setWrapMode(bool isWraped);//设置是否自动换行
        void fontSettings();//设置编辑框字体
        void deleteSelectedText();//删除选中区域的文字
        void myUpdate();
        void recomposeContent(int code);//重排版
        void changeColorGroup(QList<QColor> group);
        void autoIndentAnalysis();//自动缩进分析
        void turnOnHighlightCurrentLine(bool);
        void doAutoIndent();//做自动缩进
        void functionComment();//函数标准注释
        void quickComment();//快速注释反注释

    private:
        void generalSetting();//基础设置   
        void readSettings();
        void writeSettings();
        // 括号匹配函数
        bool matchLeftParenthesis(QTextBlock currentBlock, int index, int numRightParentheses, char symbolLeft, char symbolRight);
        bool matchRightParenthesis(QTextBlock currentBlock, int index, int numLeftParentheses, char symbolLeft, char symbolRight);
        void createParenthesisSelection(int pos);
        int getInvisibleBlockNum(int blockNum);//获取某block之前的不可见block数量
        MemoryCenter *memoryCenter;//分析中心
        CpadPalette cpadPalette; //color theme
        bool matchForFolding(QTextBlock currentBlock, QTextBlock &leftBlock, QTextBlock &rightBlock);
        bool judgeLeftBracket(QTextBlock);
        bool switchOfHighlightCurrentLine;
        QAction *undoAction;//撤销菜单项
        QAction *redoAction;//重做菜单项
        QAction *cutAction;//剪切菜单项
        QAction *copyAction;//复制菜单项
        QAction *pasteAction;//粘贴菜单项
        QAction *selectAllAction;//全选菜单项
        QAction *deleteAction;//删除菜单项
        QMenu *contextMenu;//右键弹出菜单
        bool needPaint;
};

#endif // CODEEDITOR_H
