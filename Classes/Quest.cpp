//
//  Quest.cpp
//  Role Play Game
//
//  Created by Иван Гаврилов on 09.12.2019.
//  Copyright © 2019 Иван Гаврилов. All rights reserved.
//

#include "Quest.hpp"
// Quest
Quest::~Quest() {
    for (auto i = this->EnemyList.begin(); i != this->EnemyList.end(); i++)
        delete *i;
    this->EnemyList.clear();
}
std::string Quest::getName() const { return this->name; }
std::string Quest::getDescription() const { return this->description; }
const std::list<Enemy *> &Quest::getList() const { return this->EnemyList; }
std::list<Enemy *> &Quest::getList() { return this->EnemyList; }
// KillQuest
KillQuest::KillQuest() {
    this->name = "";
    this->description = "";
}
KillQuest::KillQuest(std::string name, std::string des, const std::list<Enemy *> &list) {
    this->EnemyList = list;
    this->name = name;
    this->description = des;
}
void KillQuest::startQuest(Player *pl) {
    for (auto i = this->EnemyList.begin(); i != this->EnemyList.end(); i++)
        (*i)->initEnemyLogic(pl);
}
bool KillQuest::isCompleted() const {
    if (this->EnemyList.size() > 0) return false;
    return true;
}
int KillQuest::getType() const { return -1; }
void KillQuest::fwrite(std::ofstream &ofs) {
    ofs << true << "\n";
    ofs << this->name << " ";
    ofs << this->description << "\n";
    ofs << this->EnemyList.size() << "\n";
    for (auto i = this->EnemyList.begin(); i != this->EnemyList.end(); i++) {
        ofs << (*i)->getType() << "\n";
        ofs << (*i)->getPos()->getX() << "\n";
        ofs << (*i)->getPos()->getY() << "\n";
    }
    ofs << "\n";
}
Position KillQuest::getPos() const { return Position(-1,-1); }
bool KillQuest::fread(std::ifstream &ifs, SDL_Texture *type1, SDL_Texture *type2) {
    ifs >> this->name;
    std::getline(ifs, this->description);
    if (this->description.empty()) return false;
    int size;
    if(!(ifs >> size)) return false;
    bool type;
    int x, y;
    for (int i = 0; i < size; i++) {
        Enemy *en;
        if(!(ifs >> type >> x >> y)) return false;
        if (type) en = new Goblin(Position(x,y), type1);
        else en = new Golem(Position(x,y), type2);
        this->EnemyList.push_back(en);
    }
    return true;
}
// LootQuest
LootQuest::LootQuest(std::string name, std::string des, const std::list<Enemy *> &list, Position pos, int type) {
    this->EnemyList = list;
    this->name = name;
    this->description = des;
    this->type = type;
    this->pos = pos;
}
LootQuest::LootQuest() {
    this->name = "";
    this->description = "";
    this->type = -1;
}
int LootQuest::getType() const { return this->type; }
void LootQuest::fwrite(std::ofstream &ofs) {
    ofs << false << "\n";
    ofs << this->name << " ";
    ofs << this->description << "\n";
    ofs << this->EnemyList.size() << "\n";
    ofs << this->type;
    ofs.write(reinterpret_cast<char *>(&this->pos), sizeof(Position));
    for (auto i = this->EnemyList.begin(); i != this->EnemyList.end(); i++) {
        ofs << (*i)->getType() << "\n";
        ofs << (*i)->getPos()->getX() << "\n";
        ofs << (*i)->getPos()->getY() << "\n";
    }
    ofs << "\n";
}
bool LootQuest::fread(std::ifstream &ifs, SDL_Texture *type1, SDL_Texture *type2) {
    ifs >> this->name;
    std::getline(ifs, this->description);
    if (this->description.empty()) return false;
    int size;
    if(!(ifs >> size >> this->type)) return false;
    ifs.read(reinterpret_cast<char *>(&this->pos), sizeof(Position));
    bool type;
    int x, y;
    for (int i = 0; i < size; i++) {
        Enemy *en;
        if(!(ifs >> type >> x >> y)) return false;
        if (type)
            en = new Goblin(Position(x,y), type1);
        else
            en = new Golem(Position(x,y), type2);
        this->EnemyList.push_back(en);
    }
    return true;
}
Position LootQuest::getPos() const { return this->pos; }
void LootQuest::startQuest(Player *pl) {
    this->playerPos = pl->getPos();
    for (auto i = this->EnemyList.begin(); i != this->EnemyList.end(); i++)
        (*i)->initEnemyLogic(pl);
}
bool LootQuest::isCompleted() const {
    if (playerPos->isCollision(this->pos, 40, 40) && this->EnemyList.size() == 0) return true;
    return false;
}
