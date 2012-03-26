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
#include "syntaxhighlighter.h"
#include "cpadpalette.h"
#include "memorycenter.h"
#include <QDebug>

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent):
    QSyntaxHighlighter(parent)
{

    dataTypeList << "auto" << "double" << "const" << "int" << "long" << "enum" << "float" << "char"
        << "register" << "short" << "signed" << "static" << "union" << "extern"
        << "unsigned" << "void" << "volatile" << "struct";

    otherKeywordList << "break" << "case" << "continue" << "default"
        << "do" << "else" << "for" << "goto" << "if" << "return"
        << "sizeof" << "struct" << "switch" << "typedef" << "while"
        << "#include" << "#define" << "#ifndef" << "#ifdef" << "#endif";

    setSyntaxFormat();

}

void SyntaxHighlighter::setSyntaxFormat()
{
//[0 comment, constant, datatype, 3 othertype, macro, 5 transfer; background, numberarea; 8 alert]
    CpadPalette cpadPalette;

    commentFormat.setForeground(cpadPalette.defaultGroup[0]);
    //commentFormat.setFontWeight(QFont::Bold);
    constFormat.setForeground(cpadPalette.defaultGroup[1]);
    //constFormat.setFontWeight(QFont::Bold);
    dataTypeFormat.setForeground(cpadPalette.defaultGroup[2]);
    dataTypeFormat.setFontWeight(QFont::Bold);

    otherFormat.setForeground(cpadPalette.defaultGroup[3]);
    otherFormat.setFontWeight(QFont::Bold);
    macroFormat.setForeground(cpadPalette.defaultGroup[4]);
    //macroFormat.setFontWeight(QFont::Bold);
    transferFormat.setForeground(cpadPalette.defaultGroup[5]);
    //transferFormat.setFontWeight(QFont::Bold);
    alertFormat.setForeground(cpadPalette.defaultGroup[8]);
    alertFormat.setFontWeight(QFont::Bold);
}

void SyntaxHighlighter::changeColorGroup(QList<QColor> group)
{
    commentFormat.setForeground(group[0]);
    //commentFormat.setFontWeight(QFont::Bold);
    constFormat.setForeground(group[1]);
    //constFormat.setFontWeight(QFont::Bold);
    dataTypeFormat.setForeground(group[2]);
    dataTypeFormat.setFontWeight(QFont::Bold);
    otherFormat.setForeground(group[3]);
    otherFormat.setFontWeight(QFont::Bold);
    macroFormat.setForeground(group[4]);
    //macroFormat.setFontWeight(QFont::Bold);
    transferFormat.setForeground(group[5]);
    //transferFormat.setFontWeight(QFont::Bold);
    alertFormat.setForeground(group[8]);
    alertFormat.setFontWeight(QFont::Bold);

    rehighlight();
}

void SyntaxHighlighter::turnOnSyntaxHighlight(bool turn)
{
    turnOn = turn;
    rehighlight();
}

QTextCharFormat SyntaxHighlighter::categoryFormat(int category)
{
    //[0 comment, constant, datatype, 3 othertype, macro, 5 transfer;

    QTextCharFormat plain;

    if ( (category >= 50 && category <= 53) || category == 10 ) //4 macro
        return macroFormat;
    else if ( category >= 54 && category <= 70 ) //3 othertype
        return otherFormat;
    else if ( category >= 71 && category <= 89 ) //2 datatype
        return dataTypeFormat;
    else if ( category >= 90 && category <= 91 ) // 0 comment
        return commentFormat;
    else if ( category == 1 || category == 8 || category == 9 ) //1 constant
        return constFormat;

    return plain;
}

