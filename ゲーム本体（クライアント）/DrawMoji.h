/*�h�b�g�����`��֌W*/

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

//������̐F�ƔZ�x��ݒ�
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

//�����ꕶ����\��
void DispCharOfBox(char moji, int x, int y)
{
	int xi, yi, mi;

	//�����i�啶������������ʂ��āj�T��
	for (mi = 0; mojidata[mi].moji != '\0'; mi++)
		if (mojidata[mi].moji == moji) break;

	//���^�O�̕����Ȃ�Ε\�����Ȃ��B
	if (mojidata[mi].moji == '\0')
	{
		return;
	}

	//������`��
	for (xi = 0; xi < MOJI_XSIZE; xi++)
	{
		for (yi = 0; yi < MOJI_YSIZE; yi++)
		{
			if (mojidata[mi].value[yi][xi])
				Box(x + xi, y + yi);
		}
	}
}

//�������\��
void DispStringOfBox(int x, int y, double persize, int col, char* format, ...)
{
	int i;
	int xi = 0;
	int yi = 0;
	va_list list;
	char str[1024];
	bool colorfulflag = false;

	//�ψ�������printf�n��̏����Őݒ肷��B
	va_start(list, format);
	vsprintf_s(str, 1024, format, list);
	va_end(list);

	//����������B
	SetPosForBox(x, y);
	SetPerSizeForBox(persize);
	if (col != -1)
		SetColorForBox(col);
	else
	{
		colorfulflag = true;
		col = 0;
	}

	//������\������B
	for (i = 0; str[i] != '\0'; i++)
	{
		switch (str[i])
		{
			//���s
			case '\n':
				xi = x;
				yi += MOJI_YSIZE + 1;
				break;

			//�X�y�[�X
			case ' ':
				xi += MOJI_XSIZE + 1;
				break;

			//�ʏ�̕���
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