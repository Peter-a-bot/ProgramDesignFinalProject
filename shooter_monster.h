#ifndef SHOOTERMONSTER_H
#define SHOOTERMONSTER_H

#include<raylib.h>
#include"hero.h"
#include"monster.h"
#include"basic_monster.h"

// 射擊型怪物相關變數

//最大射擊怪物數量
#define MAX_SHOOTER_MONSTERS 10  

// 每10隻小怪死亡後生成1隻射擊怪物
#define DEAD_MONSTERS_TO_SPAWN_SHOOTER 10  

// 最大射擊物數量
#define MAX_PROJECTILES 1000  

// 射程範圍
#define PROJECTILE_RANGE 100 

// 射擊機率(數值越小越容易射擊，範圍1-100)
#define SHOOTER_FIRE_CHANCE 80  

typedef struct {
    // Texture2D texture;
    CollisionBox box;
    int x, y;
    int is_active;
    int health; 
    int width,height;
    Color color;  // 射擊怪物顏色
    int cooldown; // 射擊冷卻時間
} ShooterMonster;

typedef struct {
    CollisionBox box;
    Vector2 position;
    int x, y;           // 位置
    int direction;      // 方向 (0=上, 1=下, 2=左, 3=右)
    int range_left;     // 剩餘射程
    bool is_active;     // 是否活躍
    Color color;        // 顏色
} Projectile;

// 初始化爆炸效果
void init_shooter_monsters();
void init_projectiles();

void create_projectile(int x, int y, int direction, Color color);//生成射擊怪物的射擊物
void spawn_shooter_monster(Hero *hero);//生成射擊怪物
void update_shooter_monsters(Hero *hero);// 移動射擊型怪物和處理射擊
void create_explosion_with_radius(int x, int y, Color color, int radius);//創建指定半徑的爆炸(射擊型怪物)
bool check_player_in_explosion_radius(int explosion_x, int explosion_y, int radius,Hero *hero);// 檢查玩家是否在爆炸範圍內
void update_projectiles(Hero *hero);//火焰射擊物的更新

void draw_shooter_monsters();//繪製射擊怪物
void draw_projectiles();//繪製射擊物(火焰)
void check_shooter_monster_collision(Hero* hero);
void get_shooter_monster_demage(Hero* hero);

#endif
