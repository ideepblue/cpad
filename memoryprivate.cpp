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
#include "memorycenter.h"

void MemoryCenter :: resetLine ( )
{
    struct LineHeadPointer *lineHeadTemp1 , *lineHeadTemp2;
    struct LexicalAnalysis *wordTemp1 , *wordTemp2;
    lineHeadTemp1 = NULL;
    lineHeadTemp2 = NULL;
    for ( lineHeadTemp1 = memoryHead ; lineHeadTemp1 -> nextLine ; lineHeadTemp2 = lineHeadTemp1 , lineHeadTemp1 = lineHeadTemp1 -> nextLine );
    for ( ; lineHeadTemp1 != memoryHead ; lineHeadTemp1 = lineHeadTemp2 , lineHeadTemp2 = lineHeadTemp1 -> previousLine )
    {
        wordTemp1 = NULL;
        wordTemp2 = NULL;
        if ( lineHeadTemp1 -> firstWord == NULL )
        {
            delete ( lineHeadTemp1 );
            continue;
        }
        for ( wordTemp1 = lineHeadTemp1 -> firstWord ; wordTemp1 -> nextWord ; wordTemp2 = wordTemp1 , wordTemp1 = wordTemp1 -> nextWord );
        for ( ; wordTemp2 != NULL ; wordTemp1 = wordTemp2 , wordTemp2 = wordTemp1 -> previousWord )
            delete ( wordTemp1 );
        delete ( wordTemp1 );
        delete ( lineHeadTemp1 );
    }
    memoryHead -> logicLevel = 0;
    memoryHead -> linePosition = 0;
    memoryHead -> previousLine = NULL;
    memoryHead -> nextLine = NULL;
    memoryHead -> firstWord = NULL;
}

void MemoryCenter :: resetFunctionList ( )
{
    struct FunctionList *functionTemp1 , *functionTemp2;
    struct ParameterList *parameterTemp1 , *parameterTemp2;
    functionTemp1 = NULL;
    functionTemp2 = NULL;
    for ( functionTemp1 = functionHead ; functionTemp1 -> nextFunction ; functionTemp2 = functionTemp1 , functionTemp1 = functionTemp1 -> nextFunction );
    for ( ; functionTemp1 != functionHead ; functionTemp1 = functionTemp2 , functionTemp2 = functionTemp1 -> previousFunction )
    {
        parameterTemp1 = NULL;
        parameterTemp2 = NULL;
        if ( functionTemp1 -> firstParameter == NULL )
        {
            delete ( functionTemp1 );
            continue;
        }
        for ( parameterTemp1 = functionTemp1 -> firstParameter ; parameterTemp1 -> nextParameter ; parameterTemp2 = parameterTemp1 , parameterTemp1 = parameterTemp1 -> nextParameter );
        for ( ; parameterTemp2 != NULL ; parameterTemp1 = parameterTemp2 , parameterTemp2 = parameterTemp1 -> previousParameter )
            delete ( parameterTemp1 );
        delete ( parameterTemp1 );
        delete ( functionTemp1 );
    }

    functionHead -> functionName = "";
    functionHead -> functionNameLength = 0;
    functionHead -> functionCategory = "";
    functionHead -> functionCategoryLength = 0;
    functionHead -> parameterNumber = 0;
    functionHead -> functionPosition = 0;
    functionHead -> functionLine = 0;
    functionHead -> functionOutput = "";
    functionHead -> firstParameter = NULL;
    functionHead -> previousFunction = NULL;
    functionHead -> nextFunction = NULL;
}

void MemoryCenter :: resetCompleList ( )
{
    struct CompleList *compleTemp1 , *compleTemp2;
    compleTemp1 = NULL;
    compleTemp2 = NULL;
    for ( compleTemp1 = compleHead ; compleTemp1 -> nextComple ; compleTemp2 = compleTemp1 , compleTemp1 = compleTemp1 -> nextComple );
    for ( ; compleTemp1 != compleHead ; compleTemp1 = compleTemp2 , compleTemp2 = compleTemp1 -> previousComple )
        delete ( compleTemp1 );
    compleHead -> compleName = "";
    compleHead -> compleNameLength = 0;
    compleHead -> previousComple = NULL;
    compleHead -> nextComple = NULL;
}

void MemoryCenter :: resetLineNumberList ( )
{
    struct LineNumberList *lineNumberTemp1 , *lineNumberTemp2;
    lineNumberTemp1 = lineNumberHead -> nextLineNumber;
    if ( lineNumberTemp1 )
        lineNumberTemp2 = lineNumberTemp1 -> nextLineNumber;
    else
        lineNumberTemp2 = NULL;
    for ( ; ; )
    {
        delete ( lineNumberTemp1 );
        if ( lineNumberTemp2 )
        {
            lineNumberTemp1 = lineNumberTemp2;
            lineNumberTemp2 = lineNumberTemp1 -> nextLineNumber;
        }
        else
            break;
    }
    lineNumberHead -> lineNumber = 0;
    lineNumberHead -> linePosition = 0;
    lineNumberHead -> nextLineNumber = NULL;
}

void MemoryCenter :: buildKeyWordList ( )
{
    keyWordList [ 0 ] = "";
    keyWordList [ 1 ] = "";
    keyWordList [ 2 ] = "{";
    keyWordList [ 3 ] = "}";
    keyWordList [ 4 ] = "[";
    keyWordList [ 5 ] = "]";
    keyWordList [ 6 ] = "(";
    keyWordList [ 7 ] = ")";
    keyWordList [ 8 ] = "";
    keyWordList [ 9 ] = "";
    keyWordList [ 10 ] = "#";
    keyWordList [ 11 ] = ":";
    keyWordList [ 12 ] = ";";
    keyWordList [ 13 ] = ".";
    keyWordList [ 14 ] = "!";
    keyWordList [ 15 ] = "~";
    keyWordList [ 16 ] = "+";
    keyWordList [ 17 ] = "-";
    keyWordList [ 18 ] = "*";
    keyWordList [ 19 ] = "&";
    keyWordList [ 20 ] = "/";
    keyWordList [ 21 ] = "%";
    keyWordList [ 22 ] = "<";
    keyWordList [ 23 ] = ">";
    keyWordList [ 24 ] = "^";
    keyWordList [ 25 ] = "|";
    keyWordList [ 26 ] = "=";
    keyWordList [ 27 ] = ",";
    keyWordList [ 28 ] = "?";
    keyWordList [ 29 ] = "++";
    keyWordList [ 30 ] = "--";
    keyWordList [ 31 ] = "->";
    keyWordList [ 32 ] = "<<";
    keyWordList [ 33 ] = ">>";
    keyWordList [ 34 ] = "<=";
    keyWordList [ 35 ] = ">=";
    keyWordList [ 36 ] = "==";
    keyWordList [ 37 ] = "!=";
    keyWordList [ 38 ] = "&&";
    keyWordList [ 39 ] = "||";
    keyWordList [ 40 ] = "+=";
    keyWordList [ 41 ] = "-=";
    keyWordList [ 42 ] = "*=";
    keyWordList [ 43 ] = "/=";
    keyWordList [ 44 ] = "%=";
    keyWordList [ 45 ] = "&=";
    keyWordList [ 46 ] = "^=";
    keyWordList [ 47 ] = "!=";
    keyWordList [ 48 ] = "<<=";
    keyWordList [ 49 ] = ">>=";
    keyWordList [ 50 ] = "include";
    keyWordList [ 51 ] = "define";
    keyWordList [ 52 ] = "ifndef";
    keyWordList [ 53 ] = "endif";
    keyWordList [ 54 ] = "sizeof";
    keyWordList [ 55 ] = "case";
    keyWordList [ 56 ] = "default";
    keyWordList [ 57 ] = "typedef";
    keyWordList [ 58 ] = "struct";
    keyWordList [ 59 ] = "union";
    keyWordList [ 60 ] = "enum";
    keyWordList [ 61 ] = "if";
    keyWordList [ 62 ] = "else";
    keyWordList [ 63 ] = "for";
    keyWordList [ 64 ] = "do";
    keyWordList [ 65 ] = "while";
    keyWordList [ 66 ] = "switch";
    keyWordList [ 67 ] = "goto";
    keyWordList [ 68 ] = "break";
    keyWordList [ 69 ] = "continue";
    keyWordList [ 70 ] = "return";
    keyWordList [ 71 ] = "const";
    keyWordList [ 72 ] = "auto";
    keyWordList [ 73 ] = "unsigned";
    keyWordList [ 74 ] = "signed";
    keyWordList [ 75 ] = "volatile";
    keyWordList [ 76 ] = "register";
    keyWordList [ 77 ] = "static";
    keyWordList [ 78 ] = "extern";
    keyWordList [ 79 ] = "short";
    keyWordList [ 80 ] = "long";
    keyWordList [ 81 ] = "void";
    keyWordList [ 82 ] = "char";
    keyWordList [ 83 ] = "int";
    keyWordList [ 84 ] = "float";
    keyWordList [ 85 ] = "double";
    keyWordList [ 86 ] = "__int8";
    keyWordList [ 87 ] = "__int16";
    keyWordList [ 88 ] = "__int32";
    keyWordList [ 89 ] = "__int64";
    keyWordList [ 90 ] = "";
    keyWordList [ 91 ] = "";
    keyWordList [ 92 ] = "";
    keyWordList [ 93 ] = "";
    keyWordList [ 94 ] = "";
    keyWordList [ 95 ] = "";
    keyWordList [ 96 ] = "";
    keyWordList [ 97 ] = "";
    keyWordList [ 98 ] = "";
    keyWordList [ 99 ] = "";
}

