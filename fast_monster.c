#include "fast_monster.h"
#include "hero.h"   // 假設你有 hero 的定義
#include "treasure.h"
#include <stdlib.h> // For rand() and abs()
#include <stdbool.h> // For bool type

// 假設這些變數在其他地方定義或需要在這裡初始化
// 例如:
// int MAP_WIDTH = 50;
// int MAP_HEIGHT = 50;
// int CELL_SIZE = 10;
// int map[MAP_HEIGHT][MAP_WIDTH]; // 假設地圖陣列

// 全局怪物陣列和相關變數
Fast_Monster fast_monsters[MAX_FAST_MONSTER_COUNT];
const int fast_health_up = 1000;
int current_fast_monster_count = 0;
int active_fast_monster_count = 0;
int dead_fast_monster_count = 0;
float game_fast_monster_time = 0.0f;
float next_fast_spawn_time = 0.0f;

// 輔助陣列，用於嘗試隨機方向移動 (上、下、左、右)
int fast_dx[4] = {0, 0, 1, -1}; // x 軸方向變化
int fast_dy[4] = {1, -1, 0, 0}; // y 軸方向變化

// 初始化怪物位置
void spawn_fast_monsters(Hero *hero,int map_type) {
    Node pos;
    for (int i = 0; i < INITIAL_FAST_MONSTER_COUNT; i++) {
        random_empty_position(hero);
        fast_monsters[i].x = pos.x;
        fast_monsters[i].y = pos.y;
        fast_monsters[i].is_active = 1; // 標記為活躍
        fast_monsters[i].width = 2;     // 設置怪物寬度 2
        fast_monsters[i].height = 2;    // 設置怪物高度 2

        // 固定為快速怪物的屬性
        fast_monsters[i].color = BLUE;    // 固定為藍色
        fast_monsters[i].health = 80 + map_type*fast_health_up;   //設置血量，依地圖而變 
        fast_monsters[i].move_chance = 95; // 更高的移動機率 (接近每幀都移動)
        fast_monsters[i].speed_steps = 3; // 一次移動 3 格

        fast_monsters[i].box.rec.x = fast_monsters[i].x;
        fast_monsters[i].box.rec.y = fast_monsters[i].y;
        fast_monsters[i].box.rec.width = fast_monsters[i].width;
        fast_monsters[i].box.rec.height = fast_monsters[i].height;
        fast_monsters[i].box.rotationAngle = 0.0f;
    }
    current_fast_monster_count = INITIAL_FAST_MONSTER_COUNT;
    active_fast_monster_count = INITIAL_FAST_MONSTER_COUNT;
}

// 檢查一個 2x2 區域是否有效 (在地圖內且沒有牆壁)
// static bool is_valid_monster_position(int check_x, int check_y) {
//     for (int dx_offset = 0; dx_offset < 2; dx_offset++) {
//         for (int dy_offset = 0; dy_offset < 2; dy_offset++) {
//             int current_x = check_x + dx_offset;
//             int current_y = check_y + dy_offset;

//             if (current_x < 0 || current_y < 0 || current_x >= MAP_HEIGHT || current_y >= MAP_WIDTH || map[current_x][current_y] == 1) {
//                 return false; // 超出地圖範圍或撞牆
//             }
//         }
//     }
//     return true;
// }

// 讓怪物追蹤玩家並從四周包圍
// void move_fast_monsters_towards_player(Hero *hero) {
//     for (int i = 0; i < current_fast_monster_count; i++) {
//         // 跳過未激活的怪物
//         if (!fast_monsters[i].is_active) continue;

//         // 根據怪物的移動機率決定是否移動
//         if (rand() % 100 >= fast_monsters[i].move_chance) {
//             continue; // 跳過這次移動
//         }

//         int monster_x = fast_monsters[i].x;
//         int monster_y = fast_monsters[i].y;
//         int speed = fast_monsters[i].speed_steps; // 取得怪物自己的移動步數

//         int target_x = monster_x;
//         int target_y = monster_y;

//         // 將怪物分組，根據索引決定不同的行為模式
//         // 即使只有一種怪物，其追蹤邏輯可以保持多樣性
//         int behavior = i % 4; 

//         // 計算與玩家的距離
//         int dist_x = abs(hero->position.x - monster_x);
//         int dist_y = abs(hero->position.y - monster_y);

//         // 根據不同行為模式計算目標位置
//         switch (behavior) {
//             case 0: // 直接追蹤
//                 if (monster_x < hero->position.x) target_x += speed;
//                 else if (monster_x > hero->position.x) target_x -= speed;

//                 if (monster_y < hero->position.y) target_y += speed;
//                 else if (monster_y > hero->position.y) target_y -= speed;
//                 break;

//             case 1: // 優先從左右包抄
//                 if (monster_y < hero->position.y) target_y += speed;
//                 else if (monster_y > hero->position.y) target_y -= speed;

//                 if (dist_y <= 3 && monster_x != hero->position.x) { // 接近時調整垂直位置
//                     if (monster_x < hero->position.x) target_x += speed;
//                     else if (monster_x > hero->position.x) target_x -= speed;
//                 }
//                 break;

//             case 2: // 優先從上下包抄
//                 if (monster_x < hero->position.x) target_x += speed;
//                 else if (monster_x > hero->position.x) target_x -= speed;

