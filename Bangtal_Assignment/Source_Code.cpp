// ============================================================================================

// 전처리기

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

// 1 프레임당 50 ms
constexpr auto TICK = 50;

// 클릭 한 번이 입히는 피해량
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

// 타격 성공 시 적의 체력 감소 및 검사
void enemyHit(void) {
	enemyHp -= DAMAGE;

	if (enemyHp <= 20 and not enemyAwakening) {
		printf("각성 ON");
		enemyAwakening = true;
		// 맵 변경
		setObjectImage(enemy, ".\\Images\\Enemy\\awaken_fix.png");
		barrierAwaken = createObject("BarrierAwaken", ".\\Images\\Enemy\\barrier_awaken.png");
		locateObject(barrierAwaken, battleScene, enemyX, enemyY);
		scaleObject(barrierAwaken, 0.8f);
	}
}

// 타격 성공
void showHitEffect(SceneID scene, int x, int y, const float scale = 1.0f) {
	// 이펙트 및 사운드 부분
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

	// 타격 성공 시 처리 함수
	enemyHit();
}

// 세로 방향 레이저
// 레이저 공격 함수
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

// 레이저 경고 함수
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

// 가로 방향 레이저
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

// 레이저 관리 및 이동 관리
void laserSetting(void) {
	laserAttacking = true;

	srand((unsigned)time(NULL));

	float waitTime, laserScale;

	barrierDefault = createObject("BarrierDefault", ".\\Images\\Enemy\\barrier_default.png");
	locateObject(barrierDefault, battleScene, enemyX, enemyY);
	scaleObject(barrierDefault, 0.8f);
	showObject(barrierDefault);
	
	// 스레드로 무한 반복
	while (true) {
		// 레이저
		// 레이저 공격 전 경고 시간
		waitTime = 2 - (float)(ENEMY_MAXHP - enemyHp) / 60;
		if (waitTime < 0.5f) {
			waitTime = 0.5f;
		}

		// 레이저 크기
		laserScale = 2.0f + (float)(ENEMY_MAXHP - enemyHp) / 60;
		if (laserScale > 3.5f) {
			laserScale = 3.5f;
		}

		// 레이저 가로, 세로 결정
		int kind = rand() % 2;

		if (kind % 2) {
			int y = rand() % 431;
			showGaroLaserEffect(battleScene, 0, y, laserScale);
		}
		else {
			int x = rand() % 985;
			showSeroLaserEffect(battleScene, x, 0, laserScale);
		}
		
		// 타격 가능 시간 (초반엔 최대 4대, 후반엔 최대 1대)
		laserAttacking = false;
		if (enemyAwakening) {
			hideObject(barrierAwaken);
		}
		else {
			hideObject(barrierDefault);
		}
		std::this_thread::sleep_for(std::chrono::duration<float>(waitTime));
		
		// 사라지기 전 시간
		laserAttacking = true;
		std::this_thread::sleep_for(std::chrono::duration<float>(1.0f));

		// 이동 (이동하는 모습으로 교체하고 이동한 후, 원래 모습 + 방어막으로 다시 등장)
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

// 마우스 입력
void mouseCallback(ObjectID obj, int x, int y, MouseAction action) {
	if (obj == enemy and not laserAttacking) {
		thread Hit = thread(showHitEffect, battleScene, x, y, 0.5f);
		Hit.join();
	}
}

// ============================================================================================

int main(void) {
	// 마우스 입력 설정
	setMouseCallback(mouseCallback);

	// 전투 장면
	battleScene = createScene("Battle", "none");

	// 적
	enemy = createObject("Enemy", ".\\Images\\Enemy\\default.png");
	locateObject(enemy, battleScene, enemyX, enemyY);
	scaleObject(enemy, 0.8f);
	showObject(enemy);
	
	// 레이저 및 이동 설정
	thread Setting = thread(laserSetting);
	Setting.detach();
	
	// 게임 시작
	clock_t start = clock();
	startGame(battleScene);
}