void MemoryCenter :: buildLineNumberList ( QString codeString )
{
    int codePosition , codeLine , codeLength;
    struct LineNumberList *lineNumberTemp , *lineNumberBuild;
    resetLineNumberList ( );
    codePosition = 1;
    codeLine = 1;
    codeLength = codeString . length ( );
    if ( codeLength )
    {
        lineNumberBuild = new ( struct LineNumberList );
        lineNumberBuild -> lineNumber = 1;
        lineNumberBuild -> linePosition = 0;
        lineNumberBuild -> nextLineNumber = NULL;
        lineNumberHead -> nextLineNumber = lineNumberBuild;
        lineNumberTemp = lineNumberBuild;
    }
    else
        return;
    for ( ; codePosition < codeLength ; codePosition ++ )
        if ( codeString [ codePosition - 1 ] == '\n' )
        {
            codeLine ++;
            lineNumberBuild = new ( struct LineNumberList );
            lineNumberBuild -> lineNumber = codeLine;
            lineNumberBuild -> linePosition = codePosition;
            lineNumberBuild -> nextLineNumber = NULL;
            lineNumberTemp -> nextLineNumber = lineNumberBuild;
            lineNumberTemp = lineNumberBuild;
        }
}

int MemoryCenter :: judgeWordCategory ( const QString & thisWord , const int startFindPosition , const int endFindPosition )
{
    int findPositionTemp;
    for ( findPositionTemp = startFindPosition ; findPositionTemp <= endFindPosition ; findPositionTemp ++ )
        if ( thisWord == keyWordList [ findPositionTemp ] )
            break;
    if ( findPositionTemp > endFindPosition )
        return 92;
    else
        return findPositionTemp;
}

QString MemoryCenter :: cutWord ( const QString & codeString , const int & codePosition , int & wordLength , int & wordKind )
{
    QChar firstLetter , secondLetter , thirdLetter ;
    QString thisWord;
    int codePositionTemp , codeLength , escapeTemp;
    wordKind = -1;
    codeLength = codeString . length ( );
    if ( codePosition < codeLength )
        firstLetter = codeString [ codePosition ];
    else
        firstLetter = '\0';
    if ( firstLetter != '\0' && firstLetter != '\r' && firstLetter != '\n' && codePosition + 1 < codeLength )
        secondLetter = codeString [ codePosition + 1 ];
    else
        secondLetter = '\0';
    if ( secondLetter != '\0' && secondLetter != '\r' && secondLetter != '\n' && codePosition + 2 < codeLength )
        thirdLetter = codeString [ codePosition + 2 ];
    else
        thirdLetter = '\0';
    if ( firstLetter == '\0' || firstLetter == '\r' || firstLetter == '\n' || firstLetter == ' ' )
    {
        thisWord = "" + QString ( firstLetter );
        wordLength = 1;
        wordKind = 0;
    }
    else if ( firstLetter == '0' && ( secondLetter == 'x' || secondLetter == 'X' ) )
    {
        if ( secondLetter == 'x' )
            thisWord = "0x";
        else
            thisWord = "0X";
        for ( codePositionTemp = codePosition + 2 ; ; codePositionTemp ++ )
        {
            if ( codePositionTemp >= codeLength || ( codeString [ codePositionTemp ] != '.' && ( codeString [ codePositionTemp ] < '0' || codeString [ codePositionTemp ] > '9' ) && ( codeString [ codePositionTemp ] < 'a' || codeString [ codePositionTemp ] > 'f' ) && ( codeString [ codePositionTemp ] < 'A' || codeString [ codePositionTemp ] > 'F' ) ) )
                break;
            thisWord += codeString [ codePositionTemp ];
        }
        wordLength = thisWord . length ( );
        wordKind = 1;
    }
    else if ( ( firstLetter >= '0' && firstLetter <= '9' ) || ( firstLetter == '.' && secondLetter >= '0' && secondLetter <= '9' ) )
    {
        thisWord = "";
        for ( codePositionTemp = codePosition ; ; )
        {
            thisWord += codeString [ codePositionTemp ];
            codePositionTemp ++;
            if ( codePositionTemp >= codeLength || ( codeString [ codePositionTemp ] != '.' && codeString [ codePositionTemp ] != 'E' && codeString [ codePositionTemp ] != 'e' && ( codeString [ codePositionTemp ] < '0' || codeString [ codePositionTemp ] > '9' ) ) )
                break;
        }
        wordLength = thisWord . length ( );
        wordKind = 1;
    }
    else if ( firstLetter == '(' || firstLetter == ')' || firstLetter == '[' || firstLetter == ']' || firstLetter == '{' || firstLetter == '}')
    {
        thisWord = "";
        thisWord += firstLetter;
        wordLength = 1;
        wordKind = 2;
    }
    else if ( firstLetter == '\'' )
    {
        if ( secondLetter == '\'' )
            thisWord = "'\''";
        else if ( secondLetter == '\\' && thirdLetter == '\'' )
            thisWord = "'\\''";
        else
        {
            thisWord = "";
            for ( codePositionTemp = codePosition ; ; )
            {
                thisWord += codeString [ codePositionTemp ];
                codePositionTemp ++;
                if ( codePositionTemp >= codeLength || codeString [ codePositionTemp ] == '\n' || codeString [ codePositionTemp ] == '\r' )
                    break;
                else if ( codeString [ codePositionTemp ] == '\'' )
                {
                    thisWord += codeString [ codePositionTemp ];
                    break;
                }
            }
        }
        wordLength = thisWord . length ( );
        wordKind = 3;
    }
    else if ( firstLetter == '"' )
    {
        thisWord = "";
        thisWord += codeString [ codePosition ];
        for ( codePositionTemp = codePosition + 1 , escapeTemp = 0 ; ; codePositionTemp ++ )
        {
            if ( codePositionTemp >= codeLength || codeString [ codePositionTemp ] == '\n' || codeString [ codePositionTemp ] == '\r' )
                break;
            else if ( escapeTemp == 0 && codeString [ codePositionTemp ] == '"' )
            {
                thisWord += codeString [ codePositionTemp ];
                break;
            }
            else if ( escapeTemp == 1 && codePositionTemp < codeLength && codeString [ codePositionTemp ] != '\n' && codeString [ codePositionTemp ] != '\r' )
            {
                thisWord += codeString [ codePositionTemp ];
                escapeTemp --;
            }
            else if ( escapeTemp == 0 && codeString [ codePositionTemp ] == '\\' )
            {
                thisWord += codeString [ codePositionTemp ];
                escapeTemp ++;
            }
            else
                thisWord += codeString [ codePositionTemp ];
        }
        wordLength = thisWord . length ( );
        wordKind = 3;
    }
    else if ( firstLetter == '#' )
    {
        thisWord = "#";
        wordLength = 1;
        wordKind = 4;
    }
    else if ( firstLetter == ':' )
    {
        thisWord = ":";
        wordLength = 1;
        wordKind = 5;
    }
    else if ( firstLetter == ';' )
    {
        thisWord = ";";
        wordLength = 1;
        wordKind = 6;
    }
    else if ( ( firstLetter >= 'a' && firstLetter <= 'z' ) || ( firstLetter >= 'A' && firstLetter <= 'Z' ) || firstLetter == '_' )
    {
        thisWord = "";
        for ( codePositionTemp = codePosition ; ; )
        {
            thisWord += codeString [ codePositionTemp ];
            codePositionTemp ++;
            if ( codePositionTemp >= codeLength || ( codeString [ codePositionTemp ] != '_' && ( codeString [ codePositionTemp ] < 'a' || codeString [ codePositionTemp ] > 'z' ) && ( codeString [ codePositionTemp ] < 'A' || codeString [ codePositionTemp ] > 'Z' ) && ( codeString [ codePositionTemp ] < '0' || codeString [ codePositionTemp ] > '9' ) ) )
                break;
        }
        wordLength = thisWord . length ( );
        wordKind = 8;
    }
    else if ( firstLetter == '/' && secondLetter == '/' )
    {
        thisWord = "";
        for ( codePositionTemp = codePosition ; ; )
        {
            thisWord += codeString [ codePositionTemp ];
            codePositionTemp ++;
            if ( codePositionTemp >= codeLength || codeString [ codePositionTemp ] == '\n' || codeString [ codePositionTemp ] == '\r' )
                break;
        }
        wordLength = thisWord . length ( );
        wordKind = 9;
    }
    else if ( firstLetter == '/' && secondLetter == '*' )
    {
        thisWord = "/*";
        for ( codePositionTemp = codePosition + 2 ; ; codePositionTemp ++ )
        {
            if ( codePositionTemp >= codeLength )
                break;
            else if ( codePositionTemp + 1 >= codeLength )
            {
                thisWord += codeString [ codePositionTemp ];
                break;
            }
            else if ( codeString [ codePositionTemp ] == '*' && codeString [ codePositionTemp + 1 ] == '/' )
            {
                thisWord += "*/";
                break;
            }
            else
                thisWord += codeString [ codePositionTemp ];
        }
        wordLength = thisWord . length ( );
        wordKind = 9;
    }
    else
    {
        thisWord = "";
        thisWord += firstLetter;
        if ( firstLetter == '!' && secondLetter == '=' )
            thisWord += secondLetter;
        else if ( firstLetter == '+' && ( secondLetter == '+' || secondLetter == '=' ) )
            thisWord += secondLetter;
        else if ( firstLetter == '-' && ( secondLetter == '-' || secondLetter == '=' || secondLetter == '>' ) )
            thisWord += secondLetter;
        else if ( firstLetter == '*' && secondLetter == '=' )
            thisWord += secondLetter;
        else if ( firstLetter == '&' && ( secondLetter == '&' || secondLetter == '=' ) )
            thisWord += secondLetter;
        else if ( firstLetter == '/' && secondLetter == '=' )
            thisWord += secondLetter;
        else if ( firstLetter == '%' && secondLetter == '=' )
            thisWord += secondLetter;
        else if ( firstLetter == '<' )
        {
            if ( secondLetter == '<' )
            {
                thisWord += secondLetter;
                if( thirdLetter == '=' )
                    thisWord += thirdLetter;
            }
            else if ( secondLetter == '=' )
                thisWord += secondLetter;
        }
        else if ( firstLetter == '>' )
        {
            if ( secondLetter == '>' )
            {
                thisWord += secondLetter;
                if( thirdLetter == '=' )
                    thisWord += thirdLetter;
            }
            else if ( secondLetter == '=' )
                thisWord += secondLetter;
        }
        else if ( firstLetter == '^' && secondLetter == '=' )
            thisWord += secondLetter;
        else if ( firstLetter == '|' && ( secondLetter == '|' || secondLetter == '=' ) )
            thisWord += secondLetter;
        else if ( firstLetter == '=' && secondLetter == '=' )
            thisWord += secondLetter;
        wordLength = thisWord . length ( );
        wordKind = 7;
    }
    return thisWord;
}

