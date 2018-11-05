/*�����ݒ�A��`������w�b�_*/

#ifndef DEFINED_H_DEFINED

#include "DxLib.h"
#include "UsefulHeaders/useful.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#define Masu_1			100							//���H�̃}�X

#define SCREEN_X		1024						//�E�B���h�EX�T�C�Y
#define SCREEN_Y		768							//�E�B���h�EY�T�C�Y

#define STR_COLOR		0xFFFFFF					//�@�Â���ʂł̕�����F
#define STR_COLOR_2		0x000000					//���邢��ʂł̕�����F
#define BACK_COLOR		0x000000

#define Short_Air_X		40
#define Short_Air_Z		50

#define SEED_NUL		65536						//�V�[�h�l���ݒ肳��Ă��Ȃ��Ƃ������l

#define SAVE_DIR		"Saves"						//�Q�[���f�[�^�Z�[�u�t�H���_
#define KAKUCYOSI		".gamesav"					//�Q�[���f�[�^�g���q

#define STATUSMES_COL	8							//�X�e�[�^�X���b�Z�[�W�i��ԉ��j�̕�����F
#define USERMES_COL		7							//���[�U�[���i������Q�Ԃ߁j�̕�����F

#define MES_TITLEBAR	"Maze Traveler Ver.5"		//�E�B���h�E�^�C�g��

#define PROGRAM_FIN		-10							//�v���O�����̏I����\���߂�l

//�Q�[�����j���[�ł̃��j���[���ڂ̐F
//�Ȃ��A���j���[���ڂ͉摜�ł���B
#define S_GRAY			0							//�D�F
#define S_RED			1							//�ԐF
#define S_YELLOW		2							//���F
#define S_GREEN			3							//�ΐF
#define S_BLUE			4							//��
#define S_PURPLE		5							//��

//�`���b�g���\��������F
#define STR_JOIN 0x9999ff
#define STR_DIS  0xff9999
#define STR_RANK 0xffff99
#define CHAT_SX  (SCREEN_X / 3)
#define CHAT_SY  (SCREEN_Y / 3)

//�R���p�C�����̌x���\���}��
#pragma warning( disable:4305 4244 4305 )

//����̖ړI�Ŏg�����߂Ɍ^�̖��O��ύX
typedef int MEDIA_HANDLER;							//�n���h���[�Ŏg��int�^
typedef int** MAPDATA;								//���H�̃}�b�v�f�[�^�i���I�m�ۂQ�����z��j

//�\����

//���@
typedef struct
{
	VECTOR Pos;
	double rot;
	double rotd;
	bool stopf;
}
JIKI;

//���@���f���J����
typedef struct
{
	VECTOR Pos;
	double Kakudo,Hankei;
}
CAMERA;

//���H�}�b�v�i�}�b�v�f�[�^�A���H�̐����̒ʂ蓹�A���H�̒H�����o�H�j
typedef struct
{
	MAPDATA data;		//�}�b�v�f�[�^
	MAPDATA ans;		//���H�̓���
	MAPDATA visited;	//���H�̒H�����o�H

	int x, y, z;		//�}�b�v�̃T�C�YX, Y, Z�i�}�X�P�ʁj
}MAP;

//�A�C�e���P�̍\����
typedef struct
{
	int x, z;			//�A�C�e�����W
	bool isgotten;		//�A�C�e�����肵�����i���肵����`�悵�Ȃ��j
}
ITEM;

//�A�C�e�����R���킹���\����
typedef struct
{
	ITEM map;			//�}�b�v�A�C�e��
	ITEM ans;			//�A���T�[�A�C�e��
	ITEM key;			//�����[�h�œ��肷�錮
}
MAPITEM;

//�X�e�[�W�L�^�f�[�^
typedef struct
{
	unsigned int seed[3];	//�V�[�h�l�i�L�����Ă����΁A���Ƀ}�b�v���J���Ă��������H�ɂȂ�j
	char ischallange[3];	//���H�ɒ��킵�����i�O�F������A�P�F�s���i�A�Q�F���i�j
	int recode[3];			//���H�ɒ��킵�����̓��_�i�P�O�O�_�����s���i�j
}
STAGE;

//�Q�[���t�@�C���\����
typedef struct
{
	STAGE *normalstage;		//�ʏ�X�e�[�W�f�[�^
	STAGE *keystage;		//�����[�h�X�e�[�W�f�[�^
	bool boughtansitem;		//�A���T�[�A�C�e���w��������
	int lev_n;				//�����B���Ă���ō����x��
	int point;				//���H���v���C�������Ƃɂ��݌v�|�C���g�i�Q�[�����̉��z�ʉ݁j
	char username[16];		//���H���[�U�[��
	int online_visited;		//�I�����C���v���C��������
	int online_maxpoint;	//�I�����C���v���C�ł̍ō����_
}
GAMEFILE;