//                 if (dist_x <= 3 && monster_y != hero->position.y) { // 接近時調整水平位置
//                     if (monster_y < hero->position.y) target_y += speed;
//                     else if (monster_y > hero->position.y) target_y -= speed;
//                 }
//                 break;

//             case 3: // 繞到玩家後方
//                 if (dist_x > 5 || dist_y > 5) { // 距離較遠時直接追蹤
//                     if (monster_x < hero->position.x) target_x += speed;
//                     else if (monster_x > hero->position.x) target_x -= speed;

//                     if (monster_y < hero->position.y) target_y += speed;
//                     else if (monster_y > hero->position.y) target_y -= speed;
//                 } else {
//                     // 計算玩家移動方向，嘗試從背後接近
//                     int temp_target_x = hero->position.x;
//                     int temp_target_y = hero->position.y;

//                     // 根據最後移動方向推測玩家背後位置
//                     if (hero->direction.y == 1) temp_target_x = hero->position.x + 2;     // 玩家向上，怪物去下方
//                     else if (hero->direction.y == -1) temp_target_x = hero->position.x - 2; // 玩家向下，怪物去上方
//                     else if (hero->direction.x == -1) temp_target_y = hero->position.y + 2; // 玩家向左，怪物去右方
//                     else if (hero->direction.x == 1) temp_target_y = hero->position.y - 2;  // 玩家向右，怪物去左方

//                     // 向目標位置移動
//                     if (monster_x < temp_target_x) target_x += speed;
//                     else if (monster_x > temp_target_x) target_x -= speed;

//                     if (monster_y < temp_target_y) target_y += speed;
//                     else if (monster_y > temp_target_y) target_y -= speed;
//                 }
//                 break;
//         }

//         // 檢查計算出的目標位置是否有效 (不超出地圖範圍或撞牆)
//         bool can_move_to_target = true;

//         if (can_move_to_target) {
//             // 檢查是否會與其他怪物相撞 (只檢查目標位置的重疊)
//             bool collision_with_other_monster = false;
//             for (int j = 0; j < current_fast_monster_count; j++) {
//                 if (i != j && fast_monsters[j].is_active) {
//                     // 檢查 2x2 區域是否重疊
//                     if (target_x < fast_monsters[j].x + fast_monsters[j].width &&
//                         target_x + fast_monsters[i].width > fast_monsters[j].x &&
//                         target_y < fast_monsters[j].y + fast_monsters[j].height &&
//                         target_y + fast_monsters[i].height > fast_monsters[j].y) {
//                         collision_with_other_monster = true;
//                         break;
//                     }
//                 }
//             }

//             if (!collision_with_other_monster) {
//                 fast_monsters[i].x = target_x;
//                 fast_monsters[i].y = target_y;
//             } else {
//                 // 如果與其他怪物相撞，嘗試單步移動
//                 // 為了避免怪物堆疊，這裡依然會嘗試單步移動
//                 bool moved_one_step = false;
//                 int attempted_x = monster_x;
//                 int attempted_y = monster_y;

//                 if (monster_x < hero->position.x) attempted_x += 1;
//                 else if (monster_x > hero->position.x) attempted_x -= 1;
//                 if (monster_y < hero->position.y) attempted_y += 1;
//                 else if (monster_y > hero->position.y) attempted_y -= 1;

//                 bool single_step_collision_with_other_monster = false;
//                 for (int j = 0; j < current_fast_monster_count; j++) {
//                     if (i != j && fast_monsters[j].is_active) {
//                         if (attempted_x < fast_monsters[j].x + fast_monsters[j].width &&
//                             attempted_x + fast_monsters[i].width > fast_monsters[j].x &&
//                             attempted_y < fast_monsters[j].y + fast_monsters[j].height &&
//                             attempted_y + fast_monsters[i].height > fast_monsters[j].y) {
//                             single_step_collision_with_other_monster = true;
//                             break;
//                         }
//                     }
//                 }
//                 if (!single_step_collision_with_other_monster) {
//                     fast_monsters[i].x = attempted_x;
//                     fast_monsters[i].y = attempted_y;
//                     moved_one_step = true;
//                 }
                
//                 if (!moved_one_step) {
//                     // 如果無法移動，嘗試隨機單步移動 (原有的牆壁碰撞處理邏輯)
//                     int tried = 0;
//                     while (tried < 4) {
//                         int r = rand() % 4;
//                         int new_x = monster_x + dx[r];
//                         int new_y = monster_y + dy[r];

//                         bool random_step_collision_with_other_monster = false;
//                         for (int j = 0; j < current_fast_monster_count; j++) {
//                             if (i != j && fast_monsters[j].is_active) {
//                                 if (new_x < fast_monsters[j].x + fast_monsters[j].width &&
//                                     new_x + fast_monsters[i].width > fast_monsters[j].x &&
//                                     new_y < fast_monsters[j].y + fast_monsters[j].height &&
//                                     new_y + fast_monsters[i].height > fast_monsters[j].y) {
//                                     random_step_collision_with_other_monster = true;
//                                     break;
//                                 }
//                             }
//                         }
//                         if (!random_step_collision_with_other_monster) {
//                             fast_monsters[i].x = new_x;
//                             fast_monsters[i].y = new_y;
//                             break;
//                         }
//                         tried++;
//                     }
//                 }
//             }
//         } else {
//             // 如果目標位置無效 (撞牆)，嘗試單步移動
//             bool moved_one_step = false;
//             int attempted_x = monster_x;
//             int attempted_y = monster_y;

