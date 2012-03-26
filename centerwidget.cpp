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
#include "centerwidget.h"

CenterWidget::CenterWidget(QWidget *parent):
    QWidget(parent)
{
    codeEditor = new CodeEditor(this);
    centerLayout = new QGridLayout(this);
    syntaxHighlighter = new SyntaxHighlighter(codeEditor->document());

    centerLayout->addWidget( codeEditor, 0, 0, 1, 1 );
    centerLayout->setSpacing(0);
    centerLayout->setContentsMargins( 0, 0, 0, 0 );

    setLayout(centerLayout);
}
