#ifndef FAST_MONSTER_H
#define FAST_MONSTER_H

#include "raylib.h" 
#include "hero.h"   // 假設你有 hero 的定義
//#include "map.h"    // 假設你有 map 的定義
#include "monster.h"
#include "collisionbox.h"

// 定義怪物最大數量、初始數量、生成速率和生成間隔
#define MAX_FAST_MONSTER_COUNT 200
#define INITIAL_FAST_MONSTER_COUNT 10
#define FAST_MONSTER_SPAWN_RATE 1
#define FAST_MONSTER_SPAWN_INTERVAL 3.0f // 每 3 秒生成一次

// 定義地圖尺寸和單元格大小 (假設這些在其他地方有定義)
// #define MAP_WIDTH 50
// #define MAP_HEIGHT 50
// #define CELL_SIZE 10

// 怪物結構定義 
typedef struct Fast_Monster {
    int x, y;           // 怪物在地圖上的位置 (網格座標)
    int is_active;      // 怪物是否活躍 (1: 活躍, 0: 非活躍)
    Color color;        // 怪物顏色 (固定為藍色)
    int health;         // 怪物生命值
    int width;          // 怪物寬度 (網格單元)
    int height;         // 怪物高度 (網格單元)
    CollisionBox box;    // 怪物碰撞箱

    int move_chance;    // 移動機率 (0-100, 值越高越常移動)
    int speed_steps;    // 每次移動的步數
} Fast_Monster;

// 外部變數宣告
extern Fast_Monster fast_monsters[MAX_FAST_MONSTER_COUNT];
extern int current_fast_monster_count; // 當前怪物總數 (包括非活躍的)
extern int active_fast_monster_count;  // 當前活躍怪物數量
extern int dead_fast_monster_count;    // 已死亡怪物數量

extern float game_fast_monster_time;           // 遊戲時間
extern float next_fast_spawn_time;     // 下次怪物生成時間

// 外部函數宣告 (假設這些函數在其他地方有定義)
//extern void random_empty_position(Node *pos, Hero *hero); // 獲取隨機空閒位置
//extern bool CheckCollsionBoxes(BoundingBox box1, BoundingBox box2); // 檢查碰撞
//extern void create_explosion(int x, int y, Color c); // 創建爆炸效果

// 怪物相關函數宣告
void spawn_fast_monsters(Hero *hero,int map_type);
void move_fast_monsters_towards_player(Hero *hero);
void add_fast_monsters(Hero *hero,int map_type);
void replace_missing_fast_monsters(Hero *hero,int map_type);
void draw_fast_monsters();
void check_fast_monster_collision(Hero *hero);
void get_fast_monster_demage(Hero* hero);

#endif // FAST_MONSTER_H