struct LineHeadPointer * MemoryCenter :: analysisBraces ( const QString & firstWord )
{
    struct LineHeadPointer *memoryHeadBuild;
    struct LexicalAnalysis *wordBuild;
    memoryHeadBuild = new ( struct LineHeadPointer );
    wordBuild = new ( struct LexicalAnalysis );
    memoryHeadBuild -> logicLevel = 0;
    memoryHeadBuild -> firstWord = wordBuild;
    memoryHeadBuild -> nextLine = NULL;
    wordBuild -> whichLine = memoryHeadBuild;
    wordBuild -> wordContent = firstWord;
    wordBuild -> wordLength = firstWord . length ( );
    wordBuild -> previousWord = NULL;
    wordBuild -> nextWord = NULL;
    if ( firstWord == "{" )
        wordBuild -> wordCategory = 2;
    else
        wordBuild -> wordCategory = 3;
    return memoryHeadBuild;
}

struct LineHeadPointer * MemoryCenter :: analysisMacro ( const QString codeString , const QString & firstWord , int & codePosition )
{
    struct LineHeadPointer *memoryHeadBuild;
    struct LexicalAnalysis *wordBuild , *wordTemp;
    int wordLength , wordKind , codeLength , codePositionTemp;
    QString thisWord;
    wordLength = 0;
    wordKind = 0;
    codeLength = codeString . length ( );
    memoryHeadBuild = new ( struct LineHeadPointer );
    wordBuild = new ( struct LexicalAnalysis );
    memoryHeadBuild -> logicLevel = 0;
    memoryHeadBuild -> firstWord = wordBuild;
    memoryHeadBuild -> nextLine = NULL;
    wordBuild -> whichLine = memoryHeadBuild;
    wordBuild -> wordContent = firstWord;
    wordBuild -> wordLength = firstWord . length ( );
    wordBuild -> previousWord = NULL;
    wordBuild -> nextWord = NULL;
    wordBuild -> wordCategory = 10;
    wordTemp = wordBuild;
    for( ; ; )
    {
        thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
        if ( codePosition >= codeLength )
            return memoryHeadBuild;
        else if ( !wordKind )
            codePosition += wordLength;
        else
            break;
    }
    if( wordKind != 8 )
        return memoryHeadBuild;
    if ( thisWord == "include" )
    {
        codePositionTemp = codePosition;
        codePosition += wordLength;
        wordBuild = new ( struct LexicalAnalysis );
        wordBuild -> whichLine = memoryHeadBuild;
        wordBuild -> wordContent = thisWord;
        wordBuild -> wordLength = wordLength;
        wordBuild -> wordPosition = codePositionTemp;
        wordBuild -> previousWord = wordTemp;
        wordBuild -> nextWord = NULL;
        wordBuild -> wordCategory = 50;
        wordTemp -> nextWord = wordBuild;
        wordTemp = wordBuild;
        for ( ; ; )
        {
            thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
            if ( codePosition >= codeLength )
                return memoryHeadBuild;
            else if ( !wordKind )
                codePosition += wordLength;
            else
                break;
        }
        if ( wordKind == 3)
        {
            codePositionTemp = codePosition;
            codePosition += wordLength;
            wordBuild = new ( struct LexicalAnalysis );
            wordBuild -> whichLine = memoryHeadBuild;
            wordBuild -> wordContent = thisWord;
            wordBuild -> wordLength = wordLength;
            wordBuild -> wordPosition = codePositionTemp;
            wordBuild -> previousWord = wordTemp;
            wordBuild -> nextWord = NULL;
            wordBuild -> wordCategory = 9;
            wordTemp -> nextWord = wordBuild;
            return memoryHeadBuild;
        }
        else if ( thisWord == "<" )
        {
            for ( codePositionTemp = codePosition + 1 ; codePositionTemp < codeLength ; codePositionTemp ++ )
            {
                if ( codeString [ codePositionTemp ] == '\n' || codeString [ codePositionTemp ] == '\r' )
                    break;
                else if ( codeString [ codePositionTemp ] == '>' )
                {
                    thisWord += codeString [ codePositionTemp ];
                    break;
                }
                else
                    thisWord += codeString [ codePositionTemp ];
            }
            codePositionTemp = codePosition;
            codePosition += thisWord . length ( );
            wordBuild = new ( struct LexicalAnalysis );
            wordBuild -> whichLine = memoryHeadBuild;
            wordBuild -> wordContent = thisWord;
            wordBuild -> wordLength = thisWord . length ( );
            wordBuild -> wordPosition = codePositionTemp;
            wordBuild -> previousWord = wordTemp;
            wordBuild -> nextWord = NULL;
            wordBuild -> wordCategory = 9;
            wordTemp -> nextWord = wordBuild;
            return memoryHeadBuild;
        }
        else
            return memoryHeadBuild;
    }
    else if ( thisWord == "define" )
    {
        codePositionTemp = codePosition;
        codePosition += wordLength;
        wordBuild = new ( struct LexicalAnalysis );
        wordBuild -> whichLine = memoryHeadBuild;
        wordBuild -> wordContent = thisWord;
        wordBuild -> wordLength = wordLength;
        wordBuild -> wordPosition = codePositionTemp;
        wordBuild -> previousWord = wordTemp;
        wordBuild -> nextWord = NULL;
        wordBuild -> wordCategory = 51;
        wordTemp -> nextWord = wordBuild;
        wordTemp = wordBuild;
        for ( ; ; )
        {
            thisWord = "";
            for ( codePositionTemp = codePosition ; codePositionTemp < codeLength ; codePositionTemp ++ )
            {
                if ( codeString [ codePositionTemp ] == '\n' || codeString [ codePositionTemp ] == '\r' )
                    break;
                else
                    thisWord += codeString [ codePositionTemp ];
            }
            if ( thisWord == "" )
                return memoryHeadBuild;
            wordBuild = new ( struct LexicalAnalysis );
            wordBuild -> whichLine = memoryHeadBuild;
            wordBuild -> wordContent = thisWord;
            wordBuild -> wordLength = thisWord . length ( );
            wordBuild -> wordPosition = codePosition;
            wordBuild -> previousWord = wordTemp;
            wordBuild -> nextWord = NULL;
            wordBuild -> wordCategory = 9;
            wordTemp -> nextWord = wordBuild;
            wordTemp = wordBuild;
            codePosition = codePositionTemp;
            return memoryHeadBuild;
        }
    }
    else if ( thisWord == "ifndef" || thisWord == "if" )
    {
        codePositionTemp = codePosition;
        codePosition += wordLength;
        wordBuild = new ( struct LexicalAnalysis );
        wordBuild -> whichLine = memoryHeadBuild;
        wordBuild -> wordContent = thisWord;
        wordBuild -> wordLength = wordLength;
        wordBuild -> wordPosition = codePositionTemp;
        wordBuild -> previousWord = wordTemp;
        wordBuild -> nextWord = NULL;
        wordBuild -> wordCategory = 52;
        wordTemp -> nextWord = wordBuild;
        wordTemp = wordBuild;
        for ( ; ; )
        {
            thisWord = "";
            for ( codePositionTemp = codePosition ; codePositionTemp < codeLength ; codePositionTemp ++ )
            {
                if ( codeString [ codePositionTemp ] == '\n' || codeString [ codePositionTemp ] == '\r' )
                    break;
                else
                    thisWord += codeString [ codePositionTemp ];
            }
            if ( thisWord == "" )
                return memoryHeadBuild;
            wordBuild = new ( struct LexicalAnalysis );
            wordBuild -> whichLine = memoryHeadBuild;
            wordBuild -> wordContent = thisWord;
            wordBuild -> wordLength = thisWord . length ( );
            wordBuild -> wordPosition = codePosition;
            wordBuild -> previousWord = wordTemp;
            wordBuild -> nextWord = NULL;
            wordBuild -> wordCategory = 9;
            wordTemp -> nextWord = wordBuild;
            wordTemp = wordBuild;
            codePosition = codePositionTemp;
            return memoryHeadBuild;
        }
    }
    else if ( thisWord == "endif" )
    {
        codePositionTemp = codePosition;
        codePosition += wordLength;
        wordBuild = new ( struct LexicalAnalysis );
        wordBuild -> whichLine = memoryHeadBuild;
        wordBuild -> wordContent = thisWord;
        wordBuild -> wordLength = wordLength;
        wordBuild -> wordPosition = codePositionTemp;
        wordBuild -> previousWord = wordTemp;
        wordBuild -> nextWord = NULL;
        wordBuild -> wordCategory = 52;
        wordTemp -> nextWord = wordBuild;
        return memoryHeadBuild;
    }
    else
    {
        codePositionTemp = codePosition;
        codePosition += wordLength;
        wordBuild = new ( struct LexicalAnalysis );
        wordBuild -> whichLine = memoryHeadBuild;
        wordBuild -> wordContent = thisWord;
        wordBuild -> wordLength = wordLength;
        wordBuild -> wordPosition = codePositionTemp;
        wordBuild -> previousWord = wordTemp;
        wordBuild -> nextWord = NULL;
        wordBuild -> wordCategory = 9;
        wordTemp -> nextWord = wordBuild;
        wordTemp = wordBuild;
        for ( ; ; )
        {
            thisWord = "";
            for ( codePositionTemp = codePosition + 1 ; codePositionTemp < codeLength ; codePositionTemp ++ )
            {
                if ( codeString [ codePositionTemp ] == '\n' || codeString [ codePositionTemp ] == '\r' )
                    break;
                else
                    thisWord += codeString [ codePositionTemp ];
            }
            if ( thisWord == "" )
                return memoryHeadBuild;
            wordBuild = new ( struct LexicalAnalysis );
            wordBuild -> whichLine = memoryHeadBuild;
            wordBuild -> wordContent = thisWord;
            wordBuild -> wordLength = thisWord . length ( );
            wordBuild -> wordPosition = codePosition + 1;
            wordBuild -> previousWord = wordTemp;
            wordBuild -> nextWord = NULL;
            wordBuild -> wordCategory = 9;
            wordTemp -> nextWord = wordBuild;
            wordTemp = wordBuild;
            codePosition = codePositionTemp;
            return memoryHeadBuild;
        }
    }
    return memoryHeadBuild;
}

