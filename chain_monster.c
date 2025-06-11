#include <stdio.h>
#include <raylib.h>
#include "hero.h"
#include "collisionbox.h"
#include"basic_monster.h"
//#include "monster.h"
#include "chain_monster.h"

#define CHAIN_MONSTER_SPAWN_INTERVAL 5.0f;

double chain_monster_next_spawn_time = 3.0f;

ChainMonster chain_monsters[MAX_CHAIN_MONSTERS];
Explosion_Chain chain_explosions[MAX_CHAIN_EXPLOSIONS];  // 爆炸數組
Chain chain[MAX_CHAIN_NUM];
const int chain_heath_up = 1000;
int current_chain_count = 0;  // 當前鎖鏈怪物數量

int dead_chain_monster_count = 0; // 當前死亡的鎖鏈怪物數量

Color PURPLE_BLUE_CHAIN = (Color){ 120, 80, 254, 254 }; // 紫藍色
Color STONE_BASE_COLOR = (Color){ 100, 100, 100, 255 }; // 中灰色，作為石頭主要顏色
Color STONE_DETAIL_COLOR = (Color){ 80, 80, 80, 255 }; // 稍深一點的灰色，用於細節或陰影

Color DarkenColor(Color original, float factor) {
    return (Color) {
    (unsigned char)(original.r * factor),
    (unsigned char)(original.g * factor),
    (unsigned char)(original.b * factor),
    255
    };
}

// 初始化射擊型怪物
void init_chain_monsters() {
    for (int i = 0; i < MAX_CHAIN_MONSTERS; i++) {
        chain_monsters[i].is_active = 0;
        chain_monsters[i].cooldown = 0;
    }
}

// 初始化射擊物
void init_chain() {
    for (int i = 0; i < MAX_CHAIN_NUM; i++) {
        chain[i].is_active = false;
    }
}

void spawn_chain_monster(Hero *hero,int map_type) {
    game_time += GetFrameTime();
    if (game_time >= chain_monster_next_spawn_time) {// 檢查是否需要生成射擊型怪物
        chain_monster_next_spawn_time += CHAIN_MONSTER_SPAWN_INTERVAL;
        if (current_chain_count < MAX_CHAIN_MONSTERS) {
            //random_empty_position(hero);
            
            for (int i = 0; i < MAX_CHAIN_MONSTERS; i++) {
                if (!chain_monsters[i].is_active) {
                    Vector2 position = random_empty_position(hero);
                    chain_monsters[i].x = position.x;
                    chain_monsters[i].y = position.y;
                    chain_monsters[i].is_active = 1;

                    chain_monsters[i].color = (Color) { GetRandomValue(50, 255), GetRandomValue(50, 255), GetRandomValue(50, 255), 255};
                    
                    chain_monsters[i].cooldown = 30;   // 初始冷卻時間
                    chain_monsters[i].width=2;
                    chain_monsters[i].height=2;
                    chain_monsters[i].health = 800 + map_type*chain_heath_up;   //設置血量，依地圖而變 
                    current_chain_count++;
    
                    chain_monsters[i].box.rec.x=chain_monsters[i].x;
                    chain_monsters[i].box.rec.y=chain_monsters[i].y;
                    chain_monsters[i].box.rec.width=chain_monsters[i].width * CELL_SIZE;
                    chain_monsters[i].box.rec.height=chain_monsters[i].height * CELL_SIZE;
                    
                    return;
                }
            }
        }

        // dead_monster_count = 0;  // 重置計數器
    }
     
}

//創建指定半徑的爆炸(鎖鏈怪物)
void create_explosion_with_radius_chain(int x, int y, Color color, int radius) {
    for (int i = 0; i < MAX_CHAIN_EXPLOSIONS; i++) {
        if (!chain_explosions[i].is_active) {
            chain_explosions[i].x = x;
            chain_explosions[i].y = y;
            chain_explosions[i].frame = 0;
            chain_explosions[i].is_active = true;
            chain_explosions[i].color = color;
            chain_explosions[i].radius = radius; // 添加爆炸半徑屬性

            break;
        }
    }
}


// 檢查玩家是否在爆炸範圍內
bool check_player_in_explosion_radius_chain(int explosion_x, int explosion_y, int radius,Hero *hero) {
    Vector2 circle;
    circle.x=explosion_x;
    circle.y=explosion_y;
    bool result=CheckCollisionBoxAndCircle(hero->box,circle,6);// 檢查玩家是否有在爆炸範圍內
    if(result==1) return true;// 玩家在爆炸範圍內
    else return false;// 玩家不在爆炸範圍內
}

