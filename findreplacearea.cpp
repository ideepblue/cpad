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
#include "findreplacearea.h"
#include "cpadpalette.h"

FindReplaceArea::FindReplaceArea(QWidget *parent, CodeEditor *editor,
        QAction *find, QAction *replace):
    QDockWidget(parent)
{
    codeEditor = editor;
    findAction = find;
    replaceAction = replace;

    createWidget();
    generalSetting();

    findFlag = 0;  // for the check box;

}

void FindReplaceArea::closeEvent(QCloseEvent *event)
{
    replaceAction->setChecked(false);
    findAction->setChecked(false);
    event->accept();
}

void FindReplaceArea::createWidget()
{
    //中央控件和布局控件
    findWidget = new QWidget();
    replaceWidget = new QWidget();
    findLayout = new QGridLayout();
    findWidget->setLayout(findLayout);
    replaceLayout = new QGridLayout();
    replaceWidget->setLayout(replaceLayout);
    findLabel = new QLabel(tr("查找内容(&S):"));
    replaceLabel = new QLabel(tr("　替换为(&P):"));
    findLineEdit = new QLineEdit();
    replaceLineEdit = new QLineEdit();
    previousButton = new QPushButton(QIcon(":/resources/previous.png"), 
                        tr("上一个(&U)"));
    nextButton     = new QPushButton(QIcon(":/resources/next.png"), 
                        tr("下一个(&D)"));
    closeButton    = new QPushButton(tr("关闭(&X)"));
    replaceButton  = new QPushButton(tr("替换(&R)"));
    replaceAllButton = new QPushButton(tr("替换全部(&A)"));
    caseSensitive   = new QCheckBox(tr("区分大小写(&C)"));
    consistentMatch = new QCheckBox(tr("全字匹配(&W)"));
    highLightResult = new QCheckBox(tr("高亮结果(&L)"));
}

void FindReplaceArea::transLayout(bool topLevel)
{
    if (topLevel) {
        floatingLayout();
    }
    else {
        dockedLayout();
    }
}

void FindReplaceArea::openFindArea(bool open)
{
    if (open) {
        replaceAction->setChecked(false);
        findAction->setChecked(true);
        currentMode = FindMode;
        transLayout(isFloating());
        show();
    }
    else {
        close();
        clearAll();
    }
    findLineEdit->setFocus();
}

void FindReplaceArea::openReplaceArea(bool open)
{
    if (open) {
        findAction->setChecked(false);
        replaceAction->setChecked(true);
        currentMode = ReplaceMode;
        transLayout(isFloating());
        show();
    }
    else {
        close();
        clearAll();
    }
    findLineEdit->setFocus();
}

void FindReplaceArea::floatingLayout()
{
    if (currentMode == FindMode) {
        setWidget(findWidget);//中心区域为查找

        setWindowTitle(tr("查找"));

        findLineEdit->setFixedSize(195, 22);

        findLayout->addWidget(findLabel, 0, 0);
        findLayout->addWidget(findLineEdit, 0, 1, 1, 2);
        findLayout->addWidget(previousButton, 0, 3);
        findLayout->addWidget(nextButton, 0, 4);
        findLayout->addWidget(caseSensitive, 1, 1);
        findLayout->addWidget(consistentMatch, 1, 2);
        findLayout->addWidget(highLightResult, 1, 3);
        findLayout->addWidget(closeButton, 1, 4);
        findLayout->setContentsMargins(11, 2, 11, 0);

        //设置固定大小
        setFixedSize(480, 80);
    }
    else {
        setWidget(replaceWidget);//中心区域为替换
        setWindowTitle(tr("替换"));

        findLineEdit->setFixedSize(195, 22);
        replaceLineEdit->setFixedSize(195, 22);

        replaceLayout->addWidget(findLabel, 0, 0);
        replaceLayout->addWidget(findLineEdit, 0, 1, 1, 2);
        replaceLayout->addWidget(previousButton, 0, 3);
        replaceLayout->addWidget(nextButton, 0, 4);
        replaceLayout->addWidget(replaceLabel, 1, 0);
        replaceLayout->addWidget(replaceLineEdit, 1, 1, 1, 2);
        replaceLayout->addWidget(replaceButton, 1, 3);
        replaceLayout->addWidget(replaceAllButton, 1, 4);
        replaceLayout->addWidget(caseSensitive, 2, 1);
        replaceLayout->addWidget(consistentMatch, 2, 2);
        replaceLayout->addWidget(highLightResult, 2, 3);
        replaceLayout->addWidget(closeButton, 2, 4);
        replaceLayout->setContentsMargins(11, 2, 11, 0);
        //设置固定大小
        setFixedSize(480, 105);
    }
}

