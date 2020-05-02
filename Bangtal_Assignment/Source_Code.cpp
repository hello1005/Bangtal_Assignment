// ============================================================================================
// == 소프트웨어학부, 20200690 박진철 ==========================================================
// ============================================================================================

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

// 클릭 한 번에 적에게 입히는 피해량
constexpr auto DAMAGE = 20;

// 배율
constexpr auto SCALE = 0.8f;

// 장면
SceneID startScene, battleScene;

// 시작 시간
clock_t startTime;

// 적
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

// 플레이어
constexpr auto PLAYER_MAXHP = static_cast<int>(ENEMY_MAXHP / DAMAGE);
int playerHp = PLAYER_MAXHP;

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

void mouseCallback(ObjectID obj, const int x, const int y, MouseAction action);

// ============================================================================================

// 각성
void awaken(void) {
	enemyAwakening = true;
	enemyAwakenOnce = true;

	barrierAwaken = createObject("BarrierAwaken", ".\\Images\\Enemy\\barrier_awaken.png");
	locateObject(barrierAwaken, battleScene, enemyX, enemyY);
	scaleObject(barrierAwaken, SCALE);
}

// 각성 관리
void awakenSetting(void) {
	enemyAwakenOnce = false;
	enemyHp = static_cast<int>(ENEMY_MAXHP * ENEMY_AWAKEN_RATIO);

	// 사라짐
	hideObject(enemy);
	hideObject(barrierAwaken);
	hideObject(barrierDefault);
	Sleep(TICK * 10);

	// 나타남
	enemyX = 565;
	enemyY = 245;
	locateObject(enemy, battleScene, enemyX, enemyY);
	showObject(enemy);
	Sleep(TICK * 20);

	// 메시지 1
	ObjectID message = createObject("Message", ".\\Images\\Others\\message1.png");
	locateObject(message, battleScene, enemyX, enemyY + 250);
	scaleObject(message, SCALE);
	showObject(message);
	Sleep(TICK * 30);
	hideObject(message);
	Sleep(TICK * 20);

	// 각성 장면
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
	Sleep(TICK * 60);
	
	for (int i = 4; i >= 1; i--) {
		Sleep(TICK);

		char imageName[60];
		sprintf_s(imageName, sizeof(imageName), ".\\Images\\Enemy\\Awakening\\%d.png", i);
		setObjectImage(awakening, imageName);
	}
	Sleep(TICK);
	hideObject(awakening);
	Sleep(TICK * 20);

	// 메시지 2
	setObjectImage(message, ".\\Images\\Others\\message2.png");
	showObject(message);
	Sleep(TICK * 30);
	hideObject(message);
	Sleep(TICK * 10);

	// 이동
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

// 타격 성공 시 적의 체력 감소 및 검사
void enemyHit(void) {
	enemyHp -= DAMAGE;

	if (enemyHp <= ENEMY_MAXHP * ENEMY_AWAKEN_RATIO and not enemyAwakening) {
		awaken();
	}
	else if (enemyHp <= 0) {
		enemyDie();
	}
}

// 몹이 죽음
void enemyDie(void) {
	enemyDied = true;

	hideObject(enemy);
	hideObject(barrierAwaken);
	hideObject(barrierDefault);
	Sleep(TICK * 10);

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
	Sleep(TICK * 40);

	clock_t usedTime = clock() - startTime;
	char text[200];
	sprintf_s(text, sizeof(text), "남은 목숨: %d개 | 소요 시간: %d초\n플레이해주셔서 감사합니다.", playerHp, static_cast<int>(usedTime / CLOCKS_PER_SEC));

	showMessage(text);
	Sleep(TICK * 40);

	endGame();
}

// 레이저 관리 및 이동 관리
void enemySetting(void) {
	laserAttacking = true;

	srand((unsigned)time(NULL));

	float waitTime, laserScale;

	barrierDefault = createObject("BarrierDefault", ".\\Images\\Enemy\\barrier_default.png");
	locateObject(barrierDefault, battleScene, enemyX, enemyY);
	scaleObject(barrierDefault, SCALE);
	showObject(barrierDefault);

	Sleep(TICK * 20);

	// 스레드로 무한 반복
	while (true) {
		// 각성했을 경우 스레드 잠시 멈추면서 각성 처리
		if (enemyAwakenOnce) {
			awakenSetting();
		}

		if (enemyDied) {
			break;
		}

		// 레이저
		// 레이저 공격 전 경고 시간
		waitTime = 2 - (float)(ENEMY_MAXHP - enemyHp) / 60;
		if (waitTime < 0.5f) {
			waitTime = 0.5f;
		}

		// 레이저 크기
		laserScale = 2.1f + (float)(ENEMY_MAXHP - enemyHp) / 60;
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

		// 레이저 1번마다 플레이어의 체력 1 감소, 체력이 0이 되면 게임 오버
		playerHp -= 1;

		if (playerHp <= 0) {
			hideObject(enemy);
			hideObject(barrierAwaken);
			hideObject(barrierDefault);

			Sleep(TICK * 20);
			showMessage("적을 빠른 시간 내로 잡지 못했습니다.\n세계가 파괴되어 게임 오버됩니다.");
			Sleep(TICK * 40);

			endGame();
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

		if (enemyAwakenOnce or enemyDied) {
			continue;
		}

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

		std::this_thread::sleep_for(std::chrono::duration<float>(0.2f));
	}
}

// ============================================================================================

// 타격 성공
void showHitEffect(SceneID scene, const int x, const int y, const float scale = 1.0f) {
	// 이펙트 및 사운드 부분
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

	// 타격 성공 시 처리 함수
	enemyHit();
}

// ============================================================================================

// 세로 방향 레이저
// 레이저 경고 함수
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

// 레이저 공격 함수
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

// 가로 방향 레이저
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

// 마우스 입력
void mouseCallback(ObjectID obj, const int x, const int y, MouseAction action) {
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
	startScene = createScene("Start", "none");
	battleScene = createScene("Battle", ".\\Images\\Others\\background_fix.jpg");

	// 적
	enemy = createObject("Enemy", ".\\Images\\Enemy\\default.png");
	locateObject(enemy, battleScene, enemyX, enemyY);
	scaleObject(enemy, SCALE);
	showObject(enemy);
	
	// 레이저 및 이동 설정
	thread enemySet = thread(enemySetting);
	enemySet.detach();
	
	// 게임 시작
	startTime = clock();
	startGame(battleScene);
}
