/*ゲーム本体ヘッダ*/

#ifndef PLAY_DEFINED
#define PLAY_DEFINED

#include "Defines.h"
#include "DrawMoji.h"
#include "MyMaze.h"
#include "Map2Dim.h"
#include "BackGround.h"
#include "disphelp.h"
#include "NetWorkMatch.h"

class MazeGame
{
private:
	char name[256];					//ゲームファイル名

	MAP map;						//迷路マップ
	MAPITEM item;					//迷路アイテム
	GAMEFILE file;					//ゲームファイル

	JIKI Jiki;						//自機
	CAMERA Camera;					//カメラ

	bool dispsolveflag = false;		//答えを表示するフラグ
	bool MapSolveFlag = false;
	bool Online_ChatFlag = false;	//チャット窓表示フラグ
	bool ChatInputFlag = false;

	int StartGoalFlag = 1;			//迷路の状況（０：プレイ中、１：スタート、２：ゴール）

	MEDIA_HANDLER NetHandle;		//オンライン対戦のサーバーハンドル

	FILE *isopen;					//ファイルを二重に開かないためのファイル変数

	MEMBER_RET mret;
	ROOM_RET   rret;

	int DispMode = 0;

	int InputHandle = -1;

	bool ChatInitFlag = true;

public:
	//外から呼び出せる関数
	int start(void);		//ゲームを開始する
	int init(char *);		//ゲーム開始の準備をする。
	void dest(void);		//ゲームを終了する。

private:
	//メニュー
	int top_menu(void);			//トップメニュー
	int online_menu(void);		//オンラインメニュー（オンラインプレイ）
	int offline_menu(void);		//オフラインメニュー（ひとりでプレイ）

	//ユーザー名変更
	int ChangeUserName(void);

	//ユーザー情報表示
	int ShowUserInfo(void);

	//インターネット接続中
	int online_init(IPDATA, int);		//オンライン準備
	int online_match_main(void);		//オンライン対戦中Main
	int online_dest(void);				//オンライン終了

	//ファイル読み込み、保存。
	void load(void);
	void save(void);

	//レベルアップ
	void levelup(void);

	//迷路のプレイ（オフライン）
	int PlayMaze(unsigned int, char*, bool, int, int);
	int PlayMaze_SelectByLevelandStage(int, int, bool);
	int PlayMaze_SelectByLevelandSeed(int, int, bool);

	//迷路のプレイ（オンライン）
	int PlayMaze_online(unsigned int, char*, bool, int);

	//表示系関数
	void disp(int, bool);
	void disp_3d(bool);
	void disp_2dmap(bool, bool);
	void disp_jiki(void);
	void disp_item(bool);

	//オンライン
	void disp_online(int, bool);
	void disp_chat(void);
	void disp_3d_online(bool);
	void disp_jiki_others(void);
	int inputchat(void);

	//自機を動かし、カメラと連動させる。
	void JikiandCamMove(int, bool);

	//自機とカメラの位置を初期化。
	void PosSet_def(void);
}Game;

//ゲームを開始する。
int MazeGame::start(void)
{
	if (strcmp(name, "") == 0)
		return -2;

	return top_menu();
}

//ゲームを始める準備をする。（ゲームファイルを開く）
int MazeGame::init(char *filename)
{
	if (strcmp(name, "") != 0)
		return -2;

	if (exist("./%s/Using_%s.tmp", SAVE_DIR, filename) == true)
		return -1;

	strcpy_s(name, 256, filename);
	file.normalstage = (STAGE *)malloc(0);
	file.keystage = (STAGE *)malloc(0);
	PosSet_def();
	load();

	return 0;
}

//ゲームを終わる。（ゲームファイルを閉じる）
void MazeGame::dest(void)
{
	if (strcmp(name, "") == 0)
		return;

	save();
	strcpy_s(name, 256, "");
	free(file.normalstage);
	free(file.keystage);
	ClearDrawScreen();
}

//ユーザー名変更
int MazeGame::ChangeUserName(void)
{
	bool brkflag = false;
	char name_new[16];
	int cursor;
	static bool confflag = true;
	int nowmenu = 0;

	int framecnt = 0;

	char buf[256];
	char buf2[256];

	strcpy_s(name_new, 16, file.username);

	cursor = strlen(file.username);

	while (1)
	{
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
			brkflag = true;

		if (brkflag) break;

		ClearDrawScreen();

		if (Stick(KEY_INPUT_UP, true))
		{
			if (nowmenu > 0)
			{
				nowmenu--;
				confflag = true;
			}
		}
		if (Stick(KEY_INPUT_DOWN, true))
		{
			if (nowmenu < 1)
			{
				nowmenu++;
				confflag = true;
			}
		}

		if (Stick(KEY_INPUT_F1))
		{
			static int ret;
			ret = disp_help();

			if (ret == -1)
			{
				brkflag = true;
				continue;
			}
		}

		if (nowmenu == 0)
		{
			if (confflag == true)
			{
				ClearInputCharBuf();
				confflag = false;
			}

			char c = m_inputalpha();

			if (c != -1)
			{
				if (c == CTRL_CODE_BS)
				{
					if (cursor > 0)
					{
						cursor--;
						name_new[cursor] = '\0';
					}
				}
				else
				{
					if (cursor < 15)
					{
						name_new[cursor] = c;
						name_new[cursor + 1] = '\0';
						cursor++;
					}
				}
			}

			if (Stick(KEY_INPUT_RETURN))
			{
				disp_navi_set("UserMenu");
				confflag = true;
				break;
			}
		}
		else if (nowmenu == 1)
		{
			if (Stick(KEY_INPUT_BACK))
			{
				if (strlen(name_new) >= 1)
				{
					disp_navi_set("UserMenu");
					break;
				}
			}
			if (Stick(KEY_INPUT_RETURN))
			{
				if (strlen(name_new) >= 1)
				{
					disp_navi_set("UserMenu");
					break;
				}
			}
		}

		DrawBackGround(SCREEN_X, SCREEN_Y);

		SetFontSize(15);
		DrawFormatString(10, SCREEN_Y - 20, STR_COLOR_2, "%d FPS", GetFPS());

		SetFontSize(20);
		DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  CHANGE NAME");

		DrawGraph(SCREEN_X - 210, 100, Handle_Help_Navi, FALSE);

		if (nowmenu == 0)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "NEW NAME: %s", name_new);
			if (strcmp(file.username, name_new) == 0)
			{
				DrawGraph(10, 100 + 30 * 0, Handle_SelectTile_Green, false);
				DrawFormatString(10, 100 + 0 * 30, STR_COLOR_2, "新しいユーザー名：%s", name_new);
			}
			else
			{
				DrawGraph(10, 100 + 30 * 0, Handle_SelectTile_Blue, false);
				DrawFormatString(10, 100 + 0 * 30, STR_COLOR_2, "新しいユーザー名：%s", name_new);
			}

			if (framecnt % 60 < 30)
				DrawLine(
				10 + 190 + cursor * 11,
				100 + 30 * 0 + 3,
				10 + 190 + cursor * 11,
				100 + 30 * 1 - 3,
				STR_COLOR_2);
		}
		else
		{
			if (strcmp(file.username, name_new) == 0)
			{
				DrawGraph(10 + 50, 100 + 30 * 0, Handle_SelectTile_Green, false);
				DrawFormatString(10 + 50, 100 + 0 * 30, STR_COLOR_2, "新しいユーザー名：%s", name_new);
			}
			else
			{
				DrawGraph(10 + 50, 100 + 30 * 0, Handle_SelectTile_Blue, false);
				DrawFormatString(10 + 50, 100 + 0 * 30, STR_COLOR_2, "新しいユーザー名：%s", name_new);
			}
		}

		if (nowmenu == 1)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "BACK TO THE TOP MENU.");

			DrawGraph(10, 100 + 30 * 1 + 20, Handle_SelectTile_Purple, false);
			DrawFormatString(10, 100 + 1 * 30 + 20, STR_COLOR_2, "名前を変更しないで戻る");
		}
		else
		{
			DrawGraph(10 + 50, 100 + 30 * 1 + 20, Handle_SelectTile_Purple, false);
			DrawFormatString(10 + 50, 100 + 1 * 30 + 20, STR_COLOR_2, "名前を変更しないで戻る");
		}

		disp_navi();

		framecnt++;

		ScreenFlip();
	}

	if (brkflag)
	{
		return -1;
	}
	else
	{
		if (nowmenu == 0)
		{
			strcpy_s(file.username, 16, name_new);

			sprintf_s(buf, 256, "./%s/%s", SAVE_DIR, name);						//前のファイル
			sprintf_s(buf2, 256, "./%s/%s%s", SAVE_DIR, name_new, KAKUCYOSI);	//新しいファイル

			//名前を変えたら処理を実行、変えなかったら処理しない。
			if (strcmp(buf, buf2) != 0)
			{
				//前のファイルを保存する
				dest();

				//ファイルをコピーして前のファイルを削除。
				filecopy(buf, buf2);
				filedel("%s", buf);

				//新しい名前のファイルを読みこみ、
				sprintf_s(buf, 256, "%s%s", name_new, KAKUCYOSI);
				init(buf);
			}
		}

		return 0;
	}
}

//ユーザー情報表示
int MazeGame::ShowUserInfo(void)
{
	bool brkflag = false;
	int nowmenu = 0;
	//int ret;

	while (1)
	{
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
			brkflag = true;

		if (brkflag)			break;

		ClearDrawScreen();		//描画する画面（裏）を黒クリア

		if (Stick(KEY_INPUT_F1))
		{
			static int ret;
			ret = disp_help();
			if (ret == -1)
			{
				brkflag = true;
				continue;
			}
		}

		if (Stick(KEY_INPUT_DOWN, true))
		{
			if (nowmenu < 4) nowmenu++;
		}

		if (Stick(KEY_INPUT_UP, true))
		{
			if (nowmenu > 0) nowmenu--;
		}

		if (Stick(KEY_INPUT_BACK))
		{
			disp_navi_set("UserMenu");
			break;
		}

		if (nowmenu == 4)
		{
			if (Stick(KEY_INPUT_RETURN))
			{
				disp_navi_set("UserMenu");
				break;
			}
		}
		else
		{
			if (Stick(KEY_INPUT_RETURN))
			{
				nowmenu++;
			}
		}

		DrawBackGround(SCREEN_X, SCREEN_Y);

		SetFontSize(15);
		DrawFormatString(10, SCREEN_Y - 20, STR_COLOR_2, "%d FPS", GetFPS());

		SetFontSize(20);
		DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  INFORMATION");

		DispStringOfBox(10, SCREEN_Y - 140, 0.8, USERMES_COL, "%s %dP", file.username, file.point);

		DrawGraph(SCREEN_X - 210, 100, Handle_Help_Navi, FALSE);

		if (nowmenu == 0)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "USERNAME : %s", file.username);
		}
		else if (nowmenu == 1)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "NOW POINT : %dP", file.point);
		}
		else if (nowmenu == 2)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "NOW LEVEL : %d", file.lev_n);
		}
		else if (nowmenu == 3)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "ONLINE : %d VISIT, MAX %dP", file.online_visited, file.online_maxpoint);
		}
		else if (nowmenu == 4)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "BACK TO THE TOP MENU");
		}

		//ユーザー名
		dispselect(100 + 30 * 0, nowmenu == 0, S_BLUE, 580, "ユーザー名：%s", file.username);

		//ポイントと称号
		if (file.point <= 2000)
		{
			dispselect(100 + 30 * 1 + 20, nowmenu == 1, S_BLUE, 580, "現在のポイント : %dP 初心者", file.point);
		}
		else if (file.point <= 5000)
		{
			dispselect(100 + 30 * 1 + 20, nowmenu == 1, S_GREEN, 580, "現在のポイント : %dP 中級者", file.point);
		}
		else if (file.point <= 10000)
		{
			dispselect(100 + 30 * 1 + 20, nowmenu == 1, S_YELLOW, 580, "現在のポイント : %dP 上級者", file.point);
		}
		else if (file.point <= 20000)
		{
			dispselect(100 + 30 * 1 + 20, nowmenu == 1, S_RED, 580, "現在のポイント : %dP 超人", file.point);
		}
		else
		{
			dispselect(100 + 30 * 1 + 20, nowmenu == 1, S_PURPLE, 580, "現在のポイント : %dP マスター", file.point);
		}

		dispselect(100 + 30 * 2 + 20, nowmenu == 2, S_GREEN, 580, "現在挑戦中のレベル：Level %d", file.lev_n);

		dispselect(100 + 30 * 3 + 20, nowmenu == 3, S_GREEN, 580, "オンライン対戦：参加%d回、最高得点 %dP", file.online_visited, file.online_maxpoint);

		dispselect(100 + 30 * 4 + 40, nowmenu == 4, S_PURPLE, 580, "戻る");

		disp_navi();

		ScreenFlip();
	}

	if (brkflag)
		return -1;
	else
		return 0;
}

//トップメニュー
int MazeGame::top_menu(void)
{
	bool brkflag = false;
	int nowmenu = 0;
	int ret;

	while (1)
	{
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
			brkflag = true;

		if (brkflag)			break;

		ClearDrawScreen();		//描画する画面（裏）を黒クリア

		if (Stick(KEY_INPUT_F1))
		{
			static int ret;
			ret = disp_help();
			if (ret == -1)
			{
				brkflag = true;
				continue;
			}
		}

		if (Stick(KEY_INPUT_DOWN, true))
		{
			if (nowmenu < 4) nowmenu++;
		}

		if (Stick(KEY_INPUT_UP, true))
		{
			if (nowmenu > 0) nowmenu--;
		}
		if (Stick(KEY_INPUT_BACK))
		{
			disp_navi_set("OpenUserName");
			break;
		}

		if (nowmenu == 0)
		{
			if (Stick(KEY_INPUT_RETURN))
			{
				disp_navi_set("AlonePlay");
				ret = offline_menu();
				if (ret == -1)
				{
					brkflag = true;
					continue;
				}
			}
		}
		else if (nowmenu == 1)
		{
			if (Stick(KEY_INPUT_RETURN))
			{
				disp_navi_set("OnLinePlay");
				ret = online_menu();
				if (ret == -1)
				{
					brkflag = true;
					continue;
				}
			}
		}
		else if (nowmenu == 2)
		{
			if (Stick(KEY_INPUT_RETURN))
			{
				disp_navi_set("ChangeUserName");
				ret = ChangeUserName();
				if (ret == -1)
				{
					brkflag = true;
					continue;
				}
			}
		}
		else if (nowmenu == 3)
		{
			if (Stick(KEY_INPUT_RETURN))
			{
				disp_navi_set("UserInfo");
				ret = ShowUserInfo();
				if (ret == -1)
				{
					brkflag = true;
					continue;
				}
			}
		}
		else if (nowmenu == 4)
		{
			if (Stick(KEY_INPUT_RETURN))
			{
				disp_navi_set("OpenUserName");
				break;
			}
		}

		DrawBackGround(SCREEN_X, SCREEN_Y);

		SetFontSize(15);
		DrawFormatString(10, SCREEN_Y - 20, STR_COLOR_2, "%d FPS", GetFPS());

		SetFontSize(20);
		DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  TOP MENU");

		DispStringOfBox(10, SCREEN_Y - 140, 0.8, USERMES_COL, "%s %dP", file.username, file.point);

		DrawGraph(SCREEN_X - 210, 100, Handle_Help_Navi, FALSE);

		if (nowmenu == 0)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "ALONE PLAY");

			DrawGraph(10, 100 + 30 * 0, Handle_SelectTile_Green, false);
			DrawFormatString(10, 100 + 0 * 30, STR_COLOR_2, "ひとりでプレイ");
		}
		else
		{
			DrawGraph(10 + 50, 100 + 30 * 0, Handle_SelectTile_Green, false);
			DrawFormatString(10 + 50, 100 + 0 * 30, STR_COLOR_2, "ひとりでプレイ");
		}

		if (nowmenu == 1)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "ONLINE PLAY");

			DrawGraph(10, 100 + 30 * 1, Handle_SelectTile_Blue, false);
			DrawFormatString(10, 100 + 1 * 30, STR_COLOR_2, "オンラインプレイ");
		}
		else
		{
			DrawGraph(10 + 50, 100 + 30 * 1, Handle_SelectTile_Blue, false);
			DrawFormatString(10 + 50, 100 + 1 * 30, STR_COLOR_2, "オンラインプレイ");
		}

		if (nowmenu == 2)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "CHANGE THE USERNAME");

			DrawGraph(10, 100 + 30 * 2 + 20, Handle_SelectTile_Yellow, false);
			DrawFormatString(10, 100 + 2 * 30 + 20, STR_COLOR_2, "ユーザー名を変える");
		}
		else
		{
			DrawGraph(10 + 50, 100 + 30 * 2 + 20, Handle_SelectTile_Yellow, false);
			DrawFormatString(10 + 50, 100 + 2 * 30 + 20, STR_COLOR_2, "ユーザー名を変える");
		}

		if (nowmenu == 3)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "SHOW THE USER INFORMATION");

			DrawGraph(10, 100 + 30 * 3 + 20, Handle_SelectTile_Yellow, false);
			DrawFormatString(10, 100 + 3 * 30 + 20, STR_COLOR_2, "情報を見る");
		}
		else
		{
			DrawGraph(10 + 50, 100 + 30 * 3 + 20, Handle_SelectTile_Yellow, false);
			DrawFormatString(10 + 50, 100 + 3 * 30 + 20, STR_COLOR_2, "情報を見る");
		}

		if (nowmenu == 4)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "BACK TO FILE SELECT");

			DrawGraph(10, 100 + 30 * 4 + 40, Handle_SelectTile_Purple, false);
			DrawFormatString(10, 100 + 4 * 30 + 40, STR_COLOR_2, "ファイルを閉じる");
		}
		else
		{
			DrawGraph(10 + 50, 100 + 30 * 4 + 40, Handle_SelectTile_Purple, false);
			DrawFormatString(10 + 50, 100 + 4 * 30 + 40, STR_COLOR_2, "ファイルを閉じる");
		}

		disp_navi();

		ScreenFlip();
	}

	if (brkflag)
		return -1;
	else
		return 0;
}

