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
#ifndef CPADPALETTE_H
#define CPADPALETTE_H

#include <QList>
#include <QColor>
#include <QObject>

class QTextCharFormat;

class CpadPalette : public QObject
{
    Q_OBJECT
public:
    CpadPalette();

    QList<QColor> defaultGroup;
    QColor highlightForSearch;  //查找高亮时候的背景

public slots:
    void myFavorate(int group);

signals:
    void groupChanged(QList<QColor>);

private:
    //[0 comment, 1 constant, 2 datatype, 3 othertype, 4 macro,
    // 5 transfer; 6 background, 7 numberarea, 8 alert, 9 normal,
    // 10 highlightline 11 parentheses ]

    QList<QColor> desertDark;
    QList<QColor> koehlerDark;
    QList<QColor> morningLight;
    QList<QColor> ronLight;
    QList<QColor> sampleLight;
    QList<QColor> darkDark;

};

#endif // CPADPALETTE_H
