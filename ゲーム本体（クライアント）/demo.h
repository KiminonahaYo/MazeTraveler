/*
	Maze Traveler�^�C�g�����
*/
//�w�b�_�[�t�@�C���ǂݍ���
#include "Defines.h"
#include "MyMaze.h"
#include "DrawMoji.h"
#include "Map2Dim.h"
#include "BackGround.h"

#define Short_Air_X 40
#define Short_Air_Z 50

//�f�����X�g���[�V�����\���N���X
class MazeTitle
{
private:
	//���@�ƃJ�����ϐ�
	JIKI Jiki;
	CAMERA Camera;

	MAPITEM item;

	MAP map;

	int StartGoalFlag = 1;		//Start:1, Goal:2, Other:0

public:
	int Disp(void);

private:
	void disp(bool);
	void disp_3d(bool);
	void disp_2dmap(bool, bool);
	void disp_jiki(void);
	void disp_item(bool);

	//���@�𓮂����A�J�����ƘA��������B
	void JikiandCamMove(void);

	void PosSet_def(void);
}
Title;

//���@�ƃJ�����̈ʒu������������B
void MazeTitle::PosSet_def(void)
{
	JIKI _Jiki = { { Masu_1 / 2.0, 0, Masu_1*2.0 + Masu_1 / 2.0 } };
	CAMERA _Camera = { { 0.0, 50.0, 0.0 }, 270, 100 };

	Jiki = _Jiki;
	Camera = _Camera;

	Camera.Pos.x = sin(RAD1*Camera.Kakudo) * Camera.Hankei + Jiki.Pos.x;
	Camera.Pos.z = cos(RAD1*Camera.Kakudo) * Camera.Hankei + Jiki.Pos.z;
}