void FindReplaceArea::dockedLayout()
{
    setWindowTitle(tr(""));//附着时不用标题
    
    if (currentMode == FindMode) {
        setWidget(findWidget);//中心区域为查找
        findLineEdit->setFixedSize(150, 22);

        findLayout->addWidget(findLabel, 0, 0);
        findLayout->addWidget(findLineEdit, 0, 1);
        findLayout->addWidget(previousButton, 0, 2);
        findLayout->addWidget(nextButton, 0, 3);
        findLayout->addWidget(caseSensitive, 0, 5);
        findLayout->addWidget(consistentMatch, 0, 4);
        findLayout->addWidget(highLightResult, 1, 4);
        findLayout->setContentsMargins(7, 2, 11, 7);
        findLayout->setRowMinimumHeight(1, 23);
        findLayout->addWidget(closeButton, 1, 5);
        //设置固定大小
        setFixedSize(650, 65);
    }
    else {
        setWidget(replaceWidget);//中心区域为替换
        findLineEdit->setFixedSize(150, 22);
        replaceLineEdit->setFixedSize(150, 22);

        replaceLayout->addWidget(findLabel, 0, 0);
        replaceLayout->addWidget(findLineEdit, 0, 1);
        replaceLayout->addWidget(previousButton, 0, 2);
        replaceLayout->addWidget(nextButton, 0, 3);
        replaceLayout->addWidget(replaceLabel, 1, 0);
        replaceLayout->addWidget(replaceLineEdit, 1, 1, 1, 2);
        replaceLayout->addWidget(replaceButton, 1, 2);
        replaceLayout->addWidget(replaceAllButton, 1, 3);
        replaceLayout->addWidget(caseSensitive, 0, 5);
        replaceLayout->addWidget(consistentMatch, 0, 4);
        replaceLayout->addWidget(highLightResult, 1, 4);
        replaceLayout->setContentsMargins(7, 2, 11, 7);
        replaceLayout->addWidget(closeButton, 1, 5);
        //设置固定大小
        setFixedSize(650, 65);
    }
}
    
