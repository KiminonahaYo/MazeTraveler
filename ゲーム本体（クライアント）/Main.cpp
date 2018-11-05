//ヘッダーファイル読み込み
#include "Defines.h"
#include "BackGround.h"
#include "demo.h"
#include "play.h"
#include "DrawMoji.h"
#include "MyMaze.h"
#include "disphelp.h"

void LoadGameFileLists(void);		//ゲームファイル一覧取得
void loadmedia(void);				//外部ファイル読み込み
void init(void);					//プログラムの準備
void dest(void);					//プログラムの後始末
void GameMain(void);				//ゲームメイン関数
int filemenu(void);

SPLITDATA GameFilenames = NULL;		//ゲームファイル一覧
char **fileusername = {NULL};		//ゲームファイルのユーザー名
int *filepoint = NULL;				//ゲームファイルの得点
int Filename_n = 0;					//ゲームファイル数

//メイン関数
int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	init();

	GameMain();

	dest();

	return 0;
}

//ゲームメイン関数
void GameMain(void)
{
	bool brkflag = false;
	int ret;

	//-------------------------------------------------------------------------------------------

	loadmedia();

	LoadGameFileLists();
	if (Filename_n == 0)
	{
		disp_navi_set("NewUser");
	}

	while (1)
	{
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
			brkflag = true;

		if (brkflag)
			break;

		//タイトル画面を表示する。
		ret = Title.Disp();
		if (ret == -1)
		{
			brkflag = true;
			continue;
		}

		//ファイルメニューを表示する。
		ret = filemenu();
		if (ret == -1)
		{
			brkflag = true;
			continue;
		}
	}
}

