#include "Actor.h"
#include "StudentWorld.h"
#include "iostream"
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor constructor
Actor::Actor(int imageID, int startX, int startY, int startDirection, double size, int depth)
: GraphObject(imageID, startX, startY, startDirection, size, depth) {}

// Star constructor
Star::Star(int imageID, int startX, int startY, int startDirection, double size, int depth)
: Actor(imageID, startX, startY, startDirection, size, depth) {}

// Tells star object to move to the left and marks it dead if it leaves the screen
void Star::doSomething()
{
    moveTo(getX()-1, getY());
    if (getX() < 0)
        alive = false;
    return;
}

// Explosion constructor
Explosion::Explosion(int imageID, int startX, int startY, int startDirection, double size, int depth)
: Actor(imageID, startX, startY, startDirection, size, depth) { t_count = 0; }

// Tells the explosion to grow until the function has been called 4 times, then marks it dead
void Explosion::doSomething()
{
    if (t_count >= 4)
        alive = false;
    else
    {
        setSize(1.5 * getSize());
        t_count++;
    }
}

// DamageableObject constructor
DamageableObject::DamageableObject(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr)
: Actor(imageID, startX, startY, startDirection, size, depth), worldptr(worldptr) {}

// Spaceship constructor
Spaceship::Spaceship(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr)
: DamageableObject(imageID, startX, startY, startDirection, size, depth, worldptr) {}

// NachenBlaster constructor
NachenBlaster::NachenBlaster(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* world)
: Spaceship(imageID, startX, startY, startDirection, size, depth, world)
{
    hitPoints = 50;
    cabbagePoints = 30;
    torpedo_count = 0;
}

// If NachenBlaster is dead, immediately returns
// Otherwise, has NachenBlaster act according to user input
void NachenBlaster::doSomething()
{
    
    if (!alive)
        return;
    else
    {
        int ch;
        if (worldptr->getKey(ch))
        {
            switch (ch)
            {
                case KEY_PRESS_LEFT :
                    if (getX() - 6 >= 0)
                        moveTo(getX() - 6, getY());
                    break;
                case KEY_PRESS_RIGHT :
                    if (getX() + 6 < VIEW_WIDTH)
                        moveTo(getX() + 6, getY());
                    break;
                case KEY_PRESS_UP :
                    if (getY() + 6 < VIEW_HEIGHT)
                        moveTo(getX(), getY() + 6);
                    break;
                case KEY_PRESS_DOWN :
                    if (getY() - 6 >= 0)
                        moveTo(getX(), getY() - 6);
                    break;
                case KEY_PRESS_SPACE :
                    if (cabbagePoints >= 5)
                    {   worldptr->addActor(new Cabbage(IID_CABBAGE, getX() + 12, getY(), 0, .5, 1, worldptr));
                        worldptr->playSound(SOUND_PLAYER_SHOOT);
                        cabbagePoints -= 5;   }
                    break;
                case KEY_PRESS_TAB :
                    if (torpedo_count > 0)
                    {
                        torpedo_count--;
                        worldptr->addActor(new Torpedo(IID_TORPEDO, getX() + 12, getY(), 0, .5, 1, worldptr));
                        worldptr->playSound(SOUND_TORPEDO);
                    }
                    break;
            }
        }
        
        if (cabbagePoints < 30)
            cabbagePoints++;
    }
}

// Returns the percentage of cabbage points that the NachenBlaster currently has
int NachenBlaster::returnCabbagePercent()
{
    double cabPercent = static_cast<double>(cabbagePoints) / 30.0 * 100;
    int returnAmt = cabPercent;
    return returnAmt;
}

// Causes the NachenBlaster to lose a certain amounts of hit points
// If the hit points dip below zero, marks it dead and adds an explosion
// If still alive, and hit by a projectile, plays SOUND_BLAST
void NachenBlaster::sufferDamage(int damage)
{
    hitPoints -= damage;
    if (hitPoints <= 0)
    {
        alive = false;
        worldptr->addActor(new Explosion(IID_EXPLOSION, getX(), getY(), 0, 1, 0));
    }
    
    if (damage == 2 || damage == 8)
        worldptr->playSound(SOUND_BLAST);
    
    return;
}

