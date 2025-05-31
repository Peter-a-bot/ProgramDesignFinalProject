#include <stdio.h>
#include <raylib.h>
#include"hero.h"
#include"basic_monster.h"
#include"collisionbox.h"
#include "monster.h"
#include"shooter_monster.h"

#define SHOTER_MONSTER_SPAWN_INTERVAL 5.0f;

double shooter_monster_next_spawn_time = 3.0f;

ShooterMonster shooter_monsters[MAX_SHOOTER_MONSTERS];
Projectile projectiles[MAX_PROJECTILES];
int current_shooter_count = 0;  // 當前射擊怪物數量

int dead_shooter_monster_count = 0; // 當前死亡的射擊怪物數量

Color PURPLE_BLUE = (Color){ 120, 80, 255, 255 }; // 紫藍色

// 初始化射擊型怪物
void init_shooter_monsters() {
    for (int i = 0; i < MAX_SHOOTER_MONSTERS; i++) {
        shooter_monsters[i].is_active = 0;
        shooter_monsters[i].cooldown = 0;
    }
}

// 初始化射擊物
void init_projectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectiles[i].is_active = false;
    }
}

// // 創建新射擊物
// void create_projectile(int x, int y, int direction, Color color) {
//     for (int i = 0; i < MAX_PROJECTILES; i++) {
//         if (!projectiles[i].is_active) {
//             projectiles[i].x = x;
//             projectiles[i].y = y;
//             projectiles[i].direction = direction;
//             projectiles[i].range_left = PROJECTILE_RANGE; // 設為10格射程
//             projectiles[i].is_active = true;
//             projectiles[i].color = PURPLE_BLUE; // 固定使用紫藍色
//             break;
//         }
//     }
// }

// 創建新射擊物
// void create_projectile(int x, int y, int direction, Color color) {
//     for (int i = 0; i < MAX_PROJECTILES; i++) {
//         if (!projectiles[i].is_active) {
//             projectiles[i].position.x=x;
//             projectiles[i].position.y=y;
//             // projectiles[i].x = x;
//             // projectiles[i].y = y;
//             projectiles[i].direction = direction;
//             projectiles[i].range_left = PROJECTILE_RANGE; // 設為10格射程
//             projectiles[i].is_active = true;
//             projectiles[i].color = PURPLE_BLUE; // 固定使用紫藍色

//             projectiles[i].box.rec.x=projectiles[i].position.x;
//             projectiles[i].box.rec.y=projectiles[i].position.y;
//             projectiles[i].box.rotationAngle=0.0f;
//             printf("Create projectile at (%d,%d)\n", projectiles[i].position.x, projectiles[i].position.y);
//             break;
//         }
//     }
// }

void spawn_shooter_monster(Hero *hero) {
    game_time += GetFrameTime();
    if (game_time >= shooter_monster_next_spawn_time) {// 檢查是否需要生成射擊型怪物
        shooter_monster_next_spawn_time += SHOTER_MONSTER_SPAWN_INTERVAL;
        if (current_shooter_count < MAX_SHOOTER_MONSTERS) {
            random_empty_position(hero);
            
            for (int i = 0; i < MAX_SHOOTER_MONSTERS; i++) {
                if (!shooter_monsters[i].is_active) {
                    Vector2 position = random_empty_position(hero);
                    shooter_monsters[i].x = position.x;
                    shooter_monsters[i].y = position.y;
                    shooter_monsters[i].is_active = 1;
                    shooter_monsters[i].color = ORANGE;  // 射擊怪物使用橙色
                    shooter_monsters[i].cooldown = 30;   // 初始冷卻時間
                    shooter_monsters[i].width=2;
                    shooter_monsters[i].height=2;
                    shooter_monsters[i].health=300;
                    current_shooter_count++;
    
                    shooter_monsters[i].box.rec.x=shooter_monsters[i].x;
                    shooter_monsters[i].box.rec.y=shooter_monsters[i].y;
                    shooter_monsters[i].box.rec.width=shooter_monsters[i].width * CELL_SIZE;
                    shooter_monsters[i].box.rec.height=shooter_monsters[i].height * CELL_SIZE;
                    
                    return;
                }
            }
        }

        // dead_monster_count = 0;  // 重置計數器
    }
     
}

