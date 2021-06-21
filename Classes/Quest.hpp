//
//  Quest.hpp
//  Role Play Game
//
//  Created by Иван Гаврилов on 09.12.2019.
//  Copyright © 2019 Иван Гаврилов. All rights reserved.
//

#ifndef Quest_hpp
#define Quest_hpp

#include <stdio.h>
#include <iostream>
#include <list>
#include <fstream>
#include "Character.hpp"
class Quest {
public:
    virtual ~Quest();
    const std::list<Enemy *> &getList() const;
    std::list<Enemy *> &getList();
    virtual void startQuest(Player *pl) = 0;
    virtual bool isCompleted() const = 0;
    std::string getName() const;
    std::string getDescription() const;
    virtual void fwrite(std::ofstream &ofs) = 0;
    virtual bool fread(std::ifstream &ifs, SDL_Texture *type1, SDL_Texture *type2) = 0;
    virtual int getType() const = 0;
    virtual Position getPos() const = 0;
protected:
    std::string name;
    std::string description;
    std::list<Enemy *> EnemyList;
    SDL_Thread *questThread;
};
class KillQuest : public Quest {
public:
    KillQuest();
    KillQuest(std::string name, std::string des, const std::list<Enemy *> &list);
    virtual void startQuest(Player *pl);
    virtual void fwrite(std::ofstream &ofs);
    virtual bool fread(std::ifstream &ifs, SDL_Texture *type1, SDL_Texture *type2);
    virtual bool isCompleted() const;
    virtual int getType() const;
    virtual Position getPos() const;
};
class LootQuest : public Quest {
public:
    LootQuest();
    LootQuest(std::string name, std::string des, const std::list<Enemy *> &list, Position pos, int type);
    virtual void startQuest(Player *pl);
    virtual void fwrite(std::ofstream &ofs);
    virtual bool fread(std::ifstream &ifs, SDL_Texture *type1, SDL_Texture *type2);
    virtual bool isCompleted() const;
    virtual int getType() const;
    virtual Position getPos() const;
private:
    int type;
    Position *playerPos;
    Position pos;
};
#endif /* Quest_hpp */