// Alien constructor
Alien::Alien(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr)
: Spaceship(imageID, startX, startY, startDirection, size, depth, worldptr) {}

// If the alien is dead, returns immediately
// If alien is out of bounds, marks it dead and returns
// Checks for a collision, returns if one occurred
// Adjusts flight plan if it needs to, shoots if it needs to, and then moves according to flight plan
// Decrements flight plan length and then checks for a collision again
void Alien::doSomething()
{
    if (!alive)
        return;
    
    else if (getX() < 0)
    {   alive = false; return;  }
    
    checkForCollision();
    
    if (!alive)
        return;
    
    else if (fp_length == 0 || getY() <= 0 || getY() >= VIEW_HEIGHT - 1)
        checkNewFP();
    
    if (worldptr->playerInLineOfFire(this))
    {
        if (maybeShoot())
            return;
        maybeSwitchFP();
    }
    
    switch (fp_dir)
    {
        case 1 :
            moveTo(getX() - speed, getY() - speed);
            break;
        case 2 :
            moveTo(getX() - speed, getY());
            break;
        case 3 :
            moveTo(getX() - speed, getY() + speed);
            break;
    }
    
    fp_length--;
    
    checkForCollision();
}

// Sets a new flight plan if alien is about to go out of bounds or if it has reached the end of its flight plan
void Alien::checkNewFP()
{
    if (getY() >= VIEW_HEIGHT-1)
        fp_dir = 1;
    else if (getY() <= 0)
        fp_dir = 3;
    else if (fp_length == 0)
        fp_dir = randInt(1, 3);
    fp_length = randInt(1, 32);
}

// Possibly causes the alien to shoot a turnip, and if it does, it plays SOUND_ALIEN_SHOOT
// Returns true if it shot and false if it didn't
bool Alien::maybeShoot()
{
    if (randInt(1, (20.0/worldptr->getLevel())+5) == 1)
    {
        worldptr->addActor(new Turnip(IID_TURNIP, getX() - 14, getY(), 0, .5, 1, worldptr));
        worldptr->playSound(SOUND_ALIEN_SHOOT);
        return true;
    }
    return false;
}

// It calls a StudentWorld method to get a pointer to the player it collided with
// If a collision did occur and the pointer is not null, it acts accordingly
// Returns true if it did collide into the player and false if it didn't
bool Alien::checkForCollision()
{
    Actor* a = worldptr->getCollidingPlayer(this);
    if (a != nullptr)
    {
        a->sufferDamage(amtDamage());
        alive = false;
        worldptr->recordAlienDestroyed();
        worldptr->increaseScore(amtScore());
        worldptr->playSound(SOUND_DEATH);
        worldptr->addActor(new Explosion(IID_EXPLOSION, getX(), getY(), 0, 1, 0));
        maybeDropGoodie();
        return true;
    }
    return false;
}

// Causes the Alien to lose a certain amounts of hit points
// If the hit points dip below zero, marks it dead and adds an explosion, plays a sound, increases the score, records it in the StudentWorld, and maybe drops a goodie
// If still alive, plays SOUND_BLAST
void Alien::sufferDamage(int damage)
{
    hitPoints -= damage;
    if (hitPoints <= 0)
    {
        alive = false;
        worldptr->playSound(SOUND_DEATH);
        worldptr->recordAlienDestroyed();
        worldptr->increaseScore(amtScore());
        maybeDropGoodie();
        worldptr->addActor(new Explosion(IID_EXPLOSION, getX(), getY(), 0, 1, 0));
    }
    else
        worldptr->playSound(SOUND_BLAST);
    return;
}

// Smallgon constructor
Smallgon::Smallgon(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr)
: Alien(imageID, startX, startY, startDirection, size, depth, worldptr)
{
    hitPoints = 5 * (1 + (worldptr->getLevel() - 1) * .1);
    fp_length = 0;
    speed = 2;
}

