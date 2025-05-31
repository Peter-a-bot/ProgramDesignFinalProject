#ifndef SELECTINTERFERCE_H
#define SELECTINTERFERCE_H

#include <raylib.h>
#include "hero.h"

int WeaponSelectionInterfece();      //叫出武器選擇界面,並回傳選擇的武器號
int PassiveSelectionInterfece();     //叫出強化選擇界面，並回傳強化編號
int DeathScreen(int score);          //叫出死亡顯示界面

int VictoryScreen(double gameTime);

int CharacterSelectionInterface();   //叫出武器選擇界面
int MapSelectionInterface();         //叫出地圖選擇界面

int MainMenu();                      //叫出主頁面
int PauseScreen(Hero* hero);

#endif
