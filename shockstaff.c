#include <raylib.h>
#include <stdlib.h>
#include "collisionbox.h"
#include "weapon.h"
#include "shockstaff.h"

#define SHOCKSTAFF_OFFSET -500.0f

ShockStaff* ShockStaffInit() {
  ShockStaff* shockstaff = (ShockStaff*)malloc(sizeof(ShockStaff));
  char weaponName[51] = "ShockStaff";

  Weapon* base = WeaponInition(
    weaponName,                                            //武器名稱
    LoadTexture("resources/weapon/energyball.png"),        //武器貼圖
    LoadSound("resources/sound/shockstaff.mp3"),
    30.0f,                                                  //攻擊力
    3.0f,                                                  //攻擊範圍
    20.0f,                                                 //攻擊速度
    5.0f,                                                  //冷卻時間
    (Vector2){0,0}                                         //初始位置
  );

  shockstaff->base = *base;

  shockstaff->base.attack = ShockStaffAttack;
  shockstaff->base.update = ShockStaffUpdate;
  shockstaff->base.destroy = ShockStaffDestroy;
  shockstaff->base.checkDemage = CheckShockStaffDemage;
  free(base);

  shockstaff->circleCenter = (Vector2) {
    shockstaff->base.position.x + shockstaff->base.texture.width/2.0f + SHOCKSTAFF_OFFSET,
    shockstaff->base.position.y + shockstaff->base.texture.height/2.0f + SHOCKSTAFF_OFFSET,
  };

  shockstaff->radius = 200.0f;

  shockstaff->maxAttackTime = 5.0f;
  shockstaff->curAttackTime = 0.0f;

  return shockstaff;
}

void ShockStaffAttack(Weapon *self) {
  ShockStaff* shockstaff = (ShockStaff*)self;

  DrawTextureEx(shockstaff->base.texture, shockstaff->base.position,0.0f,shockstaff->base.attackRange, WHITE);

}

void ShockStaffUpdate(Weapon *self, double deltaTime) {
  ShockStaff* shockstaff = (ShockStaff*)self;
  int isPlaySound = 0;

  //調整貼圖的位置
  shockstaff->base.position.x += SHOCKSTAFF_OFFSET;
  shockstaff->base.position.y += SHOCKSTAFF_OFFSET;

  //調整圓心位置
  shockstaff->circleCenter = (Vector2) {
    shockstaff->base.position.x + shockstaff->base.texture.width/2.0f + 350.0f,
    shockstaff->base.position.y + shockstaff->base.texture.height/2.0f + 350.0f,
  };

  shockstaff->radius = shockstaff->base.texture.width * shockstaff->base.attackRange/2.2f;

  //武器冷卻時間計算
  if(shockstaff->base.curCooldownTime > 0.0f) {
    shockstaff->base.curCooldownTime -= deltaTime;

    if(shockstaff->base.curCooldownTime < 0.0f) {
      shockstaff->base.curCooldownTime = 0.0f;
    }
  }

  //攻擊中時,計算攻擊持續時間,並進行攻擊
  if(shockstaff->base.isAttack) {
    shockstaff->curAttackTime += deltaTime;
    shockstaff->base.attack(&shockstaff->base);

    //超過攻擊持續時間，停止攻擊,並重置冷卻狀態
    if(shockstaff->curAttackTime > shockstaff->maxAttackTime) {
      shockstaff->base.isAttack = false;
      shockstaff->curAttackTime = 0.0f;
      shockstaff->base.curCooldownTime = shockstaff->base.maxCoolTime;
    }
  }

  //冷卻結束，繼續攻擊
  if(shockstaff->base.curCooldownTime == 0.0f && !shockstaff->base.isAttack) {
    shockstaff->base.isAttack = true;
    PlaySound(shockstaff->base.sound);
  }
}

void ShockStaffDestroy(Weapon *self) {
  ShockStaff* shockstaff = (ShockStaff*)self;

  free(self);
}

double CheckShockStaffDemage(Weapon* self,struct CollisionBox* box) {
  ShockStaff* shockstaff = (ShockStaff*) self;
  double damage = 0.0f;

  if(shockstaff->base.isAttack && CheckCollisionBoxAndCircle(*box, shockstaff->circleCenter, shockstaff->radius)) {
    damage += shockstaff->base.attackPower;
  }

  return damage;
}