//オンラインメニュー（接続先指定メニュー）
int MazeGame::online_menu(void)
{
	char inputdata[16];
	int cursor;
	bool confflag = true;
	//bool setflag = false;

	bool brkflag = false;
	int nowmenu = 0;
	int ip_input_pos = 0;

	bool cannot_connect_flag = false;
	int cannot_connect_frame = -1;

	int ret;

	int c;

	int i;

	int framecnt = 0;

	while (1)
	{
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
			brkflag = true;

		if (brkflag)			break;

		//操作部
		//接続できませんのメッセージが出ていないときに操作可能にする
		if (cannot_connect_flag == false)
		{
			//ヘルプ
			if (Stick(KEY_INPUT_F1))
			{
				static int ret;
				ret = disp_help();
				if (ret == -1)
				{
					brkflag = true;
					continue;
				}
			}

			if (nowmenu >= 2)
			{
				if (Stick(KEY_INPUT_DOWN, true))
				{
					if (nowmenu < 3) nowmenu++;
				}

				if (Stick(KEY_INPUT_UP, true))
				{
					if (nowmenu > 0) nowmenu--;
				}
				if (Stick(KEY_INPUT_BACK))
				{
					disp_navi_set("UserMenu");
					break;
				}
			}

			//IP設定部分
			if (nowmenu == 0)
			{
				if (confflag == true)
				{
					ClearInputCharBuf();
					switch (ip_input_pos)
					{
					case 0: itoa(Ip.d1, inputdata, 10); break;
					case 1: itoa(Ip.d2, inputdata, 10); break;
					case 2: itoa(Ip.d3, inputdata, 10); break;
					case 3: itoa(Ip.d4, inputdata, 10); break;
					}
					cursor = 0;

					confflag = false;
				}

				c = m_inputnum();

				if (c >= 0 && c <= 9)
				{
					inputdata[cursor] = '0' + c;
					inputdata[cursor + 1] = '\0';

					if (atoi(inputdata) < 256)
					{
						cursor++;
					}
					else
					{
						if (atoi(inputdata) < 1000)
						{
							cursor++;
						}
						inputdata[0] = '2';
						inputdata[1] = '5';
						inputdata[2] = '5';
						inputdata[3] = '\0';
					}
				}
				else if (c == 10)
				{
					if (cursor > 0) cursor--;
					if (cursor >= 1)
						inputdata[cursor] = '\0';
					else
					{
						inputdata[cursor] = '0';
						inputdata[cursor + 1] = '\0';
					}
				}

				if (Stick(KEY_INPUT_DOWN, true))
				{
					if (nowmenu < 3)
					{
						confflag = true;
						switch (ip_input_pos)
						{
						case 0: Ip.d1 = atoi(inputdata); break;
						case 1: Ip.d2 = atoi(inputdata); break;
						case 2: Ip.d3 = atoi(inputdata); break;
						case 3: Ip.d4 = atoi(inputdata); break;
						}
						nowmenu++;
						ip_input_pos = 0;

						continue;
					}
				}

				if (Stick(KEY_INPUT_UP, true))
				{
					if (nowmenu > 0)
					{
						confflag = true;
						switch (ip_input_pos)
						{
						case 0: Ip.d1 = atoi(inputdata); break;
						case 1: Ip.d2 = atoi(inputdata); break;
						case 2: Ip.d3 = atoi(inputdata); break;
						case 3: Ip.d4 = atoi(inputdata); break;
						}
						nowmenu--;
						ip_input_pos = 0;

						continue;
					}
				}

				if (Stick(KEY_INPUT_LEFT, true))
				{
					if (ip_input_pos > 0)
					{
						confflag = true;
						switch (ip_input_pos)
						{
						case 0: Ip.d1 = atoi(inputdata); break;
						case 1: Ip.d2 = atoi(inputdata); break;
						case 2: Ip.d3 = atoi(inputdata); break;
						case 3: Ip.d4 = atoi(inputdata); break;
						}
						ip_input_pos--;

						continue;
					}
				}

				if (Stick(KEY_INPUT_RIGHT, true))
				{
					if (ip_input_pos < 3)
					{
						confflag = true;
						switch (ip_input_pos)
						{
						case 0: Ip.d1 = atoi(inputdata); break;
						case 1: Ip.d2 = atoi(inputdata); break;
						case 2: Ip.d3 = atoi(inputdata); break;
						case 3: Ip.d4 = atoi(inputdata); break;
						}
						ip_input_pos++;

						continue;
					}
				}
				if (Stick(KEY_INPUT_TAB))
				{
					if (ip_input_pos < 3)
					{
						confflag = true;
						switch (ip_input_pos)
						{
						case 0: Ip.d1 = atoi(inputdata); break;
						case 1: Ip.d2 = atoi(inputdata); break;
						case 2: Ip.d3 = atoi(inputdata); break;
						case 3: Ip.d4 = atoi(inputdata); break;
						}

						ip_input_pos++;

						continue;
					}
				}

				if (Stick(KEY_INPUT_RETURN))
				{
					if (ip_input_pos < 3)
					{
						confflag = true;
						switch (ip_input_pos)
						{
						case 0: Ip.d1 = atoi(inputdata); break;
						case 1: Ip.d2 = atoi(inputdata); break;
						case 2: Ip.d3 = atoi(inputdata); break;
						case 3: Ip.d4 = atoi(inputdata); break;
						}

						ip_input_pos++;

						continue;
					}
					else
					{
						if (nowmenu < 3)
						{
							confflag = true;

							switch (ip_input_pos)
							{
							case 0: Ip.d1 = atoi(inputdata); break;
							case 1: Ip.d2 = atoi(inputdata); break;
							case 2: Ip.d3 = atoi(inputdata); break;
							case 3: Ip.d4 = atoi(inputdata); break;
							}

							ip_input_pos = 0;
							nowmenu++;

							continue;
						}
					}
				}
			}
			//ポート設定部分
			else if (nowmenu == 1)
			{
				if (confflag == true)
				{
					ClearInputCharBuf();
					itoa(port, inputdata, 10);
					cursor = 0;

					confflag = false;
				}

				c = m_inputnum();

				if (c >= 0 && c <= 9)
				{
					inputdata[cursor] = '0' + c;
					inputdata[cursor + 1] = '\0';

					if (atoi(inputdata) < 65536)
					{
						cursor++;
					}
					else
					{
						if (atoi(inputdata) < 100000)
						{
							cursor++;
						}

						inputdata[0] = '6';
						inputdata[1] = '5';
						inputdata[2] = '5';
						inputdata[3] = '3';
						inputdata[4] = '5';
						inputdata[5] = '\0';
					}
				}
				else if (c == 10)
				{
					if (cursor > 0) cursor--;
					if (cursor >= 1)
						inputdata[cursor] = '\0';
					else
					{
						inputdata[cursor] = '0';
						inputdata[cursor + 1] = '\0';
					}
				}

				if (Stick(KEY_INPUT_DOWN, true))
				{
					if (nowmenu < 3)
					{
						/*
						if (atoi(inputdata) < 49152)
						{
						inputdata[0] = '4';
						inputdata[1] = '9';
						inputdata[2] = '1';
						inputdata[3] = '5';
						inputdata[4] = '2';
						inputdata[5] = '\0';
						}
						*/
						confflag = true;
						port = atoi(inputdata);
						nowmenu++;

						continue;
					}
				}

				if (Stick(KEY_INPUT_UP, true))
				{
					if (nowmenu > 0)
					{
						/*
						if (atoi(inputdata) < 49152)
						{
						inputdata[0] = '4';
						inputdata[1] = '9';
						inputdata[2] = '1';
						inputdata[3] = '5';
						inputdata[4] = '2';
						inputdata[5] = '\0';
						}
						*/
						confflag = true;
						port = atoi(inputdata);
						nowmenu--;

						continue;
					}
				}

				if (Stick(KEY_INPUT_RETURN))
				{
					if (nowmenu < 3)
					{
						/*
						if (atoi(inputdata) < 49152)
						{
						inputdata[0] = '4';
						inputdata[1] = '9';
						inputdata[2] = '1';
						inputdata[3] = '5';
						inputdata[4] = '2';
						inputdata[5] = '\0';
						}
						*/
						confflag = true;
						port = atoi(inputdata);
						nowmenu++;

						continue;
					}
				}
			}
			//接続
			else if (nowmenu == 2)
			{
				if (Stick(KEY_INPUT_RETURN))
				{
					ret = online_init(Ip, port);
					if (ret != -1)
					{
						online_match_main();
						online_dest();
					}
					else
					{
						cannot_connect_flag = true;
						cannot_connect_frame = 0;
					}
				}
			}
			//戻る
			else if (nowmenu == 3)
			{
				if (Stick(KEY_INPUT_RETURN))
				{
					disp_navi_set("UserMenu");
					break;
				}
			}
		}

		//処理
		//接続できませんの表示を2秒間行う
		//（フラグの管理）
		if (cannot_connect_flag)
		{
			if (cannot_connect_frame >= 60 * 2)
				cannot_connect_flag = false;
			cannot_connect_frame++;
		}

		//描画する画面（裏）を黒クリア
		ClearDrawScreen();
		
		DrawBackGround(SCREEN_X, SCREEN_Y);

		SetFontSize(15);
		DrawFormatString(10, SCREEN_Y - 20, STR_COLOR_2, "%d FPS", GetFPS());

		SetFontSize(20);

		DispStringOfBox(10, 10, 0.7, -1, "MAZE TRAVELER  MULTIPLAY CONNECTION");

		DrawGraph(SCREEN_X - 210, 100, Handle_Help_Navi, FALSE);

		if (nowmenu == 0)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "IP ADDRESS : %3d.%3d.%3d.%3d", Ip.d1, Ip.d2, Ip.d3, Ip.d4);

			DrawGraph(10, 100 + 30 * 0, Handle_SelectTile_Green, false);
			DrawFormatString(10, 100 + 0 * 30, STR_COLOR_2, "IPアドレス：");

			for (i = 0; i < 4; i++)
			{
				if (ip_input_pos == i)
				{
					DrawBox(10 + 150 + i * 70, 100 + 0 * 30, 10 + 120 + (i + 1) * 70, 100 + 1 * 30, GetColor(200, 200, 100), TRUE);
					if (i<3)
						DrawFormatString(10 + 150 + i * 70, 100 + 0 * 30, STR_COLOR_2, "%s.", inputdata);
					else
						DrawFormatString(10 + 150 + i * 70, 100 + 0 * 30, STR_COLOR_2, "%s", inputdata);

					if (framecnt % 60 < 30)
						DrawLine(
						10 + 150 + i * 70 + 2 + cursor * 11,
						100 + 30 * 0 + 3,
						10 + 150 + i * 70 + 2 + cursor * 11,
						100 + 30 * 1 - 3,
						STR_COLOR_2);
				}
				else
				{
					switch (i)
					{
					case 0:
						DrawFormatString(10 + 150 + i * 70, 100 + 0 * 30, STR_COLOR_2, "%d.", Ip.d1);
						break;
					case 1:
						DrawFormatString(10 + 150 + i * 70, 100 + 0 * 30, STR_COLOR_2, "%d.", Ip.d2);
						break;
					case 2:
						DrawFormatString(10 + 150 + i * 70, 100 + 0 * 30, STR_COLOR_2, "%d.", Ip.d3);
						break;
					case 3:
						DrawFormatString(10 + 150 + i * 70, 100 + 0 * 30, STR_COLOR_2, "%d", Ip.d4);
						break;
					}
				}
			}
		}
		else
		{
			DrawGraph(10 + 50, 100 + 30 * 0, Handle_SelectTile_Green, false);
			DrawFormatString(10 + 50, 100 + 0 * 30, STR_COLOR_2, "IPアドレス：");

			for (i = 0; i < 4; i++)
			{
				switch (i)
				{
				case 0:
					DrawFormatString(10 + 50 + 150 + i * 70, 100 + 0 * 30, STR_COLOR_2, "%d.", Ip.d1);
					break;
				case 1:
					DrawFormatString(10 + 50 + 150 + i * 70, 100 + 0 * 30, STR_COLOR_2, "%d.", Ip.d2);
					break;
				case 2:
					DrawFormatString(10 + 50 + 150 + i * 70, 100 + 0 * 30, STR_COLOR_2, "%d.", Ip.d3);
					break;
				case 3:
					DrawFormatString(10 + 50 + 150 + i * 70, 100 + 0 * 30, STR_COLOR_2, "%d", Ip.d4);
					break;
				}
			}
		}

		if (nowmenu == 1)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "PORT NO : %s", inputdata);

			DrawGraph(10, 100 + 30 * 1, Handle_SelectTile_Green, false);
			DrawFormatString(10, 100 + 1 * 30, STR_COLOR_2, "ポート番号(49152〜65535)：%s", inputdata);

			if (framecnt % 60 < 30)
				DrawLine(
				10 + 277 + cursor * 11,
				100 + 30 * 1 + 3,
				10 + 277 + cursor * 11,
				100 + 30 * 2 - 3,
				STR_COLOR_2);
		}
		else
		{
			DrawGraph(10 + 50, 100 + 30 * 1, Handle_SelectTile_Green, false);
			DrawFormatString(10 + 50, 100 + 1 * 30, STR_COLOR_2, "ポート番号(49152〜65535)：%d", port);
		}

		if (nowmenu == 2)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "CONNECT");

			DrawGraph(10, 100 + 30 * 2 + 20, Handle_SelectTile_Blue, false);
			DrawFormatString(10, 100 + 2 * 30 + 20, STR_COLOR_2, "この設定で接続する");
		}
		else
		{

			DrawGraph(10 + 50, 100 + 30 * 2 + 20, Handle_SelectTile_Blue, false);
			DrawFormatString(10 + 50, 100 + 2 * 30 + 20, STR_COLOR_2, "この設定で接続する");
		}

		if (nowmenu == 3)
		{
			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "BACK TO TOP MENU");

			DrawGraph(10, 100 + 30 * 3 + 40, Handle_SelectTile_Purple, false);
			DrawFormatString(10, 100 + 3 * 30 + 40, STR_COLOR_2, "戻る");
		}
		else
		{
			DrawGraph(10 + 50, 100 + 30 * 3 + 40, Handle_SelectTile_Purple, false);
			DrawFormatString(10 + 50, 100 + 3 * 30 + 40, STR_COLOR_2, "戻る");
		}

		//接続できませんの表示をする
		//-----
		if (cannot_connect_flag)
		{
			DrawGraph(SCREEN_X / 2 - 300, SCREEN_Y / 2 - 75, Handle_NetWork_Connect_Error, false);
		}

		disp_navi();

		framecnt++;

		ScreenFlip();
	}

	if (brkflag)
		return -1;
	else
		return 0;
}

//オンライン対戦の前処理（接続の開始）
int MazeGame::online_init(IPDATA Ip, int port)
{
	NetHandle = ConnectNetWork(Ip, port);

	if (NetHandle != -1)
	{
		NetMatch.init(NetHandle);
		NetMatch.send_name(file.username);

		//迷路の部屋情報が取得されるまで待つ
		while (NetMatch.GetRoomInfo(&rret) == false)
			NetMatch.update();
	}

	return NetHandle;
}

//オンライン対戦メインプログラム（メニュー選択）
int MazeGame::online_match_main(void)
{
	bool brkflag	= false;
	bool chudanflag = false;
	int menu_no = 0;
	int i;

	int pos_no;
	int space_no;
	int top_position = 100;

	int ret;

	char stagename[32];
	char roomname[32];

	bool inputflag = true;

	//新しい部屋を作る（シード値、レベル、鍵モード設定）フラグ
	bool roomsetflag = false;

	static bool confflag = true;

	MAZEINFO sendinfo = { 1, 0, false };
	char seed_no[16];
	int cursor = 0;

	//ゲーム開始までのフェードに関する変数
	int gamecnt = 0;
	bool gamestartf = false;

	while (1)
	{
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
			brkflag = true;

		if (brkflag)
			break;

		if (chudanflag)
			break;

		//接続が切られたら接続画面に戻る
		if (NetMatch.checklost() == -1)
		{
			break;
		}

		//ネットワーク通信用クラスを更新
		NetMatch.update();

		//ネットワーク通信用クラスから部屋情報を取得
		NetMatch.GetRoomInfo(&rret);

		//描画対象を裏の裏画面にする
		SetDrawScreen(Handle_Screen_Normal);

		//裏の裏画面を空にする
		ClearDrawScreen();

		//背景の描写
		DrawBackGround(SCREEN_X, SCREEN_Y, 1);


		//通常部屋選択画面
		if (roomsetflag == false)
		{
			//入力部--------------------------------------
			//メニュー項目選択
			if (Stick(KEY_INPUT_UP))
			{
				if (menu_no > 0)				menu_no--;
			}
			if (Stick(KEY_INPUT_DOWN))
			{
				if (menu_no < rret.room_n + 1)	menu_no++;
			}
			//決定
			if (Stick(KEY_INPUT_RETURN))
			{
				//ゲーム開始
				if (menu_no < rret.room_n)
				{
					if (rret.room[menu_no].isuse == true)
					{
						gamestartf = true;
						gamecnt = 0;
					}
				}
				//新しいルームを作る
				else if (menu_no == rret.room_n)
				{
					roomsetflag = true;
					menu_no = 0;
					strcpy_s(roomname, 32, "");
					strcpy_s(seed_no, 16, "");
					sendinfo.iskeymode = false;
					sendinfo.level = 1;
					sendinfo.seed = 0;
					inputflag = true;
					continue;
				}
				//戻る
				else if (menu_no == rret.room_n + 1)
				{
					disp_navi_set("UserMenu");
					break;
				}
			}

			//戻る
			if (Stick(KEY_INPUT_BACK))
			{
				disp_navi_set("UserMenu");
				break;
			}

			//表示部--------------------------------------

			//ゲーム開始フラグに関する処理
			if (gamestartf == true)
			{
				if (gamecnt >= 90)
				{
					NetMatch.room_join(menu_no);

					sprintf_s(stagename, 32,
						"%02d-%05d",
						rret.room[menu_no].minfo.level,
						rret.room[menu_no].minfo.seed);

					//メンバー情報を取得するまで繰り返す
					while (NetMatch.update() > 0);

					AddLogFiles("%d : %s\n", menu_no, stagename);

					ChatInitFlag = true;

					ret = PlayMaze_online(
						rret.room[menu_no].minfo.seed,
						stagename,
						rret.room[menu_no].minfo.iskeymode,
						rret.room[menu_no].minfo.level);

					if (ret >= 0)
					{
						file.point += ret;
						file.online_visited++;
						if (file.online_maxpoint < ret)
							file.online_maxpoint = ret;
					}
					else if (ret == -1)
					{
						brkflag = true;
					}

					gamestartf = false;

					continue;
				}
				else
				{
					gamecnt++;
				}
			}

			//タイトルとステータス
			if (menu_no < rret.room_n)
			{
				DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  ONLINE ROOM SELECT");
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "STAGE %d SELECTING", menu_no + 1);
			}
			else if (menu_no == rret.room_n)
			{
				DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  MAKE NEW ROOM");
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "MAKE NEW ROOM");
			}
			else if (menu_no == rret.room_n + 1)
			{
				DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  OTHERS");
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "BACK TO THE USERMENU");
			}

			//メニュー項目（ステージ）
			pos_no = space_no = 0;
			for (i = 0; i < rret.room_n; i++)
			{
				dispselect(
					top_position + pos_no * 30 + space_no * 20,
					menu_no == i,
					rret.room[i].isuse ? (rret.room[i].minfo.iskeymode ? S_BLUE : S_RED) : S_GRAY,
					640,
					"ステージ%02d : %s Lv.%d, %s",
					i + 1,
					rret.room[i].name,
					rret.room[i].minfo.level,
					//rret.room[i].minfo.seed,
					rret.room[i].isuse ? (rret.room[i].minfo.iskeymode ? "鍵モード有効" : "鍵モード無効") : "破棄されました");

				pos_no++;
			}
			space_no++;

			//メニュー項目（新規作成）
			dispselect(
				top_position + pos_no * 30 + space_no * 20,
				menu_no == rret.room_n,
				S_BLUE,
				640,
				"新しくルームを作る");

			pos_no++;
			space_no++;

			//メニュー項目（戻る）
			dispselect(
				top_position + pos_no * 30 + space_no * 20,
				menu_no == rret.room_n + 1,
				S_PURPLE,
				640,
				"戻る");

		}//end if(通常部屋選択画面終了
		//新しい部屋を作る（シード値、レベル、鍵モード設定）
		else
		{
			//タイトルとステータス
			DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  MAKE NEW ROOM");
			switch (menu_no)
			{
			case 0:
			{
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "ROOMNAME INPUT");
				break;
			}
			case 1:
			{
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "LEVEL SELECT");
				break;
			}
			case 2:
			{
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "SEED INPUT");
				break;
			}
			case 3:
			{
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "KEYMODE SELECT");
				break;
			}
			case 4:
			{
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "MAKE NEW ROOM");
				break;
			}
			case 5:
			{
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "BACK TO THE ROOM SELECT");
				break;
			}
			}

			//メニュー項目描画
			{
				pos_no = space_no = 0;
				dispselect(
					top_position + pos_no * 30 + space_no * 20,
					menu_no == 0,
					S_GREEN,
					640,
					"ルーム名 : %s", roomname);
				pos_no++;
				space_no++;
				dispselect(
					top_position + pos_no * 30 + space_no * 20,
					menu_no == 1,
					S_GREEN,
					640,
					"< レベル %d >", sendinfo.level);
				pos_no++;
				if (menu_no == 2)
				{
					dispselect(
						top_position + pos_no * 30 + space_no * 20,
						menu_no == 2,
						S_GREEN,
						640,
						"シード値 : %s", seed_no);
					pos_no++;
				}
				else
				{
					dispselect(
						top_position + pos_no * 30 + space_no * 20,
						menu_no == 2,
						S_GREEN,
						640,
						"シード値 : %d", sendinfo.seed);
					pos_no++;
				}
				dispselect(
					top_position + pos_no * 30 + space_no * 20,
					menu_no == 3,
					sendinfo.iskeymode ? S_BLUE : S_RED,
					640,
					"鍵モード : %s",
					sendinfo.iskeymode ? "有効" : "無効");
				pos_no++;
				space_no++;
				dispselect(
					top_position + pos_no * 30 + space_no * 20,
					menu_no == 4,
					S_GREEN,
					640,
					"ルーム作成！");
				pos_no++;
				space_no++;
				dispselect(
					top_position + pos_no * 30 + space_no * 20,
					menu_no == 5,
					S_PURPLE,
					640,
					"戻る");
			}


			//操作部（表示後方式）
			//入力部--------------------------------------
			if (menu_no == 0)
			{
				//メニュー項目選択
				if (Stick(KEY_INPUT_UP))
				{
					if (menu_no > 0)				menu_no--;
				}
				if (Stick(KEY_INPUT_DOWN))
				{
					if (menu_no < 5)				menu_no++;
				}
				//決定で次の項目へ
				if (Stick(KEY_INPUT_RETURN))
				{
					menu_no++;
				}
				//戻る
				if (Stick(KEY_INPUT_BACK))
				{
					roomsetflag = false;
					menu_no = rret.room_n;
					continue;
				}

				//ルーム名入力部分
				if (inputflag)
				{
					//入力用項目へと画面を上書き
					pos_no = space_no = 0;
					dispselect(
						top_position + pos_no * 30 + space_no * 20,
						menu_no == 0,
						S_GREEN,
						640,
						"ルーム名 > ");

					//裏の裏から裏画面に描画
					SetDrawScreen(DX_SCREEN_BACK);
					DrawGraph(0, 0, Handle_Screen_Normal, false);

					//表画面へ
					ScreenFlip();

					SetKeyInputStringColor(STR_COLOR_2, STR_COLOR_2, STR_COLOR_2, STR_COLOR_2, STR_COLOR_2, STR_COLOR_2, STR_COLOR);

					//入力
					ret = KeyInputString(
						(menu_no == 0 ? 50 : 10) + 10 * 9,
						top_position + pos_no * 30 + space_no * 20,
						31,
						roomname,
						true
						);

					if (ret == -1 || ret == 2)
					{
						//エスケープキーが間違って反応して終了しないようにする
						Stick(KEY_INPUT_ESCAPE);
						//roomsetflag = false;
						//continue;
					}
					else
					{
						Stick(KEY_INPUT_RETURN);
					}

					inputflag = false;

					menu_no++;

					continue;
				}
			}
			else if (menu_no == 1)
			{
				//メニュー項目選択
				if (Stick(KEY_INPUT_UP))
				{
					if (menu_no > 0)				menu_no--;
					inputflag = true;
					strcpy_s(stagename, 32, "");
				}
				if (Stick(KEY_INPUT_DOWN))
				{
					if (menu_no < 5)				menu_no++;
				}
				//レベル選択
				if (Stick(KEY_INPUT_LEFT))
				{
					if (sendinfo.level > 1)			sendinfo.level--;
				}
				if (Stick(KEY_INPUT_RIGHT))
				{
					if (sendinfo.level < 100)		sendinfo.level++;
				}
				//決定（次へ）
				if (Stick(KEY_INPUT_RETURN))
				{
					menu_no++;
				}
				//戻る
				if (Stick(KEY_INPUT_BACK))
				{
					roomsetflag = false;
					menu_no = rret.room_n;
					continue;
				}

				/*
				if (menu_no < 4)
				{
					menu_no++;
				}
				else if (menu_no == 4)
				{
				}
				else if (menu_no == 5)
				{
					disp_navi_set("UserMenu");
					break;
				}
				*/
			}
			//シード値選択
			else if (menu_no == 2)
			{
				//初期設定
				if (confflag)
				{
					ClearInputCharBuf();
					itoa(sendinfo.seed, seed_no, 10);
					if (sendinfo.seed != 0)
						cursor = strlen(seed_no);
					else
						cursor = 0;
					confflag = false;
				}

				//入力値取得
				int c = m_inputnum();

				//入力（数字：0-9、BackSpace：10として値が返る）
				//数字
				if (c >= 0 && c <= 9)
				{
					seed_no[cursor] = '0' + c;
					seed_no[cursor + 1] = '\0';

					//65536以上の場合自動的に65535にする
					if (atoi(seed_no) < 65536)
					{
						cursor++;
					}
					else
					{
						if (atoi(seed_no) < 100000)
						{
							cursor++;
						}
						seed_no[0] = '6';
						seed_no[1] = '5';
						seed_no[2] = '5';
						seed_no[3] = '3';
						seed_no[4] = '5';
						seed_no[5] = '\0';
					}
				}
				//BackSpace
				else if (c == 10)
				{
					if (cursor > 0) cursor--;
					if (cursor >= 1)
						seed_no[cursor] = '\0';
					else
						seed_no[cursor] = '0';
				}

				//メニュー項目選択
				if (Stick(KEY_INPUT_UP, true))
				{
					if (menu_no > 1)
					{
						menu_no--;
						sendinfo.seed = atoi(seed_no);
						confflag = true;
					}
				}
				if (Stick(KEY_INPUT_DOWN, true))
				{
					if (menu_no < 5)
					{
						menu_no++;
						sendinfo.seed = atoi(seed_no);
						confflag = true;
					}
				}
				//決定
				if (Stick(KEY_INPUT_RETURN))
				{
					menu_no++;
					sendinfo.seed = atoi(seed_no);
					confflag = true;
				}
				/*
				if (Stick(KEY_INPUT_BACK))
				{
					roomsetflag = false;
					continue;
				}
				*/
			}
			//鍵モード選択
			else if (menu_no == 3)
			{
				//項目選択
				if (Stick(KEY_INPUT_UP))
				{
					if (menu_no > 0)				menu_no--;
				}
				if (Stick(KEY_INPUT_DOWN))
				{
					if (menu_no < 5)				menu_no++;
				}
				//鍵モード選択
				if (Stick(KEY_INPUT_LEFT))
				{
					sendinfo.iskeymode = false;
				}
				if (Stick(KEY_INPUT_RIGHT))
				{
					sendinfo.iskeymode = true;
				}
				//決定（次へ）
				if (Stick(KEY_INPUT_RETURN))
				{
					menu_no++;
				}
				//戻る
				if (Stick(KEY_INPUT_BACK))
				{
					roomsetflag = false;
					menu_no = rret.room_n;
					continue;
				}
			}
			//部屋作成
			else if (menu_no == 4)
			{
				//項目選択
				if (Stick(KEY_INPUT_UP))
				{
					if (menu_no > 0)				menu_no--;
				}
				if (Stick(KEY_INPUT_DOWN))
				{
					if (menu_no < 5)				menu_no++;
				}
				//決定（部屋作成）
				if (Stick(KEY_INPUT_RETURN))
				{
					NetMatch.Make_Room();
					NetMatch.SetMazeInfo(-1, roomname, sendinfo);	//直前に作った部屋の設定をする
					roomsetflag = false;
					menu_no = rret.room_n;
				}
				//戻る
				if (Stick(KEY_INPUT_BACK))
				{
					roomsetflag = false;
					menu_no = rret.room_n;
					continue;
				}
			}
			//戻る
			else if (menu_no == 5)
			{
				//メニュー項目選択
				if (Stick(KEY_INPUT_UP))
				{
					if (menu_no > 0)				menu_no--;
				}
				if (Stick(KEY_INPUT_DOWN))
				{
					if (menu_no < 5)				menu_no++;
				}
				//戻る
				if (Stick(KEY_INPUT_RETURN))
				{
					roomsetflag = false;
					menu_no = rret.room_n;
					continue;
				}
				//戻る
				if (Stick(KEY_INPUT_BACK))
				{
					roomsetflag = false;
					menu_no = rret.room_n;
					continue;
				}
			}
		}

		//裏の裏画面から裏画面に切り替え
		SetDrawScreen(DX_SCREEN_BACK);

		//ゲーム開始前にフェードアウト
		if (gamestartf == true)
		{
			SetDrawBright(
				255 * (1.0 - double(gamecnt) / 90.0),
				255 * (1.0 - double(gamecnt) / 90.0),
				255 * (1.0 - double(gamecnt) / 90.0)
				);
		}
		//切り替え以外は通常表示
		else
		{
			SetDrawBright(255, 255, 255);
		}

		//裏画面に描画
		DrawGraph(0, 0, Handle_Screen_Normal, false);

		//表画面に表示
		ScreenFlip();
	}

	//デフォルトである裏画面に描画させる
	SetDrawScreen(DX_SCREEN_BACK);

	if (chudanflag)
		return -2;
	else if (brkflag)
		return -1;
	else
		return 0;
}