int filemenu(void)
{
	bool brkflag = false;
	bool backtitleflag = false;

	int fileno = 0;

	int pos_no;
	int space_no;
	int top_position = 100;
	int pos_no_cur;
	int space_no_cur;

	int ctrlflag = 0;

	bool copyflag = false;
	int copystep = 0;
	int copyfromno = 0;

	bool makeflag = false;

	bool delflag = false;
	bool delchk = false;
	bool delreally = false;

	char newfilename[16] = "";
	char buf[256] = "";
	char buf2[256] = "";

	int cursor = 0;

	int i;

	int framecnt = 0;

	fileno = 0;

	while (1)
	{
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
			brkflag = true;

		if (brkflag)
			break;

		if (backtitleflag)
			break;

		ClearDrawScreen();

		DrawBackGround(SCREEN_X, SCREEN_Y, 1);

		if (makeflag != 1)
		{
			//キーコントロール部（操作部）
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

			DrawGraph(SCREEN_X - 210, 100, Handle_Help_Navi, FALSE);
		}

		if (makeflag == true)
		{
			static bool confflag = true;

			if (confflag)
			{
				ClearInputCharBuf();
				confflag = false;
			}

			if (Stick(KEY_INPUT_UP, true))
			{
				if (fileno > Filename_n)
				{
					confflag = true;
					fileno--;
				}
			}
			if (Stick(KEY_INPUT_DOWN, true))
			{
				if (fileno < Filename_n + 1)
				{
					confflag = true;
					fileno++;
				}
			}

			if (fileno == Filename_n)
			{
				char c = m_inputalpha();

				if (c != -1)
				{
					if (c == CTRL_CODE_BS)
					{
						if (cursor > 0)
						{
							cursor--;
							newfilename[cursor] = '\0';
						}
					}
					else
					{
						if (cursor < 15)
						{
							newfilename[cursor] = c;
							newfilename[cursor + 1] = '\0';
							cursor++;
						}
					}
				}

				if (Stick(KEY_INPUT_RETURN))
				{
					//何かファイル名が入力された、かつ同名のファイルが既に存在しない場合
					if (strlen(newfilename) > 0 && exist("./%s/%s%s", SAVE_DIR, newfilename, KAKUCYOSI) == false)
					{
						MakeNewGameFile(newfilename);
						cursor = 0;
						makeflag = false;
						strcpy_s(newfilename, 16, "");
						LoadGameFileLists();
						fileno = 0;
						confflag = true;
						disp_navi_set("OpenUserName");
					}
				}
			}
			else
			{
				if (Stick(KEY_INPUT_RETURN))
				{
					strcpy_s(newfilename, 16, "");
					cursor = 0;
					makeflag = false;
					fileno = Filename_n;
					confflag = true;
				}
			}
		}
		if (copyflag)
		{
			static bool confflag = false;

			if (confflag)
			{
				ClearInputCharBuf();
				confflag = false;
			}

			if (copystep == 0)
			{
				if (Stick(KEY_INPUT_UP, true))
				{
					if (fileno > 0)
					{
						fileno--;
					}
				}
				if (Stick(KEY_INPUT_DOWN, true))
				{
					if (fileno < Filename_n)
					{
						fileno++;
					}
				}
				if (Stick(KEY_INPUT_RETURN))
				{
					if (fileno < Filename_n)
					{
						copyfromno = fileno;
						copystep = 1;
						confflag = true;
						fileno = 0;
						disp_navi_set("CopyName");
					}
					else
					{
						strcpy_s(newfilename, 16, "");
						cursor = 0;
						fileno = Filename_n + 1;
						copyflag = false;
						copystep = 0;
						confflag = false;
						copyfromno = 0;
					}
				}
				if (Stick(KEY_INPUT_BACK))
				{
					copystep = 0;
					copyflag = false;
					fileno = Filename_n + 1;
				}
			}
			else if (copystep == 1)
			{
				if (Stick(KEY_INPUT_UP, true))
				{
					if (fileno > 0)
					{
						fileno--;
					}
				}
				if (Stick(KEY_INPUT_DOWN, true))
				{
					if (fileno < 1)
					{
						fileno++;
					}
				}

				if (fileno == 0)
				{
					char c = m_inputalpha();

					if (c != -1)
					{
						if (c == CTRL_CODE_BS)
						{
							if (cursor > 0)
							{
								cursor--;
								newfilename[cursor] = '\0';
							}
						}
						else
						{
							if (cursor < 15)
							{
								newfilename[cursor] = c;
								newfilename[cursor + 1] = '\0';
								cursor++;
							}
						}
					}

					if (Stick(KEY_INPUT_RETURN))
					{
						sprintf_s(buf, 256, "%s%s", newfilename, KAKUCYOSI);
						//何かファイル名が入力された、かつ同名のファイルが既に存在しない場合
						if (strlen(newfilename) > 0 && exist("./%s/%s%s", SAVE_DIR, newfilename, KAKUCYOSI) == false && strcmp(GameFilenames[copyfromno], buf) != 0)
						{
							sprintf_s(buf, 256, "./%s/%s%s", SAVE_DIR, newfilename, KAKUCYOSI);
							sprintf_s(buf2, 256, "./%s/%s", SAVE_DIR, GameFilenames[copyfromno]);

							filecopy(buf2, buf);
							strcpy_s(newfilename, 16, "");
							LoadGameFileLists();
							cursor = 0;
							fileno = Filename_n + 1;
							copyflag = false;
							copystep = 0;
							confflag = false;
							copyfromno = 0;
						}
					}
				}
				else if (fileno == 1)
				{
					if (Stick(KEY_INPUT_RETURN))
					{
						strcpy_s(newfilename, 16, "");
						cursor = 0;
						fileno = Filename_n + 1;
						copyflag = false;
						copystep = 0;
						confflag = false;
						copyfromno = 0;
					}
				}
			}
		}
		if (delflag)
		{
			if (delchk == false)
			{
				if (Stick(KEY_INPUT_DOWN, true))
				{
					if (fileno < Filename_n)
						fileno++;
				}
				if (Stick(KEY_INPUT_UP, true))
				{
					if (fileno > 0)
						fileno--;
				}
				if (Stick(KEY_INPUT_BACK))
				{
					delreally = false;
					delchk = false;
					delflag = 0;
					fileno = Filename_n + 2;
				}
			}
			else
			{
				if (Stick(KEY_INPUT_LEFT))
				{
					delreally = false;
				}
				if (Stick(KEY_INPUT_RIGHT))
				{
					delreally = true;
				}
				if (Stick(KEY_INPUT_BACK))
				{
					delreally = false;
					delchk = false;
				}
			}

			if (Stick(KEY_INPUT_RETURN))
			{
				if (fileno < Filename_n)
				{
					if (delchk)
					{
						if (delreally)
						{
							filedel("./Saves/%s", GameFilenames[fileno]);
							LoadGameFileLists();
							delreally = false;
							delchk = false;
						}
						else
						{
							delreally = false;
							delchk = false;
						}
					}
					else
					{
						delchk = true;
					}
				}
				else
				{
					delreally = false;
					delchk = false;
					delflag = 0;
					fileno = Filename_n + 2;
				}
			}
		}
		else
		{
			int ret;

			if (Stick(KEY_INPUT_DOWN, true))
			{
				if (fileno < Filename_n + 4)
					fileno++;
			}
			if (Stick(KEY_INPUT_UP, true))
			{
				if (fileno > 0)
					fileno--;
			}

			if (fileno < Filename_n)			ctrlflag = 0;
			else if (fileno == Filename_n)		ctrlflag = 1;
			else if (fileno == Filename_n + 1)	ctrlflag = 2;
			else if (fileno == Filename_n + 2)	ctrlflag = 3;
			else if (fileno == Filename_n + 3)	ctrlflag = 4;
			else if (fileno == Filename_n + 4)	ctrlflag = 5;

			if (Stick(KEY_INPUT_RETURN))
			{
				switch (ctrlflag)
				{
				case 0:
					//ファイルオープンしてプレイ
					ret = Game.init(GameFilenames[fileno]);

					if (ret == 0)
					{
						disp_navi_set("UserMenu");
						ret = Game.start();
						Game.dest();

						if (ret == -1)
							brkflag = 1;

					}
					else
					{
						disp_navi_set("FileUsing");
					}

					ClearInputCharBuf();
					LoadGameFileLists();

					continue;

					break;

				case 1:
					disp_navi_set("NewUserName");
					makeflag = true;
					break;
				case 2:
					disp_navi_set("CopyUser");
					copyflag = true;
					copystep = 0;
					fileno = 0;
					break;
				case 3:
					disp_navi_set("DeleteUser");
					delflag = true;
					fileno = 0;
					break;
				case 4:
					backtitleflag = true;
					break;
				case 5:
					brkflag = true;
					break;
				}
			}
		}

		if (brkflag)
			continue;

		//表示部

		SetFontSize(15);
		DrawFormatString(10, SCREEN_Y - 20, STR_COLOR_2, "%d FPS", GetFPS());

		SetFontSize(20);

		pos_no = space_no = 0;
		
		//メニュー項目の左上y座標を決める
		pos_no_cur = space_no_cur = 0;
		for (i = 0; i <= fileno; i++)
		{
			if (delflag)
			{
				if (delchk == false)
				{
					if (i == Filename_n) space_no_cur++;
				}
			}
			else if (copyflag)
			{
				if (copystep == 0)
				{
					if (i == Filename_n) space_no_cur++;
				}
				else if (copystep == 1)
				{
					if (i == 0) space_no_cur++;
					if (i == 1) space_no_cur++;
				}
			}
			else if (makeflag)
			{
				if (i == Filename_n)		space_no_cur++;
				if (i == Filename_n + 1)	space_no_cur++;
			}
			else
			{
				if (i == Filename_n)		space_no_cur++;
				if (i == Filename_n + 3)	space_no_cur++;
			}
			if (i < fileno)
				pos_no_cur++;
		}

		//メニュー項目の左上y座標は通常は100とする。
		if (pos_no_cur * 30 + space_no_cur * 20 < 550)
			top_position = 100;
		else
			top_position = 100 - (pos_no_cur * 30 + space_no_cur * 20 - 540);

		//ユーザーを表示する。（通常、コピー、削除、新規ファイル表示の時に表示される。）
		if (delchk == false && copystep == 0)
		{
			for (i = 0; i < Filename_n; i++)
			{
				if (filepoint[i] <= 2000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, i == fileno, S_BLUE, 650, "ユーザー%02d. %s %dP 初心者 %s%s", i + 1, fileusername[i], filepoint[i], delflag ? "を削除する" : "", copyflag ? "をコピーする" : "");
				}
				else if (filepoint[i] <= 5000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, i == fileno, S_GREEN, 650, "ユーザー%02d. %s %dP 中級者 %s%s", i + 1, fileusername[i], filepoint[i], delflag ? "を削除する" : "", copyflag ? "をコピーする" : "");
				}
				else if (filepoint[i] <= 10000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, i == fileno, S_YELLOW, 650, "ユーザー%02d. %s %dP 上級者 %s%s", i + 1, fileusername[i], filepoint[i], delflag ? "を削除する" : "", copyflag ? "をコピーする" : "");
				}
				else if (filepoint[i] <= 20000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, i == fileno, S_RED, 650, "ユーザー%02d. %s %dP 超人 %s%s", i + 1, fileusername[i], filepoint[i], delflag ? "を削除する" : "", copyflag ? "をコピーする" : "");
				}
				else
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, i == fileno, S_PURPLE, 650, "ユーザー%02d. %s %dP マスター %s%s", i + 1, fileusername[i], filepoint[i], delflag ? "を削除する" : "", copyflag ? "をコピーする" : "");
				}

				pos_no++;
			}
		}

		//削除メニュー
		if (delflag)
		{
			//削除ファイルの選択画面を表示する。（ユーザーは除く、上のプログラムで代用）
			if (delchk == false)
			{
				DispStringOfBox(10, 10, 0.7, -1, "MAZE TRAVELER  DELETE FILE SELECT");

				if (fileno < Filename_n)
					DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "%02d. %s %dP", fileno + 1, fileusername[fileno], filepoint[fileno]);
				else
					DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "BACK TO FILE SELECT");

				space_no++;

				dispselect(top_position + 30 * pos_no + 20 * space_no, fileno == Filename_n, S_BLUE, 650, "戻る");
			}
			//削除ファイルを選択した後に削除の警告を出す。
			else
			{
				DispStringOfBox(10, 10, 0.7, -1, "MAZE TRAVELER  DO YOU DELETE IT?");

				//ユーザーの表示
				if (filepoint[fileno] <= 2000)
				{
					DrawGraph(SCREEN_X / 2 - 320, SCREEN_Y / 4, Handle_SelectTile_Blue, false);
					DrawFormatString(SCREEN_X / 2 - 320 + 10, SCREEN_Y / 4 + 10, STR_COLOR_2, "ユーザー%02d. %s %dP 初心者", fileno + 1, fileusername[fileno], filepoint[fileno]);
				}
				else if (filepoint[fileno] <= 5000)
				{
					DrawGraph(SCREEN_X / 2 - 320, SCREEN_Y / 4, Handle_SelectTile_Green, false);
					DrawFormatString(SCREEN_X / 2 - 320 + 10, SCREEN_Y / 4 + 10, STR_COLOR_2, "ユーザー%02d. %s %dP 中級者", fileno + 1, fileusername[fileno], filepoint[fileno]);
				}
				else if (filepoint[fileno] <= 10000)
				{
					DrawGraph(SCREEN_X / 2 - 320, SCREEN_Y / 4, Handle_SelectTile_Yellow, false);
					DrawFormatString(SCREEN_X / 2 - 320 + 10, SCREEN_Y / 4 + 10, STR_COLOR_2, "ユーザー%02d. %s %dP 上級者", fileno + 1, fileusername[fileno], filepoint[fileno]);
				}
				else if (filepoint[fileno] <= 20000)
				{
					DrawGraph(SCREEN_X / 2 - 320, SCREEN_Y / 4, Handle_SelectTile_Red, false);
					DrawFormatString(SCREEN_X / 2 - 320 + 10, SCREEN_Y / 4 + 10, STR_COLOR_2, "ユーザー%02d. %s %dP 超人", fileno + 1, fileusername[fileno], filepoint[fileno]);
				}
				else
				{
					DrawGraph(SCREEN_X / 2 - 320, SCREEN_Y / 4, Handle_SelectTile_Purple, false);
					DrawFormatString(SCREEN_X / 2 - 320 + 10, SCREEN_Y / 4 + 10, STR_COLOR_2, "ユーザー%02d. %s %dP マスター", fileno + 1, fileusername[fileno], filepoint[fileno]);
				}

				//本当に削除しますか？（Yes/No)を表示する。
				DrawGraph(SCREEN_X / 2 - 320, SCREEN_Y / 2 - 75, Handle_Delete_Warning, false);
				if (delreally)
				{
					DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "%02d. %s WILL DELETE", fileno + 1, fileusername[fileno]);
					DrawExtendGraph(SCREEN_X / 2 + 160 - 50 * 1.3, SCREEN_Y / 2 + 70 - 30 * 1.3, SCREEN_X / 2 + 160 + 50 * 1.3, SCREEN_Y / 2 + 70 + 30 * 1.3, Handle_Delete_Yes, false);
					DrawExtendGraph(SCREEN_X / 2 - 160 - 50, SCREEN_Y / 2 + 70 - 30, SCREEN_X / 2 - 160 + 50, SCREEN_Y / 2 + 70 + 30, Handle_Delete_No, false);
				}
				else
				{
					DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "%02d. %s WON'T DELETE", fileno + 1, fileusername[fileno]);
					DrawExtendGraph(SCREEN_X / 2 + 160 - 50, SCREEN_Y / 2 + 70 - 30, SCREEN_X / 2 + 160 + 50, SCREEN_Y / 2 + 70 + 30, Handle_Delete_Yes, false);
					DrawExtendGraph(SCREEN_X / 2 - 160 - 50 * 1.3, SCREEN_Y / 2 + 70 - 30 * 1.3, SCREEN_X / 2 - 160 + 50 * 1.3, SCREEN_Y / 2 + 70 + 30 * 1.3, Handle_Delete_No, false);
				}
			}
		}
		//コピーメニュー
		else if (copyflag)
		{
			//コピー元ファイルの選択画面を表示する。（ユーザー表示を除く、上のプログラムで代用する。）
			if (copystep == 0)
			{
				DispStringOfBox(10, 10, 0.7, -1, "MAZE TRAVELER  COPYFILE SELECT");

				if (fileno < Filename_n)
				{
					DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "%02d. %s %dP", fileno + 1, fileusername[fileno], filepoint[fileno]);
				}
				else if (fileno == Filename_n)
				{
					DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "BACK TO THE TITLE");
				}

				space_no++;

				dispselect(top_position + 30 * pos_no + 20 * space_no, fileno == Filename_n, S_BLUE, 650, "戻る");
			}
			//コピー先ファイルのファイル名を入力する画面を表示する。
			else if (copystep == 1)
			{
				DispStringOfBox(10, 10, 0.7, -1, "MAZE TRAVELER  COPYNAME INPUT");

				//ユーザーを表示する。
				if (filepoint[copyfromno] <= 2000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, false, S_BLUE, 650, "コピー元 : ユーザー%02d. %s %dP 初心者", copyfromno + 1, fileusername[copyfromno], filepoint[copyfromno]);
				}
				else if (filepoint[copyfromno] <= 5000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, false, S_GREEN, 650, "コピー元 : ユーザー%02d. %s %dP 中級者", copyfromno + 1, fileusername[copyfromno], filepoint[copyfromno]);
				}
				else if (filepoint[copyfromno] <= 10000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, false, S_YELLOW, 650, "コピー元 : ユーザー%02d. %s %dP 上級者", copyfromno + 1, fileusername[copyfromno], filepoint[copyfromno]);
				}
				else if (filepoint[copyfromno] <= 20000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, false, S_RED, 650, "コピー元 : ユーザー%02d. %s %dP 超人", copyfromno + 1, fileusername[copyfromno], filepoint[copyfromno]);
				}
				else
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, false, S_PURPLE, 650, "コピー元 : ユーザー%02d. %s %dP マスター", copyfromno + 1, fileusername[copyfromno], filepoint[copyfromno]);
				}
				pos_no++;

				space_no++;

				//入力画面
				dispselect(top_position + 30 * pos_no + 20 * space_no, fileno == 0, S_BLUE, 650, "コピーファイル名：%s", newfilename);

				//ステータス表示とカーソル
				if (fileno == 0)
				{
					DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "COPY TO %s", newfilename);

					if (framecnt % 60 < 30)
						DrawLine(
						10 + 190 + cursor * 11,
						top_position + 20 + 30 * 1 + 3,
						10 + 190 + cursor * 11,
						top_position + 20 + 30 * 2 - 3,
						STR_COLOR_2);
				}
				else if (fileno == 1)
				{
					DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "BACK TO THE FILE SELECT");
				}
				pos_no++;

				space_no++;

				dispselect(top_position + 30 * pos_no + 20 * space_no, fileno == 1, S_PURPLE, 650, "戻る");
			}
		}
		//新規ファイル作成画面（ユーザー表示は除く、上のプログラムで代用）
		else if (makeflag == 1)
		{
			DispStringOfBox(10, 10, 0.7, -1, "MAZE TRAVELER  FILENAME INPUT");

			if (fileno == Filename_n)
			{
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "NEW NAME : %s", newfilename);
			}
			else if (fileno == Filename_n + 1)
			{
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "BACK TO THE TITLE");
			}

			space_no++;
			dispselect(top_position + 30 * pos_no + 20 * space_no, fileno == Filename_n, S_BLUE, 650, "ユーザー名 : %s", newfilename);
			pos_no++;

			space_no++;
			dispselect(top_position + 30 * pos_no + 20 * space_no, fileno == Filename_n + 1, S_PURPLE, 650, "戻る");

			if (fileno == Filename_n)
			{
				if (framecnt % 60 < 30)
					DrawLine(
					10 + 140 + cursor * 11,
					top_position + 30 * (Filename_n + 0) + 20 + 3,
					10 + 140 + cursor * 11,
					top_position + 30 * (Filename_n + 1) + 20 - 3,
					STR_COLOR_2);
			}
		}
		//通常画面
		else
		{
			DispStringOfBox(10, 10, 0.7, -1, "MAZE TRAVELER  OPENFILE SELECT");

			//ステータス表示
			switch (ctrlflag)
			{
			case 0:
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "%02d. %s %dP", fileno + 1, fileusername[fileno], filepoint[fileno]);
				break;
			case 1:
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "NEW FILE");
				break;
			case 2:
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "COPY FILE");
				break;
			case 3:
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "DELETE FILE");
				break;
			case 4:
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "BACK TO THE TITLE");
				break;
			case 5:
				DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "EXIT THE GAME");
				break;
			}

			//ユーザー表示を除くその他のオプション
			space_no++;

			dispselect(top_position + 30 * pos_no + 20 * space_no, ctrlflag == 1, S_BLUE, 650, "新しいユーザーを作成");
			pos_no++;

			dispselect(top_position + 30 * pos_no + 20 * space_no, ctrlflag == 2, S_BLUE, 650, "ユーザーデータをコピー");
			pos_no++;

			dispselect(top_position + 30 * pos_no + 20 * space_no, ctrlflag == 3, S_RED, 650, "ユーザーを削除");
			pos_no++;

			space_no++;

			dispselect(top_position + 30 * pos_no + 20 * space_no, ctrlflag == 4, S_PURPLE, 650, "タイトルに戻る");
			pos_no++;

			dispselect(top_position + 30 * pos_no + 20 * space_no, ctrlflag == 5, S_PURPLE, 650, "ゲームを終了する");
		}

		disp_navi();

		ScreenFlip();

		framecnt++;
	}

	//強制終了フラグが立った時に強制終了の値を返す
	if (brkflag)
		return -1;
	else
		return 0;
}