void FindReplaceArea::generalSetting()
{
    //设置 Dock 可移动区域为上下
    setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    //查找栏 可关 可移 可浮出 竖向标题
    setFeatures(QDockWidget::DockWidgetClosable | 
                QDockWidget::DockWidgetMovable |
                QDockWidget::DockWidgetFloatable |
                QDockWidget::DockWidgetVerticalTitleBar
                );
    //Dock[ Widget[ Layout[ ... ] ] ] 
    //设置中央控件
    //设置label的伙伴(Buddy),作用：
    //当按下label快捷键时，findLineEdit将获取到焦点
    findLabel->setBuddy(findLineEdit);
    replaceLabel->setBuddy(replaceLineEdit);
    setWindowTitle(tr("查找和替换"));//附着时不用标题
    //任意付个初值
    currentMode = FindMode;

    QMainWindow *main = qobject_cast<QMainWindow *>(parent());
    main->addDockWidget(Qt::BottomDockWidgetArea, this);
    hide();

    connect(findAction, SIGNAL(toggled(bool)), 
            this, SLOT(openFindArea(bool)));
    connect(replaceAction, SIGNAL(toggled(bool)), 
            this, SLOT(openReplaceArea(bool)));

    connect(this, SIGNAL(topLevelChanged(bool)), 
            this, SLOT(transLayout(bool)));
    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(close()));

    //实时高亮
    connect(findLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(runtimeSearchToggled(QString)));
    //点击下一个，上一个时调用的函数
    connect(nextButton, SIGNAL(clicked()),
            this, SLOT(searchNext()));
    connect(previousButton, SIGNAL(clicked()),
            this, SLOT(searchPrevious()));
    //勾选区分大小写和全字匹配
    connect(caseSensitive, SIGNAL(stateChanged(int)),
            this, SLOT(checkButtonToggled(int)));
    connect(consistentMatch, SIGNAL(stateChanged(int)),
            this, SLOT(checkButtonToggled(int)));
    //全部替换
    connect(replaceAllButton, SIGNAL(clicked()),
            this, SLOT(replaceAll()));
    //替换
    connect(replaceButton, SIGNAL(clicked()),
            this, SLOT(replace()));
    //高亮
    connect(highLightResult, SIGNAL(stateChanged(int)),
            this, SLOT(highlightResultChecked(int)));
    //user change text
    connect(codeEditor, SIGNAL(textChanged()),
            this, SLOT(userChangedText()));

}

/* 在搜索过程中，何时会出现 cursor.isNull 何时会出现 cursor.atEnd
 * isNull 为真的情况是，光标位置到文档末尾都不存在要找的字符，和之前是否找到过关键字无关。
 * atEnd 就是光标位于文档末尾
 */
/* always search from the start */
void FindReplaceArea::runtimeSearch(QString text)
{
    QString searchString = text;
    if ( searchString == "" )
    {
        codeEditor->setTextCursor(QTextCursor(codeEditor->document()));
        return;
    }

    QTextDocument::FindFlags findFlag(0);  // init to 0, this is important

    if ( consistentMatch->isChecked() && caseSensitive->isChecked() )
        findFlag = QTextDocument::FindCaseSensitively | QTextDocument::FindWholeWords;
    else if ( consistentMatch->isChecked() && !caseSensitive->isChecked() )
        findFlag = QTextDocument::FindWholeWords;
    else if ( ! consistentMatch->isChecked() && caseSensitive->isChecked() )
        findFlag = QTextDocument::FindCaseSensitively;


    QTextDocument *document = codeEditor->document();

    //下面初始化 cursor，初始化后位置都在文档前面
    QTextCursor highlightCursor(document);
    // 移动光标去找
    highlightCursor = document->find(searchString, highlightCursor, findFlag);

    if (!highlightCursor.isNull() )
    {
        codeEditor->setTextCursor(highlightCursor);
    }
    else if ( highlightCursor.isNull() )
    {
        codeEditor->setTextCursor(QTextCursor(codeEditor->document()));
    }

    highlightCursor.clearSelection();
}

/* 改变文字的时候调用这里，如果勾选了高亮，再高亮一次 */
void FindReplaceArea::runtimeSearchToggled(QString text)
{
    runtimeSearch(text);
    if ( highLightResult->isChecked() )
        clearAndHighlightAll(text);
}

//查找下一个对应的函数
void FindReplaceArea::searchNext()
{
    QString searchString = findLineEdit->text();
    if ( searchString == "" )
        return;

    QTextDocument::FindFlags findFlag(0);  // init to 0, this is important

    if ( consistentMatch->isChecked() && caseSensitive->isChecked() )
        findFlag = QTextDocument::FindCaseSensitively | QTextDocument::FindWholeWords;
    else if ( consistentMatch->isChecked() && !caseSensitive->isChecked() )
        findFlag = QTextDocument::FindWholeWords;
    else if ( ! consistentMatch->isChecked() && caseSensitive->isChecked() )
        findFlag = QTextDocument::FindCaseSensitively;

    QTextDocument *document = codeEditor->document();

    //下面初始化两个 cursor，初始化后位置都在文档前面
    QTextCursor highlightCursor(codeEditor->textCursor());

    // 移动光标去找
    highlightCursor = document->find(searchString, highlightCursor, findFlag);

    if (!highlightCursor.isNull() )
    {
        codeEditor->setTextCursor(highlightCursor);
    }
    else if ( highlightCursor.isNull() ) //如果没有找到，从文档起始位置开始再找一次
    {
        // find one more time
        highlightCursor = document->find(searchString, QTextCursor(document), findFlag);
        if (!highlightCursor.isNull() )
        {
            codeEditor->setTextCursor(highlightCursor);
        }
    }
    highlightCursor.clearSelection();
}