//オンライン対戦の後処理（接続の終了）
int MazeGame::online_dest(void)
{
	NetMatch.update();
	CloseNetWork(NetHandle);

	if (NetHandle != -1)
	{
		NetMatch.dest();
	}
	return 0;
}

//オフライン用メニュー
int MazeGame::offline_menu(void)
{
	int i, j;

	int ret;

	int nowlevel = 1, nowstage = 1;
	int nowmenu = 0;
	bool keymode = false;
	bool keymode_custom = false;

	char seed_no[8] = "";
	int seed_num = 0;
	int selectingmode = 0;
	int confflag = true;

	bool brkflag = false;

	int framecnt = 0;

	int cursor = 0;

	int pos_no;
	int space_no;
	int top_position = 100;
	int pos_no_cur;
	int space_no_cur;

	int kindof_game = -1;
	int called_game_frame = -1;

	int mapx, mapz;

	//init();

	//メインループ
	while (1)
	{
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
			brkflag = true;

		if (brkflag)			break;

		SetDrawScreen(Handle_Screen_Normal);

		ClearDrawScreen();		//描画する画面（裏）を黒クリア

		//■■ここから

		if (Stick(KEY_INPUT_F1))
		{
			static int ret;
			ret = disp_help();
			if (ret == -1)
			{
				brkflag = true;
				continue;
			}
		}

		if (framecnt - called_game_frame >= 60)
		{
			if (kindof_game == 0)
			{
				ret = PlayMaze_SelectByLevelandStage(nowlevel, nowstage, keymode);
				ClearInputCharBuf();

				if (ret == -1)
					brkflag = true;

				if (brkflag)
					break;

				i = 0;
				if (keymode == false)
				{
					for (i = 0; i < 3; i++)
						if (file.normalstage[file.lev_n - 1].ischallange[i] != 2)
							break;
				}

				if (i == 3)
				{
					levelup();
					selectingmode = 0;
					nowstage = 1;
				}
				else
				{
					selectingmode = 1;
				}

				kindof_game = -1;
				called_game_frame = -1;

				continue;
			}
			else if (kindof_game == 1)
			{
				seed_num = atoi(seed_no);
				ret = PlayMaze_SelectByLevelandSeed(nowlevel, seed_num, keymode_custom);
				ClearInputCharBuf();

				if (ret == -1)
					brkflag = true;

				if (brkflag)
					break;

				kindof_game = -1;
				called_game_frame = -1;

				continue;
			}
		}

		//レベル選択モード（その他もあり）
		if (kindof_game == -1)
		{
			if (selectingmode == 0)
			{
				if (Stick(KEY_INPUT_UP, true))
				{
					if (nowlevel >= 2) nowlevel--;
				}
				if (Stick(KEY_INPUT_DOWN, true))
				{
					if (nowlevel < file.lev_n + 5) nowlevel++;
				}
				if (nowlevel == file.lev_n + 2)
				{
					if (file.lev_n >= 3)
					{
						if (Stick(KEY_INPUT_LEFT))
						{
							keymode = false;
						}
						if (Stick(KEY_INPUT_RIGHT))
						{
							keymode = true;
							disp_navi_set("KeyMode");
						}
					}
				}
				if (Stick(KEY_INPUT_RETURN))
				{
					if (nowlevel <= file.lev_n)
						selectingmode++;
					else if (nowlevel == file.lev_n + 1)
					{
						if (file.point >= 2000)
						{
							if (file.boughtansitem == false)
							{
								disp_navi_set("AnswerItem");
								file.point -= 2000;
								file.boughtansitem = true;
							}
						}
					}
					else if (nowlevel == file.lev_n + 2)
					{
						if (file.lev_n >= 3)
						{
							keymode = !keymode;
							if (keymode == true)
								disp_navi_set("KeyMode");
						}
					}
					else if (nowlevel == file.lev_n + 3)
					{
						if (file.lev_n >= 5)
						{
							disp_navi_set("RandomPlay");
							selectingmode = 2;
							nowlevel = RangeRand(1, 10);
							nowmenu = 5;
							itoa(RangeRand(0, 32767), seed_no, 10);
							keymode_custom = RangeRand(0, 10) >= 8 ? true : false;
						}
					}
					else if (nowlevel == file.lev_n + 4)
					{
						if (file.lev_n >= 5)
						{
							disp_navi_set("CustomPlay");
							selectingmode = 3;
							nowlevel = 1;
							nowmenu = 1;
							strcpy_s(seed_no, 8, "0");
							keymode_custom = false;
						}
					}
					else if (nowlevel == file.lev_n + 5)
					{
						break;
					}
				}
				if (Stick(KEY_INPUT_BACK))
				{
					disp_navi_set("UserMenu");
					break;
				}
			}
			//ステージ選択モード
			else if (selectingmode == 1)
			{
				if (Stick(KEY_INPUT_UP, true))
				{
					if (nowstage >= 2) nowstage--;
				}
				if (Stick(KEY_INPUT_DOWN, true))
				{
					if (nowstage <= 2) nowstage++;
				}
				if (Stick(KEY_INPUT_RETURN))
				{
					if (keymode == false)
					{
						kindof_game = 0;
						called_game_frame = framecnt;
					}
					else
					{
						if (file.normalstage[nowlevel - 1].recode[nowstage - 1] / nowlevel >= 300)
						{
							kindof_game = 0;
							called_game_frame = framecnt;
						}
					}
				}
				if (Stick(KEY_INPUT_BACK))
				{
					disp_navi_set("AlonePlay");
					selectingmode--;
					nowstage = 1;
				}
			}
			//レベル＆ステージ迷路（通常）プレイ
			//else if (selectingmode == 2)
			//{

				/*
				ret = PlayMaze_SelectByLevelandStage(nowlevel, nowstage, keymode);
				ClearInputCharBuf();

				if (ret == -1)
				brkflag = true;

				if (brkflag)
				break;

				i = 0;
				if (keymode == false)
				{
				for (i = 0; i < 3; i++)
				if (file.normalstage[file.lev_n - 1].ischallange[i] != 2)
				break;
				}

				if (i == 3)
				{
				levelup();
				selectingmode = 0;
				nowstage = 1;
				}
				else
				{
				selectingmode = 1;
				}
				*/
			//}
			//おまかせプレイ
			else if (selectingmode == 2)
			{
				static bool passed = false;

				if (passed == 0)
				{
					cursor = strlen(seed_no);
					passed = true;
				}

				if (nowmenu != 2)
				{
					if (Stick(KEY_INPUT_UP, true))
					{
						if (nowmenu > 1) nowmenu--;
					}
					if (Stick(KEY_INPUT_DOWN, true))
					{
						if (nowmenu < 6) nowmenu++;
					}
					if (nowmenu < 5)
					{
						if (Stick(KEY_INPUT_RETURN))
						{
							nowmenu++;
						}
					}
				}
				//BackSpaceで戻る（入力時以外）
				if (nowmenu != 2)
				{
					if (Stick(KEY_INPUT_BACK))
					{
						//cursor = 0;
						selectingmode = 0;
						nowlevel = file.lev_n + 3;
						passed = false;
						disp_navi_set("AlonePlay");
					}
				}
				//レベル選択
				if (nowmenu == 1)
				{
					if (Stick(KEY_INPUT_LEFT, true))
					{
						if (nowlevel >= 2) nowlevel--;
					}
					if (Stick(KEY_INPUT_RIGHT, true))
					{
						if (nowlevel <= 99) nowlevel++;
					}
				}
				//SEED値入力
				else if (nowmenu == 2)
				{
					if (confflag)
					{
						ClearInputCharBuf();
						confflag = false;
					}

					int c = m_inputnum();

					if (c >= 0 && c <= 9)
					{
						seed_no[cursor] = '0' + c;
						seed_no[cursor + 1] = '\0';

						if (atoi(seed_no) < 65536)
						{
							cursor++;
						}
						else
						{
							if (atoi(seed_no) < 100000)
							{
								cursor++;
							}
							seed_no[0] = '6';
							seed_no[1] = '5';
							seed_no[2] = '5';
							seed_no[3] = '3';
							seed_no[4] = '5';
							seed_no[5] = '\0';
						}
					}
					else if (c == 10)
					{
						if (cursor > 0) cursor--;
						if (cursor >= 1)
							seed_no[cursor] = '\0';
						else
							seed_no[cursor] = '0';
					}

					if (Stick(KEY_INPUT_UP, true))
					{
						if (nowmenu > 1)
						{
							nowmenu--;
							confflag = true;
						}
					}
					if (Stick(KEY_INPUT_DOWN, true))
					{
						if (nowmenu < 6)
						{
							nowmenu++;
							confflag = true;
						}
					}
					if (nowmenu < 4)
					{
						if (Stick(KEY_INPUT_RETURN))
						{
							nowmenu++;
							confflag = true;
						}
					}
				}
				//鍵モード設定
				else if (nowmenu == 3)
				{
					if (Stick(KEY_INPUT_LEFT))
					{
						keymode_custom = false;
					}
					if (Stick(KEY_INPUT_RIGHT))
					{
						keymode_custom = true;
					}
				}
				//アンサーアイテム購入
				else if (nowmenu == 4)
				{
					if (Stick(KEY_INPUT_RETURN))
					{
						if (file.point >= 2000)
						{
							if (file.boughtansitem == false)
							{
								disp_navi_set("AnswerItem");
								file.point -= 2000;
								file.boughtansitem = true;
							}
						}
					}
				}
				//実行
				else if (nowmenu == 5)
				{
					if (Stick(KEY_INPUT_RETURN))
					{
						kindof_game = 1;
						called_game_frame = framecnt;

						/*
						seed_num = atoi(seed_no);
						ret = PlayMaze_SelectByLevelandSeed(nowlevel, seed_num, keymode_custom);
						ClearInputCharBuf();

						if (ret == -1)
						brkflag = true;

						if (brkflag)
						break;
						*/
					}
				}
				//戻る
				else if (nowmenu == 6)
				{
					if (Stick(KEY_INPUT_RETURN))
					{
						//cursor = 0;
						selectingmode = 0;
						nowlevel = file.lev_n + 3;
						passed = false;
						disp_navi_set("AlonePlay");
					}
				}
			}
			//カスタムプレイ
			else if (selectingmode == 3)
			{
				static bool passed = false;

				if (passed == false)
				{
					cursor = 0;
					passed = true;
				}

				if (nowmenu != 2)
				{
					if (Stick(KEY_INPUT_UP, true))
					{
						if (nowmenu > 1) nowmenu--;
					}
					if (Stick(KEY_INPUT_DOWN, true))
					{
						if (nowmenu < 6) nowmenu++;
					}
					if (nowmenu < 5)
					{
						if (Stick(KEY_INPUT_RETURN))
						{
							nowmenu++;
						}
					}
				}
				//BackSpaceで戻る（入力時以外）
				if (nowmenu != 2)
				{
					if (Stick(KEY_INPUT_BACK))
					{
						cursor = 0;
						selectingmode = 0;
						nowlevel = file.lev_n + 4;
						passed = false;
						disp_navi_set("AlonePlay");
					}
				}
				//レベル選択
				if (nowmenu == 1)
				{
					if (Stick(KEY_INPUT_LEFT, true))
					{
						if (nowlevel >= 2) nowlevel--;
					}
					if (Stick(KEY_INPUT_RIGHT, true))
					{
						if (nowlevel <= 99) nowlevel++;
					}
				}
				//SEED値入力
				else if (nowmenu == 2)
				{
					if (confflag)
					{
						ClearInputCharBuf();
						confflag = false;
					}

					int c = m_inputnum();

					if (c >= 0 && c <= 9)
					{
						seed_no[cursor] = '0' + c;
						seed_no[cursor + 1] = '\0';

						if (atoi(seed_no) < 65536)
						{
							cursor++;
						}
						else
						{
							seed_no[0] = '6';
							seed_no[1] = '5';
							seed_no[2] = '5';
							seed_no[3] = '3';
							seed_no[4] = '5';
							seed_no[5] = '\0';
						}
					}
					else if (c == 10)
					{
						if (cursor > 0) cursor--;
						if (cursor >= 1)
							seed_no[cursor] = '\0';
						else
							seed_no[cursor] = '0';
					}

					if (Stick(KEY_INPUT_UP, true))
					{
						if (nowmenu > 1)
						{
							nowmenu--;
							confflag = true;
						}
					}
					if (Stick(KEY_INPUT_DOWN, true))
					{
						if (nowmenu < 6)
						{
							nowmenu++;
							confflag = true;
						}
					}
					if (nowmenu < 4)
					{
						if (Stick(KEY_INPUT_RETURN))
						{
							nowmenu++;
							confflag = true;
						}
					}
				}
				//鍵モード設定
				else if (nowmenu == 3)
				{
					if (Stick(KEY_INPUT_LEFT))
					{
						keymode_custom = false;
					}
					if (Stick(KEY_INPUT_RIGHT))
					{
						keymode_custom = true;
					}
				}
				//アンサーアイテム購入
				else if (nowmenu == 4)
				{
					if (Stick(KEY_INPUT_RETURN))
					{
						if (file.point >= 2000)
						{
							if (file.boughtansitem == false)
							{
								disp_navi_set("AnswerItem");
								file.point -= 2000;
								file.boughtansitem = true;
							}
						}
					}
				}
				//実行
				else if (nowmenu == 5)
				{
					if (Stick(KEY_INPUT_RETURN))
					{
						kindof_game = 1;
						called_game_frame = framecnt;

						/*
						seed_num = atoi(seed_no);
						ret = PlayMaze_SelectByLevelandSeed(nowlevel, seed_num, keymode_custom);
						ClearInputCharBuf();

						if (ret == -1)
							brkflag = true;

						if (brkflag)
							break;
						*/
					}
				}
				//戻る
				else if (nowmenu == 6)
				{
					if (Stick(KEY_INPUT_RETURN))
					{
						cursor = 0;
						selectingmode = 0;
						nowlevel = file.lev_n + 4;
						passed = false;
						disp_navi_set("AlonePlay");
					}
				}
			}
		}

		DrawBackGround(SCREEN_X, SCREEN_Y);

		SetFontSize(15);
		DrawFormatString(10, SCREEN_Y - 20, STR_COLOR_2, "%d FPS", GetFPS());

		SetFontSize(20);

		DispStringOfBox(10, SCREEN_Y - 140, 0.8, USERMES_COL, "%s %dP", file.username, file.point);

		DrawGraph(SCREEN_X - 210, 100, Handle_Help_Navi, FALSE);

		if (selectingmode < 3)
		{
			if (keymode)
				DrawGraph(SCREEN_X - 60, 10, Handle_Challanging_Keymode, false);
		}
		else
		{
			if (keymode_custom)
				DrawGraph(SCREEN_X - 60, 10, Handle_Challanging_Keymode, false);
		}

		if (file.boughtansitem)
		{
			DrawGraph(SCREEN_X - 120, 10, Handle_Bought_AnsItem, false);
		}

		pos_no = space_no = 0;
		pos_no_cur = space_no_cur = 0;

		//メニュー項目の左上y座標を決める
		if (selectingmode == 0)
		{
			for (i = 1; i <= nowlevel; i++)
			{
				if (i == file.lev_n + 1) space_no_cur++;
				if (i == file.lev_n + 3) space_no_cur++;
				if (i == file.lev_n + 5) space_no_cur++;

				if (i < nowlevel) pos_no_cur++;
			}
			//左上y座標は通常100とする。
			if (pos_no_cur * 30 + space_no_cur * 20 < 480)
				top_position = 100;
			else
				top_position = 100 - (pos_no_cur * 30 + space_no_cur * 20 - 470);
		}
		else
		{
			top_position = 100;
		}


		if (selectingmode == 0)
		{
			SetFontSize(20);
			
			if (keymode == false)
			{
				for (i = 0; i < file.lev_n; i++)
				{
					decidemazesize_bylevel(i + 1, &mapx, &mapz);
					for (j = 0; j < 3; j++)
					{
						if (file.normalstage[i].ischallange[j] != 2) break;
					}
					if (j == 3)
					{
						dispselect(top_position + 30 * pos_no + 20 * space_no, nowlevel == i + 1, S_BLUE, 580, "レベル%d (%d×%d)　クリア", i + 1, mapx, mapz);
					}
					else
					{
						for (j = 0; j < 3; j++)
						{
							if (file.normalstage[i].ischallange[j] != 0) break;
						}
						if (j == 3)
						{
							dispselect(top_position + 30 * pos_no + 20 * space_no, nowlevel == i + 1, S_GREEN, 580, "レベル%d (%d×%d)　未挑戦", i + 1, mapx, mapz);
						}
						else
						{
							dispselect(top_position + 30 * pos_no + 20 * space_no, nowlevel == i + 1, S_YELLOW, 580, "レベル%d (%d×%d)　挑戦中", i + 1, mapx, mapz);
						}
					}

					pos_no++;
				}
			}
			else
			{
				for (i = 0; i < file.lev_n; i++)
				{
					decidemazesize_bylevel(i + 1, &mapx, &mapz);
					for (j = 0; j < 3; j++)
					{
						if (file.keystage[i].ischallange[j] != 2) break;
					}
					if (j == 3)
					{
						dispselect(top_position + 30 * pos_no + 20 * space_no, nowlevel == i + 1, S_BLUE, 580, "レベル%d (%d×%d)　クリア", i + 1, mapx, mapz);
					}
					else
					{
						for (j = 0; j < 3; j++)
						{
							if (file.keystage[i].ischallange[j] != 0) break;
						}
						if (j == 3)
						{
							dispselect(top_position + 30 * pos_no + 20 * space_no, nowlevel == i + 1, S_GREEN, 580, "レベル%d (%d×%d)　未挑戦", i + 1, mapx, mapz);
						}
						else
						{
							dispselect(top_position + 30 * pos_no + 20 * space_no, nowlevel == i + 1, S_YELLOW, 580, "レベル%d (%d×%d)　挑戦中", i + 1, mapx, mapz);
						}
					}

					pos_no++;
				}
			}

			if (nowlevel <= file.lev_n)
			{
				DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  LEVEL SELECT");
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "LEVEL %d SELECTING", nowlevel);
			}
			else if (nowlevel == file.lev_n + 1)
			{
				//編集中
				DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  OTHERS");
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "BUY THE ANSWER ITEM");
			}
			else if (nowlevel == file.lev_n + 2)
			{
				//編集中
				DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  OTHERS");
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "KEYMODE : %s", keymode ? "ON" : "OFF");
			}
			else if (nowlevel == file.lev_n + 3)
			{
				DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  OTHERS");
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "RANDOM PLAY");
			}
			else if (nowlevel == file.lev_n + 4)
			{
				DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  OTHERS");
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "CUSTOMIZE PLAY");
			}
			else
			{
				DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  OTHERS");
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "BACK TO TOP MENU");
			}

			space_no++;

			dispselect(top_position + 30 * pos_no + 20 * space_no, nowlevel == file.lev_n + 1, file.boughtansitem ? S_BLUE : S_RED, 580, "%s", file.boughtansitem ? "現在アンサーアイテム所持中　ステージを開始すると失われます。" : "2000 ポイントでアンサーアイテムを買う");

			pos_no++;

			if (file.lev_n >= 3)
			{
				dispselect(top_position + 30 * pos_no + 20 * space_no, nowlevel == file.lev_n + 2, keymode ? S_BLUE : S_RED, 580, "鍵モード：%s", keymode ? "ON" : "OFF");
			}
			else
			{
				dispselect(top_position + 30 * pos_no + 20 * space_no, nowlevel == file.lev_n + 2, S_GRAY, 580, "鍵モード：%s", keymode ? "ON" : "OFF");
			}

			pos_no++;

			if (file.lev_n >= 5)
			{
				space_no++;

				dispselect(top_position + 30 * pos_no + 20 * space_no, nowlevel == file.lev_n + 3, S_YELLOW, 580, "おまかせプレイ");
				pos_no++;

				dispselect(top_position + 30 * pos_no + 20 * space_no, nowlevel == file.lev_n + 4, S_YELLOW, 580, "カスタムプレイ");

				pos_no++;
			}
			else
			{
				space_no++;

				dispselect(top_position + 30 * pos_no + 20 * space_no, nowlevel == file.lev_n + 3, S_GRAY, 580, "おまかせプレイ");
				pos_no++;

				dispselect(top_position + 30 * pos_no + 20 * space_no, nowlevel == file.lev_n + 4, S_GRAY, 580, "カスタムプレイ");

				pos_no++;
			}

			space_no++;

			dispselect(top_position + 30 * pos_no + 20 * space_no, nowlevel == file.lev_n + 5, S_PURPLE, 580, "戻る");
		}
		else if (selectingmode == 1)
		{
			if (keymode == false)
			{
				DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  STAGE SELECT");

				if (file.normalstage[nowlevel - 1].seed[nowstage - 1] == SEED_NUL)
					DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "STAGE %d-%c NO CHALLANGE", nowlevel, nowstage - 1 + 'A');
				else
					DispStringOfBox(
					10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "STAGE %d-%c (%02d-%05d) %dP",
					nowlevel,
					nowstage - 1 + 'A',
					nowlevel,
					file.normalstage[nowlevel - 1].seed[nowstage - 1],
					file.normalstage[nowlevel - 1].recode[nowstage - 1] / nowlevel);

				SetFontSize(20);

				for (i = 0; i < 3; i++)
				{
					switch (file.normalstage[nowlevel - 1].ischallange[i])
					{
					case 0:
						dispselect(top_position + 30 * pos_no + 20 * space_no, nowstage == i + 1, S_GREEN, 580, "ステージ%d-%c 未挑戦", nowlevel, 'A' + i);
						break;
					case 1:
						dispselect(top_position + 30 * pos_no + 20 * space_no, nowstage == i + 1, S_RED, 580, "ステージ%d-%c %d点 不合格", nowlevel, 'A' + i, file.normalstage[nowlevel - 1].recode[i] / nowlevel);
						break;
					case 2:
						if (file.normalstage[nowlevel - 1].recode[i] / nowlevel >= 300)
						{
							dispselect(top_position + 30 * pos_no + 20 * space_no, nowstage == i + 1, S_YELLOW, 580, "ステージ%d-%c %d点 優良", nowlevel, 'A' + i, file.normalstage[nowlevel - 1].recode[i] / nowlevel);
						}
						else
						{
							dispselect(top_position + 30 * pos_no + 20 * space_no, nowstage == i + 1, S_BLUE, 580, "ステージ%d-%c %d点 合格", nowlevel, 'A' + i, file.normalstage[nowlevel - 1].recode[i] / nowlevel);
						}
						break;
					}

					pos_no++;
				}
				if (file.normalstage[nowlevel - 1].recode[nowstage - 1] / nowlevel >= 300)
				{
					DrawExtendGraph(SCREEN_X / 10, 450, SCREEN_X / 10 + 200, 450 + 120, Handle_Result_VeryGood, FALSE);
				}
				else if (file.normalstage[nowlevel - 1].recode[nowstage - 1] / nowlevel >= 100)
				{
					DrawExtendGraph(SCREEN_X / 10, 450, SCREEN_X / 10 + 200, 450 + 120, Handle_Result_Passed, FALSE);
				}
				else if (file.normalstage[nowlevel - 1].ischallange[nowstage - 1] == 1)
				{
					DrawExtendGraph(SCREEN_X / 10, 450, SCREEN_X / 10 + 200, 450 + 120, Handle_Result_Failed, FALSE);
				}
			}
			else
			{
				DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  STAGE SELECT");

				if (file.keystage[nowlevel - 1].seed[nowstage - 1] == SEED_NUL)
					DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "STAGE %d-%c NO CHALLANGE", nowlevel, nowstage - 1 + 'A');
				else
				{
					if (file.normalstage[nowlevel - 1].recode[nowstage - 1] / nowlevel < 300)
					{
						DispStringOfBox(
							10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "STAGE %d-%c IS NOT SELECT",
							nowlevel,
							nowstage - 1 + 'A');
					}
					else
					{
						DispStringOfBox(
							10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "STAGE %d-%c (%02d-%05d) %dP",
							nowlevel,
							nowstage - 1 + 'A',
							nowlevel,
							file.keystage[nowlevel - 1].seed[nowstage - 1],
							file.keystage[nowlevel - 1].recode[nowstage - 1] / nowlevel);
					}
				}

				SetFontSize(20);
				for (i = 0; i < 3; i++)
				{
					if (file.normalstage[nowlevel - 1].recode[i] / nowlevel < 300)
					{
						dispselect(top_position + 30 * pos_no + 20 * space_no, nowstage == i + 1, S_GRAY, 580, "通常ステージが優良（300点）未満なので挑戦できません");
					}
					else
					{
						switch (file.keystage[nowlevel - 1].ischallange[i])
						{
						case 0:
							dispselect(top_position + 30 * pos_no + 20 * space_no, nowstage == i + 1, S_GREEN, 580, "ステージ%d-%c 未挑戦", nowlevel, 'A' + i);
							break;
						case 1:
							dispselect(top_position + 30 * pos_no + 20 * space_no, nowstage == i + 1, S_RED, 580, "ステージ%d-%c %d点 不合格", nowlevel, 'A' + i, file.keystage[nowlevel - 1].recode[i] / nowlevel);
							break;
						case 2:
							if (file.keystage[nowlevel - 1].recode[i] / nowlevel >= 300)
							{
								dispselect(top_position + 30 * pos_no + 20 * space_no, nowstage == i + 1, S_YELLOW, 580, "ステージ%d-%c %d点 優良", nowlevel, 'A' + i, file.keystage[nowlevel - 1].recode[i] / nowlevel);
							}
							else
							{
								dispselect(top_position + 30 * pos_no + 20 * space_no, nowstage == i + 1, S_BLUE, 580, "ステージ%d-%c %d点 合格", nowlevel, 'A' + i, file.keystage[nowlevel - 1].recode[i] / nowlevel);
							}
							break;
						}
					}

					pos_no++;
				}
				if (file.keystage[nowlevel - 1].recode[nowstage - 1] / nowlevel >= 300)
				{
					DrawExtendGraph(SCREEN_X / 10, 450, SCREEN_X / 10 + 200, 450 + 120, Handle_Result_VeryGood, FALSE);
				}
				else if (file.keystage[nowlevel - 1].recode[nowstage - 1] / nowlevel >= 100)
				{
					DrawExtendGraph(SCREEN_X / 10, 450, SCREEN_X / 10 + 200, 450 + 120, Handle_Result_Passed, FALSE);
				}
				else if (file.keystage[nowlevel - 1].ischallange[nowstage - 1] == 1)
				{
					DrawExtendGraph(SCREEN_X / 10, 450, SCREEN_X / 10 + 200, 450 + 120, Handle_Result_Failed, FALSE);
				}
			}
		}
		else if (selectingmode == 2 || selectingmode == 3)
		{
			if (selectingmode == 2)
				DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  RANDOM MODE");
			if (selectingmode == 3)
				DispStringOfBox(10, 10, 0.8, -1, "MAZE TRAVELER  CUSTOMIZE MODE");

			DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "%02d-%05d, Key:%s", nowlevel, atoi(seed_no), keymode_custom ? "ON" : "OFF");

			SetFontSize(20);

			for (i = 0; i < 6; i++)
			{
				switch (i)
				{
				case 0:
					decidemazesize_bylevel(nowlevel, &mapx, &mapz);
					dispselect(top_position + 30 * pos_no + 20 * space_no, nowmenu == i + 1, S_GREEN, 580, "<  レベル : %d (%d×%d)  >", nowlevel, mapx, mapz);
					break;
				case 1:
					dispselect(top_position + 30 * pos_no + 20 * space_no, nowmenu == i + 1, S_GREEN, 580, "SEED値 : %s", seed_no);

					if (nowmenu == i + 1)
					{
						if (framecnt % 60 < 30)
							DrawLine(
							10 + 96 + cursor * 11,
							100 + 30 * i + 3,
							10 + 96 + cursor * 11,
							100 + 30 * (i + 1) - 3,
							STR_COLOR_2);
					}
					break;
				case 2:
					space_no++;

					dispselect(top_position + 30 * pos_no + 20 * space_no, nowmenu == i + 1, keymode_custom ? S_BLUE : S_RED, 580, "鍵モード：%s", keymode_custom ? "ON" : "OFF");
					break;
				case 3:
					dispselect(top_position + 30 * pos_no + 20 * space_no, nowmenu == i + 1, file.boughtansitem ? S_BLUE : S_RED, 580, "%s", file.boughtansitem ? "現在アンサーアイテム所持中　ステージを開始すると失われます。" : "2000 ポイントでアンサーアイテムを買う");
					break;
				case 4:
					space_no++;
					dispselect(top_position + 30 * pos_no + 20 * space_no, nowmenu == i + 1, S_BLUE, 580, "この設定でプレイ！");
					break;
				case 5:
					space_no++;
					dispselect(top_position + 30 * pos_no + 20 * space_no, nowmenu == i + 1, S_PURPLE, 580, "レベル選択へ戻る");
					break;
				}

				pos_no++;
			}
		}

		disp_navi();

		//■■■ここまで

		//-------------------------------------------------------------------------------------------

		SetDrawScreen(DX_SCREEN_BACK);

		DrawGraph(0, 0, Handle_Screen_Normal, false);

		if((framecnt - called_game_frame) < 60 && kindof_game != -1)
		{
			SetDrawBright(
				255 * (1.0 - double(framecnt - called_game_frame) / 60.0),
				255 * (1.0 - double(framecnt - called_game_frame) / 60.0),
				255 * (1.0 - double(framecnt - called_game_frame) / 60.0)
				);
		}
		else
		{
			SetDrawBright(255, 255, 255);
		}

		framecnt++;

		//表と裏（描画画面）の入れ換え
		ScreenFlip();
	}
	//メインループ終了

	SetDrawScreen(DX_SCREEN_BACK);

	if (brkflag)
		return -1;
	else
		return 0;
}