//             if (monster_x < hero->position.x) attempted_x += 1;
//             else if (monster_x > hero->position.x) attempted_x -= 1;
//             if (monster_y < hero->position.y) attempted_y += 1;
//             else if (monster_y > hero->position.y) attempted_y -= 1;

//             bool single_step_collision_with_other_monster = false;
//             for (int j = 0; j < current_fast_monster_count; j++) {
//                 if (i != j && fast_monsters[j].is_active) {
//                     if (attempted_x < fast_monsters[j].x + fast_monsters[j].width &&
//                         attempted_x + fast_monsters[i].width > fast_monsters[j].x &&
//                         attempted_y < fast_monsters[j].y + fast_monsters[j].height &&
//                         attempted_y + fast_monsters[i].height > fast_monsters[j].y) {
//                         single_step_collision_with_other_monster = true;
//                         break;
//                     }
//                 }
//             }
//             if (!single_step_collision_with_other_monster) {
//                 fast_monsters[i].x = attempted_x;
//                 fast_monsters[i].y = attempted_y;
//                 moved_one_step = true;
//             }
            
//             if (!moved_one_step) {
//                 // 如果無法移動，嘗試隨機單步移動 (原有的牆壁碰撞處理邏輯)
//                 int tried = 0;
//                 while (tried < 4) {
//                     int r = rand() % 4;
//                     int new_x = monster_x + dx[r];
//                     int new_y = monster_y + dy[r];

//                     bool random_step_collision_with_other_monster = false;
//                     for (int j = 0; j < current_fast_monster_count; j++) {
//                         if (i != j && fast_monsters[j].is_active) {
//                             if (new_x < fast_monsters[j].x + fast_monsters[j].width &&
//                                 new_x + fast_monsters[i].width > fast_monsters[j].x &&
//                                 new_y < fast_monsters[j].y + fast_monsters[j].height &&
//                                 new_y + fast_monsters[i].height > fast_monsters[j].y) {
//                                 random_step_collision_with_other_monster = true;
//                                 break;
//                             }
//                         }
//                     }
//                     if (!random_step_collision_with_other_monster) {
//                         fast_monsters[i].x = new_x;
//                         fast_monsters[i].y = new_y;
//                         break;
//                     }
//                     tried++;
//                 }
//             }
//         }

//         // 更新碰撞箱
//         fast_monsters[i].box.rec.x = fast_monsters[i].x;
//         fast_monsters[i].box.rec.y = fast_monsters[i].y;
//         fast_monsters[i].box.rec.width = fast_monsters[i].width;
//         fast_monsters[i].box.rec.height = fast_monsters[i].height;
//     }
// }


// void move_fast_monsters_towards_player(Hero *hero) {
//     for (int i = 0; i < current_fast_monster_count; i++) {
//         // 跳過未激活的怪物
//         if (!fast_monsters[i].is_active) continue;

//         // 根據怪物的移動機率決定是否移動
//         if (rand() % 100 >= fast_monsters[i].move_chance) {
//             continue; // 跳過這次移動
//         }

//         int monster_x = fast_monsters[i].x;
//         int monster_y = fast_monsters[i].y;
//         int speed = fast_monsters[i].speed_steps; // 取得怪物自己的移動步數

//         int target_x = monster_x;
//         int target_y = monster_y;

//         // 將怪物分組，根據索引決定不同的行為模式
//         // 即使只有一種怪物，其追蹤邏輯可以保持多樣性
//         int behavior = i % 4; 

//         // 計算與玩家的距離
//         int dist_x = abs(hero->position.x - monster_x);
//         int dist_y = abs(hero->position.y - monster_y);

//         // 根據不同行為模式計算目標位置
//         switch (behavior) {
//             case 0: // 直接追蹤
//                 if (monster_x < hero->position.x) target_x += speed;
//                 else if (monster_x > hero->position.x) target_x -= speed;

//                 if (monster_y < hero->position.y) target_y += speed;
//                 else if (monster_y > hero->position.y) target_y -= speed;
//                 break;

//             case 1: // 優先從左右包抄
//                 if (monster_y < hero->position.y) target_y += speed;
//                 else if (monster_y > hero->position.y) target_y -= speed;

//                 if (dist_y <= 3 && monster_x != hero->position.x) { // 接近時調整垂直位置
//                     if (monster_x < hero->position.x) target_x += speed;
//                     else if (monster_x > hero->position.x) target_x -= speed;
//                 }
//                 break;

//             case 2: // 優先從上下包抄
//                 if (monster_x < hero->position.x) target_x += speed;
//                 else if (monster_x > hero->position.x) target_x -= speed;

//                 if (dist_x <= 3 && monster_y != hero->position.y) { // 接近時調整水平位置
//                     if (monster_y < hero->position.y) target_y += speed;
//                     else if (monster_y > hero->position.y) target_y -= speed;
//                 }
//                 break;

//             case 3: // 繞到玩家後方
//                 if (dist_x > 5 || dist_y > 5) { // 距離較遠時直接追蹤
//                     if (monster_x < hero->position.x) target_x += speed;
//                     else if (monster_x > hero->position.x) target_x -= speed;

