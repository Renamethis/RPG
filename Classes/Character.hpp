//
//  Character.hpp
//  RPG
//
//  Created by Иван Гаврилов on 06.11.2019.
//  Copyright © 2019 Иван Гаврилов. All rights reserved.
//

#ifndef Character_hpp
#define Character_hpp
#include <iostream>
#include <SDL.h>
#include <vector>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <SDL_thread.h>
#include "GameClasses.hpp"
#include "sdllibrary.hpp"
#define deltaStamina 5
#define animRun 111
#define animStay 112
#define animAttack 113
#define animSpecAttack 114
#define animDeath 115
#define animNone 0
class Character {
public:
    SDL_Texture * getdefSprite() const;
    void setdefSprite(SDL_Texture *tex);
    Position *getPos() const;
    ~Character();
    void setPos(const Position &pos);
    void setStamina(int st);
    int getDamage() const;
    int getStamina() const;
    int getHealth() const;
    int getAnimType() const;
    void setAnimType(int animType);
    bool isAnim() const;
    bool isRun() const;
    bool isAttack() const;
    void setHealth(int hp);
    void goToPos(const Position &pos);
    void animate(const std::vector<SDL_Texture*> &texts, unsigned int iters, int animType);
    void stopCharacter();
    void attack(Character *ch);
    void stopAnim();
    SDL_RendererFlip getRoute() const;
protected:
    int health, stamina, damage;
    float speed;
    Position *pos;
    bool isDead;
    bool isRunning;
    bool isAnimate;
    bool isAttacking;
    bool isSpecialAttack;
    SDL_Texture *curSprite;
    SDL_RendererFlip route;
private:
    int animType;
    static int runningThreadFunc(void *ptr);
    static int animateThreadFunc(void *ptr);
    SDL_Thread *runningThread = nullptr;
    SDL_Thread *animationThread = nullptr;
    typedef struct {
        Character *ch;
        Position pos;
    } runningData;
    typedef struct {
        Character *ch;
        std::vector<SDL_Texture*> texts;
        unsigned int iters;
    } animateData;
};
class Player: public Character {
public:
    Player();
    virtual ~Player();
    int getDistance() const;
    void setDistance(int distance);
    std::string getPlayerClassName() const;
    std::string getSpecialSkillName() const;
    int getPlayerClass() const;
    const std::string name;
    virtual void SpecialAttack(Character *en) = 0;
    bool isSpecAttack() const;
protected:
    int skillPoint, weaponLevel, specialSkill, distance;
    int PlayerClass;
    std::string PlayerClassName;
    std::string specialSkillName;
};
class Enemy: public Character {
public:
    virtual ~Enemy();
    void initEnemyLogic(Player *pl);
    void setDeath(bool isDeath);
    bool getDeath() const;
    virtual bool getType() const = 0;
    Enemy();
protected:
    static int logicThreadFunc(void *ptr);
    SDL_Thread *logicThread = nullptr;
    int visibleArea;
    int level;
    const int walkingArea = 20;
    typedef struct {
        Player *pl;
        Enemy *en;
    } logicData;
};
class Goblin : public Enemy {
public:
    Goblin(const Position &pos, SDL_Texture *tex);
    virtual bool getType() const;
};
class Golem : public Enemy {
public:
    Golem(const Position &pos, SDL_Texture *tex);
    virtual bool getType() const;
};
class Wizard : public Player {
public:
    Wizard();
    virtual void SpecialAttack(Character *en);
};
class Warrior : public Player {
public:
    Warrior();
    virtual void SpecialAttack(Character *en);
};
class Archer : public Player {
public:
    Archer();
    virtual void SpecialAttack(Character *en);
};
#endif /* Character_hpp */
