//
//  global.hpp
//  CS32.Project.1
//
//  Created by Neta Tartakovsky on 1/10/18.
//  Copyright © 2018 Neta Tartakovsky. All rights reserved.
//

#ifndef Globals_Included
#define Globals_Included

const int MAXROWS = 20;               // max number of rows in a city
const int MAXCOLS = 30;               // max number of columns in a city
const int MAXFLATULANS = 100;         // max number of Flatulans allowed
const int INITIAL_PLAYER_HEALTH = 10;
    
const int UP      = 0;
const int DOWN    = 1;
const int LEFT    = 2;
const int RIGHT   = 3;
const int NUMDIRS = 4;
    
int randInt(int min, int max);
void clearScreen();


#endif // Globals_Included



