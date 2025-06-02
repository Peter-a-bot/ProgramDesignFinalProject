#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hero.h"
#include "collisionbox.h"
#include "weapon.h"
#include "selectioninterface.h"
Hero* HeroInit(char name[51], Vector2 position, Texture2D texture, double attackPower, double speed, double maxHp, double hpRecoveryRate, double pickupRadius) {
  Hero* hero = (Hero*)malloc(sizeof(Hero));

  strcpy(hero->name, name);
  hero->level = 1; 
  hero->exp = 0;
  hero->nextLevelExp = 100;  // 需要100經驗值升級

  hero->position = position;
  hero->direction = (Vector2){0,0};
  hero->texture = texture;

  hero->weaponCount = 0;

  hero->attackPower = attackPower;
  hero->speed = speed;
  hero->maxHp = maxHp;
  hero->hp = maxHp;
  hero->pickupRadius = pickupRadius; 
  hero->hpRecoveryRate = hpRecoveryRate;
  hero->level = 1;
  hero->exp = 0;
  hero->nextLevelExp = 50;

  hero->move = HeroMove;
  hero->draw = HeroDraw;
  hero->update = HeroUpdate;
  hero->destroy = HeroDestroy;

  return hero;
}  

void HeroMove(Hero* self) {
  Vector2 nextDirection = (Vector2){0, 0};

  if(IsKeyDown(KEY_UP)) {
    self->position.y -= self->speed;
    nextDirection.y = 1;
  }
  if(IsKeyDown(KEY_DOWN)) {
    self->position.y += self->speed;
    nextDirection.y = -1;
  }
  if(IsKeyDown(KEY_RIGHT)) {
    self->position.x += self->speed;
    nextDirection.x = 1;
  }
  if(IsKeyDown(KEY_LEFT)) {
    self->position.x -= self->speed;
    nextDirection.x = -1;
  }

  if(nextDirection.x ==0 && nextDirection.y ==0) {
    return;
  }

  self->direction = nextDirection;
}   

void HeroDraw(Hero* self) {
  //角色面朝右方，正常繪製貼圖
  if(self->direction.x == 1) {
    DrawTexture(self->texture, self->position.x, self->position.y, WHITE);
  }
  //角色面朝左方，將貼圖左右反轉後再繪製
  else {
    Rectangle source = {0, 0, -self->texture.width, self->texture.height};
    DrawTextureRec(self->texture, source,self->position, WHITE);
  }
}

void HeroUpdate(Hero *self, double deltaTime) {
  // 【1】回血
    self->hp += self->hpRecoveryRate * deltaTime;
    if (self->hp > self->maxHp) self->hp = self->maxHp; // 假設最大血量100

    // 【2】經驗值升級檢查
    if (self->exp >= self->nextLevelExp) {
        self->exp -= self->nextLevelExp;
        self->level++;
        self->nextLevelExp = (int)(self->nextLevelExp * 4.5); // 升級需求提升

        // 呼叫被動選單
        int selectedPassive = PassiveSelectionInterfece();

        // 根據選擇增加能力
        if (selectedPassive == 1) {
            self->hpRecoveryRate += 1.0f; // 生命回復速度增加
        }
        else if (selectedPassive == 2) {
          self->maxHp *= 1.2f;            //最大生命值加20%
          self->hp *= 1.2f;
        }
        else if (selectedPassive == 3) {
            for (int i = 0; i < self->weaponCount; i++) {
                self->weapons[i]->attackPower*= 1.2f; // 攻擊力 +20%
            }
        }
    }
  //調整碰撞箱的大小和位置
  self->box.rec.width = self->texture.width;
  self->box.rec.height = self->texture.height;

  self->box.rec.x = self->position.x;
  self->box.rec.y = self->position.y;

  //更新各個武器的狀態
  for(int i=0; i<self->weaponCount; i++) {

    //調整武器位置
    self->weapons[i]->position.x = self->position.x;
    self->weapons[i]->position.y = self->position.y;
    //調整武器方向
    self->weapons[i]->direction = self->direction;

    //調整各個武器的狀態
    self->weapons[i]->update(self->weapons[i], deltaTime);     
  }
}

double CheckDemage(CollisionBox* box, Hero *self) {
  double damage = 0.0f;

  for(int i=0; i<self->weaponCount; i++) {
    damage += self->weapons[i]->checkDemage(self->weapons[i], box);
  }

  damage += self->attackPower/2;

  return damage;
}

void HeroDestroy(Hero *self) {
  free(self);
}