// 繪製鎖鏈怪物
void draw_chain_monsters() {
    for (int i = 0; i < MAX_CHAIN_MONSTERS; i++) {
        if (chain_monsters[i].is_active) {
            Color monster_base_color = chain_monsters[i].color;
            Color monster_detail_color = DarkenColor(monster_base_color, 0.7f);
            
            DrawRectangle(chain_monsters[i].x, chain_monsters[i].y, 
                        CELL_SIZE * chain_monsters[i].width, CELL_SIZE * chain_monsters[i].height, monster_base_color);
            
            DrawCircle(chain_monsters[i].x, 
                    chain_monsters[i].y, 
                    CELL_SIZE * 0.6f, monster_detail_color);

            DrawCircle(chain_monsters[i].x, 
                    chain_monsters[i].y+CELL_SIZE, 
                    CELL_SIZE * 0.6f, monster_detail_color);

            DrawCircle(chain_monsters[i].x+CELL_SIZE, 
                    chain_monsters[i].y+CELL_SIZE, 
                    CELL_SIZE * 0.6f, monster_detail_color);

            DrawCircle(chain_monsters[i].x+CELL_SIZE, 
                    chain_monsters[i].y, 
                    CELL_SIZE * 0.6f, monster_detail_color);
        }
    }
}

void check_chian_monster_collision(Hero *hero) {
    static int invincibility_timer = 0; // 無敵時間計數器
    
    // 如果玩家處於無敵狀態，減少計時器並返回
    if (invincibility_timer > 0) {
        invincibility_timer--;
        return;
    }

    for (int i = 0; i < current_chain_count; i++) 
    {
        if (chain_monsters[i].is_active) 
        {
            // 檢查玩家與2x2怪物的碰撞
            bool collision = false;
            bool result=CheckCollsionBoxes(hero->box,chain_monsters[i].box);
            if(result==1) collision=true;
    

            if (collision) 
            {
                // 怪物受到傷害
                chain_monsters[i].health-=10;

                 // 設置碰撞後的無敵時間
                invincibility_timer = 60;
                
                // 玩家受到傷害 (新增)
                hero->hp-=2;

                
                // 如果玩家生命值歸零，直接返回
                if (hero->hp <= 0) {
                    return;
                }
                
                // 僅當怪物生命值為0或更低時才爆炸
                if (chain_monsters[i].health <= 0) {
                    // 在怪物生命0時創建爆炸
                    create_explosion(chain_monsters[i].x, chain_monsters[i].y, chain_monsters[i].color);
                    draw_explosions();
                    // 標記怪物為非活躍
                    chain_monsters[i].is_active = 0;
                    current_chain_count--;
                    
                // 增加死亡怪物計數
                    dead_chain_monster_count++;
                    
                }

                break;
            }
        }
    } 
}

void get_chain_monster_demage(Hero* hero) {
    for(int i = 0; i < MAX_CHAIN_MONSTERS; i++) {
        if(!chain_monsters[i].is_active) continue;  // 跳過非活躍怪物，但繼續檢查
        
        chain_monsters[i].health -= CheckDemage(&chain_monsters[i].box, hero);

        if(chain_monsters[i].health <= 0) {
            create_explosion(chain_monsters[i].x, chain_monsters[i].y, chain_monsters[i].color);
            chain_monsters[i].is_active = 0;
            current_chain_count--;
            hero->exp += 2;
            dead_chain_monster_count++;
        }
    }
}

// 創造鎖鏈
void create_chain(int x, int y, int direction, Color color) {
    for (int i = 0; i < MAX_CHAIN_NUM; i++) {
        if (!chain[i].is_active) {
            // 使用統一的座標系統
            chain[i].position.x = x;
            chain[i].position.y = y;
            chain[i].direction = direction;
            chain[i].range_left = CHAIN_RANGE;
            chain[i].is_active = true;
            chain[i].color = WHITE;

            // 設定碰撞箱
            chain[i].box.rec.x = chain[i].position.x * CELL_SIZE;
            chain[i].box.rec.y = chain[i].position.y * CELL_SIZE;
            chain[i].box.rec.width = CELL_SIZE;
            chain[i].box.rec.height = CELL_SIZE;
            chain[i].box.rotationAngle = 0.0f;
            
            printf("Create projectile at (%d,%d) direction: %d\n", 
                   (int)chain[i].position.x, (int)chain[i].position.y, direction);
            break;
        }
    }
}

