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
#include <QDebug>

MemoryCenter :: MemoryCenter ( )
{
    memoryHead = new ( struct LineHeadPointer );
    memoryHead -> logicLevel = 0;
    memoryHead -> linePosition = 0;
    memoryHead -> previousLine = NULL;
    memoryHead -> nextLine = NULL;
    memoryHead -> firstWord = NULL;
    functionHead = new ( struct FunctionList );
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
    compleHead = new ( struct CompleList );
    compleHead -> compleName = "";
    compleHead -> compleNameLength = 0;
    compleHead -> previousComple = NULL;
    compleHead -> nextComple = NULL;
    lineNumberHead = new ( struct LineNumberList );
    lineNumberHead -> lineNumber = 0;
    lineNumberHead -> linePosition = 0;
    lineNumberHead -> nextLineNumber = NULL;
    buildKeyWordList ( );
}

MemoryCenter :: ~MemoryCenter ( )
{
    resetLine ( );
    resetFunctionList ( );
    resetCompleList ( );
    resetLineNumberList ( );
    delete ( memoryHead );
    delete ( functionHead );
    delete ( compleHead );
    delete ( lineNumberHead );
}

bool MemoryCenter :: analysisMain ( QString & codeString )
{
    int codePosition , wordKind;
    int codeLength , wordLength , codePositionTemp;
    QString thisWord;
    struct LineHeadPointer *memoryTemp;
    codePosition = 0;
    wordKind = 0;
    codeLength = 0;
    resetLine ( );
    if ( codeString . isNull ( ) )
        return false;
    else if ( codeString . isEmpty ( ) )
        return true;
    codeLength = codeString . length ( );
    for ( memoryTemp = memoryHead ; codePosition < codeLength ; )
    {
        thisWord = cutWord ( codeString , codePosition , wordLength , wordKind );
        if ( wordKind == 0 )
        {
            codePosition += wordLength;
            continue;
        }
        else if ( wordKind == 2 && ( thisWord == "{" || thisWord == "}" ) )
        {
            codePositionTemp = codePosition;
            codePosition += wordLength;
            memoryTemp -> nextLine = analysisBraces ( thisWord );
            memoryTemp -> nextLine -> previousLine = memoryTemp;
            memoryTemp = memoryTemp -> nextLine;
            memoryTemp -> linePosition = codePositionTemp;
            memoryTemp -> firstWord -> wordPosition = codePositionTemp;
        }
        else if ( wordKind == 4 )
        {
            codePositionTemp = codePosition;
            codePosition += wordLength;
            memoryTemp -> nextLine = analysisMacro ( codeString , thisWord , codePosition );
            memoryTemp -> nextLine -> previousLine = memoryTemp;
            memoryTemp = memoryTemp -> nextLine;
            memoryTemp -> linePosition = codePositionTemp;
            memoryTemp -> firstWord -> wordPosition = codePositionTemp;
        }
        else if ( wordKind == 6 )
        {
            codePositionTemp = codePosition;
            codePosition += wordLength;
            memoryTemp -> nextLine = analysisSemicolon ( codeString , thisWord , codePosition );
            memoryTemp -> nextLine -> previousLine = memoryTemp;
            memoryTemp = memoryTemp -> nextLine;
            memoryTemp -> linePosition = codePositionTemp;
            memoryTemp -> firstWord -> wordPosition = codePositionTemp;
        }
        else if ( wordKind == 9 )
        {
            codePositionTemp = codePosition;
            codePosition += wordLength;
            memoryTemp -> nextLine = analysisRemark ( codeString , thisWord , codePosition );
            memoryTemp -> nextLine -> previousLine = memoryTemp;
            memoryTemp = memoryTemp -> nextLine;
            memoryTemp -> linePosition = codePositionTemp;
            memoryTemp -> firstWord -> wordPosition = codePositionTemp;
        }
        else if ( wordKind == 1 || wordKind == 2 || wordKind == 3 || wordKind == 7 )
        {
            codePositionTemp = codePosition;
            codePosition += wordLength;
            memoryTemp -> nextLine = analysisOther ( codeString , thisWord , wordKind , codePosition );
            memoryTemp -> nextLine -> previousLine = memoryTemp;
            memoryTemp = memoryTemp -> nextLine;
            memoryTemp -> linePosition = codePositionTemp;
            memoryTemp -> firstWord -> wordPosition = codePositionTemp;
        }
        else if ( wordKind == 8 )
        {
            codePositionTemp = codePosition;
            codePosition += wordLength;
            memoryTemp -> nextLine = analysisKey ( codeString , thisWord , codePosition );
            memoryTemp -> nextLine -> previousLine = memoryTemp;
            memoryTemp = memoryTemp -> nextLine;
            memoryTemp -> linePosition = codePositionTemp;
            memoryTemp -> firstWord -> wordPosition = codePositionTemp;
        }
        else
            return false;
    }
    return true;
}

