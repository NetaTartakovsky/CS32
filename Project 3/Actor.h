#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, int startDirection, double size, int depth);
    virtual ~Actor() {};
    virtual void doSomething() = 0;
    bool alive = true;
    virtual bool isAlien() {return false;};
    virtual void sufferDamage(int damage) {return;};
};

class Star : public Actor
{
public:
    Star(int imageID, int startX, int startY, int startDirection, double size, int depth);
    virtual ~Star() {};
    virtual void doSomething();
};

class Explosion : public Actor
{
public:
    Explosion(int imageID, int startX, int startY, int startDirection, double size, int depth);
    virtual ~Explosion() {};
    virtual void doSomething();
private:
    int t_count;
};

class DamageableObject : public Actor
{
public:
    DamageableObject(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~DamageableObject() {};
    StudentWorld* worldptr;
private:
    virtual bool checkForCollision() = 0;
};

class Spaceship : public DamageableObject
{
public:
    Spaceship(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~Spaceship() {};
    int hitPoints;
};

class NachenBlaster : public Spaceship
{
public:
    NachenBlaster(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~NachenBlaster() {};
    virtual void doSomething();
    void incTorpedoes(int num) {torpedo_count += num;};
    int numTorpedoes() {return torpedo_count;};
    int returnHealth() {return hitPoints*2;};
    int returnCabbagePercent();
private:
    virtual bool checkForCollision() {return false;};
    virtual void sufferDamage(int damage);
    int cabbagePoints;
    int torpedo_count;
};

class Alien : public Spaceship
{
public:
    Alien(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~Alien() {};
    virtual void doSomething();
    int fp_length;
    double speed;
    int fp_dir;
private:
    virtual bool isAlien() {return true;};
    virtual bool checkForCollision();
    virtual void sufferDamage(int damage);
    virtual void maybeSwitchFP() {return;};
    virtual void checkNewFP();
    virtual bool maybeShoot();
    virtual int amtDamage() {return 5;};
    virtual int amtScore() {return 250;};
    virtual void maybeDropGoodie() {return;};
};

class Smallgon : public Alien
{
public:
    Smallgon(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~Smallgon() {};
};

class Smoregon : public Alien
{
public:
    Smoregon(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~Smoregon() {};
private:
    virtual void maybeSwitchFP();
    virtual void maybeDropGoodie();
};

class Snagglegon : public Alien
{
public:
    Snagglegon(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~Snagglegon() {};
private:
    virtual void checkNewFP();
    virtual bool maybeShoot();
    virtual void maybeDropGoodie();
    virtual int amtDamage() {return 15;};
    virtual int amtScore() {return 1000;};
};

class Projectile : public DamageableObject
{
public:
    Projectile(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~Projectile() {};
    virtual void doSomething();
private:
    virtual bool checkForCollision() = 0;
    virtual void moveAccordingly() = 0;
};

class Cabbage : public Projectile
{
public:
    Cabbage(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~Cabbage() {};
private:
    virtual void moveAccordingly();
    virtual bool checkForCollision();
};

class Turnip : public Projectile
{
public:
    Turnip(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~Turnip() {};
private:
    virtual void moveAccordingly();
    virtual bool checkForCollision();
};

class Torpedo : public Projectile
{
public:
    Torpedo(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~Torpedo() {};
private:
    virtual void moveAccordingly();
    virtual bool checkForCollision();
};

class Goodie : public DamageableObject
{
public:
    Goodie(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~Goodie() {};
    virtual void doSomething();
private:
    virtual bool checkForCollision();
    virtual void reward(NachenBlaster* nb) = 0;
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~ExtraLifeGoodie() {};
private:
    virtual void reward(NachenBlaster* nb);
};

class RepairGoodie : public Goodie
{
public:
    RepairGoodie(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~RepairGoodie() {};
private:
    virtual void reward(NachenBlaster* nb);
};

class TorpedoGoodie : public Goodie
{
public:
    TorpedoGoodie(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* worldptr);
    virtual ~TorpedoGoodie() {};
private:
    virtual void reward(NachenBlaster* nb);
};













#endif // ACTOR_H_
