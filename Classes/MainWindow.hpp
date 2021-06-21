//
//  MainWindow.hpp
//  RPG
//
//  Created by admin on 05.12.2019.
//  Copyright © 2019 infdash. All rights reserved.
//

#ifndef MainWindow_hpp
#define MainWindow_hpp

#include <stdio.h>
#include <iostream>
#include "sdllibrary.hpp"
#include "Character.hpp"
#include "Quest.hpp"
#include "Obstacle.hpp"
#include <fstream>
#define GoblinPath "Goblin.txt"
#define GolemPath "Golem.txt"
#define QuestPath "QuestData.txt"
#define ObstacleCount 1
#define runIters 8000
#define animIters 10000
class MainWindow {
private:
    class Toast {
    public:
        Toast(int x, int y, int w, int h, std::string text);
        ~Toast();
        void show();
        Uint8 getAlpha() const;
        void setText(std::string text);
        std::string getText() const;
        bool isShow() const;
        const int x, y;
        const int w, h;
    private:
        static int toastThreadFunc(void *ptr);
        bool isShowing = false;
        std::string text;
        Uint8 alpha;
        SDL_Thread *alphaThread = nullptr;
    };
public:
    MainWindow(int w, int h);
    ~MainWindow();
    void startGameThread();
private:
    std::string playerClass;
    const int w, h;
    void drawPlayerInfo();
    void drawEnemyHP(const Enemy &en);
    void drawEnemy(Enemy &en);
    void drawToast(const Toast &toast);
    void chooseClass();
    void onDied();
    void onWin();
    void drawPlayer();
    bool initRenderer();
    bool loadResources();
    bool loadPlayerResources();
    bool loadQuests();
    std::vector<SDL_Texture*> playerAnim[4]; // 0 - default, 1 - run, 2 - attack, 3 - special attack
    std::vector<SDL_Texture*> goblinAnim[3]; // 0 = Run, 1 - Attack, 2 - Death
    std::vector<SDL_Texture*> golemAnim[3]; // 0 - Run, 1 - Attack, 2 - Death
    std::string playerAnimPaths[4];
    std::string golemPaths[3];
    std::string goblinPaths[3];
    std::string chestsPaths[2] = {"resources/chest1.png", "resources/chest2.png"};
    std::string objectPaths[2] = {"resources/heart.png", "resources/tree.png"};
    SDL_Texture *chests[2];
    SDL_Texture *objects[2];
    Player *pl;
    Quest *currentQuest;
    std::list<Quest *> questList;
    std::list <GameObject *> objectList;
    SDL_Texture *playerBarBack;
    SDL_Texture *playerBarFront;
    SDL_Texture *playerBarContent;
    SDL_Renderer *ren;
    SDL_Window *win;
    SDL_Cursor *defCursor;
    SDL_Cursor *permAttackCursor;
    SDL_Cursor *unpermAttackCursor;
    SDL_Texture *map;
    SDL_Texture *dialogTexture;
    SDL_Texture *blending;
    TTF_Font *font;
    Toast *mainToast;
    bool isQuit = false;
    const SDL_Color gray = {122, 122, 122, 255};
    const SDL_Color green = {0, 220, 0, 255};
    const SDL_Color red = {255, 0, 0, 255};
    const SDL_Color lightgray = {170, 170, 170, 255};
    const SDL_Color blue = {0, 50, 200, 200};
    const SDL_Color black = {0, 0, 0, 255};
};
#endif /* MainWindow_hpp */
