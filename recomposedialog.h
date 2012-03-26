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
#ifndef RECOMPOSEDIALOG_H
#define RECOMPOSEDIALOG_H

#include <QDialog>
#include "codeeditor.h"

QT_BEGIN_NAMESPACE
class QGridLayout;
class QGroupBox;
class QPushButton;
class QCheckBox;
class QLabel;
QT_END_NAMESPACE

class RecomposeDialog : public QDialog
{
        Q_OBJECT
    public:
        RecomposeDialog(QWidget *parent, CodeEditor *editor);

    signals:
        void recomposeCode(int code);

    private slots:
        void showDialog();
        void generalSetting();
        void readToRecompose();
        void closeDialog();

    private:
        void createWidget(); //建立空间 摆位置
        QGroupBox *groupBox;
        QCheckBox *b1, //强制在声明后断行
                  *b2, //强制在函数体后断行
                  *b3, //左大括号单独一行
                  *b4, //分号、逗号前加空格 
                  *b5, //括号内侧加空格
                  *b6; //函数名后加空格
        QGridLayout *mainLayout, *inLayout;
        QPushButton *yes, *no;
        QLabel *label;
        CodeEditor *codeEditor;
};

#endif // RECOMPOSEDIALOG_H
