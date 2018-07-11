//
//  main.cpp
//  CS32.Project.1
//
//  Created by Neta Tartakovsky on 1/10/18.
//  Copyright © 2018 Neta Tartakovsky. All rights reserved.
//

#include "Game.h"
#include "City.h"
#include "History.h"
#include "Player.h"
#include "Flatulan.h"
#include "globals.h"

int main()
{
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);
    
    // Play the game
    g.play();
}
