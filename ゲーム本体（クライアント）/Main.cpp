//�w�b�_�[�t�@�C���ǂݍ���
#include "Defines.h"
#include "BackGround.h"
#include "demo.h"
#include "play.h"
#include "DrawMoji.h"
#include "MyMaze.h"
#include "disphelp.h"

void LoadGameFileLists(void);		//�Q�[���t�@�C���ꗗ�擾
void loadmedia(void);				//�O���t�@�C���ǂݍ���
void init(void);					//�v���O�����̏���
void dest(void);					//�v���O�����̌�n��
void GameMain(void);				//�Q�[�����C���֐�
int filemenu(void);

SPLITDATA GameFilenames = NULL;		//�Q�[���t�@�C���ꗗ
char **fileusername = {NULL};		//�Q�[���t�@�C���̃��[�U�[��
int *filepoint = NULL;				//�Q�[���t�@�C���̓��_
int Filename_n = 0;					//�Q�[���t�@�C����

//���C���֐�
int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	init();

	GameMain();

	dest();

	return 0;
}

//�Q�[�����C���֐�
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

		//�^�C�g����ʂ�\������B
		ret = Title.Disp();
		if (ret == -1)
		{
			brkflag = true;
			continue;
		}

		//�t�@�C�����j���[��\������B
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
			//�L�[�R���g���[�����i���암�j
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
					//�����t�@�C���������͂��ꂽ�A�������̃t�@�C�������ɑ��݂��Ȃ��ꍇ
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
						//�����t�@�C���������͂��ꂽ�A�������̃t�@�C�������ɑ��݂��Ȃ��ꍇ
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
					//�t�@�C���I�[�v�����ăv���C
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

		//�\����

		SetFontSize(15);
		DrawFormatString(10, SCREEN_Y - 20, STR_COLOR_2, "%d FPS", GetFPS());

		SetFontSize(20);

		pos_no = space_no = 0;
		
		//���j���[���ڂ̍���y���W�����߂�
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

		//���j���[���ڂ̍���y���W�͒ʏ��100�Ƃ���B
		if (pos_no_cur * 30 + space_no_cur * 20 < 550)
			top_position = 100;
		else
			top_position = 100 - (pos_no_cur * 30 + space_no_cur * 20 - 540);

		//���[�U�[��\������B�i�ʏ�A�R�s�[�A�폜�A�V�K�t�@�C���\���̎��ɕ\�������B�j
		if (delchk == false && copystep == 0)
		{
			for (i = 0; i < Filename_n; i++)
			{
				if (filepoint[i] <= 2000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, i == fileno, S_BLUE, 650, "���[�U�[%02d. %s %dP ���S�� %s%s", i + 1, fileusername[i], filepoint[i], delflag ? "���폜����" : "", copyflag ? "���R�s�[����" : "");
				}
				else if (filepoint[i] <= 5000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, i == fileno, S_GREEN, 650, "���[�U�[%02d. %s %dP ������ %s%s", i + 1, fileusername[i], filepoint[i], delflag ? "���폜����" : "", copyflag ? "���R�s�[����" : "");
				}
				else if (filepoint[i] <= 10000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, i == fileno, S_YELLOW, 650, "���[�U�[%02d. %s %dP �㋉�� %s%s", i + 1, fileusername[i], filepoint[i], delflag ? "���폜����" : "", copyflag ? "���R�s�[����" : "");
				}
				else if (filepoint[i] <= 20000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, i == fileno, S_RED, 650, "���[�U�[%02d. %s %dP ���l %s%s", i + 1, fileusername[i], filepoint[i], delflag ? "���폜����" : "", copyflag ? "���R�s�[����" : "");
				}
				else
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, i == fileno, S_PURPLE, 650, "���[�U�[%02d. %s %dP �}�X�^�[ %s%s", i + 1, fileusername[i], filepoint[i], delflag ? "���폜����" : "", copyflag ? "���R�s�[����" : "");
				}

				pos_no++;
			}
		}

		//�폜���j���[
		if (delflag)
		{
			//�폜�t�@�C���̑I����ʂ�\������B�i���[�U�[�͏����A��̃v���O�����ő�p�j
			if (delchk == false)
			{
				DispStringOfBox(10, 10, 0.7, -1, "MAZE TRAVELER  DELETE FILE SELECT");

				if (fileno < Filename_n)
					DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "%02d. %s %dP", fileno + 1, fileusername[fileno], filepoint[fileno]);
				else
					DispStringOfBox(10, SCREEN_Y - 70, 0.8, STATUSMES_COL, "BACK TO FILE SELECT");

				space_no++;

				dispselect(top_position + 30 * pos_no + 20 * space_no, fileno == Filename_n, S_BLUE, 650, "�߂�");
			}
			//�폜�t�@�C����I��������ɍ폜�̌x�����o���B
			else
			{
				DispStringOfBox(10, 10, 0.7, -1, "MAZE TRAVELER  DO YOU DELETE IT?");

				//���[�U�[�̕\��
				if (filepoint[fileno] <= 2000)
				{
					DrawGraph(SCREEN_X / 2 - 320, SCREEN_Y / 4, Handle_SelectTile_Blue, false);
					DrawFormatString(SCREEN_X / 2 - 320 + 10, SCREEN_Y / 4 + 10, STR_COLOR_2, "���[�U�[%02d. %s %dP ���S��", fileno + 1, fileusername[fileno], filepoint[fileno]);
				}
				else if (filepoint[fileno] <= 5000)
				{
					DrawGraph(SCREEN_X / 2 - 320, SCREEN_Y / 4, Handle_SelectTile_Green, false);
					DrawFormatString(SCREEN_X / 2 - 320 + 10, SCREEN_Y / 4 + 10, STR_COLOR_2, "���[�U�[%02d. %s %dP ������", fileno + 1, fileusername[fileno], filepoint[fileno]);
				}
				else if (filepoint[fileno] <= 10000)
				{
					DrawGraph(SCREEN_X / 2 - 320, SCREEN_Y / 4, Handle_SelectTile_Yellow, false);
					DrawFormatString(SCREEN_X / 2 - 320 + 10, SCREEN_Y / 4 + 10, STR_COLOR_2, "���[�U�[%02d. %s %dP �㋉��", fileno + 1, fileusername[fileno], filepoint[fileno]);
				}
				else if (filepoint[fileno] <= 20000)
				{
					DrawGraph(SCREEN_X / 2 - 320, SCREEN_Y / 4, Handle_SelectTile_Red, false);
					DrawFormatString(SCREEN_X / 2 - 320 + 10, SCREEN_Y / 4 + 10, STR_COLOR_2, "���[�U�[%02d. %s %dP ���l", fileno + 1, fileusername[fileno], filepoint[fileno]);
				}
				else
				{
					DrawGraph(SCREEN_X / 2 - 320, SCREEN_Y / 4, Handle_SelectTile_Purple, false);
					DrawFormatString(SCREEN_X / 2 - 320 + 10, SCREEN_Y / 4 + 10, STR_COLOR_2, "���[�U�[%02d. %s %dP �}�X�^�[", fileno + 1, fileusername[fileno], filepoint[fileno]);
				}

				//�{���ɍ폜���܂����H�iYes/No)��\������B
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
		//�R�s�[���j���[
		else if (copyflag)
		{
			//�R�s�[���t�@�C���̑I����ʂ�\������B�i���[�U�[�\���������A��̃v���O�����ő�p����B�j
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

				dispselect(top_position + 30 * pos_no + 20 * space_no, fileno == Filename_n, S_BLUE, 650, "�߂�");
			}
			//�R�s�[��t�@�C���̃t�@�C��������͂����ʂ�\������B
			else if (copystep == 1)
			{
				DispStringOfBox(10, 10, 0.7, -1, "MAZE TRAVELER  COPYNAME INPUT");

				//���[�U�[��\������B
				if (filepoint[copyfromno] <= 2000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, false, S_BLUE, 650, "�R�s�[�� : ���[�U�[%02d. %s %dP ���S��", copyfromno + 1, fileusername[copyfromno], filepoint[copyfromno]);
				}
				else if (filepoint[copyfromno] <= 5000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, false, S_GREEN, 650, "�R�s�[�� : ���[�U�[%02d. %s %dP ������", copyfromno + 1, fileusername[copyfromno], filepoint[copyfromno]);
				}
				else if (filepoint[copyfromno] <= 10000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, false, S_YELLOW, 650, "�R�s�[�� : ���[�U�[%02d. %s %dP �㋉��", copyfromno + 1, fileusername[copyfromno], filepoint[copyfromno]);
				}
				else if (filepoint[copyfromno] <= 20000)
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, false, S_RED, 650, "�R�s�[�� : ���[�U�[%02d. %s %dP ���l", copyfromno + 1, fileusername[copyfromno], filepoint[copyfromno]);
				}
				else
				{
					dispselect(top_position + 30 * pos_no + 20 * space_no, false, S_PURPLE, 650, "�R�s�[�� : ���[�U�[%02d. %s %dP �}�X�^�[", copyfromno + 1, fileusername[copyfromno], filepoint[copyfromno]);
				}
				pos_no++;

				space_no++;

				//���͉��
				dispselect(top_position + 30 * pos_no + 20 * space_no, fileno == 0, S_BLUE, 650, "�R�s�[�t�@�C�����F%s", newfilename);

				//�X�e�[�^�X�\���ƃJ�[�\��
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

				dispselect(top_position + 30 * pos_no + 20 * space_no, fileno == 1, S_PURPLE, 650, "�߂�");
			}
		}
		//�V�K�t�@�C���쐬��ʁi���[�U�[�\���͏����A��̃v���O�����ő�p�j
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
			dispselect(top_position + 30 * pos_no + 20 * space_no, fileno == Filename_n, S_BLUE, 650, "���[�U�[�� : %s", newfilename);
			pos_no++;

			space_no++;
			dispselect(top_position + 30 * pos_no + 20 * space_no, fileno == Filename_n + 1, S_PURPLE, 650, "�߂�");

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
		//�ʏ���
		else
		{
			DispStringOfBox(10, 10, 0.7, -1, "MAZE TRAVELER  OPENFILE SELECT");

			//�X�e�[�^�X�\��
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

			//���[�U�[�\�����������̑��̃I�v�V����
			space_no++;

			dispselect(top_position + 30 * pos_no + 20 * space_no, ctrlflag == 1, S_BLUE, 650, "�V�������[�U�[���쐬");
			pos_no++;

			dispselect(top_position + 30 * pos_no + 20 * space_no, ctrlflag == 2, S_BLUE, 650, "���[�U�[�f�[�^���R�s�[");
			pos_no++;

			dispselect(top_position + 30 * pos_no + 20 * space_no, ctrlflag == 3, S_RED, 650, "���[�U�[���폜");
			pos_no++;

			space_no++;

			dispselect(top_position + 30 * pos_no + 20 * space_no, ctrlflag == 4, S_PURPLE, 650, "�^�C�g���ɖ߂�");
			pos_no++;

			dispselect(top_position + 30 * pos_no + 20 * space_no, ctrlflag == 5, S_PURPLE, 650, "�Q�[�����I������");
		}

		disp_navi();

		ScreenFlip();

		framecnt++;
	}

	//�����I���t���O�����������ɋ����I���̒l��Ԃ�
	if (brkflag)
		return -1;
	else
		return 0;
}

