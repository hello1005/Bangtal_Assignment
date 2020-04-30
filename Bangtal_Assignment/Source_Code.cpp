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

// 1 �����Ӵ� 50 ms
constexpr auto TICK = 50;

// Ŭ�� �� ���� ������ ���ط�
constexpr auto DAMAGE = 20;

SceneID startScene, battleScene;

ObjectID enemy;
int enemyX = 565, enemyY = 245;
constexpr auto ENEMY_MAXHP = 100;
int enemyHp = 100;
bool enemyAwakening = false;

ObjectID barrierDefault, barrierAwaken;

bool laserAttacking = false;

// ============================================================================================

// Ÿ�� ���� �� ���� ü�� ���� �� �˻�
void enemyHit(void) {
	enemyHp -= DAMAGE;

	if (enemyHp <= 20 and not enemyAwakening) {
		printf("���� ON");
		enemyAwakening = true;
		// �� ����
		setObjectImage(enemy, ".\\Images\\Enemy\\awaken_fix.png");
		barrierAwaken = createObject("BarrierAwaken", ".\\Images\\Enemy\\barrier_awaken.png");
		locateObject(barrierAwaken, battleScene, enemyX, enemyY);
		scaleObject(barrierAwaken, 0.8f);
	}
}

// Ÿ�� ����
void showHitEffect(SceneID scene, int x, int y, const float scale = 1.0f) {
	// ����Ʈ �� ���� �κ�
	ObjectID hitEffect = createObject("hitEffect", ".\\Images\\HitEffects\\1.png");
	locateObject(hitEffect, scene, x, y);
	scaleObject(hitEffect, scale);

	SoundID hitSound = createSound(".\\Sounds\\hitSound.wav");

	showObject(hitEffect);
	// playSound(hitSound);

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

// ���� ���� ������
// ������ ���� �Լ�
void showSeroLaserAttack(SceneID scene, const int x, const int y, const float scale = 1.0f) {
	ObjectID laserAttackEffect = createObject("laser", ".\\Images\\Laser\\Sero\\1.png");
	locateObject(laserAttackEffect, scene, x, y);
	scaleObject(laserAttackEffect, scale);

	SoundID laserAttackSound = createSound(".\\Sounds\\laserAttackSound.wav");

	showObject(laserAttackEffect);
	// playSound(laserAttackSound);

	Sleep(TICK * 20);

	hideObject(laserAttackEffect);
}

// ������ ��� �Լ�
void showSeroLaserEffect(SceneID scene, const int x, const int y, const float scale = 1.0f) {
	ObjectID laserReadyEffect = createObject("laserReady", ".\\Images\\Laser\\Sero\\Ready\\1.png");
	locateObject(laserReadyEffect, scene, x, y);
	scaleObject(laserReadyEffect, scale);
	
	showObject(laserReadyEffect);

	for (int i = 2; i <= 10; i++) {
		Sleep(TICK);

		char imageName[60];
		sprintf_s(imageName, sizeof(imageName), ".\\Images\\Laser\\Sero\\Ready\\%d.png", i);
		setObjectImage(laserReadyEffect, imageName);
	}
	Sleep(TICK);

	hideObject(laserReadyEffect);

	Sleep(TICK * 2);
	showSeroLaserAttack(scene, x, y, scale);
}

// ���� ���� ������
void showGaroLaserAttack(SceneID scene, const int x, const int y, const float scale = 1.0f) {
	ObjectID laserAttackEffect = createObject("laser", ".\\Images\\Laser\\Garo\\1.png");
	locateObject(laserAttackEffect, scene, x, y);
	scaleObject(laserAttackEffect, scale);

	SoundID laserAttackSound = createSound(".\\Sounds\\laserAttackSound.wav");

	showObject(laserAttackEffect);
	// playSound(laserAttackSound);

	Sleep(TICK * 20);

	hideObject(laserAttackEffect);
}

void showGaroLaserEffect(SceneID scene, const int x, const int y, const float scale = 1.0f) {
	ObjectID laserReadyEffect = createObject("laserReady", ".\\Images\\Laser\\Garo\\Ready\\1.png");
	locateObject(laserReadyEffect, scene, x, y);
	scaleObject(laserReadyEffect, scale);

	showObject(laserReadyEffect);

	for (int i = 2; i <= 10; i++) {
		Sleep(TICK);

		char imageName[60];
		sprintf_s(imageName, sizeof(imageName), ".\\Images\\Laser\\Garo\\Ready\\%d.png", i);
		setObjectImage(laserReadyEffect, imageName);
	}
	Sleep(TICK);
	
	hideObject(laserReadyEffect);

	Sleep(TICK * 2);
	showGaroLaserAttack(scene, x, y, scale);
}

// ������ ���� �� �̵� ����
void laserSetting(void) {
	laserAttacking = true;

	srand((unsigned)time(NULL));

	float waitTime, laserScale;

	barrierDefault = createObject("BarrierDefault", ".\\Images\\Enemy\\barrier_default.png");
	locateObject(barrierDefault, battleScene, enemyX, enemyY);
	scaleObject(barrierDefault, 0.8f);
	showObject(barrierDefault);
	
	// ������� ���� �ݺ�
	while (true) {
		// ������
		// ������ ���� �� ��� �ð�
		waitTime = 2 - (float)(ENEMY_MAXHP - enemyHp) / 60;
		if (waitTime < 0.5f) {
			waitTime = 0.5f;
		}

		// ������ ũ��
		laserScale = 2.0f + (float)(ENEMY_MAXHP - enemyHp) / 60;
		if (laserScale > 3.5f) {
			laserScale = 3.5f;
		}

		// ������ ����, ���� ����
		int kind = rand() % 2;

		if (kind % 2) {
			int y = rand() % 431;
			showGaroLaserEffect(battleScene, 0, y, laserScale);
		}
		else {
			int x = rand() % 985;
			showSeroLaserEffect(battleScene, x, 0, laserScale);
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

		std::this_thread::sleep_for(std::chrono::duration<float>(0.1f));
	}
}

// ���콺 �Է�
void mouseCallback(ObjectID obj, int x, int y, MouseAction action) {
	if (obj == enemy and not laserAttacking) {
		thread Hit = thread(showHitEffect, battleScene, x, y, 0.5f);
		Hit.join();
	}
}

// ============================================================================================

int main(void) {
	// ���콺 �Է� ����
	setMouseCallback(mouseCallback);

	// ���� ���
	battleScene = createScene("Battle", "none");

	// ��
	enemy = createObject("Enemy", ".\\Images\\Enemy\\default.png");
	locateObject(enemy, battleScene, enemyX, enemyY);
	scaleObject(enemy, 0.8f);
	showObject(enemy);
	
	// ������ �� �̵� ����
	thread Setting = thread(laserSetting);
	Setting.detach();
	
	// ���� ����
	clock_t start = clock();
	startGame(battleScene);
}