void SyntaxHighlighter::getPositionInfo(const QString &text)
{
/*
    highlightDataType(text);
    highlightOtherkeyword(text);
    highlightSingleQuote(text);
    highlightQuote(text);
    //警示转义符号
    highlightTransfer(text);

    highlightComments(text);*/

    ////////////
    MemoryCenter memoryCenter;
    QString lineText = text;
    memoryCenter.analysisMain(lineText);

    struct LineHeadPointer * head;

    head = memoryCenter.getLineHead();
    head = head->nextLine;
    while ( head != NULL )
    {
        struct LexicalAnalysis *word = head->firstWord;
        while ( word != NULL )
        {
            QTextCharFormat format = categoryFormat(word->wordCategory);
            pushFormat(word->wordPosition, word->wordLength, format);
            word = word->nextWord;
        }
        head = head->nextLine;
    }
    //警示转义符号

    highlightTransfer(text);
    highlightComments(text);
    highlightParentheses(text);
    return;
}

/*
 * highlightBlock 函数在文本内容改变的时候自动调用。
 * text 是光标当前行的所有内容，所以在这里可以实现高亮。
 * 常量、数据类型、关键词、注释 四种颜色
 */
void SyntaxHighlighter::highlightBlock(const QString &text)
{
    if ( turnOn == false )
    {
        return;
    }
    getPositionInfo(text);
    //  开始高亮
    foreach (const PositionInfo &position, formatPosition){
        setFormat(position.start, position.count, position.format);
    }
    //因为用户可能把已经被高亮的词删掉，所以这里，用完之后把记录着高亮位置的 QList 清空，为下次做准备
    formatPosition.clear();
}

void SyntaxHighlighter::pushFormat(int start, int count, QTextCharFormat format)
{
    PositionInfo positionInfo;

    positionInfo.start = start;
    positionInfo.count = count;
    positionInfo.format = format;
    formatPosition.append(positionInfo);
}

/*
 * 得到输入信息，将信息存到 formatPosition 中
 * 参数是新输入行的内容
 */
void SyntaxHighlighter::highlightDataType(const QString &text)
{
    int start = -1;
    int pos = start;  //当前搜索位置

    for (int i = 0; i < dataTypeList.size(); i++)
    {
        while(true)
        {
            pos = text.indexOf(dataTypeList[i], start + 1);
            if ( pos != -1 )
            {   //找到子串
                int keywordLength = dataTypeList[i].length();
                if ( text.length() < keywordLength )
                    break;

                // bug 出现在这里：文本为 int，长度为3且仅为3。
                //如果试图调用text[pos+dataTypeList[i].length()]去找int后有无单词或字母就
                //相当于调用了 text[3]，无效内存引用了。
                if ( pos+keywordLength == text.length() )
                {  //也就是要高亮的关键字出现在鼠标闪烁的地方。
                    if ( pos != 0 && text[pos-1].isLetterOrNumber() )
                        break;
                    else
                    {
                        pushFormat(pos, keywordLength, dataTypeFormat);
                        break;
                    }
                }
                //到这里，已经不可能出现 pos + keywordLength 越界的情况了
                if ( pos == 0 && text[pos + keywordLength].isLetterOrNumber())
                {  //这个子串不是关键字 例如 ints, int1
                    start = pos;
                    continue;
                }
                if ( pos != 0 &&  // 这个子串不是关键字 例如 sint, 12int, int5
                        (text[pos-1].isLetterOrNumber() ||
                         text[pos+keywordLength].isLetterOrNumber()))
                {
                    start = pos;
                    continue;
                }
                // 到这里说明是关键字
                start = pos;
                pushFormat(pos, keywordLength, dataTypeFormat);
            } // end: if ( pos != -1 )
            else
            {  //没有找到子串
                start = -1; //为之后找其他关键字做准备
                break;
            }
        }  // end while(true)
    }
}

