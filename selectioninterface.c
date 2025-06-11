#include "selectioninterface.h"
#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include "hero.h"

#define MAX_PASSIVES 3
#define MAX_WEAPONS 6

int WeaponSelectionInterfece() {
    Sound sound = LoadSound("resources/sound/upgrade.mp3");
    PlaySound(sound);

    // 武器名稱和描述
    const char *weaponNames[] = {
        "Fireball", "LaserGun", "Katana", "ShockStaff", "Whip", "Barrier"
    };

    const char *weaponDescriptions[] = {
        "Fireball rotates around the player, attacking enemies.",
        "LaserGun fires a laser beam to attack enemies in front.",
        "Katana slashes enemies, cutting them down.",
        "ShockStaff emits a shockwave around the player, knocking enemies back.",
        "Whip swings horizontally to attack enemies.",
        "Barrier creates a field that attacks enemies inside it."
    };

    const int num_rows = 3;
    const int button_width = 250;
    const int button_height = 60;
    const int button_padding = 30;

    // 定義介面的顏色
    Color backgroundColor = BLACK;
    Color buttonNormalColor = MAROON;
    Color buttonHoveredColor = RED;
    Color buttonBorderColor = RED;
    Color buttonTextColor = WHITE;
    Color descriptionTextColor = YELLOW;

    // 獲取當前窗口尺寸
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int total_height = num_rows * button_height + (num_rows - 1) * button_padding;
    int starting_y = (screenHeight - total_height) / 2;

    int hoveredButton = -1;
    int selectedWeapon = -1;
    float time = 0.0f;

    // 選擇循環
    while (!WindowShouldClose() && selectedWeapon == -1) {
        time += GetFrameTime();
        Vector2 mousePoint = GetMousePosition();
        hoveredButton = -1;

        // 檢查鼠標互動
        for (int i = 0; i < 6; i++) {
            int row = i % num_rows;
            int col = i / num_rows;
            float x = (col == 0) ? 50 : (screenWidth - 50 - button_width);
            float y = starting_y + row * (button_height + button_padding);
            Rectangle buttonRect = {x, y, button_width, button_height};

            if (CheckCollisionPointRec(mousePoint, buttonRect)) {
                hoveredButton = i;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedWeapon = i;
                }
            }
        }

        // 繪製UI
        BeginDrawing();
        ClearBackground(backgroundColor);

        // 繪製標題
        const char *titleText = "Choose Your Favorite Weapon";
        int titleWidth = MeasureText(titleText, 30);
        float alpha = 0.5f + 0.5f * sinf(time * 2.0f * PI);
        Color titleColor = (Color){descriptionTextColor.r, descriptionTextColor.g, descriptionTextColor.b, (unsigned char)(alpha * 255)};
        DrawText(titleText, screenWidth / 2 - titleWidth / 2, 50, 30, titleColor);

        // 繪製按鈕
        for (int i = 0; i < 6; i++) {
            int row = i % num_rows;
            int col = i / num_rows;
            float x = (col == 0) ? 100 : (screenWidth - 100 - button_width);
            float y = starting_y + row * (button_height + button_padding);
            Rectangle buttonRect = {x, y, button_width, button_height};

            if (i == hoveredButton) {
                DrawRectangleRec(buttonRect, buttonHoveredColor);
            } else {
                DrawRectangleRec(buttonRect, buttonNormalColor);
            }
            DrawRectangleLinesEx(buttonRect, 2, buttonBorderColor);

            int textWidth = MeasureText(weaponNames[i], 20);
            DrawText(weaponNames[i], buttonRect.x + buttonRect.width / 2 - textWidth / 2, buttonRect.y + 15, 20, buttonTextColor);
        }

        // 繪製描述
        if (hoveredButton != -1) {
            int descWidth = MeasureText(weaponDescriptions[hoveredButton], 20);
            DrawText(weaponDescriptions[hoveredButton], screenWidth / 2 - descWidth / 2, screenHeight - 100, 20, descriptionTextColor);
        }

        EndDrawing();
    }

    return selectedWeapon;
}

