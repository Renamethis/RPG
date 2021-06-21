//
//  MainWindow.cpp
//  RPG
//
//  Created by admin on 05.12.2019.
//  Copyright © 2019 infdash. All rights reserved.
//

#include "MainWindow.hpp"
MainWindow::Toast::Toast(int x, int y, int w, int h, std::string text) : x(x), y(y), w(w), h(h) {
    this->alpha = 1;
    this->text = text;
}
MainWindow::Toast::~Toast() {
    int status;
    this->isShowing = false;
    if (this->alphaThread != nullptr)
        SDL_WaitThread(this->alphaThread, &status);
}
std::string MainWindow::Toast::getText() const {
    return this->text;
}
void MainWindow::Toast::setText(std::string text) {
    this->text = text;
}
Uint8 MainWindow::Toast::getAlpha() const {
    return this->alpha;
}
int MainWindow::Toast::toastThreadFunc(void *ptr) {
    Toast *toast = (Toast *) ptr;
    for (int i = 1; i < 255 && toast->isShowing; i++) {
        toast->alpha = i;
        SDL_Delay(12);
    }
    SDL_Delay(200);
    for (int i = 255; i >= 1 && toast->isShowing; i--) {
        toast->alpha = i;
        SDL_Delay(12);
    }
    toast->isShowing = false;
    return 1;
}
void MainWindow::Toast::show() {
    if (!isShowing) {
        this->isShowing = true;
        this->alphaThread = SDL_CreateThread(this->toastThreadFunc, "Toast", this);
    }
}
bool MainWindow::Toast::isShow() const {
    return this->isShowing;
}
MainWindow::MainWindow(int w, int h) : h(h), w(w) {
    if (!this->initRenderer()) {
        std::cout << "Something wrong with initializing";
        exit(0);
    }
    this->font = TTF_OpenFont("resources/mainFont.ttf", 24);
    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);
    TTF_Init();
    if (!this->loadResources()) {
        std::cout << "Problem with resources!";
        exit(0);
    }
    if (!this->loadQuests()) {
        std::cout << "Problem with loading quests";
        exit(0);
    }
    this->currentQuest = *this->questList.begin();
    for (int i = 0; i < 3; i++) {
        int type = rand() % ObstacleCount;
        GameObject *obj = new Obstacle(Position(rand() % 1280, rand() % 720), type+1, 50, 82);
        this->objectList.push_back(obj);
    }
    this->mainToast = new Toast(935, 520, 345, 100, "Mission Complete");
}
MainWindow::~MainWindow() {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_DestroyTexture(this->playerBarBack);
    SDL_DestroyTexture(this->playerBarContent);
    SDL_DestroyTexture(this->playerBarFront);
    SDL_DestroyTexture(this->map);
    SDL_DestroyTexture(this->dialogTexture);
    SDL_DestroyTexture(this->blending);
    for (int i = 0; i < 4; i++)
        for (auto j = this->playerAnim[i].begin(); j != this->playerAnim[i].end(); j++)
            SDL_DestroyTexture(*j);
    for (auto i = this->questList.begin(); i != this->questList.end(); i++) {
        delete *i;
    }
    for (auto i = this->objectList.begin();  i != this->objectList.end(); i++)
        delete *i;
    for (int i = 0; i < 2; i++)
        SDL_DestroyTexture(this->objects[i]);
    SDL_FreeCursor(this->defCursor);
    SDL_FreeCursor(this->permAttackCursor);
    delete this->mainToast;
    delete this->pl;
    SDL_Quit();
    TTF_Quit();
}
void MainWindow::drawPlayerInfo() {
    renderRect(this->ren, 0, 620, 1280, 100, this->gray);
    renderTexture(this->ren, this->playerBarContent
                   , 35, 652, (int)130.0f*(this->pl->getHealth()/100.0f), 12, SDL_FLIP_NONE);
    renderTexture(this->ren, this->playerBarBack, 10, 630, 164, 44, SDL_FLIP_NONE);
    renderTexture(this->ren, this->playerBarFront, 10, 630, 164, 44, SDL_FLIP_NONE);
    renderRect(this->ren, 20, 680, 145, 18, this->lightgray);
    renderRect(this->ren, 21, 681, static_cast<int>(143.0f*(this->pl->getStamina()/100.0f)), 16, this->green);
    renderText(ren, 52, 677, 80, 18, "Stamina", font, this->blue);
    renderRect(this->ren, 175, 652, 145, 50, this->lightgray);
    renderText(this->ren, 176, 653, 144, 15, "Quest: " + this->currentQuest->getName(), font, this->black);
    renderText(this->ren, 176, 670, 144, 15, "Description: " + this->currentQuest->getDescription(), font, this->black);
}
void MainWindow::drawEnemyHP(const Enemy &en) {
    renderRect(this->ren, en.getPos()->getX()+6, en.getPos()->getY() - 15, 50, 7, gray);
    renderRect(this->ren, en.getPos()->getX()+6, en.getPos()->getY() - 15, static_cast<int>(50.0f*(en.getHealth()/100.0f)), 7, green);
}
void MainWindow::drawEnemy(Enemy &en) {
    if (en.getType()) {
        if (en.isRun() && !en.isAnim())
            en.animate(goblinAnim[0], runIters, animRun);
        else if (en.isAttack() && !en.isAnim())
            en.animate(goblinAnim[1], animIters*1.5, animAttack);
        else if (!en.isAnim() && en.getHealth() <= 0 && !en.getDeath()) {
            en.animate(goblinAnim[2], animIters*1.5, animDeath);
            en.setDeath(true);
        } else if (!en.isAnim())
            en.setdefSprite(*this->goblinAnim[0].begin());
    } else {
        if (en.isRun() && !en.isAnim())
            en.animate(golemAnim[0], runIters, animRun);
        else if (en.isAttack() && !en.isAnim())
            en.animate(golemAnim[1], animIters*1.5, animAttack);
        else if (!en.isAnim() && en.getHealth() <= 0 && !en.getDeath()) {
            en.animate(golemAnim[2], animIters*1.5, animDeath);
            en.setDeath(true);
        } else if (!en.isAnim())
            en.setdefSprite(*this->golemAnim[0].begin());
    }
    renderTexture(ren, en.getdefSprite(), en.getPos()->getX(), en.getPos()->getY(), 64, 64, en.getRoute());
    drawEnemyHP(en);
}
void MainWindow::drawToast(const Toast &toast) {
    SDL_Color textColor = {167, 191, 88, toast.getAlpha()};
    SDL_SetTextureAlphaMod(this->dialogTexture, toast.getAlpha());
    renderTexture(ren, this->dialogTexture, toast.x, toast.y, toast.w, toast.h, SDL_FLIP_NONE);
    renderText(ren, toast.x + toast.w/4, toast.y + toast.h/2.3f, static_cast<int>(toast.w*0.6f), static_cast<int>(toast.h*0.4f), toast.getText(), this->font, textColor);
    
}
void MainWindow::drawPlayer() {
    if (pl->isSpecAttack() && pl->isAttack() && !pl->isAnim())
        pl->animate(playerAnim[3], animIters, animSpecAttack);
    else if (pl->isAttack() && !pl->isAnim())
        pl->animate(playerAnim[2], animIters, animAttack);
    else if (pl->isRun() && !pl->isAnim() && !pl->isAttack())
        pl->animate(playerAnim[1], runIters, animRun);
    else if (!pl->isAnim())
        pl->animate(playerAnim[0], animIters, animStay);
    renderTexture(ren, pl->getdefSprite(), pl->getPos()->getX(), pl->getPos()->getY(), 64, 64, pl->getRoute());
    
}
bool MainWindow::initRenderer() {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cout << "Init error! Error: " << SDL_GetError() << std::endl;
        return false;
    }
    win = SDL_CreateWindow("Role Play Game", 0, 0, this->w, this->h, SDL_WINDOW_SHOWN);
    if (win == nullptr) return false;
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) return false;
    return true;
}
bool MainWindow::loadResources() {
    std::ifstream ifs;
    ifs.open(GoblinPath);
    if (!ifs.good()) return false;
    for (int i = 0; i < 3; i++)
        ifs >> this->goblinPaths[i];
    ifs.close();
    ifs.open(GolemPath);
    if (!ifs.good()) return false;
    for (int i = 0; i < 3; i++)
        ifs >> this->golemPaths[i];
    for (int i = 0; i < 3; i++) {
        this->golemAnim[i] = LoadImages(this->ren, this->golemPaths[i], "png");
        this->goblinAnim[i] = LoadImages(this->ren, this->goblinPaths[i], "png");
        if(this->golemAnim[i].size() == 0 || this->goblinAnim[i].size() == 0) return false;
    }
    for (int i = 0; i < 2; i++) {
        this->chests[i] = LoadImage(this->ren, this->chestsPaths[i]);
        if(this->chests[i] == nullptr) return false;
    }
    for (int i = 0; i < 2; i++) {
        this->objects[i] = LoadImage(this->ren, this->objectPaths[i]);
        if(this->objects[i] == nullptr) return false;
    }
    this->playerBarBack = LoadImage(this->ren, "resources/playerBar/lifebar_back.png");
    this->playerBarFront = LoadImage(this->ren, "resources/playerBar/lifebar_front.png");
    this->playerBarContent = LoadImage(this->ren, "resources/playerBar/lifebar_content.png");
    SDL_SetTextureColorMod(this->playerBarContent, this->red.r, this->red.g, this->red.b);
    SDL_SetTextureAlphaMod(this->playerBarContent, 255);
    SDL_Surface *cursorDefSurface = SDL_LoadBMP("resources/cursors/default.bmp");
    SDL_Surface *cursorAttackSurface1 = SDL_LoadBMP("resources/cursors/permattack.bmp");
    SDL_Surface *cursrorAttackSurface2 = SDL_LoadBMP("resources/cursors/unpermattack.bmp");
    this->defCursor = SDL_CreateColorCursor(cursorDefSurface, 0, 0);
    this->permAttackCursor = SDL_CreateColorCursor(cursorAttackSurface1, 0, 0);
    this->unpermAttackCursor = SDL_CreateColorCursor(cursrorAttackSurface2, 0, 0);
    this->map = LoadImage(ren, "resources/map.png");
    this->dialogTexture = LoadImage(ren, "resources/dialog.png");
    this->blending = LoadImage(ren, "resources/blending.png");
    this->font = TTF_OpenFont("resources/mainFont.ttf", 24);
    SDL_SetTextureBlendMode(this->blending, SDL_BLENDMODE_MOD);
    SDL_SetTextureAlphaMod(this->blending, 100);
    if (this->map == nullptr || this->permAttackCursor == nullptr || this->defCursor == nullptr || this->playerBarBack == nullptr || this->playerBarFront == nullptr || this->playerBarContent == nullptr || this->font == nullptr || this->dialogTexture == nullptr || this->blending == nullptr) return false;
    SDL_SetTextureAlphaMod(this->dialogTexture, 0);
    SDL_FreeSurface(cursorAttackSurface1);
    SDL_FreeSurface(cursrorAttackSurface2);
    SDL_FreeSurface(cursorDefSurface);
    return true;
}
bool MainWindow::loadQuests() {
    std::ifstream ifs;
    ifs.open(QuestPath);
    if (!ifs.good()) return false;
    bool type;
    while (ifs >> type) {
        Quest *newQuest;
        if (type)
            newQuest = new KillQuest();
        else
            newQuest = new LootQuest();
        if (!newQuest->fread(ifs, *this->goblinAnim[0].begin(), *this->golemAnim[0].begin())) return false;
        this->questList.push_back(newQuest);
    }
    /*this->questList.clear();
    std::list<Enemy *> enl1;
    Goblin *gl = new Goblin(Position(350, 400), *this->goblinAnim[0].begin());
    Goblin *gl2 = new Goblin(Position(380, 440), *this->goblinAnim[0].begin());
    Golem *gol = new Golem(Position(310, 380), *this->golemAnim[0].begin());
    enl1.push_back(gl);
    enl1.push_back(gl2);
    enl1.push_back(gol);
    Quest *q = new KillQuest("Quest1", "Kill all!!", enl1);
    questList.push_back(q);
    enl1.clear();
    gl = new Goblin(Position(740, 520), *this->goblinAnim[0].begin());
    Golem *gol2 = new Golem(Position(800, 450), *this->golemAnim[0].begin());
    gol = new Golem(Position(850, 400), *this->golemAnim[0].begin());
    enl1.push_back(gl);
    enl1.push_back(gol2);
    enl1.push_back(gol);
    q = new LootQuest("Quest2", "Kill and loot", enl1, Position(900, 490), 1);
    this->questList.push_back(q);
    enl1.clear();
    gol = new Golem(Position(220, 140), *this->golemAnim[0].begin());
    gl = new Goblin(Position(250, 100), *this->goblinAnim[0].begin());
    gl2 = new Goblin(Position(270, 200), *this->goblinAnim[0].begin());
    gol2 = new Golem(Position(150, 170), *this->golemAnim[0].begin());
    enl1.push_back(gol);
    enl1.push_back(gl2);
    enl1.push_back(gol2);
    enl1.push_back(gl);
    q = new KillQuest("Final Quest", "Kill all!!!", enl1);
    questList.push_back(q);
    std::ofstream ofs;
    //ifs.close();
    ofs.open(QuestPath);
    for(auto i = questList.begin(); i != questList.end(); i++)
        (*i)->fwrite(ofs);*/
    return true;
}
bool MainWindow::loadPlayerResources() {
    std::ifstream ifs;
    ifs.open(this->playerClass + ".txt");
    if (!ifs.good()) return false;
    for (int i = 0; i < 4; i++)
        ifs >> this->playerAnimPaths[i];
    for (int i = 0; i < 4; i++) {
        this->playerAnim[i] = LoadImages(this->ren, playerAnimPaths[i], "png");
        if (this->playerAnim[i].size() == 0) return false;
    }
    return true;
}
void MainWindow::onWin() {
    renderText(ren, this->w/2-150, this->h/2-75, 300, 150, "YOU WIN!!!", this->font, this->red);
    renderText(ren, this->w/2-150, this->h/2+50, 300, 80, "Press any key to exit", this->font, this->red);
    SDL_RenderPresent(ren);
    SDL_Event e;
    while (1) {
        if (SDL_PollEvent(&e))
            if (e.type == SDL_KEYDOWN || e.type == SDL_QUIT)
                exit(0);
    }
}
void MainWindow::onDied() {
    for (int i = 0; i < 4; i++)
        for (auto j = this->playerAnim[i].begin(); j != this->playerAnim[i].end(); j++)
            SDL_DestroyTexture((*j));
    renderText(ren, this->w/2-150, this->h/2-75, 300, 150, "YOU DIED", this->font, this->red);
    renderText(ren, this->w/2-150, this->h/2+50, 300, 80, "Press any key to restart", this->font, this->red);
    SDL_RenderPresent(ren);
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        if (SDL_PollEvent(&e)){
           if (e.type == SDL_QUIT){
               quit = true;
           } else if (e.type == SDL_KEYDOWN) {
               SDL_RenderClear(ren);
               this->startGameThread();
           }
        }
    }
}
void MainWindow::chooseClass() {
    SDL_Event e;
    renderText(ren, this->w/2-150, this->h/2-75, 300, 150, "Choose class", this->font, this->red);
    renderText(ren, this->w/2-150, this->h/2+50, 300, 40, "Press 1 - Warrior, 2- Wizard, 3 - Archer", this->font, this->red);
    SDL_RenderPresent(ren);
    SDL_RenderClear(ren);
    bool choose = false;
    while (!choose) {
        if (SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) exit(0);
            switch(e.key.keysym.sym) {
                case SDLK_1:
                    this->playerClass = "Warrior";
                    this->pl = new Warrior();
                    choose = true;
                    break;
                case SDLK_2:
                    this->playerClass = "Wizard";
                    this->pl = new Wizard();
                    choose = true;
                    break;
                case SDLK_3:
                    this->playerClass = "Archer";
                    this->pl = new Archer();
                    choose = true;
                    break;
            }
        }
    }
    if (!this->loadPlayerResources()) {
        std::cout << "Problem with resources!";
        exit(0);
    }
}
void MainWindow::startGameThread() {
    SDL_Event e;
    chooseClass();
    bool quit = false;
    int x,y;
    this->currentQuest->startQuest(this->pl);
    Uint8 staminaCounter = 0, healthCounter = 0;
    Uint8 firstAidCount = 0;
    while (!quit && pl->getHealth() > 0) {
        SDL_GetMouseState(&x, &y);
        if (SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch(e.button.button) {
                        case SDL_BUTTON_LEFT:
                            for (auto i = this->currentQuest->getList().begin(); i != this->currentQuest->getList().end(); i++) {
                                if (pl->getPos()->calcDistance(*(*i)->getPos()) < 50.0f && (*i)->getPos()->isCollision(Position(e.button.x, e.button.y), 32, 48, (*i)->getRoute()) && !(*i)->getDeath()) {
                                    pl->attack(*i);
                                    break;
                                }
                            }
                            if (!pl->isAttack())
                                pl->goToPos(Position(e.button.x, e.button.y));
                        break;
                        case SDL_BUTTON_RIGHT:
                            for (auto i = this->currentQuest->getList().begin(); i != this->currentQuest->getList().end(); i++) {
                                if ((*i)->getPos()->isCollision(Position(e.button.x, e.button.y), 32, 48, (*i)->getRoute()) && pl->getPos()->calcDistance(*(*i)->getPos()) < pl->getDistance() && !(*i)->getDeath()) {
                                    pl->SpecialAttack((*i));
                                    break;
                                }
                            }
                        break;
                    }
            }
        }
        renderTexture(ren, map, 0, 0, SDL_FLIP_NONE);
        if (this->currentQuest->getType() != -1)
            renderTexture(ren, this->chests[this->currentQuest->getType()], this->currentQuest->getPos().getX(), this->currentQuest->getPos().getY(), 32, 32, SDL_FLIP_NONE);
        bool isAt = false;
        if (!isAt) SDL_SetCursor(this->defCursor);
        for (auto i = this->objectList.begin(); i != this->objectList.end(); i++) {
            renderTexture(this->ren, this->objects[(*i)->getType()], (*i)->getPos().getX(), (*i)->getPos().getY(), (*i)->getW(), (*i)->getH(), SDL_FLIP_NONE);
            if ((*i)->getPos().isCollision(*this->pl->getPos(), (*i)->getW(), (*i)->getH()) && !(*i)->isColided()) {
                (*i)->setCol(true);
                (*i)->ifCollision(this->pl);
                if ((*i)->getType() == 0) {
                     this->objectList.erase(i);
                     firstAidCount--;
                 }
            } else if (!(*i)->getPos().isCollision(*this->pl->getPos(), (*i)->getW(), (*i)->getH()) && (*i)->isColided())
                (*i)->setCol(false);
        }
        for (auto i = this->currentQuest->getList().begin(); i != this->currentQuest->getList().end(); i++) {
            this->drawEnemy(**i);
            if ((*i)->getDeath() && !(*i)->isAnim()) {
                this->currentQuest->getList().erase(i);
                delete *i;
                continue;
            }
            if ((*i)->getPos()->isCollision(Position(x,y), 32, 48, (*i)->getRoute()) && (*i)->getPos()->calcDistance(*pl->getPos()) < pl->getDistance()) {
                isAt = true;
                SDL_SetCursor(this->permAttackCursor);
            }
            else if ((*i)->getPos()->isCollision(Position(x,y), 32, 48, (*i)->getRoute())) {
                isAt = true;
                SDL_SetCursor(this->unpermAttackCursor);
            }
        }
        this->drawPlayer();
        if (this->currentQuest->isCompleted() && !this->mainToast->isShow()) {
            this->mainToast->show();
            this->questList.remove(this->currentQuest);
            if (questList.size() == 0)
                this->onWin();
            else {
                this->currentQuest = *questList.begin();
                this->currentQuest->startQuest(pl);
            }
        }
        renderTexture(ren, this->blending, this->pl->getPos()->getX()-1500+32, this->pl->getPos()->getY()-1000+32, SDL_FLIP_NONE);
        if (this->mainToast->isShow()) drawToast(*mainToast);
        drawPlayerInfo();
        SDL_RenderPresent(ren);
        SDL_RenderClear(ren);
        if (this->pl->getStamina() < 80 && staminaCounter >= 100)
            this->pl->setStamina(this->pl->getStamina() + 1);
        if (this->pl->getHealth() < 80 && healthCounter >= 100)
            this->pl->setHealth(this->pl->getHealth() + 1);
        for (auto i = this->currentQuest->getList().begin(); i != this->currentQuest->getList().end(); i++)
            if((*i)->getStamina() < 80 && staminaCounter >= 100)
                (*i)->setStamina((*i)->getStamina() + 1);
        if (firstAidCount < 3) {
            GameObject *newFirstAid = new FirstAid(Position(rand() % 1280, rand() % 720), 48, 48);
            this->objectList.push_back(newFirstAid);
            firstAidCount++;
        }
        if (staminaCounter >= 100)
            staminaCounter = 0;
        if (healthCounter >= 100)
            healthCounter = 0;
        staminaCounter += 3;
        healthCounter += 1;
    }
    SDL_SetCursor(this->defCursor);
    if (pl->getHealth() <= 0 && !quit)
        this->onDied();
    this->isQuit = true;
}