//查找上一个对应的函数
void FindReplaceArea::searchPrevious()
{

    QString searchString = findLineEdit->text();
    if ( searchString == "" )
        return;

    QTextDocument::FindFlags findFlag(QTextDocument::FindBackward);  // init to 0, this is important

    if ( consistentMatch->isChecked() && caseSensitive->isChecked() )
        findFlag = QTextDocument::FindCaseSensitively | QTextDocument::FindWholeWords | QTextDocument::FindBackward;
    else if ( consistentMatch->isChecked() && !caseSensitive->isChecked() )
        findFlag = QTextDocument::FindWholeWords | QTextDocument::FindBackward;
    else if ( ! consistentMatch->isChecked() && caseSensitive->isChecked() )
        findFlag = QTextDocument::FindCaseSensitively | QTextDocument::FindBackward;

    QTextDocument *document = codeEditor->document();
    QTextCursor highlightCursor(codeEditor->textCursor());

    highlightCursor = document->find(searchString, highlightCursor, findFlag);
    if ( !highlightCursor.isNull() ) //如果找到了
    {
        codeEditor->setTextCursor(highlightCursor);
    }
    else if ( highlightCursor.isNull() )//如果没有找到就再从文档末尾往回找一次
    {

        QTextCursor end(document->lastBlock());
        end.setPosition(end.position()+document->lastBlock().text().length());

        // find one more time
        highlightCursor = document->find(searchString, end, findFlag);

        if (!highlightCursor.isNull() )
        {
            codeEditor->setTextCursor(highlightCursor);
        }
    }
    highlightCursor.clearSelection();
}


//当勾选高亮全部的按钮时候，调用这个函数
void FindReplaceArea::highlightResultChecked(int state)
{
    if ( state == Qt::Checked )
    {
        clearAndHighlightAll( findLineEdit->text() );
    }
    else
    {
        clearAll();
    }
}

/*在查找高亮之前，需要清除以前的背景
 * text 是上一次查找的词*/
void FindReplaceArea::clearAll()
{
    QList<QTextEdit::ExtraSelection> extraSelection;
    CpadPalette cpadPalette;
    QColor lineColor = cpadPalette.defaultGroup[10];

    if (!codeEditor->isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = codeEditor->textCursor();
        selection.cursor.clearSelection();
        extraSelection.append(selection);
    }

    codeEditor->setExtraSelections(extraSelection);
}

void FindReplaceArea::clearAndHighlightAll(QString text)
{
    QTextDocument *document = codeEditor->document();
    QTextCursor highlightCursor(document);
    QTextCursor cursor(document);
    CpadPalette cpadPalette;
    ////////////////////////
    clearAll();
    QList<QTextEdit::ExtraSelection> selections = codeEditor->extraSelections();

    QTextEdit::ExtraSelection selection;
    QTextCharFormat format = selection.format;
    format.setBackground(cpadPalette.highlightForSearch);
    selection.format = format;

    if ( consistentMatch->isChecked() && caseSensitive->isChecked() )
        findFlag = QTextDocument::FindCaseSensitively | QTextDocument::FindWholeWords;
    else if ( consistentMatch->isChecked() && !caseSensitive->isChecked() )
        findFlag = QTextDocument::FindWholeWords;
    else if ( ! consistentMatch->isChecked() && caseSensitive->isChecked() )
        findFlag = QTextDocument::FindCaseSensitively;
    else
        findFlag = 0;

    cursor.beginEditBlock();
    while ( !highlightCursor.isNull() && !highlightCursor.atEnd() )
    {
        highlightCursor = document->find(text, highlightCursor,findFlag);
        if (!highlightCursor.isNull())
        {
            QTextCursor cursor = highlightCursor;
            cursor.setPosition(highlightCursor.position()-text.length(),QTextCursor::MoveAnchor);
            cursor.setPosition(highlightCursor.position(), QTextCursor::KeepAnchor);
            selection.cursor = cursor;
            selections.append(selection);
        }
    }
    cursor.endEditBlock();
    codeEditor->setExtraSelections(selections);

}

