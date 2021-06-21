//
//  main.cpp
//  RPG
//
//  Created by Иван Гаврилов on 05.11.2019.
//  Copyright © 2019 Иван Гаврилов. All rights reserved.
//

#include <iostream>
#include <string.h>
#include "MainWindow.hpp"
int main(int argc, const char * argv[]) {
    MainWindow *wind = new MainWindow(1280, 720);
    wind->startGameThread();
    delete wind;
    return 0;
}