// 移動射擊型怪物和處理射擊
// void update_shooter_monsters(Hero *hero) {
//     for (int i = 0; i < MAX_SHOOTER_MONSTERS; i++) {
//         if (!shooter_monsters[i].is_active) continue;
        
//         // 降低移動頻率
//         if (rand() % 100 >= MONSTER_MOVE_CHANCE) {
//             // continue;

//             int monster_x = shooter_monsters[i].x;
//             int monster_y = shooter_monsters[i].y;
            
//             // 簡單追蹤玩家行為
//             if (monster_x < hero->position.x) shooter_monsters[i].x++;
//             else if (monster_x > hero->position.x) shooter_monsters[i].x--;
            
//             if (monster_y < hero->position.y) shooter_monsters[i].y++;
//             else if (monster_y > hero->position.y) shooter_monsters[i].y--;
            
//             // 檢查新位置是否有效
//             // int valid = 1;
//             // for (int dx = 0; dx < 2 && valid; dx++) {
//             //     for (int dy = 0; dy < 2 && valid; dy++) {
//             //         int check_x = shooter_monsters[i].x + dx;
//             //         int check_y = shooter_monsters[i].y + dy;
                    
//             //         if (check_x < 0 || check_y < 0 || check_x >= MAP_HEIGHT || check_y >= MAP_WIDTH || map[check_x][check_y] == 1) {
//             //             valid = 0;
//             //         }
//             //     }
//             // }
            
//             // if (!valid) {
//             //     shooter_monsters[i].x = monster_x;  // 恢復原始位置
//             //     shooter_monsters[i].y = monster_y;
//             // }
//         }
        
//         //更新碰撞箱
//         shooter_monsters[i].box.rec.x=shooter_monsters[i].x;
//         shooter_monsters[i].box.rec.y=shooter_monsters[i].y;
//         shooter_monsters[i].box.rec.width=shooter_monsters[i].width * CELL_SIZE;
//         shooter_monsters[i].box.rec.height=shooter_monsters[i].height * CELL_SIZE;
        
//     //     if (shooter_monsters[i].cooldown == 0 && (rand() % 100 > SHOOTER_FIRE_CHANCE) ) {
//     //      if(shooter_monsters[i].cooldown <= 0) {
//     //          // printf("In the checkpoint 1\n");

//          //     for (int dir = 0; dir < 4; dir++) {
//          //         // printf("In the check point 2\n");
//          //         create_projectile(shooter_monsters[i].x, shooter_monsters[i].y, dir, PURPLE_BLUE);
//          //     }

//          // // 減少冷卻時間
//          // if (shooter_monsters[i].cooldown > 0) {
//          //     shooter_monsters[i].cooldown--;
//          // }
//     //     }   
//     // }

//          // 減少冷卻時間
//          if (shooter_monsters[i].cooldown > 0) {
//              shooter_monsters[i].cooldown--;
//          }

//          if(shooter_monsters[i].cooldown==0){
//              for (int dir = 0; dir < 4; dir++) {
//                  // printf("In the check point 2\n");
//                  create_projectile(shooter_monsters[i].x, shooter_monsters[i].y, dir, PURPLE_BLUE);

                 
//              }
//         shooter_monsters[i].cooldown = 30;             
//          }
//        }
// }

//創建指定半徑的爆炸(射擊型怪物)
void create_explosion_with_radius(int x, int y, Color color, int radius) {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (!explosions[i].is_active) {
            explosions[i].x = x;
            explosions[i].y = y;
            explosions[i].frame = 0;
            explosions[i].is_active = true;
            explosions[i].color = color;
            explosions[i].radius = radius; // 添加爆炸半徑屬性

            break;
        }
    }
}


// 檢查玩家是否在爆炸範圍內
bool check_player_in_explosion_radius(int explosion_x, int explosion_y, int radius,Hero *hero) {
    Vector2 circle;
    circle.x=explosion_x;
    circle.y=explosion_y;
    bool result=CheckCollisionBoxAndCircle(hero->box,circle,6);// 檢查玩家是否有在爆炸範圍內
    if(result==1) return true;// 玩家在爆炸範圍內
    else return false;// 玩家不在爆炸範圍內
}