void SyntaxHighlighter::highlightOtherkeyword(const QString &text)
{
    int start = -1;
    int pos = start;  //当前搜索位置

    for (int i = 0; i < otherKeywordList.size(); i++)
    {
        while(true)
        {
            pos = text.indexOf(otherKeywordList[i], start + 1);
            if ( pos != -1 )
            {   //找到子串
                int keywordLength = otherKeywordList[i].length();
                if ( text.length() < keywordLength )
                    break;

                // bug 出现在这里：文本为 int，长度为3且仅为3...和上面函数的情况一样
                if ( pos+keywordLength == text.length() )
                {  //也就是要高亮的关键字出现在鼠标闪烁的地方。
                    if ( pos+keywordLength == text.length() )
                    {  //也就是要高亮的关键字出现在鼠标闪烁的地方。
                        if ( pos != 0 && text[pos-1].isLetterOrNumber() )
                            break;
                        else
                        {
                            pushFormat(pos, keywordLength, otherFormat);
                            break;
                        }
                    }
                }
                //到这里，已经不可能出现 pos + keywordLength 越界的情况了
                if ( pos == 0 && text[pos + keywordLength].isLetterOrNumber())
                {  //这个子串不是关键字 例如 ints, int1
                    start = pos;
                    continue;
                }
                if ( pos != 0 &&  // 这个子串不是关键字 例如 sint, 12int, int5
                        (text[pos-1].isLetterOrNumber() ||
                         text[pos+keywordLength].isLetterOrNumber()))
                {
                    start = pos;
                    continue;
                }
                // 到这里说明是关键字
                start = pos;
                pushFormat(pos, keywordLength, otherFormat);
            } // end: if ( pos != -1 )
            else
            {  //没有找到子串
                start = -1; //为之后找其他关键字做准备
                break;
            }
        }  // end while(true)
    }
}

void SyntaxHighlighter::highlightComments(const QString &text)
{
    if ( previousBlockState() == commentNotFinish )
        setCurrentBlockState(commentNotFinish);
    else
        setCurrentBlockState(commentFinished);

    int textLength = text.length();

    for ( int i = 0; i < textLength; i++ )
    {
        if ( currentBlockState() == commentNotFinish )
        { //上行注释延续到本行
            int tmp = text.indexOf("*/");
            if ( tmp == -1 )
            {
                setCurrentBlockState(commentNotFinish);
                pushFormat(0, textLength, commentFormat);
                break;
            }
            else  // 找到封闭标志
            {
                setCurrentBlockState(commentFinished);
                pushFormat(0, tmp + 2, commentFormat );
                i = tmp + 1;
                continue;
            }
        }

        if ( text.mid(i, 2) == "//" )
        {
            pushFormat(i, textLength - i, commentFormat );
            break;
        }

        if ( text.mid(i, 2) == "/*" )
        {
            int endIndex = text.indexOf("*/", i);
            if ( endIndex == -1 ) //没有找到结束标志
            {
                setCurrentBlockState(commentNotFinish);
                pushFormat(i, textLength - i, commentFormat );
                break;
            }
            else
            { //找到结束标志
                setCurrentBlockState(commentFinished);
                pushFormat(i, endIndex - i + 2, commentFormat);
                i = endIndex+1;
                continue;
            }
        }

        if ( text.at(i) == '\"' )
        {
            int position = i;
            while (true)
            {
                position = text.indexOf("\"", position + 1);
                if ( position == -1)  //没有找到双引号
                    break;
                else  //找到双引号
                {
                    if ( text.at(position-1) == '\\' ) //注：这里 position-1 一定合法
                    {  //这个冒号前面可能有转移符号
                        position++;
                        continue;
                    }
                    else
                        break;
                }
            }
            if ( position == -1 )
                break;
            else
                i = position;
        }

        if ( text.mid(i, 2) == "*/" )
        {
            if ( currentBlockState() == commentFinished )
            {
                pushFormat(i,2,alertFormat);
                i++;
            }
        }
    }
}

