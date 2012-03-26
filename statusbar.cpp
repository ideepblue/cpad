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
#include "statusbar.h"

StatusBar::StatusBar(QWidget *parent):
    QStatusBar(parent)
{
    fileNameLabel = new QLabel(" ", this);
    fileDirLabel = new QLabel(" ", this);
    textCursorLabel = new QLabel("Line: 1:0 Total: 1", this);

    QFont font("Courier New", 10);
    font.setWeight(QFont::DemiBold);
    setFont(font);

    fileNameLabel->setFrameShape(QFrame::StyledPanel);
    fileNameLabel->setFrameShadow(QFrame::Sunken);
    fileDirLabel->setFrameShape(QFrame::StyledPanel);
    fileDirLabel->setFrameShadow(QFrame::Sunken);
    textCursorLabel->setFrameShape(QFrame::StyledPanel);
    textCursorLabel->setFrameShadow(QFrame::Sunken);

    addWidget(fileNameLabel);
    addWidget(fileDirLabel);
    addWidget(textCursorLabel);
}

void StatusBar::setCodeEditor(CodeEditor *editor)
{
    codeEditor = editor;
}

void StatusBar::updateTextCursorLabel()
{
    QTextCursor tcr = codeEditor->textCursor();//光标
    QTextBlock blk = tcr.block();//当前光标 block
    int pos = tcr.position() - blk.position();//计算光标和 block 的相对位置

    QString str( tr("Line: %1:%2 Total: %3")
            .arg(codeEditor->textCursor().blockNumber()+1)
            .arg(pos)
            .arg(codeEditor->blockCount())
            );
    textCursorLabel->setText(str);
}

void StatusBar::updateCurrentDirLabel(QString currentDir)
{
    fileDirLabel->setText("Dir: " + currentDir);
}

void StatusBar::updateCurrentFileLabel(QString currentFile)
{
    fileNameLabel->setText("File: " + currentFile);
}