//                     if (monster_y < hero->position.y) target_y += speed;
//                     else if (monster_y > hero->position.y) target_y -= speed;
//                 } else {
//                     // 計算玩家移動方向，嘗試從背後接近
//                     int temp_target_x = hero->position.x;
//                     int temp_target_y = hero->position.y;

//                     // 根據最後移動方向推測玩家背後位置
//                     if (hero->direction.y == 1) temp_target_x = hero->position.x + 2;     // 玩家向上，怪物去下方
//                     else if (hero->direction.y == -1) temp_target_x = hero->position.x - 2; // 玩家向下，怪物去上方
//                     else if (hero->direction.x == -1) temp_target_y = hero->position.y + 2; // 玩家向左，怪物去右方
//                     else if (hero->direction.x == 1) temp_target_y = hero->position.y - 2;  // 玩家向右，怪物去左方

//                     // 向目標位置移動
//                     if (monster_x < temp_target_x) target_x += speed;
//                     else if (monster_x > temp_target_x) target_x -= speed;

//                     if (monster_y < temp_target_y) target_y += speed;
//                     else if (monster_y > temp_target_y) target_y -= speed;
//                 }
//                 break;
//         }

//         // 檢查計算出的目標位置是否有效 (不超出地圖範圍或撞牆)
//         bool can_move_to_target = true;

//         // 檢查移動路徑上的每一步，確保不會與其他怪物重疊
//         bool path_clear = true;
//         int steps_x = target_x - monster_x;
//         int steps_y = target_y - monster_y;
//         int abs_steps_x = abs(steps_x);
//         int abs_steps_y = abs(steps_y);
//         int max_steps = abs_steps_x > abs_steps_y ? abs_steps_x : abs_steps_y;

//         if (max_steps > 0) {
//             float step_x = steps_x / (float)max_steps;
//             float step_y = steps_y / (float)max_steps;
//             for (int step = 1; step <= max_steps; step++) {
//                 int current_x = monster_x + (int)(step_x * step + 0.5); // 四捨五入
//                 int current_y = monster_y + (int)(step_y * step + 0.5);

//                 for (int j = 0; j < current_fast_monster_count; j++) {
//                     if (i != j && fast_monsters[j].is_active) {
//                         if (current_x < fast_monsters[j].x + fast_monsters[j].width &&
//                             current_x + fast_monsters[i].width > fast_monsters[j].x &&
//                             current_y < fast_monsters[j].y + fast_monsters[j].height &&
//                             current_y + fast_monsters[i].height > fast_monsters[j].y) {
//                             path_clear = false;
//                             break;
//                         }
//                     }
//                 }
//                 if (!path_clear) break;
//             }
//         }

//         if (can_move_to_target && path_clear) {
//             // 檢查最終目標位置是否與其他怪物重疊
//             bool collision_with_other_monster = false;
//             for (int j = 0; j < current_fast_monster_count; j++) {
//                 if (i != j && fast_monsters[j].is_active) {
//                     if (target_x < fast_monsters[j].x + fast_monsters[j].width &&
//                         target_x + fast_monsters[i].width > fast_monsters[j].x &&
//                         target_y < fast_monsters[j].y + fast_monsters[j].height &&
//                         target_y + fast_monsters[i].height > fast_monsters[j].y) {
//                         collision_with_other_monster = true;
//                         break;
//                     }
//                 }
//             }

//             if (!collision_with_other_monster) {
//                 fast_monsters[i].x = target_x;
//                 fast_monsters[i].y = target_y;
//             } else {
//                 // 如果目標位置有碰撞，嘗試單步移動
//                 bool moved_one_step = false;
//                 int attempted_x = monster_x;
//                 int attempted_y = monster_y;

//                 // 計算單步移動方向，考慮玩家位置
//                 if (monster_x < hero->position.x) attempted_x += 1;
//                 else if (monster_x > hero->position.x) attempted_x -= 1;
//                 if (monster_y < hero->position.y) attempted_y += 1;
//                 else if (monster_y > hero->position.y) attempted_y -= 1;

//                 // 檢查單步移動是否會與其他怪物重疊
//                 bool single_step_collision_with_other_monster = false;
//                 for (int j = 0; j < current_fast_monster_count; j++) {
//                     if (i != j && fast_monsters[j].is_active) {
//                         if (attempted_x < fast_monsters[j].x + fast_monsters[j].width &&
//                             attempted_x + fast_monsters[i].width > fast_monsters[j].x &&
//                             attempted_y < fast_monsters[j].y + fast_monsters[j].height &&
//                             attempted_y + fast_monsters[i].height > fast_monsters[j].y) {
//                             single_step_collision_with_other_monster = true;
//                             break;
//                         }
//                     }
//                 }
//                 if (!single_step_collision_with_other_monster && can_move_to_target) {
//                     fast_monsters[i].x = attempted_x;
//                     fast_monsters[i].y = attempted_y;
//                     moved_one_step = true;
//                 }
                
//                 if (!moved_one_step) {
//                     // 如果單步移動也失敗，嘗試隨機單步移動
//                     int tried = 0;
//                     while (tried < 4) {
//                         int r = rand() % 4;
//                         int new_x = monster_x + dx[r];
//                         int new_y = monster_y + dy[r];

