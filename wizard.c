#include <raylib.h>
#include <stdlib.h>
#include "hero.h"
#include "wizard.h"
#include "lasergun.h"

Wizard* WizardInit() {
  char heroName[51] = "wizard";
  Hero* base = HeroInit(
    heroName,                                            //角色姓名        
    (Vector2){0,0},                                      //角色初始位置
    LoadTexture("resources/hero/wizard.png"),            //角色貼圖
    140.0f,                                              //角色攻擊力
    1.3f,                                                //角色速度
    60.0f,                                              //角色血量
    0.3f,                                                //角色血量回復速度
    80.0f                                                //拾取範圍
    );

  Wizard* wizard = (Wizard*)malloc(sizeof(Wizard));
  wizard->base = *base;
  free(base);

  wizard->base.weapons[0] = &LaserGunInit()->base;
  wizard->base.weaponCount = 1;

  return wizard;
}

