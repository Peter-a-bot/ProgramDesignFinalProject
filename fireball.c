#include <raylib.h>
#include <stdlib.h>
#include "weapon.h"
#include "collisionbox.h"
#include "fireball.h"

FireBall* FireBallInit() {
  
  FireBall* fireball = (FireBall*)malloc(sizeof(FireBall));

  char weaponName[51] = "fireball";

  Weapon* base = WeaponInition(
    weaponName,                                      //武器名稱
    LoadTexture("resources/weapon/fireball.png"),       //武器貼圖
    40.f,                                           //攻擊力
    1.0f,                                            //攻擊範圍
    5.0f,                                           //攻擊速度
    5.0f,                                            //冷卻時間
    (Vector2){0,0}                                   //初始位置
  );

  base->attack = FireBallAttack;
  base->update = FireBallUpdate;
  base->destroy = FireBallDestroy;
  base->checkDemage = CheckFireBallDemage;

  fireball->base = *base;
  free(base);

  fireball->projectileCount = 1;
  
  fireball->radius = 20.0f;

  fireball->maxAttackTime = 7.0f;
  fireball->curAttackTime = 0.0f;

  for(int i=0; i< 6; i++) {
    fireball->boxes[i] = (CollisionBox) {
      .rec = (Rectangle) {
      .x = fireball->base.position.x,
      .y = fireball->base.position.y,
      .height = fireball->base.texture.height,
      .width = fireball->base.texture.width
    },
      .rotationAngle= 60.0f * i
    };
  }

  return fireball;
}                                 

void FireBallAttack(Weapon* self) {
  FireBall* fireball = (FireBall*)self;

  for(int i=0; i<6; i++) {
    DrawTextureEx(fireball->base.texture, fireball->base.position, fireball->boxes[i].rotationAngle, fireball->base.attackRange, WHITE);
    fireball->boxes[i].rotationAngle += fireball->base.attackSpeed;

    if(fireball->boxes[i].rotationAngle > 360.0f) {
      fireball->boxes[i].rotationAngle -= 360.0f;
    }
  }
}                       

void FireBallUpdate(Weapon *self, double deltaTime) {
  
  FireBall* fireball = (FireBall*)self;
  
  //調整碰撞箱的大小和位置
  for(int i=0; i<6; i++) {
    
    fireball->boxes[i].rec.width = fireball->base.texture.width*fireball->base.attackRange;
    fireball->boxes[i].rec.height = fireball->base.texture.height*fireball->base.attackRange;

    fireball->boxes[i].rec.x = fireball->base.position.x;
    fireball->boxes[i].rec.y = fireball->base.position.y;
  }

  //武器冷卻時間計算
  if(fireball->base.curCooldownTime > 0.0f) {
    fireball->base.curCooldownTime -= deltaTime;

    if(fireball->base.curCooldownTime < 0.0f) {
      fireball->base.curCooldownTime = 0.0f;
    }
  }

  //攻擊中時,計算攻擊持續時間,並進行攻擊
  if(fireball->base.isAttack) {
    fireball->curAttackTime += deltaTime;
    fireball->base.attack(&fireball->base);

    //超過攻擊持續時間，停止攻擊,並重置冷卻狀態
    if(fireball->curAttackTime > fireball->maxAttackTime) {
      fireball->base.isAttack = false;
      fireball->curAttackTime = 0.0f;
      fireball->base.curCooldownTime = fireball->base.maxCoolTime;
    }
  }

  //冷卻結束，繼續攻擊
  if(fireball->base.curCooldownTime == 0.0f && !fireball->base.isAttack) {
    fireball->base.isAttack = true;
  }
}

void FireBallDestroy(Weapon *self) {
  FireBall* fireball = (FireBall*)self;
  free(fireball);  
}

double CheckFireBallDemage(Weapon* self, CollisionBox* box) {
  FireBall* fireball = (FireBall*)self;

  double demage = 0.0f;

  for(int i=0; i<6; i++) {
    if(fireball->base.isAttack && CheckCollsionBoxes(fireball->boxes[i], *box)) {
      demage += fireball->base.attackPower;
    }
  }

  return demage;
}