int PassiveSelectionInterfece()
{
    //播放升級音效
    Sound upgradeSound = LoadSound("resources/sound/upgrade.mp3");
    PlaySound(upgradeSound);

    const char *passives[] = {"Recovery rate + 1", "Max hp +20%", "Attack Power +20%"};
    int numPassives = 3;
    // 檢查被動技能數量是否超過最大限制
    if (numPassives > MAX_PASSIVES) {
        TraceLog(LOG_ERROR, "Passive is too many.");
        return -1;
    }

    // 定義顏色
    Color backgroundColor = DARKPURPLE;           // 背景：深紫色
    Color buttonColor = (Color){0, 139, 139, 255}; // 按鈕：深青色
    Color buttonHoverColor = (Color){0, 255, 255, 255}; // 滑鼠懸停按鈕：亮青色
    Color titleColorBase = GOLD;                  // 標題基礎顏色：金色

    // 按鈕尺寸
    int buttonWidth = 300;
    int buttonHeight = 60;
    int spacing = 30;

    // 獲取當前螢幕尺寸
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // 計算起始Y位置，使按鈕垂直居中
    int totalHeight = buttonHeight * numPassives + spacing * (numPassives - 1);
    int startY = (screenHeight - totalHeight) / 2;

    // 設置按鈕矩形
    Rectangle buttons[MAX_PASSIVES];
    for (int i = 0; i < numPassives; i++)
    {
        buttons[i] = (Rectangle){
            (screenWidth - buttonWidth) / 2,
            startY + i * (buttonHeight + spacing),
            buttonWidth,
            buttonHeight
        };
    }

    int selectedPassive = -1;
    float time = 0.0f;

    // 主循環：直到選擇被動技能或視窗應關閉
    while (selectedPassive == -1 && !WindowShouldClose())
    {
        // 更新時間，用於脈動效果
        time += GetFrameTime();
        Vector2 mousePoint = GetMousePosition();

        BeginDrawing();
        ClearBackground(backgroundColor);

        // 繪製標題「被動升級」並添加脈動效果
        const char *titleText = "Passive upgrade";
        int titleWidth = MeasureText(titleText, 30);
        float alpha = 0.75f + 0.25f * sinf(time * 2.0f * M_PI); // 透明度在0.5到1.0之間變化
        Color titleColor = (Color){titleColorBase.r, titleColorBase.g, titleColorBase.b, (unsigned char)(alpha * 255)};
        DrawText(titleText, screenWidth / 2 - titleWidth / 2, 50, 30, titleColor);

        // 繪製按鈕並處理輸入
        for (int i = 0; i < numPassives; i++)
        {
            Color btnColor = buttonColor;
            if (CheckCollisionPointRec(mousePoint, buttons[i]))
            {
                btnColor = buttonHoverColor;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    selectedPassive = i + 1;
                    TraceLog(LOG_INFO, "Select passive ：%s", passives[i]);
                }
            }

            DrawRectangleRec(buttons[i], btnColor);
            int textWidth = MeasureText(passives[i], 20);
            DrawText(passives[i],
                     buttons[i].x + (buttonWidth - textWidth) / 2,
                     buttons[i].y + (buttonHeight - 20) / 2,
                     20,
                     RAYWHITE); // 白色文字以提高對比
        }

        EndDrawing();
    }

    return selectedPassive;
}