typedef struct
{
	char *content;
	int len;
}
MYLOG;

//-------------------------------------------------------------------------------------------
//���O�t�@�C���Z�[�u�t���O
//�����true�ɂ���΃��O�t�@�C����ۑ����܂��B
//-------------------------------------------------------------------------------------------

bool IsSaveLog = false;

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

//�n���h��
MEDIA_HANDLER L1;												//�ǉ��������C�g�̃n���h��
MEDIA_HANDLER Handle_Jiki;										//���@
MEDIA_HANDLER Handle_Wall;										//��
MEDIA_HANDLER Handle_Floor;										//��
MEDIA_HANDLER Handle_Start;										//�X�^�[�g
MEDIA_HANDLER Handle_Goal;										//�S�[��
MEDIA_HANDLER Handle_Start_ForAir;
MEDIA_HANDLER Handle_Goal_ForAir;
MEDIA_HANDLER Handle_VisitedArea_ForAir;
MEDIA_HANDLER Handle_Jiki_PosNavi_ForAir;
MEDIA_HANDLER Handle_LightModel;
MEDIA_HANDLER Handle_GoalDoor;									//�����[�h�ł̃S�[���ɂ��錮�t���h�A

MEDIA_HANDLER Handle_Title;										//�^�C�g����ʂł̃^�C�g�����摜
MEDIA_HANDLER Handle_PressEnterKey;								//�G���^�[�L�[�𑣂����߂̉摜

//���j���[��ʂł̃��j���[���ډ摜
MEDIA_HANDLER Handle_SelectTile_Blue;							//��
MEDIA_HANDLER Handle_SelectTile_Red;							//��
MEDIA_HANDLER Handle_SelectTile_Green;							//��
MEDIA_HANDLER Handle_SelectTile_Purple;							//��
MEDIA_HANDLER Handle_SelectTile_Yellow;							//���F
MEDIA_HANDLER Handle_SelectTile_Gray;							//�D�F

MEDIA_HANDLER Handle_Dotpic[10];								//�h�b�g������́��h�b�g
MEDIA_HANDLER Handle_2dimmap[80];								//�Q�����}�b�v
MEDIA_HANDLER Handle_BackGround[2];								//�����w�i�摜�P
MEDIA_HANDLER Handle_BackGround_2[2];							//�����w�i�摜�Q
MEDIA_HANDLER Handle_BackGround_3[2];							//�����w�i�摜�R

MEDIA_HANDLER Handle_TimeDisp_Tile;								//���H�v���C���̎��ԕ\����
MEDIA_HANDLER Handle_Delete_Warning;							//�v���C�f�[�^���폜����Ƃ��̊m�F���b�Z�[�W
MEDIA_HANDLER Handle_Delete_Yes;								//�u�v���C�f�[�^���@�폜����v����
MEDIA_HANDLER Handle_Delete_No;									//�u�v���C�f�[�^���폜���Ȃ��v����

MEDIA_HANDLER Handle_SolveRoute;								//���H�̐����o�H�������h�b�g

MEDIA_HANDLER Handle_Goaled;									//�S�[���������ɕ\������摜

MEDIA_HANDLER Handle_Screen_FullFalse;							//�E�B���h�E���ׂĂ��߂Ȃ��}�b�v�\���̂��߂̗��̗��̗��X�N���[��
MEDIA_HANDLER Handle_Screen_FullTrue;							//�E�B���h�E���ׂĂ��@��߂�}�b�v�\���̂��߂̗��̗��̗��X�N���[��
MEDIA_HANDLER Handle_Screen_Normal;								//�t�F�[�h�����邽�߂ɂ��̗��̗��X�N���[���ɕ`�悵�Ă��痠�X�N���[���ɕ`��
MEDIA_HANDLER Handle_Title_DispAssist;							//�^�C�g����ʁA�f����ʂɃh�b�g�������\������Ƃ��Ɍ��₷���悤�ɂ���w�i

MEDIA_HANDLER Handle_Game_Countdown;

MEDIA_HANDLER Handle_Result_Tile;								//���H�̃��U���g�\����ʂł̎��ԕ\��
MEDIA_HANDLER Handle_Result_Passed;								//���i
MEDIA_HANDLER Handle_Result_VeryGood;							//�D��
MEDIA_HANDLER Handle_Result_Failed;								//�s���i