//�^�C�g���ƃf�������݂ɕ\��
int MazeTitle::Disp(void)
{
	//-------------------------------------------------------------------------------------------

	//�ϐ��錾

	//�ėp�ϐ�
	//float x, y, z;
	int i, j, a;
	//int Room_MasuZ, Room_MasuX, Room_MasuY = 1;

	int Jiki_MasuX, Jiki_MasuZ;
	int Jiki_MasuX_before, Jiki_MasuZ_before;

	//�f�����X�g���[�V�����֌W
	int CountStartTime;
	bool demostartedflag = false;

	int seed;

	int FrameCnt = 0;
	bool MapFlag = false;
	bool keymode = false;

	int nowtime, nolmatime;
	char stagename[256] = "";

	int level;

	bool brkflag = false;			//�~����������Esc���������肵�����A�v���O�������I�����邩
	bool Endflag = false;			//Enter�������ăQ�[�����J�n����t���O�i���[�v�����ă��j���[��ʂցj

	VECTOR Center;					//�Q�����}�b�v����
	double rot;						//�Q�����}�b�v�p�x

	//�L�[�t���O
	char KeyFlag[256];

	//-------------------------------------------------------------------------------------------

	while (1)
	{
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
			brkflag = true;

		if (brkflag)
			break;

		if (Endflag)
			break;

		//���@�ƃJ�����̏�����
		PosSet_def();

		Jiki_MasuX = int(Jiki.Pos.x / Masu_1);
		Jiki_MasuZ = int(Jiki.Pos.z / Masu_1);
		Jiki_MasuX_before = Jiki_MasuX;
		Jiki_MasuZ_before = Jiki_MasuZ;

		StartGoalFlag = 1;

		/*
		Jiki.Pos.x = Masu_1 / 2.0;
		Jiki.Pos.y = 0;
		Jiki.Pos.z = Masu_1*2.0 + Masu_1 / 2.0;

		Camera.Pos.x = 0.0;
		Camera.Pos.y = 50.0;
		Camera.Pos.z = 0.0;
		Camera.Kakudo = 270 + 360 * 10;
		Camera.Hankei = 100;
		*/

		//���x���ƃV�[�h�l������
		level = RangeRand(1,10);
		decidemazesize_bylevel(level, &map.x, &map.z);
		seed = rand();
		sprintf_s(stagename, 256, "%02d-%05d", level, seed);

		map.y = 1;

		keymode = RangeRand(0, 1) ? true : false;

		//���H���쐬
		GetMazeArray(&map.data, map.x, map.z);												//���H�p�̔z����m��
		GetMazeArray(&map.visited, map.x, map.z);											//�K��ς݂̏ꏊ���i�[����z����쐬
		GetMazeArray(&map.ans, map.x, map.z);												//�K��ς݂̏ꏊ���i�[����z����쐬
		MakeMaze(map.data, seed, map.x, map.z);												//���H���쐬

		item.map.isgotten = false;
		item.ans.isgotten = false;
		item.key.isgotten = false;

		//�}�b�v�A�C�e���̍��W������
		do
		{
			//�}�b�v�A�C�e�����W����(x, z����2�`10�͈̔�)
			item.map.x = 2 + (rand() % 5) * 2;
			item.map.z = 2 + (rand() % 5) * 2;

			//�}�b�v�A�C�e���܂ł̂��ǂ�}�X�����Z�o
			a = 0;
			SolveMaze(map.data, map.ans, map.x, map.z, 0, 2, item.map.x, item.map.z);			//���H�̓������쐬
			for (i = 0; i < map.x; i++)
			{
				for (j = 0; j < map.z; j++)
				{
					if (map.ans[i][j] == 1)
					{
						a++;
					}
				}
			}
			//�}�b�v�A�C�e���܂ŉ���������A���T�[�A�C�e���Ɣ�����ꍇ��蒼��
		} while (a >= 20 || (item.map.x == 2 && item.map.z == 2));

		//�A���T�[�A�C�e���̓X�^�[�g�̖ڂ̑O
		item.ans.x = 2;
		item.ans.z = 2;

		//���̍��W������
		do
		{
			item.key.x = 2 + (rand() % ((map.x + 1) / 2 - 2)) * 2;
			item.key.z = 2 + (rand() % ((map.z + 1) / 2 - 2)) * 2;
		} while (
			(item.key.x == item.map.x && item.key.z == item.map.z) || 
			(item.key.x == item.ans.x && item.key.z == item.ans.z) || 
			(item.key.x < map.x / 2 && item.key.z < map.z / 2));

		//�Q�����}�b�v�����ݒ�
		Map2Dim.Init(map, 2.5, &item);
		Map2Dim.SetScreenSize(SCREEN_X, SCREEN_Y);

		//���C�g�n���h���̏�����
		L1 = CreateSpotLightHandle(
			Camera.Pos,
			VGet(Degsin(Camera.Kakudo + 180), 0.0, Degcos(Camera.Kakudo + 180)),
			DX_PI_F,
			DX_PI_F / 2.0,
			1500.0,
			1.0f,
			0.0005f,
			0.0f
			);

		//�K��ς݂̏ꏊ�͂Ȃ��̂ł��ׂĖ��K��ɂ���i�������j
		for (i = 0; i < map.x; i++)
		{
			for (j = 0; j < map.z; j++)
			{
				map.visited[i][j] = 0;
			}
		}

		//�ڕW���Ԃ�����i�m���}���ԁj
		//�ʏ탂�[�h�F�X�^�[�g���}�b�v�@���@�S�[���̏��ɒH�����Ƃ��ĎZ�o
		//�@�����[�h�F�X�^�[�g���}�b�v�������S�[���̏��ɒH�����Ƃ��ĎZ�o
		//�P�}�X�Q�b�Ő������[�g��H�����獇�i�_�ł��B
		{
			nolmatime = 0;
			a = 0;

			//�X�^�[�g����}�b�v�܂ł̌o�H
			SolveMaze(map.data, map.ans, map.x, map.z, 0, 2, item.map.x, item.map.z);			//���H�̓������쐬
			for (i = 0; i < map.x; i++)
			{
				for (j = 0; j < map.z; j++)
				{
					if (map.ans[i][j] == 1)
					{
						nolmatime += 2;
					}
				}
			}

			if (keymode == false)
			{
				//�}�b�v����S�[���܂ł̌o�H
				SolveMaze(map.data, map.ans, map.x, map.z, item.map.x, item.map.z, map.x - 1, map.z - 3);			//���H�̓������쐬
				for (i = 0; i < map.x; i++)
				{
					for (j = 0; j < map.z; j++)
					{
						if (map.ans[i][j] == 1)
						{
							nolmatime += 2;
						}
					}
				}
			}
			else
			{
				//�}�b�v���献�܂ł̌o�H
				SolveMaze(map.data, map.ans, map.x, map.z, item.map.x, item.map.z, item.key.x, item.key.z);			//���H�̓������쐬
				for (i = 0; i < map.x; i++)
				{
					for (j = 0; j < map.z; j++)
					{
						if (map.ans[i][j] == 1)
						{
							nolmatime += 2;
						}
					}
				}
				//������S�[���܂ł̌o�H
				SolveMaze(map.data, map.ans, map.x, map.z, item.key.x, item.key.z, map.x - 1, map.z - 3);			//���H�̓������쐬
				for (i = 0; i < map.x; i++)
				{
					for (j = 0; j < map.z; j++)
					{
						if (map.ans[i][j] == 1)
						{
							nolmatime += 2;
						}
					}
				}
			}
		}

		//�^�C�g�����

		//������
		FrameCnt = 0;
		Center.x = Masu_1 * map.x / 2;
		Center.y = 0.0;
		Center.z = Masu_1 * map.z / 2;
		rot = 0;
		//SolveMaze(map.data, map.ans, map.x, map.z, 0, 2, map.x - 1, map.z - 3);			//���H�̓������쐬

		//�\�����[�v
		while (1)
		{
			if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
				brkflag = true;

			if (brkflag)
				break;

			if (Stick(KEY_INPUT_RETURN))
				Endflag = true;

			if (Endflag == true) break;

			//�P�O�b�o������f�����X�g���[�V�����ڍs
			if (FrameCnt >= 60 * 10)
				break;

			//���z��ʂɕ\��
			SetDrawScreen(Handle_Screen_Normal);

			ClearDrawScreen();

			//�����w�i��\��
			DrawBackGround(SCREEN_X, SCREEN_Y);

			//���H��2�����}�b�v�\��
			Map2Dim.disp(Center, rot, false, false);

			//�f�����X�g���[�V���������Ă��Ȃ��Ƃ���1�b50�ɂȂ�����G���^�[�𑣂���ʂ�\��
			//�����ꍇ�͍ŏ�����\��
			if (FrameCnt >= 90 || demostartedflag == true)
			{
				//�^�C�g���摜
				DrawExtendGraph(SCREEN_X / 6, SCREEN_Y / 6, SCREEN_X / 6 * 5, SCREEN_Y / 3, Handle_Title, false);
				if (FrameCnt % 60 >= 30)
				{
					//�G���^�[�L�[�𑣂������Ɖ摜
					DrawExtendGraph(SCREEN_X / 7, SCREEN_Y / 6 * 4, SCREEN_X / 7 * 6, SCREEN_Y / 6 * 4 + 60, Handle_Title_DispAssist, false);
					DispStringOfBox(SCREEN_X / 7 + 5, SCREEN_Y / 6 * 4 + 5, 1.0, -1, "PRESS ENTER KEY...");
				}
				//�f�����X�g���[�V�����̃J�E���g�_�E����\��
				if (FrameCnt < 60 * 9 - 30)
					DispStringOfBox(20, SCREEN_Y - 80, 1.5, -1, "%d TO DEMO", 10 - (FrameCnt + 90) / 60);
				else
					DispStringOfBox(20, SCREEN_Y - 80, 1.5, -1, "DEMO SOON");
			}

			//1/60�b���Ƃ�12�x���H�̃}�b�v�����B
			if (FrameCnt < 90)
				rot += 12;

			//�ʏ헠��ʂɐ؂�ւ�
			SetDrawScreen(DX_SCREEN_BACK);

			//�f�����X�g���[�V���������āA�ēx�^�C�g����ʂ�\��������ꍇ�t�F�[�h�C��
			if (FrameCnt < 90 && demostartedflag == true)
			{
				SetDrawBright(
					255 * (double(FrameCnt) / 90.0),
					255 * (double(FrameCnt) / 90.0),
					255 * (double(FrameCnt) / 90.0)
					);
			}
			//�^�C�g����ʂ�8�b50�Ńt�F�[�h�A�E�g
			else if (FrameCnt >= 60 * 9 - 30)
			{
				SetDrawBright(
					255 * (1.0 - double(FrameCnt - (60 * 9 - 30)) / 90.0),
					255 * (1.0 - double(FrameCnt - (60 * 9 - 30)) / 90.0),
					255 * (1.0 - double(FrameCnt - (60 * 9 - 30)) / 90.0)
					);
			}
			//�؂�ւ��ȊO�͒ʏ�\��
			else
			{
				SetDrawBright(255, 255, 255);
			}
			DrawGraph(0, 0, Handle_Screen_Normal, false);

			FrameCnt++;

			//�\��ʂɕ\��
			ScreenFlip();
		}

		//�^�C�g����ʁi�f�����X�g���[�V�����j
		MapFlag = false;
		demostartedflag = true;
		FrameCnt = 0;
		CountStartTime = GetNowCount();
		Map2Dim.SetBai(1.0);
		SolveMaze(map.data, map.ans, map.x, map.z, 0, 2, item.map.x, item.map.z);			//���H�̓������쐬

		while (1)
		{
			if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
				brkflag = true;

			if (brkflag)
				break;

			if (Stick(KEY_INPUT_RETURN))
				Endflag = true;

			if (Endflag == true) break;

			SetDrawScreen(Handle_Screen_Normal);

			ClsDrawScreen();		//�`�悷���ʁi���j�����N���A

			GetHitKeyStateAll(KeyFlag);

			if ((FrameCnt + 1) % 1000 == 0)		MapFlag = !MapFlag;
			if ((FrameCnt + 1) % 1000 == 800)	MapFlag = !MapFlag;

			if (StartGoalFlag == 0)
				nowtime = (GetNowCount() - CountStartTime) / 1000;
			else if (StartGoalFlag == 1)
				nowtime = 0;

			//���@�̈ړ�
			if (StartGoalFlag == 0)
			{
				JikiandCamMove();
			}
			else if (StartGoalFlag == 2)
			{
				JikiandCamMove();
			}

			Jiki_MasuZ_before = Jiki_MasuX;
			Jiki_MasuZ_before = Jiki_MasuZ;
			Jiki_MasuX = int(Jiki.Pos.x / Masu_1);
			Jiki_MasuZ = int(Jiki.Pos.z / Masu_1);
		
			if (Jiki_MasuX != Jiki_MasuX_before || Jiki_MasuZ != Jiki_MasuZ_before)
			{
				//�}�b�v����ɓ���Ă��Ȃ�������}�b�v�܂ł̌o�H���v�Z
				if (item.map.isgotten == false)
				{
					SolveMaze(map.data, map.ans, map.x, map.z, Jiki_MasuX, Jiki_MasuZ, item.map.x, item.map.z);			//���H�̓������쐬
				}
				else
				{
					if (keymode == false)
					{
						SolveMaze(map.data, map.ans, map.x, map.z, Jiki_MasuX, Jiki_MasuZ, map.x - 1, map.z - 3);			//���H�̓������쐬
					}
					else
					{
						//������ɓ���Ă��Ȃ������献�܂ł̌o�H���v�Z
						if (item.key.isgotten == false)
						{
							SolveMaze(map.data, map.ans, map.x, map.z, Jiki_MasuX, Jiki_MasuZ, item.key.x, item.key.z);			//���H�̓������쐬
						}
						else
						{
							SolveMaze(map.data, map.ans, map.x, map.z, Jiki_MasuX, Jiki_MasuZ, map.x - 1, map.z - 3);			//���H�̓������쐬
						}
					}
				}
			}


			//�X�^�[�g�A�S�[���̊Ǘ�
			if (StartGoalFlag == 0)
			{
				//JikiandCamMove();

				if ((int)(Jiki.Pos.x / Masu_1) == map.x - 1 && (int)(Jiki.Pos.z / Masu_1) == map.z - 3)
				{
					CountStartTime = GetNowCount();
					StartGoalFlag = 2;
				}
			}
			else if (StartGoalFlag == 1)
			{
				if (GetNowCount() - CountStartTime >= 4000)
				{
					CountStartTime = GetNowCount();
					StartGoalFlag = 0;
				}
			}
			else if (StartGoalFlag == 2)
			{
				//JikiandCamMove();

				if (GetNowCount() - CountStartTime > 4000) break;
			}

			//-------------------------------------------------------------------------------------------

			//�\������
			disp(keymode);

			if (StartGoalFlag != 1)
			{
				//���݂̌o�ߎ��ԂƖڕW���Ԃ�\��
				DrawGraph(10, 10, Handle_TimeDisp_Tile, false);
				DispStringOfBox(10 + 10, 10 + 10, 1.0, 6, "%s", stagename);

				DispStringOfBox(10 + 140, 10 + 70, 1.0, 7, "%01d", nolmatime / 3600);
				DispStringOfBox(10 + 200, 10 + 70, 1.0, 3, "%02d", nolmatime / 60 % 60);
				DispStringOfBox(10 + 300, 10 + 70, 1.0, 0, "%02d", nolmatime % 60);

				DispStringOfBox(10 + 140, 10 + 140, 1.0, 7, "%01d", nowtime / 3600);
				DispStringOfBox(10 + 200, 10 + 140, 1.0, 3, "%02d", nowtime / 60 % 60);
				DispStringOfBox(10 + 300, 10 + 140, 1.0, 0, "%02d", nowtime % 60);

				DrawGraph(SCREEN_X - 120 - 10, SCREEN_Y - 100 - 10, Handle_Gotitem_Tile, false);

				if (item.map.isgotten == true)
					DrawGraph(SCREEN_X - 120 - 10 + 10 + 0, SCREEN_Y - 80 - 10 + 5, Handle_Gotitem_Map, false);
				if (item.ans.isgotten == true)
					DrawGraph(SCREEN_X - 120 - 10 + 10 + 35, SCREEN_Y - 80 - 10 + 5, Handle_Gotitem_Ans, false);
				if (item.key.isgotten == true && keymode == true)
					DrawGraph(SCREEN_X - 120 - 10 + 10 + 70, SCREEN_Y - 80 - 10 + 5, Handle_Gotitem_Key, false);

				if (keymode)
					DrawGraph(SCREEN_X - 60, 10, Handle_Challanging_Keymode, false);
			}

			//�G���^�[�L�[�𑣂������Ɖ摜
			if (FrameCnt % 60 < 30)
			{
				DrawExtendGraph(SCREEN_X / 7, SCREEN_Y / 6 * 4, SCREEN_X / 7 * 6, SCREEN_Y / 6 * 4 + 60, Handle_Title_DispAssist, false);
				DispStringOfBox(SCREEN_X / 7 + 5, SCREEN_Y / 6 * 4 + 5, 1.0, -1, "PRESS ENTER KEY...");
				//DrawGraph(SCREEN_X/2-300, SCREEN_Y/5*3, Handle_PressEnterKey, true);
			}
			if (FrameCnt % 30 < 15)
			{
				DrawExtendGraph(10, SCREEN_Y - 70, 350, SCREEN_Y - 10, Handle_Title_DispAssist, false);
				DispStringOfBox(10 + 5, SCREEN_Y - 70 + 5, 1.0, -1, "DEMO NOW");
			}

			if (StartGoalFlag == 2)
			{
				if (GetNowCount() - CountStartTime < 1000)
				{
					DrawExtendGraph(
						SCREEN_X / 2 - 320,
						SCREEN_Y / 2 - 50 * (double(GetNowCount() - CountStartTime) / 1000.0),
						SCREEN_X / 2 + 320,
						SCREEN_Y / 2 + 50 * (double(GetNowCount() - CountStartTime) / 1000.0),
						Handle_Goaled,
						false);
				}
				else
				{
					DrawExtendGraph(SCREEN_X / 2 - 320, SCREEN_Y / 2 - 50, SCREEN_X / 2 + 320, SCREEN_Y / 2 + 50, Handle_Goaled, false);
				}
			}

			//�����������܂�
			//-------------------------------------------------------------------------------------------

			SetDrawScreen(DX_SCREEN_BACK);

			DrawGraph(0, 0, Handle_Screen_Normal, false);

			if (FrameCnt < 90)
			{
				SetDrawBright(
					255 * (double(FrameCnt) / 90.0),
					255 * (double(FrameCnt) / 90.0),
					255 * (double(FrameCnt) / 90.0)
					);
			}
			else if ((GetNowCount() - CountStartTime) >= 3000 && StartGoalFlag == 2)
			{
				SetDrawBright(
					255 * (1.0 - double(GetNowCount() - CountStartTime - 3000) / 1000.0),
					255 * (1.0 - double(GetNowCount() - CountStartTime - 3000) / 1000.0),
					255 * (1.0 - double(GetNowCount() - CountStartTime - 3000) / 1000.0)
					);
			}
			else
			{
				SetDrawBright(255, 255, 255);
			}

			FrameCnt++;

			//�\�Ɨ��i�`���ʁj�̓��ꊷ��
			ScreenFlip();
		}
		//���C�����[�v�I��

		//StartGoalFlag = 1;

		//��n��
		Map2Dim.Dest();
		DeleteMazeArray(map.data, map.x);
		DeleteMazeArray(map.visited, map.x);
		DeleteMazeArray(map.ans, map.x);
		DeleteLightHandle(L1);
	}
	//-------------------------------------------------------------------------------------------

	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawBright(255, 255, 255);

	if (brkflag)
		return -1;
	else
		return 0;
}