int DeathScreen(int score) {
    //播放死亡BGM
    Music deadBGM = LoadMusicStream("resources/bgm/failed.mp3");
    PlayMusicStream(deadBGM);
    // 定義常量
    const char* titleText = "Game Over";
    const int fontSizeTitle = 30;
    const int fontSizeText = 20;
    const int buttonWidth = 200;
    const int buttonHeight = 60;
    const int horizontalButtonPadding = 30; // 按鈕間水平間距
    const int verticalPadding = 20; // 文本元素間垂直間距

    // 顏色
    Color backgroundColor = BLACK;
    Color buttonNormalColor = DARKGRAY;
    Color buttonHoveredColor = RED;
    Color buttonBorderColor = RED;
    Color buttonTextColor = WHITE;
    Color titleColor = RED;
    Color textColor = YELLOW;

    // 獲取屏幕尺寸
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // 計算 UI 總高度
    float totalHeight = fontSizeTitle + 3 * verticalPadding + 2 * fontSizeText + buttonHeight;

    // 計算起始 y 坐標以垂直居中
    float y0 = (screenHeight - totalHeight) / 2.0f;

    // 計算各元素 y 坐標
    float titleY = y0;
    float deathY = titleY + fontSizeTitle + verticalPadding;
    float scoreY = deathY + fontSizeText + verticalPadding;
    float buttonsY = scoreY + fontSizeText + verticalPadding;

    // 計算按鈕 x 坐標
    float totalButtonsWidth = 2 * buttonWidth + horizontalButtonPadding;
    float leftX = (screenWidth - totalButtonsWidth) / 2.0f;

    // 定義按鈕矩形
    Rectangle restartButton = { leftX, buttonsY, (float)buttonWidth, (float)buttonHeight };
    Rectangle quitButton = { leftX + buttonWidth + horizontalButtonPadding, buttonsY, (float)buttonWidth, (float)buttonHeight };

    int selectedOption = -1; // -1: 未選擇, 0: 重新開始, 1: 退出
    float time = 0.0f;

    // 主循環
    while (!WindowShouldClose() && selectedOption == -1) {
        time += GetFrameTime();
        Vector2 mousePoint = GetMousePosition();
        int hoveredButton = -1;

        //更新BGM
        UpdateMusicStream(deadBGM);

        // 檢查重新開始按鈕
        if (CheckCollisionPointRec(mousePoint, restartButton)) {
            hoveredButton = 0;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedOption = 0; // 重新開始
            }
        }
        // 檢查退出按鈕
        else if (CheckCollisionPointRec(mousePoint, quitButton)) {
            hoveredButton = 1;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedOption = 1; // 退出
            }
        }

        // 開始繪製
        BeginDrawing();
        ClearBackground(backgroundColor);

        // 繪製標題（帶閃爍效果）
        float alpha = 0.5f + 0.5f * sinf(time * 2.0f * PI);
        Color pulsatingTitleColor = { titleColor.r, titleColor.g, titleColor.b, (unsigned char)(alpha * 255) };
        int titleWidth = MeasureText(titleText, fontSizeTitle);
        DrawText(titleText, screenWidth / 2 - titleWidth / 2, (int)titleY, fontSizeTitle, pulsatingTitleColor);

        // 繪製得分
        char scoreText[50];
        sprintf(scoreText, "Score: %d", score);
        int scoreWidth = MeasureText(scoreText, fontSizeText);
        DrawText(scoreText, screenWidth / 2 - scoreWidth / 2, (int)scoreY, fontSizeText, textColor);

        // 繪製重新開始按鈕
        if (hoveredButton == 0) {
            DrawRectangleRec(restartButton, buttonHoveredColor);
        } else {
            DrawRectangleRec(restartButton, buttonNormalColor);
        }
        DrawRectangleLinesEx(restartButton, 2, buttonBorderColor);
        int restartTextWidth = MeasureText("Restart", fontSizeText);
        DrawText("Restart", (int)(restartButton.x + restartButton.width / 2 - restartTextWidth / 2), (int)(restartButton.y + 15), fontSizeText, buttonTextColor);

        // 繪製退出按鈕
        if (hoveredButton == 1) {
            DrawRectangleRec(quitButton, buttonHoveredColor);
        } else {
            DrawRectangleRec(quitButton, buttonNormalColor);
        }
        DrawRectangleLinesEx(quitButton, 2, buttonBorderColor);
        int quitTextWidth = MeasureText("Quit", fontSizeText);
        DrawText("Quit", (int)(quitButton.x + quitButton.width / 2 - quitTextWidth / 2), (int)(quitButton.y + 15), fontSizeText, buttonTextColor);

        EndDrawing();
    }

    UnloadMusicStream(deadBGM);
    return selectedOption;
}