//挑戦可能レベルアップ
void MazeGame::levelup(void)
{
	int j;

	file.normalstage = (STAGE *)realloc(file.normalstage, sizeof(STAGE)*(file.lev_n + 1));
	file.keystage = (STAGE *)realloc(file.keystage, sizeof(STAGE)*(file.lev_n + 1));

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
}

//ゲームファイルロード
void MazeGame::load(void)
{
	FILE *fp;
	char buf[256];
	char buf2[256];
	char buf3[256];
	int i;

	sprintf_s(buf, 256, "./%s/%s", SAVE_DIR, name);
	sprintf_s(buf2, 256, "./%s/Using_%s.tmp", SAVE_DIR, name);

	fopen_s(&fp, buf, "rb");

	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_SET);

		strcpy_s(buf3, 256, name);
		for (i = strlen(buf3); i >= 0 && buf3[i] != '.'; i--);
		if (i >= 0 && i < 16)
			buf3[i] = '\0';
		else
			buf3[15] = '\0';
		strcpy_s(file.username, 16, buf3);

		//fseek(fp, sizeof(int) * 16, SEEK_CUR);
		//fread(file.username, sizeof(char), 16, fp);

		fread(&file.lev_n, sizeof(int), 1, fp);
		fread(&file.point, sizeof(int), 1, fp);
		fread(&file.online_visited, sizeof(int), 1, fp);
		fread(&file.online_maxpoint, sizeof(int), 1, fp);
		fread(&file.boughtansitem, sizeof(bool), 1, fp);

		file.normalstage = (STAGE *)realloc(file.normalstage, sizeof(STAGE)*(file.lev_n));
		file.keystage = (STAGE *)realloc(file.keystage, sizeof(STAGE)*(file.lev_n));

		fread(file.normalstage, sizeof(STAGE), file.lev_n, fp);
		fread(file.keystage, sizeof(STAGE), file.lev_n, fp);

		fclose(fp);

		fopen_s(&isopen, buf2, "w");
		fclose(isopen);
	}
}

//ゲームファイルセーブ
void MazeGame::save(void)
{
	FILE *fp;
	char buf[256];
	char buf2[256];

	sprintf_s(buf, 256, "./%s/%s", SAVE_DIR, name);
	sprintf_s(buf2, 256, "./%s/Using_%s.tmp", SAVE_DIR, name);

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

		filedel(buf2);
	}
}