// Smoregon constructor
Smoregon::Smoregon(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr)
: Alien(imageID, startX, startY, startDirection, size, depth, worldptr)
{
    hitPoints = 5 * (1 + (worldptr->getLevel() - 1) * .1);
    fp_length = 0;
    speed = 2;
}

// Possibly switches Smoregon's flight plan and causes it to go straight to the left at a faster speed
void Smoregon::maybeSwitchFP()
{
    if (randInt(1, (20.0/worldptr->getLevel())+5) == 1)
    {
        fp_dir = 2;
        fp_length = VIEW_WIDTH;
        speed = 5;
    }
    return;
}

// There's a 1 in 3 chance that this function creates either a RepairGoodie or a TorpedoGoodie
void Smoregon::maybeDropGoodie()
{
    if (randInt(1, 3) == 2)
    {
        int num = randInt(1, 2);
        if (num == 1)
            worldptr->addActor(new RepairGoodie(IID_REPAIR_GOODIE, getX(), getY(), 0, .5, 1, worldptr));
        else if (num == 2)
            worldptr->addActor(new TorpedoGoodie(IID_TORPEDO_GOODIE, getX(), getY(), 0, .5, 1, worldptr));
    }
}

// Snagglegon constructor
Snagglegon::Snagglegon(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr)
: Alien(imageID, startX, startY, startDirection, size, depth, worldptr)
{
    hitPoints = 10 * (1 + (worldptr->getLevel() - 1) * .1);
    fp_dir = 1;
    fp_length = 0;
    speed = 1.75;
}

// Sets a new flight plan for the Snagglegon if it is about to go out of bounds
void Snagglegon::checkNewFP()
{
    if (getY() >= VIEW_HEIGHT-1)
        fp_dir = 1;
    else if (getY() <= 0)
        fp_dir = 3;
}

// Possibly causes the Snagglegon to shoot a torpedo and play a sounds
// Returns true if it did shoot and false if it didn't
bool Snagglegon::maybeShoot()
{
    if (randInt(1, (15.0/worldptr->getLevel())+5) == 1)
    {
        worldptr->world_vector.push_back(new Torpedo(IID_TORPEDO, getX()-14, getY(), 180, .5, 1, worldptr));
        worldptr->playSound(SOUND_TORPEDO);
        return true;
    }
    return false;
}

// There is a 1 in 6 chance that this will create an ExtraLifeGoodie on the screen
void Snagglegon::maybeDropGoodie()
{
    if (randInt(1, 6) == 3)
        worldptr->addActor(new ExtraLifeGoodie(IID_LIFE_GOODIE, getX(), getY(), 0, .5, 1, worldptr));
}

// Projectile constructor
Projectile::Projectile(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr)
: DamageableObject(imageID, startX, startY, startDirection, size, depth, worldptr) {}

// Checks if the projectile is alive and within the bounds and returns if it's not
// Checks for a collision and returns if it happened
// Moves according to what type of projectile it is and then checks for a collision again
void Projectile::doSomething()
{
    if (!alive)
        return;
    
    else if (getX() >= VIEW_WIDTH || getX() < 0)
    {   alive = false; return;  }
    
    else if (checkForCollision())
        return;
    
    else
    {
        moveAccordingly();
        checkForCollision();
    }
}

// Cabbage constructor
Cabbage::Cabbage(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr)
: Projectile(imageID, startX, startY, startDirection, size, depth, worldptr) {}

// Moves the cabbage 8 pixels to the right and rotates it 20 degrees
void Cabbage::moveAccordingly()
{
    moveTo(getX() + 8, getY());
    int dir = getDirection() + 20;
    if (dir > 360)
        dir -= 360;
    setDirection(dir);
}

// Checks if the cabbage has collided with an alien
// If it has, causes damage to the alien, sets itself to dead, and returns true (otherwise, returns false)
bool Cabbage::checkForCollision()
{
    Actor* a = worldptr->getOneCollidingAlien(this);
    if (a != nullptr)
    {
        a->sufferDamage(2);
        alive = false;
        return true;
    }
    return false;
}