int CharacterSelectionInterface() {

    // 角色名稱和描述
    const char *characterNames[] = {
        "Warrior", "Wizard", "Archer"
    };

    const char *characterDescriptions[] = {
        "Warrior: A melee fighter with high strength",
        "Wizard: A mage skilled in ranged magic attacks",
        "Archer: A sharpshooter with precise ranged attacks"
    };

    // 按鈕參數
    const int numButtons = 3;
    const int buttonWidth = 200;
    const int buttonHeight = 60;
    const int buttonPadding = 20;

    // 定義介面的顏色
    Color backgroundColor = BLACK;
    Color buttonNormalColor = MAROON;
    Color buttonHoveredColor = RED;
    Color buttonBorderColor = RED;
    Color buttonTextColor = WHITE;
    Color descriptionTextColor = YELLOW;

    // 獲取當前窗口尺寸
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // 計算按鈕總高度並垂直居中
    int totalHeight = numButtons * buttonHeight + (numButtons - 1) * buttonPadding;
    int startingY = (screenHeight - totalHeight) / 2;

    int hoveredButton = -1;
    int selectedCharacter = -1;
    float time = 0.0f;

    // 選擇循環
    while (!WindowShouldClose() && selectedCharacter == -1) {
        time += GetFrameTime();
        Vector2 mousePoint = GetMousePosition();
        hoveredButton = -1;

        // 檢查鼠標互動
        for (int i = 0; i < numButtons; i++) {
            float x = (screenWidth - buttonWidth) / 2.0f; // 按鈕水平居中
            float y = startingY + i * (buttonHeight + buttonPadding);
            Rectangle buttonRect = {x, y, buttonWidth, buttonHeight};

            if (CheckCollisionPointRec(mousePoint, buttonRect)) {
                hoveredButton = i;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedCharacter = i + 1; // 返回 1, 2, 或 3
                }
            }
        }

        // 繪製UI
        BeginDrawing();
        ClearBackground(backgroundColor);

        // 繪製標題
        const char *titleText = "Choose Your Character";
        int titleWidth = MeasureText(titleText, 30);
        float alpha = 0.5f + 0.5f * sinf(time * 2.0f * PI);
        Color titleColor = {descriptionTextColor.r, descriptionTextColor.g, descriptionTextColor.b, (unsigned char)(alpha * 255)};
        DrawText(titleText, screenWidth / 2 - titleWidth / 2, 50, 30, titleColor);

        // 繪製按鈕
        for (int i = 0; i < numButtons; i++) {
            float x = (screenWidth - buttonWidth) / 2.0f;
            float y = startingY + i * (buttonHeight + buttonPadding);
            Rectangle buttonRect = {x, y, buttonWidth, buttonHeight};

            if (i == hoveredButton) {
                DrawRectangleRec(buttonRect, buttonHoveredColor);
            } else {
                DrawRectangleRec(buttonRect, buttonNormalColor);
            }
            DrawRectangleLinesEx(buttonRect, 2, buttonBorderColor);

            int textWidth = MeasureText(characterNames[i], 20);
            DrawText(characterNames[i], buttonRect.x + buttonRect.width / 2 - textWidth / 2, buttonRect.y + 15, 20, buttonTextColor);
        }

        // 繪製描述
        if (hoveredButton != -1) {
            int descWidth = MeasureText(characterDescriptions[hoveredButton], 20);
            DrawText(characterDescriptions[hoveredButton], screenWidth / 2 - descWidth / 2, screenHeight - 100, 20, descriptionTextColor);
        }

        EndDrawing();
    }

    return selectedCharacter;
}

