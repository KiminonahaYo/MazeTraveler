/*ドット文字描画関係*/

#ifndef DISP_MOJI_DEFINED

#define DISP_MOJI_DEFINED

#include "DrawMoji_set.h"

#define DOTSIZE_STDXandY 10

void DispCharOfBox(char, int, int);
void DispStringOfBox(int, int, double, int, char*, ...);

void SetColorForBox(int);
void SetPosForBox(int, int);
void SetPerSizeForBox(double);
void Box(int, int, int);

int _col = 0;
int _box_x = 0;
int _box_y = 0;
double _persize = 1.0;

void SetPerSizeForBox(double size)
{
	if (size <= 0.0) return;
	_persize = size;
}

void SetPosForBox(int posx, int posy)
{
	_box_x = posx;
	_box_y = posy;
}

//文字列の色と濃度を設定
void SetColorForBox(int col)
{
	if (col < 0 || col>9) return;
	_col = col;
}

void Box(int dotx, int doty)
{
	DrawExtendGraph(
		_box_x + dotx*int(DOTSIZE_STDXandY*_persize),
		_box_y + doty*int(DOTSIZE_STDXandY*_persize),
		_box_x + (dotx + 1)*int(DOTSIZE_STDXandY*_persize),
		_box_y + (doty + 1)*int(DOTSIZE_STDXandY*_persize),
		Handle_Dotpic[_col], false);
}

//文字一文字を表示
void DispCharOfBox(char moji, int x, int y)
{
	int xi, yi, mi;

	//文字（大文字小文字を区別して）探索
	for (mi = 0; mojidata[mi].moji != '\0'; mi++)
		if (mojidata[mi].moji == moji) break;

	//収録外の文字ならば表示しない。
	if (mojidata[mi].moji == '\0')
	{
		return;
	}

	//文字を描画
	for (xi = 0; xi < MOJI_XSIZE; xi++)
	{
		for (yi = 0; yi < MOJI_YSIZE; yi++)
		{
			if (mojidata[mi].value[yi][xi])
				Box(x + xi, y + yi);
		}
	}
}

//文字列を表示
void DispStringOfBox(int x, int y, double persize, int col, char* format, ...)
{
	int i;
	int xi = 0;
	int yi = 0;
	va_list list;
	char str[1024];
	bool colorfulflag = false;

	//可変引数からprintf系列の書式で設定する。
	va_start(list, format);
	vsprintf_s(str, 1024, format, list);
	va_end(list);

	//初期化する。
	SetPosForBox(x, y);
	SetPerSizeForBox(persize);
	if (col != -1)
		SetColorForBox(col);
	else
	{
		colorfulflag = true;
		col = 0;
	}

	//文字を表示する。
	for (i = 0; str[i] != '\0'; i++)
	{
		switch (str[i])
		{
			//改行
			case '\n':
				xi = x;
				yi += MOJI_YSIZE + 1;
				break;

			//スペース
			case ' ':
				xi += MOJI_XSIZE + 1;
				break;

			//通常の文字
			default:
				if (colorfulflag)	SetColorForBox(col);
				DispCharOfBox(str[i], xi, yi);
				if (colorfulflag)	col = (col + 1) % 10;
				xi += MOJI_XSIZE + 1;
				break;
		}
	}
}

#endif