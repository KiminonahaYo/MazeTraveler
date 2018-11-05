/*初期設定、定義をするヘッダ*/

#ifndef DEFINED_H_DEFINED

#include "DxLib.h"
#include "UsefulHeaders/useful.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#define Masu_1			100							//迷路のマス

#define SCREEN_X		1024						//ウィンドウXサイズ
#define SCREEN_Y		768							//ウィンドウYサイズ

#define STR_COLOR		0xFFFFFF					//　暗い画面での文字列色
#define STR_COLOR_2		0x000000					//明るい画面での文字列色
#define BACK_COLOR		0x000000

#define Short_Air_X		40
#define Short_Air_Z		50

#define SEED_NUL		65536						//シード値が設定されていないという数値

#define SAVE_DIR		"Saves"						//ゲームデータセーブフォルダ
#define KAKUCYOSI		".gamesav"					//ゲームデータ拡張子

#define STATUSMES_COL	8							//ステータスメッセージ（一番下）の文字列色
#define USERMES_COL		7							//ユーザー情報（下から２番め）の文字列色

#define MES_TITLEBAR	"Maze Traveler Ver.5"		//ウィンドウタイトル

#define PROGRAM_FIN		-10							//プログラムの終了を表す戻り値

//ゲームメニューでのメニュー項目の色
//なお、メニュー項目は画像である。
#define S_GRAY			0							//灰色
#define S_RED			1							//赤色
#define S_YELLOW		2							//黄色
#define S_GREEN			3							//緑色
#define S_BLUE			4							//青
#define S_PURPLE		5							//紫

//チャット窓表示文字列色
#define STR_JOIN 0x9999ff
#define STR_DIS  0xff9999
#define STR_RANK 0xffff99
#define CHAT_SX  (SCREEN_X / 3)
#define CHAT_SY  (SCREEN_Y / 3)

//コンパイル時の警告表示抑制
#pragma warning( disable:4305 4244 4305 )

//特定の目的で使うために型の名前を変更
typedef int MEDIA_HANDLER;							//ハンドラーで使うint型
typedef int** MAPDATA;								//迷路のマップデータ（動的確保２次元配列）

//構造体

//自機
typedef struct
{
	VECTOR Pos;
	double rot;
	double rotd;
	bool stopf;
}
JIKI;

//自機を映すカメラ
typedef struct
{
	VECTOR Pos;
	double Kakudo,Hankei;
}
CAMERA;

//迷路マップ（マップデータ、迷路の正解の通り道、迷路の辿った経路）
typedef struct
{
	MAPDATA data;		//マップデータ
	MAPDATA ans;		//迷路の答え
	MAPDATA visited;	//迷路の辿った経路

	int x, y, z;		//マップのサイズX, Y, Z（マス単位）
}MAP;

//アイテム１つの構造体
typedef struct
{
	int x, z;			//アイテム座標
	bool isgotten;		//アイテム入手したか（入手したら描画しない）
}
ITEM;

//アイテムを３つ合わせた構造体
typedef struct
{
	ITEM map;			//マップアイテム
	ITEM ans;			//アンサーアイテム
	ITEM key;			//鍵モードで入手する鍵
}
MAPITEM;

//ステージ記録データ
typedef struct
{
	unsigned int seed[3];	//シード値（記憶しておけば、次にマップを開いても同じ迷路になる）
	char ischallange[3];	//迷路に挑戦したか（０：未挑戦、１：不合格、２：合格）
	int recode[3];			//迷路に挑戦した時の得点（１００点未満不合格）
}
STAGE;

//ゲームファイル構造体
typedef struct
{
	STAGE *normalstage;		//通常ステージデータ
	STAGE *keystage;		//鍵モードステージデータ
	bool boughtansitem;		//アンサーアイテム購入したか
	int lev_n;				//今到達している最高レベル
	int point;				//迷路をプレイしたことによる累計ポイント（ゲーム内の仮想通貨）
	char username[16];		//迷路ユーザー名
	int online_visited;		//オンラインプレイをした回数
	int online_maxpoint;	//オンラインプレイでの最高得点
}
GAMEFILE;

typedef struct
{
	char *content;
	int len;
}
MYLOG;

