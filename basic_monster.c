#include"basic_monster.h"
#include "hero.h"
#include "monster.h"
#include "treasure.h"
#include <raylib.h>

float pre_game_time = 0.0f;
float game_time = 0.0f;
float next_spawn_time = 3.0f;
int dead_monster_count = 0;
int current_monster_count = 0;
int active_monster_count = 0;
Monster monsters[MAX_MONSTER_COUNT];

// 初始化怪物位置
void spawn_monsters(Hero *hero) {
    for (int i = 0; i < INITIAL_MONSTER_COUNT; i++) {
        Vector2 position = random_empty_position(hero);
        monsters[i].x= position.x;
        monsters[i].y = position.y;
        monsters[i].is_active = 1;  // 標記為活躍
        monsters[i].color = RED;    // 設置怪物為紅色
        monsters[i].health = 300;    // 設置怪物為紅色
        monsters[i].width = 2;    // 設置怪物寬度2
        monsters[i].height = 2;    // 設置怪物為高度2

        monsters[i].box.rec.x=monsters[i].x;
        monsters[i].box.rec.y=monsters[i].y;
        monsters[i].box.rec.width=monsters[i].width * CELL_SIZE;
        monsters[i].box.rec.height=monsters[i].height * CELL_SIZE;
        monsters[i].box.rotationAngle=0.0f;
    }
    current_monster_count = INITIAL_MONSTER_COUNT;
    active_monster_count = INITIAL_MONSTER_COUNT;
}


