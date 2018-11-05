/*
	Maze Travelerタイトル画面
*/
//ヘッダーファイル読み込み
#include "Defines.h"
#include "MyMaze.h"
#include "DrawMoji.h"
#include "Map2Dim.h"
#include "BackGround.h"

#define Short_Air_X 40
#define Short_Air_Z 50

//デモンストレーション表示クラス
class MazeTitle
{
private:
	//自機とカメラ変数
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

	//自機を動かし、カメラと連動させる。
	void JikiandCamMove(void);

	void PosSet_def(void);
}
Title;

//自機とカメラの位置を初期化する。
void MazeTitle::PosSet_def(void)
{
	JIKI _Jiki = { { Masu_1 / 2.0, 0, Masu_1*2.0 + Masu_1 / 2.0 } };
	CAMERA _Camera = { { 0.0, 50.0, 0.0 }, 270, 100 };

	Jiki = _Jiki;
	Camera = _Camera;

	Camera.Pos.x = sin(RAD1*Camera.Kakudo) * Camera.Hankei + Jiki.Pos.x;
	Camera.Pos.z = cos(RAD1*Camera.Kakudo) * Camera.Hankei + Jiki.Pos.z;
}

//タイトルとデモを交互に表示
int MazeTitle::Disp(void)
{
	//-------------------------------------------------------------------------------------------

	//変数宣言

	//汎用変数
	//float x, y, z;
	int i, j, a;
	//int Room_MasuZ, Room_MasuX, Room_MasuY = 1;

	int Jiki_MasuX, Jiki_MasuZ;
	int Jiki_MasuX_before, Jiki_MasuZ_before;

	//デモンストレーション関係
	int CountStartTime;
	bool demostartedflag = false;

	int seed;

	int FrameCnt = 0;
	bool MapFlag = false;
	bool keymode = false;

	int nowtime, nolmatime;
	char stagename[256] = "";

	int level;

	bool brkflag = false;			//×を押したりEscを押したりした時、プログラムを終了するか
	bool Endflag = false;			//Enterを押してゲームを開始するフラグ（ループ抜けてメニュー画面へ）

	VECTOR Center;					//２次元マップ中央
	double rot;						//２次元マップ角度

	//キーフラグ
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

		//自機とカメラの初期化
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

		//レベルとシード値を決定
		level = RangeRand(1,10);
		decidemazesize_bylevel(level, &map.x, &map.z);
		seed = rand();
		sprintf_s(stagename, 256, "%02d-%05d", level, seed);

		map.y = 1;

		keymode = RangeRand(0, 1) ? true : false;

		//迷路を作成
		GetMazeArray(&map.data, map.x, map.z);												//迷路用の配列を確保
		GetMazeArray(&map.visited, map.x, map.z);											//訪問済みの場所を格納する配列を作成
		GetMazeArray(&map.ans, map.x, map.z);												//訪問済みの場所を格納する配列を作成
		MakeMaze(map.data, seed, map.x, map.z);												//迷路を作成

		item.map.isgotten = false;
		item.ans.isgotten = false;
		item.key.isgotten = false;

		//マップアイテムの座標を決定
		do
		{
			//マップアイテム座標決定(x, z共に2～10の範囲)
			item.map.x = 2 + (rand() % 5) * 2;
			item.map.z = 2 + (rand() % 5) * 2;

			//マップアイテムまでのたどるマス数を算出
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
			//マップアイテムまで遠すぎたりアンサーアイテムと被った場合やり直し
		} while (a >= 20 || (item.map.x == 2 && item.map.z == 2));

		//アンサーアイテムはスタートの目の前
		item.ans.x = 2;
		item.ans.z = 2;

		//鍵の座標を決定
		do
		{
			item.key.x = 2 + (rand() % ((map.x + 1) / 2 - 2)) * 2;
			item.key.z = 2 + (rand() % ((map.z + 1) / 2 - 2)) * 2;
		} while (
			(item.key.x == item.map.x && item.key.z == item.map.z) || 
			(item.key.x == item.ans.x && item.key.z == item.ans.z) || 
			(item.key.x < map.x / 2 && item.key.z < map.z / 2));

		//２次元マップ初期設定
		Map2Dim.Init(map, 2.5, &item);
		Map2Dim.SetScreenSize(SCREEN_X, SCREEN_Y);

		//ライトハンドルの初期化
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

		//訪問済みの場所はないのですべて未訪問にする（初期化）
		for (i = 0; i < map.x; i++)
		{
			for (j = 0; j < map.z; j++)
			{
				map.visited[i][j] = 0;
			}
		}

		//目標時間を決定（ノルマ時間）
		//通常モード：スタート→マップ　→　ゴールの順に辿ったとして算出
		//　鍵モード：スタート→マップ→鍵→ゴールの順に辿ったとして算出
		//１マス２秒で正解ルートを辿ったら合格点です。
		{
			nolmatime = 0;
			a = 0;

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

		//タイトル画面

		//初期化
		FrameCnt = 0;
		Center.x = Masu_1 * map.x / 2;
		Center.y = 0.0;
		Center.z = Masu_1 * map.z / 2;
		rot = 0;
		//SolveMaze(map.data, map.ans, map.x, map.z, 0, 2, map.x - 1, map.z - 3);			//迷路の答えを作成

		//表示ループ
		while (1)
		{
			if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
				brkflag = true;

			if (brkflag)
				break;

			if (Stick(KEY_INPUT_RETURN))
				Endflag = true;

			if (Endflag == true) break;

			//１０秒経ったらデモンストレーション移行
			if (FrameCnt >= 60 * 10)
				break;

			//仮想画面に表示
			SetDrawScreen(Handle_Screen_Normal);

			ClearDrawScreen();

			//動く背景を表示
			DrawBackGround(SCREEN_X, SCREEN_Y);

			//迷路の2次元マップ表示
			Map2Dim.disp(Center, rot, false, false);

			//デモンストレーションを見ていないときは1秒50になったらエンターを促す画面を表示
			//見た場合は最初から表示
			if (FrameCnt >= 90 || demostartedflag == true)
			{
				//タイトル画像
				DrawExtendGraph(SCREEN_X / 6, SCREEN_Y / 6, SCREEN_X / 6 * 5, SCREEN_Y / 3, Handle_Title, false);
				if (FrameCnt % 60 >= 30)
				{
					//エンターキーを促す文字と画像
					DrawExtendGraph(SCREEN_X / 7, SCREEN_Y / 6 * 4, SCREEN_X / 7 * 6, SCREEN_Y / 6 * 4 + 60, Handle_Title_DispAssist, false);
					DispStringOfBox(SCREEN_X / 7 + 5, SCREEN_Y / 6 * 4 + 5, 1.0, -1, "PRESS ENTER KEY...");
				}
				//デモンストレーションのカウントダウンを表示
				if (FrameCnt < 60 * 9 - 30)
					DispStringOfBox(20, SCREEN_Y - 80, 1.5, -1, "%d TO DEMO", 10 - (FrameCnt + 90) / 60);
				else
					DispStringOfBox(20, SCREEN_Y - 80, 1.5, -1, "DEMO SOON");
			}

			//1/60秒ごとに12度迷路のマップが回る。
			if (FrameCnt < 90)
				rot += 12;

			//通常裏画面に切り替え
			SetDrawScreen(DX_SCREEN_BACK);

			//デモンストレーションを見て、再度タイトル画面を表示させる場合フェードイン
			if (FrameCnt < 90 && demostartedflag == true)
			{
				SetDrawBright(
					255 * (double(FrameCnt) / 90.0),
					255 * (double(FrameCnt) / 90.0),
					255 * (double(FrameCnt) / 90.0)
					);
			}
			//タイトル画面は8秒50でフェードアウト
			else if (FrameCnt >= 60 * 9 - 30)
			{
				SetDrawBright(
					255 * (1.0 - double(FrameCnt - (60 * 9 - 30)) / 90.0),
					255 * (1.0 - double(FrameCnt - (60 * 9 - 30)) / 90.0),
					255 * (1.0 - double(FrameCnt - (60 * 9 - 30)) / 90.0)
					);
			}
			//切り替え以外は通常表示
			else
			{
				SetDrawBright(255, 255, 255);
			}
			DrawGraph(0, 0, Handle_Screen_Normal, false);

			FrameCnt++;

			//表画面に表示
			ScreenFlip();
		}

		//タイトル画面（デモンストレーション）
		MapFlag = false;
		demostartedflag = true;
		FrameCnt = 0;
		CountStartTime = GetNowCount();
		Map2Dim.SetBai(1.0);
		SolveMaze(map.data, map.ans, map.x, map.z, 0, 2, item.map.x, item.map.z);			//迷路の答えを作成

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

			ClsDrawScreen();		//描画する画面（裏）を黒クリア

			GetHitKeyStateAll(KeyFlag);

			if ((FrameCnt + 1) % 1000 == 0)		MapFlag = !MapFlag;
			if ((FrameCnt + 1) % 1000 == 800)	MapFlag = !MapFlag;

			if (StartGoalFlag == 0)
				nowtime = (GetNowCount() - CountStartTime) / 1000;
			else if (StartGoalFlag == 1)
				nowtime = 0;

			//自機の移動
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
				//マップを手に入れていなかったらマップまでの経路を計算
				if (item.map.isgotten == false)
				{
					SolveMaze(map.data, map.ans, map.x, map.z, Jiki_MasuX, Jiki_MasuZ, item.map.x, item.map.z);			//迷路の答えを作成
				}
				else
				{
					if (keymode == false)
					{
						SolveMaze(map.data, map.ans, map.x, map.z, Jiki_MasuX, Jiki_MasuZ, map.x - 1, map.z - 3);			//迷路の答えを作成
					}
					else
					{
						//鍵を手に入れていなかったら鍵までの経路を計算
						if (item.key.isgotten == false)
						{
							SolveMaze(map.data, map.ans, map.x, map.z, Jiki_MasuX, Jiki_MasuZ, item.key.x, item.key.z);			//迷路の答えを作成
						}
						else
						{
							SolveMaze(map.data, map.ans, map.x, map.z, Jiki_MasuX, Jiki_MasuZ, map.x - 1, map.z - 3);			//迷路の答えを作成
						}
					}
				}
			}


			//スタート、ゴールの管理
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

			//表示部分
			disp(keymode);

			if (StartGoalFlag != 1)
			{
				//現在の経過時間と目標時間を表示
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

			//エンターキーを促す文字と画像
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

			//■■■ここまで
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

			//表と裏（描画画面）の入れ換え
			ScreenFlip();
		}
		//メインループ終了

		//StartGoalFlag = 1;

		//後始末
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

	//目の前に壁がなかったら
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
				//歩く
			{
				Jiki.Pos.x -= sin(RAD1*Camera.Kakudo) * 4;
				Jiki.Pos.z -= cos(RAD1*Camera.Kakudo) * 4;
			}
		}
	}
	//壁にぶつかったら
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

		//向きを変える
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

	//現在の場所を訪問済みにする
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

