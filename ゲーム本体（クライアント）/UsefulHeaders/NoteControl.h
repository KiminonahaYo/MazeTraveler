#ifndef NOTECONTROL_DEFINED

#include <stdio.h>
#include <string.h>

//�O���[�o���ϐ�
char* Buf = NULL;
int BufSize;

//�v���g�^�C�v�錾
void Notesel(char[], int);

int Noteadd(int, const char*, ...);
int Noteget(char[], int);
int Notedel(int);

int Noteload(char*);
int Notesave(char*);

int GetNoteLinelen(void);

//�������m�[�g�p�b�h�Ƃ��Ďg���z����w��
void Notesel(char NoteBuf[], int NoteBufSize)
{
	Buf = NoteBuf;
	BufSize = NoteBufSize;
	strcpy_s(Buf, BufSize, "");
	return;
}

//�s�ɕ������ǉ�
//�s�ԍ���0����n�܂�
int Noteadd(int LineNo, const char *format, ...)
{
	if (Buf != NULL)
	{
		int slen;
		int i;
		int cnt, LineStart;
		int Blen;
		char Str[1024];
		va_list list;

		va_start(list, format);
		vsprintf_s(Str, 1024, format, list);
		va_end(list);

		slen = strlen(Str);
		Blen = strlen(Buf);
		cnt = 0;
		for (i = 0; cnt < LineNo || (LineNo == -1 && i < Blen); i++)
		{
			if (LineNo != -1 && Buf[i] == '\0') return -1;
			if (Buf[i] == '\n') cnt++;
		}
		LineStart = i;

		for (i = Blen; i >= LineStart; i--)
		{
			Buf[i + slen] = Buf[i];
		}
		for (i = 0; i < slen; i++)
		{
			Buf[LineStart + i] = Str[i];
		}

		return 0;
	}
	else return -1;
}

//�s���當�����ǂݍ���
int Noteget(char Buf[], int LineNo)
{
	if (Buf != NULL)
	{
		int i;
		int cnt, LineStart;

		cnt = 0;
		for (i = 0; cnt < LineNo; i++)
		{
			if (Buf[i] == '\0') return -1;
			if (Buf[i] == '\n') cnt++;
		}
		LineStart = i;

		for (i = 0; Buf[i + LineStart] != '\n'; i++)
		{
			Buf[i] = Buf[i + LineStart];
		}
		Buf[i] = '\0';

		return 0;
	}
	else return -1;
}

//�s�̕�������폜
int Notedel(int LineNo)
{
	if (Buf != NULL)
	{
		int i;
		int cnt, LineStart, Linelen;
		int Blen;

		Blen = strlen(Buf);
		cnt = 0;
		for (i = 0; cnt < LineNo; i++)
		{
			if (Buf[i] == '\0') return -1;
			if (Buf[i] == '\n') cnt++;
		}
		LineStart = i;

		for (i = 0; Buf[i + LineStart] != '\n'; i++);
		Linelen = i + 1;

		for (i = LineStart; i + Linelen <= Blen; i++)
		{
			Buf[i] = Buf[i + Linelen];
		}

		return 0;
	}
	else return -1;
}

//�s�����擾
int GetNoteLinelen(void)
{
	if (Buf != NULL)
	{
		int i, cnt = 0;

		for (i = 0; Buf[i] != '\0'; i++)
		{
			if (Buf[i] == '\n') cnt++;
		}
		return cnt;
	}
	else return -1;
}

//�e�L�X�g�t�@�C�����當�����ǂݍ���
int Noteload(char *Filename)
{
	if (Buf != NULL)
	{
		FILE *fp;
		char Line[256];

		fopen_s(&fp, Filename, "r");
		if (fp == NULL)
		{
			strcpy_s(Buf, BufSize, "");
		}
		else
		{
			while (fgets(Line, 256, fp) != NULL)
			{
				strcat_s(Buf, BufSize, Line);
			}
			fclose(fp);
		}
		return 0;
	}
	else return -1;
}

//��������e�L�X�g�t�@�C���ɕۑ�
int Notesave(char *Filename)
{
	if (Buf != NULL)
	{
		FILE *fp;

		fopen_s(&fp, Filename, "w");
		fprintf(fp, "%s", Buf);
		fclose(fp);
		return 0;
	}
	else return -1;
}

#define NOTECONTROL_DEFINED
#endif
