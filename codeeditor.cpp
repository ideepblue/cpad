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
#include <QProcess>
#include "codeeditor.h"
#include "linenumberarea.h"
#include "functionfoldarea.h"
#include "cpadpalette.h"
#include "runcompile.h"

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    //added by lijunsong
    //this is for the find action
    //it's import, don't delete it!
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Active, QPalette::Highlight, palette.highlight());
    palette.setBrush(QPalette::Inactive, QPalette::Highlight, palette.highlight());
    palette.setColor(QPalette::Base, cpadPalette.defaultGroup[6]);
    palette.setColor(QPalette::Text, cpadPalette.defaultGroup[9]);
    this->setPalette(palette);
    // end by lijunsong
    //新建控件
    lineNumberArea = new LineNumberArea(this);
    functionFoldArea = new FunctionFoldArea(this);
    memoryCenter = new MemoryCenter();

    //事件处理
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), functionFoldArea, SLOT(update()));
    connect(this, SIGNAL(textChanged()), functionFoldArea, SLOT(repaint()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(matchParentheses()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(myUpdate()));

    //初始化
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    generalSetting();
    readSettings();
    mousePosition.setX(0);
    mousePosition.setY(0);
    needPaint = false;
}

void CodeEditor::turnOnHighlightCurrentLine(bool swi)
{
    switchOfHighlightCurrentLine = swi;
    highlightCurrentLine();
}

void CodeEditor::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab) {
        QTextCursor tcr = this->textCursor();//光标
        QTextBlock blk( tcr.block() );
        int pos = tcr.position() - blk.position();
        pos %= 4;
        pos = 4 - pos;
        QString str(pos, ' ');
        tcr.insertText(str);
        return;
    }
    else if (event->key() == Qt::Key_Return) {
        QPlainTextEdit::keyPressEvent(event);
        QSettings settings("5-up", "cPad");
        if ( ! settings.value("AutoIndentSwitch").toBool() )
           return; 
        QTextCursor tcr = this->textCursor();//光标
        QString str = this->toPlainText();
        //qDebug() << tcr.position();
        int num = memoryCenter->autoIndent(str, tcr.position());
        //qDebug() << num;
        QString in(num, ' ');
        tcr.insertText(in);
        this->setTextCursor(tcr);
        this->moveCursor(QTextCursor::Left);
        this->moveCursor(QTextCursor::Right);
        doAutoIndent();
        return;
    }
    else if (  event->key() == Qt::Key_BraceLeft
            || event->key() == Qt::Key_BraceRight
            || event->key() == Qt::Key_E
            || event->key() == Qt::Key_T ) {
        QPlainTextEdit::keyPressEvent(event);
        QSettings settings("5-up", "cPad");
        if ( ! settings.value("AutoIndentSwitch").toBool() )
           return; 
        autoIndentAnalysis();
        return;
    }
    else if (event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_N) {
        emit needAutoComple(1);
        return;
    }
    else if (event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_M) {
        quickComment();
    }
    else if ( event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_J )
    { //move down
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::Down);
        this->setTextCursor(cursor);
        return;
    }
    else if ( event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_H )
    {//move left
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::Left);
        this->setTextCursor(cursor);
        return;
    }
    else if ( event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_K )
    {//move up
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::Up);
        this->setTextCursor(cursor);
        return;
    }
    else if ( event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_L )
    {//move right
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::Right);
        this->setTextCursor(cursor);
        return;
    }
    else if ( event->modifiers( ) == Qt::AltModifier && event->key() == Qt::Key_D )
    { // delete current Line
        QTextCursor cursor(this->textCursor().block());
        cursor.movePosition(QTextCursor::StartOfBlock);
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        return;
    }
    else if ( event->modifiers( ) == Qt::AltModifier && event->key() == Qt::Key_Y )
    { // copy current Line
        QTextCursor cursor(this->textCursor().block());
        cursor.movePosition(QTextCursor::StartOfBlock);
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        QTextCursor old(this->textCursor());
        this->setTextCursor(cursor);
        this->copy();
        this->setTextCursor(old);
        return;
    }
    else if ( event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_P )
    { // paste
        this->paste();
        return;
    }
    else if ( event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_X )
    { // paste
        QTextCursor cursor(this->textCursor());
        if (cursor.atBlockEnd())
            cursor.deletePreviousChar();
        else
            cursor.deleteChar();
        this->setTextCursor(cursor);
        return;
    }
    else if(event->key() == Qt::Key_Backspace)
    {
        QTextBlock block = textCursor().block();
        //qDebug() << block.blockNumber();
        if(block.blockNumber() != 0 && (block.isVisible() == false || block.previous().isVisible() == false) &&
           textCursor().atBlockStart() == true)
        {
            if(block.isVisible()) block = block.previous();
            while(block.blockNumber() != -1 && block.isVisible() == false)
            {
                block = block.previous();
            }
            functionUnfold(blockBoundingGeometry(block).top()+3);
        }
    }
    QPlainTextEdit::keyPressEvent(event);
}

void CodeEditor::recomposeContent(int code)
{
    QString str(this->toPlainText());
    if (str == "   cPad" && code == 11) {
        this->selectAll();
        QTextCursor tcr = textCursor();
        tcr.insertText(tr("\n\n\n\n\n\n\n\n               维护人董帆、许航、李俊崧、黄文雄、蒋严泽\n                    请帮助贫困的中国程序猿！！"));
        tcr.movePosition(QTextCursor::Start);
        return;
    }
    memoryCenter->analysisMain(str);
    this->selectAll();
    QTextCursor tcr = textCursor();
    tcr.insertText(memoryCenter->relayout(code));
    tcr.movePosition(QTextCursor::Start);
}

CodeEditor::~CodeEditor()
{
    //析构时记录字体等数据
    writeSettings();
}