MEDIA_HANDLER Handle_Jiki_Stopping;								//���@���~�܂��Ă��鎞�̑S�g���f��
MEDIA_HANDLER Handle_Jiki_Moving_Body;							//���@�������Ă��鎞�̑�
MEDIA_HANDLER Handle_Jiki_Moving_Foot;							//���@�������Ă��鎞�̑�
MEDIA_HANDLER Handle_Jiki_Moving_Arm;							//���@�������Ă��鎞�̘r

MEDIA_HANDLER Handle_Item_Map;									//�}�b�v�A�C�e���i�j
MEDIA_HANDLER Handle_Item_Ans;									//�A���T�[�A�C�e���i�ԁj
MEDIA_HANDLER Handle_Item_Key;									//�����[�h�ł̌��i���F�j

MEDIA_HANDLER Handle_Gotitem_Tile;								//�A�C�e���l���\����
MEDIA_HANDLER Handle_Gotitem_Map;								//�}�b�v�A�C�e�����l������
MEDIA_HANDLER Handle_Gotitem_Ans;								//�A���T�[�A�C�e�����l������
MEDIA_HANDLER Handle_Gotitem_Key;								//���A�C�e�����l������

MEDIA_HANDLER Handle_Challanging_Keymode;						//�����[�h�ɒ��킷��Ƃ��ɕ\������

MEDIA_HANDLER Handle_Level3_Unlock;								//�A�����b�N�i���x���R����\���j
MEDIA_HANDLER Handle_Level5_Unlock;								//�A�����b�N�i���x���R����\���j
MEDIA_HANDLER Handle_Unlock_KeymodeInStage_UntilLevel2;			//�A�����b�N�i�D�Ǔ_�ɒB�������Ɍ����[�h��ŃX�e�[�W�J���A�����[�h����O�j
MEDIA_HANDLER Handle_Unlock_KeymodeInStage;						//�A�����b�N�i�D�Ǔ_�ɒB�������Ɍ����[�h��ŃX�e�[�W�J���A�����[�h�����j
MEDIA_HANDLER Handle_Unlock_Level;								//�A�����b�N�i���̃��x������j

MEDIA_HANDLER Handle_Help_Navi;									//�w���v�̃{�^����������摜
MEDIA_HANDLER Handle_Help_Back;									//�@�@�@�@�w���v�\���w�i

MEDIA_HANDLER Handle_Bought_AnsItem;							//�A���T�[�A�C�e�����w���������ɕ\������

MEDIA_HANDLER Handle_Navigate_Back;								//�i�r�Q�[�V�����\���w�i

MEDIA_HANDLER Handle_Screen_Chat;								//�`���b�g�p���X�N���[��

MEDIA_HANDLER Handle_NetWork_Connect_Error;

MEDIA_HANDLER Handle_Chat_navigate;								//�`���b�g�𑣂����b�Z�[�W

MEDIA_HANDLER Handle_Online_Mine;

//MEDIA_HANDLER Handle_Map_Back[8];

//�w���v��
char help_text[32768];
SPLITDATA help_text_spl;
int help_text_spl_n;

//�i�r�Q�[�V������
char navi_text[32768];
SPLITDATA navi_text_spl;
int navi_text_spl_n;

//IP�A�h���X
IPDATA Ip = { 127, 0, 0, 1 };
int port = 9850;				//0�`65535�͈̔�

MYLOG MyLog;

//-------------------------------------------------------------------------------------------

int seedset(int);									//�V�[�h�l�ݒ�
void decidemazesize_bylevel(int, int*, int*);		//���x�����w�肵�Ė��H�T�C�Y��Ԃ��֐�
void MakeNewGameFile(char *);						//�V�����Q�[���t�@�C�����쐬
void DispSelect(int, bool, int, int, char *, ...);	//���j���[���ڕ\��
int GetFPS(void);									//FPS�擾

void AddLogFiles(const char *format, ...)
{
	va_list list;
	char buf[1024];
	FILE *fp;

	int len;

	if (IsSaveLog == true)
	{
		va_start(list, format);
		vsprintf_s(buf, 256, format, list);
		va_end(list);

		/*

		fopen_s(&fp, "MyLog.txt", "a");
		fprintf(fp, "%s", buf);
		fclose(fp);
		*/

		fopen_s(&fp, "MyLog.txt", "a");
		fprintf(fp, "%s", buf);
		fclose(fp);

		/*
		len = strlen(buf);

		MyLog.len += len;
		ChangeArraySize_1dim(char, MyLog.content, MyLog.len + 1);
		sprintf_s(MyLog.content, MyLog.len + 1, "%s%s", MyLog.content, buf);
		*/
	}
}

//FPS�擾
int GetFPS(void)
{
	static int cntstarttime = GetNowCount();
	static int fps = 0;
	static int cnt = 0;

	cnt++;
	if (GetNowCount() - cntstarttime >= 1000)
	{
		fps = cnt;
		cnt = 0;
		cntstarttime = GetNowCount();
	}

	return fps;
}