struct LineHeadPointer * MemoryCenter :: analysisSemicolon ( const QString codeString , const QString & firstWord , int & codePosition )
{
    struct LineHeadPointer *memoryHeadBuild;
    struct LexicalAnalysis *wordBuild;
    memoryHeadBuild = new ( struct LineHeadPointer );
    wordBuild = new ( struct LexicalAnalysis );
    memoryHeadBuild -> logicLevel = 0;
    memoryHeadBuild -> firstWord = wordBuild;
    memoryHeadBuild -> nextLine = NULL;
    wordBuild -> whichLine = memoryHeadBuild;
    wordBuild -> wordContent = firstWord;
    wordBuild -> wordLength = firstWord . length ( );
    wordBuild -> previousWord = NULL;
    wordBuild -> nextWord = NULL;
    wordBuild -> wordCategory = 12;
    return memoryHeadBuild;
}

struct LineHeadPointer * MemoryCenter :: analysisRemark ( const QString codeString , const QString & firstWord , int & codePosition )
{
    struct LineHeadPointer *memoryHeadBuild;
    struct LexicalAnalysis *wordBuild;
    memoryHeadBuild = new ( struct LineHeadPointer );
    wordBuild = new ( struct LexicalAnalysis );
    memoryHeadBuild -> logicLevel = 0;
    memoryHeadBuild -> firstWord = wordBuild;
    memoryHeadBuild -> nextLine = NULL;
    wordBuild -> whichLine = memoryHeadBuild;
    wordBuild -> wordContent = firstWord;
    wordBuild -> wordLength = firstWord . length ( );
    wordBuild -> previousWord = NULL;
    wordBuild -> nextWord = NULL;
    if ( firstWord [ 1 ] == '/' )
        wordBuild -> wordCategory = 90;
    else
        wordBuild -> wordCategory = 91;
    return memoryHeadBuild;
}

struct LineHeadPointer * MemoryCenter :: analysisOther ( const QString codeString , const QString & firstWord , const int & firstWordKind , int & codePosition )
{
    struct LineHeadPointer *memoryHeadBuild;
    struct LexicalAnalysis *wordBuild , *wordTemp;
    int wordLength , wordKind , codeLength , codePositionTemp;
    QString thisWord;
    wordLength = 0;
    wordKind = 0;
    codeLength = codeString . length ( );
    memoryHeadBuild = new ( struct LineHeadPointer );
    wordBuild = new ( struct LexicalAnalysis );
    memoryHeadBuild -> logicLevel = 0;
    memoryHeadBuild -> firstWord = wordBuild;
    memoryHeadBuild -> nextLine = NULL;
    wordBuild -> whichLine = memoryHeadBuild;
    wordBuild -> wordContent = firstWord;
    wordBuild -> wordLength = firstWord . length ( );
    wordBuild -> previousWord = NULL;
    wordBuild -> nextWord = NULL;
    if ( firstWordKind == 1 )
        wordBuild -> wordCategory = 1;
    else if ( firstWordKind == 2 )
        wordBuild -> wordCategory = judgeWordCategory ( firstWord , 4 , 7 );
    else if ( firstWordKind == 3 && firstWord [ 0 ] == '\'' )
        wordBuild -> wordCategory = 8;
    else if ( firstWordKind == 3 && firstWord [ 0 ] == '"' )
        wordBuild -> wordCategory = 9;
    else if ( firstWordKind == 7 )
        wordBuild -> wordCategory = judgeWordCategory ( firstWord , 10 , 49 );
    wordTemp = wordBuild;
    for ( ; codePosition < codeLength ; )
    {
        thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
        if ( wordKind == 0 )
        {
            codePosition += wordLength;
            continue;
        }
        else
        {
            codePositionTemp = codePosition;
            codePosition += wordLength;
            wordBuild = new ( struct LexicalAnalysis );
            wordBuild -> whichLine = memoryHeadBuild;
            wordBuild -> wordContent = thisWord;
            wordBuild -> wordLength = wordLength;
            wordBuild -> wordPosition = codePositionTemp;
            wordBuild -> previousWord = wordTemp;
            wordBuild -> nextWord = NULL;
            wordTemp -> nextWord = wordBuild;
            wordTemp = wordBuild;
            if ( wordKind == 1 )
                wordBuild -> wordCategory = 1;
            else if ( wordKind == 2 )
                wordBuild -> wordCategory = judgeWordCategory ( thisWord , 2 , 7 );
            else if ( wordKind == 3 && thisWord [ 0 ] == '\'' )
                wordBuild -> wordCategory = 8;
            else if ( wordKind == 3 && thisWord [ 0 ] == '"' )
                wordBuild -> wordCategory = 9;
            else if ( wordKind == 4 )
                wordBuild -> wordCategory = 10;
            else if ( wordKind == 5 )
                wordBuild -> wordCategory = 11;
            else if ( wordKind == 6 )
            {
                wordBuild -> wordCategory = 12;
                break;
            }
            else if ( wordKind == 7 )
                wordBuild -> wordCategory = judgeWordCategory ( thisWord , 10 , 49 );
            else if ( wordKind == 8 )
                wordBuild -> wordCategory = judgeWordCategory ( thisWord , 50 , 89 );
            else if ( wordKind == 9 && thisWord [ 1 ] == '/' )
                wordBuild -> wordCategory = 90;
            else if ( wordKind == 9 && thisWord [ 1 ] == '*' )
                wordBuild -> wordCategory = 91;
        }
    }
    return memoryHeadBuild;
}