void MazeTitle::JikiandCamMove(void)
{
	static bool MoveFlag = true;
	static bool DefSetFlag = true;
	static int kend;

	int hokox, hokoz;
	int hokox1, hokoz1;
	int hokox2, hokoz2;
	int hokox3, hokoz3;

	//�ڂ̑O�ɕǂ��Ȃ�������
	if (MoveFlag == true)
	{
		DefSetFlag = true;

		switch ((int)(Amarif(Camera.Kakudo, 360)))
		{
		case 0:
			hokox = 0;
			hokoz = -1;
			break;
		case 90:
			hokox = -1;
			hokoz = 0;
			break;
		case 180:
			hokox = 0;
			hokoz = 1;
			break;
		case 270:
			hokox = 1;
			hokoz = 0;
			break;
		}

		if ((int)((Jiki.Pos.x + Masu_1 / 2.0) / Masu_1) >= 0 && (int)((Jiki.Pos.x + Masu_1 / 2.0) / Masu_1) < map.x &&
			(int)((Jiki.Pos.z + Masu_1 / 2.0) / Masu_1) >= 0 && (int)((Jiki.Pos.z + Masu_1 / 2.0) / Masu_1) < map.z)
		{
			if (
				(hokox != 0)
				? (
				(hokox == 1)
				? (map.ans[(int)((Jiki.Pos.x - Masu_1 / 2.0) / Masu_1) + hokox][(int)(Jiki.Pos.z / Masu_1) + hokoz] != 1)
				: (map.ans[(int)((Jiki.Pos.x + Masu_1 / 2.0) / Masu_1) + hokox][(int)(Jiki.Pos.z / Masu_1) + hokoz] != 1)
				)
				: (
				(hokoz == 1)
				? (map.ans[(int)(Jiki.Pos.x / Masu_1) + hokox][(int)((Jiki.Pos.z - Masu_1 / 2.0) / Masu_1) + hokoz] != 1)
				: (map.ans[(int)(Jiki.Pos.x / Masu_1) + hokox][(int)((Jiki.Pos.z + Masu_1 / 2.0) / Masu_1) + hokoz] != 1)
				)
				)MoveFlag = false;
		}

		if ((int)((Jiki.Pos.x + Masu_1 / 2.0) / Masu_1) >= 0 && (int)((Jiki.Pos.x + Masu_1 / 2.0) / Masu_1) < map.x &&
			(int)((Jiki.Pos.z + Masu_1 / 2.0) / Masu_1) >= 0 && (int)((Jiki.Pos.z + Masu_1 / 2.0) / Masu_1) < map.z)
		{
			if (
				!(
				(hokox != 0)
				? (
				(hokox == 1)
				? (map.ans[(int)((Jiki.Pos.x - Masu_1 / 2.0) / Masu_1) + hokox][(int)(Jiki.Pos.z / Masu_1) + hokoz] != 1)
				: (map.ans[(int)((Jiki.Pos.x + Masu_1 / 2.0) / Masu_1) + hokox][(int)(Jiki.Pos.z / Masu_1) + hokoz] != 1)
				)
				: (
				(hokoz == 1)
				? (map.ans[(int)(Jiki.Pos.x / Masu_1) + hokox][(int)((Jiki.Pos.z - Masu_1 / 2.0) / Masu_1) + hokoz] != 1)
				: (map.ans[(int)(Jiki.Pos.x / Masu_1) + hokox][(int)((Jiki.Pos.z + Masu_1 / 2.0) / Masu_1) + hokoz] != 1)
				)
				)
				)
				//����
			{
				Jiki.Pos.x -= sin(RAD1*Camera.Kakudo) * 4;
				Jiki.Pos.z -= cos(RAD1*Camera.Kakudo) * 4;
			}
		}
	}
	//�ǂɂԂ�������
	else
	{
		if (DefSetFlag == true)
		{
			switch ((int)(Amarif(Camera.Kakudo - 90, 360)))
			{
			case 0:
				hokox1 = 0;
				hokoz1 = -1;
				break;
			case 90:
				hokox1 = -1;
				hokoz1 = 0;
				break;
			case 180:
				hokox1 = 0;
				hokoz1 = 1;
				break;
			case 270:
				hokox1 = 1;
				hokoz1 = 0;
				break;
			}
			switch ((int)(Amarif(Camera.Kakudo + 90, 360)))
			{
			case 0:
				hokox2 = 0;
				hokoz2 = -1;
				break;
			case 90:
				hokox2 = -1;
				hokoz2 = 0;
				break;
			case 180:
				hokox2 = 0;
				hokoz2 = 1;
				break;
			case 270:
				hokox2 = 1;
				hokoz2 = 0;
				break;
			}
			switch ((int)(Amarif(Camera.Kakudo + 180, 360)))
			{
			case 0:
				hokox3 = 0;
				hokoz3 = -1;
				break;
			case 90:
				hokox3 = -1;
				hokoz3 = 0;
				break;
			case 180:
				hokox3 = 0;
				hokoz3 = 1;
				break;
			case 270:
				hokox3 = 1;
				hokoz3 = 0;
				break;
			}

			if (map.ans[(int)(Jiki.Pos.x / Masu_1) + hokox1][(int)(Jiki.Pos.z / Masu_1) + hokoz1] == 1)			kend = Camera.Kakudo - 90;
			else if (map.ans[(int)(Jiki.Pos.x / Masu_1) + hokox2][(int)(Jiki.Pos.z / Masu_1) + hokoz2] == 1)	kend = Camera.Kakudo + 90;
			else if (map.ans[(int)(Jiki.Pos.x / Masu_1) + hokox3][(int)(Jiki.Pos.z / Masu_1) + hokoz3] == 1)	kend = Camera.Kakudo + 180;
			else																								kend = Camera.Kakudo + 0;

			DefSetFlag = false;
		}

		//������ς���
		if (Camera.Kakudo<kend)			Camera.Kakudo += 3;
		else if (Camera.Kakudo>kend)	Camera.Kakudo -= 3;
		else							MoveFlag = true;
	}

	Camera.Pos.x = sin(RAD1*Camera.Kakudo) * Camera.Hankei + Jiki.Pos.x;
	Camera.Pos.z = cos(RAD1*Camera.Kakudo) * Camera.Hankei + Jiki.Pos.z;

	if (int(Jiki.Pos.x / Masu_1) == item.map.x && int(Jiki.Pos.z / Masu_1) == item.map.z && item.map.isgotten == false)
		item.map.isgotten = true;

	if (int(Jiki.Pos.x / Masu_1) == item.ans.x && int(Jiki.Pos.z / Masu_1) == item.ans.z && item.ans.isgotten == false)
		item.ans.isgotten = true;

	if (int(Jiki.Pos.x / Masu_1) == item.key.x && int(Jiki.Pos.z / Masu_1) == item.key.z && item.key.isgotten == false)
		item.key.isgotten = true;

	//���݂̏ꏊ��K��ς݂ɂ���
	map.visited[int(Jiki.Pos.x / 100.0)][int(Jiki.Pos.z / 100.0)] = 1;
}

