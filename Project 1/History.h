//
//  History.hpp
//  CS32.Project.1
//
//  Created by Neta Tartakovsky on 1/11/18.
//  Copyright © 2018 Neta Tartakovsky. All rights reserved.
//

#ifndef History_Included
#define History_Included

#include "globals.h"

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
private:
    int m_rows;
    int m_cols;
    int numTimes;
    char historyRecord[MAXROWS][MAXCOLS];
};

#endif // History_Included
