#ifndef DISPHELP_DEFINED

#define DISPHELP_DEFINED

#include "Defines.h"
#include "BackGround.h"

int disp_help(void);
void disp_navi(bool);
void disp_navi_set(char *);

//ナビゲーション
int navi_page = -1;

//ヘルプページを表示
int disp_help(void)
{
	static int page = 0;

	bool brkflag = false;
	bool endflag = false;

	int i;

	SPLITDATA spldata;
	int spl_n;

	spl_n = split(help_text_spl[page], "\n", &spldata);

	while (1)
	{
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE))
			brkflag = true;

		if (brkflag)
			break;

		if (endflag)
			break;

		SetDrawScreen(Handle_Screen_Normal);

		ClearDrawScreen();

		DrawBackGround(SCREEN_X, SCREEN_Y, 2);
		DrawGraph(12, 60, Handle_Help_Back, TRUE);

		if (Stick(KEY_INPUT_LEFT, true, 5))
		{
			if (page > 0)
			{
				page--;
			}
			else
			{
				page = help_text_spl_n - 1;
			}

			delsplit(spl_n, spldata);
			spl_n = split(help_text_spl[page], "\n", &spldata);
		}
		if (Stick(KEY_INPUT_RIGHT, true, 5))
		{
			if (page < help_text_spl_n - 1)
			{
				page++;
			}
			else
			{
				page = 0;
			}

			delsplit(spl_n, spldata);
			spl_n = split(help_text_spl[page], "\n", &spldata);
		}
		if (Stick(KEY_INPUT_RETURN))
		{
			if (page < help_text_spl_n - 1)
			{
				page++;
			}
			else
			{
				page = 0;
			}

			delsplit(spl_n, spldata);
			spl_n = split(help_text_spl[page], "\n", &spldata);
		}
		if (Stick(KEY_INPUT_BACK))
		{
			endflag = true;
		}
		if (Stick(KEY_INPUT_F1))
		{
			endflag = true;
		}

		SetFontSize(36);

		DrawFormatString(SCREEN_X / 2 - 180, 20, GetColor(255, 50, 50), "Maze Traveler ヘルプ");

		for (i = 0; i < spl_n; i++)
		{
			DrawFormatString(32, 72 + 45 * i, GetColor(0, 0, 0), "%s", spldata[i]);
		}

		DrawFormatString(SCREEN_X / 2 - 250, SCREEN_Y - 100, GetColor(50, 50, 150), "←　　%d / %d ページ　　→", page + 1, help_text_spl_n);
		DrawFormatString(20, SCREEN_Y - 40, GetColor(50, 50, 150), "Enterで次に進む、BackSpace, F1でヘルプを閉じる");

		SetDrawScreen(DX_SCREEN_BACK);

		DrawGraph(0, 0, Handle_Screen_Normal, false);

		ScreenFlip();
	}

	delsplit(spl_n, spldata);

	SetDrawScreen(DX_SCREEN_BACK);

	if (brkflag)
		return -1;
	else
		return 0;
}

//ナビゲーションを表示するためにセットする
//テキストファイルにあるインデックス文字列を指定して表示する
void disp_navi_set(char *indexname)
{
	SPLITDATA spldata;
	SPLITDATA spldata2;
	int spl_n;
	int spl_n2;

	int i;

	for (i = 0; i < navi_text_spl_n; i++)
	{
		spl_n  = split(navi_text_spl[i], "\n", &spldata);
		spl_n2 = split(spldata[0], "'", &spldata2);

		if (strcmp(spldata2[0], indexname) == 0)
		{
			break;
		}

		delsplit(spl_n, spldata);
		delsplit(spl_n2, spldata2);
	}

	if (i == navi_text_spl_n)
		navi_page = -1;
	else
	{
		delsplit(spl_n, spldata);
		delsplit(spl_n2, spldata2);

		navi_page = i;
	}
}

void disp_navi(bool underflag = true)
{
	static int framecnt = 0;
	static int before_page = -1;

	bool brkflag = false;
	bool endflag = false;

	int i;
	int kankaku;

	SPLITDATA spldata;
	SPLITDATA spldata2;
	int spl_n;
	int spl_n2;

	if (navi_page != before_page)
	{
		if (navi_page != -1)
			framecnt = 0;
		before_page = navi_page;
	}

	if (navi_page != -1)
	{
		spl_n = split(navi_text_spl[navi_page], "\n", &spldata);
		spl_n2 = split(spldata[0] , "'", &spldata2);
		kankaku = (250 - 30) / (spl_n - 1);

		if (underflag == false)
		{
			DrawGraph(SCREEN_X - 400, SCREEN_Y / 2 - 125, Handle_Navigate_Back, TRUE);

			SetFontSize(15);

			DrawFormatString(SCREEN_X - 400 + 15, SCREEN_Y / 2 - 125 + 0 * kankaku + 15, 0xFF0000, "ナビゲーション");
			DrawFormatString(SCREEN_X - 400 + 15, SCREEN_Y / 2 - 125 + 1 * kankaku + 15, 0x0000FF, "%s", spldata2[1]);
			for (i = 1; i < spl_n; i++)
			{
				DrawFormatString(SCREEN_X - 400 + 15, SCREEN_Y / 2 - 125 + (i + 1) * kankaku + 15, STR_COLOR_2, "%s", spldata[i]);
			}
		}
		else
		{
			DrawGraph(SCREEN_X - 400, SCREEN_Y - 250, Handle_Navigate_Back, TRUE);

			SetFontSize(15);

			DrawFormatString(SCREEN_X - 400 + 15, SCREEN_Y - 250 + 0 * kankaku + 15, 0xFF0000, "ナビゲーション");
			DrawFormatString(SCREEN_X - 400 + 15, SCREEN_Y - 250 + 1 * kankaku + 15, 0x0000FF, "%s", spldata2[1]);
			for (i = 1; i < spl_n; i++)
			{
				DrawFormatString(SCREEN_X - 400 + 15, SCREEN_Y - 250 + (i + 1) * kankaku + 15, STR_COLOR_2, "%s", spldata[i]);
			}
		}

		delsplit(spl_n, spldata);
		delsplit(spl_n2, spldata2);
	}

	if (navi_page != -1)
	{
		if (framecnt < 60 * 7)
			framecnt++;
		else
			navi_page = -1;
	}
}

#endif