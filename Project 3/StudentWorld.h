#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld() {};
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void deleteDeadThings();
    
    Actor* getOneCollidingAlien(const Actor* a) const;
    Actor* getCollidingPlayer(const Actor* a) const;
    bool playerInLineOfFire(const Actor* a) const;
    void addActor(Actor* a);
    void recordAlienDestroyed();
    NachenBlaster* getPlayer();
    
    std::vector<Actor*> world_vector;
    std::vector<NachenBlaster*> nb_vector;
private:
    int numShipsDestroyed;
};

#endif // STUDENTWORLD_H_