void CodeEditor::generalSetting()
{
    setWordWrapMode(QTextOption::WrapAnywhere);//设置词语Wrap为任意地方Wrap

    undoAction = new QAction(QIcon(":/resources/undo.png"), tr("撤销(&U)"), this);
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setStatusTip(tr("撤销上次的操作"));
    //默认设置不可用
    undoAction->setEnabled(false);
    connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));
    //编辑器可撤销信号与菜单项可用设置链接
    connect(this, SIGNAL(undoAvailable(bool)),
            undoAction, SLOT(setEnabled(bool)));

    redoAction = new QAction(QIcon(":/resources/redo.png"), tr("重做(&R)"), this);
    redoAction->setShortcut(QKeySequence::Redo);
    redoAction->setStatusTip(tr("重做上次的操作"));
    //默认设置不可用
    redoAction->setEnabled(false);
    connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));
    //编辑器可重做信号与菜单项可用设置链接
    connect(this, SIGNAL(redoAvailable(bool)),
            redoAction, SLOT(setEnabled(bool)));

    cutAction = new QAction(QIcon(":/resources/cut.png"), tr("剪切(&T)"), this);
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("剪切框选的内容至剪贴板"));
    //默认设置不可用
    cutAction->setEnabled(false);
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));
    //编辑器可复制信号与菜单项可用设置链接
    connect(this, SIGNAL(copyAvailable(bool)),
            cutAction, SLOT(setEnabled(bool)));

    copyAction = new QAction(QIcon(":/resources/copy.png"), tr("复制(&C)"), this);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("复制框选的内容至剪贴板"));
    //默认设置不可用
    copyAction->setEnabled(false);
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));
    //编辑器可复制信号与菜单项可用设置链接
    connect(this, SIGNAL(copyAvailable(bool)),
            copyAction, SLOT(setEnabled(bool)));

    pasteAction = new QAction(QIcon(":/resources/paste.png"), tr("粘贴(&P)"), this);
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("粘贴剪贴板中的内容至当前位置"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    selectAllAction = new QAction(QIcon(":/resources/selectall.png"), tr("全选(&A)"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setStatusTip(tr("将文本框中全部内容框选"));
    connect(selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));

    deleteAction = new QAction(QIcon(":/resources/delete.png"), tr("删除(&L)"), this);
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("删除框选的内容"));
    //默认设置不可用
    deleteAction->setEnabled(false);
    connect(deleteAction, SIGNAL(triggered()),
            this, SLOT(deleteSelectedText()));
    //编辑器可复制信号与菜单项可用设置链接
    connect(this, SIGNAL(copyAvailable(bool)),
            deleteAction, SLOT(setEnabled(bool)));
    contextMenu = new QMenu(this);
    contextMenu->addAction(undoAction);
    contextMenu->addAction(redoAction);
    contextMenu->addSeparator();
    contextMenu->addAction(cutAction);
    contextMenu->addAction(copyAction);
    contextMenu->addAction(pasteAction);
    contextMenu->addAction(deleteAction);
    contextMenu->addAction(selectAllAction);
}

void CodeEditor::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenu->exec(event->globalPos());
}

void CodeEditor::readSettings()
{
    QSettings settings("5-up", "cPad");

    //读取字体设置
    QFont font = settings.value("fontFamily", "Courier New").toString();
    int fontPointSize = settings.value("fontPiontSize", 11).toInt();
    font.setPointSize(fontPointSize);
    setFont(font);
    //设置 tab 宽度
    setTabStopWidth(4*fontMetrics().averageCharWidth());
}

void CodeEditor::writeSettings()
{
    QSettings settings("5-up", "cPad");

    //保存字体设置
    settings.setValue("fontFamily", font().family());
    settings.setValue("fontPiontSize", font().pointSize());
}

void CodeEditor::fontSettings()
{
    bool ok;

    //弹出对话框进行字体选择 成功选择后进行设置
    QFont tfont = QFontDialog::getFont(&ok, font(), this, tr("字体设置"));
    if (ok) {
        emit fontChanged(tfont);
        QFont afont(tfont.family(), tfont.pointSize());
        setFont(afont);
        //设置 tab 宽度
        setTabStopWidth(4*fontMetrics().averageCharWidth());

    }
}

//进行 Wrap 自动换行设置
void CodeEditor::setWrapMode(bool isWrap)
{
    if(isWrap)
        setLineWrapMode(QPlainTextEdit::WidgetWidth);
    else
        setLineWrapMode(QPlainTextEdit::NoWrap);
}

/*
    计算行号区宽度：
        获取最后一个block的编号，根据此编号计算出应设宽度，预留宽度为5个像素。
*/
int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 5 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

/*
    更新行号区宽度：
        设置编辑区的位置，其left坐标为行号区宽度加上函数折叠区宽度（20）。
*/
void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth()+20, 0, 0, 0);
}

/*
    更新行号区：
        利用scroll,update等函数触发paintEvent事件。
*/
void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

/*
    为区域内的控件重新分配大小:
        通过CodeEdit框的左上角顶点坐标计算行号区和函数折叠区的位置。
*/
void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
    functionFoldArea->setGeometry(QRect(cr.left()+lineNumberAreaWidth(), cr.top(), 20, cr.height()));
}

