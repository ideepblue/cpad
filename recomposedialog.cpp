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
#include "recomposedialog.h"

RecomposeDialog::RecomposeDialog(QWidget *parent, CodeEditor *editor):
    QDialog(parent)
{
    codeEditor = editor;
    createWidget();
    generalSetting();
}

void RecomposeDialog::generalSetting()
{
    setWindowTitle(tr("重排版"));
    setFixedSize(430, 161);
    connect(yes, SIGNAL(clicked()),
            this, SLOT(readToRecompose()));
    connect(no, SIGNAL(clicked()),
            this, SLOT(closeDialog()));
    connect(this, SIGNAL(recomposeCode(int)),
            codeEditor, SLOT(recomposeContent(int)));
}

void RecomposeDialog::closeDialog()
{
    close();
}

void RecomposeDialog::readToRecompose()
{
    int code = 0;
    if (b1->isChecked())
        code += 1;
    if (b2->isChecked())
        code += 2;
    if (b3->isChecked())
        code += 4;
    if (b4->isChecked())
        code += 8;
    if (b5->isChecked())
        code += 16;
    if (b6->isChecked())
        code += 32;
    emit recomposeCode(code);
    close();
}

void RecomposeDialog::createWidget()
{
    groupBox = new QGroupBox(tr("重排版风格") ,this);

    b1 = new QCheckBox(tr("强制在声明后断行"), this);
    b1->setChecked(true);
    b2 = new QCheckBox(tr("强制在函数体后断行"), this);
    b2->setChecked(true);
    b3 = new QCheckBox(tr("左大括号单独一行"), this);
    b4 = new QCheckBox(tr("分号、逗号前加空格"), this);
    b5 = new QCheckBox(tr("括号内侧加空格"), this);
    b5->setChecked(true);
    b6 = new QCheckBox(tr("函数名、区块关键词后加空格"), this);
    b6->setChecked(true);
    
    inLayout = new QGridLayout(groupBox);
    
    inLayout->addWidget(b1, 0, 0);
    inLayout->addWidget(b2, 0, 1);
    inLayout->addWidget(b3, 1, 0);
    inLayout->addWidget(b4, 1, 1);
    inLayout->addWidget(b5, 2, 0);
    inLayout->addWidget(b6, 2, 1);

    groupBox->setLayout(inLayout);

    yes = new QPushButton(tr("确定"), this);
    no  = new QPushButton(tr("取消"), this);

    mainLayout = new QGridLayout(this);

    mainLayout->addWidget(groupBox, 0, 0, 4, 4);
    mainLayout->addWidget(yes, 4, 2);
    mainLayout->addWidget( no, 4, 3);
}

void RecomposeDialog::showDialog()
{
    show();
}