void MazeTitle::disp_jiki(void)
{
	static bool passed = false;
	static VECTOR before;
	static double rot = 0.0;
	static double rotd = 4.0;
	VECTOR Position;

	if (passed == false)
		before = Jiki.Pos;

	if (before.x != Jiki.Pos.x || before.z != Jiki.Pos.z)
	{
		rot += rotd;
		if (rot >= 60.0) rotd = -4.0;
		if (rot <= -60.0) rotd = 4.0;
		before = Jiki.Pos;

		Position = Jiki.Pos;
		MV1SetPosition(Handle_Jiki_Moving_Body, Position);
		MV1SetScale(Handle_Jiki_Moving_Body, VGet(1.0, 1.0, 1.0));
		MV1SetRotationXYZ(Handle_Jiki_Moving_Body, VGet(0.0, DegtoRad(Camera.Kakudo - 180.0) + DegtoRad(-90.0), 0.0));
		MV1DrawModel(Handle_Jiki_Moving_Body);

		Position = VGet(
			Jiki.Pos.x + Degsin(Camera.Kakudo + 90) * 4,
			Jiki.Pos.y + 12,
			Jiki.Pos.z + Degcos(Camera.Kakudo + 90) * 4
			);
		MV1SetPosition(Handle_Jiki_Moving_Foot, Position);
		MV1SetScale(Handle_Jiki_Moving_Foot, VGet(1.0, 1.0, 1.0));
		MV1SetRotationXYZ(Handle_Jiki_Moving_Foot, VGet(DegtoRad(rot), DegtoRad(Camera.Kakudo - 180.0), 0.0));
		MV1DrawModel(Handle_Jiki_Moving_Foot);

		Position = VGet(
			Jiki.Pos.x + Degsin(Camera.Kakudo - 90) * 6,
			Jiki.Pos.y + 20,
			Jiki.Pos.z + Degcos(Camera.Kakudo - 90) * 6
			);
		MV1SetPosition(Handle_Jiki_Moving_Arm, Position);
		MV1SetScale(Handle_Jiki_Moving_Arm, VGet(1.0, 1.0, 1.0));
		MV1SetRotationXYZ(Handle_Jiki_Moving_Arm, VGet(DegtoRad(rot), DegtoRad(Camera.Kakudo - 180.0), 0.0));
		MV1DrawModel(Handle_Jiki_Moving_Arm);

		Position = VGet(
			Jiki.Pos.x + Degsin(Camera.Kakudo - 90) * 4,
			Jiki.Pos.y + 12,
			Jiki.Pos.z + Degcos(Camera.Kakudo - 90) * 4
			);
		MV1SetPosition(Handle_Jiki_Moving_Foot, Position);
		MV1SetScale(Handle_Jiki_Moving_Foot, VGet(1.0, 1.0, 1.0));
		MV1SetRotationXYZ(Handle_Jiki_Moving_Foot, VGet(DegtoRad(-rot), DegtoRad(Camera.Kakudo - 180.0), 0.0));
		MV1DrawModel(Handle_Jiki_Moving_Foot);

		Position = VGet(
			Jiki.Pos.x + Degsin(Camera.Kakudo + 90) * 6,
			Jiki.Pos.y + 20,
			Jiki.Pos.z + Degcos(Camera.Kakudo + 90) * 6
			);
		MV1SetPosition(Handle_Jiki_Moving_Arm, Position);
		MV1SetScale(Handle_Jiki_Moving_Arm, VGet(1.0, 1.0, 1.0));
		MV1SetRotationXYZ(Handle_Jiki_Moving_Arm, VGet(DegtoRad(-rot), DegtoRad(Camera.Kakudo - 180.0), 0.0));
		MV1DrawModel(Handle_Jiki_Moving_Arm);
	}
	else
	{
		rot = 0.0;
		rotd = 4.0;

		MV1SetPosition(Handle_Jiki_Stopping, Jiki.Pos);
		MV1SetScale(Handle_Jiki_Stopping, VGet(1.0, 1.0, 1.0));
		MV1SetRotationXYZ(Handle_Jiki_Stopping, VGet(0.0, RAD1*(Camera.Kakudo - 180.0) + DegtoRad(-90.0), 0.0));
		MV1DrawModel(Handle_Jiki_Stopping);
	}

	passed = true;
}

