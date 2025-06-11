#ifndef WEAPON_H
#define WEAPON_H

#include <raylib.h>
#include "collisionbox.h"

typedef struct Weapon{
  //武器基本屬性
  char name[51];                 //武器名稱 
  Texture2D texture;             //武器貼圖
  Sound sound;                   //武器攻擊音效
  
  double attackPower;            //攻擊力
  double attackRange;            //攻擊範圍
  double attackSpeed;            //攻擊速度

  double curCooldownTime;        //紀錄武器的冷卻時間
  double maxCoolTime;            //武器默認的冷卻時間
  unsigned int level;            //武器當前等級

  Vector2 position;              //武器目前位置（通常和角色當前位置相同）
  Vector2 direction;             //武器目前朝向

  bool isAttack;                 //用以判斷武器是否正在攻擊狀態

  //武器行為
  void (*attack)(struct Weapon* self);                        //武器攻擊函數，用以生成CollsionBox,或是Circle來進行攻擊碰撞判定
  void (*update)(struct Weapon* self, double deltaTime);      //武器狀態更新函數，用以更新武器冷卻時間、繪製攻擊動畫
  void (*destroy)(struct Weapon* self);                       //武器銷毀函數，用以將武器從記憶體中清除

  double (*checkDemage)(struct Weapon* self,struct CollisionBox* box); //傷害檢查函數，確認傳進來的碰撞箱是否在武器的攻擊範圍內，並回傳受到的傷害值
 
}Weapon;

//武器初始化函數，用以初始化基本的武器資訊
Weapon* WeaponInition(char name[51],Texture2D texture,Sound sound, double attackPower, double attackRange, double attackSpeed, double maxCoolTime, Vector2 position);  

#endif
