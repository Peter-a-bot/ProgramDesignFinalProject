#ifndef BASICMONSTER_H
#define BASICMONSTER_H

#include"monster.h"
#include <raylib.h>
#include <stdlib.h>
#include "monster.h"
#include "collisionbox.h"
#include "hero.h"

// 初始怪物數量//////////////
#define INITIAL_MONSTER_COUNT 15  

 // 最大怪物數量為150///////////
#define MAX_MONSTER_COUNT 150 

// 每次新增的怪物數量/////////////
#define MONSTER_SPAWN_RATE 5  

// 添加遊戲時間相關變量（在全局變量部分）
// 每隔3秒生成一批怪物///////////////// 
 
#define MONSTER_SPAWN_INTERVAL 5.0f  
extern float game_time;              // 遊戲進行時間（秒）///////////////////
extern float next_spawn_time;        // 下一次生成怪物的時間點//////////////

extern int dead_monster_count;     // 死亡小怪數量計數器(每死十隻普通怪物就加一隻射擊怪物)

typedef struct {
    // Texture2D texture;
    CollisionBox box;
    int x, y;
    int width, height;
    int is_active;
    int health; 
    Color color;  // 射擊怪物顏色
} Monster;

extern Monster monsters[MAX_MONSTER_COUNT];//怪物的陣列
extern int current_monster_count;  // 初始化為0，將在spawn_monsters函數中設置
extern int active_monster_count;   // 當前活躍的怪物數量


void spawn_monsters(Hero* hero,int map_type) ;// 初始化怪物位置
void move_monsters_towards_player(Hero *hero);// 讓怪物追蹤玩家並從四周包圍
void add_monsters(Hero *hero,int map_type);//新增新的怪物
void replace_missing_monsters(Hero *hero,int map_type);//檢查並替換消失的怪物
void check_collision(Hero *hero);//檢查怪物和玩家的碰撞
void draw_monsters();
void get_demage(Hero* hero);

#endif
