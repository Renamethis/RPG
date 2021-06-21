//
//  GameClasses.hpp
//  RPG
//
//  Created by Иван Гаврилов on 06.11.2019.
//  Copyright © 2019 Иван Гаврилов. All rights reserved.
//

#ifndef GameClasses_hpp
#define GameClasses_hpp
#include <cmath>
#include <iostream>
#include <list>
#include <SDL.h>
#define winWidth 1280
#define winHeight 620
class Position {
public:
    Position();
    Position(int x, int y);
    Position(const  Position &pos);
    bool isCollision(Position pos, int areaw, int areah) const;
    bool isCollision(Position pos, int areaw, int areah, SDL_RendererFlip flip) const;
    int getX() const;
    int getY() const;
    float getFX() const;
    float getFY() const;
    void setX(const int &x);
    void setY(const int &y);
    void setPos(const int &y, const int &x);
    void changePos(const Position &pos);
    float calcDistance(Position pos) const;
    float getTan(const Position &pos) const;
    float getCos(const Position &pos) const;
    float getSin(const Position &pos) const;
private:
    int x, y;
};
#endif /* GameClasses_hpp */