//-------------------------------------------------------------------------------------------
//ログファイルセーブフラグ
//これをtrueにすればログファイルを保存します。
//-------------------------------------------------------------------------------------------

bool IsSaveLog = false;

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

//ハンドラ
MEDIA_HANDLER L1;												//追加したライトのハンドル
MEDIA_HANDLER Handle_Jiki;										//自機
MEDIA_HANDLER Handle_Wall;										//壁
MEDIA_HANDLER Handle_Floor;										//床
MEDIA_HANDLER Handle_Start;										//スタート
MEDIA_HANDLER Handle_Goal;										//ゴール
MEDIA_HANDLER Handle_Start_ForAir;
MEDIA_HANDLER Handle_Goal_ForAir;
MEDIA_HANDLER Handle_VisitedArea_ForAir;
MEDIA_HANDLER Handle_Jiki_PosNavi_ForAir;
MEDIA_HANDLER Handle_LightModel;
MEDIA_HANDLER Handle_GoalDoor;									//鍵モードでのゴールにある鍵付きドア

MEDIA_HANDLER Handle_Title;										//タイトル画面でのタイトル名画像
MEDIA_HANDLER Handle_PressEnterKey;								//エンターキーを促すための画像

//メニュー画面でのメニュー項目画像
MEDIA_HANDLER Handle_SelectTile_Blue;							//青
MEDIA_HANDLER Handle_SelectTile_Red;							//赤
MEDIA_HANDLER Handle_SelectTile_Green;							//緑
MEDIA_HANDLER Handle_SelectTile_Purple;							//紫
MEDIA_HANDLER Handle_SelectTile_Yellow;							//黄色
MEDIA_HANDLER Handle_SelectTile_Gray;							//灰色

MEDIA_HANDLER Handle_Dotpic[10];								//ドット文字列の■ドット
MEDIA_HANDLER Handle_2dimmap[80];								//２次元マップ
MEDIA_HANDLER Handle_BackGround[2];								//動く背景画像１
MEDIA_HANDLER Handle_BackGround_2[2];							//動く背景画像２
MEDIA_HANDLER Handle_BackGround_3[2];							//動く背景画像３

MEDIA_HANDLER Handle_TimeDisp_Tile;								//迷路プレイ中の時間表示板
MEDIA_HANDLER Handle_Delete_Warning;							//プレイデータを削除するときの確認メッセージ
MEDIA_HANDLER Handle_Delete_Yes;								//「プレイデータを　削除する」項目
MEDIA_HANDLER Handle_Delete_No;									//「プレイデータを削除しない」項目

MEDIA_HANDLER Handle_SolveRoute;								//迷路の正解経路を示すドット

MEDIA_HANDLER Handle_Goaled;									//ゴールした時に表示する画像

MEDIA_HANDLER Handle_Screen_FullFalse;							//ウィンドウすべてを占めないマップ表示のための裏の裏の裏スクリーン
MEDIA_HANDLER Handle_Screen_FullTrue;							//ウィンドウすべてを　占めるマップ表示のための裏の裏の裏スクリーン
MEDIA_HANDLER Handle_Screen_Normal;								//フェードをするためにこの裏の裏スクリーンに描画してから裏スクリーンに描画
MEDIA_HANDLER Handle_Title_DispAssist;							//タイトル画面、デモ画面にドット文字列を表示するときに見やすいようにする背景

MEDIA_HANDLER Handle_Game_Countdown;

MEDIA_HANDLER Handle_Result_Tile;								//迷路のリザルト表示画面での時間表示
MEDIA_HANDLER Handle_Result_Passed;								//合格
MEDIA_HANDLER Handle_Result_VeryGood;							//優良
MEDIA_HANDLER Handle_Result_Failed;								//不合格

MEDIA_HANDLER Handle_Jiki_Stopping;								//自機が止まっている時の全身モデル
MEDIA_HANDLER Handle_Jiki_Moving_Body;							//自機が動いている時の体
MEDIA_HANDLER Handle_Jiki_Moving_Foot;							//自機が動いている時の足
MEDIA_HANDLER Handle_Jiki_Moving_Arm;							//自機が動いている時の腕

MEDIA_HANDLER Handle_Item_Map;									//マップアイテム（青）
MEDIA_HANDLER Handle_Item_Ans;									//アンサーアイテム（赤）
MEDIA_HANDLER Handle_Item_Key;									//鍵モードでの鍵（黄色）