//                         bool random_step_collision_with_other_monster = false;
//                         for (int j = 0; j < current_fast_monster_count; j++) {
//                             if (i != j && fast_monsters[j].is_active) {
//                                 if (new_x < fast_monsters[j].x + fast_monsters[j].width &&
//                                     new_x + fast_monsters[i].width > fast_monsters[j].x &&
//                                     new_y < fast_monsters[j].y + fast_monsters[j].height &&
//                                     new_y + fast_monsters[i].height > fast_monsters[j].y) {
//                                     random_step_collision_with_other_monster = true;
//                                     break;
//                                 }
//                             }
//                         }
//                         if (!random_step_collision_with_other_monster && can_move_to_target) {
//                             fast_monsters[i].x = new_x;
//                             fast_monsters[i].y = new_y;
//                             break;
//                         }
//                         tried++;
//                     }
//                 }
//             }
//         } else {
//             // 如果目標位置無效或路徑上有碰撞，嘗試單步移動
//             bool moved_one_step = false;
//             int attempted_x = monster_x;
//             int attempted_y = monster_y;

//             if (monster_x < hero->position.x) attempted_x += 1;
//             else if (monster_x > hero->position.x) attempted_x -= 1;
//             if (monster_y < hero->position.y) attempted_y += 1;
//             else if (monster_y > hero->position.y) attempted_y -= 1;

//             bool single_step_collision_with_other_monster = false;
//             for (int j = 0; j < current_fast_monster_count; j++) {
//                 if (i != j && fast_monsters[j].is_active) {
//                     if (attempted_x < fast_monsters[j].x + fast_monsters[j].width &&
//                         attempted_x + fast_monsters[i].width > fast_monsters[j].x &&
//                         attempted_y < fast_monsters[j].y + fast_monsters[j].height &&
//                         attempted_y + fast_monsters[i].height > fast_monsters[j].y) {
//                         single_step_collision_with_other_monster = true;
//                         break;
//                     }
//                 }
//             }
//             if (!single_step_collision_with_other_monster && can_move_to_target) {
//                 fast_monsters[i].x = attempted_x;
//                 fast_monsters[i].y = attempted_y;
//                 moved_one_step = true;
//             }
            
//             if (!moved_one_step) {
//                 // 如果單步移動也失敗，嘗試隨機單步移動
//                 int tried = 0;
//                 while (tried < 4) {
//                     int r = rand() % 4;
//                     int new_x = monster_x + dx[r];
//                     int new_y = monster_y + dy[r];

//                     bool random_step_collision_with_other_monster = false;
//                     for (int j = 0; j < current_fast_monster_count; j++) {
//                         if (i != j && fast_monsters[j].is_active) {
//                             if (new_x < fast_monsters[j].x + fast_monsters[j].width &&
//                                 new_x + fast_monsters[i].width > fast_monsters[j].x &&
//                                 new_y < fast_monsters[j].y + fast_monsters[j].height &&
//                                 new_y + fast_monsters[i].height > fast_monsters[j].y) {
//                                 random_step_collision_with_other_monster = true;
//                                 break;
//                             }
//                         }
//                     }
//                     if (!random_step_collision_with_other_monster && can_move_to_target) {
//                         fast_monsters[i].x = new_x;
//                         fast_monsters[i].y = new_y;
//                         break;
//                     }
//                     tried++;
//                 }
//             }
//         }

//         // 更新碰撞箱
//         fast_monsters[i].box.rec.x = fast_monsters[i].x;
//         fast_monsters[i].box.rec.y = fast_monsters[i].y;
//         fast_monsters[i].box.rec.width = fast_monsters[i].width;
//         fast_monsters[i].box.rec.height = fast_monsters[i].height;
//     }
// }


