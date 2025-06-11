#include <raylib.h>
#include <stdlib.h>
#include "shockstaff.h"
#include "weapon.h"
#include "katana.h"

Katana* KatanaInit() {
  Katana* katana = (Katana*)malloc(sizeof(Katana));

  char weaponName[51] = "Katana";

  Weapon* base = WeaponInition(
    weaponName,                                      //武器名稱
    LoadTexture("resources/weapon/katana.png"),       //武器貼圖
    LoadSound("resources/sound/katana.mp3"),        //武器攻擊音效
    40.f,                                           //攻擊力
    1.0f,                                            //攻擊範圍
    5.0f,                                           //攻擊速度
    5.0f,                                            //冷卻時間
    (Vector2){0,0}                                   //初始位置
  );

  katana->base = *base;

  free(base);

  katana->base.attack = KatanaAttack;
  katana->base.update = KatanaUpdate;
  katana->base.destroy = KatanaDestroy;
  katana->base.checkDemage = CheckKatanaDemage;
  
  katana -> box = (CollisionBox) {
    (Rectangle){katana->base.position.x, katana->base.position.y, katana->base.texture.width, katana->base.texture.height},
    0.0f
  }; 

  katana->curAttackTime = 0.0f;
  katana->maxAttackTime = 4.0f;
  katana->attackAngle = 0.0f;
  
  return katana;
}

void KatanaAttack(Weapon *self) {
  Katana* katana = (Katana*)self;

  //調整武士刀的攻擊角度
  katana->attackAngle += katana->base.attackSpeed;

  if (katana->attackAngle >=360.0f) {
    katana->attackAngle -= 360.0f;
  }

  //將碰撞箱的旋轉角度調整至和武士刀一致
  katana->box.rotationAngle = katana->attackAngle;
  
  //繪製攻擊特效
  DrawTextureEx(katana->base.texture, katana->base.position, katana->attackAngle, katana->base.attackRange, WHITE);
}

void KatanaUpdate(Weapon *self, double deltaTime) {
  Katana* katana = (Katana*)self;
  
  //調整碰撞箱的大小和位置
  katana->box.rec.width = katana->base.texture.width*katana->base.attackRange;
  katana->box.rec.height = katana->base.texture.height*katana->base.attackRange;

  katana->box.rec.x = katana->base.position.x;
  katana->box.rec.y = katana->base.position.y;

  //武器冷卻時間計算
  if(katana->base.curCooldownTime > 0.0f) {
    katana->base.curCooldownTime -= deltaTime;

    if(katana->base.curCooldownTime < 0.0f) {
      katana->base.curCooldownTime = 0.0f;
    }
  }

  //攻擊中時,計算攻擊持續時間,並進行攻擊
  if(katana->base.isAttack) {
    katana->curAttackTime += deltaTime;
    katana->base.attack(&katana->base);

    //超過攻擊持續時間，停止攻擊,並重置冷卻狀態
    if(katana->curAttackTime > katana->maxAttackTime) {
      katana->base.isAttack = false;
      katana->curAttackTime = 0.0f;
      katana->base.curCooldownTime = katana->base.maxCoolTime;
    }
  }

  //冷卻結束，繼續攻擊
  if(katana->base.curCooldownTime == 0.0f && !katana->base.isAttack) {
    katana->base.isAttack = true;
    PlaySound(katana->base.sound);
  }
}

void KatanaDestroy(Weapon *self) {
  Katana* katana = (Katana*)self;

  UnloadTexture(katana->base.texture);
  
  free(katana);
}

double CheckKatanaDemage(Weapon *self, struct CollisionBox *box) {
  Katana* katana = (Katana*) self;
  double damage = 0.0f;

  if(katana->base.isAttack && CheckCollsionBoxes(*box, katana->box)) {
    damage += katana->base.attackPower;
  }

  return damage;
}