//�\���S�� �\���͂����o�R�ł���B
void MazeTitle::disp(bool keymode)
{
	disp_3d(keymode);

	if (item.map.isgotten)
		disp_2dmap(false, keymode);
}

//3D�̖��H��\������B
void MazeTitle::disp_3d(bool keymode)
{
	int x, y, z;
	int i, j;

	SetCameraPositionAndTarget_UpVecY(
		Camera.Pos,
		VGet(Jiki.Pos.x, Jiki.Pos.y + 40, Jiki.Pos.z)
		);

	DeleteLightHandle(L1);
	L1 = CreateSpotLightHandle(
		Camera.Pos,
		VGet(Degsin(Camera.Kakudo + 180), 0.0, Degcos(Camera.Kakudo + 180)),
		DX_PI_F,
		DX_PI_F / 2.0,
		1500.0,
		1.0f,
		0.0005f,
		0.0f
		);

	for (x = (((int)(Jiki.Pos.x / Masu_1) - 10) >= 0.0f ? ((int)(Jiki.Pos.x / Masu_1) - 10) : 0);
		x < ((((int)(Jiki.Pos.x / Masu_1) + 10) <= map.x) ? ((int)(Jiki.Pos.x / Masu_1) + 10) : map.x); x++)
	{
		for (z = (((int)(Jiki.Pos.z / Masu_1) - 10) >= 0.0f ? ((int)(Jiki.Pos.z / Masu_1) - 10) : 0);
			z < ((((int)(Jiki.Pos.z / Masu_1) + 10) <= map.z) ? ((int)(Jiki.Pos.z / Masu_1) + 10) : map.z); z++)
		{
			if (map.data[(int)x][(int)z] == 0)
			{
				if ((int)(x) == 0 && (int)(z) == 2)
				{
					MV1SetPosition(Handle_Start, VGet(x*Masu_1, 0.0f, z*Masu_1));
					MV1SetScale(Handle_Start, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_Start, VGet(0.0, 0.0, 0.0));
					MV1DrawModel(Handle_Start);
				}
				else if ((int)(x) == map.x - 1 && (int)(z) == map.z - 3)
				{
					MV1SetPosition(Handle_Goal, VGet(x*Masu_1, 0.0f, z*Masu_1));
					MV1SetScale(Handle_Goal, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_Goal, VGet(0.0, 0.0, 0.0));
					MV1DrawModel(Handle_Goal);
				}
				else
				{
					MV1SetPosition(Handle_Floor, VGet(x*Masu_1, 0.0f, z*Masu_1));
					MV1SetScale(Handle_Floor, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_Floor, VGet(0.0, 0.0, 0.0));
					MV1DrawModel(Handle_Floor);
				}
			}
		}
	}

	for (x = (((int)(Jiki.Pos.x / Masu_1) - 10) >= 0.0f ? ((int)(Jiki.Pos.x / Masu_1) - 10) : 0);
		x < ((((int)(Jiki.Pos.x / Masu_1) + 10) <= map.x) ? ((int)(Jiki.Pos.x / Masu_1) + 10) : map.x); x++)
	{
		for (z = (((int)(Jiki.Pos.z / Masu_1) - 10) >= 0.0f ? ((int)(Jiki.Pos.z / Masu_1) - 10) : 0);
			z < ((((int)(Jiki.Pos.z / Masu_1) + 10) <= map.z) ? ((int)(Jiki.Pos.z / Masu_1) + 10) : map.z); z++)
		{
			if (map.data[(int)x][(int)z] == 0)
			{
				if ((int)(x) == 0 && (int)(z) == 2)
				{
					MV1SetPosition(Handle_Start, VGet(x*Masu_1, Masu_1*map.y, z*Masu_1));
					MV1SetScale(Handle_Start, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_Start, VGet(0.0, 0.0, 0.0));
					MV1DrawModel(Handle_Start);
				}
				else if ((int)(x) == map.x - 1 && (int)(z) == map.z - 3)
				{
					MV1SetPosition(Handle_Goal, VGet(x*Masu_1, Masu_1*map.y, z*Masu_1));
					MV1SetScale(Handle_Goal, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_Goal, VGet(0.0, 0.0, 0.0));
					MV1DrawModel(Handle_Goal);
				}
				else
				{
					MV1SetPosition(Handle_Floor, VGet(x*Masu_1, Masu_1*map.y, z*Masu_1));
					MV1SetScale(Handle_Floor, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_Floor, VGet(0.0, 0.0, 0.0));
					MV1DrawModel(Handle_Floor);

					MV1SetPosition(Handle_LightModel, VGet(x*Masu_1 + Masu_1 / 2, Masu_1*map.y, z*Masu_1 + Masu_1 / 2));
					MV1SetScale(Handle_LightModel, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_LightModel, VGet(0.0, 0.0, 0.0));
					MV1DrawModel(Handle_LightModel);
				}
			}
		}
	}

	//-------------------------------------------------------------------------------------------

	for (x = (((int)(Jiki.Pos.x / Masu_1) - 10) >= 0.0f ? ((int)(Jiki.Pos.x / Masu_1) - 10) : 0);
		x < ((((int)(Jiki.Pos.x / Masu_1) + 10) <= map.x) ? ((int)(Jiki.Pos.x / Masu_1) + 10) : map.x); x++)
	{
		for (y = 0.0f; y<map.y; y++)
		{
			if ((Jiki.Pos.z / Masu_1) - 10 <= map.z && (Jiki.Pos.z / Masu_1) + 10>map.z)
			{
				MV1SetPosition(Handle_Wall, VGet(x*Masu_1, y*Masu_1, map.z*Masu_1));
				MV1SetScale(Handle_Wall, VGet(1.0, 1.0, 1.0));
				MV1SetRotationXYZ(Handle_Wall, VGet(RAD1*-90.0, 0.0, 0.0));
				MV1DrawModel(Handle_Wall);
			}
		}
	}

	for (z = (((int)(Jiki.Pos.z / Masu_1) - 10) >= 0.0f ? ((int)(Jiki.Pos.z / Masu_1) - 10) : 0);
		z < ((((int)(Jiki.Pos.z / Masu_1) + 10) <= map.z) ? ((int)(Jiki.Pos.z / Masu_1) + 10) : map.z); z++)
	{
		for (y = 0.0f; y<map.y; y++)
		{
			if ((Jiki.Pos.x / Masu_1) - 10 <= map.x && (Jiki.Pos.x / Masu_1) + 10>map.x && map.z - 2 != z && map.z - 4 != z)
			{
				if (map.z - 3 == z)
				{
					MV1SetPosition(Handle_Goal, VGet(map.x*Masu_1, y*Masu_1, z*Masu_1));
					MV1SetScale(Handle_Goal, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_Goal, VGet(0.0, 0.0, RAD1*90.0));
					MV1DrawModel(Handle_Goal);
				}
				else
				{
					MV1SetPosition(Handle_Wall, VGet(map.x*Masu_1, y*Masu_1, z*Masu_1));
					MV1SetScale(Handle_Wall, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_Wall, VGet(0.0, 0.0, RAD1*90.0));
					MV1DrawModel(Handle_Wall);
				}
			}
		}
	}

	//-------------------------------------------------------------------------------------------

	for (i = (((Jiki.Pos.x / Masu_1) - 10) >= 0.0f ? ((Jiki.Pos.x / Masu_1) - 10) : 0);
		i < ((((Jiki.Pos.x / Masu_1) + 10) <= map.x) ? ((Jiki.Pos.x / Masu_1) + 10) : map.x); i++)
	{
		for (j = (((Jiki.Pos.z / Masu_1) - 10) >= 0.0f ? ((Jiki.Pos.z / Masu_1) - 10) : 0);
			j < ((((Jiki.Pos.z / Masu_1) + 10) <= map.z) ? ((Jiki.Pos.z / Masu_1) + 10) : map.z); j++)
		{
			if (i>0 ? ((map.data[i][j] == WALL && map.data[i - 1][j] == ROAD) || (map.data[i][j] == ROAD && map.data[i - 1][j] == WALL)) : (j != 3 && j != 1 ? 1 : 0))
			{
				if (((int)(i) == 0 || (int)(i - 1) == 0) && (int)(j) == 2)
				{
					MV1SetPosition(Handle_Start, VGet(i*Masu_1, 0.0, j*Masu_1));
					MV1SetScale(Handle_Start, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_Start, VGet(0.0, 0.0, RAD1*90.0));
					MV1DrawModel(Handle_Start);
				}
				else if (((int)(i) == map.x - 1 || (int)(i - 1) == map.x - 1) && (int)(j) == map.z - 3)
				{
					MV1SetPosition(Handle_Goal, VGet(i*Masu_1, 0.0, j*Masu_1));
					MV1SetScale(Handle_Goal, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_Goal, VGet(0.0, 0.0, RAD1*90.0));
					MV1DrawModel(Handle_Goal);
				}
				else
				{
					MV1SetPosition(Handle_Wall, VGet(i*Masu_1, 0.0, j*Masu_1));
					MV1SetScale(Handle_Wall, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_Wall, VGet(0.0, 0.0, RAD1*90.0));
					MV1DrawModel(Handle_Wall);
				}
			}
			if (j > 0 ? (map.data[i][j] == WALL && map.data[i][j - 1] == ROAD) || (map.data[i][j] == ROAD && map.data[i][j - 1] == WALL) : 1)
			{
				if ((int)(i) == 0 && ((int)(j) == 2 || (int)(j - 1) == 2))
				{
					MV1SetPosition(Handle_Start, VGet(i*Masu_1, 0.0, j*Masu_1));
					MV1SetScale(Handle_Start, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_Start, VGet(RAD1*-90.0, 0.0, 0.0));
					MV1DrawModel(Handle_Start);
				}
				else if ((int)(i) == map.x - 1 && ((int)(j) == map.z - 3 || (int)(j - 1) == map.z - 3))
				{
					MV1SetPosition(Handle_Goal, VGet(i*Masu_1, 0.0, j*Masu_1));
					MV1SetScale(Handle_Goal, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_Goal, VGet(RAD1*-90.0, 0.0, 0.0));
					MV1DrawModel(Handle_Goal);
				}
				else
				{
					MV1SetPosition(Handle_Wall, VGet(i*Masu_1, 0.0, j*Masu_1));
					MV1SetScale(Handle_Wall, VGet(1.0, 1.0, 1.0));
					MV1SetRotationXYZ(Handle_Wall, VGet(RAD1*-90.0, 0.0, 0.0));
					MV1DrawModel(Handle_Wall);
				}
			}
		}
	}

	//�����[�h�̎��ɁA�S�[���O�̌��t���h�A��\������B
	if ((map.x - 1) - 10 <= int(Jiki.Pos.x / Masu_1) && (map.x - 1) + 10 > int(Jiki.Pos.x / Masu_1) && (map.z - 3) - 10 <= int(Jiki.Pos.z / Masu_1) && (map.z - 3) + 10 > int(Jiki.Pos.z / Masu_1))
	{
		if (keymode == true && StartGoalFlag != 2)
		{
			MV1SetPosition(Handle_GoalDoor, VGet((map.x - 1)*Masu_1 + 5, 0.0f, (map.z - 3)*Masu_1 + Masu_1 / 2));
			MV1SetScale(Handle_GoalDoor, VGet(1.0, 1.0, 1.0));
			MV1SetRotationXYZ(Handle_GoalDoor, VGet(0.0, 0.0, 0.0));
			MV1DrawModel(Handle_GoalDoor);
		}
	}

	//�A�C�e���Ǝ��@��\������B
	disp_item(keymode);
	disp_jiki();
}

