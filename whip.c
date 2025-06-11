#include <raylib.h>
#include <stdlib.h>
#include "collisionbox.h"
#include "weapon.h"
#include "whip.h"

Whip* WhipInit() {
  Whip* whip = (Whip*)malloc(sizeof(Whip));

  char weaponName[51] = "Whip";

  Weapon* base = WeaponInition(
    weaponName,                                      //武器名稱
    LoadTexture("resources/weapon/whip.png"),        //武器貼圖
    LoadSound("resources/sound/whip.mp3"),           //武器攻擊音效
    30.f,                                            //攻擊力
    1.0f,                                            //攻擊範圍
    20.0f,                                           //攻擊速度
    2.5f,                                            //冷卻時間
    (Vector2){0,0}                                   //初始位置
  );

  whip->base = *base;
  free(base);

  whip->base.attack = WhipAttack;
  whip->base.update = WhipUpdate;
  whip->base.destroy = WhipDestroy;
  whip->base.checkDemage = CheckWhipDemage;

  whip->maxAttackTime = 2.5f;
  whip->curAttackTime = 0.0f;

  whip->box = (CollisionBox){
    .rec = (Rectangle) {
      .x = whip->base.position.x - whip->base.texture.width,
      .y = whip->base.position.y,
      .width = whip->base.texture.width*2,
      .height = whip->base.texture.height
    },
    .rotationAngle = 0.0f
  };

  return whip;
}

void WhipAttack(Weapon *self) {
  Whip* whip = (Whip*)self;

  //繪製攻擊特效
  DrawTextureEx(whip->base.texture, whip->base.position, 0.0f, whip->base.attackRange, WHITE);

  Rectangle source = {0, 0, -whip->base.texture.width, whip->base.texture.height};
  Rectangle dest = {whip->box.rec.x, whip->box.rec.y, whip->base.texture.width * whip->base.attackRange, whip->base.texture.height * whip->base.attackRange};

  DrawTexturePro(whip->base.texture, source, dest, (Vector2){0,0}, 0.0f, WHITE);
  
}

void WhipUpdate(Weapon *self, double deltaTime) {
  Whip* whip = (Whip*)self;

  //調整碰撞箱位置
  whip->box.rec.width = whip->base.texture.width*whip->base.attackRange*2;
  whip->box.rec.height = whip->base.texture.height*whip->base.attackRange;

  whip->box.rec.x = whip->base.position.x - whip->base.texture.width;
  whip->box.rec.y = whip->base.position.y;

  
  //武器冷卻時間計算
  if(whip->base.curCooldownTime > 0.0f) {
    whip->base.curCooldownTime -= deltaTime;

    if(whip->base.curCooldownTime < 0.0f) {
      whip->base.curCooldownTime = 0.0f;
    }
  }

  //攻擊中時,計算攻擊持續時間,並進行攻擊
  if(whip->base.isAttack) {
    whip->curAttackTime += deltaTime;
    whip->base.attack(&whip->base);

    //超過攻擊持續時間，停止攻擊,並重置冷卻狀態
    if(whip->curAttackTime > whip->maxAttackTime) {
      whip->base.isAttack = false;
      whip->curAttackTime = 0.0f;
      whip->base.curCooldownTime = whip->base.maxCoolTime;
    }
  }

  //冷卻結束，繼續攻擊
  if(whip->base.curCooldownTime == 0.0f && !whip->base.isAttack) {
    whip->base.isAttack = true;
    PlaySound(whip->base.sound);
  }
}

void WhipDestroy(Weapon *self) {
  Whip* whip = (Whip*)self;

  free(whip);
}

double CheckWhipDemage(Weapon *self, struct CollisionBox* box) {
  Whip* whip = (Whip*)self;
  double damage = 0.0f;

  if(whip->base.isAttack && CheckCollsionBoxes(*box, whip->box)) {
    damage += whip->base.attackPower;
  }

  return damage;
}