//プログラムの準備
void init(void)
{
	FILE *fp;

	//画面サイズとウィンドウモード設定
	SetGraphMode(SCREEN_X, SCREEN_Y, 32);
	ChangeWindowMode(true);										//ウィンドウモードで起動、コメントアウトでフルスクリーン
	SetDoubleStartValidFlag(true);
	SetMainWindowText(MES_TITLEBAR);
	SetWindowIconID(101);
	SetAlwaysRunFlag(true);
	SetOutApplicationLogValidFlag(false);

	//-------------------------------------------------------------------------------------------

	if (DxLib_Init() == -1) return exit(-1);					//DXライブラリ初期化

	SetFullSceneAntiAliasingMode(4, 2);							// フルシーンアンチエイリアスを設定、少し遅くなりますがきれいに表示されます。

	SetBackgroundColor(0, 0, 0);								//背景色
	SetDrawScreen(DX_SCREEN_BACK);								//裏画面に描画

	srand((unsigned)time(NULL));

	
	if (IsSaveLog)
	{
		fopen_s(&fp, "MyLog.txt", "w");
		fclose(fp);
	}

	GetArray1Dim(char, MyLog.content, 1);
	MyLog.content[0] = '\0';
	MyLog.len = 0;
}

//プログラムの後始末
void dest(void)
{
	FILE *fp;

	delsplit(Filename_n, GameFilenames);
	delsplit(help_text_spl_n, help_text_spl);
	DelArray1Dim(filepoint);
	DelArray2Dim(fileusername, Filename_n);

	/*
	if (IsSaveLog)
	{
		fopen_s(&fp, "MyLog.txt", "w");
		fprintf(fp, "%s", MyLog.content);
		fclose(fp);
	}
	*/

	DelArray1Dim(MyLog.content);

	DxLib_End();
}

