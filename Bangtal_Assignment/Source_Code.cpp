// ============================================================================================
// ����Ʈ�����к�, 20200690 ����ö
// ������ǻ�����α׷��� ���� ������ �ҽ� �ڵ�
// ============================================================================================

// ��ó����
#include <Bangtal.h>
#pragma comment(lib, "Bangtal.lib")

#include <iostream>
#include <cstdio>
#include <Windows.h>
#include <time.h>
#include <cstdlib>
#include <thread>
#include <chrono>

using std::thread;

// ============================================================================================

// ���
SceneID startScene, battleScene;

// UI
ObjectID startButton, howButton, howToPlay, hp, hpInfo;

// ���� �ð�
clock_t startTime;

// 1 �����Ӵ� 50 ms
constexpr auto TICK = 50;

// Ŭ�� �� ���� ������ ������ ���ط�
constexpr auto DAMAGE = 5;

// ����
constexpr auto SCALE = 0.8f;

// ��
ObjectID enemy;
int enemyX = 565, enemyY = 245;
constexpr auto ENEMY_MAXHP = 100;
constexpr auto ENEMY_AWAKEN_RATIO = 0.4f;
int enemyHp = ENEMY_MAXHP;

bool enemyAwakening = false;
bool enemyAwakenOnce = false;
bool enemyDied = false;

ObjectID barrierDefault, barrierAwaken;

bool laserAttacking = false;

// �÷��̾�
ObjectID shield;
constexpr auto PLAYER_MAXHP = static_cast<int>(ENEMY_MAXHP / DAMAGE / 2);
int playerHp = PLAYER_MAXHP;

bool playerShield = false;

// ============================================================================================

void awaken(void);
void awakenSetting(void);

void enemyHit(void);
void enemyDie(void);
void enemySetting(void);

void showHitEffect(SceneID scene, const int x, const int y, const float scale);

void showSeroLaserEffect(SceneID scene, const int x, const int y, const float scale);
void showSeroLaserAttack(SceneID scene, const int x, const int y, const float scale);

void showGaroLaserEffect(SceneID scene, const int x, const int y, const float scale);
void showGaroLaserAttack(SceneID scene, const int x, const int y, const float scale);

void showShield(void);

void mouseCallback(ObjectID obj, const int x, const int y, MouseAction action);

// ============================================================================================

// ����
void awaken(void) {
	enemyAwakening = true;
	enemyAwakenOnce = true;

	barrierAwaken = createObject("BarrierAwaken", ".\\Images\\Enemy\\barrier_awaken.png");
	locateObject(barrierAwaken, battleScene, enemyX, enemyY);
	scaleObject(barrierAwaken, SCALE);
}

