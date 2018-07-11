#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

// StudentWorld constructor
StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir) {}

// Initializes the StudentWorld
int StudentWorld::init()
{
    numShipsDestroyed = 0;
    
    // Adds the player to the world
    nb_vector.push_back(new NachenBlaster(IID_NACHENBLASTER, 0, 128, 0, 1, 0, this));
    
    // Adds 30 stars to the world
    for (int i = 0; i < 30; i++)
    {
        world_vector.push_back(new Star(IID_STAR, randInt(0, VIEW_WIDTH-1), randInt(0, VIEW_HEIGHT-1), 0, randInt(1, 10) * .05, 3));
    }
    
    // Uploads game statistics into the world
    ostringstream gameStats;
    gameStats << "Lives: " << getLives() << "  ";
    gameStats << "Health: " << nb_vector[0]->returnHealth() << "%  ";
    gameStats << "Score: " << getScore() << "  ";
    gameStats << "Level: " << getLevel() << "  ";
    gameStats << "Cabbages: " << nb_vector[0]->returnCabbagePercent() << "%  ";
    gameStats << "Torpedoes: " << nb_vector[0]->numTorpedoes();
    string s = gameStats.str();
    
    setGameStatText(s);
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    int needToDestroy = 6 + (4 * getLevel());
    int maxShips = 4 + (.5 * getLevel());
    
    // Has the player do something if it is alive, or tell the game if the player died
    if (nb_vector[0] != nullptr)
    {
        if (nb_vector[0]->alive == false)
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        else
            nb_vector[0]->doSomething();
    }
    
    // Has each actor do something if they are alive
    for (int i = 0; i < world_vector.size(); i++)
    {
        if (world_vector[i]->getX() <= -1)
            world_vector[i]->alive = false;
        
        else if (world_vector[i]->alive)
        {
            world_vector[i]->doSomething();
            if (nb_vector[0]->alive == false)
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
        }
    }
    
    // Checks if the player has finished the level
    if (numShipsDestroyed >= needToDestroy)
        return GWSTATUS_FINISHED_LEVEL;
    
    // Update the number of alien ships currently on the screen
    int currentNumShips = 0;
    for (int i = 0; i < world_vector.size(); i++)
    {
        if (world_vector[i]->isAlien())
            currentNumShips++;
    }
    
    // Add new alien ships onto the screen
    if (currentNumShips < min(needToDestroy-numShipsDestroyed, maxShips))
    {
        int S1 = 60;
        int S2 = 20 + getLevel() * 5;
        int S3 = 5 + getLevel() * 10;
        int S = S1 + S2 + S3;
        int r = randInt(1, S);
        if (r <= S1)
            world_vector.push_back(new Smallgon(IID_SMALLGON, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), 0, 1.5, 1, this));
        else if (r > S1 && r <= S1+S2)
            world_vector.push_back(new Smoregon(IID_SMOREGON, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), 0, 1.5, 1, this));
        else if (r > S1+S2)
            world_vector.push_back(new Snagglegon(IID_SNAGGLEGON, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), 0, 1.5, 1, this));
    }
    
    // Add more stars onto the screen
    if (randInt(1, 15) == 5)
    {
        world_vector.push_back(new Star(IID_STAR, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), 0, randInt(1, 10) * .05, 3));
    }
    
    // Delete all the dead actors
    deleteDeadThings();
    
    // Update all the game statistics on the screen
    ostringstream gameStats;
    gameStats << "Lives: " << getLives() << "  ";
    gameStats << "Health: " << nb_vector[0]->returnHealth() << "%  ";
    gameStats << "Score: " << getScore() << "  ";
    gameStats << "Level: " << getLevel() << "  ";
    gameStats << "Cabbages: " << nb_vector[0]->returnCabbagePercent() << "%  ";
    gameStats << "Torpedoes: " << nb_vector[0]->numTorpedoes();
    string s = gameStats.str();
    
    setGameStatText(s);
    
    return GWSTATUS_CONTINUE_GAME;
}

// Delete everything from the world, dead or alive
void StudentWorld::cleanUp()
{
    for (vector<Actor*> :: iterator it = world_vector.begin(); it != world_vector.end(); it++)
    {
        if (*it != nullptr)
            delete (*it);
    }
    world_vector.clear();

    for (vector<NachenBlaster*> :: iterator is = nb_vector.begin(); is != nb_vector.end(); is++)
    {
        if (*is != nullptr)
            delete (*is);
    }
    nb_vector.clear();
}

// Delete all the dead actors from the world
void StudentWorld::deleteDeadThings()
{
    vector<Actor*> :: iterator it = world_vector.begin();
    for (int i = 0; i < world_vector.size(); i++)
    {
        if (world_vector[i]->alive == false)
        {
            Actor* killme = *it;
            it = world_vector.erase(it);
            delete killme;
            i--;
        }
        else
            it++;
    }
    
    vector<NachenBlaster*> :: iterator is = nb_vector.begin();
    for (int i = 0; i < nb_vector.size(); i++)
    {
        if (nb_vector[i]->alive == false)
        {
            Actor* killme = *is;
            is = nb_vector.erase(is);
            delete killme;
            i--;
        }
        else
            is++;
    }
}

// Return the pointer to an alien if a has collided with it
// Return nullptr if it hasn't collided with an alien
Actor* StudentWorld::getOneCollidingAlien(const Actor *a) const
{
    for (int i = 0; i < world_vector.size(); i++)
    {
        if (world_vector[i]->isAlien())
        {
            double first = a->getX() - world_vector[i]->getX();
            double second = a->getY() - world_vector[i]->getY();
            first *= first;
            second *= second;
            double dist = sqrt(first + second);
            if (dist < .75 * (a->getRadius() + world_vector[i]->getRadius()))
                return world_vector[i];
        }
    }
    return nullptr;
}

// Return the pointer to the player if a has collided with it
// Return nullptr if it hasn't collided with the player
Actor* StudentWorld::getCollidingPlayer(const Actor *a) const
{
    if (nb_vector[0] != nullptr)
    {
        double first = a->getX() - nb_vector[0]->getX();
        double second = a->getY() - nb_vector[0]->getY();
        first *= first;
        second *= second;
        double dist = sqrt(first + second);
        if (dist < .75 * (a->getRadius() + nb_vector[0]->getRadius()))
            return nb_vector[0];
    }
    return nullptr;
}

// Returns true if the player is in a's line of fire
// Returns false if it's not
bool StudentWorld::playerInLineOfFire(const Actor* a) const
{
    if (nb_vector[0] != nullptr)
    {
        if (nb_vector[0]->getX() < a->getX())
        {
            int y = nb_vector[0]->getY() - a->getY();
            if (y >= -4 && y <= 4)
                return true;
        }
    }
    return false;
}

// Adds a to the world vector of actors
void StudentWorld::addActor(Actor *a)
{
    world_vector.push_back(a);
}

// Increases the number of aliens destroyed by one
void StudentWorld::recordAlienDestroyed()
{
    numShipsDestroyed++;
}

// Returns a pointer to the player
NachenBlaster* StudentWorld::getPlayer()
{
    return nb_vector[0];
}











