#include <raylib.h>
#include "weapon.h"
#include "katana.h"
#include "lasergun.h"
#include "barrier.h"
#include "whip.h"
#include "shockstaff.h"
#include "fireball.h"

#include "hero.h"
#include "warrior.h"
#include "wizard.h"
#include "archer.h"

#include "basic_monster.h"
#include "monster.h"
#include "shooter_monster.h"

#include "treasure.h"
#include "map_system.h"
#include "selectioninterface.h"

void DrawProgressBar(int x, int y, int width, int height, float progress, Color bgColor, Color fgColor);
void DrawGameUI(float time, int currentHP, int maxHP, int currentXP, int maxXP);

int main() {
  //初始化視窗
  const int screenWidth = 1980;
  const int screenHeight = 1080;
  //
  // const int screenWidth = 800;
  // const int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "Restaurator");

  SetTargetFPS(60);

  while(!WindowShouldClose()) {
    //顯示主畫面
    int isExit = MainMenu();

    //若選擇退出，直接關閉遊戲
    if(isExit) {
      break;
    }

    //地圖選擇
    Map* map;
    int mapCode = MapSelectionInterface();

    switch (mapCode) {
      case 1:
        map = MapInit(LoadTexture("resources/map/grassland.png"));
        break;

      case 2:
        map = MapInit(LoadTexture("resources/map/island.png"));
        break;

      case 3:
        map = MapInit(LoadTexture("resources/map/volcano.png"));
        break;
    }

    //選擇角色
    int characterCode = CharacterSelectionInterface();
    Hero* hero;

    switch(characterCode) {
      case 1:
        hero = &WarriorInit()->base;        
        break;

      case 2:
        hero = &WizardInit()->base;
        break;

      case 3:
        hero = &ArcherInit()->base;
        break;
    }

    //選擇武器
    int weaponCode = WeaponSelectionInterfece();
    Weapon* weapon;

    switch (weaponCode) {
      case 0:
        weapon = &FireBallInit()->base;
        break;
      case 1:
        weapon = &LaserGunInit()->base;
        break;

      case 2:
        weapon = &KatanaInit()->base;
        break;

      case 3:
        weapon = &ShockStaffInit()->base;
        break;

      case 4:
        weapon = &WhipInit()->base;
        break;

      case 5:
        weapon = &BarrierInit()->base;
        break;
    }

    hero->weapons[0] = weapon;
    hero->weaponCount = 1;

    //主遊戲部份
    // 怪物初始化
    spawn_monsters(hero);
    init_shooter_monsters();
    init_projectiles();
    init_explosions();

    //寶箱初始化
    Treasure_InitSystem();

    //時間計算相關參數
    // preTime代表上次取得的時間
    // curTime代表遊戲運行後經過的時間
    // nowTime代表現在時間
    double preTime = GetTime(), curTime = 0, nowTime = preTime;
    bool isHeroDead = 0, isHeroWin = 0;

    //調整角色初始位置
    hero->position.x = screenWidth/2;
    hero->position.y = screenHeight/2;

    //初始化攝影機
    Camera2D camera = {0};
    camera.target = (Vector2) {hero->position.x/2.0f, hero->position.y/2.0f};
    camera.offset = (Vector2){screenWidth/2.0f, screenHeight/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    

    //主遊戲循環
    while(curTime < 300.0f && !isHeroDead && !isHeroWin) {
      //確認角色血量
      if(hero->hp <= 0) {
        isHeroDead = 1;
      }
      //時間更新
      nowTime = GetTime();
      double timeDiff = nowTime - preTime;
      preTime = nowTime;

      //確認是否暫停
      if(IsKeyPressed(KEY_P)) {
        int option = PauseScreen(hero);

        if(option == 0) {
          continue;
        }
        else if(option == 1) {
          isExit = true;
          break;
        }
      }

      //測試用時的程式
      if(IsKeyPressed(KEY_W)) {
        isHeroWin = true;
      }

      if(IsKeyPressed(KEY_L)) {
        isHeroDead = true;
      }
      if(IsKeyPressed(KEY_T)) {
        hero->exp = hero->nextLevelExp;
      }


      //更新遊戲時間
      curTime += timeDiff;

      //英雄移動
      hero->move(hero);

      //新增敵人
      add_monsters(hero);
      spawn_shooter_monster(hero);
      update_shooter_monsters(hero);
      update_projectiles(hero);


      //怪物向敵人移動
      move_monsters_towards_player(hero);
      // 
      //攝影機調整至角色中心
      camera.target = (Vector2) {hero->position.x/2.0f, hero->position.y/2.0f};

      BeginDrawing();
        //繪製背景
        DrawMap(map, hero);


        //啟動攝影機模式
        BeginMode2D(camera);

          //更新英雄狀態
          hero->update(hero, timeDiff);
          hero->draw(hero);
          draw_monsters();

          check_collision(hero);
          check_shooter_monster_collision(hero);
          get_demage(hero);
          get_shooter_monster_demage(hero);

          update_explosion();
          draw_shooter_monsters();
          draw_projectiles();
          draw_explosions();

          Treasure_DrawAll();

          weaponCode = Treasure_Update(hero);
          Weapon* newWepon;

          if(hero->weaponCount <= 6) {
            switch (weaponCode) {
              case 0:
                hero->weapons[hero->weaponCount] = &FireBallInit()->base;
                hero->weaponCount++;
                break;
              case 1:
                hero->weapons[hero->weaponCount] = &LaserGunInit()->base;
                hero->weaponCount++;
                break;

              case 2:
                hero->weapons[hero->weaponCount] = &KatanaInit()->base;
                hero->weaponCount++;
                break;

              case 3:
                hero->weapons[hero->weaponCount] = &ShockStaffInit()->base;
                hero->weaponCount++;
                break;

              case 4:
                hero->weapons[hero->weaponCount] = &WhipInit()->base;
                hero->weaponCount++;
                break;

              case 5:
                hero->weapons[hero->weaponCount] = &BarrierInit()->base;
                hero->weaponCount++;
                break;

              default:
                break;
            }
            
          }



        EndMode2D();
    
        //畫出遊戲的UI
        DrawGameUI(curTime, hero->hp, hero->maxHp,hero->exp, hero->nextLevelExp);

      EndDrawing();

      //替換已有的敵人
      replace_missing_monsters(hero);
    }

    if(curTime >= 300.0f) {
      isHeroWin = 1;
    }

    //勝利時呼叫勝利畫面
    if(isHeroWin) {
      isExit = VictoryScreen(curTime);
    }

    //死亡時呼叫死亡畫面
    if(isHeroDead) {
      isExit = DeathScreen(curTime);
    }

    if(isExit) {
      break;
    }
  }

  CloseWindow();

}

void DrawProgressBar(int x, int y, int width, int height, float progress, Color bgColor, Color fgColor) {
    // 確保進度值在 0 到 1 之間
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;

    // 繪製背景矩形
    DrawRectangle(x, y, width, height, bgColor);

    // 計算前景矩形的寬度
    int fgWidth = (int)(width * progress);

    // 繪製前景矩形
    DrawRectangle(x, y, fgWidth, height, fgColor);
}

void DrawGameUI(float time, int currentHP, int maxHP, int currentXP, int maxXP) {
    // 定義常量
    const int fontSize = 20;
    const int barWidth = 200;
    const int barHeight = 20;
    const int padding = 10;

    // 計算位置
    int timerX = padding;
    int timerY = padding;
    int hpTextX = padding;
    int hpTextY = timerY + fontSize + padding;
    int hpBarX = padding + 150; // 留出文字空間
    int hpBarY = hpTextY;
    int xpTextX = padding;
    int xpTextY = hpBarY + barHeight + padding;
    int xpBarX = padding + 150;
    int xpBarY = xpTextY;

    // 繪製遊戲運行時間
    int minutes = (int)time / 60;
    int seconds = (int)time % 60;
    DrawText(TextFormat("Time: %02d:%02d", minutes, seconds), timerX, timerY, fontSize, GREEN);

    // 繪製 HP 文字
    DrawText(TextFormat("HP: %d/%d", currentHP, maxHP), hpTextX, hpTextY, fontSize, GREEN);

    // 繪製 HP 進度條
    DrawRectangle(hpBarX, hpBarY, barWidth, barHeight, GRAY);
    float hpRatio = (float)currentHP / maxHP;
    if (hpRatio > 1.0f) hpRatio = 1.0f;
    if (hpRatio < 0.0f) hpRatio = 0.0f;
    int hpFillWidth = (int)(barWidth * hpRatio);
    DrawRectangle(hpBarX, hpBarY, hpFillWidth, barHeight, RED);

    // 繪製 XP 文字
    DrawText(TextFormat("XP: %d/%d", currentXP, maxXP), xpTextX, xpTextY, fontSize, GREEN);

    // 繪製 XP 進度條
    DrawRectangle(xpBarX, xpBarY, barWidth, barHeight, GRAY);
    float xpRatio = (float)currentXP / maxXP;
    if (xpRatio > 1.0f) xpRatio = 1.0f;
    if (xpRatio < 0.0f) xpRatio = 0.0f;
    int xpFillWidth = (int)(barWidth * xpRatio);
    DrawRectangle(xpBarX, xpBarY, xpFillWidth, barHeight, BLUE);
}