MEDIA_HANDLER Handle_Gotitem_Tile;								//アイテム獲得表示板
MEDIA_HANDLER Handle_Gotitem_Map;								//マップアイテムを獲得した
MEDIA_HANDLER Handle_Gotitem_Ans;								//アンサーアイテムを獲得した
MEDIA_HANDLER Handle_Gotitem_Key;								//鍵アイテムを獲得した

MEDIA_HANDLER Handle_Challanging_Keymode;						//鍵モードに挑戦するときに表示する

MEDIA_HANDLER Handle_Level3_Unlock;								//アンロック（レベル３挑戦可能時）
MEDIA_HANDLER Handle_Level5_Unlock;								//アンロック（レベル３挑戦可能時）
MEDIA_HANDLER Handle_Unlock_KeymodeInStage_UntilLevel2;			//アンロック（優良点に達した時に鍵モード上でステージ開放、鍵モード解放前）
MEDIA_HANDLER Handle_Unlock_KeymodeInStage;						//アンロック（優良点に達した時に鍵モード上でステージ開放、鍵モード解放後）
MEDIA_HANDLER Handle_Unlock_Level;								//アンロック（次のレベル解放）

MEDIA_HANDLER Handle_Help_Navi;									//ヘルプのボタンを教える画像
MEDIA_HANDLER Handle_Help_Back;									//　　　　ヘルプ表示背景

MEDIA_HANDLER Handle_Bought_AnsItem;							//アンサーアイテムを購入した時に表示する

MEDIA_HANDLER Handle_Navigate_Back;								//ナビゲーション表示背景

MEDIA_HANDLER Handle_Screen_Chat;								//チャット用裏スクリーン

MEDIA_HANDLER Handle_NetWork_Connect_Error;

MEDIA_HANDLER Handle_Chat_navigate;								//チャットを促すメッセージ

MEDIA_HANDLER Handle_Online_Mine;

//MEDIA_HANDLER Handle_Map_Back[8];

//ヘルプ文
char help_text[32768];
SPLITDATA help_text_spl;
int help_text_spl_n;

//ナビゲーション文
char navi_text[32768];
SPLITDATA navi_text_spl;
int navi_text_spl_n;

//IPアドレス
IPDATA Ip = { 127, 0, 0, 1 };
int port = 9850;				//0～65535の範囲

MYLOG MyLog;

//-------------------------------------------------------------------------------------------

int seedset(int);									//シード値設定
void decidemazesize_bylevel(int, int*, int*);		//レベルを指定して迷路サイズを返す関数
void MakeNewGameFile(char *);						//新しいゲームファイルを作成
void DispSelect(int, bool, int, int, char *, ...);	//メニュー項目表示
int GetFPS(void);									//FPS取得

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

//FPS取得
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

//シード値設定
int seedset(int seed)
{
	if (seed < 0)
	{
		return rand();
	}
	else
		return seed;
}

//レベルを指定して迷路サイズを返す関数
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

//新規にファイルを作成
void MakeNewGameFile(char *filename)
{
	FILE *fp;
	char buf[256];
	int i, j;
	GAMEFILE file;

	sprintf_s(buf, 256, "./%s/%s%s", SAVE_DIR, filename, KAKUCYOSI);

	//引数で指定したファイルが存在していなかったら
	//新規にファイルを作成
	if (exist("%s", buf) == false)
	{
		//strcpy_s(file.username, 16, filename);

		//ファイル初期化

		file.lev_n = 0;					//レベル最大値
		file.point = 0;					//累計ポイント
		file.online_visited = 0;		//オンラインプレイをした回数
		file.online_maxpoint = 0;		//オンラインプレイ最高得点
		file.boughtansitem = false;		//アンサーアイテム購入したか

		//ステージデータ動的配列確保
		file.normalstage = (STAGE *)malloc(sizeof(STAGE)*(file.lev_n + 1));
		file.keystage = (STAGE *)malloc(sizeof(STAGE)*(file.lev_n + 1));

		//ステージデータ
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

		//ファイルに書き込み
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

		//ステージデータ動的配列開放
		free(file.normalstage);
		free(file.keystage);
	}
}

//メニュー項目を表示
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
