#ifndef SHOCKSTAFF_H
#define SHOCKSTAFF_H

#include <raylib.h>
#include "weapon.h"

/*--------------------------------------
調用ShockStaffAttack時，鎮集杖會依據角色的位置，在角色的中心處生成一個半徑為radius的沖擊波，並擊退此範圍內的敵人。
radius會根據attackRange進行調整。
--------------------------------------*/


typedef struct ShockStaff{
  Weapon base;                      //震擊杖的基本資訊
  Vector2 circleCenter;             //震擊杖的的攻擊圓心
  double radius;                    //震擊杖的攻擊半徑
  double maxAttackTime;             //震擊杖的攻擊持續時間
  double curAttackTime;             //震擊杖目前的攻擊持續時間
}ShockStaff;

ShockStaff* ShockStaffInit();                               //震擊杖的初始化函數
void ShockStaffAttack(Weapon* self);                        //震擊杖的攻擊函數
void ShockStaffUpdate(Weapon* self, double deltaTime);       //震擊杖的狀態更新函數
void ShockStaffDestroy(Weapon* self);                       //震擊杖的銷毀函數
double CheckShockStaffDemage(Weapon* self, struct CollisionBox* box); //震擊杖的傷害碰撞函數
#endif
