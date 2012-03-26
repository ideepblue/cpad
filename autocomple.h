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
#ifndef AUTOCOMPLE_H
#define AUTOCOMPLE_H

#include <QListWidget>
#include "codeeditor.h"
#include "memorycenter.h"

class AutoComple : public QListWidget
{
        Q_OBJECT
    public:
        AutoComple(CodeEditor *editor);
    protected:
        void keyPressEvent(QKeyEvent *event);
    private slots:
        void doAutoComple(int mode=1);
        void compleString(QListWidgetItem *item);
        void fontSettings(QFont f);
    private:
        bool autoCompleCheck(int mode);
        int matchNumber, matchLength;
        CodeEditor *codeEditor;
        MemoryCenter *memoryCenter;
};

#endif // AUTOCOMPLE_H