//表示全般 表示はここ経由でする。
void MazeTitle::disp(bool keymode)
{
	disp_3d(keymode);

	if (item.map.isgotten)
		disp_2dmap(false, keymode);
}

//3Dの迷路を表示する。
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

//２次元のマップを表示する。
void MazeTitle::disp_2dmap(bool FullScreenFlag, bool keymode)
{
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
	if (item.ans.isgotten)
		Map2Dim.disp(Jiki.Pos, Camera.Kakudo + 180.0, true, true, true, keymode, true);
	else
		Map2Dim.disp(Jiki.Pos, Camera.Kakudo + 180.0, true, false, true, keymode, true);

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
}

//アイテムを表示する。
//回って上がったり下がったりしながら表示している。
void MazeTitle::disp_item(bool keymode)
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
		if (item.ans.x - 10 <= int(Jiki.Pos.x / Masu_1) && item.ans.x + 10 > int(Jiki.Pos.x / Masu_1) && item.ans.z - 10 <= int(Jiki.Pos.z / Masu_1) && item.ans.z + 10 > int(Jiki.Pos.z / Masu_1))
		{
			MV1SetPosition(Handle_Item_Ans, VGet(item.ans.x * Masu_1 + Masu_1 / 2, height, item.ans.z * Masu_1 + Masu_1 / 2));
			MV1SetScale(Handle_Item_Ans, VGet(1.0, 1.0, 1.0));
			MV1SetRotationXYZ(Handle_Item_Ans, VGet(0.0, DegtoRad(rot), 0.0));
			MV1DrawModel(Handle_Item_Ans);
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