int VictoryScreen(double gameTime) {
    //載入勝利BGM
    Music victoryBGM = LoadMusicStream("resources/bgm/victory.mp3");
    PlayMusicStream(victoryBGM);
    
    // 定義常量
    const char* titleText = "Victory!";
    const int fontSizeTitle = 30;
    const int fontSizeText = 20;
    const int buttonWidth = 200;
    const int buttonHeight = 60;
    const int horizontalButtonPadding = 30; // 按鈕間水平間距
    const int verticalPadding = 20; // 文本元素間垂直間距

    // 顏色
    Color backgroundColor = BLACK;
    Color buttonNormalColor = DARKBLUE;
    Color buttonHoveredColor = BLUE;
    Color buttonBorderColor = BLUE;
    Color buttonTextColor = WHITE;
    Color titleColor = BLUE;
    Color textColor = WHITE;


    // 獲取屏幕尺寸
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // 計算 UI 總高度
    float totalHeight = fontSizeTitle + 2 * verticalPadding + fontSizeText + buttonHeight;

    // 計算起始 y 坐標以垂直居中
    float y0 = (screenHeight - totalHeight) / 2.0f;

    // 計算各元素 y 坐標
    float titleY = y0;
    float timeY = titleY + fontSizeTitle + verticalPadding;
    float buttonsY = timeY + fontSizeText + verticalPadding;

    // 計算按鈕 x 坐標
    float totalButtonsWidth = 2 * buttonWidth + horizontalButtonPadding;
    float leftX = (screenWidth - totalButtonsWidth) / 2.0f;

    // 定義按鈕矩形
    Rectangle restartButton = { leftX, buttonsY, (float)buttonWidth, (float)buttonHeight };
    Rectangle quitButton = { leftX + buttonWidth + horizontalButtonPadding, buttonsY, (float)buttonWidth, (float)buttonHeight };

    int selectedOption = -1; // -1: 未選擇, 0: 重新開始, 1: 退出
    float time = 0.0f;

    // 主循環
    while (!WindowShouldClose() && selectedOption == -1) {
        time += GetFrameTime();
        Vector2 mousePoint = GetMousePosition();
        int hoveredButton = -1;

        UpdateMusicStream(victoryBGM);

        // 檢查重新開始按鈕
        if (CheckCollisionPointRec(mousePoint, restartButton)) {
            hoveredButton = 0;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedOption = 0; // 重新開始
            }
        }
        // 檢查退出按鈕
        else if (CheckCollisionPointRec(mousePoint, quitButton)) {
            hoveredButton = 1;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedOption = 1; // 退出
            }
        }

        // 開始繪製
        BeginDrawing();
        ClearBackground(backgroundColor);

        // 繪製標題（帶閃爍效果）
        float alpha = 0.5f + 0.5f * sinf(time * 2.0f * PI);
        Color pulsatingTitleColor = { titleColor.r, titleColor.g, titleColor.b, (unsigned char)(alpha * 255) };
        int titleWidth = MeasureText(titleText, fontSizeTitle);
        DrawText(titleText, screenWidth / 2 - titleWidth / 2, (int)titleY, fontSizeTitle, pulsatingTitleColor);

        // 繪製遊戲完成時間
        int minutes = (int)gameTime / 60;
        int seconds = (int)gameTime % 60;
        char timeText[50];
        sprintf(timeText, "Time: %02d:%02d", minutes, seconds);
        int timeWidth = MeasureText(timeText, fontSizeText);
        DrawText(timeText, screenWidth / 2 - timeWidth / 2, (int)timeY, fontSizeText, textColor);

        // 繪製重新開始按鈕
        if (hoveredButton == 0) {
            DrawRectangleRec(restartButton, buttonHoveredColor);
        } else {
            DrawRectangleRec(restartButton, buttonNormalColor);
        }
        DrawRectangleLinesEx(restartButton, 2, buttonBorderColor);
        int restartTextWidth = MeasureText("Restart", fontSizeText);
        DrawText("Restart", (int)(restartButton.x + restartButton.width / 2 - restartTextWidth / 2), (int)(restartButton.y + 15), fontSizeText, buttonTextColor);

        // 繪製退出按鈕
        if (hoveredButton == 1) {
            DrawRectangleRec(quitButton, buttonHoveredColor);
        } else {
            DrawRectangleRec(quitButton, buttonNormalColor);
        }
        DrawRectangleLinesEx(quitButton, 2, buttonBorderColor);
        int quitTextWidth = MeasureText("Quit", fontSizeText);
        DrawText("Quit", (int)(quitButton.x + quitButton.width / 2 - quitTextWidth / 2), (int)(quitButton.y + 15), fontSizeText, buttonTextColor);

        EndDrawing();

    }

    UnloadMusicStream(victoryBGM);
    return selectedOption;
}
int MainMenu() {
    //播放BGM
    Music mainBGM = LoadMusicStream("resources/bgm/mainMenu.mp3");
    PlayMusicStream(mainBGM);

    // 定義常量
    const char *titleText = "Restaurator";
    const char *buttonNames[] = {"Start Game", "Quit Game"};
    const int numButtons = 2;
    const int buttonWidth = 200;
    const int buttonHeight = 60;
    const int buttonPadding = 20;

    // 定義介面的顏色
    Color backgroundColor = BLACK;
    Color buttonNormalColor = MAROON;
    Color buttonHoveredColor = RED;
    Color buttonBorderColor = RED;
    Color buttonTextColor = WHITE;
    Color titleColor = RED;

    // 獲取當前窗口尺寸
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // 計算按鈕總高度並垂直居中
    int totalHeight = numButtons * buttonHeight + (numButtons - 1) * buttonPadding;
    int startingY = (screenHeight - totalHeight) / 2;

    int hoveredButton = -1;
    int selectedOption = -1;
    float time = 0.0f;

    // 主循環
    while (!WindowShouldClose() && selectedOption == -1) {
        time += GetFrameTime();
        Vector2 mousePoint = GetMousePosition();
        hoveredButton = -1;

        //更新BGM
        UpdateMusicStream(mainBGM);

        // 檢查鼠標互動
        for (int i = 0; i < numButtons; i++) {
            float x = (screenWidth - buttonWidth) / 2.0f; // 按鈕水平居中
            float y = startingY + i * (buttonHeight + buttonPadding);
            Rectangle buttonRect = {x, y, buttonWidth, buttonHeight};

            if (CheckCollisionPointRec(mousePoint, buttonRect)) {
                hoveredButton = i;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedOption = i; // 0: 開始遊戲, 1: 退出遊戲
                }
            }
        }

        // 繪製UI
        BeginDrawing();
        ClearBackground(backgroundColor);

        // 繪製標題
        int titleWidth = MeasureText(titleText, 40);
        float alpha = 0.5f + 0.5f * sinf(time * 2.0f * PI);
        Color pulsatingTitleColor = {titleColor.r, titleColor.g, titleColor.b, (unsigned char)(alpha * 255)};
        DrawText(titleText, screenWidth / 2 - titleWidth / 2, 100, 40, pulsatingTitleColor);

        // 繪製按鈕
        for (int i = 0; i < numButtons; i++) {
            float x = (screenWidth - buttonWidth) / 2.0f;
            float y = startingY + i * (buttonHeight + buttonPadding);
            Rectangle buttonRect = {x, y, buttonWidth, buttonHeight};

            if (i == hoveredButton) {
                DrawRectangleRec(buttonRect, buttonHoveredColor);
            } else {
                DrawRectangleRec(buttonRect, buttonNormalColor);
            }
            DrawRectangleLinesEx(buttonRect, 2, buttonBorderColor);

            int textWidth = MeasureText(buttonNames[i], 20);
            DrawText(buttonNames[i], buttonRect.x + buttonRect.width / 2 - textWidth / 2, buttonRect.y + 15, 20, buttonTextColor);
        }

        EndDrawing();
    }

    UnloadMusicStream(mainBGM);

    return selectedOption;
}

