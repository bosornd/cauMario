#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>

#define PLAYER_ANIMATION_TIME		0.05f
#define monster3_ANIMATION_TIME		0.15f
#define PLAYER_SPEED				13
#define monster3_SPEED				1
#define monster3_NUMBER				12
#define ENDmonster3_STATE			3	//������ ���� ����(���� ������ ����)
#define PLAYER_IMAGE_SIZE			100
#define monster3_IMAGE_SIZE			100
#define GAP							13

extern SceneID titleScene;
SceneID scene_g63;
ObjectID startButton_g63, restartButton_g63, restartButton2_g63, goMapButton_g63;
ObjectID player_g63;
ObjectID monster3[monster3_NUMBER];
ObjectID countDown3;
TimerID countDown3Timer, playTimer_g63, monster3Timer_g63, growUpTimer_g63;
extern SoundID bgm_g6, countDownSound_g6, catchSound1_g6, catchSound2_g6, gameOverSound, gameClearSound, growUpSound_g6, buttonClickSound;

extern void enterTitle(int clearScene);	//Ŭ����� ������ �Լ�

extern int nowGameSceneNum;
int playerX_g63 = 600, playerY_g63 = 350;
int setMonster3X[monster3_NUMBER] = { 200,1000,200,1000,  450,750,450,750,  130,580,1040,580 };
int setMonster3Y[monster3_NUMBER] = { 550,550,150,150,    450,450,220,220,  320,570,320,40 };
int monster3X[monster3_NUMBER], monster3Y[monster3_NUMBER];
int dx_g63 = 0, dy_g63 = 0, dmx_g63 = 0, dmy_g63 = 0;

int playerState_g63 = 1;	//�÷��̾� ����(ũ��) : ó���� 1�ܰ�
int monster3State[monster3_NUMBER] = { 1,1,1,1, 2,2,2,2, 3,3,3,3 };	//���� ����
int monster3Number[ENDmonster3_STATE] = { 4,4,4 };		//state�� ���� ������
float monster3Size[monster3_NUMBER] = { 70,70,70,70, 100,100,100,100, 150,150,150,150 };
float playerSize_g63 = monster3Size[0];

bool monster3IsShown[monster3_NUMBER];

extern int num;
int monster3Count = 0;
int countDown3Num = 1;
int nowState_g63 = 1;
int growUpCount_g63 = 0;
bool stage5Clear = false;

extern ObjectID createObject(const char* name, SceneID scene, int x, int y, bool shown, float size);

extern int random();

void restart_g63() {

	playerX_g63 = 600, playerY_g63 = 350;
	dx_g63 = 0, dy_g63 = 0, dmx_g63 = 0, dmy_g63 = 0;

	playerState_g63 = 1;
	playerSize_g63 = monster3Size[0];

	monster3Count = 0;
	countDown3Num = 1;
	nowState_g63 = 1;
	growUpCount_g63 = 0;

	setTimer(countDown3Timer, 0.7f);
	setObjectImage(countDown3, "image/game6/3.png");

	locateObject(player_g63, scene_g63, playerX_g63, playerY_g63);
	scaleObject(player_g63, (float)playerSize_g63 / PLAYER_IMAGE_SIZE);
	showObject(player_g63);

	//���� ����
	for (int i = 0; i < monster3_NUMBER; i++) {
		monster3X[i] = setMonster3X[i], monster3Y[i] = setMonster3Y[i];
		locateObject(monster3[i], scene_g63, monster3X[i], monster3Y[i]);
		showObject(monster3[i]);
		monster3IsShown[i] = true;
	}

}


void gameClear_g63() {
	
	stopSound(bgm_g6);
	playSound(gameClearSound);

	stopTimer(countDown3Timer);
	stopTimer(monster3Timer_g63);
	stopTimer(playTimer_g63);
	stopTimer(growUpTimer_g63);

	hideObject(player_g63);

	showMessage("���� Ŭ����");
	stage5Clear = true;

	hideObject(restartButton2_g63);
	showObject(goMapButton_g63);

}

void gameOver_g63() {
	
	playSound(gameOverSound);

	hideObject(player_g63);			
	stopTimer(countDown3Timer);
	stopTimer(monster3Timer_g63);
	stopTimer(playTimer_g63);
	stopTimer(growUpTimer_g63);
	

	for (int i = 0; i < monster3_NUMBER; i++) {
		hideObject(monster3[i]);
	}

	hideObject(restartButton2_g63);
	showObject(restartButton_g63);
	showObject(goMapButton_g63);
}

void growUp_g63() {

	playSound(growUpSound_g6);

	startTimer(growUpTimer_g63);
}