// 火焰射擊物更新函數
// void update_projectiles(Hero *hero) {
//     for (int i = 0; i < MAX_PROJECTILES; i++) {
//         if (!projectiles[i].is_active) continue;
        
//         // 更新位置
//         switch(projectiles[i].direction) {
//             case 0: // 上
//                 projectiles[i].position.x--;
//                 break;
//             case 1: // 下
//                 projectiles[i].position.x++;
//                 break;
//             case 2: // 左
//                 projectiles[i].position.y--;
//                 break;
//             case 3: // 右
//                 projectiles[i].position.y++;
//                 break;
//         }

//         //更新碰撞箱
//         projectiles[i].box.rec.x=projectiles[i].position.x;
//         projectiles[i].box.rec.y=projectiles[i].position.y;
        
//         // 減少剩餘射程
//         projectiles[i].range_left--;

//         // 檢查是否超出地圖或撞到障礙物或射程耗盡
//         bool should_explode = false;
        
//         // 檢查是否超出地圖或撞到障礙物
//         // if (projectiles[i].x < 0 || projectiles[i].y < 0 || 
//         //     projectiles[i].x >= MAP_HEIGHT || projectiles[i].y >= MAP_WIDTH || 
//         //     map[projectiles[i].x][projectiles[i].y] == 1 || 
//         //     projectiles[i].range_left <= 0) {
            
//         //         should_explode = true;
//         // }
//         //

//         if(projectiles[i].range_left <= 0) {
//             should_explode = true;
//         }
        
//         // 檢查是否擊中玩家（考慮2x2玩家大小）
//         bool hit_player = false;
//         // for (int px = 0; px < 2 && !hit_player; px++) {
//         //     for (int py = 0; py < 2 && !hit_player; py++) {
//         //         if (hero->position.x + px == projectiles[i].x && hero->position.y + py == projectiles[i].y) {
//         //             hit_player = true;
//         //             should_explode = true;
//         //         }
//         //     }
//         // }
//         bool result=CheckCollisionBoxAndCircle(hero->box, projectiles[i].position,3);
//         if(result==1){
//             hit_player = true;
//             should_explode = true;
//         }

//         // 如果需要爆炸
//         if (should_explode) {
//             // 創建鮮豔紫藍色爆炸效果
//             create_explosion_with_radius(projectiles[i].x, projectiles[i].y, PURPLE_BLUE, 6);
//             projectiles[i].is_active = false;
            
//             // 檢查玩家是否在爆炸範圍內
//             bool player_in_explosion = check_player_in_explosion_radius(projectiles[i].x, projectiles[i].y, 6,hero);
//             if (hit_player || player_in_explosion) {
//                 hero->hp-=50;
                
//                 // 如果生命值耗盡，直接返回
//                 if (hero->hp <= 0) return;
//             }
            
//             continue;
//         }
//     }
// }

// 绘制射击型怪物
void draw_shooter_monsters() {
    for (int i = 0; i < MAX_SHOOTER_MONSTERS; i++) {
        if (shooter_monsters[i].is_active) {
            // 绘制主体为橙色的怪物，有特殊标记
            DrawRectangle(shooter_monsters[i].x, shooter_monsters[i].y, 
                        CELL_SIZE * shooter_monsters[i].width, CELL_SIZE * shooter_monsters[i].height, ORANGE);
            
            // 增加特殊标记（例如中间有个圆形）
            DrawCircle(shooter_monsters[i].x, 
                    shooter_monsters[i].y, 
                    CELL_SIZE * 0.6f, RED);
        }
    }
}



// 绘制射击物（火焰）
// void draw_projectiles() {
//     for (int i = 0; i < MAX_PROJECTILES; i++) {
//         if (projectiles[i].is_active) {
//             // 火焰主体 - 紫蓝色
//             Color flame = PURPLE_BLUE;
//             // 根据射程剩余量调整透明度
//             flame.a = 180 + (unsigned char)(75.0f * projectiles[i].range_left / PROJECTILE_RANGE);
            
