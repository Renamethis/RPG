//
//  GameClasses.cpp
//  RPG
//
//  Created by Иван Гаврилов on 06.11.2019.
//  Copyright © 2019 Иван Гаврилов. All rights reserved.
//

#include "GameClasses.hpp"
// Position methods
Position::Position() {
    this->x = this->y = 0;
}
Position::Position(int x, int y) {
    this->x = x;
    this->y = y;
}
Position::Position(const Position &pos) {
    this->x = pos.getX();
    this->y = pos.getY();
}
bool Position::isCollision(Position pos, int areaw, int areah) const {
    if (pos.getX() - areaw/2 < this->x && pos.getY() - areah < this->y && pos.getX()+areaw/2 > this->x && pos.getY()+areah/2 > this->y) return true;
    return false;
}
bool Position::isCollision(Position pos, int areaw, int areah, SDL_RendererFlip flip) const {
    if (flip == SDL_FLIP_NONE) {
        if (pos.getX() - areaw < this->x && pos.getY() - areah < this->y && pos.getX()+areaw/2 > this->x && pos.getY()+areah/2 > this->y) return true;
        return false;
    } else {
        if (pos.getX()-areaw*1.5 < this->x && pos.getY() - areah < this->y && pos.getX() > this->x && pos.getY()+areah/2 > this->y) return true;
        return false;
    }
}
int Position::getX() const { return this->x; }
int Position::getY() const { return this->y; }
float Position::getFX() const { return static_cast<float>(this->x); }
float Position::getFY() const { return static_cast<float>(this->y); }
void Position::setX(const int &x){if (x < winWidth && x > 0) this->x = x;}
void Position::setY(const int &y){if (y < winHeight && y > 0) this->y = y;}
void Position::setPos(const int &x, const int &y){this->y=y;this->x=x;}
void Position::changePos(const Position &pos){
    this->x = pos.getX();
    this->y = pos.getY();
}
float Position::calcDistance(Position pos) const {
    return (sqrt(pow((this->x-pos.getX()), 2) + pow((this->y-pos.getY()), 2)));
}
float Position::getTan(const Position &pos) const {
    return (pos.getX() - this->x != 0) ? ((pos.getY()-this->y)/(pos.getX()-this->x)) : 0.0f;
}
float Position::getCos(const Position &pos) const {
    return (this->calcDistance(pos) != 0.0f) ? (pos.getX() - this->x)/(this->calcDistance(pos)) : 0.0f;
}
float Position::getSin(const Position &pos) const {
    return (this->calcDistance(pos) != 0.0f) ? (pos.getY() - this->y)/(this->calcDistance(pos)) : 0.0f;
}
