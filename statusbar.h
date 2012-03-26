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
#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>
#include "codeeditor.h"

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class StatusBar : public QStatusBar
{
        Q_OBJECT
    public:
        StatusBar(QWidget *parent);
        void setCodeEditor(CodeEditor *edit);
    private slots:
        void updateTextCursorLabel();
        void updateCurrentDirLabel(QString currentDir);
        void updateCurrentFileLabel(QString currentFile);
    private:
        QLabel *fileNameLabel,//文件名
               *fileDirLabel,//文件路径
               *textCursorLabel;//光标
        CodeEditor *codeEditor;//编辑器

};

#endif // STATUSBAR_H