// Turnip constructor
Turnip::Turnip(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr)
: Projectile(imageID, startX, startY, startDirection, size, depth, worldptr) {}

// Moves the turnip 6 pixels to the left and rotates it 20 degrees
void Turnip::moveAccordingly()
{
    moveTo(getX() - 6, getY());
    int dir = getDirection() + 20;
    if (dir > 360)
        dir -= 360;
    setDirection(dir);
}

// Checks if the cabbage has collided with the player
// If it has, causes damage to the player, sets itself to dead, and returns true (otherwise, returns false)
bool Turnip::checkForCollision()
{
    Actor* a = worldptr->getCollidingPlayer(this);
    if (a != nullptr)
    {
        a->sufferDamage(2);
        alive = false;
        return true;
    }
    return false;
}

// Torpedo constructor
Torpedo::Torpedo(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr)
: Projectile(imageID, startX, startY, startDirection, size, depth, worldptr) {}

// Moves torpedo 8 pixels to the right if the player shot it, and 8 to the left if an alien shot it
void Torpedo::moveAccordingly()
{
    if (getDirection() == 0)
        moveTo(getX() + 8, getY());
    
    else if (getDirection() == 180)
        moveTo(getX() - 8, getY());
}

// Checks if it has collided with its enemy
// If it has, causes damage to its enemy, sets itself to dead, and returns true (otherwise, returns false)
bool Torpedo::checkForCollision()
{
    if (getDirection() == 180)
    {
        Actor* a = worldptr->getCollidingPlayer(this);
        if (a != nullptr)
        {
            a->sufferDamage(8);
            alive = false;
            return true;
        }
    }
    
    if (getDirection() == 0)
    {
        Actor* a = worldptr->getOneCollidingAlien(this);
        if (a != nullptr)
        {
            a->sufferDamage(8);
            alive = false;
            return true;
        }
    }
    
    return false;
}

// Goodie constructor
Goodie::Goodie(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr)
: DamageableObject(imageID, startX, startY, startDirection, size, depth, worldptr) {}

// Checks if it is alive and in bounds, and returns if it's not
// Checks for a collision and returns if one occurred
// Moves down and left and checks for a collision again
void Goodie::doSomething()
{
    if (!alive)
        return;
    
    else if (getX() < 0 || getY() < 0)
    {   alive = false; return;  }
    
    if (checkForCollision())
        return;
    
    moveTo(getX() - .75, getY() - .75);
    
    checkForCollision();
}

// Checks if it has collided with the player
// If it has, increases the score, sets itself to dead, plays a sound, rewards the player, and returns true
// Otherwise, returns false
bool Goodie::checkForCollision()
{
    Actor* a = worldptr->getCollidingPlayer(this);
    if (a != nullptr)
    {
        worldptr->increaseScore(100);
        alive = false;
        worldptr->playSound(SOUND_GOODIE);
        NachenBlaster* nb = worldptr->getPlayer();
        reward(nb);
        return true;
    }
    return false;
}

// ExtraLifeGoodie constructor
ExtraLifeGoodie::ExtraLifeGoodie(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr)
: Goodie(imageID, startX, startY, startDirection, size, depth, worldptr) {}

// Increases the player's lives by one
void ExtraLifeGoodie::reward(NachenBlaster* nb)
{
    worldptr->incLives();
}

// RepairGoodie constructor
RepairGoodie::RepairGoodie(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr)
: Goodie(imageID, startX, startY, startDirection, size, depth, worldptr) {}

// Gives the player an extra 10 hit points (or however many before reaching max hit points)
void RepairGoodie::reward(NachenBlaster* nb)
{
    nb->hitPoints += 10;
    if (nb->hitPoints > 50)
        nb->hitPoints = 50;
}

// TorpedoGoodie constructor
TorpedoGoodie::TorpedoGoodie(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr)
: Goodie(imageID, startX, startY, startDirection, size, depth, worldptr) {}

// Gives the player 5 torpedoes
void TorpedoGoodie::reward(NachenBlaster* nb)
{
    nb->incTorpedoes(5);
}















