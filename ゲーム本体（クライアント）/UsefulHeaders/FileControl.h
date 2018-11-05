#ifndef FILECONTROL_DEFINED

#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <malloc.h>
#include <shellapi.h>
#include <windows.h>

#define FILE_PATH_LENGTH 256

#define DATAFILE 0
#define APPFILE 1

//�v���g�^�C�v�錾
int filecopy(char *, char *);
int filedel(char *, ...);
int MakeDir(char *, ...);
bool exist(char *, ...);
void ExecCmd(char *, int, char *);

void dirlist(char[], int, char *, char *);
char *GetCurName(void);

//�t�@�C���R�s�[
int filecopy(char *CopyMotoFileName, char *CopySakiFileName)
{
	FILE *MotoFilePtr;
	FILE *SakiFilePtr;
	unsigned char c;

	fopen_s(&MotoFilePtr, CopyMotoFileName, "rb");

	if (MotoFilePtr != NULL && strcmp(CopyMotoFileName, CopySakiFileName) != 0)
	{
		fopen_s(&SakiFilePtr, CopySakiFileName, "wb");

		if (SakiFilePtr != NULL)
		{
			while (feof(MotoFilePtr)==0)
			{
				fread(&c, sizeof(unsigned char), 1, MotoFilePtr);
				fwrite(&c, sizeof(unsigned char), 1, SakiFilePtr);
			}

			//�t�@�C���|�C���^��n��
			fclose(SakiFilePtr);
		}

		//�t�@�C���|�C���^��n��
		fclose(MotoFilePtr);

		return 0;
	}
	else
	{
		return -1;
	}
}

int filedel(char *filenameFormat, ...)
{
	va_list list;
	char buf[256];

	va_start(list, filenameFormat);
	vsprintf_s(buf, 256, filenameFormat, list);
	va_end(list);

	return remove(buf);
}

int MakeDir(char *DirNameFormat, ...)
{
	va_list list;
	char buf[256];

	va_start(list, DirNameFormat);
	vsprintf_s(buf, 256, DirNameFormat, list);
	va_end(list);

	return _mkdir(buf);
}

bool exist(char *FileNameFormat, ...)
{
	char buf[256];
	FILE *fp;
	va_list list;

	va_start(list, FileNameFormat);
	vsprintf_s(buf, 256, FileNameFormat, list);
	va_end(list);

	fopen_s(&fp, buf, "r");
	if (fp == NULL){
		return false;
	}
	else{
		fclose(fp);
		return true;
	}
}

void ExecCmd(char *command, int Mode, char *param = NULL)
{
	if (Mode)	ShellExecute(NULL, NULL, command, NULL, NULL, SW_SHOWNORMAL);
	else		ShellExecute(NULL, "open", command, param, NULL, SW_SHOWNORMAL);
}

void dirlist(char Dest[], int destsize, char *FindDir, char *FindFile)
{
	WIN32_FIND_DATA find;
	HANDLE hFind;
	TCHAR szPath[MAX_PATH];
	LPTSTR p;

	//GetModuleFileName(NULL, szPath, MAX_PATH);	//���s�t�@�C���̂���p�X���擾
	lstrcpy(szPath, FindDir);

	p = _tcsrchr(szPath, _T('\\'));					//�Ō��\�}�[�N�����o
	*p = '\0';										//�Ō��\�}�[�N������
	lstrcat(szPath, TEXT("\\"));
	lstrcat(szPath, TEXT(FindFile));

	hFind = FindFirstFile(szPath, &find);			//���s�t�@�C���̂���f�B���N�g��������

	strcpy_s(Dest, destsize, "");

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			//���݂̃f�B���N�g��(.)��O�̃f�B���N�g��(..)���p�X�Ƃ��ďo�Ă��܂��̂ŏ��O
			if (lstrcmpi(find.cFileName, TEXT(".")) != 0 && lstrcmpi(find.cFileName, TEXT("..")) != 0)
			{
				//printfDx("%s\n", find.cFileName);	//�����ŏo��
				sprintf_s(Dest, destsize, "%s%s\n", Dest, find.cFileName);
			}
		} while (FindNextFile(hFind, &find));

		FindClose(hFind);
	}
}

char *GetCurName(void)
{
	static TCHAR szPath[MAX_PATH];

	GetModuleFileName(NULL, szPath, MAX_PATH);	//���s�t�@�C���̂���p�X���擾

	return szPath;
}

#define FILECONTROL_DEFINED
#endif
