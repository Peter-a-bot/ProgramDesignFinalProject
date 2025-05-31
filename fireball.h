#ifndef FIREBALL_H
#define FIREBALL_H

#include <raylib.h>
#include "collisionbox.h"
#include "weapon.h"

/*--------------------------------------
調用FireBallAttack時，會依據角色位置,在角色中心半徑為radius的圓上持續繞圈，並攻擊碰到的敵人。
火球數量會受到projectileCount的影響。
每個火球都會在boxes中生成對應的CollsitionBox。
--------------------------------------*/

typedef struct FireBall{
  Weapon base;
  unsigned int projectileCount;
  double radius;
  CollisionBox boxes[6];
}FireBall;

FireBall* FireBallInit();                                  //火球的初始化函數
void FireBallAttack(Weapon* self);                        //火球的攻擊函數
void FireBallUpdate(Weapon* self, double deltaTime);       //火球的狀態更新函數
void FireBallDestroy(Weapon* self);                       //火球的銷毀函數
double CheckFireBallDemage(Weapon* self, struct CollisionBox box); //火球的傷害碰撞函數

#endif