//�v���O�����̏���
void init(void)
{
	FILE *fp;

	//��ʃT�C�Y�ƃE�B���h�E���[�h�ݒ�
	SetGraphMode(SCREEN_X, SCREEN_Y, 32);
	ChangeWindowMode(true);										//�E�B���h�E���[�h�ŋN���A�R�����g�A�E�g�Ńt���X�N���[��
	SetDoubleStartValidFlag(true);
	SetMainWindowText(MES_TITLEBAR);
	SetWindowIconID(101);
	SetAlwaysRunFlag(true);
	SetOutApplicationLogValidFlag(false);

	//-------------------------------------------------------------------------------------------

	if (DxLib_Init() == -1) return exit(-1);					//DX���C�u����������

	SetFullSceneAntiAliasingMode(4, 2);							// �t���V�[���A���`�G�C���A�X��ݒ�A�����x���Ȃ�܂������ꂢ�ɕ\������܂��B

	SetBackgroundColor(0, 0, 0);								//�w�i�F
	SetDrawScreen(DX_SCREEN_BACK);								//����ʂɕ`��

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

//�v���O�����̌�n��
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

//�O���t�@�C���ǂݍ���
void loadmedia(void)
{
	Handle_Jiki_Stopping		= MV1LoadModel("Model/Jiki/Jiki_Alpha_Stopping.mqo");		//���@��~��
	Handle_Jiki_Moving_Body		= MV1LoadModel("Model/Jiki/Jiki_Alpha_Moving_Body.mqo");	//���@�ړ����̑�
	Handle_Jiki_Moving_Foot		= MV1LoadModel("Model/Jiki/Jiki_Alpha_Moving_Foot.mqo");	//���@�ړ����̑�
	Handle_Jiki_Moving_Arm		= MV1LoadModel("Model/Jiki/Jiki_Alpha_Moving_Arm.mqo");		//���@�ړ����̘r
	Handle_LightModel			= MV1LoadModel("Model/Light.mqo");
	Handle_Item_Map				= MV1LoadModel("Model/item/item_map.mqo");
	Handle_Item_Ans				= MV1LoadModel("Model/item/item_ans.mqo");
	Handle_Item_Key				= MV1LoadModel("Model/item/item_key.mqo");
	Handle_Wall					= MV1LoadModel("Model/Wall.mqo");							//���H��
	Handle_Floor				= MV1LoadModel("Model/Floor.mqo");							//���H���E�V��
	Handle_Start				= MV1LoadModel("Model/Start.mqo");							//���H�X�^�[�g�n�_
	Handle_Goal					= MV1LoadModel("Model/Goal.mqo");							//���H�S�[���n�_
	Handle_GoalDoor				= MV1LoadModel("Model/Goal_Door_Keymode.mqo");

	Handle_Title				= LoadGraph("Picture/MazeTitle.png");						//MazeTraveler�^�C�g��

	Handle_SelectTile_Blue		= LoadGraph("Picture/SelectTile_Blue.png");					//�I�����j���[��
	Handle_SelectTile_Red		= LoadGraph("Picture/SelectTile_Red.png");					//�I�����j���[��
	Handle_SelectTile_Green		= LoadGraph("Picture/SelectTile_Green.png");				//�I�����j���[��
	Handle_SelectTile_Purple	= LoadGraph("Picture/SelectTile_Purple.png");				//�I�����j���[��
	Handle_SelectTile_Yellow	= LoadGraph("Picture/SelectTile_Yellow.png");				//�I�����j���[���F
	Handle_SelectTile_Gray		= LoadGraph("Picture/SelectTile_Gray.png");					//�I�����j���[�D�F

	Handle_TimeDisp_Tile		= LoadGraph("Picture/TimeDisp_Tile.png");					//���H�̃X�e�[�W���Ǝ��ԕ\��

	Handle_Delete_Warning		= LoadGraph("Picture/Delete_Warning.png");					//�폜���x������\��
	Handle_Delete_Yes			= LoadGraph("Picture/Delete_Yes.png");						//�폜����̑I��
	Handle_Delete_No			= LoadGraph("Picture/Delete_No.png");						//�폜���Ȃ��̑I��

	Handle_SolveRoute			= LoadGraph("Picture/SolveRoute.png");						//�������[�g�̃i�r�Q�[�g

	Handle_Goaled				= LoadGraph("Picture/Goaled.png");							//�S�[���������Ƃ������\��
	Handle_Title_DispAssist		= LoadGraph("Picture/Title_DispAssist.png");				//�^�C�g����ʂ̕����������₷���悤�ɂ��邽�߂̔w�i

	Handle_Result_Tile			= LoadGraph("Picture/Result_Tile.png");						//�X�e�[�W�̌��ʁi�X�e�[�W���Ǝ��ԕ\���A�ڕW���ԂƂ̑Δ�O���t�j
	Handle_Result_Passed		= LoadGraph("Picture/Result_Passed.png");					//�X�e�[�W���i
	Handle_Result_VeryGood		= LoadGraph("Picture/Result_VeryGood.png");
	Handle_Result_Failed		= LoadGraph("Picture/Result_Failed.png");					//�X�e�[�W�s���i
	Handle_Gotitem_Tile			= LoadGraph("Picture/gotitem_background.png");				//��ɓ��ꂽ�A�C�e���\����
	Handle_Gotitem_Map			= LoadGraph("Picture/gotitem_map.png");						//�A�C�e���́@�@�}�b�v�@����ɓ���܂����B
	Handle_Gotitem_Ans			= LoadGraph("Picture/gotitem_ans.png");						//�A�C�e���́@�o�H�\���@����ɓ���܂����B
	Handle_Gotitem_Key			= LoadGraph("Picture/gotitem_key.png");						//�A�C�e���́@�@�@�@���@����ɓ���܂����B
	Handle_Challanging_Keymode	= LoadGraph("Picture/Challanging_Keymode.png");				//�����[�h����������摜
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
	LoadDivGraph("Picture/Dot4.png", 10, 10, 1, 10, 10, Handle_Dotpic);						//DispStringOfBox�֐��ŕ\�����镶���̃h�b�g�i1�����ӂ�3�~5�j
	LoadDivGraph("Picture/Map_2Dim.png", 80, 8, 10, 10, 10, Handle_2dimmap);				//2�����}�b�v�̕\���摜
	LoadDivGraph("Picture/BackGround.png", 2, 1, 2, 20, 20, Handle_BackGround);				//���̃Q�[���̓����w�i�摜�i�Q�[���I���j
	LoadDivGraph("Picture/BackGround_2.png", 2, 1, 2, 20, 20, Handle_BackGround_2);			//���̃Q�[���̓����w�i�摜�i���[�U�[�I���j
	LoadDivGraph("Picture/BackGround_3.png", 2, 1, 2, 20, 20, Handle_BackGround_3);			//���̃Q�[���̓����w�i�摜�i���[�U�[�I���j
	SetTransColor(0, 0, 0);

	SetTransColor(200, 200, 200);
	Handle_Help_Back = LoadGraph("Picture/Help_Back.png");
	SetTransColor(0, 0, 0);

	SetTransColor(200, 200, 200);
	Handle_Navigate_Back = LoadGraph("Picture/Navigate_Back.png");
	SetTransColor(0, 0, 0);

	Handle_Screen_FullFalse		= MakeScreen(SCREEN_X / 3, SCREEN_Y / 3);					//2�����}�b�v�i�r�Q�[�g�̔�t���X�N���[����
	Handle_Screen_FullTrue		= MakeScreen(SCREEN_X, SCREEN_Y);							//2�����}�b�v�i�r�Q�[�g�̃t���X�N���[����
	Handle_Screen_Normal		= MakeScreen(SCREEN_X, SCREEN_Y);							//�ʏ���
	Handle_Screen_Chat			= MakeScreen(SCREEN_X / 3, SCREEN_Y / 3);					//�`���b�g���X�N���[��

	SetDrawScreen(Handle_Screen_Chat);
	ClearDrawScreen();
	SetDrawScreen(Handle_Screen_FullFalse);
	ClearDrawScreen();
	SetDrawScreen(Handle_Screen_FullTrue);
	ClearDrawScreen();
	SetDrawScreen(Handle_Screen_Normal);
	ClearDrawScreen();

	//�w���v�t�@�C���ǂݍ���
	Notesel(help_text, 32768);
	Noteload("helps/help_text.txt");
	help_text_spl_n = split(help_text, ";", &help_text_spl);

	//�i�r�Q�[�V�����ǂݍ���
	Notesel(navi_text, 32768);
	Noteload("helps/navi_text.txt");
	navi_text_spl_n = split(navi_text, ";", &navi_text_spl);
}

//�Q�[���t�@�C���ꗗ�擾
void LoadGameFileLists(void)
{
	char filelist[2048];
	char buf[256];
	char username[256] = "";
	int i, j;
	FILE *fp;

	//�Q�[���f�[�^�ꗗ�擾�i���s�ŋ�؂��Ă���j
	MakeDir("./Saves");
	dirlist(filelist, 2048, "./" SAVE_DIR "\\", "*" KAKUCYOSI);

	//�O�񓮓I�z�񂪊m�ۂ���Ă�����J��
	if (GameFilenames != NULL)
	{
		delsplit(Filename_n, GameFilenames);
		DelArray1Dim(filepoint);
		DelArray2Dim(fileusername, Filename_n);
	}
	//�Q�[���f�[�^�ꗗ�擾����������ɂ����āA
	//���s�����ŋ�؂��ĕ�������
	Filename_n = split(filelist, "\n", &GameFilenames);
	Filename_n--;

	//���I�m��
	GetArray1Dim(int, filepoint, Filename_n);
	GetArray2Dim(char, fileusername, Filename_n, 16);

	//�t�@�C���������J��Ԃ�
	for (i = 0; i < Filename_n; i++)
	{
		//�t�@�C�������݂�����J��
		sprintf_s(buf, 256, "./Saves/%s", GameFilenames[i]);
		fopen_s(&fp, buf, "rb");
		if (fp != NULL)
		{
			//���[�U�[�����t�@�C�������璊�o�i�g���q��������Ɓj
			strcpy_s(username, 256, GameFilenames[i]);
			for (j = strlen(username); j >= 0 && username[j] != '.'; j--);
			if (j >= 0 && j < 16)
				username[j] = '\0';
			else
				username[15] = '\0';
			strcpy_s(fileusername[i], 16, username);

			//�t�@�C���̐擪�ɃJ�[�\�������킹��
			fseek(fp, 0, SEEK_SET);

			//�t�@�C�����̃|�C���g���i�[����Ă���ꏊ�ɃJ�[�\�������킹��
			fseek(fp, sizeof(int), SEEK_CUR);

			//�|�C���g��ǂ݂���
			fread(&filepoint[i], sizeof(int), 1, fp);

			//�t�@�C�������
			fclose(fp);
		}
	}
}

