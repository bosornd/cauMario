#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>

#define PLAYER_ANIMATION_TIME		0.05f
#define MONSTER_ANIMATION_TIME		0.15f
#define PLAYER_SPEED				13
#define MONSTER_SPEED				1
#define MONSTER_NUMBER				8
#define ENDMONSTER_STATE			2	//������ ���� ����(���� ������ ����)
#define PLAYER_IMAGE_SIZE			100
#define MONSTER_IMAGE_SIZE			100
#define GAP							10

extern SceneID scene_g62, titleScene;
extern SoundID buttonClickSound, gameOverSound;
SceneID scene_g6;
ObjectID startButton_g6, restartButton_g6, restartButton2_g6, goMapButton_g6;
ObjectID player_g6;
ObjectID monster[MONSTER_NUMBER];
ObjectID countDown;
TimerID countDownTimer, playTimer_g6, monsterTimer_g6, growUpTimer_g6;
SoundID bgm_g6, countDownSound_g6, catchSound1_g6, catchSound2_g6, growUpSound_g6, gameClearSound_g6;

extern void enterTitle(int clearScene);
extern int nowGameSceneNum;
extern int nowGame6Stage;
int playerX_g6 = 600, playerY_g6 = 350;
int setMonsterX[MONSTER_NUMBER] = { 200,600,600,1000,  420,780,420,780 };
int setMonsterY[MONSTER_NUMBER] = { 350,630,70,350,   480,480,200,200 };
int monsterX[MONSTER_NUMBER], monsterY[MONSTER_NUMBER];
int dx_g6 = 0, dy_g6 = 0, dmx = 0, dmy = 0;

int playerState_g6 = 1;	//�÷��̾� ����(ũ��) : ó���� 1�ܰ�
int monsterState[MONSTER_NUMBER] = { 1,1,1,1,2,2,2,2 };	//���� ����
int monsterNumber[ENDMONSTER_STATE] = { 4,4 };		//state�� ���� ������
float monsterSize[MONSTER_NUMBER] = { 70,70,70,70,100,100,100,100 };
float playerSize_g6 = monsterSize[0];

bool monsterIsShown[MONSTER_NUMBER];

int num;
int monsterCount = 0;
int countDownNum = 1;
int nowState_g6 = 1;
int growUpCount_g6 = 0;

extern ObjectID createObject(const char* name, SceneID scene, int x, int y, bool shown, float size);

int random() {

	//0~3
	num = rand() % 4;

	return num;
}

void restart_g6() {

	playerX_g6 = 600, playerY_g6 = 350;
	dx_g6 = 0, dy_g6 = 0, dmx = 0, dmy = 0;

	playerState_g6 = 1;	
	playerSize_g6 = monsterSize[0];

	monsterCount = 0;
	countDownNum = 1;
	nowState_g6 = 1;
	growUpCount_g6 = 0;
	
	setTimer(countDownTimer, 0.7f);
	setObjectImage(countDown, "image/game6/3.png");

	locateObject(player_g6, scene_g6, playerX_g6, playerY_g6);
	scaleObject(player_g6, (float)playerSize_g6 / PLAYER_IMAGE_SIZE);
	showObject(player_g6);

	//���� ����
	for (int i = 0; i < MONSTER_NUMBER; i++) {
		monsterX[i] = setMonsterX[i], monsterY[i] = setMonsterY[i];
		locateObject(monster[i],scene_g6, monsterX[i], monsterY[i]);
		showObject(monster[i]);
		monsterIsShown[i] = true;
	}

}

void gameClear_g6() {

	playSound(gameClearSound_g6);

	stopTimer(countDownTimer);
	stopTimer(monsterTimer_g6);
	stopTimer(playTimer_g6);
	stopTimer(growUpTimer_g6);

	nowGame6Stage = 2;
	showMessage("1 �������� Ŭ����");

	enterScene(scene_g62);

}

void gameOver_g6() {

	playSound(gameOverSound);

	hideObject(player_g6);
	stopTimer(countDownTimer);
	stopTimer(monsterTimer_g6);
	stopTimer(playTimer_g6);
	stopTimer(growUpTimer_g6);


	for (int i = 0; i < MONSTER_NUMBER; i++) {
		hideObject(monster[i]);
	}

	hideObject(restartButton2_g6);
	showObject(restartButton_g6);
	showObject(goMapButton_g6);

}

void growUp_g6() {

	playSound(growUpSound_g6);

	startTimer(growUpTimer_g6);
}