/*
    高亮当前行:
        获取光标所在行，并设置此行背景属性。
    <add by lijunsong> 注意：这里 append 的顺序不能颠倒，否则会影响到已经设置的extraSelection  
*/
void CodeEditor::highlightCurrentLine()
{
    /*QList<QTextEdit::ExtraSelection> extraSelection = extraSelections();
    CpadPalette cpadPalette;
    QColor lineColor = cpadPalette.defaultGroup[10];

    QList<QTextEdit::ExtraSelection> extraSelectionNew;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelectionNew.append(selection);
    }
    //过滤
    for (int i = 0; i < extraSelection.length(); i++)
    {
        if ( extraSelection[i].format.background() != lineColor )
            extraSelectionNew.append(extraSelection[i]);
    }

    setExtraSelections(extraSelectionNew);*/
    QList<QTextEdit::ExtraSelection> extraSelection = extraSelections();
    QTextEdit::ExtraSelection * selection;

    QColor lineColor = cpadPalette.defaultGroup[10];
    if(switchOfHighlightCurrentLine == false)
    {
        lineColor = cpadPalette.defaultGroup[6];
    }

    if ( extraSelection.size() != 0 )
    {
        selection = &extraSelection[0];
        selection->format.setBackground(lineColor);
        selection->format.setProperty(QTextFormat::FullWidthSelection, true);
        selection->cursor = textCursor();
        selection->cursor.clearSelection();
    }
    else
    {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelection.append(selection);
    }
    setExtraSelections(extraSelection);

}

/*
    自定义行号区画图函数:
        先为整个行号区画背景色，然后通过firstVisibleBlock()找到第一个可见block，再通过计算编辑框高度找
    到可见的底部block，最后从第一个可见block的编号开始，依次往下画出相应的行号。
*/
void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), cpadPalette.defaultGroup[6]);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(cpadPalette.defaultGroup[7]);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        //qDebug() << "top = " << top << "bottom = " << bottom;
        ++blockNumber;
    }
    //qDebug() << "                      ";
}

void CodeEditor::myUpdate()
{
    hide();
    show();
}

/*
    获取鼠标所在block：
        根据鼠标y坐标计算鼠标与第一个可见block的距离，然后再从以一个可见block开始外下数（不计不可见block）
*/
QTextBlock CodeEditor::getMousePositionBlock(int mouseY)
{
    QTextBlock block;
    block = firstVisibleBlock();
    while(block.blockNumber() != -1 && block.isVisible() == false)
    {
        block = block.next();
    }
    if(block.blockNumber() == -1) return block;
    int blockHeight = (int)blockBoundingRect(block).height();
    int blockNum = mouseY/blockHeight+1;

    if(blockNum <= blockCount())
    {
        for(int i = 1 ; i < blockNum ; i++)
        {
            if(block.blockNumber() == -1) return block;
            if(block.isVisible() == false) i--;
            block = block.next();
        }
        while(block.blockNumber() != -1 && block.isVisible() == false)
        {
            block = block.next();
        }
        return block;
    }
    else return block;
}

/*
    获取某block之前的不可见block数量
*/
int CodeEditor::getInvisibleBlockNum(int blockNum)
{
    QTextBlock block = firstVisibleBlock();
    int count = 0;
    for(int i = 1 ; i <= blockNum ; i++)
    {
        if(block.blockNumber() == -1) break;
        if(block.isVisible() == false)
        {
            i--;
            count++;
        }
        block = block.next();
    }

    return count;
}

/*
    函数折叠
*/
void CodeEditor::functionFold(int y)
{
    //int topBlockNum , bottomBlockNum;
    //bool isNeededPaint = false;
    //getTopAndBottom(y,topBlockNum,bottomBlockNum,isNeededPaint);
    QTextBlock blockNow = getMousePositionBlock(y);
    QTextBlock topBlock,bottomBlock;
    if(judgeLeftBracket(blockNow)) blockNow = blockNow.next();

    if(matchForFolding(blockNow,topBlock,bottomBlock))
    {
        //bool cursorMove = topBlock.blockNumber() >= firstVisibleBlock().blockNumber() ? false : true;
        bool cursorMove = true;
        if(topBlock.blockNumber() != 0 && judgeLeftBracket(topBlock.previous())) topBlock = topBlock.previous();
        //QTextBlock block = firstVisibleBlock();
        //topBlockNum += getInvisibleBlockNum(topBlockNum);
        //bottomBlockNum += getInvisibleBlockNum(bottomBlockNum);
        //topBlockNum += block.blockNumber()-1;
        //bottomBlockNum += block.blockNumber()-1;
        //while(block.previous().blockNumber() != -1 && block.previous().blockNumber() > topBlockNum)
        //{
          //  block = block.previous();
        //}

        if(cursorMove)
        {
            QTextCursor cursor = textCursor();
            QTextBlock block1 = cursor.block();
            do
            {
                if(block1.blockNumber() < topBlock.blockNumber())
                {
                    block1 = block1.next();
                    moveCursor(QTextCursor::NextBlock);
                }
                else
                {
                    block1 = block1.previous();
                    moveCursor(QTextCursor::PreviousBlock);
                }
            }while(block1.blockNumber() != topBlock.blockNumber() && textCursor().block().blockNumber() != 0);
        }

        if(topBlock.blockNumber() == 0 || judgeLeftBracket(topBlock.previous()) == false)
            topBlock = topBlock.next();
        if(topBlock.blockNumber() == -1) return;
        QTextBlock block = topBlock;
        do
        {
            block.setVisible(false);
            block = block.next();
            block.setVisible(false);
        }while(block.blockNumber() != -1 && block.blockNumber() < bottomBlock.blockNumber());
        myUpdate();
    }
}

/*
    函数打开
*/
void CodeEditor::functionUnfold(int y)
{
    QTextBlock block = getMousePositionBlock(y);
    if(block.isValid())
    {
        while(block.isVisible() == false && block.blockNumber() != -1)
        {
            block = block.next();
        }
        block = block.next();
        while(block.isVisible() == false && block.blockNumber() != -1)
        {
            block.setVisible(true);
            block = block.next();
        }
        myUpdate();
    }
}

bool CodeEditor::judgeLeftBracket(QTextBlock block)
{
    //if(block.blockNumber() == -1) return false;
    if(block.next().blockNumber() == -1) return false;
    block = block.next();
    for(int i = 0 ; i < block.text().length() ; i++)
    {
        if(block.text().at(i) != QChar(' ') && block.text().at(i) != QChar('{')) return false;
        else if(block.text().at(i) == QChar('{')) return true;
    }
    return false;
}

