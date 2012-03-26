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
#include "cpadpalette.h"
#include <QtGui>

CpadPalette::CpadPalette()
{
    //[0 comment, 1 constant, 2 datatype, 3 othertype, 4 macro,
    // 5 transfer; 6 background, 7 numberarea, 8 alert, 9 normal,
    // 10 highlightline  11 parentheses]

    desertDark << QColor(138, 203, 237) << QColor(249, 162, 159) << QColor(190, 182, 108)
            << QColor(223, 235, 149) << QColor(203, 95, 88 ) << QColor(194, 219, 178)
            << QColor(51 ,51 ,51 ) << QColor(253,255,0  ) << QColor(253, 1, 0)
               << QColor(Qt::white) << QColor(180 , 238, 180).darker(120) << QColor(Qt::darkGreen);

    koehlerDark << QColor(126,163,250) << QColor(249,162,159) << QColor(97 ,253,97 )
            << QColor(252,255,96 ) << QColor(253,129,253)  << QColor(255,163,3  )
            << QColor(0  ,0  ,0  ) << QColor(253,255,0  ) << QColor(253,1  ,0  )
                <<QColor(Qt::white) << QColor(180 , 238, 180).darker(120) <<  QColor(Qt::darkGreen);


    morningLight << QColor(0  ,1  ,253)  << QColor(255,0  ,255)  << QColor(44 ,139,86 )
            << QColor(126,68 ,60 ) << QColor(162,33 ,237)  << QColor(123,85 ,198)
            << QColor(255,255,255)  << QColor(102,76 ,69 ) << QColor(253,1  ,0  )
            << QColor(0  ,0  ,0  ) << QColor(228, 240, 253) << QColor(Qt::green).lighter(150);

    ronLight << QColor(128,128,128) << QColor(249,162,159)  << QColor(44 ,139,86 )
            << QColor(142,59 ,60 ) << QColor(162,33 ,237)  << QColor(128,128,128)
            << QColor(255,255,255) << QColor(253,255,0  ) << QColor(253,1  ,0  )
            << QColor(0  ,0  ,0  ) << QColor(228, 240, 253) << QColor(Qt::green).lighter(150);

    sampleLight << QColor(0  ,130,0  ) << QColor(0  ,3  ,123)  << QColor(129,129,0  )
                << QColor(129,129,0  )  << QColor(0  ,3  ,123)   << QColor(0  ,130,0  )
                << QColor(255,255,255)  << QColor(160,160,160)   << QColor(0  ,130,0  )
                << QColor(0  ,0  ,0  ) << QColor(228,240,253) << QColor(Qt::green).lighter(150);

    darkDark << QColor(126,163,250) << QColor(231,157,141)  << QColor(97 ,253,97 )
            << QColor(252,255,96 )  << QColor(250,125,235) << QColor(246,157,11 )
            << QColor(51 ,51 ,51 ) << QColor(242,240,8  ) << QColor(253,1  ,0  )
             << QColor(Qt::white) << QColor(180 , 238, 180).darker(120) << QColor(Qt::darkGreen);

    defaultGroup = morningLight;
    highlightForSearch = QColor(Qt::red);
}


void CpadPalette::myFavorate(int group)
{

    //0 desert   1 koehler   2 morning   3 ron   4 sample   5 dark
    if ( group == 0 ) {
        defaultGroup = desertDark;
    }
    else if ( group == 1 ) {
        defaultGroup = koehlerDark;
    }
    else if ( group == 2 ) {
        defaultGroup = morningLight;
    }
    else if ( group == 3 ) {
        defaultGroup = ronLight;
    }
    else if ( group == 4 ) {
        defaultGroup = sampleLight;
    }
    else if ( group == 5 ) {
        defaultGroup = darkDark;
    }

    emit groupChanged(defaultGroup);
}