//当区分大小写和全字匹配勾选或取消勾选的时候，调用这里重新搜索
void FindReplaceArea::checkButtonToggled(int state)
{
    QString text = findLineEdit->text();
    if ( highLightResult->isChecked() == true )
        clearAndHighlightAll(text);
    runtimeSearch(text);
}

//替换全部
void FindReplaceArea::replaceAll()
{
    QString searchString = findLineEdit->text();
    if ( searchString == "" )
        return;
    QString replaceWith = replaceLineEdit->text();

    if ( consistentMatch->isChecked() && caseSensitive->isChecked() )
        findFlag = QTextDocument::FindCaseSensitively | QTextDocument::FindWholeWords;
    else if ( consistentMatch->isChecked() && !caseSensitive->isChecked() )
        findFlag = QTextDocument::FindWholeWords;
    else if ( ! consistentMatch->isChecked() && caseSensitive->isChecked() )
        findFlag = QTextDocument::FindCaseSensitively;

    QTextDocument *document = codeEditor->document();

    //下面初始化两个 cursor，初始化后位置都在文档前面
    QTextCursor highlightCursor(document);
    QTextCursor cursor(document);

    cursor.beginEditBlock();

    // 移动光标去找
    while ( !highlightCursor.isNull() && !highlightCursor.atEnd() )
    {
        highlightCursor = document->find(searchString, highlightCursor, findFlag);
        if (!highlightCursor.isNull())
        {
            highlightCursor.removeSelectedText();
            highlightCursor.insertText(replaceWith);
        }
    }
    cursor.endEditBlock();
}

void FindReplaceArea::replace()
{
    QString replaceWith = replaceLineEdit->text();
    QTextCursor cursor = codeEditor->textCursor();

    if ( cursor.hasSelection() )
    {
        cursor.removeSelectedText();
        cursor.insertText(replaceWith);
    }
    //替换之后再查找一次
    searchNext();
}

/*
 * 用户更改文本时候调用这个函数
 * 作用是在用户输入时，判断输入的位置是否有查找的高亮存在，有则把高亮取消掉
 */
void FindReplaceArea::userChangedText()
{
    QTextCursor cursor = codeEditor->textCursor();
    int listPosition = inExtraSelection(cursor.position());

    if ( listPosition == -1 )
        return;
    else
    {
        QList <QTextEdit::ExtraSelection> extraSelection = codeEditor->extraSelections();
        extraSelection.removeAt(listPosition);
        codeEditor->setExtraSelections(extraSelection);
    }

}

/*
 * 查找给定光标是否在已有的 extraSelection 中。 
 */
int FindReplaceArea::inExtraSelection(int cursorPosition)
{
    QList <QTextEdit::ExtraSelection> extraSelection = codeEditor->extraSelections();
    int listLength = extraSelection.length();
    if ( listLength == 1 )  // 这个里面只存了当前的行高亮
        return -1;

    int searchLength = findLineEdit->text().length();

    for ( int i = 1; i < listLength; i++ )
    {
        int end = extraSelection[i].cursor.position();
        int start = end - searchLength;
        if ( cursorPosition < end && cursorPosition > start )
            return i;
    }

    return -1;
}