int MapSelectionInterface() {
    // 地圖名稱和描述
    const char *mapNames[] = {
        "Grassland", "Island", "Volcano"
    };

    const char *mapDescriptions[] = {
        "Grassland: Lush fields with open terrain",
        "Island: Tropical isles surrounded by water",
        "Volcano: Fiery terrain with dangerous lava"
    };

    // 按鈕參數
    const int numButtons = 3;
    const int buttonWidth = 200;
    const int buttonHeight = 60;
    const int buttonPadding = 20;

    // 定義介面的顏色
    Color backgroundColor = BLACK;
    Color buttonNormalColor = MAROON;
    Color buttonHoveredColor = RED;
    Color buttonBorderColor = RED;
    Color buttonTextColor = WHITE;
    Color descriptionTextColor = YELLOW;

    // 獲取當前窗口尺寸
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // 計算按鈕總高度並垂直居中
    int totalHeight = numButtons * buttonHeight + (numButtons - 1) * buttonPadding;
    int startingY = (screenHeight - totalHeight) / 2;

    int hoveredButton = -1;
    int selectedMap = -1;
    float time = 0.0f;

    // 選擇循環
    while (!WindowShouldClose() && selectedMap == -1) {
        time += GetFrameTime();
        Vector2 mousePoint = GetMousePosition();
        hoveredButton = -1;

        // 檢查鼠標互動
        for (int i = 0; i < numButtons; i++) {
            float x = (screenWidth - buttonWidth) / 2.0f; // 按鈕水平居中
            float y = startingY + i * (buttonHeight + buttonPadding);
            Rectangle buttonRect = {x, y, buttonWidth, buttonHeight};

            if (CheckCollisionPointRec(mousePoint, buttonRect)) {
                hoveredButton = i;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedMap = i + 1; // 返回 1, 2, 或 3
                }
            }
        }

        // 繪製UI
        BeginDrawing();
        ClearBackground(backgroundColor);

        // 繪製標題
        const char *titleText = "Choose Your Map";
        int titleWidth = MeasureText(titleText, 30);
        float alpha = 0.5f + 0.5f * sinf(time * 2.0f * PI);
        Color titleColor = {descriptionTextColor.r, descriptionTextColor.g, descriptionTextColor.b, (unsigned char)(alpha * 255)};
        DrawText(titleText, screenWidth / 2 - titleWidth / 2, 50, 30, titleColor);

        // 繪製按鈕
        for (int i = 0; i < numButtons; i++) {
            float x = (screenWidth - buttonWidth) / 2.0f;
            float y = startingY + i * (buttonHeight + buttonPadding);
            Rectangle buttonRect = {x, y, buttonWidth, buttonHeight};

            if (i == hoveredButton) {
                DrawRectangleRec(buttonRect, buttonHoveredColor);
            } else {
                DrawRectangleRec(buttonRect, buttonNormalColor);
            }
            DrawRectangleLinesEx(buttonRect, 2, buttonBorderColor);

            int textWidth = MeasureText(mapNames[i], 20);
            DrawText(mapNames[i], buttonRect.x + buttonRect.width / 2 - textWidth / 2, buttonRect.y + 15, 20, buttonTextColor);
        }

        // 繪製描述
        if (hoveredButton != -1) {
            int descWidth = MeasureText(mapDescriptions[hoveredButton], 20);
            DrawText(mapDescriptions[hoveredButton], screenWidth / 2 - descWidth / 2, screenHeight - 100, 20, descriptionTextColor);
        }

        EndDrawing();
    }

    return selectedMap;
}