//�V�[�h�l�ݒ�
int seedset(int seed)
{
	if (seed < 0)
	{
		return rand();
	}
	else
		return seed;
}

//���x�����w�肵�Ė��H�T�C�Y��Ԃ��֐�
void decidemazesize_bylevel(int level, int* sizex, int* sizez)
{
	if (level < 1)
	{
		*sizex = 0;
		*sizez = 0;
		return;
	}

	level--;

	*sizex = ((level + 1) / 2 + 1) * 20 + 1;
	*sizez = (level / 2 + 1) * 20 + 1;
}

//�V�K�Ƀt�@�C�����쐬
void MakeNewGameFile(char *filename)
{
	FILE *fp;
	char buf[256];
	int i, j;
	GAMEFILE file;

	sprintf_s(buf, 256, "./%s/%s%s", SAVE_DIR, filename, KAKUCYOSI);

	//�����Ŏw�肵���t�@�C�������݂��Ă��Ȃ�������
	//�V�K�Ƀt�@�C�����쐬
	if (exist("%s", buf) == false)
	{
		//strcpy_s(file.username, 16, filename);

		//�t�@�C��������

		file.lev_n = 0;					//���x���ő�l
		file.point = 0;					//�݌v�|�C���g
		file.online_visited = 0;		//�I�����C���v���C��������
		file.online_maxpoint = 0;		//�I�����C���v���C�ō����_
		file.boughtansitem = false;		//�A���T�[�A�C�e���w��������

		//�X�e�[�W�f�[�^���I�z��m��
		file.normalstage = (STAGE *)malloc(sizeof(STAGE)*(file.lev_n + 1));
		file.keystage = (STAGE *)malloc(sizeof(STAGE)*(file.lev_n + 1));

		//�X�e�[�W�f�[�^
		for (j = 0; j < 3; j++)
		{
			file.normalstage[file.lev_n].ischallange[j] = 0;
			file.normalstage[file.lev_n].recode[j] = 0;
			file.normalstage[file.lev_n].seed[j] = SEED_NUL;

			file.keystage[file.lev_n].ischallange[j] = 0;
			file.keystage[file.lev_n].recode[j] = 0;
			file.keystage[file.lev_n].seed[j] = SEED_NUL;
		}

		file.lev_n++;

		//�t�@�C���ɏ�������
		fopen_s(&fp, buf, "wb");

		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_SET);

			//fwrite(file.username, sizeof(char), 16, fp);

			fwrite(&file.lev_n, sizeof(int), 1, fp);
			fwrite(&file.point, sizeof(int), 1, fp);
			fwrite(&file.online_visited, sizeof(int), 1, fp);
			fwrite(&file.online_maxpoint, sizeof(int), 1, fp);
			fwrite(&file.boughtansitem, sizeof(bool), 1, fp);

			fwrite(file.normalstage, sizeof(STAGE), file.lev_n, fp);
			fwrite(file.keystage, sizeof(STAGE), file.lev_n, fp);

			fclose(fp);
		}

		//�X�e�[�W�f�[�^���I�z��J��
		free(file.normalstage);
		free(file.keystage);
	}
}

//���j���[���ڂ�\��
void dispselect(int y, bool isselect, int kind, int bottompos, char *format, ...)
{
	va_list list;
	char buf[1024];
	MEDIA_HANDLER Handle_Tile;

	va_start(list, format);
	vsprintf_s(buf, 1024, format, list);
	va_end(list);

	switch (kind)
	{
	case S_GRAY:
		Handle_Tile = Handle_SelectTile_Gray;
		break;
	case S_RED:
		Handle_Tile = Handle_SelectTile_Red;
		break;
	case S_GREEN:
		Handle_Tile = Handle_SelectTile_Green;
		break;
	case S_BLUE:
		Handle_Tile = Handle_SelectTile_Blue;
		break;
	case S_PURPLE:
		Handle_Tile = Handle_SelectTile_Purple;
		break;
	case S_YELLOW:
		Handle_Tile = Handle_SelectTile_Yellow;
		break;
	default:
		return;
		break;
	}

	SetFontSize(20);
	if (y >= 100 && y < bottompos)
	{
		if (isselect)
		{
			DrawGraph(10, y, Handle_Tile, false);
			DrawFormatString(10, y, STR_COLOR_2, "%s", buf);
		}
		else
		{
			DrawGraph(10 + 50, y, Handle_Tile, false);
			DrawFormatString(10 + 50, y, STR_COLOR_2, "%s", buf);
		}
	}
}

#define DEFINED_H_DEFINED

#endif