//外部ファイル読み込み
void loadmedia(void)
{
	Handle_Jiki_Stopping		= MV1LoadModel("Model/Jiki/Jiki_Alpha_Stopping.mqo");		//自機停止時
	Handle_Jiki_Moving_Body		= MV1LoadModel("Model/Jiki/Jiki_Alpha_Moving_Body.mqo");	//自機移動時の体
	Handle_Jiki_Moving_Foot		= MV1LoadModel("Model/Jiki/Jiki_Alpha_Moving_Foot.mqo");	//自機移動時の足
	Handle_Jiki_Moving_Arm		= MV1LoadModel("Model/Jiki/Jiki_Alpha_Moving_Arm.mqo");		//自機移動時の腕
	Handle_LightModel			= MV1LoadModel("Model/Light.mqo");
	Handle_Item_Map				= MV1LoadModel("Model/item/item_map.mqo");
	Handle_Item_Ans				= MV1LoadModel("Model/item/item_ans.mqo");
	Handle_Item_Key				= MV1LoadModel("Model/item/item_key.mqo");
	Handle_Wall					= MV1LoadModel("Model/Wall.mqo");							//迷路壁
	Handle_Floor				= MV1LoadModel("Model/Floor.mqo");							//迷路床・天井
	Handle_Start				= MV1LoadModel("Model/Start.mqo");							//迷路スタート地点
	Handle_Goal					= MV1LoadModel("Model/Goal.mqo");							//迷路ゴール地点
	Handle_GoalDoor				= MV1LoadModel("Model/Goal_Door_Keymode.mqo");

	Handle_Title				= LoadGraph("Picture/MazeTitle.png");						//MazeTravelerタイトル

	Handle_SelectTile_Blue		= LoadGraph("Picture/SelectTile_Blue.png");					//選択メニュー青
	Handle_SelectTile_Red		= LoadGraph("Picture/SelectTile_Red.png");					//選択メニュー赤
	Handle_SelectTile_Green		= LoadGraph("Picture/SelectTile_Green.png");				//選択メニュー緑
	Handle_SelectTile_Purple	= LoadGraph("Picture/SelectTile_Purple.png");				//選択メニュー紫
	Handle_SelectTile_Yellow	= LoadGraph("Picture/SelectTile_Yellow.png");				//選択メニュー黄色
	Handle_SelectTile_Gray		= LoadGraph("Picture/SelectTile_Gray.png");					//選択メニュー灰色

	Handle_TimeDisp_Tile		= LoadGraph("Picture/TimeDisp_Tile.png");					//迷路のステージ名と時間表示

	Handle_Delete_Warning		= LoadGraph("Picture/Delete_Warning.png");					//削除を警告する表示
	Handle_Delete_Yes			= LoadGraph("Picture/Delete_Yes.png");						//削除するの選択
	Handle_Delete_No			= LoadGraph("Picture/Delete_No.png");						//削除しないの選択

	Handle_SolveRoute			= LoadGraph("Picture/SolveRoute.png");						//正解ルートのナビゲート

	Handle_Goaled				= LoadGraph("Picture/Goaled.png");							//ゴールしたことを示す表示
	Handle_Title_DispAssist		= LoadGraph("Picture/Title_DispAssist.png");				//タイトル画面の文字が見えやすいようにするための背景

	Handle_Result_Tile			= LoadGraph("Picture/Result_Tile.png");						//ステージの結果（ステージ名と時間表示、目標時間との対比グラフ）
	Handle_Result_Passed		= LoadGraph("Picture/Result_Passed.png");					//ステージ合格
	Handle_Result_VeryGood		= LoadGraph("Picture/Result_VeryGood.png");
	Handle_Result_Failed		= LoadGraph("Picture/Result_Failed.png");					//ステージ不合格
	Handle_Gotitem_Tile			= LoadGraph("Picture/gotitem_background.png");				//手に入れたアイテム表示板
	Handle_Gotitem_Map			= LoadGraph("Picture/gotitem_map.png");						//アイテムの　　マップ　を手に入れました。
	Handle_Gotitem_Ans			= LoadGraph("Picture/gotitem_ans.png");						//アイテムの　経路表示　を手に入れました。
	Handle_Gotitem_Key			= LoadGraph("Picture/gotitem_key.png");						//アイテムの　　　　鍵　を手に入れました。
	Handle_Challanging_Keymode	= LoadGraph("Picture/Challanging_Keymode.png");				//鍵モード挑戦を示す画像
	Handle_Bought_AnsItem		= LoadGraph("Picture/Bought_AnsItem.png");
	Handle_Level3_Unlock		= LoadGraph("Picture/Unlocked_LEVEL3.png");
	Handle_Level5_Unlock		= LoadGraph("Picture/Unlocked_LEVEL5.png");
	Handle_Unlock_Level			= LoadGraph("Picture/Unlocked_LEVEL.png");
	Handle_Help_Navi			= LoadGraph("Picture/Help_Navi.png");

	Handle_Online_Mine			= LoadGraph("Picture/Online_Mine.png");

	Handle_NetWork_Connect_Error = LoadGraph("Picture/NetWork_Connect_Error.png");

	Handle_Unlock_KeymodeInStage_UntilLevel2	= LoadGraph("Picture/Unlocked_keymode_Until_LEVEL2.png");
	Handle_Unlock_KeymodeInStage				= LoadGraph("Picture/Unlocked_keymode.png");

	Handle_Chat_navigate						= LoadGraph("Picture/Chat_navigate.png");

	SetTransColor(100, 100, 100);
	LoadDivGraph("Picture/Dot4.png", 10, 10, 1, 10, 10, Handle_Dotpic);						//DispStringOfBox関数で表示する文字のドット（1文字辺り3×5）
	LoadDivGraph("Picture/Map_2Dim.png", 80, 8, 10, 10, 10, Handle_2dimmap);				//2次元マップの表示画像
	LoadDivGraph("Picture/BackGround.png", 2, 1, 2, 20, 20, Handle_BackGround);				//このゲームの動く背景画像（ゲーム選択）
	LoadDivGraph("Picture/BackGround_2.png", 2, 1, 2, 20, 20, Handle_BackGround_2);			//このゲームの動く背景画像（ユーザー選択）
	LoadDivGraph("Picture/BackGround_3.png", 2, 1, 2, 20, 20, Handle_BackGround_3);			//このゲームの動く背景画像（ユーザー選択）
	SetTransColor(0, 0, 0);

	SetTransColor(200, 200, 200);
	Handle_Help_Back = LoadGraph("Picture/Help_Back.png");
	SetTransColor(0, 0, 0);

	SetTransColor(200, 200, 200);
	Handle_Navigate_Back = LoadGraph("Picture/Navigate_Back.png");
	SetTransColor(0, 0, 0);

	Handle_Screen_FullFalse		= MakeScreen(SCREEN_X / 3, SCREEN_Y / 3);					//2次元マップナビゲートの非フルスクリーン版
	Handle_Screen_FullTrue		= MakeScreen(SCREEN_X, SCREEN_Y);							//2次元マップナビゲートのフルスクリーン版
	Handle_Screen_Normal		= MakeScreen(SCREEN_X, SCREEN_Y);							//通常画面
	Handle_Screen_Chat			= MakeScreen(SCREEN_X / 3, SCREEN_Y / 3);					//チャット窓スクリーン

	SetDrawScreen(Handle_Screen_Chat);
	ClearDrawScreen();
	SetDrawScreen(Handle_Screen_FullFalse);
	ClearDrawScreen();
	SetDrawScreen(Handle_Screen_FullTrue);
	ClearDrawScreen();
	SetDrawScreen(Handle_Screen_Normal);
	ClearDrawScreen();

	//ヘルプファイル読み込み
	Notesel(help_text, 32768);
	Noteload("helps/help_text.txt");
	help_text_spl_n = split(help_text, ";", &help_text_spl);

	//ナビゲーション読み込み
	Notesel(navi_text, 32768);
	Noteload("helps/navi_text.txt");
	navi_text_spl_n = split(navi_text, ";", &navi_text_spl);
}