struct LineHeadPointer * MemoryCenter :: getLineHead ( )
{
    return memoryHead;
}

QString MemoryCenter :: relayout ( int styleNumber )
{
    QString relayoutResult;
    resetLogicLevel ( );
    analysisLogicLevel ( );
    relayoutResult = relayoutStyle ( styleNumber );
    return relayoutResult;
}

bool MemoryCenter :: makeFunctionList ( QString & codeString )
{
    struct LineHeadPointer *memoryTemp;
    struct FunctionList *functionTemp , *functionBuild;
    struct LexicalAnalysis *wordTemp;
    struct LineNumberList *lineNumberTemp;
    struct ParameterList *parameterTemp;
    resetFunctionList ( );
    memoryTemp = memoryHead;
    if ( memoryTemp == NULL || memoryTemp -> nextLine == NULL )
        return false;
    for ( memoryTemp = memoryTemp -> nextLine ; memoryTemp -> nextLine ; memoryTemp = memoryTemp -> nextLine )
        if ( ( ( memoryTemp -> firstWord -> wordCategory >= 58 && memoryTemp -> firstWord -> wordCategory <= 60 ) || ( memoryTemp -> firstWord -> wordCategory >= 71 && memoryTemp -> firstWord -> wordCategory <= 89 ) || ( memoryTemp -> firstWord -> wordCategory == 92 && memoryTemp -> firstWord -> nextWord && memoryTemp -> firstWord -> nextWord -> wordCategory != 6 ) ) && memoryTemp -> nextLine -> firstWord -> wordCategory == 2 )
        {
            for ( wordTemp = memoryTemp -> firstWord -> nextWord ; wordTemp ; wordTemp = wordTemp -> nextWord )
                if ( wordTemp -> wordCategory == 6 )
                    break;
            if ( wordTemp == NULL )
                continue;
            functionBuild = analysisFunction ( memoryTemp );
            for ( functionTemp = functionHead ; functionTemp -> nextFunction ; functionTemp = functionTemp -> nextFunction )
                if ( functionTemp -> nextFunction -> functionName > functionBuild -> functionName )
                    break;
            functionBuild -> nextFunction = functionTemp -> nextFunction;
            functionBuild -> previousFunction = functionTemp;
            if ( functionTemp -> nextFunction )
                functionTemp -> nextFunction -> previousFunction = functionBuild;
            functionTemp -> nextFunction = functionBuild;
        }
    buildLineNumberList ( codeString );
    for ( functionTemp = functionHead -> nextFunction ; functionTemp ; functionTemp = functionTemp -> nextFunction )
    {
        for( lineNumberTemp = lineNumberHead ; lineNumberTemp -> nextLineNumber ; lineNumberTemp = lineNumberTemp -> nextLineNumber )
            if ( lineNumberTemp -> nextLineNumber -> linePosition == functionTemp -> functionPosition )
            {
                functionTemp -> functionLine = lineNumberTemp -> nextLineNumber -> lineNumber;
                break;
            }
            else if ( lineNumberTemp -> nextLineNumber -> linePosition > functionTemp -> functionPosition )
            {
                functionTemp -> functionLine = lineNumberTemp -> lineNumber;
                break;
            }
        functionTemp -> functionOutput = functionTemp -> functionCategory + " " + functionTemp -> functionName;
        if ( functionTemp -> parameterNumber == 0 )
            functionTemp -> functionOutput += " (";
        for ( parameterTemp = functionTemp -> firstParameter ; parameterTemp ; parameterTemp = parameterTemp -> nextParameter )
        {
            if ( parameterTemp == functionTemp -> firstParameter )
                functionTemp -> functionOutput += " (";
            else
                functionTemp -> functionOutput += " ,";
            if ( parameterTemp -> parameterCategory != "" )
                functionTemp -> functionOutput += " " + parameterTemp -> parameterCategory;
            if ( parameterTemp -> parameterName != "" )
                functionTemp -> functionOutput += " " + parameterTemp -> parameterName;
            if ( parameterTemp -> parameterTail != "" )
                functionTemp -> functionOutput += " " + parameterTemp -> parameterTail;
        }
        functionTemp -> functionOutput += " )";
    }
    return true;
}