struct LineHeadPointer * MemoryCenter :: analysisKey ( const QString codeString , const QString & firstWord , int & codePosition )
{
    struct LineHeadPointer *memoryHeadBuild;
    struct LexicalAnalysis *wordBuild , *wordTemp;
    int wordLength , wordKind , codeLength , wordCategoryTemp , parenthesesTemp , codePositionTemp;
    QString thisWord;
    wordKind = 0;
    wordLength = 0;
    wordCategoryTemp = 92;
    codeLength = codeString . length ( );
    memoryHeadBuild = new ( struct LineHeadPointer );
    wordBuild = new ( struct LexicalAnalysis );
    memoryHeadBuild -> logicLevel = 0;
    memoryHeadBuild -> firstWord = wordBuild;
    memoryHeadBuild -> nextLine = NULL;
    wordBuild -> whichLine = memoryHeadBuild;
    wordBuild -> wordContent = firstWord;
    wordBuild -> wordLength = firstWord . length ( );
    wordBuild -> previousWord = NULL;
    wordBuild -> nextWord = NULL;
    wordBuild -> wordCategory = judgeWordCategory ( firstWord , 54 , 89 );
    wordTemp = wordBuild;
    if ( wordBuild -> wordCategory == 62 )
    {
        for ( ; codePosition < codeLength ; )
        {
            thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
            if ( wordKind == 0 )
            {
                codePosition += wordLength;
                continue;
            }
            else
                break;
        }
        wordCategoryTemp = judgeWordCategory ( thisWord , 54 , 89 );
        if ( wordCategoryTemp != 61 )
            return memoryHeadBuild;
        else
        {
            codePositionTemp = codePosition;
            codePosition += wordLength;
            wordBuild = new ( struct LexicalAnalysis );
            wordBuild -> whichLine = memoryHeadBuild;
            wordBuild -> wordContent = thisWord;
            wordBuild -> wordLength = wordLength;
            wordBuild -> wordPosition = codePositionTemp;
            wordBuild -> previousWord = wordTemp;
            wordBuild -> nextWord = NULL;
            wordBuild -> wordCategory = 61;
            wordTemp -> nextWord = wordBuild;
            wordTemp = wordBuild;
            for ( ; codePosition < codeLength ; )
            {
                thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
                if ( wordKind == 0 )
                {
                    codePosition += wordLength;
                    continue;
                }
                else
                    break;
            }
            wordCategoryTemp = judgeWordCategory ( thisWord , 2 , 7 );
            if ( wordCategoryTemp != 6 )
                return memoryHeadBuild;
            else
            {
                codePositionTemp = codePosition;
                codePosition += wordLength;
                wordBuild = new ( struct LexicalAnalysis );
                wordBuild -> whichLine = memoryHeadBuild;
                wordBuild -> wordContent = thisWord;
                wordBuild -> wordLength = wordLength;
                wordBuild -> wordPosition = codePositionTemp;
                wordBuild -> previousWord = wordTemp;
                wordBuild -> nextWord = NULL;
                wordBuild -> wordCategory = 6;
                wordTemp -> nextWord = wordBuild;
                wordTemp = wordBuild;
                for ( parenthesesTemp = 1 ; codePosition < codeLength ; )
                {
                    thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
                    if ( wordKind == 0 )
                    {
                        codePosition += wordLength;
                        continue;
                    }
                    else
                    {
                        codePositionTemp = codePosition;
                        codePosition += wordLength;
                        wordBuild = new ( struct LexicalAnalysis );
                        wordBuild -> whichLine = memoryHeadBuild;
                        wordBuild -> wordContent = thisWord;
                        wordBuild -> wordLength = wordLength;
                        wordBuild -> wordPosition = codePositionTemp;
                        wordBuild -> previousWord = wordTemp;
                        wordBuild -> nextWord = NULL;
                        wordTemp -> nextWord = wordBuild;
                        wordTemp = wordBuild;
                        if ( wordKind == 1 )
                            wordBuild -> wordCategory = 1;
                        else if ( wordKind == 2 )
                            wordBuild -> wordCategory = judgeWordCategory ( thisWord , 2 , 7 );
                        else if ( wordKind == 3 && thisWord [ 0 ] == '\'' )
                            wordBuild -> wordCategory = 8;
                        else if ( wordKind == 3 && thisWord [ 0 ] == '"' )
                            wordBuild -> wordCategory = 9;
                        else if ( wordKind >= 4 && wordKind <= 7 )
                            wordBuild -> wordCategory = judgeWordCategory ( thisWord , 10 , 49 );
                        else if ( wordKind == 8 )
                            wordBuild -> wordCategory = judgeWordCategory ( thisWord , 50 , 89 );
                        else if ( wordKind == 9 && thisWord [ 1 ] == '/' )
                            wordBuild -> wordCategory = 90;
                        else if ( wordKind == 9 && thisWord [ 1 ] == '*' )
                            wordBuild -> wordCategory = 91;
                        if ( wordBuild -> wordCategory == 6 )
                            parenthesesTemp ++;
                        else if ( wordBuild -> wordCategory == 7 )
                        {
                            parenthesesTemp --;
                            if ( parenthesesTemp == 0 )
                                return memoryHeadBuild;
                        }
                    }
                }
            }
        }
    }
    else if ( wordBuild -> wordCategory == 55 || wordBuild -> wordCategory == 56 )
        for ( ; codePosition < codeLength ; )
        {
            thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
            if ( wordKind == 0 )
            {
                codePosition += wordLength;
                continue;
            }
            else
            {
                codePositionTemp = codePosition;
                codePosition += wordLength;
                wordBuild = new ( struct LexicalAnalysis );
                wordBuild -> whichLine = memoryHeadBuild;
                wordBuild -> wordContent = thisWord;
                wordBuild -> wordLength = wordLength;
                wordBuild -> wordPosition = codePositionTemp;
                wordBuild -> previousWord = wordTemp;
                wordBuild -> nextWord = NULL;
                wordTemp -> nextWord = wordBuild;
                wordTemp = wordBuild;
                if ( wordKind == 1 )
                    wordBuild -> wordCategory = 1;
                else if ( wordKind == 2 )
                    wordBuild -> wordCategory = judgeWordCategory ( thisWord , 2 , 7 );
                else if ( wordKind == 3 && thisWord [ 0 ] == '\'' )
                    wordBuild -> wordCategory = 8;
                else if ( wordKind == 3 && thisWord [ 0 ] == '"' )
                    wordBuild -> wordCategory = 9;
                else if ( wordKind >= 4 && wordKind <= 7 )
                    wordBuild -> wordCategory = judgeWordCategory ( thisWord , 10 , 49 );
                else if ( wordKind == 8 )
                    wordBuild -> wordCategory = judgeWordCategory ( thisWord , 50 , 89 );
                else if ( wordKind == 9 && thisWord [ 1 ] == '/' )
                    wordBuild -> wordCategory = 90;
                else if ( wordKind == 9 && thisWord [ 1 ] == '*' )
                     wordBuild -> wordCategory = 91;
                if ( wordBuild -> wordCategory == 11 )
                    return memoryHeadBuild;
            }
        }
    else if ( wordBuild -> wordCategory == 64 )
        return memoryHeadBuild;
    else if ( wordBuild -> wordCategory == 61 || wordBuild -> wordCategory == 63 || wordBuild -> wordCategory == 65 || wordBuild -> wordCategory == 66 )
    {
        for ( ; codePosition < codeLength ; )
        {
            thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
            if ( wordKind == 0 )
            {
                codePosition += wordLength;
                continue;
            }
            else
                break;
        }
        wordCategoryTemp = judgeWordCategory ( thisWord , 2 , 7 );
        if ( wordCategoryTemp != 6 )
            return memoryHeadBuild;
        else
        {
            codePositionTemp = codePosition;
            codePosition += wordLength;
            wordBuild = new ( struct LexicalAnalysis );
            wordBuild -> whichLine = memoryHeadBuild;
            wordBuild -> wordContent = thisWord;
            wordBuild -> wordLength = wordLength;
            wordBuild -> wordPosition = codePositionTemp;
            wordBuild -> previousWord = wordTemp;
            wordBuild -> nextWord = NULL;
            wordBuild -> wordCategory = 6;
            wordTemp -> nextWord = wordBuild;
            wordTemp = wordBuild;
            for ( parenthesesTemp = 1 ; codePosition < codeLength ; )
            {
                thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
                if ( wordKind == 0 )
                {
                    codePosition += wordLength;
                    continue;
                }
                else
                {
                    codePositionTemp = codePosition;
                    codePosition += wordLength;
                    wordBuild = new ( struct LexicalAnalysis );
                    wordBuild -> whichLine = memoryHeadBuild;
                    wordBuild -> wordContent = thisWord;
                    wordBuild -> wordLength = wordLength;
                    wordBuild -> wordPosition = codePositionTemp;
                    wordBuild -> previousWord = wordTemp;
                    wordBuild -> nextWord = NULL;
                    wordTemp -> nextWord = wordBuild;
                    wordTemp = wordBuild;
                    if ( wordKind == 1 )
                        wordBuild -> wordCategory = 1;
                    else if ( wordKind == 2 )
                        wordBuild -> wordCategory = judgeWordCategory ( thisWord , 2 , 7 );
                    else if ( wordKind == 3 && thisWord [ 0 ] == '\'' )
                        wordBuild -> wordCategory = 8;
                    else if ( wordKind == 3 && thisWord [ 0 ] == '"' )
                        wordBuild -> wordCategory = 9;
                    else if ( wordKind >= 4 && wordKind <= 7 )
                        wordBuild -> wordCategory = judgeWordCategory ( thisWord , 10 , 49 );
                    else if ( wordKind == 8 )
                        wordBuild -> wordCategory = judgeWordCategory ( thisWord , 50 , 89 );
                    else if ( wordKind == 9 && thisWord [ 1 ] == '/' )
                        wordBuild -> wordCategory = 90;
                    else if ( wordKind == 9 && thisWord [ 1 ] == '*' )
                        wordBuild -> wordCategory = 91;
                    if ( wordBuild -> wordCategory == 6 )
                        parenthesesTemp ++;
                    else if ( wordBuild -> wordCategory == 7 )
                    {
                        parenthesesTemp --;
                        if ( parenthesesTemp == 0 )
                            return memoryHeadBuild;
                    }
                }
            }
        }
    }
    else if ( wordBuild -> wordCategory == 57 )
    {
        for ( ; codePosition < codeLength ; )
        {
            thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
            if ( wordKind == 0 )
            {
                codePosition += wordLength;
                continue;
            }
            else
                break;
        }
        wordCategoryTemp = judgeWordCategory ( thisWord , 50 , 89 );
        if ( wordCategoryTemp < 58 || wordCategoryTemp > 60 )
        {
            codePositionTemp = codePosition;
            codePosition += wordLength;
            wordBuild = new ( struct LexicalAnalysis );
            wordBuild -> whichLine = memoryHeadBuild;
            wordBuild -> wordContent = thisWord;
            wordBuild -> wordLength = wordLength;
            wordBuild -> wordPosition = codePositionTemp;
            wordBuild -> previousWord = wordTemp;
            wordBuild -> nextWord = NULL;
            wordBuild -> wordCategory = 6;
            wordTemp -> nextWord = wordBuild;
            wordTemp = wordBuild;
            for ( ; codePosition < codeLength ; )
            {
                thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
                if ( wordKind == 0 )
                {
                    codePosition += wordLength;
                    continue;
                }
                else
                {
                    codePositionTemp = codePosition;
                    codePosition += wordLength;
                    wordBuild = new ( struct LexicalAnalysis );
                    wordBuild -> whichLine = memoryHeadBuild;
                    wordBuild -> wordContent = thisWord;
                    wordBuild -> wordLength = wordLength;
                    wordBuild -> wordPosition = codePositionTemp;
                    wordBuild -> previousWord = wordTemp;
                    wordBuild -> nextWord = NULL;
                    wordTemp -> nextWord = wordBuild;
                    wordTemp = wordBuild;
                    if ( wordKind == 1 )
                        wordBuild -> wordCategory = 1;
                    else if ( wordKind == 2 )
                        wordBuild -> wordCategory = judgeWordCategory ( thisWord , 2 , 7 );
                    else if ( wordKind == 3 && thisWord [ 0 ] == '\'' )
                        wordBuild -> wordCategory = 8;
                    else if ( wordKind == 3 && thisWord [ 0 ] == '"' )
                        wordBuild -> wordCategory = 9;
                    else if ( wordKind >= 4 && wordKind <= 7 )
                        wordBuild -> wordCategory = judgeWordCategory ( thisWord , 10 , 49 );
                    else if ( wordKind == 8 )
                        wordBuild -> wordCategory = judgeWordCategory ( thisWord , 50 , 89 );
                    else if ( wordKind == 9 && thisWord [ 1 ] == '/' )
                        wordBuild -> wordCategory = 90;
                    else if ( wordKind == 9 && thisWord [ 1 ] == '*' )
                        wordBuild -> wordCategory = 91;
                    if ( wordBuild -> wordCategory == 12 )
                        return memoryHeadBuild;
                }
            }
        }
        else
        {
            codePositionTemp = codePosition;
            codePosition += wordLength;
            wordBuild = new ( struct LexicalAnalysis );
            wordBuild -> whichLine = memoryHeadBuild;
            wordBuild -> wordContent = thisWord;
            wordBuild -> wordLength = wordLength;
            wordBuild -> wordPosition = codePositionTemp;
            wordBuild -> previousWord = wordTemp;
            wordBuild -> nextWord = NULL;
            wordBuild -> wordCategory = 6;
            wordTemp -> nextWord = wordBuild;
            wordTemp = wordBuild;
            for ( ; codePosition < codeLength ; )
            {
                thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
                if ( wordKind == 0 )
                {
                    codePosition += wordLength;
                    continue;
                }
                else
                {
                    if ( wordKind == 1 )
                        wordCategoryTemp = 1;
                    else if ( wordKind == 2 )
                        wordCategoryTemp = judgeWordCategory ( thisWord , 2 , 7 );
                    else if ( wordKind == 3 && thisWord [ 0 ] == '\'' )
                        wordCategoryTemp = 8;
                    else if ( wordKind == 3 && thisWord [ 0 ] == '"' )
                        wordCategoryTemp = 9;
                    else if ( wordKind >= 4 && wordKind <= 7 )
                        wordCategoryTemp = judgeWordCategory ( thisWord , 10 , 49 );
                    else if ( wordKind == 8 )
                        wordCategoryTemp = judgeWordCategory ( thisWord , 50 , 89 );
                    else if ( wordKind == 9 && thisWord [ 1 ] == '/' )
                        wordCategoryTemp = 90;
                    else if ( wordKind == 9 && thisWord [ 1 ] == '*' )
                        wordCategoryTemp = 91;
                    if ( wordCategoryTemp == 2 )
                        return memoryHeadBuild;
                    else
                    {
                        codePositionTemp = codePosition;
                        codePosition += wordLength;
                        wordBuild = new ( struct LexicalAnalysis );
                        wordBuild -> whichLine = memoryHeadBuild;
                        wordBuild -> wordContent = thisWord;
                        wordBuild -> wordLength = wordLength;
                        wordBuild -> wordPosition = codePositionTemp;
                        wordBuild -> previousWord = wordTemp;
                        wordBuild -> nextWord = NULL;
                        wordBuild -> wordCategory = wordCategoryTemp;
                        wordTemp -> nextWord = wordBuild;
                        wordTemp = wordBuild;
                    }
                }
            }
        }
    }
    else if ( wordBuild -> wordCategory >= 58 && wordBuild -> wordCategory <= 60 )
    {
        for ( ; codePosition < codeLength ; )
        {
            thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
            if ( wordKind == 0 )
            {
                codePosition += wordLength;
                continue;
            }
            else
            {
                if ( wordKind == 1 )
                    wordCategoryTemp = 1;
                else if ( wordKind == 2 )
                    wordCategoryTemp = judgeWordCategory ( thisWord , 2 , 7 );
                else if ( wordKind == 3 && thisWord [ 0 ] == '\'' )
                    wordCategoryTemp = 8;
                else if ( wordKind == 3 && thisWord [ 0 ] == '"' )
                    wordCategoryTemp = 9;
                else if ( wordKind >= 4 && wordKind <= 7 )
                    wordCategoryTemp = judgeWordCategory ( thisWord , 10 , 49 );
                else if ( wordKind == 8 )
                    wordCategoryTemp = judgeWordCategory ( thisWord , 50 , 89 );
                else if ( wordKind == 9 && thisWord [ 1 ] == '/' )
                    wordCategoryTemp = 90;
                else if ( wordKind == 9 && thisWord [ 1 ] == '*' )
                    wordCategoryTemp = 91;
                if ( wordCategoryTemp == 2 )
                    return memoryHeadBuild;
                else
                {
                    codePositionTemp = codePosition;
                    codePosition += wordLength;
                    wordBuild = new ( struct LexicalAnalysis );
                    wordBuild -> whichLine = memoryHeadBuild;
                    wordBuild -> wordContent = thisWord;
                    wordBuild -> wordLength = wordLength;
                    wordBuild -> wordPosition = codePositionTemp;
                    wordBuild -> previousWord = wordTemp;
                    wordBuild -> nextWord = NULL;
                    wordBuild -> wordCategory = wordCategoryTemp;
                    wordTemp -> nextWord = wordBuild;
                    wordTemp = wordBuild;
                }
                if ( wordCategoryTemp == 12 )
                    return memoryHeadBuild;
                else if ( wordCategoryTemp == 6 )
                    break;
            }
        }
        for ( parenthesesTemp = 1 ; codePosition < codeLength ; )
        {
            thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
            if ( wordKind == 0 )
            {
                codePosition += wordLength;
                continue;
            }
            else
            {
                codePositionTemp = codePosition;
                codePosition += wordLength;
                wordBuild = new ( struct LexicalAnalysis );
                wordBuild -> whichLine = memoryHeadBuild;
                wordBuild -> wordContent = thisWord;
                wordBuild -> wordLength = wordLength;
                wordBuild -> wordPosition = codePositionTemp;
                wordBuild -> previousWord = wordTemp;
                wordBuild -> nextWord = NULL;
                wordTemp -> nextWord = wordBuild;
                wordTemp = wordBuild;
                if ( wordKind == 1 )
                    wordBuild -> wordCategory = 1;
                else if ( wordKind == 2 )
                    wordBuild -> wordCategory = judgeWordCategory ( thisWord , 2 , 7 );
                else if ( wordKind == 3 && thisWord [ 0 ] == '\'' )
                    wordBuild -> wordCategory = 8;
                else if ( wordKind == 3 && thisWord [ 0 ] == '"' )
                    wordBuild -> wordCategory = 9;
                else if ( wordKind >= 4 && wordKind <= 7 )
                    wordBuild -> wordCategory = judgeWordCategory ( thisWord , 10 , 49 );
                else if ( wordKind == 8 )
                    wordBuild -> wordCategory = judgeWordCategory ( thisWord , 50 , 89 );
                else if ( wordKind == 9 && thisWord [ 1 ] == '/' )
                    wordBuild -> wordCategory = 90;
                else if ( wordKind == 9 && thisWord [ 1 ] == '*' )
                    wordBuild -> wordCategory = 91;
                if ( wordBuild -> wordCategory == 6 )
                    parenthesesTemp ++;
                else if ( wordBuild -> wordCategory == 7 )
                {
                    parenthesesTemp --;
                    if ( parenthesesTemp == 0 )
                        return memoryHeadBuild;
                }
            }
        }
    }
    else if ( wordBuild -> wordCategory >= 71 && wordBuild -> wordCategory <= 89 )
    {
        for ( ; codePosition < codeLength ; )
        {
            thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
            if ( wordKind == 0 )
            {
                codePosition += wordLength;
                continue;
            }
            else
            {
                codePositionTemp = codePosition;
                codePosition += wordLength;
                wordBuild = new ( struct LexicalAnalysis );
                wordBuild -> whichLine = memoryHeadBuild;
                wordBuild -> wordContent = thisWord;
                wordBuild -> wordLength = wordLength;
                wordBuild -> wordPosition = codePositionTemp;
                wordBuild -> previousWord = wordTemp;
                wordBuild -> nextWord = NULL;
                wordTemp -> nextWord = wordBuild;
                wordTemp = wordBuild;
                if ( wordKind == 1 )
                    wordBuild -> wordCategory = 1;
                else if ( wordKind == 2 )
                    wordBuild -> wordCategory = judgeWordCategory ( thisWord , 2 , 7 );
                else if ( wordKind == 3 && thisWord [ 0 ] == '\'' )
                    wordBuild -> wordCategory = 8;
                else if ( wordKind == 3 && thisWord [ 0 ] == '"' )
                    wordBuild -> wordCategory = 9;
                else if ( wordKind >= 4 && wordKind <= 7 )
                    wordBuild -> wordCategory = judgeWordCategory ( thisWord , 10 , 49 );
                else if ( wordKind == 8 )
                    wordBuild -> wordCategory = judgeWordCategory ( thisWord , 50 , 89 );
                else if ( wordKind == 9 && thisWord [ 1 ] == '/' )
                    wordBuild -> wordCategory = 90;
                else if ( wordKind == 9 && thisWord [ 1 ] == '*' )
                    wordBuild -> wordCategory = 91;
                if ( wordBuild -> wordCategory == 12 )
                    return memoryHeadBuild;
                else if ( wordBuild -> wordCategory == 6 )
                    break;
            }
        }
        for ( parenthesesTemp = 1 ; codePosition < codeLength ; )
        {
            thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
            if ( wordKind == 0 )
            {
                codePosition += wordLength;
                continue;
            }
            else
            {
                codePositionTemp = codePosition;
                codePosition += wordLength;
                wordBuild = new ( struct LexicalAnalysis );
                wordBuild -> whichLine = memoryHeadBuild;
                wordBuild -> wordContent = thisWord;
                wordBuild -> wordLength = wordLength;
                wordBuild -> wordPosition = codePositionTemp;
                wordBuild -> previousWord = wordTemp;
                wordBuild -> nextWord = NULL;
                wordTemp -> nextWord = wordBuild;
                wordTemp = wordBuild;
                if ( wordKind == 1 )
                    wordBuild -> wordCategory = 1;
                else if ( wordKind == 2 )
                    wordBuild -> wordCategory = judgeWordCategory ( thisWord , 2 , 7 );
                else if ( wordKind == 3 && thisWord [ 0 ] == '\'' )
                    wordBuild -> wordCategory = 8;
                else if ( wordKind == 3 && thisWord [ 0 ] == '"' )
                    wordBuild -> wordCategory = 9;
                else if ( wordKind >= 4 && wordKind <= 7 )
                    wordBuild -> wordCategory = judgeWordCategory ( thisWord , 10 , 49 );
                else if ( wordKind == 8 )
                    wordBuild -> wordCategory = judgeWordCategory ( thisWord , 50 , 89 );
                else if ( wordKind == 9 && thisWord [ 1 ] == '/' )
                    wordBuild -> wordCategory = 90;
                else if ( wordKind == 9 && thisWord [ 1 ] == '*' )
                    wordBuild -> wordCategory = 91;
                if ( wordBuild -> wordCategory == 6 )
                    parenthesesTemp ++;
                else if ( wordBuild -> wordCategory == 7 )
                {
                    parenthesesTemp --;
                    if ( parenthesesTemp == 0 )
                        return memoryHeadBuild;
                }
            }
        }
    }
    else
    {
        for ( ; codePosition < codeLength ; )
        {
            thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
            if ( wordKind == 0 )
            {
                codePosition += wordLength;
                continue;
            }
            else
            {
                if ( wordKind == 1 )
                    wordCategoryTemp = 1;
                else if ( wordKind == 2 )
                    wordCategoryTemp = judgeWordCategory ( thisWord , 2 , 7 );
                else if ( wordKind == 3 && thisWord [ 0 ] == '\'' )
                    wordCategoryTemp = 8;
                else if ( wordKind == 3 && thisWord [ 0 ] == '"' )
                    wordCategoryTemp = 9;
                else if ( wordKind >= 4 && wordKind <= 7 )
                    wordCategoryTemp = judgeWordCategory ( thisWord , 10 , 49 );
                else if ( wordKind == 8 )
                    wordCategoryTemp = judgeWordCategory ( thisWord , 50 , 89 );
                else if ( wordKind == 9 && thisWord [ 1 ] == '/' )
                    wordCategoryTemp = 90;
                else if ( wordKind == 9 && thisWord [ 1 ] == '*' )
                    wordCategoryTemp = 91;
                if ( wordCategoryTemp == 2 )
                    return memoryHeadBuild;
                else
                {
                    codePositionTemp = codePosition;
                    codePosition += wordLength;
                    wordBuild = new ( struct LexicalAnalysis );
                    wordBuild -> whichLine = memoryHeadBuild;
                    wordBuild -> wordContent = thisWord;
                    wordBuild -> wordLength = wordLength;
                    wordBuild -> wordPosition = codePositionTemp;
                    wordBuild -> previousWord = wordTemp;
                    wordBuild -> nextWord = NULL;
                    wordBuild -> wordCategory = wordCategoryTemp;
                    wordTemp -> nextWord = wordBuild;
                    wordTemp = wordBuild;
                    if ( wordBuild -> wordCategory == 12 )
                        return memoryHeadBuild;
                }
            }
        }
    }
    return memoryHeadBuild;
}

