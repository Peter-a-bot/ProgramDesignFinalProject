# Restaurator
## Background
某一天，世界各地出現了裂縫，從中出現了一群方塊狀的敵人，無差別攻擊人民，並且製造幻覺，使武器的攻擊範圍產生混亂。危難之中，一群名為「Restaurator」的英雄站了出來，他們使用由敵人靈魂構成的武器對抗敵人，守衛家園。

## Basic Controls
上下左右鍵移動 \\
P:暫停

## Characters

### Warrior
戰士擁有較高傷害和高血量,但是移動速度較低

![](resources/hero/warrior.png)

### Wizard
巫師具有最高的攻擊力、最低的移動速度和較少的血量
![](resources/hero/wizard.png)

### Archer
弓箭手的攻擊力和血量較低，但是移動速度最高

![](resources/hero/archer.png)

## Weapons

### Fireball
火球會繞著角色旋轉，攻擊碰到的敵人
![](resources/weapon/fireball.png)

### LaserGun
雷射槍會發射激光，攻擊眼前的敵人
![](resources/weapon/laser.png)

### Katana
武士刀會斬擊敵人，將敵人斬於刀下
![](resources/weapon/katana.png)

### ShockStaff
震擊杖會在角色周圍發射一圈沖擊波，攻擊並擊退敵人
![](resources/weapon/energyball.png)

### Whip
鞭子會水平揮舞，攻擊敵人
![](resources/weapon/whip.png)

### Barrier
結界會在角色周圍生成，攻擊進入結界內部的敵人
![](resources/weapon/barrier.png)

## Monsters

### Normal monster
一點一點閃爍,並靠近玩家的敵人

### Chain monster
發射綠色子彈，有高血量的敵人

### Faster monster
有著藍色外觀，以快速的速度接近玩家的敵人

### Shooter monster
會朝隨機方向射擊的敵人

### How to run the game?
在linux系統下，安裝完raylib後，使用以下命令編譯程式： \\
'''
gcc -o restaurator restaurator.c shooter_monster.c  treasure.c map_system.c collisionbox.c hero.c warrior.c archer.c barrier.c katana.c whip.c wizard.c basic_monster.c monster.c lasergun.c shockstaff.c weapon.c selectioninterface.c fireball.c chain_monster.c fast_monster.c -g -lm -lraylib -lpthread -ldl -lrt -lX11

'''
接著再運行restaurator即可進行遊玩

## Work allocation table
胡柏安：角色設計、武器設計、程式碼審查＆管理、UI設計 \\
蘇睦堯：怪物設計 \\
陳祈翰：寶箱系統 \\
蔡沂哲：角色升級系統 \\
廖彥翔：地圖選擇系統 ＆ 角色選擇系統 \\
蔡政祐：武器升級系統
