#include <raylib.h>
#include <stdlib.h>
#include "collisionbox.h"
#include "weapon.h"
#include "lasergun.h"

void LaserGunAttack(Weapon *self);
void LaserGunUpdate(Weapon *self, double deltaTime);
void LaserGunDestroy(Weapon *self);

LaserGun* LaserGunInit() {
  LaserGun* lasergun = (LaserGun*)malloc(sizeof(LaserGun));
  char weaponName[51] = "Laser gun";

  Weapon* base = WeaponInition(
    weaponName,                                      //武器名稱
    LoadTexture("resources/weapon/laser.png"),       //武器貼圖
    LoadSound("resources/sound/laser.mp3"),          //武器攻擊音效
    100.f,                                           //攻擊力
    0.5f,                                            //攻擊範圍
    20.0f,                                           //攻擊速度
    5.0f,                                            //冷卻時間
    (Vector2){0,0}                                   //初始位置
  );


  base -> attack = LaserGunAttack;
  base -> update = LaserGunUpdate;
  base -> destroy = LaserGunDestroy;
  base -> checkDemage = CheckLaserGunDemage;

  lasergun -> base = *base;
  free(base);

  lasergun -> box = (CollisionBox) {
    (Rectangle){lasergun->base.position.x, lasergun->base.position.y, lasergun->base.texture.width, lasergun->base.texture.height},
    0.0f
  }; 

  lasergun -> maxAttackTime = 5.0f;
  lasergun -> curAttackTime = 0.0f;

  return lasergun;
}

void LaserGunAttack(Weapon *self) {
  LaserGun* laser = (LaserGun*)self;

  //將雷射的朝向調整至角色面對的方向
  CollsionBoxRotation(&laser->box, laser->base.direction);


  //繪製攻擊特效
  DrawTextureEx(
    laser->base.texture,                     //貼圖
    laser->base.position,                    //位置
    laser->box.rotationAngle,                //旋轉角度
    laser->base.attackRange,            //攻擊範圍
    WHITE                                    //顏色
  );


}

void LaserGunUpdate(Weapon *self, double deltaTime) {
  LaserGun* laser = (LaserGun*)self;

  //調整碰撞箱的大小和位置
  laser->box.rec.width = laser->base.texture.width*laser->base.attackRange;
  laser->box.rec.height = laser->base.texture.height*laser->base.attackRange;

  laser->box.rec.x = laser->base.position.x;
  laser->box.rec.y = laser->base.position.y;

  //武器冷卻時間計算
  if(laser->base.curCooldownTime > 0.0f) {
    laser->base.curCooldownTime -= deltaTime;

    if(laser->base.curCooldownTime < 0.0f) {
      laser->base.curCooldownTime = 0.0f;
    }
  }

  //攻擊中時,計算攻擊持續時間,並進行攻擊
  if(laser->base.isAttack) {
    laser->curAttackTime += deltaTime;
    laser->base.attack(&laser->base);

    //超過攻擊持續時間，停止攻擊,並重置冷卻狀態
    if(laser->curAttackTime > laser->maxAttackTime) {
      laser->base.isAttack = false;
      laser->curAttackTime = 0.0f;
      laser->base.curCooldownTime = laser->base.maxCoolTime;
    }
  }

  //冷卻結束，繼續攻擊
  if(laser->base.curCooldownTime == 0.0f && !laser->base.isAttack) {
    laser->base.isAttack = true;
    PlaySound(laser->base.sound);
  }
  
}

void LaserGunDestroy(Weapon *self) {
  UnloadTexture(self->texture);

  free(self);
}

double CheckLaserGunDemage(Weapon *self,CollisionBox* box) {
  LaserGun* laser = (LaserGun*) self;
  double damage = 0.0f;

  if(laser->base.isAttack && CheckCollsionBoxes(*box, laser->box)) {
    damage += laser->base.attackPower;
  }

  return damage;
}
