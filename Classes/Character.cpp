//
//  Character.cpp
//  RPG
//
//  Created by Иван Гаврилов on 06.11.2019.
//  Copyright © 2019 Иван Гаврилов. All rights reserved.
//

#include "Character.hpp"
// Character methods
using namespace std::chrono_literals;
SDL_Texture* Character::getdefSprite() const { return this->curSprite; }
void Character::setdefSprite(SDL_Texture *tex) { this->curSprite = tex; }
SDL_RendererFlip Character::getRoute() const { return this->route; }
Position* Character::getPos() const { return this->pos; }
void Character::setPos( const Position &pos) { this->pos = new Position(pos); }
void Character::setStamina(int st) { this->stamina = st; }
int Character::getDamage() const { return this->damage; }
int Character::getStamina() const { return this->stamina; }
int Character::getHealth() const { return this->health; }
bool Character::isAnim() const { return this->isAnimate; }
bool Character::isRun() const { return this->isRunning; }
bool Character::isAttack() const { return this->isAttacking; }
int Character::getAnimType() const { return this->animType; }
void Character::setAnimType(int animType) { this->animType = animType; }
void Character::setHealth(int hp) { this->health = (hp <= 100) ? hp : 100; }
void Character::stopCharacter() { this->isRunning = false; }
Character::~Character() {
    int result;
    if (this->animationThread != nullptr && this->isAnimate) {
        this->isAnimate = false;
        SDL_WaitThread(this->animationThread, &result);
    }
    if (this->runningThread != nullptr && this->isRunning) {
        this->isRunning = false;
        SDL_WaitThread(this->runningThread, &result);
    }
}
void Character::stopAnim() {
    this->isAnimate = false;
    int result;
    SDL_WaitThread(this->animationThread, &result);
}
void Character::attack(Character *ch) {
    if (!this->isAttacking && this->stamina > 0) {
        this->isAttacking = true;
        this->isAnimate = false;
        int result;
        SDL_WaitThread(animationThread, &result);
        if (this->getPos()->getX() - ch->pos->getX() > 0)
            this->route = SDL_FLIP_HORIZONTAL;
        else
            this->route = SDL_FLIP_NONE;
        ch->health-=this->damage;
        this->stamina-=deltaStamina;
    }
}
int Character::animateThreadFunc(void *ptr) {
    animateData *data = (animateData *) ptr;
    for (auto i = data->texts.begin(); i != data->texts.end() && data->ch->isAnimate; ++i) {
        data->ch->curSprite = *i;
        for (int j = 0; j < data->iters*5000; j++) {
            if (!data->ch->isAnimate) {data->ch->animType = animNone; return 0;}
        }
    }
    if (data->ch->animType == animAttack)
        data->ch->isAttacking = false;
    if (data->ch->animType == animSpecAttack) {
        data->ch->isAttacking = false;
        data->ch->isSpecialAttack = false;
    }
    data->ch->isAnimate = false;
    data->ch->animType = animNone;
    return 1;
}
void Character::animate(const std::vector<SDL_Texture*> &texts, unsigned int iters, int animType) {
    if (!this->isAnimate) {
        this->animType = animType;
        this->isAnimate = true;
        animateData *data = new animateData;
        data->ch = this;
        data->texts = texts;
        data->iters = iters;
        animationThread = SDL_CreateThread(animateThreadFunc, "Animate", data);

    } else {
        this->isAnimate = false;
        int result;
        SDL_WaitThread(animationThread, &result);
        this->animate(texts, iters, animType);
    }
}
int Character::runningThreadFunc(void *ptr) {
    runningData *data = (runningData *) ptr;
    Position pos = data->pos;
    Character *ch = data->ch;
    float deltaX = ch->speed*(ch->getPos()->getCos(pos));
    float deltaY = ch->speed*(ch->getPos()->getSin(pos));
    float x = ch->getPos()->getFX();
    float y = ch->getPos()->getFY();
    while (ch->getPos()->calcDistance(pos) > 10.0f)  {
        x+=deltaX;
        y+=deltaY;
        ch->getPos()->setX(static_cast<int>(x));
        ch->getPos()->setY(static_cast<int>(y));
        if (!data->ch->isRunning) return 0;
        SDL_Delay(5);
    }
    ch->isRunning = false;
    ch->isAnimate = false;
    return 1;
}
void Character::goToPos(const Position &pos) {
    if (!this->isRunning && !this->isAttacking) {
        this->isRunning = true;
        if (this->animType != animRun)
            this->isAnimate = false;
        runningData *data = new runningData;
        data->ch = this;
        data->pos = pos;
        if (this->getPos()->getX() - pos.getX() > 0)
            this->route = SDL_FLIP_HORIZONTAL;
        else
            this->route = SDL_FLIP_NONE;
        runningThread = SDL_CreateThread(this->runningThreadFunc, "RunningThread", (void*)data);
    } else if (this->isRunning){
        this->isRunning = false;
        int status;
        SDL_WaitThread(this->runningThread, &status);
        this->goToPos(pos);
    }
}
// Enemy methods
Enemy::~Enemy() {
    int result;
    this->isDead = true;
    if (this->logicThread != nullptr)
        SDL_WaitThread(this->logicThread, &result);
}
int Enemy::logicThreadFunc(void *ptr) {
    logicData *data = (logicData*)ptr;
    Enemy *en = data->en;
    Player *pl = data->pl;
    Position buf;
    while (en->health > 0 && pl->getHealth() > 0 && !en->isDead) {
        if (pl->getPos()->calcDistance(*en->getPos()) < en->visibleArea && (!en->isRunning || pl->getPos()->calcDistance(buf) > 7.0f) && !(en->getPos()->calcDistance(*pl->getPos()) < 50.0f)) {
            en->goToPos(*pl->getPos());
            buf = *pl->getPos();
        } else if ((en->getAnimType() == animRun && en->isRunning) && (pl->getPos()->calcDistance(*en->getPos()) > en->visibleArea || en->getPos()->calcDistance(*pl->getPos()) < 50.0f) && !en->isAttacking) {
            en->isRunning = false; en->isAnimate = false;}
        else if (en->getPos()->calcDistance(*pl->getPos()) < 50.0f) {
            en->attack(pl);
        }
        SDL_Delay(500);
    }
    return 1;
}
void Enemy::initEnemyLogic(Player *pl) {
    logicData *data = new logicData;
    data->pl = pl;
    data->en = this;
    logicThread = SDL_CreateThread(this->logicThreadFunc, "EnemyLogic", data);
}
Enemy::Enemy() {
    this->speed = speed;
    this->isDead = false;
    this->isRunning = false;
    this->isAnimate = false;
    this->isAttacking = false;
    this->health = 100;
    this->stamina = 100;
    this->route = SDL_FLIP_NONE;
}
void Enemy::setDeath(bool isDeath) { this->isDead = isDeath; }
bool Enemy::getDeath() const { return this->isDead; }
// Golem Methods
Golem::Golem(const Position &pos, SDL_Texture *tex) {
    this->curSprite = tex;
    this->pos = new Position(pos);
    this->damage = 5;
    this->speed = 0.5f;
    this->visibleArea = 150;
}
bool Golem::getType() const { return false; }
// Goblin Methods
Goblin::Goblin(const Position &pos, SDL_Texture *tex) {
    this->curSprite = tex;
    this->pos = new Position(pos);
    this->damage = 3;
    this->speed = 0.6f;
    this->visibleArea = 200;
}
bool Goblin::getType() const { return true; }
// Player methods
Player::Player() {
    this->skillPoint = this->weaponLevel = this->specialSkill = 0;
    this->speed = 0.8f;
    this->pos = new Position(0,0);
    this->isDead = false;
    this->isRunning = false;
    this->isAnimate = false;
    this->isAttacking = false;
    this->isSpecialAttack = false;
    this->health = 100;
    this->stamina = 100;
    this->damage = 100;
    this->specialSkill = 15;
    this->route = SDL_FLIP_NONE;
}
std::string Player::getPlayerClassName() const { return this->PlayerClassName; }
std::string Player::getSpecialSkillName() const { return this->specialSkillName; }
int Player::getPlayerClass() const { return this->PlayerClass; }
bool Player::isSpecAttack() const { return this->isSpecialAttack; }
int Player::getDistance() const { return this->distance; }
void Player::setDistance(int distance) { this->distance = distance; }
Player::~Player() {
        // Nothing
}
// Wizard methods
Wizard::Wizard() {
    this->distance = 150;
    this->PlayerClassName = "Wizard";
    this->PlayerClass = 0;
    this->specialSkillName = "Cast";
}
void Wizard::SpecialAttack(Character *en) {
    if (this->stamina >= 20 && !this->isAttacking) {
        this->isSpecialAttack = true;
        this->health+=5;
        int buf = this->damage;
        this->damage = this->specialSkill;
        this->attack(en);
        this->stamina -= 15;
        this->damage = buf;
    }
}
// Warrior methods
Warrior::Warrior() {
    this->distance = 50;
    this->PlayerClassName = "Warrior";
    this->PlayerClass = 1;
    this->specialSkillName = "Super Attack";
}
void Warrior::SpecialAttack(Character *en) {
    if (this->stamina > 15 && !this->isAttacking) {
        int buf = this->damage;
        this->isSpecialAttack = true;
        this->damage = this->specialSkill + 5;
        this->attack(en);
        this->stamina -= 10;
        this->damage = buf;
    }
}
// Archer methods
Archer::Archer() {
    this->distance = 150;
    this->PlayerClassName = "Archer";
    this->PlayerClass = 2;
    this->specialSkillName = "Shot";
}
void Archer::SpecialAttack(Character *en) {
    if (this->stamina >= 20 && !this->isAttacking) {
        this->isSpecialAttack = true;
        if (this->distance < 250)
            this->distance+=10;
        else
            this->distance = 150;
        int buf = this->damage;
        this->damage = this->specialSkill;
        this->attack(en);
        this->stamina -= 15;
        this->damage = buf;
    }
}