void SyntaxHighlighter::printList(QList<PositionInfo> &formatPosition)
{
    qDebug() << "printList:";
    for (int i = 0; i < formatPosition.size(); i++){
        PositionInfo test;
        test = formatPosition[i];
        qDebug() << "(" << test.start <<", " << test.count <<", "<<test.format<<")";
    }
    qDebug() << "printList end";
}
void SyntaxHighlighter::highlightQuote(const QString &text)
{
    int startIndex = 0;
    int endIndex = 0;

    startIndex = text.indexOf("\"");
    endIndex = startIndex;

    while (startIndex >= 0){
        endIndex = text.indexOf("\"", startIndex + 1);

        if ( endIndex != -1 && endIndex != 0 && text[endIndex - 1] == '\\' ) 
        {   //如果找到的引号前面是转义字符
            pushFormat(startIndex, endIndex-startIndex+1, constFormat);
            startIndex = endIndex;
            continue;
        }

        int quoteLength;

        if ( endIndex == -1 ) //没有找到
            quoteLength = text.length() - startIndex;
        else
            quoteLength = endIndex - startIndex + 1;
        pushFormat(startIndex, quoteLength, constFormat);
        startIndex = text.indexOf("\"", startIndex + quoteLength); //再往后找
    }    
}


void SyntaxHighlighter::highlightSingleQuote(const QString &text)
{
    int startIndex = text.indexOf("\'");
    int highlightLength = 0;

    while ( startIndex >= 0 )
    {
        int transferIndex = text.indexOf("\\", startIndex);
        if ( transferIndex - startIndex == 1 ) //单引号后面紧接着转义符
        {
            if ( text.length() - startIndex >= 4 )
                highlightLength = 4;
            else
                highlightLength = text.length() - startIndex;
        }
        else
        {
            if ( text.length() - startIndex >= 3 )
                highlightLength = 3;
            else
                highlightLength = text.length() - startIndex;
        }

        pushFormat(startIndex, highlightLength, constFormat);
        startIndex = text.indexOf("\'", startIndex + highlightLength );
    }

}
void SyntaxHighlighter::highlightTransfer(const QString &text)
{
    int startIndex = text.indexOf("\\");
    PositionInfo positionInfo;

    while( startIndex >= 0 )
    {
        positionInfo.start = startIndex;
        positionInfo.count = 2;
        positionInfo.format = alertFormat;
        formatPosition.append(positionInfo);
        int next = startIndex+1;
        if ( next < text.length() && text[next] == '\\')
            next++;
        startIndex = text.indexOf("\\", next);
    }
}

// 这个函数只是找注释符号会不会在引号之间
bool SyntaxHighlighter::inQuote(const QString &text, int pos)
{


    if ( pos >= text.size() || pos < 0)
        return false;

    // quotePosition 存成 [4, 8, 18, 40] 这样子
    QList<int> quotePosition;
    int quote = text.indexOf("\"");
    if ( quote == -1 ) //没有引号
        return false;
    while ( quote >= 0 )
    {
        quotePosition << quote;
        quote = text.indexOf("\"", quote+1);
    }

    int i = 0;
    while ( true )
    {
        if ( i >= quotePosition.size() )
            break;
        if ( quotePosition.at(i) < pos )
        {
            i++;
            continue;
        }
        else
            break;
    }
    // 到这里 pos = 15, quotePosition = [ 4, 8, 13 ]
    // 1. 如果 i >= quotePosition.size()
    // 1.1 quotePosition.size() 是奇数，说明在括号里面
    // 1.2 quotePosition.size() 是偶数，说明不在括号里面
    // 2. 如果 i 是偶数，说明不在括号里面
    // 3. 如果 i 是奇数，说明在括号里面

    if ( i>= quotePosition.size() )
    {
        if ( quotePosition.size() % 2 == 0 )
            return false;
        else
            return true;
    }

    if ( i % 2 == 0 )
        return false;
    else
        return true;


}