void MemoryCenter :: resetLogicLevel ( )
{
    struct LineHeadPointer *memoryTemp;
    for ( memoryTemp = memoryHead -> nextLine ; memoryTemp ; memoryTemp = memoryTemp -> nextLine )
        memoryTemp -> logicLevel = 0;
}

void MemoryCenter :: analysisLogicLevel ( )
{
    struct LineHeadPointer *memoryTemp1 , *memoryTemp2;
    int bracesTemp;
    for ( memoryTemp1 = memoryHead -> nextLine ; memoryTemp1 ; memoryTemp1 = memoryTemp1 -> nextLine )
    {
        if ( memoryTemp1 -> firstWord -> wordCategory >= 58 && memoryTemp1 -> firstWord ->wordCategory <= 66 && memoryTemp1 -> nextLine )
        {
            for ( memoryTemp2 = memoryTemp1 ; memoryTemp2 -> nextLine ; memoryTemp2 = memoryTemp2 -> nextLine )
                if ( memoryTemp2 -> nextLine -> firstWord -> wordCategory != 90 && memoryTemp2 -> nextLine -> firstWord -> wordCategory != 91 )
                    break;
                else
                    memoryTemp2 -> nextLine -> logicLevel = memoryTemp1 -> logicLevel;
            if ( memoryTemp2 -> nextLine && memoryTemp2 -> nextLine -> nextLine && memoryTemp2 -> nextLine -> firstWord -> wordCategory == 2 )
            {
                memoryTemp1 = memoryTemp2;
                memoryTemp1 -> nextLine -> logicLevel = memoryTemp1 -> logicLevel + 1;
                for ( memoryTemp2 = memoryTemp1 -> nextLine -> nextLine , bracesTemp = 1 ; memoryTemp2 && bracesTemp ; memoryTemp2 = memoryTemp2 -> nextLine )
                {
                    memoryTemp2 -> logicLevel = memoryTemp2 -> previousLine -> logicLevel;
                    if ( memoryTemp2 -> firstWord -> wordCategory == 2 )
                        bracesTemp ++;
                    else if ( memoryTemp2 -> firstWord -> wordCategory == 3 )
                        bracesTemp --;
                }
            }
            else
                memoryTemp2 -> logicLevel = memoryTemp1 -> logicLevel + 1;
        }
        else if ( ( memoryTemp1 -> firstWord -> wordCategory == 57 || ( memoryTemp1 -> firstWord -> wordCategory >= 71 && memoryTemp1 -> firstWord -> wordCategory <= 89 ) ) && memoryTemp1 -> nextLine )
        {
            for ( memoryTemp2 = memoryTemp1 ; memoryTemp2 -> nextLine ; memoryTemp2 = memoryTemp2 -> nextLine )
                if ( memoryTemp2 -> nextLine -> firstWord -> wordCategory != 90 && memoryTemp2 -> nextLine -> firstWord -> wordCategory != 91 )
                    break;
                else
                    memoryTemp2 -> nextLine -> logicLevel = memoryTemp1 -> logicLevel;
            if ( memoryTemp2 -> nextLine && memoryTemp2 -> nextLine -> nextLine && memoryTemp2 -> nextLine -> firstWord -> wordCategory == 2 )
            {
                memoryTemp1 = memoryTemp2;
                memoryTemp1 -> nextLine -> logicLevel = memoryTemp1 -> logicLevel + 1;
                for ( memoryTemp2 = memoryTemp1 -> nextLine -> nextLine , bracesTemp = 1 ; memoryTemp2 && bracesTemp ; memoryTemp2 = memoryTemp2 -> nextLine )
                {
                    memoryTemp2 -> logicLevel = memoryTemp2 -> previousLine -> logicLevel;
                    if ( memoryTemp2 -> firstWord -> wordCategory == 2 )
                        bracesTemp ++;
                    else if ( memoryTemp2 -> firstWord -> wordCategory == 3 )
                        bracesTemp --;
                }
            }
            else if ( memoryTemp2 -> firstWord -> wordCategory == 2 )
                memoryTemp1 -> nextLine -> logicLevel = memoryTemp1 -> logicLevel + 1;
        }
        else if ( ( memoryTemp1 -> firstWord -> wordCategory == 55 || memoryTemp1 -> firstWord -> wordCategory == 56 ) && memoryTemp1 -> nextLine )
            for ( memoryTemp2 = memoryTemp1 -> nextLine , bracesTemp = 0 ; memoryTemp2 ; memoryTemp2 = memoryTemp2 -> nextLine )
            {
                if ( memoryTemp2 -> firstWord -> wordCategory == 55 || memoryTemp2 -> firstWord -> wordCategory == 56 )
                    break;
                else if ( memoryTemp2 -> firstWord -> wordCategory == 3 && bracesTemp == 0 )
                    break;
                else if ( memoryTemp2 -> firstWord -> wordCategory == 3 )
                    bracesTemp --;
                else if ( memoryTemp2 -> firstWord -> wordCategory == 2 )
                    bracesTemp ++;
                memoryTemp2 -> logicLevel = memoryTemp1 -> logicLevel + 1;
            }
    }
}

