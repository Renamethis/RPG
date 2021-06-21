//
//  Obstacle.hpp
//  Role Play Game
//
//  Created by Иван Гаврилов on 12.12.2019.
//  Copyright © 2019 Иван Гаврилов. All rights reserved.
//

#ifndef Obstacle_hpp
#define Obstacle_hpp

#include <stdio.h>
#include "GameClasses.hpp"
#include "Character.hpp"
class GameObject {
public:
    virtual void ifCollision(Player *pl) = 0;
    virtual int getType() const = 0;
    virtual ~GameObject();
    bool isColided() const;
    void setCol(bool col);
    int getW() const;
    int getH() const;
    Position getPos() const;
protected:
    Position pos;
    int w, h;
    bool isCollided = false;
};
class FirstAid: public GameObject {
public:
    FirstAid(Position pos, int w, int h);
    virtual void ifCollision(Player *pl);
    virtual int getType() const;
};
class Obstacle: public GameObject {
public:
    Obstacle(Position pos, int type, int w, int h);
    virtual void ifCollision(Player *pl);
    virtual int getType() const;
private:
    int type;
};
#endif /* Obstacle_hpp */
