#include <raylib.h>
#include <stdlib.h>
#include <string.h>

#include "weapon.h"


Weapon* WeaponInition(char name[51], Texture2D texture,Sound sound, double attackPower, double attackRange, double attackSpeed, double maxCoolTime, Vector2 position) {
  Weapon* weapon = (Weapon*)malloc(sizeof(Weapon));

  strcpy(weapon->name, name);
  weapon->texture = texture;
  weapon->sound = sound;

  weapon -> attackPower = attackPower;
  weapon -> attackRange = attackRange;
  weapon -> attackSpeed = attackSpeed;
  weapon -> maxCoolTime = maxCoolTime;
  weapon -> curCooldownTime = maxCoolTime;
  weapon -> level = 1;

  weapon -> position = position;
  weapon -> direction = (Vector2){-1,0};

  weapon -> isAttack = false;  

  weapon -> attack = NULL;
  weapon -> update = NULL;
  weapon -> destroy = NULL;

  weapon -> checkDemage = NULL;

  return weapon;
} 