// 讓怪物追蹤玩家並從四周包圍
void move_monsters_towards_player(Hero *hero) {
    for (int i = 0; i < current_monster_count; i++) {
        // 跳過未激活的怪物
        if (!monsters[i].is_active) continue;
        
        // 降低怪物移動的頻率 (調整MONSTER_MOVE_CHANCE來控制怪物速度)
        // if (rand() % 100 < MONSTER_MOVE_CHANCE) {
        //     continue;  // 跳過這次移動
        // }

        int monster_x = monsters[i].x;
        int monster_y = monsters[i].y;
        
        // 將怪物分組，根據索引決定不同的行為模式
        int behavior = i % 4;  // 將怪物分為4種行為
        
        // 計算與玩家的距離
        int dist_x = abs(hero->position.x - monster_x);
        int dist_y = abs(hero->position.y - monster_y);
        
        // 根據不同行為模式移動
        switch(behavior) {
            case 0:  // 直接追蹤
                if (monster_x < hero->position.x) monsters[i].x++;
                else if (monster_x > hero->position.x) monsters[i].x--;
                
                if (monster_y < hero->position.y) monsters[i].y++;
                else if (monster_y > hero->position.y) monsters[i].y--;
                break;
                
            case 1:  // 優先從左右包抄
                if (monster_y < hero->position.y) monsters[i].y++;
                else if (monster_y > hero->position.y) monsters[i].y--;
                
                if (dist_y <= 3 && monster_x != hero->position.x) {  // 接近時調整垂直位置
                    if (monster_x < hero->position.x) monsters[i].x++;
                    else if (monster_x > hero->position.x) monsters[i].x--;
                }
                break;
                
            case 2:  // 優先從上下包抄
                if (monster_x < hero->position.x) monsters[i].x++;
                else if (monster_x > hero->position.x) monsters[i].x--;
                
                if (dist_x <= 3 && monster_y != hero->position.y) {  // 接近時調整水平位置
                    if (monster_y < hero->position.y) monsters[i].y++;
                    else if (monster_y > hero->position.y) monsters[i].y--;
                }
                break;
                
            case 3:  // 繞到玩家後方
                if (dist_x > 5 || dist_y > 5) {  // 距離較遠時直接追蹤
                    if (monster_x < hero->position.x) monsters[i].x++;
                    else if (monster_x > hero->position.x) monsters[i].x--;
                    
                    if (monster_y < hero->position.y) monsters[i].y++;
                    else if (monster_y > hero->position.y) monsters[i].y--;
                } else {
                    // 計算玩家移動方向，嘗試從背後接近
                    int target_x = hero->position.x;
                    int target_y = hero->position.y;
                    
                    // 根據最後移動方向推測玩家背後位置
                    if (hero->direction.y == 1) target_x = hero->position.x + 2;  // 玩家向上，怪物去下方
                    else if (hero->direction.y == -1) target_x = hero->position.x - 2;  // 玩家向下，怪物去上方
                    else if (hero->direction.x == -1) target_y = hero->position.y + 2;  // 玩家向左，怪物去右方
                    else if (hero->direction.x == 1) target_y = hero->position.y - 2;  // 玩家向右，怪物去左方
                    
                    // 向目標位置移動
                    if (monster_x < target_x) monsters[i].x++;
                    else if (monster_x > target_x) monsters[i].x--;
                    
                    if (monster_y < target_y) monsters[i].y++;
                    else if (monster_y > target_y) monsters[i].y--;
                }
                break;
        }

        // 確保怪物不會超出地圖範圍或撞牆
        // 考慮2x2大小
        // int valid = 1;
        // for (int dx = 0; dx < 2 && valid; dx++) {
        //     for (int dy = 0; dy < 2 && valid; dy++) {
        //         int check_x = monsters[i].x + dx;
        //         int check_y = monsters[i].y + dy;
                
        //         if (check_x < 0 || check_y < 0 || check_x >= MAP_HEIGHT || check_y >= MAP_WIDTH || map[check_x][check_y] == 1) {
        //             valid = 0;
        //         }
        //     }
        // }
        
        // if (!valid) {
        //     monsters[i].x = monster_x;  // 恢復原始位置
        //     monsters[i].y = monster_y;
            
        //     // 嘗試其他方向
        //     int tried = 0;
        //     while (tried < 4) {
        //         int r = rand() % 4;
        //         int new_x = monster_x + dx[r];
        //         int new_y = monster_y + dy[r];
                
        //         // 檢查新位置是否有效 (考慮2x2大小)
        //         int new_valid = 1;
        //         for (int ndx = 0; ndx < 2 && new_valid; ndx++) {
        //             for (int ndy = 0; ndy < 2 && new_valid; ndy++) {
        //                 int check_x = new_x + ndx;
        //                 int check_y = new_y + ndy;
                        
        //                 if (check_x < 0 || check_y < 0 || check_x >= MAP_HEIGHT || check_y >= MAP_WIDTH || map[check_x][check_y] == 1) {
        //                     new_valid = 0;
        //                 }
        //             }
        //         }
                
        //         if (new_valid) {
        //             monsters[i].x = new_x;
        //             monsters[i].y = new_y;
        //             break;
        //         }
                
        //         tried++;
        //     }
        // }

        // 檢查怪物間是否相撞
        for (int j = 0; j < current_monster_count; j++) {
            if (i != j && monsters[j].is_active) {
                // 檢查2x2區域是否重疊
                bool collision = false;
                for (int mi = 0; mi < 2 && !collision; mi++) {
                    for (int mj = 0; mj < 2 && !collision; mj++) {
                        for (int ni = 0; ni < 2 && !collision; ni++) {
                            for (int nj = 0; nj < 2 && !collision; nj++) {
                                if (monsters[i].x + mi == monsters[j].x + ni && 
                                    monsters[i].y + mj == monsters[j].y + nj) {
                                    collision = true;
                                }
                            }
                        }
                    }
                }
                
                if (collision) {
                    // 返回原始位置
                    monsters[i].x = monster_x;
                    monsters[i].y = monster_y;
                    break;
                }
            }
        }

        //更新碰撞箱
        monsters[i].box.rec.x=monsters[i].x;
        monsters[i].box.rec.y=monsters[i].y;
        monsters[i].box.rec.width=monsters[i].width * CELL_SIZE;
        monsters[i].box.rec.height=monsters[i].height * CELL_SIZE;
    }
}