struct FunctionList * MemoryCenter :: getFunctionList ( )
{
    return functionHead;
}

bool MemoryCenter :: makeCompleList ( )
{
    struct LineHeadPointer *memoryTemp;
    struct CompleList *compleTemp , *compleBuild;
    struct LexicalAnalysis *wordTemp;
    resetCompleList ( );
    memoryTemp = memoryHead;
    if ( memoryTemp == NULL || memoryTemp -> nextLine == NULL )
        return false;
    for ( memoryTemp = memoryTemp -> nextLine ; memoryTemp ; memoryTemp = memoryTemp -> nextLine )
        for ( wordTemp = memoryTemp -> firstWord ; wordTemp ; wordTemp = wordTemp -> nextWord )
            if ( wordTemp -> wordCategory == 92 )
            {
                for ( compleTemp = compleHead ; compleTemp -> nextComple ; compleTemp = compleTemp -> nextComple )
                    if ( compleTemp -> nextComple -> compleName >= wordTemp -> wordContent )
                        break;
                if ( compleTemp -> nextComple && compleTemp -> nextComple -> compleName == wordTemp -> wordContent )
                    continue;
                compleBuild = new ( struct CompleList );
                compleBuild -> compleName = wordTemp -> wordContent;
                compleBuild -> compleNameLength = wordTemp -> wordLength;
                compleBuild -> nextComple = compleTemp -> nextComple;
                compleBuild -> previousComple = compleTemp;
                if ( compleTemp -> nextComple )
                    compleTemp -> nextComple -> previousComple = compleBuild;
                compleTemp -> nextComple = compleBuild;
            }
    return true;
}

struct CompleList * MemoryCenter :: findCompleList ( QString & wordString , int & matchNumber , int & matchLength )
{
    struct CompleList *compleTemp , *compleResult;
    int findPositionTemp;
    matchNumber = 0;
    compleResult = NULL;
    matchLength = wordString . length ( );
    for ( compleTemp = compleHead -> nextComple ; compleTemp ; compleTemp = compleTemp -> nextComple )
    {
        findPositionTemp = compleTemp -> compleName . indexOf ( wordString , 0 , Qt::CaseSensitive );
        if ( findPositionTemp == 0 && compleTemp -> compleName != wordString )
        {
            if ( compleResult == NULL )
                compleResult = compleTemp;
            matchNumber ++;
        }
        else if ( matchNumber != 0 )
            break;
    }
    return compleResult;
}

