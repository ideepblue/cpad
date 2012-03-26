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
#include "widget.h"
#include <QtGui>

Widget::Widget(QWidget *parent)
    : QMessageBox(parent)
{
    this->setIcon(QMessageBox::Warning);
    this->setWindowTitle(trUtf8("清除 cPad 注册表信息"));
    this->setText(trUtf8(
            "cPad 注册表信息记录了您使用 cPad 的一些习惯\n"
            "您是否仍要删除 cPad 注册表信息？"));
    yesButton = this->addButton(trUtf8("确定"),QMessageBox::AcceptRole);
    noButton  = this->addButton(trUtf8("取消"),QMessageBox::RejectRole);
        //QSettings settings("5-up", "cPad");
        //settings.remove("");
    connect(this, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(checkResult(QAbstractButton*)));
}

void Widget::checkResult(QAbstractButton *button)
{
    if (button == yesButton) {
        QSettings settings("5-up", "cPad");
        settings.remove("");
        QMessageBox::information(this, tr("信息"),
                                 tr("cPad 注册表信息清理完毕！\n"
                                        "如需卸载 cPad 直接删除安装目录下的所有文件即可。"),
                                 tr("确定"),
                                 0, 0);
    }
}