// 新增多個怪物的函數
void add_monsters(Hero *hero) {
    // 基於時間生成怪物
    game_time += GetFrameTime();
    
    if (game_time >= next_spawn_time) 
    {
        int to_add = MONSTER_SPAWN_RATE;
    
        // 確保不超過上限
        if (current_monster_count + to_add > MAX_MONSTER_COUNT) {
            to_add = MAX_MONSTER_COUNT - current_monster_count;
        }
        
        // 新增指定數量的怪物
        for (int i = 0; i < to_add; i++) {
            if (current_monster_count < MAX_MONSTER_COUNT) {
                Vector2 position = random_empty_position(hero);
                monsters[current_monster_count].x = position.x;
                monsters[current_monster_count].y = position.y;
                monsters[current_monster_count].is_active = 1;
                monsters[current_monster_count].color = RED;
                monsters[current_monster_count].width = 2;
                monsters[current_monster_count].height = 2;
                monsters[current_monster_count].health = 300;

                //碰撞箱
                monsters[i].box.rec.x=monsters[i].x;
                monsters[i].box.rec.y=monsters[i].y;
                monsters[i].box.rec.width=monsters[i].width * CELL_SIZE;
                monsters[i].box.rec.height=monsters[i].height * CELL_SIZE;
                monsters[i].box.rotationAngle=0.0f;

                //更新當前普通怪物數量
                current_monster_count++;
                active_monster_count++;
            }
        }
        next_spawn_time = game_time + MONSTER_SPAWN_INTERVAL;
    }

}

// 檢查並替換消失的怪物
void replace_missing_monsters(Hero *hero) {
    int missing_count = 0;
    
    // 計算已消失的怪物數量
    for (int i = 0; i < current_monster_count; i++) {
        if (!monsters[i].is_active) {
            missing_count++;
        }
    }
    
    // 如果有消失的怪物，替換它們
    if (missing_count > 0) {
        int to_replace = missing_count;
        Node pos;
        
        // 找到未活躍的怪物並重新生成
        for (int i = 0; i < current_monster_count && to_replace > 0; i++) {
            if (!monsters[i].is_active) {
                Vector2 position = random_empty_position(hero);
                monsters[i].x = position.x;
                monsters[i].y = position.y;
                monsters[i].is_active = 1;
                monsters[i].health = 300;

                //碰撞箱
                monsters[i].box.rec.x=monsters[i].x;
                monsters[i].box.rec.y=monsters[i].y;
                monsters[i].box.rec.width=monsters[i].width * CELL_SIZE;
                monsters[i].box.rec.height=monsters[i].height * CELL_SIZE;
                monsters[i].box.rotationAngle=0.0f;

                to_replace--;
                active_monster_count++;
            }
        }
    }
}