int MemoryCenter :: autoIndent ( QString & codeString , int codePosition )
{
    int letterPositionTemp , codePositionTemp1 , codePositionTemp2 , spaceNumber , bracesTemp;
    spaceNumber = 0;
    for ( codePositionTemp2 = codePosition ; ; )
    {
        for ( codePositionTemp2 -- ; codePositionTemp2 >= 0 ; codePositionTemp2 -- )
            if ( codeString [ codePositionTemp2 ] == '\n' )
                break;
        if ( codePositionTemp2 <= 0 )
            return 0;
        for ( codePositionTemp1 = codePositionTemp2 - 1 ; codePositionTemp1 > 0 ; codePositionTemp1 -- )
            if ( codeString [ codePositionTemp1 - 1 ] == '\n' )
                break;
        for ( letterPositionTemp = codePositionTemp1 ; letterPositionTemp <= codePositionTemp2 ; letterPositionTemp ++ )
            if ( codeString [ letterPositionTemp ] != ' ' && codeString [ letterPositionTemp ] != '\r' && codeString [ letterPositionTemp ] != '\n' )
                break;
        if ( letterPositionTemp <= codePositionTemp2 )
            break;
    }
    for ( letterPositionTemp = codePositionTemp1 ; letterPositionTemp <= codePositionTemp2 ; letterPositionTemp ++ )
        if ( codeString [ letterPositionTemp ] == ' ' )
            spaceNumber ++;
        else
            break;
    if ( codeString [ letterPositionTemp ] == '#' )
        return spaceNumber;
    for ( letterPositionTemp = codePositionTemp2 ; letterPositionTemp >= codePositionTemp1 ; letterPositionTemp -- )
        if ( codeString [ letterPositionTemp ] != '\r' && codeString [ letterPositionTemp ] != '\n' && codeString [ letterPositionTemp ] != ' ' )
            break;
    if ( codeString [ letterPositionTemp ] == '{' || codeString [ letterPositionTemp ] == ')' || codeString [ letterPositionTemp ] == ':' || ( letterPositionTemp - 3 >= 0 && codeString [ letterPositionTemp - 3 ] == 'e' && codeString [ letterPositionTemp - 2 ] == 'l' && codeString [ letterPositionTemp - 1 ] == 's' && codeString [ letterPositionTemp ] == 'e' && ( letterPositionTemp - 4 < 0 || codeString [ letterPositionTemp - 4 ] == ' ' || codeString [ letterPositionTemp - 4 ] == '\n' || codeString [ letterPositionTemp - 4 ] == '\r' ) ) )
        return spaceNumber + 4;
    else if ( codeString [ letterPositionTemp ] != ';' && codeString [ letterPositionTemp ] != '}' )
        return spaceNumber;
    for ( bracesTemp = 1 , codePositionTemp2 ++ ; ; )
    {
        for ( codePositionTemp2 -- ; codePositionTemp2 >= 0 ; codePositionTemp2 -- )
            if ( codeString [ codePositionTemp2 ] == '\n' )
                break;
        if ( codePositionTemp2 <= 0 )
            return 0;
        for ( codePositionTemp1 = codePositionTemp2 - 1 ; codePositionTemp1 > 0 ; codePositionTemp1 -- )
            if ( codeString [ codePositionTemp1 - 1 ] == '\n' )
                break;
        for ( spaceNumber = 0 , letterPositionTemp = codePositionTemp1 ; letterPositionTemp <= codePositionTemp2 ; letterPositionTemp ++ )
            if ( codeString [ letterPositionTemp ] == ' ' )
                spaceNumber ++;
            else
                break;
        for ( letterPositionTemp = codePositionTemp2 ; letterPositionTemp >= codePositionTemp1 ; letterPositionTemp -- )
            if ( codeString [ letterPositionTemp ] != '\r' && codeString [ letterPositionTemp ] != '\n' && codeString [ letterPositionTemp ] != ' ' )
                break;
        if ( letterPositionTemp < 0 )
            return 0;
        if ( codeString [ letterPositionTemp ] == '{' )
            bracesTemp --;
        else if ( codeString [ letterPositionTemp ] == '}' )
            bracesTemp ++;
        else if ( codeString [ letterPositionTemp ] == ';' && letterPositionTemp >= 1 && codeString [ letterPositionTemp - 1 ] == '}' )
            bracesTemp ++;
        else if ( codeString [ letterPositionTemp ] == ':' );
        else
            continue;
        if ( codeString [ letterPositionTemp ] == ':' && bracesTemp == 1 )
            return spaceNumber + 4;
        else if ( bracesTemp == 0 )
            return spaceNumber + 4;
    }
    return 0;
}

