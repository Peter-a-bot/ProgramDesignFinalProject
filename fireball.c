#include <raylib.h>
#include "weapon.h"
#include "collisionbox.h"
#include "fireball.h"

FireBall* FireBallInit() {
  
}                                 

void FireBallAttack(Weapon* self) {
  
}                       
void FireBallUpdate(Weapon* self, double deltaTime);       //火球的狀態更新函數
void FireBallDestroy(Weapon* self);                       //火球的銷毀函數
double CheckFireBallDemage(Weapon* self, CollisionBox box); //火球的傷害碰撞函數
                      