//             // 绘制火焰效果 - 主体为紫蓝色，半径为3
//             DrawCircle(projectiles[i].x * CELL_SIZE + CELL_SIZE/2,
//                     projectiles[i].y * CELL_SIZE +  CELL_SIZE/2,
//                     CELL_SIZE * 0.3f * 3, flame); // 半径为3
                    
//             // 火焰内核 - 较亮的紫蓝色
//             Color inner_flame = (Color){ 150, 120, 255, 255 };
//             DrawCircle(projectiles[i].x * CELL_SIZE + CELL_SIZE/2,
//                     projectiles[i].y * CELL_SIZE +  CELL_SIZE/2,
//                     CELL_SIZE * 0.2f * 3, inner_flame);
            
//             // 火焰中心 - 最亮
//             Color core = (Color){ 200, 180, 255, 255 };
//             DrawCircle(projectiles[i].x * CELL_SIZE + CELL_SIZE/2,
//                     projectiles[i].y * CELL_SIZE +  CELL_SIZE/2,
//                     CELL_SIZE * 0.1f * 3, core);
                    
//             // 添加火焰方向指示
//             int tail_x = projectiles[i].x;
//             int tail_y = projectiles[i].y;
            
//             // 根据方向绘制火焰尾巴
//             switch(projectiles[i].direction) {
//                 case 0: tail_x += 1; break; // 上
//                 case 1: tail_x -= 1; break; // 下
//                 case 2: tail_y += 1; break; // 左
//                 case 3: tail_y -= 1; break; // 右
//             }
            
//             // 绘制尾巴
//             // if (tail_x >= 0 && tail_y >= 0 && tail_x < MAP_HEIGHT && tail_y < MAP_WIDTH) {
//             //     Color tail = PURPLE_BLUE;
//             //     tail.a = 100;
//             //     DrawCircle(tail_y * CELL_SIZE + CELL_SIZE/2,
//             //             tail_x * CELL_SIZE + CELL_SIZE/2,
//             //             CELL_SIZE * 0.15f * 3, tail);
//             // }
//         }
//     }
// }

void check_shooter_monster_collision(Hero *hero) {
    static int invincibility_timer = 0; // 無敵時間計數器
    
    // 如果玩家處於無敵狀態，減少計時器並返回
    if (invincibility_timer > 0) {
        invincibility_timer--;
        return;
    }

    for (int i = 0; i < current_monster_count; i++) 
    {
        if (shooter_monsters[i].is_active) 
        {
            // 檢查玩家與2x2怪物的碰撞
            bool collision = false;
            bool result=CheckCollsionBoxes(hero->box,shooter_monsters[i].box);
            if(result==1) collision=true;
    

            if (collision) 
            {
                // 怪物受到傷害
                shooter_monsters[i].health--;

                 // 設置碰撞後的無敵時間
                invincibility_timer = 60;
                
                // 玩家受到傷害 (新增)
                hero->hp-=2;

                
                // 如果玩家生命值歸零，直接返回
                if (hero->hp <= 0) {
                    return;
                }
                
                // 僅當怪物生命值為0或更低時才爆炸
                if (shooter_monsters[i].health <= 0) {
                    // 在怪物生命0時創建爆炸
                    create_explosion(shooter_monsters[i].x, shooter_monsters[i].y, shooter_monsters[i].color);
                    draw_explosions();
                    // 標記怪物為非活躍
                    shooter_monsters[i].is_active = 0;
                    current_shooter_count--;
                    
                // 增加死亡怪物計數
                    dead_shooter_monster_count++;
                    
                }

                break;
            }
        }
    } 
}

// void get_shooter_monster_demage(Hero* hero) {
//     for(int i=0; i<current_monster_count; i++) {
//         if(shooter_monsters[i].is_active) {
//             shooter_monsters[i].health -= CheckDemage(&shooter_monsters[i].box, hero);

//             if(shooter_monsters[i].health <= 0) {
//                 // 在怪物生命0時創建爆炸
//                 create_explosion(shooter_monsters[i].x, shooter_monsters[i].y, shooter_monsters[i].color);
//                 draw_explosions();
//                 // 標記怪物為非活躍
//                 shooter_monsters[i].is_active = 0;
//                 current_shooter_count--;

//                 //增加角色經驗值
//                 hero->exp += 2;
                