int MemoryCenter :: changeIndent ( QString & codeString , QString & lineString , int codePosition , int changeMode )
{
    int letterPositionTemp1 , letterPositionTemp2 , codePositionTemp1 , codePositionTemp2 , spaceNumber , bracesTemp , wordLength , wordKind , spaceNumberTemp , lineLength;
    QString thisWord;
    spaceNumber = 0;
    lineLength = lineString . length ( );
    for ( spaceNumberTemp = 0 ; spaceNumberTemp < lineLength ; spaceNumberTemp ++ )
        if ( lineString [ spaceNumberTemp ] != ' ' && lineString [ spaceNumberTemp ] != '\r' && lineString [ spaceNumberTemp ] != '\n' )
            break;
    for ( codePositionTemp2 = codePosition ; ; )
    {
        for ( codePositionTemp2 -- ; codePositionTemp2 >= 0 ; codePositionTemp2 -- )
            if ( codeString [ codePositionTemp2 ] == '\n' )
                break;
        if ( codePositionTemp2 <= 0 )
            return 0 - spaceNumberTemp;
        for ( codePositionTemp1 = codePositionTemp2 - 1 ; codePositionTemp1 > 0 ; codePositionTemp1 -- )
            if ( codeString [ codePositionTemp1 - 1 ] == '\n' )
                break;
        for ( letterPositionTemp1 = codePositionTemp1 ; letterPositionTemp1 <= codePositionTemp2 ; letterPositionTemp1 ++ )
            if ( codeString [ letterPositionTemp1 ] != ' ' && codeString [ letterPositionTemp1 ] != '\r' && codeString [ letterPositionTemp1 ] != '\n' )
                break;
        if ( letterPositionTemp1 <= codePositionTemp2 )
            break;
    }
    for ( letterPositionTemp1 = codePositionTemp1 ; letterPositionTemp1 <= codePositionTemp2 ; letterPositionTemp1 ++ )
        if ( codeString [ letterPositionTemp1 ] == ' ' )
            spaceNumber ++;
        else
            break;
    if ( codeString [ letterPositionTemp1 ] != ';' && codeString [ letterPositionTemp1 ] != '}' && codeString [ letterPositionTemp1 ] != '{' && changeMode == 0 )
        return spaceNumber - spaceNumberTemp;
    for ( bracesTemp = 1 , codePositionTemp2 ++ ; ; )
    {
        for ( codePositionTemp2 -- ; codePositionTemp2 >= 0 ; codePositionTemp2 -- )
            if ( codeString [ codePositionTemp2 ] == '\n' )
                break;
        if ( codePositionTemp2 <= 0 )
            return 0 - spaceNumberTemp;
        for ( codePositionTemp1 = codePositionTemp2 - 1 ; codePositionTemp1 > 0 ; codePositionTemp1 -- )
            if ( codeString [ codePositionTemp1 - 1 ] == '\n' )
                break;
        for ( spaceNumber = 0 , letterPositionTemp1 = codePositionTemp1 ; letterPositionTemp1 <= codePositionTemp2 ; letterPositionTemp1 ++ )
            if ( codeString [ letterPositionTemp1 ] == ' ' )
                spaceNumber ++;
            else
                break;
        for ( letterPositionTemp2 = codePositionTemp2 ; letterPositionTemp2 >= codePositionTemp1 ; letterPositionTemp2 -- )
            if ( codeString [ letterPositionTemp2 ] != '\r' && codeString [ letterPositionTemp2 ] != '\n' && codeString [ letterPositionTemp2 ] != ' ' )
                break;
        if ( codeString [ letterPositionTemp2 ] == '{' )
            bracesTemp --;
        if ( letterPositionTemp1 < letterPositionTemp2 && codeString [ letterPositionTemp1 ] == '{' )
            bracesTemp --;
        if ( codeString [ letterPositionTemp2 ] == '}' )
            bracesTemp ++;
        if ( letterPositionTemp1 < letterPositionTemp2 && codeString [ letterPositionTemp1 ] == '}' )
            bracesTemp ++;
        if ( ( codeString [ letterPositionTemp2 ] == ')' || ( letterPositionTemp2 >= 1 && codeString [ letterPositionTemp2 - 1 ] == ')' && codeString [ letterPositionTemp2 ] == ';' ) ) && bracesTemp == 1 && changeMode == 2 )
        {
            thisWord = cutWord ( codeString , letterPositionTemp1 , wordLength , wordKind );
            if ( thisWord == "if" )
                return spaceNumber - spaceNumberTemp;
        }
        else if ( bracesTemp <= 0 && changeMode == 1 )
            return spaceNumber - spaceNumberTemp;
        else if ( bracesTemp <= 0 )
            return spaceNumber + 4 - spaceNumberTemp;
    }
    return 0;
}

QString MemoryCenter :: makeFunctionRemark ( )
{
    FunctionList *functionTemp;
    ParameterList *parameterTemp;
    QString functionRemark;
    functionRemark = "";
    if ( functionHead -> nextFunction )
        for ( functionTemp = functionHead -> nextFunction ; functionTemp ; functionTemp = functionTemp -> nextFunction )
            if ( functionTemp -> functionLine == 1 )
                break;
            else;
    else
        functionTemp = NULL;
    if ( functionTemp != NULL )
    {
        functionRemark += "/**\n * @brief\n";
        if ( functionTemp -> parameterNumber != 0 )
        {
            functionRemark += " *\n";
            for ( parameterTemp = functionTemp -> firstParameter ; parameterTemp ; parameterTemp = parameterTemp -> nextParameter )
                functionRemark += " * @param   " + parameterTemp -> parameterName + "\n";
        }
        if ( functionTemp -> functionCategory != "void" )
            functionRemark += " *\n * @return\n";
        functionRemark += " */\n";
    }
    return functionRemark;
}