void CodeEditor::paintEvent(QPaintEvent *event)
{
    QPlainTextEdit::paintEvent(event);

    QTextBlock block = firstVisibleBlock();
    while(block.isVisible() == false && block.blockNumber() != -1)
    {
        block = block.next();
    }
    if(block.blockNumber() == -1) return;
    int blockHeight = (int)blockBoundingRect(block).height();
    int visibleBlockNum = rect().height()/blockHeight+1;
    int blockCounter = 1;
    QPainter painter(viewport());
    //painter.setPen(Qt::black);
    painter.setPen(cpadPalette.defaultGroup[9]);
    int littleChange = firstVisibleBlock().blockNumber() == 0 ? 4 : 0;
    while(block.blockNumber() != -1 && blockCounter <= visibleBlockNum)
    {
        if(block.isVisible() == false)
        {
            if(block.previous().text().indexOf('{') == -1)
                painter.drawText(QRect(block.previous().text().length()*fontMetrics().width('a')+30,
                                       (int)blockBoundingGeometry(block.previous()).top()+littleChange,
                                       fontMetrics().width('a')*5,fontMetrics().height()),
                                 Qt::AlignCenter,QString("{...}"));
            else painter.drawText(QRect(block.previous().text().length()*fontMetrics().width('a')+30,
                                        (int)blockBoundingGeometry(block.previous()).top()+littleChange,
                                        fontMetrics().width('a')*5,fontMetrics().height()),
                                  Qt::AlignCenter,QString("...}"));
            painter.drawRoundRect(QRect(block.previous().text().length()*fontMetrics().width('a')+25,
                                   (int)blockBoundingGeometry(block.previous()).top()+littleChange,
                                   fontMetrics().width('a')*5+10,fontMetrics().height()));
            while(block.blockNumber() != -1 && block.isVisible() == false)
            {
                block = block.next();
            }
        }
        else block = block.next();
        if(block.blockNumber() != -1 && block.isVisible()) blockCounter++;
    }

    if(needPaint)
    {
        needPaint = false;

        QBrush brush(QColor(200,230,255));
        brush.setStyle(Qt::Dense4Pattern);
        QPainter painter(viewport());

        QTextBlock block = getMousePositionBlock(mousePosition.y());
        //int blockHeight = (int)blockBoundingRect(block).height();
        /*int blockNum = mousePosition.y()/blockHeight+1;

        if(blockNum <= blockCount())
        {
            for(int i = 1 ; i < blockNum ; i++)
            {
                if(block.blockNumber() == -1) return;
                if(block.isVisible() == false) i--;
                block = block.next();
            }

            while(block.isVisible() == false && block.blockNumber() != -1)
            {
                block = block.next();
            }
        }
        else return;*/
        if(block.blockNumber() == -1) return;
        if(judgeLeftBracket(block)) block = block.next();

        QTextBlock leftBracketBlock,rightBracketBlock;
        if(matchForFolding(block,leftBracketBlock,rightBracketBlock))
        {
            int littleChange = firstVisibleBlock().blockNumber() == 0 ? 4 : 0;
            int bottomBlockPosition = blockBoundingGeometry(leftBracketBlock).bottom()+
                                      (rightBracketBlock.blockNumber()-leftBracketBlock.blockNumber())*
                                      blockBoundingGeometry(leftBracketBlock).height();
            painter.fillRect(QRect(0,littleChange,event->rect().width(),
                                   blockBoundingGeometry(leftBracketBlock).top()),brush);
            painter.fillRect(QRect(0,bottomBlockPosition+littleChange,
                                   event->rect().width(),
                                   event->rect().height()-bottomBlockPosition),
                             brush);
            painter.fillRect(QRect(0,(int)blockBoundingGeometry(leftBracketBlock).top()+littleChange,5,
                                   bottomBlockPosition-
                                   (int)blockBoundingGeometry(leftBracketBlock).top()),brush);
            painter.fillRect(QRect(event->rect().width()-5,
                                   (int)blockBoundingGeometry(leftBracketBlock).top()+littleChange,5,
                                   bottomBlockPosition-
                                   (int)blockBoundingGeometry(leftBracketBlock).top()),brush);
            myUpdate();
        }
        else return;
    }
}