//                 // 增加死亡怪物計數
//                 dead_shooter_monster_count++;

//             }
//         }
//         else {
//             break;
//         }
//     }
// }

void get_shooter_monster_demage(Hero* hero) {
    for(int i = 0; i < MAX_SHOOTER_MONSTERS; i++) {
        if(!shooter_monsters[i].is_active) continue;  // 跳過非活躍怪物，但繼續檢查
        
        shooter_monsters[i].health -= CheckDemage(&shooter_monsters[i].box, hero);

        if(shooter_monsters[i].health <= 0) {
            create_explosion(shooter_monsters[i].x, shooter_monsters[i].y, shooter_monsters[i].color);
            shooter_monsters[i].is_active = 0;
            current_shooter_count--;
            hero->exp += 2;
            dead_shooter_monster_count++;
        }
    }
}

// 修正後的 create_projectile 函數
void create_projectile(int x, int y, int direction, Color color) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].is_active) {
            // 使用統一的座標系統
            projectiles[i].position.x = x;
            projectiles[i].position.y = y;
            projectiles[i].direction = direction;
            projectiles[i].range_left = PROJECTILE_RANGE;
            projectiles[i].is_active = true;
            projectiles[i].color = PURPLE_BLUE;

            // 設定碰撞箱
            projectiles[i].box.rec.x = projectiles[i].position.x * CELL_SIZE;
            projectiles[i].box.rec.y = projectiles[i].position.y * CELL_SIZE;
            projectiles[i].box.rec.width = CELL_SIZE;
            projectiles[i].box.rec.height = CELL_SIZE;
            projectiles[i].box.rotationAngle = 0.0f;
            
            printf("Create projectile at (%d,%d) direction: %d\n", 
                   (int)projectiles[i].position.x, (int)projectiles[i].position.y, direction);
            break;
        }
    }
}

// 修正後的 update_projectiles 函數
void update_projectiles(Hero *hero) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].is_active) continue;
        
        // 修正方向移動邏輯
        switch(projectiles[i].direction) {
            case 0: // 上
                projectiles[i].position.y--;
                break;
            case 1: // 下
                projectiles[i].position.y++;
                break;
            case 2: // 左
                projectiles[i].position.x--;
                break;
            case 3: // 右
                projectiles[i].position.x++;
                break;
        }

        // 更新碰撞箱
        projectiles[i].box.rec.x = projectiles[i].position.x * CELL_SIZE;
        projectiles[i].box.rec.y = projectiles[i].position.y * CELL_SIZE;
        
        // 減少剩餘射程
        projectiles[i].range_left--;

        bool should_explode = false;
        
        // 檢查射程耗盡
        if(projectiles[i].range_left <= 0) {
            should_explode = true;
        }
        
        // 檢查地圖邊界和障礙物
        // if (projectiles[i].position.x < 0 || projectiles[i].position.y < 0 || 
        //     projectiles[i].position.x >= MAP_WIDTH || projectiles[i].position.y >= MAP_HEIGHT) {
        //     should_explode = true;
        // }
        
        // 檢查是否撞到地圖障礙物 (如果有地圖數組的話)
        // if (map[(int)projectiles[i].position.y][(int)projectiles[i].position.x] == 1) {
        //     should_explode = true;
        // }
        
        // 檢查是否擊中玩家
        bool hit_player = false;
        bool result = CheckCollisionBoxAndCircle(hero->box, projectiles[i].position, 3);
        if(result == 1){
            hit_player = true;
            should_explode = true;
        }

        // 如果需要爆炸
        if (should_explode) {
            // 使用正確的座標創建爆炸
            create_explosion_with_radius((int)projectiles[i].position.x, 
                                       (int)projectiles[i].position.y, 
                                       PURPLE_BLUE, 6);
            projectiles[i].is_active = false;
            
            // 檢查玩家是否在爆炸範圍內
            if (hit_player) {
                hero->hp -= 2;
                printf("Player hit by projectile! HP: %d\n", hero->hp);
                
                // 如果生命值耗盡，直接返回
                if (hero->hp <= 0) return;
            }
        }
    }
}

