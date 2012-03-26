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
#include <QString>

//语法行首指针结构定义
struct LineHeadPointer
{
    int logicLevel;//存放该行逻辑等级
    int linePosition;//存放该行在原文中的位置
    struct LineHeadPointer *previousLine;//存放上一语法行
    struct LineHeadPointer *nextLine;//存放下一语法行
    struct LexicalAnalysis *firstWord;//存放本语法行首单词
};

//词法分析结构定义
struct LexicalAnalysis
{
    QString wordContent;//存放该词内容
    int wordLength;//存放原词长度
    int wordCategory;//存放原词类型
    int wordPosition;//存放原词在原文中位置
    struct LineHeadPointer *whichLine;//存放该词所在行
    struct LexicalAnalysis *previousWord;//存放上一语法词
    struct LexicalAnalysis *nextWord;//存放下一语法词
};

//函数列表结构定义
struct FunctionList
{
    QString functionName;//存放函数名称
    int functionNameLength;//存放函数名称长度
    QString functionCategory;//存放函数类型
    int functionCategoryLength;//存放函数类型长度
    int parameterNumber;//存放参数个数
    int functionPosition;//存放函数在原文中的位置
    int functionLine;//存放函数在原文中所在行
    QString functionOutput;//存放函数列表输出格式
    struct ParameterList *firstParameter;//存放参数表首个参数
    struct FunctionList *previousFunction;//存放上一个函数
    struct FunctionList *nextFunction;//存放下一个函数
};

//参数表结构定义
struct ParameterList
{
    QString parameterName;//存放参数名称
    int parameterNameLength;//存放参数名称长度
    QString parameterCategory;//存放参数类型
    int parameterCategoryLength;//存放参数类型长度
    QString parameterTail;//存放参数尾部
    int parameterTailLength;//存放参数尾部长度
    struct FunctionList *whichFunction;//存放该参数所在函数
    struct ParameterList *previousParameter;//存放上一参数
    struct ParameterList *nextParameter;//存放下一参数
};

//行号表结构定义
struct LineNumberList
{
    int lineNumber;//存放行号
    int linePosition;//存放当前行在原文的位置
    struct LineNumberList *nextLineNumber;//存放下一行号
};

//补全表结构定义
struct CompleList
{
    QString compleName;//存放变量名称
    int compleNameLength;//存放变量名称长度
    struct CompleList *previousComple;//存放上一变量
    struct CompleList *nextComple;//存放下一变量
};
