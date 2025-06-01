#ifndef CHAINMONSTER_H
#define CHAINMONSTER_H

#include<raylib.h>
#include"hero.h"
#include"monster.h"
#include"basic_monster.h"

// 鎖鏈鎖鏈型怪物相關變數

//最大射擊怪物數量
#define MAX_CHAIN_MONSTERS 10  

// 同時存在的最大鎖鏈爆炸數量
#define MAX_CHAIN_EXPLOSIONS 10///////////

// 每10隻小怪死亡後生成1隻射擊怪物
#define DEAD_MONSTERS_TO_SPAWN_CHAIN 10  

// 最大鎖鏈數量
#define MAX_CHAIN_NUM 1000  

// 射程範圍
#define CHAIN_RANGE 100 

// 射擊機率(數值越小越容易射擊，範圍1-100)
#define CHAIN_FIRE_CHANCE 80  

typedef struct {
    // Texture2D texture;
    CollisionBox box;
    int x, y;
    int is_active;
    int health; 
    int width,height;
    Color color;  // 鎖鏈怪物顏色
    int cooldown; // 射擊冷卻時間
} ChainMonster;

typedef struct {
    CollisionBox box;
    Vector2 position;
    int x, y;           // 位置
    int direction;      // 方向 (0=上, 1=下, 2=左, 3=右)
    int range_left;     // 剩餘射程
    bool is_active;     // 是否活躍
    Color color;        // 顏色
} Chain;

typedef struct {
    CollisionBox box;
    int x, y;           // 爆炸中心點
    int frame;          // 當前爆炸幀
    bool is_active;     // 爆炸是否活躍
    Color color;        // 爆炸顏色
    int radius;         // 爆炸半徑
} Explosion_Chain;



// 初始化爆炸效果
void init_chain_monsters();
void init_chain();

void create_chain(int x, int y, int direction, Color color);//生成鎖鏈怪物的射擊物
void spawn_chain_monster(Hero *hero);//生成鎖鏈怪物
void update_chain_monsters(Hero *hero);// 移動鎖鏈型怪物和處理射擊
void create_explosion_with_radius(int x, int y, Color color, int radius);//創建指定半徑的爆炸(鎖鏈型怪物)
bool check_player_in_explosion_radius(int explosion_x, int explosion_y, int radius,Hero *hero);// 檢查玩家是否在爆炸範圍內
void update_chain(Hero *hero);//鎖鏈的更新

void draw_chain_monsters();//繪製鎖鏈怪物
void draw_chain();//繪製鎖鏈
void check_chain_monster_collision(Hero* hero);
void get_chain_monster_demage(Hero* hero);

#endif