void move_fast_monsters_towards_player(Hero *hero) {
    for (int i = 0; i < current_fast_monster_count; i++) {
        // 跳過未激活的怪物
        if (!fast_monsters[i].is_active) continue;

        // 根據怪物的移動機率決定是否移動
        if (rand() % 100 >= fast_monsters[i].move_chance) {
            continue; // 跳過這次移動
        }

        // 儲存當前位置作為上一次位置
        int prev_x = fast_monsters[i].x;
        int prev_y = fast_monsters[i].y;

        int monster_x = fast_monsters[i].x;
        int monster_y = fast_monsters[i].y;
        int speed = fast_monsters[i].speed_steps; // 取得怪物自己的移動步數

        int target_x = monster_x;
        int target_y = monster_y;

        // 將怪物分組，根據索引決定不同的行為模式
        // 即使只有一種怪物，其追蹤邏輯可以保持多樣性
        int behavior = i % 4; 

        // 計算與玩家的距離
        int dist_x = abs(hero->position.x - monster_x);
        int dist_y = abs(hero->position.y - monster_y);

        // 根據不同行為模式計算目標位置
        switch (behavior) {
            case 0: // 直接追蹤
                if (monster_x < hero->position.x) target_x += speed;
                else if (monster_x > hero->position.x) target_x -= speed;

                if (monster_y < hero->position.y) target_y += speed;
                else if (monster_y > hero->position.y) target_y -= speed;
                break;

            case 1: // 優先從左右包抄
                if (monster_y < hero->position.y) target_y += speed;
                else if (monster_y > hero->position.y) target_y -= speed;

                if (dist_y <= 3 && monster_x != hero->position.x) { // 接近時調整垂直位置
                    if (monster_x < hero->position.x) target_x += speed;
                    else if (monster_x > hero->position.x) target_x -= speed;
                }
                break;

            case 2: // 優先從上下包抄
                if (monster_x < hero->position.x) target_x += speed;
                else if (monster_x > hero->position.x) target_x -= speed;

                if (dist_x <= 3 && monster_y != hero->position.y) { // 接近時調整水平位置
                    if (monster_y < hero->position.y) target_y += speed;
                    else if (monster_y > hero->position.y) target_y -= speed;
                }
                break;

            case 3: // 繞到玩家後方
                if (dist_x > 5 || dist_y > 5) { // 距離較遠時直接追蹤
                    if (monster_x < hero->position.x) target_x += speed;
                    else if (monster_x > hero->position.x) target_x -= speed;

                    if (monster_y < hero->position.y) target_y += speed;
                    else if (monster_y > hero->position.y) target_y -= speed;
                } else {
                    // 計算玩家移動方向，嘗試從背後接近
                    int temp_target_x = hero->position.x;
                    int temp_target_y = hero->position.y;

                    // 根據最後移動方向推測玩家背後位置
                    if (hero->direction.y == 1) temp_target_x = hero->position.x + 2;     // 玩家向上，怪物去下方
                    else if (hero->direction.y == -1) temp_target_x = hero->position.x - 2; // 玩家向下，怪物去上方
                    else if (hero->direction.x == -1) temp_target_y = hero->position.y + 2; // 玩家向左，怪物去右方
                    else if (hero->direction.x == 1) temp_target_y = hero->position.y - 2;  // 玩家向右，怪物去左方

                    // 向目標位置移動
                    if (monster_x < temp_target_x) target_x += speed;
                    else if (monster_x > temp_target_x) target_x -= speed;

                    if (monster_y < temp_target_y) target_y += speed;
                    else if (monster_y > temp_target_y) target_y -= speed;
                }
                break;
        }

        // 檢查計算出的目標位置是否有效 (不超出地圖範圍或撞牆)
        bool can_move_to_target = true;

        if (can_move_to_target) {
            // 檢查最終目標位置是否與其他怪物重疊
            bool collision_with_other_monster = false;
            for (int j = 0; j < current_fast_monster_count; j++) {
                if (i != j && fast_monsters[j].is_active) {
                    if (target_x < fast_monsters[j].x + fast_monsters[j].width &&
                        target_x + fast_monsters[i].width > fast_monsters[j].x &&
                        target_y < fast_monsters[j].y + fast_monsters[j].height &&
                        target_y + fast_monsters[i].height > fast_monsters[j].y) {
                        collision_with_other_monster = true;
                        break;
                    }
                }
            }

            if (!collision_with_other_monster) {
                fast_monsters[i].x = target_x;
                fast_monsters[i].y = target_y;
            } else {
                // 如果目標位置有碰撞，嘗試單步移動
                bool moved_one_step = false;
                int attempted_x = monster_x;
                int attempted_y = monster_y;

                if (monster_x < hero->position.x) attempted_x += 1;
                else if (monster_x > hero->position.x) attempted_x -= 1;
                if (monster_y < hero->position.y) attempted_y += 1;
                else if (monster_y > hero->position.y) attempted_y -= 1;

                bool single_step_collision_with_other_monster = false;
                for (int j = 0; j < current_fast_monster_count; j++) {
                    if (i != j && fast_monsters[j].is_active) {
                        if (attempted_x < fast_monsters[j].x + fast_monsters[j].width &&
                            attempted_x + fast_monsters[i].width > fast_monsters[j].x &&
                            attempted_y < fast_monsters[j].y + fast_monsters[j].height &&
                            attempted_y + fast_monsters[i].height > fast_monsters[j].y) {
                            single_step_collision_with_other_monster = true;
                            break;
                        }
                    }
                }
                if (!single_step_collision_with_other_monster) {
                    fast_monsters[i].x = attempted_x;
                    fast_monsters[i].y = attempted_y;
                    moved_one_step = true;
                }
                
                if (!moved_one_step) {
                    // 如果單步移動也失敗，嘗試隨機單步移動
                    int tried = 0;
                    while (tried < 4) {
                        int r = rand() % 4;
                        int new_x = monster_x + dx[r];
                        int new_y = monster_y + dy[r];

                        bool random_step_collision_with_other_monster = false;
                        for (int j = 0; j < current_fast_monster_count; j++) {
                            if (i != j && fast_monsters[j].is_active) {
                                if (new_x < fast_monsters[j].x + fast_monsters[j].width &&
                                    new_x + fast_monsters[i].width > fast_monsters[j].x &&
                                    new_y < fast_monsters[j].y + fast_monsters[j].height &&
                                    new_y + fast_monsters[i].height > fast_monsters[j].y) {
                                    random_step_collision_with_other_monster = true;
                                    break;
                                }
                            }
                        }
                        if (!random_step_collision_with_other_monster) {
                            fast_monsters[i].x = new_x;
                            fast_monsters[i].y = new_y;
                            break;
                        }
                        tried++;
                    }
                }

                // 如果所有移動嘗試都失敗，恢復到上一次位置
                if (fast_monsters[i].x == monster_x && fast_monsters[i].y == monster_y) {
                    fast_monsters[i].x = prev_x;
                    fast_monsters[i].y = prev_y;
                }
            }
        } else {
            // 如果目標位置無效 (撞牆)，嘗試單步移動
            bool moved_one_step = false;
            int attempted_x = monster_x;
            int attempted_y = monster_y;

            if (monster_x < hero->position.x) attempted_x += 1;
            else if (monster_x > hero->position.x) attempted_x -= 1;
            if (monster_y < hero->position.y) attempted_y += 1;
            else if (monster_y > hero->position.y) attempted_y -= 1;

            bool single_step_collision_with_other_monster = false;
            for (int j = 0; j < current_fast_monster_count; j++) {
                if (i != j && fast_monsters[j].is_active) {
                    if (attempted_x < fast_monsters[j].x + fast_monsters[j].width &&
                        attempted_x + fast_monsters[i].width > fast_monsters[j].x &&
                        attempted_y < fast_monsters[j].y + fast_monsters[j].height &&
                        attempted_y + fast_monsters[i].height > fast_monsters[j].y) {
                        single_step_collision_with_other_monster = true;
                        break;
                    }
                }
            }
            if (!single_step_collision_with_other_monster) {
                fast_monsters[i].x = attempted_x;
                fast_monsters[i].y = attempted_y;
                moved_one_step = true;
            }
            
            if (!moved_one_step) {
                // 如果單步移動也失敗，嘗試隨機單步移動
                int tried = 0;
                while (tried < 4) {
                    int r = rand() % 4;
                    int new_x = monster_x + dx[r];
                    int new_y = monster_y + dy[r];

                    bool random_step_collision_with_other_monster = false;
                    for (int j = 0; j < current_fast_monster_count; j++) {
                        if (i != j && fast_monsters[j].is_active) {
                            if (new_x < fast_monsters[j].x + fast_monsters[j].width &&
                                new_x + fast_monsters[i].width > fast_monsters[j].x &&
                                new_y < fast_monsters[j].y + fast_monsters[j].height &&
                                new_y + fast_monsters[i].height > fast_monsters[j].y) {
                                random_step_collision_with_other_monster = true;
                                break;
                            }
                        }
                    }
                    if (!random_step_collision_with_other_monster) {
                        fast_monsters[i].x = new_x;
                        fast_monsters[i].y = new_y;
                        break;
                    }
                    tried++;
                }
            }
            // 如果所有移動嘗試都失敗，恢復到上一次位置
            if (fast_monsters[i].x == monster_x && fast_monsters[i].y == monster_y) {
                fast_monsters[i].x = prev_x;
                fast_monsters[i].y = prev_y;
            }
        }

        // 更新碰撞箱
        fast_monsters[i].box.rec.x = fast_monsters[i].x;
        fast_monsters[i].box.rec.y = fast_monsters[i].y;
        fast_monsters[i].box.rec.width = fast_monsters[i].width;
        fast_monsters[i].box.rec.height = fast_monsters[i].height;
    }
}