//�Q�����̃}�b�v��\������B
void MazeTitle::disp_2dmap(bool FullScreenFlag, bool keymode)
{
	int sx, sy;

	//�}�b�v�̑傫����ݒ�
	if (FullScreenFlag) sx = SCREEN_X, sy = SCREEN_Y;
	else				sx = SCREEN_X / 3, sy = SCREEN_Y / 3;

	Map2Dim.SetScreenSize(sx, sy);

	//�t���X�N���[���������łȂ����ŁA�}�b�v�p����ʂ��g��������B
	if (FullScreenFlag)
		SetDrawScreen(Handle_Screen_FullTrue);
	else
		SetDrawScreen(Handle_Screen_FullFalse);

	//�}�b�v�p����ʂ��������A�����B
	ClearDrawScreen();

	//�����w�i��\��
	DrawBackGround(sx, sy);

	//�}�b�v��\��
	if (item.ans.isgotten)
		Map2Dim.disp(Jiki.Pos, Camera.Kakudo + 180.0, true, true, true, keymode, true);
	else
		Map2Dim.disp(Jiki.Pos, Camera.Kakudo + 180.0, true, false, true, keymode, true);

	//�ʏ�̗���ʂɐ؂�ւ���B
	SetDrawScreen(Handle_Screen_Normal);

	if (FullScreenFlag)
	{
		DrawExtendGraph(0, 0, SCREEN_X, SCREEN_Y, Handle_Screen_FullTrue, false);
	}
	else
	{
		DrawExtendGraph(SCREEN_X / 3 * 2, SCREEN_Y / 3 * 2, SCREEN_X, SCREEN_Y, Handle_Screen_FullFalse, false);
	}
}