//ゲームファイル一覧取得
void LoadGameFileLists(void)
{
	char filelist[2048];
	char buf[256];
	char username[256] = "";
	int i, j;
	FILE *fp;

	//ゲームデータ一覧取得（改行で区切られている）
	MakeDir("./Saves");
	dirlist(filelist, 2048, "./" SAVE_DIR "\\", "*" KAKUCYOSI);

	//前回動的配列が確保されていたら開放
	if (GameFilenames != NULL)
	{
		delsplit(Filename_n, GameFilenames);
		DelArray1Dim(filepoint);
		DelArray2Dim(fileusername, Filename_n);
	}
	//ゲームデータ一覧取得した文字列において、
	//改行文字で区切って分解する
	Filename_n = split(filelist, "\n", &GameFilenames);
	Filename_n--;

	//動的確保
	GetArray1Dim(int, filepoint, Filename_n);
	GetArray2Dim(char, fileusername, Filename_n, 16);

	//ファイル数だけ繰り返す
	for (i = 0; i < Filename_n; i++)
	{
		//ファイルが存在したら開く
		sprintf_s(buf, 256, "./Saves/%s", GameFilenames[i]);
		fopen_s(&fp, buf, "rb");
		if (fp != NULL)
		{
			//ユーザー名をファイル名から抽出（拡張子を除く作業）
			strcpy_s(username, 256, GameFilenames[i]);
			for (j = strlen(username); j >= 0 && username[j] != '.'; j--);
			if (j >= 0 && j < 16)
				username[j] = '\0';
			else
				username[15] = '\0';
			strcpy_s(fileusername[i], 16, username);

			//ファイルの先頭にカーソルを合わせる
			fseek(fp, 0, SEEK_SET);

			//ファイル中のポイントが格納されている場所にカーソルを合わせる
			fseek(fp, sizeof(int), SEEK_CUR);

			//ポイントを読みだす
			fread(&filepoint[i], sizeof(int), 1, fp);

			//ファイルを閉じる
			fclose(fp);
		}
	}
}