//更新鎖鏈
void update_chain(Hero *hero) {
    for (int i = 0; i < MAX_CHAIN_NUM; i++) {
        if (!chain[i].is_active) continue;
        
        // 修正方向移動邏輯
        switch(chain[i].direction) {
            case 0: // 上
                chain[i].position.y--;
                break;
            case 1: // 下
                chain[i].position.y++;
                break;
            case 2: // 左
                chain[i].position.x--;
                break;
            case 3: // 右
                chain[i].position.x++;
                break;
        }

        // 更新碰撞箱
        chain[i].box.rec.x = chain[i].position.x * CELL_SIZE;
        chain[i].box.rec.y = chain[i].position.y * CELL_SIZE;
        
        // 減少剩餘射程
        chain[i].range_left--;

        bool should_explode = false;
        
        // 檢查射程耗盡
        if(chain[i].range_left <= 0) {
            should_explode = true;
        }
        
        // 檢查是否擊中玩家
        bool hit_player = false;
        bool result = CheckCollisionBoxAndCircle(hero->box, chain[i].position, 3);
        if(result == 1){
            hit_player = true;
            should_explode = true;
        }

        // 如果需要爆炸
        if (should_explode) {
            // 使用正確的座標創建爆炸
            //
            Color explotion_color = (Color) { GetRandomValue(50, 255), GetRandomValue(50, 255), GetRandomValue(50, 255), 255};
             
            create_explosion_with_radius((int)chain[i].position.x, 
                                       (int)chain[i].position.y, 
                                       explotion_color, 6);
            chain[i].is_active = false;
            
            // 檢查玩家是否在爆炸範圍內
            if (hit_player) {
                hero->hp -= 2;
                printf("Player hit by chain! HP: %d\n", hero->hp);
                
                // 如果生命值耗盡，直接返回
                if (hero->hp <= 0) return;
            }
        }
    }
}

//繪製鎖鏈
void draw_chain() {
    for (int i = 0; i < MAX_CHAIN_NUM; i++) {
        if (chain[i].is_active) {
            // 使用正確的座標繪製
            int screen_x = (int)chain[i].position.x+ CELL_SIZE/2;
            int screen_y = (int)chain[i].position.y+ CELL_SIZE/2;
            

            //計算爆炸透明度
            float alpha_ratio = chain[i].range_left / (float)CHAIN_RANGE;
            unsigned char alpha_outer = (unsigned char)(180.0f * alpha_ratio);
            unsigned char alpha_middle = (unsigned char)(220.0f * alpha_ratio);

           
            // 繪製火焰效果
            Color flame = (Color){80, 0, 120, alpha_outer};
            DrawCircle(screen_x, screen_y, CELL_SIZE * 0.3f * 3, flame);
                    
            // 火焰內核
            Color inner_flame = (Color){ 0, 255, 0, alpha_middle};
            DrawCircle(screen_x, screen_y, CELL_SIZE * 0.2f * 3, inner_flame);
            
            // 火焰中心
            Color core = (Color){ 200, 255, 0, 255 };
            DrawCircle(screen_x, screen_y, CELL_SIZE * 0.1f * 3, core);
            
        }
        
    }
    
}

//更新鎖鏈怪物
void update_chain_monsters(Hero *hero) {
    for (int i = 0; i < MAX_CHAIN_MONSTERS; i++) {
        if (!chain_monsters[i].is_active) continue;
        
        // 移動邏輯 
        if (rand() % 100 < MONSTER_MOVE_CHANCE) {  //(降低移動頻率)
            int monster_x = chain_monsters[i].x;
            int monster_y = chain_monsters[i].y;
            
            // 簡單追蹤玩家行為
            if (monster_x < hero->position.x) chain_monsters[i].x++;
            else if (monster_x > hero->position.x) chain_monsters[i].x--;
            
            if (monster_y < hero->position.y) chain_monsters[i].y++;
            else if (monster_y > hero->position.y) chain_monsters[i].y--;
        }
        
        // 更新碰撞箱
        chain_monsters[i].box.rec.x = chain_monsters[i].x;
        chain_monsters[i].box.rec.y = chain_monsters[i].y;
        chain_monsters[i].box.rec.width = chain_monsters[i].width * CELL_SIZE;
        chain_monsters[i].box.rec.height = chain_monsters[i].height * CELL_SIZE;
        
        // 減少冷卻時間
        if (chain_monsters[i].cooldown > 0) {
            chain_monsters[i].cooldown--;
        }

        // 射擊邏輯
        if(chain_monsters[i].cooldown == 0){
            printf("Shooter monster %d firing at position (%d,%d)\n", i, chain_monsters[i].x, chain_monsters[i].y);
              // 計算怪物中心位置（轉換為格子座標）
            int center_x = chain_monsters[i].x + CELL_SIZE * 2;
            int center_y = chain_monsters[i].y + CELL_SIZE * 2;
              
            // 向四個方向發射
            for (int dir = 0; dir < 4; dir++) {
                // create_projectile(shooter_monsters[i].x, shooter_monsters[i].y, dir, PURPLE_BLUE);
                create_chain(center_x, center_y,dir,PURPLE_BLUE_CHAIN);
            }
            
            // 重設冷卻時間
            chain_monsters[i].cooldown = 30;             
        }
    }
}


