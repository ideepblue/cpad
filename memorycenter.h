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
#ifndef CLASSDEFINE_H
#define CLASSDEFINE_H
#include "memorystruct.h"
#include <QString>
class MemoryCenter
{
    private:
        struct LineHeadPointer *memoryHead;//存放语法行首指针的头指针
        struct FunctionList *functionHead;//存放函数列表首指针的头指针
        struct CompleList *compleHead;//存放补全名称表首指真的头指针
        struct LineNumberList *lineNumberHead;//存放行号表首指针的头指针
        QString keyWordList [ 100 ];//存放运算符和关键词的表
        void resetLine ( );//置新语法行函数
        void resetFunctionList ( );//置新函数列表函数
        void resetCompleList ( );//置新变量表函数
        void resetLineNumberList ( );//置新行号表函数
        void buildKeyWordList ( );//建立关键词表函数
        void buildLineNumberList ( QString codeString );//建立行号表函数
        int judgeWordCategory ( const QString & thisWord , const int startFindPosition , const int endFindPosition );//判断词类型函数
        QString cutWord ( const QString & codeString , const int & codePosition , int & wordLength , int & wordKind );//切割单词函数
        struct LineHeadPointer *analysisBraces ( const QString & firstWord );//大括号词法分析函数
        struct LineHeadPointer *analysisMacro ( const QString codeString , const QString & firstWord , int & codePosition );//宏定义词法分析函数
        struct LineHeadPointer *analysisSemicolon ( const QString codeString , const QString & firstWord , int & codePosition );//分号词法分析函数
        struct LineHeadPointer *analysisRemark ( const QString codeString , const QString & firstWord , int & codePosition );//注释词法分析函数
        struct LineHeadPointer *analysisOther ( const QString codeString , const QString & firstWord , const int & firstWordKind , int & codePosition );//其他非关键词词法分析函数
        struct LineHeadPointer *analysisKey ( const QString codeString , const QString & firstWord , int & codePosition );//关键词词法分析函数
        void resetLogicLevel ( );//置新语法行逻辑等级函数
        void analysisLogicLevel ( );//语法行逻辑等级分析函数
        QString relayoutStyle ( int styleNumber );//根据要求格式重拍版函数
        QString relayoutRemark ( int logicLevel , QString remarkString );//重排版多行注释函数
        struct FunctionList *analysisFunction ( LineHeadPointer *memoryTemp );//生成函数结构函数
    public:
        MemoryCenter ( );//构造函数
        ~MemoryCenter ( );//析构函数
        bool analysisMain ( QString & codeString );//词法分析主体函数
        struct LineHeadPointer * getLineHead ( );//提取语法行首指针函数
        bool makeFunctionList ( QString & codeString );//生成函数列表函数
        struct FunctionList * getFunctionList ( );//提取函数列表函数
        bool makeCompleList ( );//生成补全表函数
        struct CompleList * findCompleList ( QString & wordString , int & matchNumber , int & matcheLength );//查询补全表函数
        QString relayout ( int styleNumber );//重排版函数
        int autoIndent ( QString & codeString , int codePosition );//自动缩进函数
        int changeIndent ( QString & codeString , QString & lineString , int codePosition , int changeMode );//缩进变更函数
        QString makeFunctionRemark ( );//生成函数标准注释函数
};

#endif