void Game6_mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	if (object == startButton_g6) {		//Ÿ�̸� �����鼭(������Ʈ �̵� ����) ���ӽ���
		playSound(countDownSound_g6);
		showObject(restartButton2_g6);
		showObject(countDown);
		startTimer(countDownTimer);
		hideObject(startButton_g6);
		hideObject(goMapButton_g6);
	}

	else if (object == restartButton_g6 || object == restartButton2_g6) {
		stopTimer(monsterTimer_g6);
		restart_g6();
		playSound(countDownSound_g6);
		showObject(restartButton2_g6);
		showObject(countDown);
		startTimer(countDownTimer);
		hideObject(restartButton_g6);
		hideObject(goMapButton_g6);
	}

	else if (object == goMapButton_g6) {
		playSound(buttonClickSound);
		stopSound(bgm_g6);
		enterTitle(0);
	}
}

void Game6_keyboardCallback(KeyCode code, KeyState state)
{
	if (nowGameSceneNum == 6) {

		if (code == 84) {			// UP
			dy_g6 += (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
		}
		else if (code == 85) {		// DOWN
			dy_g6 -= (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
		}
		else if (code == 83) {		// RIGHT
			setObjectImage(player_g6, "image/game6/������������.png");
			dx_g6 += (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
		}
		else if (code == 82) {		// LEFT
			setObjectImage(player_g6, "image/game6/������������.png");
			dx_g6 -= (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
		}
	}
}

void Game6_timerCallback(TimerID timer)
{

	if (timer == countDownTimer) {
		countDownNum++;
		playSound(countDownSound_g6);

		if (countDownNum == 2) {
			setObjectImage(countDown, "image/game6/2.png");
			setTimer(timer, 0.7f);
			startTimer(timer);
		}
		else if (countDownNum == 3) {
			setObjectImage(countDown, "image/game6/1.png");
			setTimer(timer, 0.7f);
			startTimer(timer);
		}
		else {
			hideObject(countDown);
			startTimer(playTimer_g6);
			startTimer(monsterTimer_g6);
		}
	}

	//�� ����
	if (timer == playTimer_g6) {
		if (playerX_g6 + dx_g6 > 1280 - playerSize_g6 || playerX_g6 + dx_g6 < 0 || playerY_g6 + dy_g6 > 720 - playerSize_g6 || playerY_g6 + dy_g6 < 0) {}	//�׵θ� ������ �̵� �Ƚ�Ű��

		else {
			playerX_g6 += dx_g6; playerY_g6 += dy_g6;
			locateObject(player_g6, scene_g6, playerX_g6, playerY_g6);
		}
		for (int i = 0; i < MONSTER_NUMBER; i++) {
			if (monsterIsShown[i] == true) {			//����(�Ⱥ��̴�) ���Ϳ��� ����x
				if (monsterX[i] - playerX_g6 <= playerSize_g6 - GAP && monsterX[i] - playerX_g6 >= -monsterSize[i] + GAP &&		//�÷��̾ ���Ͷ� x�� �ε�����
					monsterY[i] - playerY_g6 <= playerSize_g6 - GAP && monsterY[i] - playerY_g6 >= -monsterSize[i] + GAP) {		//y�൵ �ε�����

					if (playerState_g6 == monsterState[i]) {		//�ε����µ� ������ �Ȱ�����
						
						hideObject(monster[i]);					//���� ����
						monsterIsShown[i] = false;				//�Ⱥ��̴� �������� �迭�� ����
						monsterCount++;

						if (monsterCount % 2 == 0)
							playSound(catchSound1_g6);
						else 
							playSound(catchSound2_g6);

						if (monsterCount == monsterNumber[playerState_g6 - 1]) {				//���� ���� ���� �� ���̸�
							if (playerState_g6 != ENDMONSTER_STATE) {						//Ŭ��� �ƴϸ�
								nowState_g6 = playerState_g6;
								growUp_g6();
								
								playerSize_g6 = monsterSize[monsterNumber[playerState_g6]];			//�÷��̾� ������ ���� ���� ���Ϳ� ���� �����
								playerState_g6++;													//�÷��̾� ���� �ø�
								scaleObject(player_g6, (float)playerSize_g6 / PLAYER_IMAGE_SIZE);		//�÷��̾� ������ �ٲ���
								monsterCount = 0;
							}
							else {															//	<����Ŭ����>
								gameClear_g6();
								return;
							}
						}
					}

					else {										//�ε����µ� ���� ������ �� ũ��
						gameOver_g6();
						return;
					}
				}
			}
		}

		setTimer(timer, PLAYER_ANIMATION_TIME);
		startTimer(timer);

	}

	//���� ���� �̵�
	if (timer == monsterTimer_g6) {

		for (int j = 0; j < MONSTER_NUMBER; j++) {
			if (monsterIsShown[j] == true) {			//����(�Ⱥ��̴�) ���Ϳ��� ����x

				random();

				if (num == 0)	//���� �̵�
					dmx -= MONSTER_SPEED;
				else if (num == 1)	//������ �̵�
					dmx += MONSTER_SPEED;
				else if (num == 2)	//�Ʒ� �̵�
					dmy -= MONSTER_SPEED;
				else if (num == 3)	//�� �̵�
					dmy += MONSTER_SPEED;

				for (int i = 0; i < 32; i++) {
					if (monsterX[j] + dmx > 1185 || monsterX[j] + dmx < 15 || monsterY[j] + dmy > 635 || monsterY[j] + dmy < 15)	//�׵θ� ������ �̵� �Ƚ�Ű��
						break;

					else {
						monsterX[j] += dmx, monsterY[j] += dmy;
						locateObject(monster[j], scene_g6, monsterX[j], monsterY[j]);			//32�� �ɰ��� �̵�
					}
				}
				dmx = 0, dmy = 0;	//�̵� ������ dmx, dmy �ʱ�ȭ
			}
		}
		setTimer(timer, MONSTER_ANIMATION_TIME);
		startTimer(timer);
	}

	if (timer == growUpTimer_g6) {

		if (growUpCount_g6 % 2 == 0) {
			playerSize_g6 = monsterSize[monsterNumber[nowState_g6]];
		}

		else {
			playerSize_g6 = monsterSize[0];
		}

		scaleObject(player_g6, (float)playerSize_g6 / PLAYER_IMAGE_SIZE);

		if (growUpCount_g6 < 4) {
			growUpCount_g6++;
			setTimer(growUpTimer_g6, 0.07f);
			startTimer(growUpTimer_g6);
		}
	}
}

void Game6_soundCallback(SoundID sound) {

	if (sound == bgm_g6)
		playSound(bgm_g6);

}


void Game6_main()
{
	scene_g6 = createScene("STGAGE5-1 �ϴ� ��", "image/game6/�ϴù��.png");
	player_g6 = createObject("image/game6/������������.png", scene_g6, playerX_g6, playerY_g6, true, (float)playerSize_g6 / PLAYER_IMAGE_SIZE);

	//����1 ���� ����
	for (int i = 0; i < monsterNumber[0]; i++) {
		monsterX[i] = setMonsterX[i], monsterY[i] = setMonsterY[i];
		monster[i] = createObject("image/game6/��������.png", scene_g6, monsterX[i], monsterY[i], true, (float)monsterSize[i] / MONSTER_IMAGE_SIZE);
		monsterIsShown[i] = true;
	}
	//����2 ���� ����	
	for (int i = monsterNumber[0]; i < monsterNumber[0] + monsterNumber[1]; i++) {
		monsterX[i] = setMonsterX[i], monsterY[i] = setMonsterY[i];
		monster[i] = createObject("image/game6/�����ʷϱ���.png", scene_g6, monsterX[i], monsterY[i], true, (float)monsterSize[i] / MONSTER_IMAGE_SIZE);
		monsterIsShown[i] = true;
	}

	startButton_g6 = createObject("image/game6/start.png", scene_g6, 520, 280, true, 1.0f);
	restartButton_g6 = createObject("image/game6/restart.png", scene_g6, 480, 350, false, 1.0f);
	restartButton2_g6 = createObject("image/game6/restart.png", scene_g6, 10, 10, false, 0.5f);
	goMapButton_g6 = createObject("image/game6/goMap.png", scene_g6, 20, 20, true, 1.0f);
	countDown = createObject("image/game6/3.png", scene_g6, 570, 300, false, 0.5f);

	playTimer_g6 = createTimer(PLAYER_ANIMATION_TIME);
	monsterTimer_g6 = createTimer(MONSTER_ANIMATION_TIME);
	countDownTimer = createTimer(0.7f);
	growUpTimer_g6 = createTimer(0.07f);

	bgm_g6 = createSound("sounds/�����/�ϴü�.mp3");
	countDownSound_g6 = createSound("sounds/game6/ī��Ʈ�ٿ�.mp3");
	catchSound1_g6 = createSound("sounds/game6/����1.mp3");
	catchSound2_g6 = createSound("sounds/game6/����2.mp3");
	growUpSound_g6 = createSound("sounds/game6/�Ŀ���.wav");
	gameClearSound_g6 = createSound("sounds/game6/��������Ŭ����.mp3");
}