//レベルとシード値指定でのファイルの初期処理と後処理。
int MazeGame::PlayMaze_SelectByLevelandSeed(int level, int seed, bool keymode)
{
	int result;
	char stagename[32];

	sprintf_s(stagename, 32, "%02d-%05d", level, seed);

	result = PlayMaze(seed, stagename, keymode, level, 0);

	if (file.boughtansitem == true)
		file.boughtansitem = false;

	if (result >= 0)
	{
		if (result >= 100)
		{
			file.point += result;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return result;
	}
}

//レベルとステージ指定でのファイルの初期処理と後処理。
int MazeGame::PlayMaze_SelectByLevelandStage(int level, int stageno, bool keymode)
{
	int result;
	int seed;
	char stagename[32];

	level--;
	stageno--;

	if (level >= file.lev_n)
		return -1;

	if (keymode == false)
	{
		if (file.normalstage[level].seed[stageno] == SEED_NUL)
		{
			seed = RangeRand(0, 65535);
			file.normalstage[level].seed[stageno] = seed;
			file.keystage[level].seed[stageno] = seed;
		}
		else
		{
			seed = file.normalstage[level].seed[stageno];
		}

		sprintf_s(stagename, 32, "STAGE %d-%c", level + 1, 'A' + stageno);

		result = PlayMaze(seed, stagename, keymode, level + 1, stageno + 1);

		if (file.boughtansitem == true)
			file.boughtansitem = false;

		if (result >= 0)
		{
			if (result >= 100)	file.normalstage[level].ischallange[stageno] = 2;
			else				file.normalstage[level].ischallange[stageno] = 1;

			if (file.normalstage[level].recode[stageno] < result)
				file.normalstage[level].recode[stageno] = result;

			file.point += result;

			return 0;
		}
		else
		{
			return result;
		}
	}
	else
	{
		if (file.keystage[level].seed[stageno] == SEED_NUL)
		{
			seed = RangeRand(0, 65535);
			file.normalstage[level].seed[stageno] = seed;
			file.keystage[level].seed[stageno] = seed;
		}
		else
		{
			seed = file.keystage[level].seed[stageno];
		}

		sprintf_s(stagename, 32, "STAGE %d-%c", level + 1, 'A' + stageno);

		result = PlayMaze(seed, stagename, keymode, level + 1, stageno + 1);

		if (file.boughtansitem == true)
			file.boughtansitem = false;

		if (result >= 0)
		{
			if (result >= 100)	file.keystage[level].ischallange[stageno] = 2;
			else				file.keystage[level].ischallange[stageno] = 1;

			if (file.keystage[level].recode[stageno] < result)
				file.keystage[level].recode[stageno] = result;

			file.point += result;

			return 0;
		}
		else
		{
			return result;
		}
	}
}

//オフライン用迷路プレイ
int MazeGame::PlayMaze(unsigned int seed, char *stagename, bool keymode, int level, int stage = 0)
{
	//-------------------------------------------------------------------------------------------

	//変数宣言
	//迷路データファイル書き込み関連
	//FILE *fp;
	//char *FileName = "MazeKotae.txt";

	//汎用変数
	int i, j, a;
	//自機とカメラ変数
	PosSet_def();

	//int GoaledFlag = 0;
	int TimeCounter = 0;
	int StartedTime = 0;
	int GoaledTime = 0;
	//int StartGoalFlag = 1; //Start:1, Goal:2, Other:0

	//int seedtmp;
	int nolmatime = 0;
	int nowtime = 0;

	bool brkflag = false;
	bool chudanflag = false;

	int framecnt = 0;

	int Jiki_MasuX, Jiki_MasuZ;
	int Jiki_MasuX_before, Jiki_MasuZ_before;

	decidemazesize_bylevel(level, &map.x, &map.z);

	map.y = 1;
	StartGoalFlag = 1;
	DispMode = 0;
	Online_ChatFlag = false;

	//迷路を作成
	GetMazeArray(&map.data, map.x, map.z);				//迷路用の配列を確保
	GetMazeArray(&map.visited, map.x, map.z);
	GetMazeArray(&map.ans, map.x, map.z);
	MakeMaze(map.data, seed, map.x, map.z);
	//SolveMaze(map.data, map.ans, map.x, map.z, 0, 2, map.x - 1, map.z - 3);

	Jiki_MasuX = int(Jiki.Pos.x / Masu_1);
	Jiki_MasuZ = int(Jiki.Pos.z / Masu_1);
	Jiki_MasuX_before = Jiki_MasuX;
	Jiki_MasuZ_before = Jiki_MasuZ;

	item.map.isgotten = false;
	item.ans.isgotten = false;
	item.key.isgotten = false;

	MapSolveFlag = true;

	do
	{
		item.map.x = 2 + (rand() % 5) * 2;
		item.map.z = 2 + (rand() % 5) * 2;

		a = 0;
		SolveMaze(map.data, map.ans, map.x, map.z, 0, 2, item.map.x, item.map.z);			//迷路の答えを作成
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
	} while (a >= 20 || (item.map.x == 2 && item.map.z == 2));

	item.ans.x = 2;
	item.ans.z = 2;

	do
	{
		item.key.x = 2 + (rand() % ((map.x + 1) / 2 - 2)) * 2;
		item.key.z = 2 + (rand() % ((map.z + 1) / 2 - 2)) * 2;
	} while ((item.key.x == item.map.x && item.key.z == item.map.z) || (item.key.x == item.ans.x && item.key.z == item.ans.z) || (item.key.x < map.x / 2 && item.key.z < map.z / 2));

	Map2Dim.Init(map, 0.8, &item);
	dispsolveflag = false;

	for (i = 0; i < map.x; i++)
	{
		for (j = 0; j < map.z; j++)
		{
			map.visited[i][j] = 0;
		}
	}

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

	//目標時間の計算　鍵モードの場合、鍵を取ってくる時間があるので、時間を延長させる。
	{
		nolmatime = 0;

		//スタートからマップまでの経路
		SolveMaze(map.data, map.ans, map.x, map.z, 0, 2, item.map.x, item.map.z);			//迷路の答えを作成
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
			//マップからゴールまでの経路
			SolveMaze(map.data, map.ans, map.x, map.z, item.map.x, item.map.z, map.x - 1, map.z - 3);			//迷路の答えを作成
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
			//マップから鍵までの経路
			SolveMaze(map.data, map.ans, map.x, map.z, item.map.x, item.map.z, item.key.x, item.key.z);			//迷路の答えを作成
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
			//鍵からゴールまでの経路
			SolveMaze(map.data, map.ans, map.x, map.z, item.key.x, item.key.z, map.x - 1, map.z - 3);			//迷路の答えを作成
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

	//メインループ
	TimeCounter = GetNowCount();
	disp_navi_set("");
	while (1)
	{
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
			brkflag = true;

		if (brkflag)
			break;

		if (chudanflag)
			break;

		SetDrawScreen(Handle_Screen_Normal);

		ClsDrawScreen();

		Jiki_MasuX_before = Jiki_MasuX;
		Jiki_MasuZ_before = Jiki_MasuZ;
		Jiki_MasuX = int(Jiki.Pos.x / Masu_1);
		Jiki_MasuZ = int(Jiki.Pos.z / Masu_1);

		/*
		if (level <= 10)
		{
		if (Jiki_MasuX != Jiki_MasuX_before || Jiki_MasuZ != Jiki_MasuZ_before)
		{
		MapSolveFlag = true;
		}
		}
		*/
		if (Jiki_MasuX != Jiki_MasuX_before || Jiki_MasuZ != Jiki_MasuZ_before)
		{
			if (map.ans[Jiki_MasuX][Jiki_MasuZ] == 1)
			{
				map.ans[Jiki_MasuX_before][Jiki_MasuZ_before] = 0;
			}
			else
			{
				map.ans[Jiki_MasuX][Jiki_MasuZ] = 1;
			}
		}

		if (MapSolveFlag == true)
		{
			//マップを手に入れていなかったらマップまでの経路を計算
			if (item.map.isgotten == false)
			{
				SolveMaze(map.data, map.ans, map.x, map.z, int(Jiki.Pos.x / Masu_1), int(Jiki.Pos.z / Masu_1), item.map.x, item.map.z);			//迷路の答えを作成
			}
			else
			{
				if (keymode == false)
				{
					SolveMaze(map.data, map.ans, map.x, map.z, int(Jiki.Pos.x / Masu_1), int(Jiki.Pos.z / Masu_1), map.x - 1, map.z - 3);			//迷路の答えを作成
				}
				else
				{
					//鍵を手に入れていなかったら鍵までの経路を計算
					if (item.key.isgotten == false)
					{
						SolveMaze(map.data, map.ans, map.x, map.z, int(Jiki.Pos.x / Masu_1), int(Jiki.Pos.z / Masu_1), item.key.x, item.key.z);			//迷路の答えを作成
					}
					else
					{
						SolveMaze(map.data, map.ans, map.x, map.z, int(Jiki.Pos.x / Masu_1), int(Jiki.Pos.z / Masu_1), map.x - 1, map.z - 3);			//迷路の答えを作成
					}
				}
			}
			MapSolveFlag = false;
		}


		if (StartGoalFlag != 1)
		{
			if (Stick(KEY_INPUT_Z))
			{
				if (item.map.isgotten == true)
				{
					if (DispMode < 2)	DispMode++;
					else				DispMode = 0;
				}
			}
			if (Stick(KEY_INPUT_X))
			{
				if (item.ans.isgotten == true && file.boughtansitem == true)
				{
					dispsolveflag = !dispsolveflag;
				}
			}
		}

		if (StartGoalFlag == 0)
		{
			if (Stick(KEY_INPUT_F1))
			{
				static int ret;
				int help_starttime;
				int help_endtime;
				help_starttime = GetNowCount();
				ret = disp_help();
				help_endtime = GetNowCount();

				StartedTime += help_endtime - help_starttime;

				if (ret == -1)
				{
					brkflag = true;
					continue;
				}
			}
		}
		if (StartGoalFlag == 1)
		{
			if (Stick(KEY_INPUT_F1))
			{
				static int ret;
				int help_starttime;
				int help_endtime;
				help_starttime = GetNowCount();
				ret = disp_help();
				help_endtime = GetNowCount();

				TimeCounter += help_endtime - help_starttime;

				if (ret == -1)
				{
					brkflag = true;
					continue;
				}
			}
		}

		if (Stick(KEY_INPUT_BACK))
		{
			chudanflag = true;
		}
		if (chudanflag)
			continue;

		if (StartGoalFlag == 0)
		{
			if (((int)(Jiki.Pos.x / 100.0) == map.x - 1 && (int)(Jiki.Pos.z / 100.0) == map.z - 3) && (keymode == false || item.key.isgotten == true))
			{
				StartGoalFlag = 2;
				GoaledTime = GetNowCount();
			}
			nowtime = (GetNowCount() - StartedTime) / 1000;
		}
		else if (StartGoalFlag == 1)
		{
			if ((GetNowCount() - TimeCounter) / 1000 >= 5)
			{
				disp_navi_set("Play_Start");
				StartGoalFlag = 0;
				StartedTime = GetNowCount();
			}
		}
		else if (StartGoalFlag == 2)
		{
			nowtime = (GoaledTime - StartedTime) / 1000;
			if ((GetNowCount() - GoaledTime) / 1000 >= 4) break;
		}


		//■■■ここから

		//-------------------------------------------------------------------------------------------

		disp(DispMode, keymode);

		if (StartGoalFlag != 1)
		{
			if (DispMode != 2)
			{
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
				if (item.ans.isgotten == true && file.boughtansitem == true)
					DrawGraph(SCREEN_X - 120 - 10 + 10 + 35, SCREEN_Y - 80 - 10 + 5, Handle_Gotitem_Ans, false);
				if (item.key.isgotten == true && keymode == true)
					DrawGraph(SCREEN_X - 120 - 10 + 10 + 70, SCREEN_Y - 80 - 10 + 5, Handle_Gotitem_Key, false);
			}
			else
			{
				DispStringOfBox(10, SCREEN_Y - 70, 1.0, -1, "FULL VIEW");
			}
		}
		if (keymode)
			DrawGraph(SCREEN_X - 60, 10, Handle_Challanging_Keymode, false);

		if (file.boughtansitem)
		{
			DrawGraph(SCREEN_X - 120, 10, Handle_Bought_AnsItem, false);
		}

		if (StartGoalFlag == 0)
		{
			int jikan = GetNowCount() - StartedTime;
			double bairitu;

			if (jikan < 1000)
			{
				bairitu = 1.0 + 0.5*((jikan % 1000) / 1000.0);
				DispStringOfBox(SCREEN_X / 2 - 100 * bairitu, SCREEN_Y / 2 - 70 * bairitu, 3.0*bairitu, 2, "GO");
			}

			JikiandCamMove(DispMode, keymode);

			if (DispMode != 2)
			{
				DrawGraph(SCREEN_X - 210, 100, Handle_Help_Navi, FALSE);
			}
		}
		else if (StartGoalFlag == 1)
		{
			int jikan = GetNowCount() - TimeCounter;
			int cntdown = 5 - jikan / 1000;
			double bairitu;

			if (cntdown <= 3)
			{
				bairitu = 1.0 + 0.5*((jikan % 1000) / 1000.0);
				DispStringOfBox(SCREEN_X / 2 - 50 * bairitu, SCREEN_Y / 2 - 70 * bairitu, 3.0*bairitu, (cntdown - 1) * 3, "%d", cntdown);
			}
			else
			{
				DispStringOfBox(SCREEN_X / 2 - 350, SCREEN_Y / 2 - 100, 3.0, -1, "READY?");
			}

			if (DispMode != 2)
			{
				DrawGraph(SCREEN_X - 210, 100, Handle_Help_Navi, FALSE);
			}
		}
		else if (StartGoalFlag == 2)
		{
			JikiandCamMove(DispMode, keymode);

			if ((GetNowCount() - GoaledTime) < 1000)
			{
				DrawExtendGraph(
					SCREEN_X / 2 - 320,
					SCREEN_Y / 2 - 50 * (double(GetNowCount() - GoaledTime) / 1000.0),
					SCREEN_X / 2 + 320,
					SCREEN_Y / 2 + 50 * (double(GetNowCount() - GoaledTime) / 1000.0),
					Handle_Goaled,
					false);
			}
			else
			{
				DrawExtendGraph(SCREEN_X / 2 - 320, SCREEN_Y / 2 - 50, SCREEN_X / 2 + 320, SCREEN_Y / 2 + 50, Handle_Goaled, false);
			}
		}

		SetFontSize(15);
		DrawFormatString(10, SCREEN_Y - 20, STR_COLOR_2, "%d FPS", GetFPS());

		disp_navi(false);

		//■■■ここまで

		//-------------------------------------------------------------------------------------------

		SetDrawScreen(DX_SCREEN_BACK);

		DrawGraph(0, 0, Handle_Screen_Normal, false);

		if (framecnt < 30)
		{
			SetDrawBright(
				255 * (double(framecnt) / 30.0),
				255 * (double(framecnt) / 30.0),
				255 * (double(framecnt) / 30.0)
				);
		}
		else if ((GetNowCount() - GoaledTime) >= 3500 && StartGoalFlag == 2)
		{
			SetDrawBright(
				255 * (1.0 - double((GetNowCount() - GoaledTime) - 3500) / 500.0),
				255 * (1.0 - double((GetNowCount() - GoaledTime) - 3500) / 500.0),
				255 * (1.0 - double((GetNowCount() - GoaledTime) - 3500) / 500.0)
				);
		}
		else
		{
			SetDrawBright(255, 255, 255);
		}

		framecnt++;

		//表と裏（描画画面）の入れ換え
		ScreenFlip();
	}
	//メインループ終了

	SetDrawScreen(DX_SCREEN_BACK);

	//結果発表
	{
		double disp_nolmatime = 0;
		double disp_nowtime = 0;
		double disp_point = 0;
		double disp_totalpoint = file.point;
		int point = (int)(((double)nolmatime / nowtime)*100.0) * level;
		int unlockflag = 0;

		nowtime = (GoaledTime - StartedTime) / 1000;

		framecnt = 0;
		while (1)
		{
			if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
				brkflag = true;

			if (brkflag)
				break;

			if (chudanflag)
				break;

			SetDrawScreen(Handle_Screen_Normal);

			ClearDrawScreen();

			DrawBackGround(SCREEN_X, SCREEN_Y);

			if (framecnt >= 480)
			{
				if (Stick(KEY_INPUT_RETURN))
					break;
			}
			else if (framecnt >= 30)
			{
				//早送り
				if (Stick(KEY_INPUT_RETURN))
				{
					disp_nolmatime = nolmatime;
					disp_nowtime = nowtime;
					disp_point = point;
					framecnt = 480;
				}
			}

			if (framecnt >= 150)
			{
				if (disp_nolmatime + nolmatime / 60.0 < nolmatime)
					disp_nolmatime += nolmatime / 60.0;
				else
					disp_nolmatime = nolmatime;
			}

			if (framecnt >= 240)
			{
				if (disp_nowtime + nowtime / 60.0 < nowtime)
					disp_nowtime += nowtime / 60.0;
				else
					disp_nowtime = nowtime;
			}

			if (framecnt >= 360)
			{
				if (disp_point + point / 60 < point)
					disp_point += point / 60;
				else
					disp_point = point;
			}

			if (framecnt >= 0)
			{
				DispStringOfBox(20, 50, 1.0, -1, "RESULT IN THE %s", stagename);

				if (keymode)
					DrawGraph(SCREEN_X - 60, 10, Handle_Challanging_Keymode, false);

				if (file.boughtansitem)
				{
					DrawGraph(SCREEN_X - 120, 10, Handle_Bought_AnsItem, false);
				}
			}
			if (framecnt >= 90)
			{
				DrawGraph(12, 180, Handle_Result_Tile, false);
				DispStringOfBox(12 + 10, 180 + 10, 1.0, 6, "%s", stagename);

				DispStringOfBox(12 + 140, 180 + 70, 1.0, 7, "%01d", (int)disp_nolmatime / 3600);
				DispStringOfBox(12 + 200, 180 + 70, 1.0, 3, "%02d", (int)disp_nolmatime / 60 % 60);
				DispStringOfBox(12 + 300, 180 + 70, 1.0, 0, "%02d", (int)disp_nolmatime % 60);

				DispStringOfBox(12 + 140, 180 + 140, 1.0, 7, "%01d", (int)disp_nowtime / 3600);
				DispStringOfBox(12 + 200, 180 + 140, 1.0, 3, "%02d", (int)disp_nowtime / 60 % 60);
				DispStringOfBox(12 + 300, 180 + 140, 1.0, 0, "%02d", (int)disp_nowtime % 60);


				for (i = 0; i < 600 && i < int((double)disp_nolmatime / nolmatime * 300); i++)
				{
					if (i <= 100)		DrawLine(12 + 401 + i, 180 + 70 + 20, 12 + 401 + i, 180 + 140 - 20, GetHSVColor(60, 150, 255));
					else if (i <= 300)	DrawLine(12 + 401 + i, 180 + 70 + 20, 12 + 401 + i, 180 + 140 - 20, GetHSVColor(0, 150, 255));
					else				DrawLine(12 + 401 + i, 180 + 70 + 20, 12 + 401 + i, 180 + 140 - 20, GetHSVColor(180, 150, 255));
				}

				for (i = 0; i < 600 && i < int((double)disp_nowtime / nolmatime * 300); i++)
				{
					if (i <= 100)		DrawLine(12 + 401 + i, 180 + 140 + 20, 12 + 401 + i, 180 + 210 - 20, GetHSVColor(60, 150, 255));
					else if (i <= 300)	DrawLine(12 + 401 + i, 180 + 140 + 20, 12 + 401 + i, 180 + 210 - 20, GetHSVColor(0, 150, 255));
					else				DrawLine(12 + 401 + i, 180 + 140 + 20, 12 + 401 + i, 180 + 210 - 20, GetHSVColor(180, 150, 255));
				}
			}
			if (framecnt >= 360)
			{
				DispStringOfBox(10, 420, 1.0, 0, "%.0f POINT", disp_point / level);
				DispStringOfBox(10, 480, 1.0, 6, "->*LEVEL%d = %.0f", level, disp_point);

				for (i = 0; i < 500 && i < int(disp_point / level); i++)
				{
					if (i < 100)		DrawLine(12 + 401 + i, 420, 12 + 401 + i, 420 + 50, GetHSVColor(180, 150, 255));
					else if (i < 300)	DrawLine(12 + 401 + i, 420, 12 + 401 + i, 420 + 50, GetHSVColor(0, 150, 255));
					else				DrawLine(12 + 401 + i, 420, 12 + 401 + i, 420 + 50, GetHSVColor(60, 150, 255));
				}
			}
			if (framecnt >= 480)
			{
				if (point / level >= 300)
				{
					DrawExtendGraph(SCREEN_X / 4 * 3 - 100, 520, SCREEN_X / 4 * 3 + 100, 520 + 120, Handle_Result_VeryGood, FALSE);
				}
				else if (point / level >= 100)
				{
					DrawExtendGraph(SCREEN_X / 4 * 3 - 100, 520, SCREEN_X / 4 * 3 + 100, 520 + 120, Handle_Result_Passed, FALSE);
				}
				else
				{
					DrawExtendGraph(SCREEN_X / 4 * 3 - 100, 520, SCREEN_X / 4 * 3 + 100, 520 + 120, Handle_Result_Failed, FALSE);
				}

				if (framecnt % 60 < 30)
				{
					DispStringOfBox(SCREEN_X / 2 - 350, 680, 1.0, -1, "PRESS ENTER KEY...");
				}
			}

			SetDrawScreen(DX_SCREEN_BACK);

			DrawGraph(0, 0, Handle_Screen_Normal, false);

			if (framecnt < 30)
			{
				SetDrawBright(
					255 * (double(framecnt) / 30.0),
					255 * (double(framecnt) / 30.0),
					255 * (double(framecnt) / 30.0)
					);
			}
			else
			{
				SetDrawBright(255, 255, 255);
			}

			framecnt++;

			ScreenFlip();
		}

		SetDrawScreen(DX_SCREEN_BACK);
		SetDrawBright(255, 255, 255);

		framecnt = 0;
		while (1)
		{
			if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
				brkflag = true;

			if (brkflag)
				break;

			if (chudanflag)
				break;

			ClearDrawScreen();

			DrawBackGround(SCREEN_X, SCREEN_Y);

			if (disp_point <= 0.0)
			{
				if (Stick(KEY_INPUT_RETURN))
					break;
			}
			else
			{
				//早送り
				if (Stick(KEY_INPUT_RETURN))
				{
					disp_totalpoint += disp_point;
					disp_point = 0.0;
					framecnt = 60;
				}
			}

			if (framecnt >= 60)
			{
				if (disp_point > 0.0)
				{
					disp_point -= 2.0;
					disp_totalpoint += 2.0;
				}
				if (disp_point < 0.0)
				{
					disp_point += 1.0;
					disp_totalpoint -= 1.0;
				}
			}

			if (framecnt >= 0)
			{
				DispStringOfBox(20, SCREEN_Y / 4 * 0 + 50, 1.0, -1, "RESULT IN THE %s", stagename);

				if (keymode)
					DrawGraph(SCREEN_X - 60, 10, Handle_Challanging_Keymode, false);

				if (file.boughtansitem)
				{
					DrawGraph(SCREEN_X - 120, 10, Handle_Bought_AnsItem, false);
				}
			}
			if (framecnt >= 60)
			{
				DispStringOfBox(50, SCREEN_Y / 4 * 1 + 50, 1.0, 6, "STAGE: %5.0f POINT", disp_point);
				DispStringOfBox(50, SCREEN_Y / 4 * 2 + 50, 1.0, 7, "TOTAL: %5.0f POINT", disp_totalpoint);
			}
			if (disp_point <= 0.0)
			{
				if (framecnt % 60 < 30)
				{
					DispStringOfBox(SCREEN_X / 2 - 350, SCREEN_Y / 4 * 3 + 50, 1.0, -1, "PRESS ENTER KEY...");
				}
			}

			framecnt++;

			ScreenFlip();
		}

		if (stage != 0 && keymode == false)
		{
			unlockflag = 0;
			//鍵モードのアンロック
			if (file.normalstage[level - 1].recode[stage - 1] < 300 && point / level >= 300)
				unlockflag = 3;

			framecnt = 0;
			while (1)
			{
				if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
					brkflag = true;

				if (brkflag)
					break;

				if (chudanflag)
					break;

				if (unlockflag == 0)
					break;

				ClearDrawScreen();

				DrawBackGround(SCREEN_X, SCREEN_Y);

				if (framecnt >= 180)
				{
					if (Stick(KEY_INPUT_RETURN))
						break;
				}
				else
				{
					//早送り
					if (Stick(KEY_INPUT_RETURN))
					{
						framecnt = 180;
					}
				}

				if (framecnt >= 0)
				{
					DispStringOfBox(20, 50, 1.0, -1, "BONUS UNLOCK THE KEYMODE");

					if (keymode)
						DrawGraph(SCREEN_X - 60, 10, Handle_Challanging_Keymode, false);

					if (file.boughtansitem)
					{
						DrawGraph(SCREEN_X - 120, 10, Handle_Bought_AnsItem, false);
					}
				}
				if (framecnt >= 120)
				{
					if (unlockflag == 3)
					{
						if (file.lev_n <= 2)
						{
							DrawGraph(SCREEN_X / 2 - 400, SCREEN_Y / 2 - 200, Handle_Unlock_KeymodeInStage_UntilLevel2, FALSE);
						}
						else
						{
							DrawGraph(SCREEN_X / 2 - 400, SCREEN_Y / 2 - 150, Handle_Unlock_KeymodeInStage, FALSE);
						}
					}
				}
				if (framecnt >= 180)
				{
					if (framecnt % 60 < 30)
					{
						DispStringOfBox(SCREEN_X / 2 - 350, SCREEN_Y / 4 * 3 + 60, 1.0, -1, "PRESS ENTER KEY...");
					}
				}

				framecnt++;

				ScreenFlip();
			}
			unlockflag = 0;

			//レベルのアンロック
			for (i = 0; i < 3; i++)
			{
				if (i + 1 != stage)
				{
					if (file.normalstage[level - 1].ischallange[i] != 2)
						break;
				}
			}
			if (i == 3 && point / level >= 100 && level == file.lev_n) unlockflag = 4;

			framecnt = 0;
			while (1)
			{
				if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
					brkflag = true;

				if (brkflag)
					break;

				if (chudanflag)
					break;

				if (unlockflag == 0)
					break;

				ClearDrawScreen();

				DrawBackGround(SCREEN_X, SCREEN_Y);

				if (framecnt >= 180)
				{
					if (Stick(KEY_INPUT_RETURN))
						break;
				}
				else
				{
					//早送り
					if (Stick(KEY_INPUT_RETURN))
					{
						disp_navi_set("LevelUp");
						framecnt = 180;
					}
				}

				if (framecnt == 120)
					disp_navi_set("LevelUp");

				if (framecnt >= 0)
				{
					DispStringOfBox(20, 50, 1.0, -1, "BONUS UNLOCK THE LEVEL");

					if (keymode)
						DrawGraph(SCREEN_X - 60, 10, Handle_Challanging_Keymode, false);

					if (file.boughtansitem)
					{
						DrawGraph(SCREEN_X - 120, 10, Handle_Bought_AnsItem, false);
					}
				}
				if (framecnt >= 120)
				{
					if (unlockflag == 4)
					{
						DrawGraph(SCREEN_X / 2 - 400, SCREEN_Y / 2 - 150, Handle_Unlock_Level, FALSE);
					}
				}
				if (framecnt >= 180)
				{
					if (framecnt % 60 < 30)
					{
						DispStringOfBox(SCREEN_X / 2 - 350, SCREEN_Y / 4 * 3 + 60, 1.0, -1, "PRESS ENTER KEY...");
					}
				}

				disp_navi();

				framecnt++;

				ScreenFlip();
			}
			unlockflag = 0;

			//各レベル突破のアンロック
			if (file.lev_n == 2)
			{
				for (i = 0; i < 3; i++)
				{
					if (i + 1 != stage)
					{
						if (file.normalstage[2 - 1].ischallange[i] != 2)
							break;
					}
				}
				if (i == 3 && point / level >= 100) unlockflag = 1;
			}
			if (file.lev_n == 4)
			{
				for (i = 0; i < 3; i++)
				{
					if (i + 1 != stage)
					{
						if (file.normalstage[4 - 1].ischallange[i] != 2)
							break;
					}
				}
				if (i == 3 && point / level >= 100) unlockflag = 2;
			}

			framecnt = 0;
			while (1)
			{
				if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
					brkflag = true;

				if (brkflag)
					break;

				if (chudanflag)
					break;

				if (unlockflag == 0)
					break;

				ClearDrawScreen();

				DrawBackGround(SCREEN_X, SCREEN_Y);

				if (framecnt >= 180)
				{
					if (Stick(KEY_INPUT_RETURN))
						break;
				}
				else
				{
					//早送り
					if (Stick(KEY_INPUT_RETURN))
					{
						if (unlockflag == 1)
							disp_navi_set("UnlockedKeyMode");
						if (unlockflag == 2)
							disp_navi_set("UnlockedCustomPlay");
						framecnt = 180;
					}
				}

				if (framecnt == 120)
				{
					if (unlockflag == 1)
						disp_navi_set("UnlockedKeyMode");
					if (unlockflag == 2)
						disp_navi_set("UnlockedCustomPlay");
				}

				if (framecnt >= 0)
				{
					DispStringOfBox(20, 50, 1.0, -1, "BONUS UNLOCK THE MODE");

					if (keymode)
						DrawGraph(SCREEN_X - 60, 10, Handle_Challanging_Keymode, false);

					if (file.boughtansitem)
					{
						DrawGraph(SCREEN_X - 120, 10, Handle_Bought_AnsItem, false);
					}
				}
				if (framecnt >= 120)
				{
					if (unlockflag == 1)
					{
						DrawGraph(SCREEN_X / 2 - 400, SCREEN_Y / 2 - 150, Handle_Level3_Unlock, FALSE);
					}
					if (unlockflag == 2)
					{
						DrawGraph(SCREEN_X / 2 - 400, SCREEN_Y / 2 - 150, Handle_Level5_Unlock, FALSE);
					}
				}
				if (framecnt >= 180)
				{
					if (framecnt % 60 < 30)
					{
						DispStringOfBox(SCREEN_X / 2 - 350, SCREEN_Y / 4 * 3 + 60, 1.0, -1, "PRESS ENTER KEY...");
					}
				}

				disp_navi();

				framecnt++;

				ScreenFlip();
			}
		}
	}

	disp_navi_set("");

	DeleteMazeArray(map.data, map.x);
	DeleteMazeArray(map.visited, map.x);
	DeleteMazeArray(map.ans, map.x);
	DeleteLightHandle(L1);

	Map2Dim.Dest();

	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawBright(255, 255, 255);

	if (chudanflag)
		return -2;
	else if (brkflag)
		return -1;
	else
		return (int)(((double)nolmatime / nowtime)*100.0) * level;
}

//オンライン用迷路プレイ
int MazeGame::PlayMaze_online(unsigned int seed, char *stagename, bool keymode, int level)
{
	//汎用変数
	int i, j, a;
	//自機とカメラ変数
	PosSet_def();

	//int GoaledFlag = 0;
	int TimeCounter = 0;
	int StartedTime = 0;
	int GoaledTime = 0;
	//int StartGoalFlag = 1; //Start:1, Goal:2, Other:0

	//int seedtmp;
	int nolmatime = 0;
	int nowtime = 0;

	bool brkflag = false;
	bool chudanflag = false;

	int framecnt = 0;

	int Jiki_MasuX, Jiki_MasuZ;
	int Jiki_MasuX_before, Jiki_MasuZ_before;

	int point = 0;
	int point_sub1;
	int point_sub2;
	int point_sub3;
	int yourrank;
	int member_n;

	FORSEND fs;
	MAZE_DB mdb;

	AddLogFiles("Level = %d\n", level);

	decidemazesize_bylevel(level, &map.x, &map.z);

	AddLogFiles("size = (%d,%d)\n", map.x, map.z);

	map.y = 1;
	StartGoalFlag = 1;
	DispMode = 0;

	//迷路を作成
	GetMazeArray(&map.data, map.x, map.z);				//迷路用の配列を確保
	GetMazeArray(&map.visited, map.x, map.z);
	GetMazeArray(&map.ans, map.x, map.z);
	MakeMaze(map.data, seed, map.x, map.z);
	//SolveMaze(map.data, map.ans, map.x, map.z, 0, 2, map.x - 1, map.z - 3);

	Jiki_MasuX = int(Jiki.Pos.x / Masu_1);
	Jiki_MasuZ = int(Jiki.Pos.z / Masu_1);
	Jiki_MasuX_before = Jiki_MasuX;
	Jiki_MasuZ_before = Jiki_MasuZ;

	item.map.isgotten = false;
	item.ans.isgotten = true;
	item.key.isgotten = false;

	MapSolveFlag = true;


	do
	{
		item.map.x = 2 + (rand() % 5) * 2;
		item.map.z = 2 + (rand() % 5) * 2;

		a = 0;
		SolveMaze(map.data, map.ans, map.x, map.z, 0, 2, item.map.x, item.map.z);			//迷路の答えを作成
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
	} while (a >= 20 || (item.map.x == 2 && item.map.z == 2));

	item.ans.x = 2;
	item.ans.z = 2;

	do
	{
		item.key.x = 2 + (rand() % ((map.x + 1) / 2 - 2)) * 2;
		item.key.z = 2 + (rand() % ((map.z + 1) / 2 - 2)) * 2;
	} while ((item.key.x == item.map.x && item.key.z == item.map.z) || (item.key.x == item.ans.x && item.key.z == item.ans.z) || (item.key.x < map.x / 2 && item.key.z < map.z / 2));

	Map2Dim.Init(map, 0.8, &item);
	dispsolveflag = false;

	for (i = 0; i < map.x; i++)
	{
		for (j = 0; j < map.z; j++)
		{
			map.visited[i][j] = 0;
		}
	}

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

	//目標時間の計算　鍵モードの場合、鍵を取ってくる時間があるので、時間を延長させる。
	{
		nolmatime = 0;

		//スタートからマップまでの経路
		SolveMaze(map.data, map.ans, map.x, map.z, 0, 2, item.map.x, item.map.z);			//迷路の答えを作成
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
			//マップからゴールまでの経路
			SolveMaze(map.data, map.ans, map.x, map.z, item.map.x, item.map.z, map.x - 1, map.z - 3);			//迷路の答えを作成
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
			//マップから鍵までの経路
			SolveMaze(map.data, map.ans, map.x, map.z, item.map.x, item.map.z, item.key.x, item.key.z);			//迷路の答えを作成
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
			//鍵からゴールまでの経路
			SolveMaze(map.data, map.ans, map.x, map.z, item.key.x, item.key.z, map.x - 1, map.z - 3);			//迷路の答えを作成
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

	//メインループ
	TimeCounter = GetNowCount();
	disp_navi_set("");
	nowtime = 0;
	while (1)
	{
		if (ProcessMessage() == -1 || (Stick(KEY_INPUT_ESCAPE) && ChatInputFlag == false))
			brkflag = true;

		if (brkflag)
		{
			NetMatch.maze_cancel();
			break;
		}

		if (chudanflag)
		{
			NetMatch.maze_cancel();
			break;
		}

		SetDrawScreen(Handle_Screen_Normal);

		ClsDrawScreen();

		//接続が切られたら接続画面に戻る
		if (NetMatch.checklost() == -1)
		{
			chudanflag = true;
			continue;
		}

		//自分の情報を送信する
		fs.mazetime = nowtime;
		mdb.Jiki = Jiki;
		mdb.CamKakudo = Camera.Kakudo;
		strcpy_s(mdb.username, 16, file.username);
		NetMatch.SetMyInfo(mdb, fs);

		//受信状況を確認などする
		NetMatch.update();

		//他のプレイヤーの情報を得る
		NetMatch.GetMemberInfo(&mret);

		Jiki_MasuX_before = Jiki_MasuX;
		Jiki_MasuZ_before = Jiki_MasuZ;
		Jiki_MasuX = int(Jiki.Pos.x / Masu_1);
		Jiki_MasuZ = int(Jiki.Pos.z / Masu_1);

		/*
		if (level <= 10)
		{
		if (Jiki_MasuX != Jiki_MasuX_before || Jiki_MasuZ != Jiki_MasuZ_before)
		{
		MapSolveFlag = true;
		}
		}
		*/
		//マス座標が変化したら
		if (Jiki_MasuX != Jiki_MasuX_before || Jiki_MasuZ != Jiki_MasuZ_before)
		{
			if (map.ans[Jiki_MasuX][Jiki_MasuZ] == 1)
			{
				map.ans[Jiki_MasuX_before][Jiki_MasuZ_before] = 0;
			}
			else
			{
				map.ans[Jiki_MasuX][Jiki_MasuZ] = 1;
			}
		}

		if (MapSolveFlag == true)
		{
			//マップを手に入れていなかったらマップまでの経路を計算
			if (item.map.isgotten == false)
			{
				SolveMaze(map.data, map.ans, map.x, map.z, int(Jiki.Pos.x / Masu_1), int(Jiki.Pos.z / Masu_1), item.map.x, item.map.z);			//迷路の答えを作成
			}
			else
			{
				if (keymode == false)
				{
					SolveMaze(map.data, map.ans, map.x, map.z, int(Jiki.Pos.x / Masu_1), int(Jiki.Pos.z / Masu_1), map.x - 1, map.z - 3);			//迷路の答えを作成
				}
				else
				{
					//鍵を手に入れていなかったら鍵までの経路を計算
					if (item.key.isgotten == false)
					{
						SolveMaze(map.data, map.ans, map.x, map.z, int(Jiki.Pos.x / Masu_1), int(Jiki.Pos.z / Masu_1), item.key.x, item.key.z);			//迷路の答えを作成
					}
					else
					{
						SolveMaze(map.data, map.ans, map.x, map.z, int(Jiki.Pos.x / Masu_1), int(Jiki.Pos.z / Masu_1), map.x - 1, map.z - 3);			//迷路の答えを作成
					}
				}
			}
			MapSolveFlag = false;
		}

		//チャット入力時以外に操作可能
		if (ChatInputFlag == false)
		{
			if (StartGoalFlag != 1)
			{
				//マップ表示切り替え
				if (Stick(KEY_INPUT_Z))
				{
					if (item.map.isgotten == true)
					{
						if (DispMode < 2)	DispMode++;
						else				DispMode = 0;
					}
				}
				////答え表示切り替え
				//if (Stick(KEY_INPUT_X))
				//{
				//	if (item.ans.isgotten == true && file.boughtansitem == true)
				//	{
				//		dispsolveflag = !dispsolveflag;
				//	}
				//}
			}

			//チャット窓表示切替
			if (Stick(KEY_INPUT_C))
			{
				Online_ChatFlag = !Online_ChatFlag;
			}

			//ヘルプ
			if (StartGoalFlag == 0)
			{
				if (Stick(KEY_INPUT_F1))
				{
					static int ret;
					int help_starttime;
					int help_endtime;
					help_starttime = GetNowCount();
					ret = disp_help();
					help_endtime = GetNowCount();

					StartedTime += help_endtime - help_starttime;

					if (ret == -1)
					{
						brkflag = true;
						continue;
					}
				}
			}
			else if (StartGoalFlag == 1)
			{
				if (Stick(KEY_INPUT_F1))
				{
					static int ret;

					ret = disp_help();

					if (ret == -1)
					{
						brkflag = true;
						continue;
					}
				}
			}

			//戻る
			if (Stick(KEY_INPUT_BACK))
			{
				chudanflag = true;
				NetMatch.maze_cancel();
				continue;
			}

			if (StartGoalFlag == 0)
			{
				JikiandCamMove(DispMode, keymode);
			}
			else if (StartGoalFlag == 1)
			{
			}
			else if (StartGoalFlag == 2)
			{
				JikiandCamMove(DispMode, keymode);
			}
		}

		//プレイ中
		if (StartGoalFlag == 0)
		{
			if (((int)(Jiki.Pos.x / 100.0) == map.x - 1 && (int)(Jiki.Pos.z / 100.0) == map.z - 3) && (keymode == false || item.key.isgotten == true))
			{
				StartGoalFlag = 2;
				GoaledTime = GetNowCount();
				NetMatch.maze_goal();
			}
			nowtime = (GetNowCount() - StartedTime) / 1000;
		}
		//スタート
		else if (StartGoalFlag == 1)
		{
			if ((GetNowCount() - TimeCounter) / 1000 >= 5)
			{
				disp_navi_set("Play_Start");
				StartGoalFlag = 0;
				StartedTime = GetNowCount();
				NetMatch.maze_start();
			}
		}
		//ゴール
		else if (StartGoalFlag == 2)
		{
			nowtime = (GoaledTime - StartedTime) / 1000;
			if ((GetNowCount() - GoaledTime) / 1000 >= 4) break;
		}


		//表示部
		//-------------------------------------------------------------------------------------------

		disp_online(DispMode, keymode);

		if (DispMode != 2)
			inputchat();

		//時間板
		if (StartGoalFlag != 1)
		{
			if (DispMode != 2)
			{
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
				//if (item.ans.isgotten == true && file.boughtansitem == true)
				//	DrawGraph(SCREEN_X - 120 - 10 + 10 + 35, SCREEN_Y - 80 - 10 + 5, Handle_Gotitem_Ans, false);
				if (item.key.isgotten == true && keymode == true)
					DrawGraph(SCREEN_X - 120 - 10 + 10 + 70, SCREEN_Y - 80 - 10 + 5, Handle_Gotitem_Key, false);

				SetFontSize(20);
				a = 0;
				for (i = 0; i < mret.mdb_n; i++)
				{
					if (i < 5 || i == mret.yourno)
					{
						//順位順
						switch (mret.member[i].rank)
						{
							//ベスト3
						case 1:
							DrawExtendGraph(10, 220 + a * 30, 300, 220 + (a + 1) * 30, Handle_SelectTile_Yellow, false);
							break;
						case 2:
							DrawExtendGraph(10, 220 + a * 30, 300, 220 + (a + 1) * 30, Handle_SelectTile_Gray, false);
							break;
						case 3:
							DrawExtendGraph(10, 220 + a * 30, 300, 220 + (a + 1) * 30, Handle_SelectTile_Red, false);
							break;
							//挑戦中
						case 32768:
							DrawExtendGraph(10, 220 + a * 30, 300, 220 + (a + 1) * 30, Handle_SelectTile_Purple, false);
							break;
						default:
							DrawExtendGraph(10, 220 + a * 30, 300, 220 + (a + 1) * 30, Handle_SelectTile_Green, false);
							break;
						}
						if (i == mret.yourno)
							DrawGraph(10, 220 + a * 30, Handle_Online_Mine, true);

						if (mret.member[i].rank != 32768)
						{
							DrawFormatString(10, 220 + a * 30, STR_COLOR_2,
								"%d位 %sさん(%02d:%02d:%02d)",
								mret.member[i].rank,
								mret.mdb[i].username,
								mret.member[i].fs.mazetime / 60 / 60,
								mret.member[i].fs.mazetime / 60 % 60,
								mret.member[i].fs.mazetime % 60
								);
						}
						else
						{
							DrawFormatString(10, 220 + a * 30, STR_COLOR_2,
								"挑戦中 %sさん(%02d:%02d:%02d)",
								mret.mdb[i].username,
								mret.member[i].fs.mazetime / 60 / 60,
								mret.member[i].fs.mazetime / 60 % 60,
								mret.member[i].fs.mazetime % 60
								);
						}
						a++;
					}
				}
			}
			else
			{
				DispStringOfBox(10, SCREEN_Y - 70, 1.0, -1, "FULL VIEW");

				SetFontSize(20);
				a = 0;
				for (i = 0; i < mret.mdb_n; i++)
				{
					if (i < 10 || i == mret.yourno)
					{
						//順位順
						switch (mret.member[i].rank)
						{
						case 1:
							DrawExtendGraph(10, 10 + a * 30, 300, 10 + (a + 1) * 30, Handle_SelectTile_Yellow, false);
							break;
						case 2:
							DrawExtendGraph(10, 10 + a * 30, 300, 10 + (a + 1) * 30, Handle_SelectTile_Gray, false);
							break;
						case 3:
							DrawExtendGraph(10, 10 + a * 30, 300, 10 + (a + 1) * 30, Handle_SelectTile_Red, false);
							break;
							//挑戦中
						case 32768:
							DrawExtendGraph(10, 10 + a * 30, 300, 10 + (a + 1) * 30, Handle_SelectTile_Purple, false);
							break;
						default:
							DrawExtendGraph(10, 10 + a * 30, 300, 10 + (a + 1) * 30, Handle_SelectTile_Green, false);
							break;
						}
						if (i == mret.yourno)
							DrawGraph(10, 10 + a * 30, Handle_Online_Mine, true);

						if (mret.member[i].rank != 32768)
						{
							DrawFormatString(10, 10 + a * 30, STR_COLOR_2,
								"%d位 %sさん(%02d:%02d:%02d)",
								mret.member[i].rank,
								mret.mdb[i].username,
								mret.member[i].fs.mazetime / 60 / 60,
								mret.member[i].fs.mazetime / 60 % 60,
								mret.member[i].fs.mazetime % 60
								);
						}
						else
						{
							DrawFormatString(10, 10 + a * 30, STR_COLOR_2,
								"挑戦中 %sさん(%02d:%02d:%02d)",
								mret.mdb[i].username,
								mret.member[i].fs.mazetime / 60 / 60,
								mret.member[i].fs.mazetime / 60 % 60,
								mret.member[i].fs.mazetime % 60
								);
						}
						a++;
					}
				}
			}
		}

		/*
		clsDx();
		printfDx("NetBuffer = %dByte\n", GetNetWorkDataLength(NetHandle));
		*/

		//鍵モード
		if (keymode)
			DrawGraph(SCREEN_X - 60, 10, Handle_Challanging_Keymode, false);

		//if (file.boughtansitem)
		//{
		//	DrawGraph(SCREEN_X - 120, 10, Handle_Bought_AnsItem, false);
		//}

		if (StartGoalFlag == 0)
		{
			int jikan = GetNowCount() - StartedTime;
			double bairitu;

			if (jikan < 1000)
			{
				bairitu = 1.0 + 0.5*((jikan % 1000) / 1000.0);
				DispStringOfBox(SCREEN_X / 2 - 100 * bairitu, SCREEN_Y / 2 - 70 * bairitu, 3.0*bairitu, 2, "GO");
			}

			if (DispMode != 2)
			{
				DrawGraph(SCREEN_X - 210, 100, Handle_Help_Navi, FALSE);
			}
		}
		else if (StartGoalFlag == 1)
		{
			int jikan = GetNowCount() - TimeCounter;
			int cntdown = 5 - jikan / 1000;
			double bairitu;

			if (cntdown <= 3)
			{
				bairitu = 1.0 + 0.5*((jikan % 1000) / 1000.0);
				DispStringOfBox(SCREEN_X / 2 - 50 * bairitu, SCREEN_Y / 2 - 70 * bairitu, 3.0*bairitu, (cntdown - 1) * 3, "%d", cntdown);
			}
			else
			{
				DispStringOfBox(SCREEN_X / 2 - 350, SCREEN_Y / 2 - 100, 3.0, -1, "READY?");
			}

			if (DispMode != 2)
			{
				DrawGraph(SCREEN_X - 210, 100, Handle_Help_Navi, FALSE);
			}
		}
		else if (StartGoalFlag == 2)
		{
			//JikiandCamMove(DispMode, keymode);

			if ((GetNowCount() - GoaledTime) < 1000)
			{
				DrawExtendGraph(
					SCREEN_X / 2 - 320,
					SCREEN_Y / 2 - 50 * (double(GetNowCount() - GoaledTime) / 1000.0),
					SCREEN_X / 2 + 320,
					SCREEN_Y / 2 + 50 * (double(GetNowCount() - GoaledTime) / 1000.0),
					Handle_Goaled,
					false);
			}
			else
			{
				DrawExtendGraph(SCREEN_X / 2 - 320, SCREEN_Y / 2 - 50, SCREEN_X / 2 + 320, SCREEN_Y / 2 + 50, Handle_Goaled, false);
			}
		}

		//SetFontSize(15);
		//DrawFormatString(10, SCREEN_Y - 20, STR_COLOR_2, "%d FPS", GetFPS());

		disp_navi(false);

		//■■■ここまで

		//-------------------------------------------------------------------------------------------

		//裏の裏画面から裏画面へ描画を変更
		SetDrawScreen(DX_SCREEN_BACK);

		//裏の裏画面の描画を裏画面にコピー
		DrawGraph(0, 0, Handle_Screen_Normal, false);

		//フェードイン、フェードアウト
		if (framecnt < 30)
		{
			SetDrawBright(
				255 * (double(framecnt) / 30.0),
				255 * (double(framecnt) / 30.0),
				255 * (double(framecnt) / 30.0)
				);
		}
		else if ((GetNowCount() - GoaledTime) >= 3500 && StartGoalFlag == 2)
		{
			SetDrawBright(
				255 * (1.0 - double((GetNowCount() - GoaledTime) - 3500) / 500.0),
				255 * (1.0 - double((GetNowCount() - GoaledTime) - 3500) / 500.0),
				255 * (1.0 - double((GetNowCount() - GoaledTime) - 3500) / 500.0)
				);
		}
		else
		{
			SetDrawBright(255, 255, 255);
		}

		framecnt++;

		//表と裏（描画画面）の入れ換え
		ScreenFlip();
	}
	//メインループ終了

	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawBright(255, 255, 255);

	//受信状況を確認などする
	while (NetMatch.update() > 0);

	//他のプレイヤーの情報を得る
	NetMatch.GetMemberInfo(&mret);

	NetMatch.room_dis();

	//受信状況を確認などする
	while (NetMatch.update() > 0);

	//結果発表
	if (brkflag == false && chudanflag == false)
	{
		//自分の順位、得点などを計算する
		yourrank = mret.member[mret.yourno].rank;
		member_n = mret.mdb_n;
		point_sub1 = (int)(((double)nolmatime / nowtime)*100.0) * level;
		point_sub2 = (int)((double)(member_n*point_sub1*(member_n - yourrank)) / 10.0);
		point_sub3 = point_sub1 + point_sub2;
		point = point_sub3 * level;

		//自分の情報を送信する
		fs.mazetime = nowtime;
		mdb.Jiki = Jiki;
		mdb.CamKakudo = Camera.Kakudo;
		strcpy_s(mdb.username, 16, file.username);
		NetMatch.SetMyInfo(mdb, fs);

		double disp_nolmatime = 0;
		double disp_nowtime = 0;
		double disp_point = 0;
		double disp_point_sub1 = 0;
		double disp_point_sub2 = 0;
		double disp_point_sub3 = 0;
		double disp_totalpoint = file.point;
		double disp_onlinepoint = file.online_maxpoint;
		//int point = (int)(((double)nolmatime / nowtime)*100.0) * level;
		//int unlockflag = 0;

		//結果がネットから送られてくる
		nowtime = mret.member[mret.yourno].fs.mazetime;

		//
		framecnt = 0;
		while (1)
		{
			if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
				brkflag = true;

			if (brkflag)
				break;

			if (chudanflag)
				break;

			NetMatch.update();

			SetDrawScreen(Handle_Screen_Normal);

			ClearDrawScreen();

			DrawBackGround(SCREEN_X, SCREEN_Y);

			if (framecnt >= 480)
			{
				if (Stick(KEY_INPUT_RETURN))
					break;
			}
			else if (framecnt >= 30)
			{
				//早送り
				if (Stick(KEY_INPUT_RETURN))
				{
					disp_nolmatime = nolmatime;
					disp_nowtime = nowtime;
					disp_point_sub1 = point_sub1;
					framecnt = 480;
				}
			}

			if (framecnt >= 150)
			{
				if (disp_nolmatime + nolmatime / 60.0 < nolmatime)
					disp_nolmatime += nolmatime / 60.0;
				else
					disp_nolmatime = nolmatime;
			}

			if (framecnt >= 240)
			{
				if (disp_nowtime + nowtime / 60.0 < nowtime)
					disp_nowtime += nowtime / 60.0;
				else
					disp_nowtime = nowtime;
			}

			if (framecnt >= 360)
			{
				if (disp_point_sub1 + point_sub1 / 60 < point_sub1)
					disp_point_sub1 += point_sub1 / 60;
				else
					disp_point_sub1 = point_sub1;
			}

			if (framecnt >= 0)
			{
				DispStringOfBox(20, 50, 1.0, -1, "RESULT IN THE %s", stagename);

				if (keymode)
					DrawGraph(SCREEN_X - 60, 10, Handle_Challanging_Keymode, false);
			}
			if (framecnt >= 90)
			{
				DrawGraph(12, 180, Handle_Result_Tile, false);
				DispStringOfBox(12 + 10, 180 + 10, 1.0, 6, "%s", stagename);

				DispStringOfBox(12 + 140, 180 + 70, 1.0, 7, "%01d", (int)disp_nolmatime / 3600);
				DispStringOfBox(12 + 200, 180 + 70, 1.0, 3, "%02d", (int)disp_nolmatime / 60 % 60);
				DispStringOfBox(12 + 300, 180 + 70, 1.0, 0, "%02d", (int)disp_nolmatime % 60);

				DispStringOfBox(12 + 140, 180 + 140, 1.0, 7, "%01d", (int)disp_nowtime / 3600);
				DispStringOfBox(12 + 200, 180 + 140, 1.0, 3, "%02d", (int)disp_nowtime / 60 % 60);
				DispStringOfBox(12 + 300, 180 + 140, 1.0, 0, "%02d", (int)disp_nowtime % 60);


				//if (file.boughtansitem)
				//{
				//	DrawGraph(SCREEN_X - 120, 10, Handle_Bought_AnsItem, false);
				//}

				for (i = 0; i < 600 && i < int((double)disp_nolmatime / nolmatime * 300); i++)
				{
					if (i <= 100)		DrawLine(12 + 401 + i, 180 + 70 + 20, 12 + 401 + i, 180 + 140 - 20, GetHSVColor(60, 150, 255));
					else if (i <= 300)	DrawLine(12 + 401 + i, 180 + 70 + 20, 12 + 401 + i, 180 + 140 - 20, GetHSVColor(0, 150, 255));
					else				DrawLine(12 + 401 + i, 180 + 70 + 20, 12 + 401 + i, 180 + 140 - 20, GetHSVColor(180, 150, 255));
				}

				for (i = 0; i < 600 && i < int((double)disp_nowtime / nolmatime * 300); i++)
				{
					if (i <= 100)		DrawLine(12 + 401 + i, 180 + 140 + 20, 12 + 401 + i, 180 + 210 - 20, GetHSVColor(60, 150, 255));
					else if (i <= 300)	DrawLine(12 + 401 + i, 180 + 140 + 20, 12 + 401 + i, 180 + 210 - 20, GetHSVColor(0, 150, 255));
					else				DrawLine(12 + 401 + i, 180 + 140 + 20, 12 + 401 + i, 180 + 210 - 20, GetHSVColor(180, 150, 255));
				}
			}
			if (framecnt >= 360)
			{
				DispStringOfBox(10, 420, 1.0, 0, "%.0f POINT", disp_point_sub1 / level);
				DispStringOfBox(10, 480, 1.0, 6, "->*LEVEL%d = %.0f", level, disp_point_sub1);

				for (i = 0; i < 500 && i < int(disp_point_sub1 / level); i++)
				{
					if (i < 100)		DrawLine(12 + 401 + i, 420, 12 + 401 + i, 420 + 50, GetHSVColor(180, 150, 255));
					else if (i < 300)	DrawLine(12 + 401 + i, 420, 12 + 401 + i, 420 + 50, GetHSVColor(0, 150, 255));
					else				DrawLine(12 + 401 + i, 420, 12 + 401 + i, 420 + 50, GetHSVColor(60, 150, 255));
				}
			}
			if (framecnt >= 480)
			{
				if (point_sub1 / level >= 300)
				{
					DrawExtendGraph(SCREEN_X / 4 * 3 - 100, 520, SCREEN_X / 4 * 3 + 100, 520 + 120, Handle_Result_VeryGood, FALSE);
				}
				else if (point_sub1 / level >= 100)
				{
					DrawExtendGraph(SCREEN_X / 4 * 3 - 100, 520, SCREEN_X / 4 * 3 + 100, 520 + 120, Handle_Result_Passed, FALSE);
				}
				else
				{
					DrawExtendGraph(SCREEN_X / 4 * 3 - 100, 520, SCREEN_X / 4 * 3 + 100, 520 + 120, Handle_Result_Failed, FALSE);
				}

				if (framecnt % 60 < 30)
				{
					DispStringOfBox(SCREEN_X / 2 - 350, 680, 1.0, -1, "PRESS ENTER KEY...");
				}
			}

			SetDrawScreen(DX_SCREEN_BACK);

			DrawGraph(0, 0, Handle_Screen_Normal, false);

			if (framecnt < 30)
			{
				SetDrawBright(
					255 * (double(framecnt) / 30.0),
					255 * (double(framecnt) / 30.0),
					255 * (double(framecnt) / 30.0)
					);
			}
			else
			{
				SetDrawBright(255, 255, 255);
			}

			framecnt++;

			ScreenFlip();
		}

		SetDrawScreen(DX_SCREEN_BACK);
		SetDrawBright(255, 255, 255);

		disp_point_sub1 = 0.0;
		disp_point_sub2 = 0.0;
		disp_point_sub3 = 0.0;
		disp_onlinepoint = 0.0;
		framecnt = 0;
		while (1)
		{
			if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
				brkflag = true;

			if (brkflag)
				break;

			if (chudanflag)
				break;

			NetMatch.update();

			ClearDrawScreen();

			DrawBackGround(SCREEN_X, SCREEN_Y);

			if (framecnt >= 60 + 120 * 5)
			{
				if (Stick(KEY_INPUT_RETURN))
					break;
			}
			else
			{
				//早送り
				if (Stick(KEY_INPUT_RETURN))
				{
					framecnt = 60 + 120 * 5;
					disp_point_sub1 = point_sub1;
					disp_point_sub2 = point_sub2;
					disp_point_sub3 = point_sub3;
					disp_onlinepoint = point;
				}
			}

			if (framecnt >= 60)
			{
				if (disp_point_sub1 < point_sub1)
				{
					disp_point_sub1 += (double)point_sub1 / 90.0;
				}
				else
				{
					disp_point_sub1 = point_sub1;
				}
			}
			if (framecnt >= 60 + 120 * 1)
			{
				if (disp_point_sub2 < point_sub2)
				{
					disp_point_sub2 += (double)point_sub2 / 90.0;
				}
				else
				{
					disp_point_sub2 = point_sub2;
				}
			}
			if (framecnt >= 60 + 120 * 2)
			{
				if (disp_point_sub3 < point_sub3)
				{
					disp_point_sub3 += (double)point_sub3 / 90.0;
				}
				else
				{
					disp_point_sub3 = point_sub3;
				}
			}
			if (framecnt >= 60 + 120 * 4)
			{
				if (disp_onlinepoint < point)
				{
					disp_onlinepoint += (double)point / 90.0;
				}
				else
				{
					disp_onlinepoint = point;
				}
			}

			if (framecnt >= 0)
			{
				DispStringOfBox(20, SCREEN_Y / 7 * 0 + 50, 1.0, -1, "RESULT IN THE %s", stagename);

				if (keymode)
					DrawGraph(SCREEN_X - 60, 10, Handle_Challanging_Keymode, false);

				//if (file.boughtansitem)
				//{
				//	DrawGraph(SCREEN_X - 120, 10, Handle_Bought_AnsItem, false);
				//}
			}
			if (framecnt >= 60)
			{
				DispStringOfBox(50, SCREEN_Y / 7 * 1 + 50, 1.0, 6, "    STAGE: %5.0f POINT", disp_point_sub1);
			}
			if (framecnt >= 60 + 120 * 1)
			{
				DispStringOfBox(50, SCREEN_Y / 7 * 2 + 50, 1.0, 6, "    RANK : %5.0f POINT", disp_point_sub2);
			}
			if (framecnt >= 60 + 120 * 2)
			{
				DispStringOfBox(50, SCREEN_Y / 7 * 3 + 50, 1.0, 7, " +  TOTAL: %5.0f POINT", disp_point_sub3);
			}
			if (framecnt >= 60 + 120 * 3)
			{
				DispStringOfBox(50, SCREEN_Y / 7 * 4 + 50, 1.0, 6, "    LEVEL: %d", level);
			}
			if (framecnt >= 60 + 120 * 4)
			{
				DispStringOfBox(50, SCREEN_Y / 7 * 5 + 50, 1.0, 7, " *  TOTAL: %5.0f POINT", disp_onlinepoint);
			}
			if (framecnt >= 60 + 120 * 5)
			{
				if (framecnt % 60 < 30)
				{
					DispStringOfBox(SCREEN_X / 2 - 350, SCREEN_Y / 7 * 6 + 50, 1.0, -1, "PRESS ENTER KEY...");
				}
			}

			framecnt++;

			ScreenFlip();
		}

		//ゲームポイント加算
		framecnt = 0;
		disp_totalpoint = file.point;
		disp_point = point;
		while (1)
		{
			if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
				brkflag = true;

			if (brkflag)
				break;

			if (chudanflag)
				break;

			NetMatch.update();

			ClearDrawScreen();

			DrawBackGround(SCREEN_X, SCREEN_Y);

			if (disp_point <= 0.0)
			{
				if (Stick(KEY_INPUT_RETURN))
					break;
			}
			else
			{
				//早送り
				if (Stick(KEY_INPUT_RETURN))
				{
					disp_totalpoint += disp_point;
					disp_point = 0.0;
					framecnt = 60;
				}
			}

			if (framecnt >= 60)
			{
				if (disp_point > 0.0)
				{
					disp_point -= 2.0;
					disp_totalpoint += 2.0;
				}
				if (disp_point < 0.0)
				{
					disp_point += 1.0;
					disp_totalpoint -= 1.0;
				}
			}

			if (framecnt >= 0)
			{
				DispStringOfBox(20, SCREEN_Y / 4 * 0 + 50, 1.0, -1, "RESULT IN THE %s", stagename);

				if (keymode)
					DrawGraph(SCREEN_X - 60, 10, Handle_Challanging_Keymode, false);
			}
			if (framecnt >= 60)
			{
				DispStringOfBox(50, SCREEN_Y / 4 * 1 + 50, 1.0, 6, "STAGE: %5.0f POINT", disp_point);
				DispStringOfBox(50, SCREEN_Y / 4 * 2 + 50, 1.0, 7, "TOTAL: %5.0f POINT", disp_totalpoint);
			}
			if (disp_point <= 0.0)
			{
				if (framecnt % 60 < 30)
				{
					DispStringOfBox(SCREEN_X / 2 - 350, SCREEN_Y / 4 * 3 + 50, 1.0, -1, "PRESS ENTER KEY...");
				}
			}

			framecnt++;

			ScreenFlip();
		}
	}

	disp_navi_set("");

	DeleteMazeArray(map.data, map.x);
	DeleteMazeArray(map.visited, map.x);
	DeleteMazeArray(map.ans, map.x);
	DeleteLightHandle(L1);

	Map2Dim.Dest();

	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawBright(255, 255, 255);

	//受信を空にする
	while (NetMatch.update() > 0);

	if (chudanflag)
		return -2;
	else if (brkflag)
		return -1;
	else
		return point;
}

//自機とカメラの位置を初期化する。
void MazeGame::PosSet_def(void)
{
	JIKI _Jiki = { Masu_1 / 2.0, 0, Masu_1*2.0 + Masu_1 / 2.0, 0.0, 4.0, true };
	CAMERA _Camera = { 0.0, 50.0, 0.0, 270 + 360 * 10, 80 };

	Jiki = _Jiki;
	Camera = _Camera;

	Camera.Pos.x = sin(RAD1*Camera.Kakudo) * Camera.Hankei + Jiki.Pos.x;
	Camera.Pos.z = cos(RAD1*Camera.Kakudo) * Camera.Hankei + Jiki.Pos.z;
}

//自機を動かし、それによってカメラも動かす。
void MazeGame::JikiandCamMove(int mode, bool keymode)
{
	//キーフラグ
	char KeyFlag[256];
	double Jikix_Temp, Jikiz_Temp;

	GetHitKeyStateAll(KeyFlag);

	switch (mode)
	{
	case 0:
	case 1:
		if (KeyFlag[KEY_INPUT_LEFT])	Camera.Kakudo -= 3.0;
		if (KeyFlag[KEY_INPUT_RIGHT])	Camera.Kakudo += 3.0;

		if (keymode == false)
		{
			if (KeyFlag[KEY_INPUT_UP])
			{
				Jikix_Temp = Jiki.Pos.x - sin(RAD1*Camera.Kakudo)*3.0;
				Jikiz_Temp = Jiki.Pos.z - cos(RAD1*Camera.Kakudo)*3.0;

				if ((Jikix_Temp >= 0.0 && Jikiz_Temp >= 0.0) && (Jikix_Temp < Masu_1*map.x && Jikiz_Temp < Masu_1*map.z))
				{
					if (map.data[int(Jikix_Temp / Masu_1)][int(Jikiz_Temp / Masu_1)] == ROAD)
					{
						Jiki.Pos.x -= sin(RAD1*Camera.Kakudo)*3.0;
						Jiki.Pos.z -= cos(RAD1*Camera.Kakudo)*3.0;
					}
				}
			}
			if (KeyFlag[KEY_INPUT_DOWN])
			{
				Jikix_Temp = Jiki.Pos.x + sin(RAD1*Camera.Kakudo)*3.0;
				Jikiz_Temp = Jiki.Pos.z + cos(RAD1*Camera.Kakudo)*3.0;

				if ((Jikix_Temp >= 0.0 && Jikiz_Temp >= 0.0) && (Jikix_Temp < Masu_1*map.x && Jikiz_Temp < Masu_1*map.z))
				{
					if (map.data[int(Jikix_Temp / Masu_1)][int(Jikiz_Temp / Masu_1)] == ROAD)
					{
						Jiki.Pos.x += sin(RAD1*Camera.Kakudo)*3.0;
						Jiki.Pos.z += cos(RAD1*Camera.Kakudo)*3.0;
					}
				}
			}
		}
		else
		{
			if (KeyFlag[KEY_INPUT_UP])
			{
				Jikix_Temp = Jiki.Pos.x - sin(RAD1*Camera.Kakudo)*3.0;
				Jikiz_Temp = Jiki.Pos.z - cos(RAD1*Camera.Kakudo)*3.0;

				if ((Jikix_Temp >= 0.0 && Jikiz_Temp >= 0.0) && (Jikix_Temp < Masu_1*map.x && Jikiz_Temp < Masu_1*map.z))
				{
					if (map.data[int(Jikix_Temp / Masu_1)][int(Jikiz_Temp / Masu_1)] == ROAD && ((int(Jikix_Temp / Masu_1) != map.x - 1 || int(Jikiz_Temp / Masu_1) != map.z - 3) || item.key.isgotten == true))
					{
						Jiki.Pos.x -= sin(RAD1*Camera.Kakudo)*3.0;
						Jiki.Pos.z -= cos(RAD1*Camera.Kakudo)*3.0;
					}
				}
			}
			if (KeyFlag[KEY_INPUT_DOWN])
			{
				Jikix_Temp = Jiki.Pos.x + sin(RAD1*Camera.Kakudo)*3.0;
				Jikiz_Temp = Jiki.Pos.z + cos(RAD1*Camera.Kakudo)*3.0;

				if ((Jikix_Temp >= 0.0 && Jikiz_Temp >= 0.0) && (Jikix_Temp < Masu_1*map.x && Jikiz_Temp < Masu_1*map.z))
				{
					if (map.data[int(Jikix_Temp / Masu_1)][int(Jikiz_Temp / Masu_1)] == ROAD && ((int(Jikix_Temp / Masu_1) != map.x - 1 || int(Jikiz_Temp / Masu_1) != map.z - 3) || item.key.isgotten == true))
					{
						Jiki.Pos.x += sin(RAD1*Camera.Kakudo)*3.0;
						Jiki.Pos.z += cos(RAD1*Camera.Kakudo)*3.0;
					}
				}
			}
		}

		//if (Camera.Pos.y > 50.0 && KeyFlag[KEY_INPUT_W]) Camera.Pos.y--;
		//if (Camera.Pos.y < 70.0 && KeyFlag[KEY_INPUT_S]) Camera.Pos.y++;

		Camera.Pos.x = sin(RAD1*Camera.Kakudo) * Camera.Hankei + Jiki.Pos.x;
		Camera.Pos.z = cos(RAD1*Camera.Kakudo) * Camera.Hankei + Jiki.Pos.z;

		if (int(Jiki.Pos.x / Masu_1) == item.map.x && int(Jiki.Pos.z / Masu_1) == item.map.z && item.map.isgotten == false)
		{
			if (keymode)	disp_navi_set("Play_ForKey");
			else			disp_navi_set("Play_ForGoal_UnKey");
			MapSolveFlag = true;
			item.map.isgotten = true;
			DispMode = 1;
		}

		if (int(Jiki.Pos.x / Masu_1) == item.ans.x && int(Jiki.Pos.z / Masu_1) == item.ans.z && item.ans.isgotten == false && file.boughtansitem == true)
		{
			disp_navi_set("Play_GetAnswer");
			dispsolveflag = true;
			item.ans.isgotten = true;
		}

		if (int(Jiki.Pos.x / Masu_1) == item.key.x && int(Jiki.Pos.z / Masu_1) == item.key.z && item.key.isgotten == false && keymode == true)
		{
			disp_navi_set("Play_ForGoal_Key");
			MapSolveFlag = true;
			item.key.isgotten = true;
		}

		map.visited[int(Jiki.Pos.x / Masu_1)][int(Jiki.Pos.z / Masu_1)] = 1;

		break;
	}
}

//表示全般 表示はここ経由でする。
void MazeGame::disp(int mode, bool keymode)
{
	switch (mode)
	{
	case 0: disp_3d(keymode); break;
	case 1: disp_3d(keymode); disp_2dmap(false, keymode); break;
	case 2: disp_2dmap(true, keymode); break;
	}
}

void MazeGame::disp_online(int mode, bool keymode)
{
	switch (mode)
	{
	case 0: disp_3d_online(keymode); break;
	case 1: disp_3d_online(keymode); disp_2dmap(false, keymode); break;
	case 2: disp_2dmap(true, keymode); break;
	}

	if (Online_ChatFlag && mode != 2)
		disp_chat();
	else
		DrawGraph(0, SCREEN_Y - 70, Handle_Chat_navigate, false);
}

int MazeGame::inputchat(void)
{
	//int ret;
	static int InputStatus;
	static int chatmode = 0;	//0:初期化、1:入力しない、2:入力中、3:送信
	static char sendchatvalue[32] = "";

	switch (chatmode)
	{
	case 0:
	{
		InputHandle = MakeKeyInput(255, true, false, false);
		chatmode = 1;
		strcpy_s(sendchatvalue, 32, "");
		break;
	}
	case 1:
	{
		if (Stick(KEY_INPUT_SPACE) && Online_ChatFlag)
		{
			SetActiveKeyInput(InputHandle);
			ChatInputFlag = true;
			chatmode = 2;
		}
		break;
	}
	case 2:
	{
		//communicate()

		//disp();

		if ((InputStatus = CheckKeyInput(InputHandle)), InputStatus == 1 || InputStatus == 2 || InputStatus == -1)
		{
			if (InputStatus == 1)
			{
				GetKeyInputString(sendchatvalue, InputHandle);
				if (!(strcmp(sendchatvalue, "") == 0))
				{
					chatmode = 3;
					ChatInputFlag = false;
					Stick(KEY_INPUT_ESCAPE);
					Stick(KEY_INPUT_RETURN);
				}
				else
				{
					chatmode = 4;
					ChatInputFlag = false;
					Stick(KEY_INPUT_ESCAPE);
					Stick(KEY_INPUT_RETURN);
				}
			}
			else
			{
				chatmode = 4;
				ChatInputFlag = false;
				Stick(KEY_INPUT_ESCAPE);
				Stick(KEY_INPUT_RETURN);
			}
		}

		break;
	}
	case 3:
	{
		NetMatch.send_chat(sendchatvalue);
		chatmode = 4;
		break;
	}
	case 4:
	{
		DeleteKeyInput(InputHandle);
		chatmode = 0;
		break;
	}
	}

	return 0;
}

//3Dの迷路を表示する。
void MazeGame::disp_3d(bool keymode)
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

	//鍵モードの時に、ゴール前の鍵付きドアを表示する。
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

	//アイテムと自機を表示する。
	disp_item(keymode);
	disp_jiki();
}

void MazeGame::disp_3d_online(bool keymode)
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

	//鍵モードの時に、ゴール前の鍵付きドアを表示する。
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

	//アイテムと自機を表示する。
	disp_item(keymode);
	disp_jiki();
	disp_jiki_others();
}

//２次元のマップを表示する。
void MazeGame::disp_2dmap(bool FullScreenFlag, bool keymode)
{
	static bool passed = false;
	int sx, sy;

	//マップの大きさを設定
	if (FullScreenFlag) sx = SCREEN_X, sy = SCREEN_Y;
	else				sx = SCREEN_X / 3, sy = SCREEN_Y / 3;

	Map2Dim.SetScreenSize(sx, sy);

	//フルスクリーンかそうでないかで、マップ用裏画面を使い分ける。
	if (FullScreenFlag)
		SetDrawScreen(Handle_Screen_FullTrue);
	else
		SetDrawScreen(Handle_Screen_FullFalse);

	//マップ用裏画面を初期化、消去。
	ClearDrawScreen();

	//動く背景を表示
	DrawBackGround(sx, sy);

	//マップを表示
	Map2Dim.disp(Jiki.Pos, Camera.Kakudo + 180.0, true, dispsolveflag, true, keymode, file.boughtansitem);

	//通常の裏画面に切り替える。
	SetDrawScreen(Handle_Screen_Normal);

	if (FullScreenFlag)
	{
		DrawExtendGraph(0, 0, SCREEN_X, SCREEN_Y, Handle_Screen_FullTrue, false);
	}
	else
	{
		DrawExtendGraph(SCREEN_X / 3 * 2, SCREEN_Y / 3 * 2, SCREEN_X, SCREEN_Y, Handle_Screen_FullFalse, false);
	}

	passed = true;
}

//自機を表示する。
void MazeGame::disp_jiki(void)
{
	static bool passed = false;
	static VECTOR before;
	VECTOR Position;

	if (passed == false)
		before = Jiki.Pos;

	//自機が動いている時、足を動かす。
	if (before.x != Jiki.Pos.x || before.z != Jiki.Pos.z)
	{
		Jiki.stopf = false;

		Jiki.rot += Jiki.rotd;
		if (Jiki.rot >= 60.0) Jiki.rotd = -4.0;
		if (Jiki.rot <= -60.0) Jiki.rotd = 4.0;
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
		MV1SetRotationXYZ(Handle_Jiki_Moving_Foot, VGet(DegtoRad(Jiki.rot), DegtoRad(Camera.Kakudo - 180.0), 0.0));
		MV1DrawModel(Handle_Jiki_Moving_Foot);

		Position = VGet(
			Jiki.Pos.x + Degsin(Camera.Kakudo - 90) * 6,
			Jiki.Pos.y + 20,
			Jiki.Pos.z + Degcos(Camera.Kakudo - 90) * 6
			);
		MV1SetPosition(Handle_Jiki_Moving_Arm, Position);
		MV1SetScale(Handle_Jiki_Moving_Arm, VGet(1.0, 1.0, 1.0));
		MV1SetRotationXYZ(Handle_Jiki_Moving_Arm, VGet(DegtoRad(Jiki.rot), DegtoRad(Camera.Kakudo - 180.0), 0.0));
		MV1DrawModel(Handle_Jiki_Moving_Arm);

		Position = VGet(
			Jiki.Pos.x + Degsin(Camera.Kakudo - 90) * 4,
			Jiki.Pos.y + 12,
			Jiki.Pos.z + Degcos(Camera.Kakudo - 90) * 4
			);
		MV1SetPosition(Handle_Jiki_Moving_Foot, Position);
		MV1SetScale(Handle_Jiki_Moving_Foot, VGet(1.0, 1.0, 1.0));
		MV1SetRotationXYZ(Handle_Jiki_Moving_Foot, VGet(DegtoRad(-Jiki.rot), DegtoRad(Camera.Kakudo - 180.0), 0.0));
		MV1DrawModel(Handle_Jiki_Moving_Foot);

		Position = VGet(
			Jiki.Pos.x + Degsin(Camera.Kakudo + 90) * 6,
			Jiki.Pos.y + 20,
			Jiki.Pos.z + Degcos(Camera.Kakudo + 90) * 6
			);
		MV1SetPosition(Handle_Jiki_Moving_Arm, Position);
		MV1SetScale(Handle_Jiki_Moving_Arm, VGet(1.0, 1.0, 1.0));
		MV1SetRotationXYZ(Handle_Jiki_Moving_Arm, VGet(DegtoRad(-Jiki.rot), DegtoRad(Camera.Kakudo - 180.0), 0.0));
		MV1DrawModel(Handle_Jiki_Moving_Arm);
	}
	//自機が止まっているときは、足を動かさない。
	else
	{
		Jiki.stopf = true;

		Jiki.rot = 0.0;
		Jiki.rotd = 4.0;

		MV1SetPosition(Handle_Jiki_Stopping, Jiki.Pos);
		MV1SetScale(Handle_Jiki_Stopping, VGet(1.0, 1.0, 1.0));
		MV1SetRotationXYZ(Handle_Jiki_Stopping, VGet(0.0, RAD1*(Camera.Kakudo - 180.0) + DegtoRad(-90.0), 0.0));
		MV1DrawModel(Handle_Jiki_Stopping);
	}

	passed = true;
}

//自分以外の自機を表示する。
void MazeGame::disp_jiki_others(void)
{
	//static bool initflag = true;
	//static JIKI *before;
	//static int before_n = 0;
	//static int before_member_n = mret.member_n;
	VECTOR Position;
	int i;

	////参加メンバー人数が変わった場合再度初期化する
	//if (before_member_n != mret.member_n)
	//{
	//	before_member_n = mret.member_n;
	//	DelArray1Dim(before);
	//	initflag = true;
	//}

	////呼び出し最初の時と前述のことがあったら初期化
	//if (initflag == true)
	//{
	//	GetArray1Dim(JIKI, before, mret.member_n);
	//	for (i = 0; i < mret.member_n; i++)
	//		before[i] = mret.mdb[i].Jiki;
	//	initflag = false;
	//	//before = Jiki.Pos;
	//}

	for (i = 0; i < mret.member_n; i++)
	{
		if (i != mret.yourno && mret.member[i].ischallange == 1)
		{
			//自機が動いている時、足を動かす。
			//if (before[i].Pos.x != mret.mdb[i].Jiki.Pos.x || before[i].Pos.z != mret.mdb[i].Jiki.Pos.z)
			if (mret.mdb[i].Jiki.stopf == false)
			{
				//Jiki.rot += Jiki.rotd;
				//if (Jiki.rot >= 60.0) Jiki.rotd = -4.0;
				//if (Jiki.rot <= -60.0) Jiki.rotd = 4.0;
				//before[i] = mret.mdb[i].Jiki;

				Position = mret.mdb[i].Jiki.Pos;
				MV1SetPosition(Handle_Jiki_Moving_Body, Position);
				MV1SetScale(Handle_Jiki_Moving_Body, VGet(1.0, 1.0, 1.0));
				MV1SetRotationXYZ(Handle_Jiki_Moving_Body, VGet(0.0, DegtoRad(mret.mdb[i].CamKakudo - 180.0) + DegtoRad(-90.0), 0.0));
				MV1DrawModel(Handle_Jiki_Moving_Body);

				Position = VGet(
					mret.mdb[i].Jiki.Pos.x + Degsin(mret.mdb[i].CamKakudo + 90) * 4,
					mret.mdb[i].Jiki.Pos.y + 12,
					mret.mdb[i].Jiki.Pos.z + Degcos(mret.mdb[i].CamKakudo + 90) * 4
					);
				MV1SetPosition(Handle_Jiki_Moving_Foot, Position);
				MV1SetScale(Handle_Jiki_Moving_Foot, VGet(1.0, 1.0, 1.0));
				MV1SetRotationXYZ(Handle_Jiki_Moving_Foot, VGet(DegtoRad(mret.mdb[i].Jiki.rot), DegtoRad(mret.mdb[i].CamKakudo - 180.0), 0.0));
				MV1DrawModel(Handle_Jiki_Moving_Foot);

				Position = VGet(
					mret.mdb[i].Jiki.Pos.x + Degsin(mret.mdb[i].CamKakudo - 90) * 6,
					mret.mdb[i].Jiki.Pos.y + 20,
					mret.mdb[i].Jiki.Pos.z + Degcos(mret.mdb[i].CamKakudo - 90) * 6
					);
				MV1SetPosition(Handle_Jiki_Moving_Arm, Position);
				MV1SetScale(Handle_Jiki_Moving_Arm, VGet(1.0, 1.0, 1.0));
				MV1SetRotationXYZ(Handle_Jiki_Moving_Arm, VGet(DegtoRad(mret.mdb[i].Jiki.rot), DegtoRad(mret.mdb[i].CamKakudo - 180.0), 0.0));
				MV1DrawModel(Handle_Jiki_Moving_Arm);

				Position = VGet(
					mret.mdb[i].Jiki.Pos.x + Degsin(mret.mdb[i].CamKakudo - 90) * 4,
					mret.mdb[i].Jiki.Pos.y + 12,
					mret.mdb[i].Jiki.Pos.z + Degcos(mret.mdb[i].CamKakudo - 90) * 4
					);
				MV1SetPosition(Handle_Jiki_Moving_Foot, Position);
				MV1SetScale(Handle_Jiki_Moving_Foot, VGet(1.0, 1.0, 1.0));
				MV1SetRotationXYZ(Handle_Jiki_Moving_Foot, VGet(DegtoRad(-mret.mdb[i].Jiki.rot), DegtoRad(mret.mdb[i].CamKakudo - 180.0), 0.0));
				MV1DrawModel(Handle_Jiki_Moving_Foot);

				Position = VGet(
					mret.mdb[i].Jiki.Pos.x + Degsin(mret.mdb[i].CamKakudo + 90) * 6,
					mret.mdb[i].Jiki.Pos.y + 20,
					mret.mdb[i].Jiki.Pos.z + Degcos(mret.mdb[i].CamKakudo + 90) * 6
					);
				MV1SetPosition(Handle_Jiki_Moving_Arm, Position);
				MV1SetScale(Handle_Jiki_Moving_Arm, VGet(1.0, 1.0, 1.0));
				MV1SetRotationXYZ(Handle_Jiki_Moving_Arm, VGet(DegtoRad(-mret.mdb[i].Jiki.rot), DegtoRad(mret.mdb[i].CamKakudo - 180.0), 0.0));
				MV1DrawModel(Handle_Jiki_Moving_Arm);
			}
			//自機が止まっているときは、足を動かさない。
			else
			{
				//Jiki.rot = 0.0;
				//Jiki.rotd = 4.0;

				MV1SetPosition(Handle_Jiki_Stopping, mret.mdb[i].Jiki.Pos);
				MV1SetScale(Handle_Jiki_Stopping, VGet(1.0, 1.0, 1.0));
				MV1SetRotationXYZ(Handle_Jiki_Stopping, VGet(0.0, RAD1*(mret.mdb[i].CamKakudo - 180.0) + DegtoRad(-90.0), 0.0));
				MV1DrawModel(Handle_Jiki_Stopping);
			}
		}
	}
}

//アイテムを表示する。
//回って上がったり下がったりしながら表示している。
void MazeGame::disp_item(bool keymode)
{
	static double rot = 0;
	static double height = 0;
	static double height_d = 30.0 / 45.0;

	//マップアイテム
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

	//アンサーアイテム
	if (item.ans.isgotten == false)
	{
		if (file.boughtansitem == true)
		{
			if (item.ans.x - 10 <= int(Jiki.Pos.x / Masu_1) && item.ans.x + 10 > int(Jiki.Pos.x / Masu_1) && item.ans.z - 10 <= int(Jiki.Pos.z / Masu_1) && item.ans.z + 10 > int(Jiki.Pos.z / Masu_1))
			{
				MV1SetPosition(Handle_Item_Ans, VGet(item.ans.x * Masu_1 + Masu_1 / 2, height, item.ans.z * Masu_1 + Masu_1 / 2));
				MV1SetScale(Handle_Item_Ans, VGet(1.0, 1.0, 1.0));
				MV1SetRotationXYZ(Handle_Item_Ans, VGet(0.0, DegtoRad(rot), 0.0));
				MV1DrawModel(Handle_Item_Ans);
			}
		}
	}

	//鍵
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

	//上がったり下がったりさせる
	height += height_d;
	if (height >= 30) height_d = -30.0 / 45.0;
	if (height <= 0) height_d = 30.0 / 45.0;

	//回らせる
	rot += 360.0 / 90.0;
	rot = Amarif(rot, 360.0);
}


void MazeGame::disp_chat(void)
{
	SPLITDATA spldata;
	int spl_n;
	int i;
	static int pos_y_min = 0;
	static int before_pos_y_min;
	static int pos_y = pos_y_min;
	int rot;
	static bool newchatflag = false;

	int mousex, mousey;

	before_pos_y_min = pos_y_min;

	spl_n = split(mret.Chat.content, "\n", &spldata);

	//画面に収まる行以上のチャットがあったら左上座標を下げる
	if (spl_n * 10 + 10 >= CHAT_SY - 10)
	{
		pos_y_min = -(spl_n * 10 + 10 - (CHAT_SY - 10));
	}
	else pos_y_min = 0;

	{
		//回転度とマウス座標を取得
		GetMousePoint(&mousex, &mousey);

		if (
			mousex >= 0 &&
			mousey >= SCREEN_Y / 3 * 2 &&
			mousex < SCREEN_X / 3 &&
			mousey < SCREEN_Y)
		{
			rot = GetMouseWheelRotVol();
			pos_y += rot * 30;
			if (pos_y < pos_y_min)
			{
				pos_y = pos_y_min;
			}
			else if (pos_y > 0)
			{
				pos_y = 0;
			}
			if (pos_y == pos_y_min)
			{
				newchatflag = false;
			}
		}
	}


	//新しいメッセージが来たら
	if (before_pos_y_min != pos_y_min)
	{
		if (pos_y == before_pos_y_min || ChatInitFlag)
		{
			pos_y = pos_y_min;
			ChatInitFlag = false;
		}
		else
		{
			newchatflag = true;
		}
	}

	//表示部
	//チャット用画面に描画させる
	SetDrawScreen(Handle_Screen_Chat);

	//ClearDrawScreen();

	DrawBox(0, 0, CHAT_SX, CHAT_SY, GetColor(50, 50, 50), true);

	//チャット用フォントサイズにする
	SetFontSize(10);

	for (i = 0; i < spl_n; i++)
	{
		if (strstr(spldata[i], " : ") == NULL && strstr(spldata[i], "参加") != NULL)
			DrawFormatString(10, pos_y + 10 + 10 * i, STR_JOIN, "%s", spldata[i]);
		else if (strstr(spldata[i], " : ") == NULL && strstr(spldata[i], "退出") != NULL)
			DrawFormatString(10, pos_y + 10 + 10 * i, STR_DIS, "%s", spldata[i]);
		else if (strstr(spldata[i], "さんはタイム") != NULL && strstr(spldata[i], "位になりました！") != NULL)
			DrawFormatString(10, pos_y + 10 + 10 * i, STR_RANK, "%s", spldata[i]);
		else
			DrawFormatString(10, pos_y + 10 + 10 * i, STR_COLOR, "%s", spldata[i]);
	}

	DrawBox(0, CHAT_SY - 10, CHAT_SX, CHAT_SY, GetColor(50, 50, 50), TRUE);

	if (ChatInputFlag == false)
		DrawFormatString(10, CHAT_SY - 10, STR_COLOR, "マウスでスクロール、Spaceで入力");
	else
	{
		SetKeyInputStringColor(STR_COLOR, STR_COLOR, STR_COLOR, STR_COLOR, STR_COLOR, STR_COLOR, STR_COLOR_2);
		DrawFormatString(10, CHAT_SY - 10, STR_COLOR, "Enter送信、Esc解除 %s > ", file.username);
		DrawKeyInputString(10 + 22 * 6 + strlen(file.username) * 6, CHAT_SY - 10, InputHandle);
		DrawKeyInputModeString(CHAT_SX - 6 * 12, 10);
	}

	if (newchatflag == true)
	{
		DrawBox(CHAT_SX - 50, 0, CHAT_SX, 10, GetColor(50, 50, 50), TRUE);
		DrawFormatString(CHAT_SX - 60, 0, STR_COLOR, "新しいログ");
	}

	//通常のフォントサイズに戻す
	SetFontSize(20);

	//通常の裏の裏画面に切り替える
	SetDrawScreen(Handle_Screen_Normal);

	DrawExtendGraph(0, SCREEN_Y / 3 * 2, SCREEN_X / 3, SCREEN_Y, Handle_Screen_Chat, false);

	delsplit(spl_n, spldata);
}

#endif