// ���� ����
void awakenSetting(void) {
	enemyAwakenOnce = false;
	enemyHp = static_cast<int>(ENEMY_MAXHP * ENEMY_AWAKEN_RATIO);

	// �����
	hideObject(enemy);
	hideObject(barrierAwaken);
	hideObject(barrierDefault);
	Sleep(TICK * 10);

	// ��Ÿ��
	enemyX = 565;
	enemyY = 245;
	locateObject(enemy, battleScene, enemyX, enemyY);
	showObject(enemy);
	Sleep(TICK * 20);

	// �޽��� 1
	ObjectID message = createObject("Message", ".\\Images\\Others\\message1.png");
	locateObject(message, battleScene, enemyX, enemyY + 250);
	scaleObject(message, SCALE);
	showObject(message);
	Sleep(TICK * 30);
	hideObject(message);
	Sleep(TICK * 20);

	// ���� ���
	ObjectID awakening = createObject("Awakening", ".\\Images\\Enemy\\Awakening\\1.png");
	locateObject(awakening, battleScene, enemyX - 10, enemyY);
	showObject(awakening);

	for (int i = 2; i <= 5; i++) {
		Sleep(TICK);

		char imageName[60];
		sprintf_s(imageName, sizeof(imageName), ".\\Images\\Enemy\\Awakening\\%d.png", i);
		setObjectImage(awakening, imageName);
	}
	setObjectImage(enemy, ".\\Images\\Enemy\\awaken_fix.png");
	Sleep(TICK * 40);
	
	for (int i = 4; i >= 1; i--) {
		Sleep(TICK);

		char imageName[60];
		sprintf_s(imageName, sizeof(imageName), ".\\Images\\Enemy\\Awakening\\%d.png", i);
		setObjectImage(awakening, imageName);
	}
	Sleep(TICK);
	hideObject(awakening);
	Sleep(TICK * 20);

	// �޽��� 2
	setObjectImage(message, ".\\Images\\Others\\message2.png");
	showObject(message);
	Sleep(TICK * 30);
	hideObject(message);
	Sleep(TICK * 10);

	// �̵�
	hideObject(barrierDefault);
	setObjectImage(enemy, ".\\Images\\Enemy\\move_awaken.png");
	std::this_thread::sleep_for(std::chrono::duration<float>(0.5f));
	hideObject(enemy);

	enemyX = rand() % 1131;
	enemyY = rand() % 491;

	locateObject(enemy, battleScene, enemyX, enemyY);
	locateObject(barrierAwaken, battleScene, enemyX, enemyY);
	std::this_thread::sleep_for(std::chrono::duration<float>(1.0f));
	setObjectImage(enemy, ".\\Images\\Enemy\\awaken_fix.png");
	showObject(enemy);
	showObject(barrierAwaken);
}

// ============================================================================================

// Ÿ�� ���� �� ���� ü�� ���� �� �˻�
void enemyHit(void) {
	enemyHp -= DAMAGE;

	if (enemyHp <= ENEMY_MAXHP * ENEMY_AWAKEN_RATIO and not enemyAwakening) {
		awaken();
	}
	else if (enemyHp <= 0) {
		enemyDie();
	}
}

// ���� ����
void enemyDie(void) {
	enemyDied = true;

	hideObject(enemy);
	hideObject(barrierAwaken);
	hideObject(barrierDefault);
	hideObject(hp);
	hideObject(hpInfo);
	Sleep(TICK * 20);

	enemyX = 565;
	enemyY = 245;
	locateObject(enemy, battleScene, enemyX, enemyY);
	showObject(enemy);
	Sleep(TICK * 20);

	ObjectID message = createObject("Message", ".\\Images\\Others\\message_die1.png");
	locateObject(message, battleScene, enemyX, enemyY + 250);
	scaleObject(message, SCALE);
	showObject(message);
	Sleep(TICK * 30);
	hideObject(message);
	Sleep(TICK * 20);
	
	setObjectImage(message, ".\\Images\\Others\\message_die2.png");
	showObject(message);
	Sleep(TICK * 30);

	hideObject(message);
	hideObject(enemy);
	Sleep(TICK * 20);

	ObjectID win = createObject("Win", ".\\Images\\Others\\win.png");
	locateObject(win, battleScene, 520, 400);
	showObject(win);
	Sleep(TICK * 20);

	clock_t usedTime = clock() - startTime;
	char text[200];
	sprintf_s(text, sizeof(text), "���� ���: %d�� | �ҿ� �ð�: %d��\n�÷������ּż� �����մϴ�.", playerHp, static_cast<int>(usedTime / CLOCKS_PER_SEC));

	showMessage(text);
	Sleep(TICK * 60);

	endGame();
}