//�A�C�e����\������B
//����ďオ�����艺�������肵�Ȃ���\�����Ă���B
void MazeTitle::disp_item(bool keymode)
{
	static double rot = 0;
	static double height = 0;
	static double height_d = 30.0 / 45.0;

	//�}�b�v�A�C�e��
	if (item.map.isgotten == false)
	{
		if (item.map.x - 10 <= int(Jiki.Pos.x / Masu_1) && item.map.x + 10 > int(Jiki.Pos.x / Masu_1) && item.map.z - 10 <= int(Jiki.Pos.z / Masu_1) && item.map.z + 10 > int(Jiki.Pos.z / Masu_1))
		{
			MV1SetPosition(Handle_Item_Map, VGet(item.map.x * Masu_1 + Masu_1 / 2, height, item.map.z * Masu_1 + Masu_1 / 2));
			MV1SetScale(Handle_Item_Map, VGet(1.0, 1.0, 1.0));
			MV1SetRotationXYZ(Handle_Item_Map, VGet(0.0, DegtoRad(rot), 0.0));
			MV1DrawModel(Handle_Item_Map);
		}
	}

	//�A���T�[�A�C�e��
	if (item.ans.isgotten == false)
	{
		if (item.ans.x - 10 <= int(Jiki.Pos.x / Masu_1) && item.ans.x + 10 > int(Jiki.Pos.x / Masu_1) && item.ans.z - 10 <= int(Jiki.Pos.z / Masu_1) && item.ans.z + 10 > int(Jiki.Pos.z / Masu_1))
		{
			MV1SetPosition(Handle_Item_Ans, VGet(item.ans.x * Masu_1 + Masu_1 / 2, height, item.ans.z * Masu_1 + Masu_1 / 2));
			MV1SetScale(Handle_Item_Ans, VGet(1.0, 1.0, 1.0));
			MV1SetRotationXYZ(Handle_Item_Ans, VGet(0.0, DegtoRad(rot), 0.0));
			MV1DrawModel(Handle_Item_Ans);
		}
	}

	//��
	if (item.key.isgotten == false)
	{
		if (keymode == true)
		{
			if (item.key.x - 10 <= int(Jiki.Pos.x / Masu_1) && item.key.x + 10 > int(Jiki.Pos.x / Masu_1) && item.key.z - 10 <= int(Jiki.Pos.z / Masu_1) && item.key.z + 10 > int(Jiki.Pos.z / Masu_1))
			{
				MV1SetPosition(Handle_Item_Key, VGet(item.key.x * Masu_1 + Masu_1 / 2, height, item.key.z * Masu_1 + Masu_1 / 2));
				MV1SetScale(Handle_Item_Key, VGet(1.0, 1.0, 1.0));
				MV1SetRotationXYZ(Handle_Item_Key, VGet(0.0, DegtoRad(rot), 0.0));
				MV1DrawModel(Handle_Item_Key);
			}
		}
	}

	//�オ�����艺�������肳����
	height += height_d;
	if (height >= 30) height_d = -30.0 / 45.0;
	if (height <= 0) height_d = 30.0 / 45.0;

	//��点��
	rot += 360.0 / 90.0;
	rot = Amarif(rot, 360.0);
}