/*
    自定义函数折叠区画图函数:
        语句块背景色：先为整个函数折叠区涂背景色，然后判断当鼠标在本区域上时，通过向getTopAndBottom函数传递当前鼠标y坐标
    参数，判断当前鼠标是否在某个语句块中，如果在，函数返回当前鼠标所在语句块的顶部block号和底部block号，通过
    这两个值计算当前语句块的起始y坐标和末尾y坐标，并高亮此区域；反之，则不做任何处理。
        语句块折叠标记：搜索当前可见的所有block，根据‘{’确定语句块折叠标记的位置。
*/
void CodeEditor::functionFoldAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(functionFoldArea);
    painter.fillRect(event->rect(), cpadPalette.defaultGroup[6]);

    if(mousePosition.x()>0 && mousePosition.x()<20)
    {
        QTextBlock block = getMousePositionBlock(mousePosition.y());
        if(block.blockNumber() != -1)
        {
            QTextBlock topBlock,bottomBlock;
            if(judgeLeftBracket(block)) block = block.next();

            if(matchForFolding(block,topBlock,bottomBlock))
            {
                needPaint = true;
                if(topBlock.blockNumber() != 0 && judgeLeftBracket(topBlock.previous()))
                    topBlock = topBlock.previous();
                //QTextBlock block = firstVisibleBlock();
                //int blockHeight = (int)blockBoundingRect(block).height();
                //int topPosition = (topBlockNum-1)*blockHeight;
                //int highlightHeight = bottomBlockNum*blockHeight-topPosition+3;
                QLinearGradient linearGrad(QPointF(1,(int)blockBoundingGeometry(topBlock).top()),
                                           QPointF(19,(int)blockBoundingGeometry(topBlock).top()));
                linearGrad.setColorAt(0,cpadPalette.defaultGroup[10]);
                linearGrad.setColorAt(1,cpadPalette.defaultGroup[10].lighter(150));
                QBrush brush(linearGrad);
                int bottomBlockPosition = blockBoundingGeometry(topBlock).bottom()+
                                          (bottomBlock.blockNumber()-topBlock.blockNumber())*
                                          blockBoundingGeometry(topBlock).height();
                painter.fillRect(QRect(1,(int)blockBoundingGeometry(topBlock).top(),18,
                                       bottomBlockPosition-(int)blockBoundingGeometry(topBlock).top()),brush);
                /*QTextDocument *allDocument = document();
                QTextCursor highlightCursor(allDocument);
                QTextCursor cursor(allDocument);
                cursor.beginEditBlock();

                QTextCharFormat plainFormat(highlightCursor.charFormat());
                QTextCharFormat colorFormat = plainFormat;
                colorFormat.setBackground(Qt::gray);

                int blockNum = mousePosition.y()/blockHeight+1;

                if(blockNum <= blockCount())
                {
                    for(int i = 1 ; i < blockNum ; i++)
                    {
                        if(block.blockNumber() == -1) return;
                        if(block.isVisible() == false) i--;
                        block = block.next();
                    }

                    while(block.isVisible() == false && block.blockNumber() != -1)
                    {
                        block = block.next();
                    }
                }
                else return;

                QTextBlock leftBracketBlock,rightBracketBlock;
                if(matchForFolding(block,leftBracketBlock,rightBracketBlock))
                {
                    while(highlightCursor.block().blockNumber() != leftBracketBlock.blockNumber())
                    {
                        highlightCursor.movePosition(QTextCursor::Down,QTextCursor::KeepAnchor);
                    }
                    highlightCursor.mergeCharFormat(colorFormat);
                }
                else return;*/
            }
        }
    }

    QTextBlock block = firstVisibleBlock();
    while(block.isVisible() == false && block.blockNumber() != -1)
    {
        block = block.next();
    }
    if(block.blockNumber() == -1) return;
    int blockHeight = (int)blockBoundingRect(block).height();
    int visibleBlockNum = rect().height()/blockHeight+1;
    int blockNum = 1;
    while(block.blockNumber() != -1 && blockNum <= visibleBlockNum)
    {
        //for(int i = 0 ; i < block.text().length() ; i++ )
        //{
          //  if(block.text().at(i) == QChar('{') )
            //{
              //  int bracketBlockNum = blockNum;
                //if((i == 0 || block.text().at(i-1) == (QChar)' ') && block.isVisible() == true)
                //{
                  //  bracketBlockNum--;
                //}
        QTextBlock topBlock,bottomBlock;
        if(matchForFolding(block,topBlock,bottomBlock) &&
           topBlock.blockNumber() >= firstVisibleBlock().blockNumber())
        {
            int foldPosition;
            if(topBlock.blockNumber() != 0 && topBlock.previous().isVisible() == false)
            {
                block = block.next();
                if(block.blockNumber() != -1 && block.isVisible()) blockNum++;
                continue;
            }
            if(topBlock.blockNumber() != 0 && judgeLeftBracket(topBlock.previous()))
                topBlock = topBlock.previous();
            foldPosition = (int)blockBoundingGeometry(topBlock).top();
                // = (bracketBlockNum-1)*blockHeight;
            painter.setPen(cpadPalette.defaultGroup[9]);
            int littleChange = firstVisibleBlock().blockNumber() == 0 ? 4 : 0;
                //if(judgeLeftBracket(block.previous()) == false && block.next().blockNumber() != -1)
                  //  block = block.next();
            if(topBlock.next().isVisible())
            {
                    //painter.drawText(0, foldPosition+littleChange, 20, fontMetrics().height(),
                      //           Qt::AlignCenter, QString("-"));
                QPointF points[3] = {
                    QPointF(12,foldPosition+littleChange+5),
                    QPointF(6,foldPosition+littleChange+11),
                    QPointF(12,foldPosition+littleChange+11)
                };
                painter.drawPolygon(points,3);
                QPointF points1[3] = {
                    QPointF(11,foldPosition+littleChange+6),
                    QPointF(7,foldPosition+littleChange+10),
                    QPointF(11,foldPosition+littleChange+10)
                };
                painter.drawPolygon(points1,3);
                QPointF points2[3] = {
                    QPointF(10,foldPosition+littleChange+7),
                    QPointF(8,foldPosition+littleChange+9),
                    QPointF(10,foldPosition+littleChange+9)
                };
                painter.drawPolygon(points2,3);
            }
            else
            {
                    //painter.drawText(0, foldPosition+littleChange, 20, fontMetrics().height(),
                      //           Qt::AlignCenter, QString("+"));
                QPointF points1[3] = {
                    QPointF(7,foldPosition+littleChange+4),
                    QPointF(7,foldPosition+littleChange+12),
                    QPointF(11,foldPosition+littleChange+8)
                };
                painter.drawPolygon(points1,3);
            }
            //}
            //block = bottomBlock;
            //blockNum += bottomBlock.blockNumber()-topBlock.blockNumber();
        }
        block = block.next();
        if(block.blockNumber() != -1 && block.isVisible()) blockNum++;
    }
}

