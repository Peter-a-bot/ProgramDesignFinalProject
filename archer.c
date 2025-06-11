#include <raylib.h>
#include <stdlib.h>
#include "hero.h"
#include "archer.h"
#include "lasergun.h"

Archer* ArcherInit() {
  char heroName[51] = "archer";
  Hero* base = HeroInit(
    heroName,                                            //角色姓名        
    (Vector2){0,0},                                      //角色初始位置
    LoadTexture("resources/hero/archer.png"),            //角色貼圖
    30.0f,                                              //角色攻擊力
    3.0f,                                                //角色速度
    150.0f,                                              //角色血量
    0.1f,                                                //角色血量回復速度
    40.0f                                                //拾取範圍
    );

  Archer* archer = (Archer*)malloc(sizeof(Archer));
  archer->base = *base;
  free(base);

  archer->base.weapons[0] = &LaserGunInit()->base;
  archer->base.weaponCount = 1;

  return archer;
}

