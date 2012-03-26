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
#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QString>
#include <QTextDocument>
#include <QList>
#include <QColor>
#include "textblockdata.h"

class QTextCharFormat;

class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    SyntaxHighlighter(QTextDocument *parent);

public slots:
    void changeColorGroup(QList<QColor> group);
    void turnOnSyntaxHighlight(bool turn);

private:

    // 常量、数据类型、关键词、注释 四种颜色
    QStringList dataTypeList;
    QStringList otherKeywordList;
    QTextCharFormat constFormat;  // 包括数字，字符串，单字符
    QTextCharFormat dataTypeFormat;  // 数据类型
    QTextCharFormat commentFormat;   // 单行和多行注释
    QTextCharFormat otherFormat;  //其他词，比如 for while return
    QTextCharFormat transferFormat;
    QTextCharFormat macroFormat;
    QTextCharFormat alertFormat;  //警示
    enum { commentFinished = 1, commentNotFinish };
    struct PositionInfo
    {
        int start;
        int count;
        QTextCharFormat format;
    };
    QList<PositionInfo> formatPosition;  //存储 struct{start, count, format}
    int findNextAvailableChar(const QString & text, char ch, int pos = 0);
    bool turnOn;

protected:
    void setSyntaxFormat();
    //重新实现 highlightBlock
    void highlightBlock(const QString &text);
    //得到位置信息，将信息存到 formatPosition 中
    void getPositionInfo(const QString &text); 
    void printList(QList<PositionInfo> &formatPosition);
    //将setFormat需要的信息存到 formatPosition 中
    void pushFormat(int start, int count, QTextCharFormat format);
    //高亮各种类型单独写成函数，在 getPositionInfo 里面调用。
    void highlightDataType(const QString &text);
    void highlightOtherkeyword(const QString &text);
    void highlightComments(const QString &text);
    void highlightSingleQuote(const QString &text);
    void highlightQuote(const QString &text);
    void highlightTransfer(const QString &text);
    void highlightParentheses(const QString &text);
    //检查某个符号在不在双引号里面里面
    bool inQuote(const QString &text, int pos);
    QTextCharFormat categoryFormat(int category);

};


#endif // SYNTAXHIGHLIGHTER_H