/*
    获取语句块的顶部和底部block号:
        首先，找到鼠标当前所在的block，然后从此block往下搜索第一个未配对的‘}’，若找不到，直接返回；反之，记录下此底部block号以及
    '}'位置，再从此位置往上搜索第一个为配对的'{'，若找不到，直接返回；反之，记录下此顶部block号，最后，判断找到的目标'{'是否为本行
    除空格外的第一个字符，如果是，顶部block减去1，反之，不做任何处理（诸多编辑器如此）。
*/
void CodeEditor::getTopAndBottom(int mouseY , int& topBlockNum , int& bottomBlockNum , bool& isNeededPaint)
{
    /*QTextBlock block;
    block = firstVisibleBlock();
    int blockHeight = (int)blockBoundingRect(block).height();
    int blockNum = mouseY/blockHeight+1;

    if(blockNum <= blockCount())
    {
        for(int i = 1 ; i < blockNum ; i++)
        {
            if(block.blockNumber() == -1) return;
            if(block.isVisible() == false) i--;
            block = block.next();
        }

        while(block.isVisible() == false && block.blockNumber() != -1)
        {
            block = block.next();
        }

        int blockNumInSearch = blockNum;
        int brackets = 0;
        int bracketPosition;
        bool firstLeftBracket = false;
        bool stop = false;
        while(block.blockNumber() != -1 && !stop)
        {
            for(int i = 0 ; i < block.text().length() ; i++ )
            {
                if(block.text().at(i) == QChar('{') )
                {
                    if(!firstLeftBracket && (blockNumInSearch == blockNum ||
                                             (blockNumInSearch-1 == blockNum &&
                                              (i == 0 || block.text().at(i-1) == (QChar)' '))))
                    {
                        firstLeftBracket = true;
                        continue;
                    }
                    else brackets-- ;
                }
                else if(block.text().at(i) == QChar('}') )
                {
                    brackets++ ;
                    if ( brackets == 1 )
                    {
                        bottomBlockNum = blockNumInSearch;
                        bracketPosition = i;
                        stop = true;
                        break;
                    }
                }
            }
            if(!stop)
            {
                block = block.next();
                blockNumInSearch++;
            }
        }

        if(!stop) return;
        blockNumInSearch = bottomBlockNum;
        brackets = 0;
        stop = false;
        int invisibleBlockNum = 0;
        while(block.blockNumber() != -1 && !stop)
        {
            if(block.isVisible() == false) invisibleBlockNum++;
            int i;
            if(blockNumInSearch == bottomBlockNum) i = bracketPosition-1;
            else i = block.text().length()-1;
            for( ; i >= 0 ; i-- )
            {
                if(block.text().at(i) == QChar('}') ) brackets-- ;
                else if(block.text().at(i) == QChar('{') )
                {
                    brackets++ ;
                    if ( brackets == 1 )
                    {
                        if(i==0||block.text().at(i-1)==QChar(' ')) topBlockNum = blockNumInSearch-1;
                        else topBlockNum = blockNumInSearch;
                        stop = true;
                        break;
                    }
                }
            }
            if(!stop)
            {
                block = block.previous();
                blockNumInSearch--;
            }
        }

        if(!stop) return;
        if(blockNum<topBlockNum||blockNum>bottomBlockNum) return;

        isNeededPaint = true;
        bottomBlockNum -= invisibleBlockNum;
    }*/
}

/* 实现括号匹配 
 * 用户一边输入，一边储存括号。详见 syntaxhighlighter.cpp 
 * 这里实现的是对用户光标的移动进行括号匹配
 * */
void CodeEditor::matchParentheses()
{
    QList<QTextEdit::ExtraSelection> selections = extraSelections();
    QList<QTextEdit::ExtraSelection> selectionsNew;

    for (int i = 0; i < selections.length(); i++)
    {
        if ( selections[i].format.background() != cpadPalette.defaultGroup[11] ) //过滤掉以前匹配的括号，否则以前匹配好的括号背景不会消除
            selectionsNew.append( selections[i] );
    }
    setExtraSelections(selectionsNew);

    TextBlockData *data = static_cast<TextBlockData *>(textCursor().block().userData());

    if (data) {
        QVector<ParenthesisInfo *> infos = data->parentheses();

        int pos = textCursor().block().position();
        for (int i = 0; i < infos.size(); ++i) {
            ParenthesisInfo *info = infos.at(i);

            int curPos = textCursor().position() - textCursor().block().position();
            if (( info->position == curPos - 1 || info->position == curPos ) && info->character == '(')
            {
                if (matchLeftParenthesis(textCursor().block(), i + 1, 0, '(', ')'))
                    createParenthesisSelection(pos + info->position);
            } else if (( info->position == curPos - 1 || info->position == curPos ) && info->character == ')')
            {
                if (matchRightParenthesis(textCursor().block(), i - 1, 0, '(', ')'))
                    createParenthesisSelection(pos + info->position);
            }
            ////

            if (( info->position == curPos - 1 || info->position == curPos ) && info->character == '[')
            {
                if (matchLeftParenthesis(textCursor().block(), i + 1, 0, '[', ']'))
                    createParenthesisSelection(pos + info->position);
            } else if (( info->position == curPos - 1 || info->position == curPos ) && info->character == ']')
            {
                if (matchRightParenthesis(textCursor().block(), i - 1, 0, '[', ']'))
                    createParenthesisSelection(pos + info->position);
            }
            ////
            if (( info->position == curPos - 1 || info->position == curPos ) && info->character == '{')
            {

                if (matchLeftParenthesis(textCursor().block(), i + 1, 0, '{', '}'))
                    createParenthesisSelection(pos + info->position);
            } else if (( info->position == curPos - 1 || info->position == curPos ) && info->character == '}')
            {

                if (matchRightParenthesis(textCursor().block(), i - 1, 0, '{', '}'))
                    createParenthesisSelection(pos + info->position);
            }
        }
    }

}

