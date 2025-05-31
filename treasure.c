#include "treasure.h"
#include "hero.h"
#include "weapon.h"
#include "selectioninterface.h"
#include <stdlib.h>
#include <time.h>
#include "basic_monster.h"

#define MAX_TREASURES 5

static Treasure treasures[MAX_TREASURES];
static int treasure_count = 0;

static int needed_dead_monster = 30;

// 初始化寶箱系統
void Treasure_InitSystem(void) {
    srand((unsigned int)time(NULL));
    treasure_count = 0;
}

// 隨機產生寶箱位置
static Vector2 Treasure_GetRandomPositionNearPlayer(const Hero* hero) {
    // 設定寶箱生成距離範圍 100~200 像素
    const float min_dist = 300.0f;
    const float max_dist = 400.0f;

    // 隨機角度
    float angle = ((float)rand() / RAND_MAX) * 2.0f * PI;
    // 隨機距離
    float dist = min_dist + ((float)rand() / RAND_MAX) * (max_dist - min_dist);

    // 偏移向量
    Vector2 offset = {
        cosf(angle) * dist,
        sinf(angle) * dist
    };

    // 寶箱位置 = 玩家位置 + 偏移
    Vector2 pos = {
        hero->position.x + offset.x,
        hero->position.y + offset.y
    };

    return pos;
}

Treasure Treasure_Init(Vector2 position, Texture2D closedTex, Texture2D openedTex) {
    Treasure treasure;
    treasure.position = position;
    treasure.is_opened = false;
    treasure.closedTexture = closedTex;
    treasure.openedTexture = openedTex;
    Rectangle rec = {position.x, position.y, closedTex.width, closedTex.height};
    treasure.hitbox.rec = rec;
    treasure.hitbox.rotationAngle = 0;
    return treasure;
}


// 怪物被擊殺時呼叫，達30隻生成寶箱
void Treasure_OnMonsterKilled(Hero* hero) {
    if (dead_monster_count >= needed_dead_monster && treasure_count < MAX_TREASURES) {
        Vector2 pos = Treasure_GetRandomPositionNearPlayer(hero);
        Texture2D closedTex = LoadTexture("resources/treasure/closedtreasure.png");
        Texture2D openedTex = LoadTexture("resources/treasure/openedtreasure.png");
        treasures[treasure_count++] = Treasure_Init(pos, closedTex, openedTex);
        needed_dead_monster += 30;
    }
}


// 判斷玩家與寶箱碰撞
static bool Treasure_CheckCollision(const Treasure* treasure, Vector2 player_pos, double pickup_radius) {
    // 使用 collisionbox.h 的函式
    return CheckCollisionBoxAndCircle(treasure->hitbox, player_pos, pickup_radius);
}

// 更新所有寶箱狀態，處理玩家碰撞與武器選擇
int Treasure_Update(Hero* hero) {
    int weaponCode = -1;
    for (int i = 0; i < treasure_count; i++) {
        if (!treasures[i].is_opened && Treasure_CheckCollision(&treasures[i], hero->position, hero->pickupRadius)) {
            treasures[i].is_opened = true;

            // 呼叫武器選擇介面，等待玩家選擇武器
            weaponCode = WeaponSelectionInterfece();
            
            
        }
    }

    return weaponCode;
}

//繪製寶箱
void Treasure_Draw(const Treasure* treasure) {
    if (treasure->is_opened) {
        DrawTexture(treasure->openedTexture, treasure->position.x, treasure->position.y, WHITE);
    } else {
        DrawTexture(treasure->closedTexture, treasure->position.x, treasure->position.y, WHITE);
    }
}

// 繪製所有寶箱
void Treasure_DrawAll(void) {
    for (int i = 0; i < treasure_count; i++) {
        Treasure_Draw(&treasures[i]);
    }
}

//釋放寶箱
void Treasure_Destroy(Treasure* treasure) {
    UnloadTexture(treasure->closedTexture);
    UnloadTexture(treasure->openedTexture);
}

// 釋放所有寶箱
void Treasure_DestroyAll(void) {
    for (int i = 0; i < treasure_count; i++) {
        Treasure_Destroy(&treasures[i]);
    }
    treasure_count = 0;
}