int PauseScreen(Hero *hero) {
    // 定義常量
    const char *titleText = "Game Paused";
    const char *buttonNames[] = {"Continue", "Quit", "Toggle Sort"};
    const int numButtons = 3;
    const int buttonWidth = 200;
    const int buttonHeight = 60;
    const int buttonPadding = 20;
    const int fontSizeTitle = 30;
    const int fontSizeText = 20;
    const int verticalPadding = 20;

    // 定義介面的顏色
    Color backgroundColor = BLACK;
    Color buttonNormalColor = MAROON;
    Color buttonHoveredColor = RED;
    Color buttonBorderColor = RED;
    Color buttonTextColor = WHITE;
    Color titleColor = RED;
    Color weaponTextColor = YELLOW;

    // 獲取當前窗口尺寸
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // 計算 UI 總高度（標題 + 武器列表 + 按鈕）
    int maxWeapons = hero->weaponCount > MAX_WEAPONS ? MAX_WEAPONS : hero->weaponCount;
    int totalHeight = fontSizeTitle + verticalPadding + (maxWeapons * (fontSizeText + verticalPadding)) + (numButtons * buttonHeight + (numButtons - 1) * buttonPadding);
    int startingY = (screenHeight - totalHeight) / 2;

    // 定義按鈕矩形
    Rectangle buttons[3];
    for (int i = 0; i < numButtons; i++) {
        float x = (screenWidth - buttonWidth) / 2.0f;
        float y = startingY + fontSizeTitle + verticalPadding + (maxWeapons * (fontSizeText + verticalPadding)) + i * (buttonHeight + buttonPadding);
        buttons[i] = (Rectangle){x, y, buttonWidth, buttonHeight};
    }

    int hoveredButton = -1;
    int selectedOption = -1; // -1: 未選擇, 0: 繼續, 1: 退出, 2: 切換排序
    bool sortAscending = true; // 初始排序為由小到大
    float time = 0.0f;

    // 創建排序後的武器索引數組
    int weaponIndices[MAX_WEAPONS];
    for (int i = 0; i < maxWeapons; i++) {
        weaponIndices[i] = i;
    }

    // 主循環
    while (!WindowShouldClose() && selectedOption == -1) {
        time += GetFrameTime();
        Vector2 mousePoint = GetMousePosition();
        hoveredButton = -1;

        // 檢查鼠標互動
        for (int i = 0; i < numButtons; i++) {
            if (CheckCollisionPointRec(mousePoint, buttons[i])) {
                hoveredButton = i;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (i == 2) {
                        // 切換排序方式
                        sortAscending = !sortAscending;
                        // 簡單的冒泡排序
                        for (int j = 0; j < maxWeapons - 1; j++) {
                            for (int k = 0; k < maxWeapons - j - 1; k++) {
                                int power1 = hero->weapons[weaponIndices[k]]->attackPower;
                                int power2 = hero->weapons[weaponIndices[k + 1]]->attackPower;
                                if ((sortAscending && power1 > power2) || (!sortAscending && power1 < power2)) {
                                    int temp = weaponIndices[k];
                                    weaponIndices[k] = weaponIndices[k + 1];
                                    weaponIndices[k + 1] = temp;
                                }
                            }
                        }
                    } else {
                        selectedOption = i; // 0: 繼續, 1: 退出
                    }
                }
            }
        }

        // 繪製UI
        BeginDrawing();
        ClearBackground(backgroundColor);

        // 繪製標題（帶閃爍效果）
        float alpha = 0.5f + 0.5f * sinf(time * 2.0f * PI);
        Color pulsatingTitleColor = {titleColor.r, titleColor.g, titleColor.b, (unsigned char)(alpha * 255)};
        int titleWidth = MeasureText(titleText, fontSizeTitle);
        DrawText(titleText, screenWidth / 2 - titleWidth / 2, startingY, fontSizeTitle, pulsatingTitleColor);

        // 繪製武器列表
        for (int i = 0; i < maxWeapons; i++) {
            int index = weaponIndices[i];
            char weaponText[100];
            snprintf(weaponText, sizeof(weaponText), "%s (Attack: %f)", hero->weapons[index]->name, hero->weapons[index]->attackPower);
            int textWidth = MeasureText(weaponText, fontSizeText);
            DrawText(weaponText, screenWidth / 2 - textWidth / 2, startingY + fontSizeTitle + verticalPadding + i * (fontSizeText + verticalPadding), fontSizeText, weaponTextColor);
        }

        // 繪製按鈕
        for (int i = 0; i < numButtons; i++) {
            if (i == hoveredButton) {
                DrawRectangleRec(buttons[i], buttonHoveredColor);
            } else {
                DrawRectangleRec(buttons[i], buttonNormalColor);
            }
            DrawRectangleLinesEx(buttons[i], 2, buttonBorderColor);
            int textWidth = MeasureText(buttonNames[i], fontSizeText);
            DrawText(buttonNames[i], buttons[i].x + buttons[i].width / 2 - textWidth / 2, buttons[i].y + 15, fontSizeText, buttonTextColor);
        }

        EndDrawing();
    }

    return selectedOption;
}