void Game63_mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	if (object == startButton_g63) {		//Ÿ�̸� �����鼭(������Ʈ �̵� ����) ���ӽ���
		playSound(countDownSound_g6);
		showObject(restartButton2_g63);
		showObject(countDown3);
		startTimer(countDown3Timer);
		hideObject(startButton_g63);
		hideObject(goMapButton_g63);
	}

	else if (object == restartButton_g63 || object == restartButton2_g63) {
		stopTimer(monster3Timer_g63);
		restart_g63();
		playSound(countDownSound_g6);
		showObject(restartButton2_g63);
		showObject(countDown3);
		startTimer(countDown3Timer);
		hideObject(restartButton_g63);
		hideObject(goMapButton_g63);
	}

	else if (object == goMapButton_g63) {
		playSound(buttonClickSound);
		stopSound(bgm_g6);

		if (stage5Clear == true)
			enterTitle(5);
		else
			enterTitle(0);
	}
}

void Game63_keyboardCallback(KeyCode code, KeyState state)
{
	if (nowGameSceneNum == 6) {
		
		if (code == 84) {			// UP
			dy_g63 += (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
		}
		else if (code == 85) {		// DOWN
			dy_g63 -= (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
		}
		else if (code == 83) {		// RIGHT
			setObjectImage(player_g63, "image/game6/������������.png");
			dx_g63 += (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
		}
		else if (code == 82) {		// LEFT
			setObjectImage(player_g63, "image/game6/������������.png");
			dx_g63 -= (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
		}
	}
}

void Game63_timerCallback(TimerID timer)
{

	if (timer == countDown3Timer) {
		countDown3Num++;
		playSound(countDownSound_g6);

		if (countDown3Num == 2) {
			setObjectImage(countDown3, "image/game6/2.png");
			setTimer(timer, 0.7f);
			startTimer(timer);
		}
		else if (countDown3Num == 3) {
			setObjectImage(countDown3, "image/game6/1.png");
			setTimer(timer, 0.7f);
			startTimer(timer);
		}
		else {
			hideObject(countDown3);
			startTimer(playTimer_g63);
			startTimer(monster3Timer_g63);
		}
	}

	//�� ����
	if (timer == playTimer_g63) {
		if (playerX_g63 + dx_g63 > 1280 - playerSize_g63 || playerX_g63 + dx_g63 < 0 || playerY_g63 + dy_g63 > 720 - playerSize_g63 || playerY_g63 + dy_g63 < 0) {}	//�׵θ� ������ �̵� �Ƚ�Ű��

		else {
			playerX_g63 += dx_g63; playerY_g63 += dy_g63;
			locateObject(player_g63, scene_g63, playerX_g63, playerY_g63);
		}
		for (int i = 0; i < monster3_NUMBER; i++) {
			if (monster3IsShown[i] == true) {			//����(�Ⱥ��̴�) ���Ϳ��� ����x
				if (monster3X[i] - playerX_g63 <= playerSize_g63 - GAP && monster3X[i] - playerX_g63 >= -monster3Size[i] + GAP &&		//�÷��̾ ���Ͷ� x�� �ε�����
					monster3Y[i] - playerY_g63 <= playerSize_g63 - GAP && monster3Y[i] - playerY_g63 >= -monster3Size[i] + GAP) {		//y�൵ �ε�����

					if (playerState_g63 == monster3State[i]) {		//�ε����µ� ������ �Ȱ�����
						
						hideObject(monster3[i]);					//���� ����
						monster3IsShown[i] = false;				//�Ⱥ��̴� �������� �迭�� ����
						monster3Count++;

						if (monster3Count % 2 == 0)
							playSound(catchSound1_g6);
						else
							playSound(catchSound2_g6);

						if (monster3Count == monster3Number[playerState_g63 - 1]) {				//���� ���� ���� �� ���̸�
							if (playerState_g63 != ENDmonster3_STATE) {						//Ŭ��� �ƴϸ�
								nowState_g63 = playerState_g63;
								growUpCount_g63 = 0;
								growUp_g63();

								if (playerState_g63 == 1) //1�����̾�����
									playerSize_g63 = monster3Size[monster3Number[playerState_g63]];			//�÷��̾� ������ ���� ���� ���Ϳ� ���� �����
								else //2�����̾�����
									playerSize_g63 = monster3Size[monster3_NUMBER - 1];

								playerState_g63++;													//�÷��̾� ���� �ø�
								scaleObject(player_g63, (float)playerSize_g63 / PLAYER_IMAGE_SIZE);		//�÷��̾� ������ �ٲ���
								monster3Count = 0;
							}
							else {															//	<����Ŭ����>
								gameClear_g63();
								return;
							}
						}
					}

					else {										//�ε����µ� ���� ������ �� ũ��
						gameOver_g63();
						return;
					}
				}
			}
		}

		setTimer(timer, PLAYER_ANIMATION_TIME);
		startTimer(timer);

	}

	//���� ���� �̵�
	if (timer == monster3Timer_g63) {

		for (int j = 0; j < monster3_NUMBER; j++) {
			if (monster3IsShown[j] == true) {			//����(�Ⱥ��̴�) ���Ϳ��� ����x

				random();

				if (num == 0)	//���� �̵�
					dmx_g63 -= monster3_SPEED;
				else if (num == 1)	//������ �̵�
					dmx_g63 += monster3_SPEED;
				else if (num == 2)	//�Ʒ� �̵�
					dmy_g63 -= monster3_SPEED;
				else if (num == 3)	//�� �̵�
					dmy_g63 += monster3_SPEED;

				for (int i = 0; i < 32; i++) {
					if (monster3X[j] + dmx_g63 > 1185 || monster3X[j] + dmx_g63 < 15 || monster3Y[j] + dmy_g63 > 635 || monster3Y[j] + dmy_g63 < 15)	//�׵θ� ������ �̵� �Ƚ�Ű��
						break;

					else {
						monster3X[j] += dmx_g63, monster3Y[j] += dmy_g63;
						locateObject(monster3[j], scene_g63, monster3X[j], monster3Y[j]);			//32�� �ɰ��� �̵�
					}
				}
				dmx_g63 = 0, dmy_g63 = 0;	//�̵� ������ dmx_g63, dmy_g63 �ʱ�ȭ
			}
		}
		setTimer(timer, monster3_ANIMATION_TIME);
		startTimer(timer);
	}

	if (timer == growUpTimer_g63) {

		if (growUpCount_g63 % 2 == 0) {
			
			if (nowState_g63 == 1)
				playerSize_g63 = monster3Size[monster3Number[nowState_g63]];
			else 
				playerSize_g63 = monster3Size[monster3_NUMBER - 1];
		}
			
		else {
			if (nowState_g63 == 1)
				playerSize_g63 = monster3Size[0];
			else 
				playerSize_g63 = monster3Size[monster3Number[1]];

		}

		scaleObject(player_g63, (float)playerSize_g63 / PLAYER_IMAGE_SIZE);

		if (growUpCount_g63 < 4) {
			growUpCount_g63++;
			setTimer(growUpTimer_g63, 0.07f);
			startTimer(growUpTimer_g63);
		}
	}
}



void Game63_main()
{
	scene_g63 = createScene("STAGE6-3 �ϴ� ��", "image/game6/�ϴù��.png");
	player_g63 = createObject("image/game6/������������.png", scene_g63, playerX_g63, playerY_g63, true, (float)playerSize_g63 / PLAYER_IMAGE_SIZE);

	//����1 ���� ����
	for (int i = 0; i < monster3Number[0]; i++) {
		monster3X[i] = setMonster3X[i], monster3Y[i] = setMonster3Y[i];
		monster3[i] = createObject("image/game6/��������.png", scene_g63, monster3X[i], monster3Y[i], true, (float)monster3Size[i] / monster3_IMAGE_SIZE);
		monster3IsShown[i] = true;
	}
	//����2 ���� ����	
	for (int i = monster3Number[0]; i < monster3Number[0] + monster3Number[1]; i++) {
		monster3X[i] = setMonster3X[i], monster3Y[i] = setMonster3Y[i];
		monster3[i] = createObject("image/game6/�����ʷϱ���.png", scene_g63, monster3X[i], monster3Y[i], true, (float)monster3Size[i] / monster3_IMAGE_SIZE);
		monster3IsShown[i] = true;
	}
	//����3 ���� ����	
	for (int i = monster3Number[0] + monster3Number[1]; i < monster3Number[0] + monster3Number[1] + monster3Number[2]; i++) {
		monster3X[i] = setMonster3X[i], monster3Y[i] = setMonster3Y[i];
		monster3[i] = createObject("image/game6/�����ݱ���.png", scene_g63, monster3X[i], monster3Y[i], true, (float)monster3Size[i] / monster3_IMAGE_SIZE);
		monster3IsShown[i] = true;
	}

	startButton_g63 = createObject("image/game6/start.png", scene_g63, 520, 280, true, 1.0f);
	restartButton_g63 = createObject("image/game6/restart.png", scene_g63, 480, 350, false, 1.0f);
	restartButton2_g63 = createObject("image/game6/restart.png", scene_g63, 10, 10, false, 0.5f);
	goMapButton_g63 = createObject("image/game6/goMap.png", scene_g63, 20, 20, true, 1.0f);
	countDown3 = createObject("image/game6/3.png", scene_g63, 570, 300, false, 0.5f);

	playTimer_g63 = createTimer(PLAYER_ANIMATION_TIME);
	monster3Timer_g63 = createTimer(monster3_ANIMATION_TIME);
	countDown3Timer = createTimer(0.7f);
	growUpTimer_g63 = createTimer(0.07f);

}