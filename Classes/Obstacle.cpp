//
//  Obstacle.cpp
//  Role Play Game
//
//  Created by Иван Гаврилов on 12.12.2019.
//  Copyright © 2019 Иван Гаврилов. All rights reserved.
//

#include "Obstacle.hpp"
// GameObject
Position GameObject::getPos() const { return this->pos; }
GameObject::~GameObject() {
    // Nothing
}
bool GameObject::isColided() const { return this->isCollided; }
int GameObject::getW() const { return this->w; }
int GameObject::getH() const { return this->h; }
void GameObject::setCol(bool col) { this->isCollided = col; }
// FirstAid
FirstAid::FirstAid(Position pos, int w, int h) {
    this->pos = pos;
    this->w = w;
    this->h = h;
}
void FirstAid::ifCollision(Player *pl) {
    pl->setHealth(pl->getHealth() + 15);
    delete this;
}
int FirstAid::getType() const { return 0; }
// Obstacle
Obstacle::Obstacle(Position pos, int type, int w, int h) {
    this->pos = pos;
    this->type = type;
    this->w = w;
    this->h = h;
}
void Obstacle::ifCollision(Player *pl) {
    pl->stopCharacter();
    this->isCollided = true;
}
int Obstacle::getType() const { return this->type; }