QString MemoryCenter :: relayoutStyle ( int styleNumber )
{
    struct LineHeadPointer *memoryTemp;
    struct LexicalAnalysis *wordTemp;
    QString relayoutResult;
    int logicLevelTemp;
    relayoutResult = "";
    for ( memoryTemp = memoryHead -> nextLine ; memoryTemp ; memoryTemp = memoryTemp -> nextLine )
    {
        if ( memoryTemp -> previousLine != memoryHead )
            if ( ( styleNumber & 2 ) == 2 && memoryTemp -> logicLevel == 0 && ( ( memoryTemp -> previousLine -> firstWord -> wordCategory == 3 && memoryTemp -> firstWord ->wordCategory != 12 ) || ( memoryTemp -> previousLine -> firstWord -> wordCategory == 12 && memoryTemp -> previousLine -> previousLine -> firstWord -> wordCategory == 3 ) ) )
                relayoutResult += "\n\n";
            else if ( ( styleNumber & 1 ) == 1 && memoryTemp -> logicLevel == 0 && memoryTemp -> nextLine && memoryTemp -> previousLine -> firstWord -> wordCategory == 12 && memoryTemp -> nextLine -> firstWord -> wordCategory == 2 )
                relayoutResult += "\n\n";
            else if ( ( styleNumber & 1 ) == 1 && memoryTemp -> logicLevel != 0 && memoryTemp -> firstWord -> wordCategory != 2 && ( ( memoryTemp -> previousLine -> firstWord -> wordCategory >= 71 && memoryTemp -> previousLine -> firstWord -> wordCategory <= 89 ) || ( memoryTemp -> previousLine -> firstWord -> wordCategory >= 58 && memoryTemp -> previousLine -> firstWord -> wordCategory <= 60 ) ) && ( memoryTemp -> firstWord -> wordCategory < 58 || ( memoryTemp -> firstWord -> wordCategory > 60 && memoryTemp -> firstWord -> wordCategory < 71 ) || memoryTemp -> firstWord -> wordCategory > 89 ) )
                relayoutResult += "\n\n";
            else if ( memoryTemp -> logicLevel == 0 && memoryTemp -> previousLine -> firstWord -> wordCategory == 10 && memoryTemp -> firstWord -> wordCategory != 10)
                relayoutResult += "\n\n";
            else if ( memoryTemp -> firstWord -> wordCategory == 12 )
                if ( ( styleNumber & 8 ) == 8 )
                    relayoutResult += " ";
                else;
            else if ( ( styleNumber & 4 ) != 4 && memoryTemp -> firstWord -> wordCategory == 2 && memoryTemp -> previousLine && ( memoryTemp -> previousLine -> firstWord -> wordCategory == 90 || memoryTemp -> previousLine -> firstWord -> wordCategory == 91 ) )
                relayoutResult += "\n";
            else if ( ( styleNumber & 4 ) != 4 && memoryTemp -> firstWord -> wordCategory == 2 );
            else
                relayoutResult += "\n";
        else;
        if ( memoryTemp -> firstWord -> wordCategory == 2 || memoryTemp -> firstWord -> wordCategory == 3 )
            logicLevelTemp = 1;
        else
            logicLevelTemp = 0;
        if ( ( ( styleNumber & 4 ) != 4 && memoryTemp -> firstWord -> wordCategory == 2 ) || memoryTemp -> firstWord -> wordCategory == 12 );
        else
        {
            for ( ; logicLevelTemp < memoryTemp -> logicLevel ; logicLevelTemp ++ )
                relayoutResult += "    ";
            if ( memoryTemp -> firstWord -> wordCategory == 91 )
                memoryTemp -> firstWord -> wordContent = relayoutRemark ( memoryTemp -> logicLevel , memoryTemp -> firstWord -> wordContent );
        }
        relayoutResult += memoryTemp -> firstWord -> wordContent;
        for ( wordTemp = memoryTemp -> firstWord -> nextWord ; wordTemp ; wordTemp = wordTemp -> nextWord )
            if ( wordTemp -> wordCategory == 12 || wordTemp -> wordCategory == 27 )
                if ( ( styleNumber & 8 ) == 8 )
                    relayoutResult += " " + wordTemp -> wordContent;
                else
                    relayoutResult += wordTemp -> wordContent;
            else if ( wordTemp -> previousWord && wordTemp -> previousWord -> wordCategory == 10 )
                relayoutResult += wordTemp -> wordContent;
            else if ( wordTemp -> previousWord -> wordCategory == 4 || wordTemp -> previousWord -> wordCategory == 6 || wordTemp -> wordCategory == 5 || wordTemp -> wordCategory == 7 )
                if ( ( styleNumber & 16 ) == 16 )
                    relayoutResult += " " + wordTemp -> wordContent;
                else
                    relayoutResult += wordTemp -> wordContent;
            else if ( wordTemp -> wordCategory == 6 )
                if ( ( styleNumber & 32 ) == 32 )
                    relayoutResult += " " + wordTemp -> wordContent;
                else
                    relayoutResult += wordTemp -> wordContent;
            else
                relayoutResult += " " + wordTemp -> wordContent;
    }
    return relayoutResult;
}