// 新增多個怪物的函數
void add_fast_monsters(Hero *hero,int map_type) {
    // 基於時間生成怪物
    game_fast_monster_time += GetFrameTime();

    if (game_fast_monster_time >= next_fast_spawn_time) {
        int to_add = FAST_MONSTER_SPAWN_RATE;

        // 確保不超過上限
        if (current_fast_monster_count + to_add > MAX_FAST_MONSTER_COUNT) {
            to_add = MAX_FAST_MONSTER_COUNT - current_fast_monster_count;
        }

        // 新增指定數量的怪物
        for (int i = 0; i < to_add; i++) {
            if (current_fast_monster_count < MAX_FAST_MONSTER_COUNT) {
                Vector2 position = random_empty_position(hero);
                fast_monsters[current_fast_monster_count].x = position.x;
                fast_monsters[current_fast_monster_count].y = position.y;
                fast_monsters[current_fast_monster_count].is_active = 1;
                fast_monsters[current_fast_monster_count].width = 2;
                fast_monsters[current_fast_monster_count].height = 2;

                // 固定為快速怪物的屬性
                fast_monsters[current_fast_monster_count].color = BLUE;
                fast_monsters[i].health = 300 + map_type*fast_health_up;   //設置血量，依地圖而變 
                fast_monsters[current_fast_monster_count].move_chance = 95;
                fast_monsters[current_fast_monster_count].speed_steps = 5;

                // 碰撞箱
                fast_monsters[current_fast_monster_count].box.rec.x = fast_monsters[current_fast_monster_count].x;
                fast_monsters[current_fast_monster_count].box.rec.y = fast_monsters[current_fast_monster_count].y;
                fast_monsters[current_fast_monster_count].box.rec.width = fast_monsters[current_fast_monster_count].width;
                fast_monsters[current_fast_monster_count].box.rec.height = fast_monsters[current_fast_monster_count].height;
                fast_monsters[current_fast_monster_count].box.rotationAngle = 0.0f;

                // 更新當前怪物數量
                current_fast_monster_count++;
                active_fast_monster_count++;
            }
        }
        next_fast_spawn_time = game_fast_monster_time + FAST_MONSTER_SPAWN_INTERVAL;
    }
}