// 修正後的 draw_projectiles 函數
void draw_projectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].is_active) {
            // 使用正確的座標繪製
            int screen_x = (int)projectiles[i].position.x * CELL_SIZE + CELL_SIZE/2;
            int screen_y = (int)projectiles[i].position.y * CELL_SIZE + CELL_SIZE/2;
            
            // 火焰主體 - 紫藍色
            Color flame = PURPLE_BLUE;
            // 根據射程剩餘量調整透明度
            flame.a = 180 + (unsigned char)(75.0f * projectiles[i].range_left / PROJECTILE_RANGE);
            
            // 繪製火焰效果
            DrawCircle(screen_x, screen_y, CELL_SIZE * 0.3f * 3, flame);
                    
            // 火焰內核
            Color inner_flame = (Color){ 150, 120, 255, 255 };
            DrawCircle(screen_x, screen_y, CELL_SIZE * 0.2f * 3, inner_flame);
            
            // 火焰中心
            Color core = (Color){ 200, 180, 255, 255 };
            DrawCircle(screen_x, screen_y, CELL_SIZE * 0.1f * 3, core);
            // 添加火焰方向指示
            int tail_x = projectiles[i].x;
            int tail_y = projectiles[i].y;
            
            // 根据方向绘制火焰尾巴
            switch(projectiles[i].direction) {
                case 0: tail_x += 1; break; // 上
                case 1: tail_x -= 1; break; // 下
                case 2: tail_y += 1; break; // 左
                case 3: tail_y -= 1; break; // 右
            }
            
            // 绘制尾巴
            if (tail_x >= 0 && tail_y >= 0) {
                Color tail = PURPLE_BLUE;
                tail.a = 100;
                DrawCircle(tail_y * CELL_SIZE + CELL_SIZE/2,
                        tail_x * CELL_SIZE + CELL_SIZE/2,
                        CELL_SIZE * 0.15f * 3, tail);
            }
        }
        
    }
    
}

// 確保射擊怪物正確生成射擊物
void update_shooter_monsters(Hero *hero) {
    for (int i = 0; i < MAX_SHOOTER_MONSTERS; i++) {
        if (!shooter_monsters[i].is_active) continue;
        
        // 移動邏輯 (降低移動頻率)
        if (rand() % 100 < MONSTER_MOVE_CHANCE) {  // 注意這裡改成 < 號
            int monster_x = shooter_monsters[i].x;
            int monster_y = shooter_monsters[i].y;
            
            // 簡單追蹤玩家行為
            if (monster_x < hero->position.x) shooter_monsters[i].x++;
            else if (monster_x > hero->position.x) shooter_monsters[i].x--;
            
            if (monster_y < hero->position.y) shooter_monsters[i].y++;
            else if (monster_y > hero->position.y) shooter_monsters[i].y--;
        }
        
        // 更新碰撞箱
        shooter_monsters[i].box.rec.x = shooter_monsters[i].x;
        shooter_monsters[i].box.rec.y = shooter_monsters[i].y;
        shooter_monsters[i].box.rec.width = shooter_monsters[i].width * CELL_SIZE;
        shooter_monsters[i].box.rec.height = shooter_monsters[i].height * CELL_SIZE;
        
        // 減少冷卻時間
        if (shooter_monsters[i].cooldown > 0) {
            shooter_monsters[i].cooldown--;
        }

        // 射擊邏輯
        if(shooter_monsters[i].cooldown == 0){
            printf("Shooter monster %d firing at position (%d,%d)\n", i, shooter_monsters[i].x, shooter_monsters[i].y);
              // 計算怪物中心位置（轉換為格子座標）
            int center_x = shooter_monsters[i].x + shooter_monsters[i].width * CELL_SIZE/2;
            int center_y = shooter_monsters[i].y + shooter_monsters[i].height * CELL_SIZE/2;
              
            // 向四個方向發射
            for (int dir = 0; dir < 4; dir++) {
                // create_projectile(shooter_monsters[i].x, shooter_monsters[i].y, dir, PURPLE_BLUE);
                create_projectile(center_x, center_y,dir,PURPLE_BLUE);
            }
            
            // 重設冷卻時間
            shooter_monsters[i].cooldown = 30;             
        }
    }
}