// ������ ���� �� �̵� ����
void enemySetting(void) {
	laserAttacking = true;

	srand((unsigned)time(NULL));

	float waitTime, laserScale;

	barrierDefault = createObject("BarrierDefault", ".\\Images\\Enemy\\barrier_default.png");
	locateObject(barrierDefault, battleScene, enemyX, enemyY);
	scaleObject(barrierDefault, SCALE);
	showObject(barrierDefault);

	Sleep(TICK * 30);

	// ������� ���� �ݺ�
	while (true) {
		// �������� ��� ������ ��� ���߸鼭 ���� ó��
		if (enemyAwakenOnce) {
			awakenSetting();
		}

		if (enemyDied) {
			break;
		}

		// ������
		// ������ ���� �� ��� �ð�
		waitTime = 2 - (float)(ENEMY_MAXHP - enemyHp) / 60;
		if (waitTime < 0.5f) {
			waitTime = 0.5f;
		}

		// ������ ũ��
		laserScale = 2.1f + (float)(ENEMY_MAXHP - enemyHp) / 60;
		if (laserScale > 3.5f) {
			laserScale = 3.5f;
		}

		Sleep(TICK * 10);
		showShield();
		if (enemyAwakening) {
			Sleep(TICK * 15);
		}
		else {
			Sleep(TICK * 20);
		}
		
		if (not playerShield) {
			hideObject(shield);
		}
		
		Sleep(TICK * 10);

		// ������ ����, ���� ����
		int kind = rand() % 2;
		
		if (kind % 2) {
			int y = rand() % 431;
			showGaroLaserEffect(battleScene, 0, y, laserScale);
			if (not playerShield) {
				playerHp -= 1;
			}
			

			if (enemyAwakening) {
				int x = rand() % 985;
				showSeroLaserEffect(battleScene, x, 0, laserScale);
				if (not playerShield) {
					playerHp -= 1;
				}
			}
		}
		else {
			int x = rand() % 985;
			showSeroLaserEffect(battleScene, x, 0, laserScale);
			if (not playerShield) {
				playerHp -= 1;
			}

			if (enemyAwakening) {
				int y = rand() % 431;
				showGaroLaserEffect(battleScene, 0, y, laserScale);
				if (not playerShield) {
					playerHp -= 1;
				}
			}
		}
		

		if (playerHp <= static_cast<int>(PLAYER_MAXHP * 0.5f)) {
			setObjectImage(hp, ".\\Images\\Others\\hp2.png");
		}

		// ü���� 0�� �Ǹ� ���� ����
		if (playerHp <= 0) {
			hideObject(enemy);
			hideObject(barrierAwaken);
			hideObject(barrierDefault);
			hideObject(hp);
			hideObject(hpInfo);
			Sleep(TICK * 20);

			ObjectID lose = createObject("Lose", ".\\Images\\Others\\lose.png");
			locateObject(lose, battleScene, 500, 400);
			showObject(lose);
			Sleep(TICK * 20);

			showMessage("����� �׸� �̱��� ���߽��ϴ�.\n���谡 �ı��Ǿ� ���� �����˴ϴ�.");
			Sleep(TICK * 60);

			endGame();
		}

		// Ÿ�� ���� �ð� (�ʹݿ� �ִ� 4��, �Ĺݿ� �ִ� 1��)
		laserAttacking = false;
		if (enemyAwakening) {
			hideObject(barrierAwaken);
		}
		else {
			hideObject(barrierDefault);
		}
		std::this_thread::sleep_for(std::chrono::duration<float>(waitTime));

		// ������� �� �ð�
		laserAttacking = true;
		std::this_thread::sleep_for(std::chrono::duration<float>(1.0f));

		if (enemyAwakenOnce or enemyDied) {
			continue;
		}

		// �̵� (�̵��ϴ� ������� ��ü�ϰ� �̵��� ��, ���� ��� + ������ �ٽ� ����)
		if (enemyAwakening) {
			hideObject(barrierDefault);
			setObjectImage(enemy, ".\\Images\\Enemy\\move_awaken.png");
			std::this_thread::sleep_for(std::chrono::duration<float>(0.5f));
			hideObject(enemy);

			enemyX = rand() % 1131;
			enemyY = rand() % 491;

			locateObject(enemy, battleScene, enemyX, enemyY);
			locateObject(barrierAwaken, battleScene, enemyX, enemyY);
			std::this_thread::sleep_for(std::chrono::duration<float>(1.0f));
			setObjectImage(enemy, ".\\Images\\Enemy\\awaken_fix.png");
			showObject(enemy);
			showObject(barrierAwaken);
		}
		else {
			setObjectImage(enemy, ".\\Images\\Enemy\\move_default.png");
			std::this_thread::sleep_for(std::chrono::duration<float>(0.5f));
			hideObject(enemy);

			enemyX = rand() % 1131;
			enemyY = rand() % 491;

			locateObject(enemy, battleScene, enemyX, enemyY);
			locateObject(barrierDefault, battleScene, enemyX, enemyY);
			std::this_thread::sleep_for(std::chrono::duration<float>(1.0f));
			setObjectImage(enemy, ".\\Images\\Enemy\\default.png");
			showObject(enemy);
			showObject(barrierDefault);
		}

		std::this_thread::sleep_for(std::chrono::duration<float>(0.2f));
	}
}

