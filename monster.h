#ifndef MONSTER_H
#define MONSTER_H

#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "collisionbox.h"
#include "hero.h"

// 怪物移動間隔設定
 
// 越大怪物移動越慢 (1-100) 
#define MONSTER_MOVE_CHANCE 50  

#define CELL_SIZE 20

// 爆炸效果設定

// 同時存在的最大爆炸數量
#define MAX_EXPLOSIONS 1000  

// 爆炸動畫的總幀數
#define EXPLOSION_FRAMES 15  

// 爆炸的半徑
#define EXPLOSION_RADIUS 8  
// 爆炸結構
typedef struct {
    CollisionBox box;
    int x, y;           // 爆炸中心點
    int frame;          // 當前爆炸幀
    bool is_active;     // 爆炸是否活躍
    Color color;        // 爆炸顏色
    int radius;         // 爆炸半徑
} Explosion;
extern Explosion explosions[MAX_EXPLOSIONS];  // 爆炸數組


// 方向移動
extern int dx[4]; // 上下左右
extern int dy[4];

typedef struct {
    int x, y;
} Node;//怪物座標

Vector2 random_empty_position(Hero *hero);// 隨機生成怪物位置，考慮2x2大小

//爆炸相關
void init_explosions();//初始化爆炸
void update_explosion(); //更新爆炸
void create_explosion(int x, int y, Color color);//創建爆炸
void draw_explosions();//繪製爆炸



#endif