QString MemoryCenter :: relayoutRemark ( int logicLevel , QString remarkString )
{
    int remarkLength , remarkTemp , logicLevelTemp;
    QString remarkResult;
    remarkLength = remarkString . length ( );
    remarkResult = "";
    for ( remarkTemp = 0 ; remarkTemp < remarkLength - 1 ; remarkTemp ++ )
    {
        remarkResult += remarkString [ remarkTemp ];
        if ( remarkString [ remarkTemp ] == '\n' )
        {
            for ( ; remarkTemp < remarkLength - 1 ; remarkTemp ++ )
                if ( remarkString [ remarkTemp + 1 ] != ' ' )
                    break;
            if ( remarkTemp == remarkLength - 1 && remarkString [ remarkTemp ] == ' ' )
                return remarkResult;
            else if ( remarkTemp == remarkLength - 1 )
                remarkTemp --;
            for ( logicLevelTemp = 0 ; logicLevelTemp < logicLevel ; logicLevelTemp ++ )
                remarkResult += "    ";
        }
    }
    remarkResult += remarkString [ remarkTemp ];
    return remarkResult;
}

struct FunctionList * MemoryCenter :: analysisFunction ( LineHeadPointer *memoryTemp )
{
    struct LexicalAnalysis *wordTemp1 , *wordTemp2 , *wordTemp3;
    struct FunctionList *functionBuild;
    struct ParameterList *parameterBuild;
    functionBuild = new ( struct FunctionList );
    for ( wordTemp1 = memoryTemp -> firstWord ; wordTemp1 ; wordTemp1 = wordTemp1 -> nextWord )
        if ( wordTemp1 -> wordCategory == 6 )
            break;
    wordTemp2 = wordTemp1 -> previousWord;
    functionBuild -> functionName = wordTemp2 -> wordContent;
    functionBuild -> functionNameLength = wordTemp2 -> wordLength;
    functionBuild -> functionCategory = memoryTemp -> firstWord -> wordContent;
    for ( wordTemp1 = memoryTemp -> firstWord -> nextWord ; wordTemp1 != wordTemp2 ; wordTemp1 = wordTemp1 -> nextWord )
        functionBuild -> functionCategory += " " + wordTemp1 -> wordContent;
    functionBuild -> functionCategoryLength = functionBuild -> functionCategory . length ( );
    functionBuild -> parameterNumber = 0;
    functionBuild -> firstParameter = NULL;
    functionBuild -> functionPosition = memoryTemp -> linePosition;
    functionBuild -> previousFunction = NULL;
    functionBuild -> nextFunction = NULL;
    for ( wordTemp2 = wordTemp2 -> nextWord ; wordTemp2 ; wordTemp2 = wordTemp2 -> nextWord )
        if ( wordTemp2 -> wordCategory == 7 )
            break;
    for ( wordTemp1 = wordTemp2 -> previousWord ; wordTemp1 ; wordTemp1 = wordTemp1 -> previousWord )
        if ( wordTemp1 -> wordCategory == 6 )
            break;
    if ( wordTemp1 -> nextWord != wordTemp2 )
        for ( ; ; )
        {
            functionBuild -> parameterNumber ++;
            for ( wordTemp1 = wordTemp2 -> previousWord ; wordTemp1 ; wordTemp1 = wordTemp1 -> previousWord )
                if ( wordTemp1 -> wordCategory == 6 || wordTemp1 -> wordCategory == 27 )
                    break;
            parameterBuild = new ( ParameterList );
            parameterBuild -> whichFunction = functionBuild;
            parameterBuild -> previousParameter = NULL;
            parameterBuild -> nextParameter = functionBuild -> firstParameter;
            if ( parameterBuild -> nextParameter )
                parameterBuild -> nextParameter -> previousParameter = parameterBuild;
            functionBuild -> firstParameter = parameterBuild;
            if ( wordTemp2 -> previousWord -> wordCategory < 50 )
                for ( ; wordTemp2 -> previousWord != wordTemp1 ; wordTemp2 = wordTemp2 -> previousWord )
                    if ( wordTemp2 -> previousWord -> wordCategory >= 50 )
                        break;
            if ( wordTemp2 -> wordCategory != 27 && wordTemp2 -> wordCategory != 7 )
            {
                wordTemp3 = wordTemp2;
                parameterBuild -> parameterTail = wordTemp3 -> wordContent;
                for ( wordTemp3 = wordTemp3 -> nextWord ; wordTemp3 -> wordCategory != 27 && wordTemp3 -> wordCategory != 7 ; wordTemp3 = wordTemp3 -> nextWord )
                    parameterBuild -> parameterTail += " " + wordTemp3 -> wordContent;
                parameterBuild -> parameterTailLength = parameterBuild -> parameterTail . length ( );
            }
            else
            {
                parameterBuild -> parameterTail = "";
                parameterBuild -> parameterTailLength = 0;
            }
            if ( wordTemp2 -> previousWord -> wordCategory != 92 || ( wordTemp1 -> nextWord -> wordCategory < 58 || ( wordTemp1 -> nextWord -> wordCategory > 60 && wordTemp1 -> nextWord -> wordCategory < 71 ) || wordTemp1 -> nextWord -> wordCategory > 89 ) )
            {
                if ( wordTemp2 -> previousWord -> wordCategory == 92 )
                {
                    wordTemp2 = wordTemp2 -> previousWord;
                    parameterBuild -> parameterName = wordTemp2 -> wordContent;
                    parameterBuild -> parameterNameLength = wordTemp2 -> wordLength;
                    wordTemp1 = wordTemp1 -> nextWord;
                    if ( wordTemp1 != wordTemp2 )
                    {
                        parameterBuild -> parameterCategory = wordTemp1 -> wordContent;
                        for ( wordTemp1 = wordTemp1 -> nextWord ; wordTemp1 != wordTemp2 ; wordTemp1 = wordTemp1 -> nextWord )
                            parameterBuild -> parameterCategory += " " + wordTemp1 -> wordContent;
                    }
                    else
                        parameterBuild -> parameterCategory = "";
                    parameterBuild -> parameterCategoryLength = parameterBuild -> parameterCategory . length ( );
                }
                else
                {
                    parameterBuild -> parameterName = "";
                    parameterBuild -> parameterNameLength = 0;
                    wordTemp1 = wordTemp1 -> nextWord;
                    if ( wordTemp1 != wordTemp2 )
                    {
                        parameterBuild -> parameterCategory = wordTemp1 -> wordContent;
                        for ( wordTemp1 = wordTemp1 -> nextWord ; wordTemp1 != wordTemp2 ; wordTemp1 = wordTemp1 -> nextWord )
                            parameterBuild -> parameterCategory += " " + wordTemp1 -> wordContent;
                    }
                    else
                        parameterBuild -> parameterCategory = "";
                    parameterBuild -> parameterCategoryLength = parameterBuild -> parameterCategory . length ( );
                }
            }
            else
            {
                wordTemp2 = wordTemp2 -> previousWord;
                parameterBuild -> parameterName = wordTemp2 -> wordContent;
                parameterBuild -> parameterNameLength = wordTemp2 -> wordLength;
                wordTemp1 = wordTemp1 -> nextWord;
                parameterBuild -> parameterCategory = wordTemp1 -> wordContent;
                for ( wordTemp1 = wordTemp1 -> nextWord ; wordTemp1 != wordTemp2 ; wordTemp1 = wordTemp1 -> nextWord )
                    parameterBuild -> parameterCategory += " " + wordTemp1 -> wordContent;
                parameterBuild -> parameterCategoryLength = parameterBuild -> parameterCategory . length ( );
            }
            for ( wordTemp2 = wordTemp2 -> previousWord ; wordTemp2 ; wordTemp2 = wordTemp2 -> previousWord )
                if ( wordTemp2 -> wordCategory == 6 || wordTemp2 -> wordCategory == 27 )
                    break;
            if ( wordTemp2 -> wordCategory == 6 )
                break;
        }
    return functionBuild;
}