/* 括号匹配高亮 */
void SyntaxHighlighter::highlightParentheses(const QString &text)
{
    TextBlockData *data = new TextBlockData;

    //int leftPos = text.indexOf('(');
    int leftPos = findNextAvailableChar(text, '(');
    while (leftPos != -1) {
        ParenthesisInfo *info = new ParenthesisInfo;
        info->character = '(';
        info->position = leftPos;

        data->insert(info);
        //leftPos = text.indexOf('(', leftPos + 1);
        leftPos = findNextAvailableChar(text, '(', leftPos + 1);
    }

    //int rightPos = text.indexOf(')');
    int rightPos = findNextAvailableChar(text, ')');
    while (rightPos != -1) {
        ParenthesisInfo *info = new ParenthesisInfo;
        info->character = ')';
        info->position = rightPos;
        data->insert(info);

        //rightPos = text.indexOf(')', rightPos +1);
        rightPos = findNextAvailableChar(text, ')', rightPos + 1);
    }
    /////////
    //leftPos = text.indexOf('[');
    leftPos = findNextAvailableChar(text, '[');
    while (leftPos != -1) {
        ParenthesisInfo *info = new ParenthesisInfo;
        info->character = '[';
        info->position = leftPos;

        data->insert(info);
        //leftPos = text.indexOf('[', leftPos + 1);
        leftPos = findNextAvailableChar(text, '[', leftPos + 1);
    }

    //rightPos = text.indexOf(']');
    rightPos = findNextAvailableChar(text, ']');
    while (rightPos != -1) {
        ParenthesisInfo *info = new ParenthesisInfo;
        info->character = ']';
        info->position = rightPos;
        data->insert(info);

        //rightPos = text.indexOf(']', rightPos +1);
        rightPos = findNextAvailableChar(text, ']', rightPos + 1);
    }
    ////////////
    //leftPos = text.indexOf('{');
    leftPos = findNextAvailableChar(text, '{');
    while (leftPos != -1) {
        ParenthesisInfo *info = new ParenthesisInfo;
        info->character = '{';
        info->position = leftPos;

        data->insert(info);
        //leftPos = text.indexOf('{', leftPos + 1);
        leftPos = findNextAvailableChar(text, '{', leftPos + 1);
    }

    //rightPos = text.indexOf('}');
    rightPos = findNextAvailableChar(text, '}');
    while (rightPos != -1) {
        ParenthesisInfo *info = new ParenthesisInfo;
        info->character = '}';
        info->position = rightPos;
        data->insert(info);

        //rightPos = text.indexOf('}', rightPos +1);
        rightPos = findNextAvailableChar(text, '}', rightPos + 1);
    }

    /*for ( int i = 0; i < data->parentheses().size(); i++)
    {
        qDebug() << i <<"  < " << data->parentheses()[i]->character << "  " << data->parentheses()[i]->position << ">";
    }
    qDebug() << "=============";*/
    setCurrentBlockUserData(data);
}

/* 找下一个不在引号里面的 ch，返回其位置，未找到返回 -1*/
int SyntaxHighlighter::findNextAvailableChar(const QString &text, char ch, int pos)
{
    int length = text.length();
    if ( pos >= length )
        return -1;

    bool comment1Flag = false; // 用于检测当前是否是在单引号里面
    bool comment2Flag = false; // 用于检测当前是否是在双引号里面
    int i = pos;
    while ( i < length )
    {
        if ( text[i] == '\'' && comment2Flag == false) //单引号不在双引号中
        {
            if ( i == 0 || (i != 0 && text[i] != '\\'))
            {
                if ( comment1Flag == false)
                    comment1Flag = true;
                else
                    comment1Flag = false;
                i++;
                continue;
            }
        }

        if ( text[i] == '"' && comment1Flag == false) //双引号不在单引号中
        {
            if ( i == 0 || (i != 0 && text[i] != '\\'))
            {
                if ( comment2Flag == false)
                    comment2Flag = true;
                else
                    comment2Flag = false;
                i++;
                continue;
            }
        }

        if ( comment1Flag == true || comment2Flag == true )
        {//如果现在在引号里面，忽略所有东西
            i++;
            continue;
        }
        else //不在双引号里面
        {
            if ( text[i] == ch )
            {
                break;
            }
            else
            {
                i++;
            }
        }
    }
    if ( i == length)
    {
        return -1;
    }
    else
    {
        return i;
    }
}
