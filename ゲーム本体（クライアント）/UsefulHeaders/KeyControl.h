//DxLib��p

#ifndef KEYCONTROL_DEFINED

#include "MyMath.h"

bool Stick(int, bool, int);			//�L�[�������ꂽ�u�Ԃɑ�����󂯕t����
bool StickMouse(int, bool, int);	//�}�E�X���N���b�N���ꂽ�u�Ԃɑ�����󂯕t����

bool GetKey(int);
int GetMouse(int);					//�}�E�X�N���b�N��Ԓ���

bool GetKey_any_push(void);			//������������Ă���Ԃ����m����
bool Stick_any_push(void);			//�����������ꂽ�u�Ԃ����m����B

int Key_PressedCount(int);			//�������A�������u�Ԃɗ����Ă���/�����Ă������Ԃ��ǂꂭ�炢���Ԃ��֐�
int Mouse_PressedCount(int);		//�N���b�N�����A�������u�Ԃɗ����Ă���/�N���b�N���Ă������Ԃ��ǂꂭ�炢���Ԃ��֐�

int m_inputnum(void);				//�����݂̂���͂���B
char m_inputalpha(void);			//�p�����݂̂���͂���B

//�L�[���͊֐�

//������������Ă���Ԃ����m����
bool GetKey_any_push(void)
{
	//�L�[���͏���ÓI�m��
	static char PushedKey[256];
	static int  PushCount[256];
	bool ret;

	int i;

	GetHitKeyStateAll(PushedKey);

	for (i = 0; i < 256; i++)
	{
		if (PushedKey[i] != 0) break;
	}
	if (i < 256)	ret = true;
	else			ret = false;

	return ret;
}

//�����������ꂽ�u�Ԃ����m����
bool Stick_any_push(void)
{
	bool pushedkey = GetKey_any_push();
	static bool pushedflag = true;
	bool ret;

	if (pushedkey == true)
	{
		if (pushedflag == false)
		{
			pushedflag = true;
			ret = true;
		}
		else
		{
			ret = false;
		}
	}
	else
	{
		pushedflag = false;
		ret = false;
	}

	return ret;
}

//�L�[���������u�Ԃɑ�����󂯕t����
bool Stick(int KeyCode, bool ForSelectStickFlag = 0, int Select_kirikae_kankaku = 3)
{
	//�L�[���͏���ÓI�m��
	static char PushedKey[256];
	static int  PushCount[256];
	bool ret;

	//�L�[�������ꂽ��
	if (GetKey(KeyCode))
	{
		PushCount[KeyCode]++;

		//�L�[���������u�Ԃ܂��͉������ςȂ��ő���ł��郂�[�h�̂Ƃ�
		if (PushedKey[KeyCode] == 0 || (ForSelectStickFlag && (PushCount[KeyCode] == 1 || PushCount[KeyCode] >= 30)))
		{
			//�L�[�����������ǂ����̃t���O�𗧂Ă�
			PushedKey[KeyCode] = 1;

			//�������ςȂ��ő���ł��郂�[�h�̂Ƃ�
			if (ForSelectStickFlag && PushCount[KeyCode] >= 30)
			{
				if (PushCount[KeyCode] % Select_kirikae_kankaku == 0)
					ret = true;
				else
					ret = false;
			}
			//�����ꂽ�u�Ԃ̂Ƃ�
			else
			{
				ret = true;
			}
		}
		//�������u�ԈȊO�̂Ƃ�
		else
		{
			ret = false;
		}
	}
	//�L�[��������Ă��Ȃ�������
	else
	{
		PushCount[KeyCode] = 0;
		PushedKey[KeyCode] = 0;
		ret = false;
		//return false;
	}
	return ret;
}

bool GetKey(int KeyCode)
{
	bool ret = CheckHitKey(KeyCode)!= 0 ? true : false;

	return ret;
}

//�L�[�������ė����ꂽ/������ĉ����ꂽ�u�Ԃɉ�����/���������Ԃ�Ԃ��֐�
int Key_PressedCount(int KeyCode)
{
	//�L�[���͏���ÓI�m��
	static bool PushFlag[256];
	static int  PushCount[256];
	int ret;

	//�����ꂽ��
	if (GetKey(KeyCode))
	{
		//�����ꂽ�u�ԁA�����ꂽ���ԁi�}�C�i�X�j��Ԃ�
		if (PushFlag[KeyCode] == false)
		{
			ret = PushCount[KeyCode];
			PushFlag[KeyCode] = true;
			PushCount[KeyCode] = 0;
		}
		//����ȊO�̓_�~�[��Ԃ��ăv���X�J�E���g
		else
		{
			ret = 0;
			PushFlag[KeyCode] = true;
			PushCount[KeyCode]++;
		}
	}
	//�����ꂽ��
	else
	{
		//�����ꂽ�u�ԁA���������ԁi�v���X�j��Ԃ�
		if (PushFlag[KeyCode] == true)
		{
			ret = PushCount[KeyCode];
			PushFlag[KeyCode] = false;
			PushCount[KeyCode] = 0;
		}
		//����ȊO�̓_�~�[��Ԃ��ă}�C�i�X�J�E���g
		else
		{
			ret = 0;
			PushFlag[KeyCode] = false;
			PushCount[KeyCode]--;
		}
	}
	return ret;
}