/* 此函数能够顺序查找 userdata 里面的括号，实现括号匹配
 * currentBlock 是当前 block
 * i 是block中储存的userdata(一个 QVector)的下标
 * numLeftParentheses 用来查看找到的反括号是不是对应的括号,
 *     当numLeftParentheses==0则表明匹配到对应的括号了*/
bool CodeEditor::matchLeftParenthesis(QTextBlock currentBlock, int i, int numLeftParentheses, char symbolLeft, char symbolRight)
{
    TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());
    QVector<ParenthesisInfo *> infos = data->parentheses();
    int docPos = currentBlock.position();
    for (; i < infos.size(); ++i) {
        ParenthesisInfo *info = infos.at(i);
        if (info->character == symbolLeft) //遇到相同的括号，num++
        {
            ++numLeftParentheses;
            continue;
        }

        if ( info->character == symbolRight ) //遇到反括号，如果num还不是0，就num--；如果是0, 说明匹配上了。
        {
            if ( numLeftParentheses == 0)
            {
                createParenthesisSelection(docPos + info->position);
                return true;
            }
            else
            {
                numLeftParentheses--;
            }
        }
    }

    currentBlock = currentBlock.next();
    if (currentBlock.isValid())
        return matchLeftParenthesis(currentBlock, 0, numLeftParentheses, symbolLeft, symbolRight);

    return false;
}

/* 此函数能够顺序查找 userdata 里面的括号，实现括号匹配
 * currentBlock 是当前 block
 * i 是block中储存的userdata(一个 QVector)的下标
 * numLeftParentheses 用来查看找到的反括号是不是对应的括号,
 *     当numLeftParentheses==0则表明匹配到对应的括号了*/
bool CodeEditor::matchRightParenthesis(QTextBlock currentBlock, int i, int numRightParentheses, char symbolLeft, char symbolRight)
{
    TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());
    QVector<ParenthesisInfo *> parentheses = data->parentheses();

    int docPos = currentBlock.position();
    for (; i > -1 && parentheses.size() > 0; --i) {
        ParenthesisInfo *info = parentheses.at(i);
        if (info->character == symbolRight) {
            ++numRightParentheses;
            continue;
        }
        if ( info->character == symbolLeft )
        {
            if ( numRightParentheses == 0 )
            {
                createParenthesisSelection(docPos + info->position);
                return true;
            }
            else
            {
                numRightParentheses--;
            }
        }
    }

    currentBlock = currentBlock.previous();
    if (currentBlock.isValid())
    {
        TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());
        QVector<ParenthesisInfo *> parentheses = data->parentheses();
        int num = parentheses.size();  //get the QVector size of next block, this is important;
        return matchRightParenthesis(currentBlock, num - 1, numRightParentheses, symbolLeft, symbolRight);
    }
    return false;
}

/* 设置当前 QPlainTextEdit 的 extraSelection，
 * 将 pos 位置放到 extraSelection 中*/
void CodeEditor::createParenthesisSelection(int pos)
{
    QList<QTextEdit::ExtraSelection> selections = extraSelections();

    QTextEdit::ExtraSelection selection;
    QTextCharFormat format = selection.format;
    format.setBackground(cpadPalette.defaultGroup[11]);
    selection.format = format;

    QTextCursor cursor = textCursor();
    cursor.setPosition(pos);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    selection.cursor = cursor;
    selections.append(selection);

    setExtraSelections(selections);
}

void CodeEditor::deleteSelectedText()
{
    textCursor().removeSelectedText();
}

/*
 * 点击切换主题的时候，group传递进来当前用户选择的主题是哪一个
 */
void CodeEditor::changeColorGroup(QList<QColor>group)
{
    QPalette palette(this->palette());
    palette.setColor(QPalette::Base, group[6]);
    palette.setColor(QPalette::Text, group[9]);
    this->setPalette(palette);
    ///////////////////////////////////////////

    QList<QTextEdit::ExtraSelection> extraSelection = extraSelections();
    QList<QTextEdit::ExtraSelection> extraSelectionNew;

    for (int i = 0; i < extraSelection.length(); i++)
    {
        if ( extraSelection[i].format.background() != cpadPalette.defaultGroup[11] ) //过滤掉以前匹配的括号，否则以前匹配好的括号背景不会消除
            extraSelectionNew.append( extraSelection[i] );
    }

    QColor lineColor = group[10];
    if ( switchOfHighlightCurrentLine == false )
        lineColor = group[6];

    QTextEdit::ExtraSelection * selection = & extraSelectionNew[0];
    selection->format.setBackground(lineColor);
    setExtraSelections(extraSelectionNew);

    //切换当前的group
    cpadPalette.defaultGroup = group; //important
    matchParentheses();
}

bool CodeEditor::matchForFolding(QTextBlock currentBlock, QTextBlock &leftBlock, QTextBlock &rightBlock)
{
    if ( ! currentBlock.isValid() )
        return false;


    QTextBlock current = currentBlock;
    bool flag = false;
    int numCount = 0;
    //try to find "{"
    while ( true )
    {
        TextBlockData *data = static_cast<TextBlockData *>(current.userData());
        if ( data )
        {
            QVector<ParenthesisInfo *> infos = data->parentheses();

            for ( int i = 0; i < infos.size(); i++ )
            {
                ParenthesisInfo *info = infos.at(i);
                if ( info->character == '{')
                {
                    if ( numCount == 0)
                    {
                        leftBlock = current;
                        flag = true; //found it
                        break;
                    }
                    else
                        numCount--;
                }
                else if ( info->character == '}')
                    numCount++;
            }
            if ( flag == true )
                break;
            else  // not found on current block
            {
                current = current.previous();
                if ( ! current.isValid() )
                    break;
            }
        }
        else
        {
            current = current.previous();
            if ( ! current.isValid() )
                break;
        }
    }
    if ( flag == false ) // not found { on block before
        return false;

    //try to find "}"
    flag = false;
    current = currentBlock;
    numCount = 0;
    while ( true )
    {
        TextBlockData *data = static_cast<TextBlockData *>(current.userData());
        if ( data )
        {
            QVector<ParenthesisInfo *> infos = data->parentheses();
            for ( int i = 0; i < infos.size(); i++ )
            {
                ParenthesisInfo *info = infos.at(i);
                if ( info->character == '{' && current.blockNumber() != currentBlock.blockNumber())
                    numCount++;

                if ( info->character == '}' )
                {
                    if (numCount == 0 )
                    {
                        rightBlock = current;
                        flag = true;
                        break;
                    }
                    else
                        numCount--;
                }
            }
            if ( flag == true )
                break;
            else
            {
                current = current.next();
                if ( ! current.isValid() )
                    break;
            }
        }
        else
        {
            current = current.next();
            if ( ! current.isValid() )
                break;
        }
    }
    if ( flag == false )
        return false;

    return true;
}