// 檢查並替換消失的怪物
void replace_missing_fast_monsters(Hero *hero,int map_type) {
    int missing_count = 0;

    // 計算已消失的怪物數量
    for (int i = 0; i < current_fast_monster_count; i++) {
        if (!fast_monsters[i].is_active) {
            missing_count++;
        }
    }

    // 如果有消失的怪物，替換它們
    if (missing_count > 0) {
        int to_replace = missing_count;

        // 找到未活躍的怪物並重新生成
        for (int i = 0; i < current_fast_monster_count && to_replace > 0; i++) {
            if (!fast_monsters[i].is_active) {
                Vector2 position = random_empty_position(hero);
                fast_monsters[i].x = position.x;
                fast_monsters[i].y = position.y;
                fast_monsters[i].is_active = 1;
                fast_monsters[i].width = 2;
                fast_monsters[i].height = 2;

                // 固定為快速怪物的屬性
                fast_monsters[i].color = BLUE;
                fast_monsters[i].health = 150 + map_type*fast_health_up;   //設置血量，依地圖而變 
                fast_monsters[i].move_chance = 95;
                fast_monsters[i].speed_steps = 3;

                // 碰撞箱
                fast_monsters[i].box.rec.x = fast_monsters[i].x;
                fast_monsters[i].box.rec.y = fast_monsters[i].y;
                fast_monsters[i].box.rec.width = fast_monsters[i].width;
                fast_monsters[i].box.rec.height = fast_monsters[i].height;
                fast_monsters[i].box.rotationAngle = 0.0f;

                to_replace--;
                active_fast_monster_count++;
            }
        }
    }
}

// 繪製怪物
void draw_fast_monsters() {
    for (int i = 0; i < current_fast_monster_count; i++) {
        if (fast_monsters[i].is_active) {
            // 繪製快速怪物，顏色固定為藍色，並保持閃爍和邊框效果
            if ((int)(game_fast_monster_time * 10) % 2 == 0) { // 快速閃爍
                DrawRectangle(fast_monsters[i].x, fast_monsters[i].y, CELL_SIZE * fast_monsters[i].height, CELL_SIZE * fast_monsters[i].width, fast_monsters[i].color);
            } else {
                DrawRectangle(fast_monsters[i].x, fast_monsters[i].y, CELL_SIZE * fast_monsters[i].height, CELL_SIZE * fast_monsters[i].width, DARKBLUE); // 閃爍時變暗
            }
            // 繪製一個黃色邊框來突出快速怪物
            DrawRectangleLinesEx((Rectangle){fast_monsters[i].x, fast_monsters[i].y, CELL_SIZE * fast_monsters[i].height, CELL_SIZE * fast_monsters[i].width}, 1, YELLOW);
        }
    }
}

// 檢查玩家是否與怪物相撞
void check_fast_monster_collision(Hero *hero) {
    static int invincibility_timer = 0; // 無敵時間計數器

    // 如果玩家處於無敵狀態，減少計時器並返回
    if (invincibility_timer > 0) {
        invincibility_timer--;
        return;
    }

    for (int i = 0; i < current_fast_monster_count; i++) {
        if (fast_monsters[i].is_active) {
            // 檢查玩家與怪物的碰撞
            bool collision = CheckCollsionBoxes(hero->box, fast_monsters[i].box);

            if (collision) {
                // 怪物受到傷害
                fast_monsters[i].health--;

                // 設置碰撞後的無敵時間
                invincibility_timer = 60; // 假設 60 幀為 1 秒

                // 玩家受到傷害
                hero->hp -= 2;

                // 如果玩家生命值歸零，直接返回
                if (hero->hp <= 0) {
                    return;
                }

                // 僅當怪物生命值為 0 或更低時才爆炸
                if (fast_monsters[i].health <= 0) {
                    // 在怪物生命 0 時創建爆炸
                    // 播放爆炸音效
                    Sound sound = LoadSound("resources/sound/mansterDie.mp3");
                    PlaySound(sound);
                    create_explosion(fast_monsters[i].x, fast_monsters[i].y, fast_monsters[i].color);
                    // 標記怪物為非活躍
                    fast_monsters[i].is_active = 0;
                    active_fast_monster_count--;

                    // 增加死亡怪物計數
                     dead_fast_monster_count++;
                }

                break; // 每次只處理一次與一個怪物的碰撞
            }
        }
    }
}

void get_fast_monster_demage(Hero* hero) {
    for(int i=0; i<current_fast_monster_count; i++) {
        if(fast_monsters[i].is_active) {
            fast_monsters[i].health -= CheckDemage(&fast_monsters[i].box, hero);

            if(fast_monsters[i].health <= 0) {
                // 在怪物生命0時創建爆炸
                // 播放爆炸音效
                Sound sound = LoadSound("resources/sound/mansterDie.mp3");
                PlaySound(sound);
                create_explosion(fast_monsters[i].x, fast_monsters[i].y, fast_monsters[i].color);
                draw_explosions();
                // 標記怪物為非活躍
                fast_monsters[i].is_active = 0;
                active_fast_monster_count--;

                //增加角色經驗值
                hero->exp += 2;
                
                // 增加死亡怪物計數
                dead_fast_monster_count++;

                //檢查寶箱
                Treasure_OnMonsterKilled(hero);
            }
        }
    }
}