// 绘制普通怪物
void draw_monsters() {
    static Color now_color;
    for (int i = 0; i < current_monster_count; i++) {
        if (monsters[i].is_active) {
            // DrawRectangle(monsters[i].x *10, monsters[i].y * 10, 10 *monsters[i].width, 10 * monsters[i].height, RED);
            if(i%2==0) {
                if((int)(game_time*2)%5==0) {
                    int choose_color=rand()%7;
                    if(choose_color==0){
                        DrawRectangle(monsters[i].x, monsters[i].y, CELL_SIZE *monsters[i].width, CELL_SIZE * monsters[i].height, RED);
                        now_color=RED;
                    }
                    else if(choose_color==1){
                        DrawRectangle(monsters[i].x, monsters[i].y, CELL_SIZE * monsters[i].width, CELL_SIZE * monsters[i].height, WHITE);
                        now_color=WHITE;
                    }
                    else if(choose_color==2){
                        DrawRectangle(monsters[i].x, monsters[i].y, CELL_SIZE * monsters[i].width, CELL_SIZE * monsters[i].height, GREEN);
                        now_color=GREEN;
                    }
                    else if(choose_color==3){
                        DrawRectangle(monsters[i].x, monsters[i].y , CELL_SIZE * monsters[i].width, CELL_SIZE * monsters[i].height, ORANGE);
                        now_color=ORANGE;
                    }
                    else if(choose_color==4){
                        DrawRectangle(monsters[i].x, monsters[i].y , CELL_SIZE * monsters[i].width, CELL_SIZE * monsters[i].height, YELLOW);
                        now_color=YELLOW;
                    }
                    else if(choose_color==5){
                        DrawRectangle(monsters[i].x, monsters[i].y, CELL_SIZE * monsters[i].width, CELL_SIZE * monsters[i].height, BLUE);
                        now_color=BLUE;
                    }
                    else if(choose_color==6){
                        DrawRectangle(monsters[i].x , monsters[i].y, CELL_SIZE * monsters[i].width, CELL_SIZE * monsters[i].height, PURPLE);
                        now_color=PURPLE;
                    }
                }
                else {
                    DrawRectangle(monsters[i].x, monsters[i].y, CELL_SIZE * monsters[i].width, CELL_SIZE * monsters[i].height, now_color);
                }
            } else {
                int choose_color=rand()%7;
                if(choose_color==0){
                    DrawRectangle(monsters[i].x , monsters[i].y, CELL_SIZE * monsters[i].width, CELL_SIZE * monsters[i].height, RED);
                    now_color=RED;
                }
                else if(choose_color==1){
                    DrawRectangle(monsters[i].x , monsters[i].y, CELL_SIZE * monsters[i].width, CELL_SIZE * monsters[i].height, WHITE);
                    now_color=WHITE;
                }
                else if(choose_color==2){
                    DrawRectangle(monsters[i].x, monsters[i].y, CELL_SIZE * monsters[i].width, CELL_SIZE * monsters[i].height, GREEN);
                    now_color=GREEN;
                }
                else if(choose_color==3){
                    DrawRectangle(monsters[i].x , monsters[i].y, CELL_SIZE * monsters[i].width, CELL_SIZE * monsters[i].height, ORANGE);
                    now_color=ORANGE;
                }
                else if(choose_color==4){
                    DrawRectangle(monsters[i].x , monsters[i].y, CELL_SIZE * monsters[i].width, CELL_SIZE * monsters[i].height, YELLOW);
                    now_color=YELLOW;
                }
                else if(choose_color==5){
                    DrawRectangle(monsters[i].x , monsters[i].y, CELL_SIZE * monsters[i].width, CELL_SIZE * monsters[i].height, BLUE);
                    now_color=BLUE;
                }
                else if(choose_color==6){
                    DrawRectangle(monsters[i].x, monsters[i].y, CELL_SIZE * monsters[i].width, CELL_SIZE * monsters[i].height, PURPLE);
                    now_color=PURPLE;
                }

            }
        }
    }
}

// 檢查玩家是否與怪物相撞
void check_collision(Hero *hero) {
    static int invincibility_timer = 0; // 無敵時間計數器
    
    // 如果玩家處於無敵狀態，減少計時器並返回
    if (invincibility_timer > 0) {
        invincibility_timer--;
        return;
    }

    for (int i = 0; i < current_monster_count; i++) 
    {
        if (monsters[i].is_active) 
        {
            // 檢查玩家與2x2怪物的碰撞
            bool collision = false;
            bool result=CheckCollsionBoxes(hero->box,monsters[i].box);
            if(result==1) collision=true;
    

            if (collision) 
            {
                // 怪物受到傷害
                monsters[i].health--;

                 // 設置碰撞後的無敵時間
                invincibility_timer = 60;
                
                // 玩家受到傷害 (新增)
                hero->hp-=2;

                
                // 如果玩家生命值歸零，直接返回
                if (hero->hp <= 0) {
                    return;
                }
                
                // 僅當怪物生命值為0或更低時才爆炸
                if (monsters[i].health <= 0) {
                    // 在怪物生命0時創建爆炸
                    create_explosion(monsters[i].x, monsters[i].y, monsters[i].color);
                    draw_explosions();
                    // 標記怪物為非活躍
                    monsters[i].is_active = 0;
                    active_monster_count--;
                    
                // 增加死亡怪物計數
                    dead_monster_count++;
                    
                }

                break;
            }
        }
    } 
}


void get_demage(Hero* hero) {
    for(int i=0; i<current_monster_count; i++) {
        if(monsters[i].is_active) {
            monsters[i].health -= CheckDemage(&monsters[i].box, hero);

            if(monsters[i].health <= 0) {
                // 在怪物生命0時創建爆炸
                create_explosion(monsters[i].x, monsters[i].y, monsters[i].color);
                draw_explosions();
                // 標記怪物為非活躍
                monsters[i].is_active = 0;
                active_monster_count--;

                //增加角色經驗值
                hero->exp += 2;
                
                // 增加死亡怪物計數
                dead_monster_count++;

                //檢查寶箱
                Treasure_OnMonsterKilled(hero);
            }
        }
    }
}