// ============================================================================================

// Ÿ�� ����
void showHitEffect(SceneID scene, const int x, const int y, const float scale = 1.0f) {
	// ����Ʈ �� ���� �κ�
	ObjectID hitEffect = createObject("hitEffect", ".\\Images\\HitEffects\\1.png");
	locateObject(hitEffect, scene, (enemyX - 125) + x, (enemyY - 140) + y);
	scaleObject(hitEffect, scale);

	SoundID hitSound = createSound(".\\Sounds\\hitSound.wav");

	showObject(hitEffect);
	playSound(hitSound);

	for (int i = 2; i <= 6; i++) {
		Sleep(TICK);

		char imageName[60];
		sprintf_s(imageName, sizeof(imageName), ".\\Images\\HitEffects\\%d.png", i);
		setObjectImage(hitEffect, imageName);
	}
	Sleep(TICK);

	hideObject(hitEffect);

	// Ÿ�� ���� �� ó�� �Լ�
	enemyHit();
}

// ============================================================================================

// ���� ���� ������
// ������ ��� �Լ�
void showSeroLaserEffect(SceneID scene, const int x, const int y, const float scale = 1.0f) {
	ObjectID laserReadyEffect = createObject("laserReady", ".\\Images\\Laser\\Sero\\Ready\\1.png");
	locateObject(laserReadyEffect, scene, x, y);
	scaleObject(laserReadyEffect, scale);
	
	SoundID laserEffectSound = createSound(".\\Sounds\\laserEffectSound.wav");

	showObject(laserReadyEffect);
	playSound(laserEffectSound);

	for (int i = 2; i <= 10; i++) {
		Sleep(TICK);

		char imageName[60];
		sprintf_s(imageName, sizeof(imageName), ".\\Images\\Laser\\Sero\\Ready\\%d.png", i);
		setObjectImage(laserReadyEffect, imageName);
	}
	Sleep(TICK);

	hideObject(laserReadyEffect);
	stopSound(laserEffectSound);

	Sleep(TICK * 2);
	showSeroLaserAttack(scene, x, y, scale);
}

// ������ ���� �Լ�
void showSeroLaserAttack(SceneID scene, const int x, const int y, const float scale = 1.0f) {
	ObjectID laserAttackEffect = createObject("laser", ".\\Images\\Laser\\Sero\\1.png");
	locateObject(laserAttackEffect, scene, x, y);
	scaleObject(laserAttackEffect, scale);

	SoundID laserAttackSound = createSound(".\\Sounds\\laserAttackSound.wav");

	showObject(laserAttackEffect);
	playSound(laserAttackSound);

	Sleep(TICK * 20);

	hideObject(laserAttackEffect);
}

// ���� ���� ������
void showGaroLaserEffect(SceneID scene, const int x, const int y, const float scale = 1.0f) {
	ObjectID laserReadyEffect = createObject("laserReady", ".\\Images\\Laser\\Garo\\Ready\\1.png");
	locateObject(laserReadyEffect, scene, x, y);
	scaleObject(laserReadyEffect, scale);

	SoundID laserEffectSound = createSound(".\\Sounds\\laserEffectSound.wav");

	showObject(laserReadyEffect);
	playSound(laserEffectSound);

	for (int i = 2; i <= 10; i++) {
		Sleep(TICK);

		char imageName[60];
		sprintf_s(imageName, sizeof(imageName), ".\\Images\\Laser\\Garo\\Ready\\%d.png", i);
		setObjectImage(laserReadyEffect, imageName);
	}
	Sleep(TICK);

	hideObject(laserReadyEffect);
	stopSound(laserEffectSound);

	Sleep(TICK * 2);
	showGaroLaserAttack(scene, x, y, scale);
}

