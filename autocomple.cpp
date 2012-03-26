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
#include "autocomple.h"

AutoComple::AutoComple(CodeEditor *editor)
{
    codeEditor = editor;
    memoryCenter = new MemoryCenter;
    setGeometry(100, 100, 200, 60);
    setWindowFlags(Qt::Popup);
    //setWindowModality(Qt::WindowModal);
    setFocus(Qt::MouseFocusReason);
    setFont(codeEditor->font());
    connect(this, SIGNAL(itemActivated(QListWidgetItem*)),
            this, SLOT(compleString(QListWidgetItem*)));
    connect(this, SIGNAL(itemPressed(QListWidgetItem*)),
            this, SLOT(compleString(QListWidgetItem*)));
}

void AutoComple::compleString(QListWidgetItem *item)
{
    QString word( item->text() );
    word.remove(0, matchLength);
    QTextCursor tcr( codeEditor->textCursor() );
    tcr.insertText(word);
    codeEditor->setTextCursor(tcr);
    this->close();
}

void AutoComple::doAutoComple(int mode)
{
    if (autoCompleCheck(mode)) {
        //show之前计算坐标
        QTextCursor tcr(codeEditor->textCursor());
        QPoint pp = codeEditor->getCompleXY();
        setGeometry(pp.x(), pp.y(), this->width(), this->height());
        show();
    }
    else 
        close();
}

bool AutoComple::autoCompleCheck(int mode)
{
    QTextCursor tcr( codeEditor->textCursor() );
    QTextBlock blk( tcr.block() );
    QString halfLineText = blk.text();
    int posA( blk.position() ), posB( tcr.position() );
    int length( halfLineText.length() );
    halfLineText.remove( posB-posA, length-(posB-posA) );

    if (halfLineText.endsWith(" ") || halfLineText.isEmpty() ) 
        return false;
    else {
        LineHeadPointer *lineHeadTemp;
        LexicalAnalysis *wordTemp;
        CompleList *findCompleResult;
        QString wordString;
        //matchNumber = matchLength = 0;
        //matchNumber是返回的链表的长度;matchLength是当前半词长度
        //这里必须先保证最后一个字符不是空格
        memoryCenter -> analysisMain ( halfLineText );//参数为当前行内容
        lineHeadTemp = memoryCenter -> getLineHead ( );
        for ( ; lineHeadTemp -> nextLine ; lineHeadTemp = lineHeadTemp -> nextLine );
        for ( wordTemp = lineHeadTemp -> firstWord ; wordTemp -> nextWord ; wordTemp = wordTemp -> nextWord );
        if ( wordTemp -> wordCategory == 92 || ( wordTemp -> wordCategory >= 50 && wordTemp -> wordCategory <= 89 ) )
        {
            wordString = wordTemp -> wordContent;
            if (mode) {
                QString codeText( codeEditor->toPlainText() );
                memoryCenter -> analysisMain ( codeText );//参数为全文内容
                memoryCenter -> makeCompleList ( );
            }
            findCompleResult = memoryCenter -> findCompleList ( wordString , matchNumber , matchLength );
        }
        else
            findCompleResult = NULL;

        if (!findCompleResult)
            return false;

        this->clear();//清除原来的数据
        QListWidgetItem *i;

        int maxLength = 0, tempLength;
        QString maxWord;
        for ( int j = matchNumber; j; j-- , findCompleResult = findCompleResult -> nextComple )
        {
            i = new QListWidgetItem();
            i->setText(findCompleResult -> compleName);
            tempLength = (i->text()).length(); 
            if (tempLength > maxLength) { 
                maxLength = tempLength;
                maxWord = i->text();
            }
            i->setData(Qt::UserRole, findCompleResult -> compleNameLength);
            this->addItem(i);
        }
        this->setCurrentRow(0);
        /*if (matchNumber == 1) {
            emit itemActivated(item(0));
            return false;
        }*/
        if (matchNumber >= 5) {
            this->setFixedHeight(5*(fontMetrics().height()+4));
            maxWord.append("    ");
        }
        else {
            this->setFixedHeight(matchNumber*(fontMetrics().height()+4));
            maxWord.append("   ");
        }

        this->setFixedWidth(fontMetrics().width(maxWord));
        return true;
    }
}

void AutoComple::fontSettings(QFont f)
{
    setFont(f);
}

void AutoComple::keyPressEvent(QKeyEvent *event) 
{
    //QPlainTextEdit::keyPressEvent(event); 
    if ( event->key() == Qt::Key_Escape ) {
       close();
       return;
    }
    else if (event->key() == Qt::Key_Down ||
             event->key() == Qt::Key_Up ) {
        QListWidget::keyPressEvent(event);
        return;
    }
    else if (event->key() == Qt::Key_Left ||
             event->key() == Qt::Key_Right ) {
        codeEditor->keyPressEvent(event);
        doAutoComple(0);
        return;
    }
    else if (event->key() == Qt::Key_Return) {
        emit itemActivated(currentItem());
        close();
        return;
    }
    else if ( event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_N ) {
        QKeyEvent my(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
        keyPressEvent(&my);
        return;
    }
    else if ( event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_P ) {
        QKeyEvent my(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
        keyPressEvent(&my);
        return;
    }

    codeEditor->keyPressEvent(event);
    doAutoComple(0);
    
    QListWidget::keyPressEvent(event);
}
