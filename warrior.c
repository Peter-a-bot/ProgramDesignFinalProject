#include <raylib.h>
#include <stdlib.h>
#include "hero.h"
#include "warrior.h"
#include "lasergun.h"

Warrior* WarriorInit() {
  char heroName[51] = "Warrior";
  Hero* base = HeroInit(
    heroName,            
    (Vector2){0,0},
    LoadTexture("resources/hero/warrior.png"),
    100.0f,                                         //角色攻擊力
    2.0f,                                           //角色速度
    220.0f,                                         //角色血量
    0.1f,                                          //角色血量回復速度
    50.0f                                          //角色拾取範圍
    );

  Warrior* warrior = (Warrior*)malloc(sizeof(Warrior));
  warrior->base = *base;
  free(base);

  warrior->base.weapons[0] = &LaserGunInit()->base;
  warrior->base.weaponCount = 1;

  return warrior;
}

