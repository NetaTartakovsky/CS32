//
//  History.cpp
//  CS32.Project.1
//
//  Created by Neta Tartakovsky on 1/11/18.
//  Copyright © 2018 Neta Tartakovsky. All rights reserved.
//

#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    for (int r = 0; r < nRows; r++)
    {
        for (int c = 0; c < nCols; c++)
        {
            historyRecord[r][c] = '.';
        }
    }
}

bool History::record(int r, int c)
{
    if (r > m_rows || r < 1)
        return false;
    if (c > m_cols || c < 1)
        return false;
    r--;
    c--;
    switch (historyRecord[r][c])
    {
        case '.' :
            historyRecord[r][c] = 'A';
            break;
        case 'A' :
            historyRecord[r][c] = 'B';
            break;
        case 'B' :
            historyRecord[r][c] = 'C';
            break;
        case 'C' :
            historyRecord[r][c] = 'D';
            break;
        case 'D' :
            historyRecord[r][c] = 'E';
            break;
        case 'E' :
            historyRecord[r][c] = 'F';
            break;
        case 'F' :
            historyRecord[r][c] = 'G';
            break;
        case 'G' :
            historyRecord[r][c] = 'H';
            break;
        case 'H' :
            historyRecord[r][c] = 'I';
            break;
        case 'I' :
            historyRecord[r][c] = 'J';
            break;
        case 'J' :
            historyRecord[r][c] = 'K';
            break;
        case 'K' :
            historyRecord[r][c] = 'L';
            break;
        case 'L' :
            historyRecord[r][c] = 'M';
            break;
        case 'M' :
            historyRecord[r][c] = 'N';
            break;
        case 'N' :
            historyRecord[r][c] = 'O';
            break;
        case 'O' :
            historyRecord[r][c] = 'P';
            break;
        case 'P' :
            historyRecord[r][c] = 'Q';
            break;
        case 'Q' :
            historyRecord[r][c] = 'R';
            break;
        case 'R' :
            historyRecord[r][c] = 'S';
            break;
        case 'S' :
            historyRecord[r][c] = 'T';
            break;
        case 'T' :
            historyRecord[r][c] = 'U';
            break;
        case 'U' :
            historyRecord[r][c] = 'V';
            break;
        case 'V' :
            historyRecord[r][c] = 'W';
            break;
        case 'W' :
            historyRecord[r][c] = 'X';
            break;
        case 'X' :
            historyRecord[r][c] = 'Y';
            break;
        case 'Y' :
            historyRecord[r][c] = 'Z';
            break;
        case 'Z' :
            break;
        default:
            break;
    }
    return true;
}

void History::display() const
{
    // Draw the grid
    clearScreen();
    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++)
        {
            cout << historyRecord[r][c];
        }
        cout << endl;
    }
    cout << endl;
}