void showGaroLaserAttack(SceneID scene, const int x, const int y, const float scale = 1.0f) {
	ObjectID laserAttackEffect = createObject("laser", ".\\Images\\Laser\\Garo\\1.png");
	locateObject(laserAttackEffect, scene, x, y);
	scaleObject(laserAttackEffect, scale);

	SoundID laserAttackSound = createSound(".\\Sounds\\laserAttackSound.wav");

	showObject(laserAttackEffect);
	playSound(laserAttackSound);

	Sleep(TICK * 20);

	hideObject(laserAttackEffect);
}

// ============================================================================================

// ����
void showShield(void) {
	playerShield = false;

	srand((unsigned)time(NULL));

	int x = rand() % 1051 + 100;
	int y = rand() % 501 + 100;

	locateObject(shield, battleScene, x, y);
	showObject(shield);
}

// ============================================================================================

// ���콺 �Է�
void mouseCallback(ObjectID obj, const int x, const int y, MouseAction action) {
	if (obj == howButton) {
		showObject(howToPlay);
	}
	else if (obj == howToPlay) {
		hideObject(howToPlay);
	}
	else if (obj == startButton) {
		enterScene(battleScene);
		startTime = clock();

		thread enemySet = thread(enemySetting);
		enemySet.detach();
	}
	else if (obj == shield) {
		playerShield = true;
		SoundID shieldSound = createSound(".\\Sounds\\shieldSound.wav");
		playSound(shieldSound);
		hideObject(shield);
	}
	else if (obj == enemy and not laserAttacking) {
		thread Hit = thread(showHitEffect, battleScene, x, y, 0.5f);
		Hit.join();
	}
}

// ============================================================================================

int main(void) {
	// ���콺 �Է� ����
	setMouseCallback(mouseCallback);

	// ���� ���
	startScene = createScene("Start", ".\\Images\\Others\\background_fix.jpg");
	battleScene = createScene("Battle", ".\\Images\\Others\\background_fix.jpg");

	// UI
	startButton = createObject("StartButton", ".\\Images\\Others\\start_fix.png");
	locateObject(startButton, startScene, 570, 130);
	scaleObject(startButton, SCALE);
	showObject(startButton);

	howButton = createObject("HowButton", ".\\Images\\Others\\how_fix.png");
	locateObject(howButton, startScene, 581, 50);
	scaleObject(howButton, SCALE);
	showObject(howButton);

	howToPlay = createObject("HowToPlay", ".\\Images\\Others\\howtoplay_fix2.png");
	locateObject(howToPlay, startScene, 497, 60);
	scaleObject(howToPlay, SCALE);

	hp = createObject("Hp", ".\\Images\\Others\\hp.png");
	locateObject(hp, battleScene, 50, 30);
	scaleObject(hp, SCALE);
	showObject(hp);

	hpInfo = createObject("HpInfo", ".\\Images\\Others\\hp_info.png");
	locateObject(hpInfo, battleScene, 35, 80);
	scaleObject(hpInfo, SCALE);
	showObject(hpInfo);

	// ��
	enemy = createObject("Enemy", ".\\Images\\Enemy\\default.png");
	locateObject(enemy, battleScene, enemyX, enemyY);
	scaleObject(enemy, SCALE);
	showObject(enemy);

	// �÷��̾� ���� (x: 100 ~ 1150 / y: 100 ~ 600)
	shield = createObject("Shield", ".\\Images\\Others\\shield_fix.png");
	locateObject(shield, battleScene, 1150, 600);
	scaleObject(shield, SCALE);

	// ���� ����
	startGame(startScene);
}