//�}�E�X���N���b�N���ė�����/�����ăN���b�N�����u�ԂɃN���b�N����/���������Ԃ�Ԃ��֐�
int Mouse_PressedCount(int KeyCode)
{
	//�L�[���͏���ÓI�m��
	static bool PushFlag[8];
	static int  PushCount[8];
	int ret;

	//�}�E�X�̃L�[�R�[�h��2�̏搔�ɂȂ��Ă��邽�߁Alog�֐����g��
	int Key = (int)(log((double)KeyCode) / log(2.0));

	//�N���b�N���ꂽ��
	if (GetMouse(KeyCode))
	{
		//�N���b�N���ꂽ�u�ԁA�����ꂽ���ԁi�}�C�i�X�j��Ԃ�
		if (PushFlag[Key] == false)
		{
			ret = PushCount[Key];
			PushFlag[Key] = true;
			PushCount[Key] = 0;
		}
		//����ȊO�̓_�~�[��Ԃ��ăv���X�J�E���g
		else
		{
			ret = 0;
			PushFlag[Key] = true;
			PushCount[Key]++;
		}
	}
	//�����ꂽ��
	else
	{
		//�����ꂽ�u�ԁA�N���b�N�������ԁi�v���X�j��Ԃ�
		if (PushFlag[Key] == true)
		{
			ret = PushCount[Key];
			PushFlag[Key] = false;
			PushCount[Key] = 0;
		}
		//����ȊO�̓_�~�[��Ԃ��ă}�C�i�X�J�E���g
		else
		{
			ret = 0;
			PushFlag[Key] = false;
			PushCount[Key]--;
		}
	}
	return ret;
}

//�}�E�X���N���b�N�����u�Ԃɑ�����󂯕t����
bool StickMouse(int KeyCode, bool ForSelectStickFlag = 0, int Select_kirikae_kankaku = 3)
{
	static char PushedKey[8];
	static int  PushCount[8];
	bool ret;

	//�}�E�X�̃L�[�R�[�h��2�̏搔�ɂȂ��Ă��邽�߁Alog�֐����g��
	int Key = (int)(log((double)KeyCode) / log(2.0));

	//�}�E�X���N���b�N����Ă���Ƃ�
	if (GetMouse(KeyCode))
	{
		PushCount[Key]++;

		//�}�E�X���N���b�N���ꂽ�u�Ԃ܂��̓}�E�X���N���b�N�����܂܂ł�����ł��郂�[�h�̂Ƃ�
		if (PushedKey[Key] == 0 || (ForSelectStickFlag && (PushCount[Key] == 1 || PushCount[Key] >= 30)))
		{
			PushedKey[Key] = 1;

			//�}�E�X���N���b�N�����܂܂ł�����ł��郂�[�h�̂Ƃ�
			if (ForSelectStickFlag && PushCount[Key] >= 30)
			{
				if (PushCount[Key] % Select_kirikae_kankaku == 0)
					ret = true;
				else
					ret = false;
			}
			//�}�E�X���N���b�N���ꂽ�u�Ԃ̂Ƃ�
			else
			{
				ret = true;
			}
		}
		//�}�E�X���N���b�N���ꂽ�u�ԈȊO�̂Ƃ�
		else
		{
			ret = false;
		}
	}
	//�}�E�X���N���b�N����Ă��Ȃ��Ƃ�
	else
	{
		PushCount[Key] = 0;
		PushedKey[Key] = 0;
		ret = false;
	}

	return ret;
}

//�}�E�X�̃N���b�N��Ԃ𒲂ׂ�֐�
int GetMouse(int KeyCode)
{
	int InputState = GetMouseInput();
	int KeyFlag = InputState & KeyCode;

	return KeyFlag;
}

//�������͊֐�
//�����݂̂���͂���
int m_inputnum(void)
{
	char c = GetInputChar(true);

	if(c>='0' && c<='9')
	{
		return c - '0';
	}
	else if(c == CTRL_CODE_BS)
	{
		return 10;
	}
	else
	{
		while (GetInputChar(true) != 0);
		return -1;
	}
}

//�p�����݂̂���͂���
char m_inputalpha(void)
{
	char c = GetInputChar(true);

	if ((c >= 'A' && c <= 'Z') ||(c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == CTRL_CODE_BS)
	{
		return c;
	}
	else
	{
		while (GetInputChar(true) != 0);
		return -1;
	}
}

#define KEYCONTROL_DEFINED
#endif