//void CodeEditor::autoCompleCheck()
//{
    //QString str = textCursor().block().text();
    //if (str.endsWith(" ")) {
        //emit showAutoComple(false);
        //return;
    //}
    //emit showAutoComple(true);
//}

void CodeEditor::autoIndentAnalysis()
{
    if (!textCursor().atBlockEnd())
        return;
    QString textLine(textCursor().block().text());
    LineHeadPointer * memoryHeadTemp;
    memoryCenter->analysisMain ( textLine );
    memoryHeadTemp = memoryCenter->getLineHead ( );
    if (memoryHeadTemp->nextLine)
        if (memoryHeadTemp->nextLine->nextLine != NULL || memoryHeadTemp->nextLine->firstWord->nextWord != NULL)
            return;//分析是否一个词
    doAutoIndent();
}

void CodeEditor::doAutoIndent()
{
    LineHeadPointer * memoryHeadTemp;
    int changeMode;
    changeMode = 5;
    QString textLine(textCursor().block().text());
    memoryCenter->analysisMain ( textLine );
    memoryHeadTemp = memoryCenter->getLineHead ( );
    if ( memoryHeadTemp -> nextLine )
    {
        if ( memoryHeadTemp -> nextLine -> firstWord -> wordCategory == 2 )
            changeMode = 0;
        else if ( memoryHeadTemp -> nextLine -> firstWord -> wordCategory == 3 )
            changeMode = 1;
        else if ( memoryHeadTemp -> nextLine -> firstWord -> wordCategory == 62 )
            changeMode = 2;
        else if ( memoryHeadTemp -> nextLine -> firstWord -> wordCategory == 55 )
            changeMode = 3;
        else if ( memoryHeadTemp -> nextLine -> firstWord -> wordCategory == 56 )
            changeMode = 4;
    }
    int changeNum = 0;
    QTextCursor tcrBegin = textCursor();
    tcrBegin.movePosition(QTextCursor::StartOfBlock);
    QString textAll = this->toPlainText();
    if ( changeMode < 5 )
        changeNum = 
            memoryCenter->changeIndent(textAll, textLine, tcrBegin.position(), changeMode);
    if (changeNum == 0)
        return;
    else if (changeNum > 0) {
        QString insertStr(changeNum, ' ');
        tcrBegin.insertText(insertStr);
        this->moveCursor(QTextCursor::Left);
        this->moveCursor(QTextCursor::Right);
        return;
    }
    else {
        tcrBegin.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, -changeNum);
        tcrBegin.removeSelectedText();
        this->moveCursor(QTextCursor::Left);
        this->moveCursor(QTextCursor::Right);
        return;
    }
}

void CodeEditor::functionComment()
{
    QString remarkResult, codeText;
    QTextCursor tcr( textCursor() );
    QTextBlock blk( tcr.block() );
    for ( int i = 0; i < 10; i++ ) {
        if (!blk.isValid())
            break;
        codeText.append(blk.text());
        codeText.append('\n');
        blk = blk.next();
    }
    if (codeText.isEmpty())
        return;
    memoryCenter -> analysisMain ( codeText );//参数为至少2行，如果想表现的比较强大，可以考虑6-10行，否则2-5行，显得不太智能。
    memoryCenter -> makeFunctionList ( codeText );//参数跟上边的一样
    remarkResult = memoryCenter -> makeFunctionRemark ( );//remarkResult就是标准注释，如果他使用命令的位置不是函数头，或者函数写的有问题，返回的是空串
    if (remarkResult.isEmpty())
        return;
    tcr.movePosition(QTextCursor::StartOfBlock);
    tcr.insertText(remarkResult);
}

QPoint CodeEditor::getCompleXY()
{
    //return blockBoundingGeometry(blk);
    QTextCursor tcr = textCursor();
    QTextBlock blk = textCursor().block();
    int a = (int) blockBoundingGeometry(blk).translated(contentOffset()).top();
    
    QPoint p = mapToGlobal(pos());

    int height = (int) blockBoundingRect(blk).height();
    int width = textCursor().columnNumber() * fontMetrics().averageCharWidth();

    return QPoint(p.x()+width+lineNumberAreaWidth()+20, a + p.y() + height);
}

void CodeEditor::quickComment()
{
    //快速注释反注释
    QTextCursor tcr(textCursor());
    QString line(tcr.block().text());
    QRegExp reg("\\s*//");
    if (reg.indexIn(line) == 0 ) {
        //已经有注释
        int num = line.indexOf("//");
        line.remove(num, 2);
        tcr.movePosition(QTextCursor::StartOfBlock);
        tcr.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        tcr.insertText(line);
    }
    else  {
        int length = line.length();
        int num = 0;
        for (; num < length; num ++ )
            if (line[num] != ' ' )
                break;
        line.insert(num, "//");
        tcr.movePosition(QTextCursor::StartOfBlock);
        tcr.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        tcr.insertText(line);
    }
    return;
}
