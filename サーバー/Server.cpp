#include "DxLib.h"
#include <math.h>
#include <string.h>

#include "UsefulHeaders/useful.h"

#define SCREEN_X 640
#define SCREEN_Y 480
#define STR_COLOR 0xFFFFFF
#define STR_JOIN 0x9999ff
#define STR_DIS  0xff9999
#define STR_RANK 0xffff99

//-------------------------------------------------------------------------------------------
//���O�t�@�C���Z�[�u�t���O
//�����true�ɂ���΃��O�t�@�C����ۑ����܂��B
//-------------------------------------------------------------------------------------------

bool IsSaveLog = false;

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

typedef enum
{
	DAT_JOINED,
	DAT_DIS,
	DAT_NAME,

	//�`���b�g
	DAT_CHAT,

	DAT_ROOM_N,
	DAT_ROOM_NAME,
	DAT_ROOM_ISUSE,
	DAT_ROOM_MAZEINFO,
	DAT_ROOM_ADD,

	DAT_CREATE_ROOM,
	DAT_DEST_ROOM,

	DAT_JOIN_ROOM,
	DAT_DIS_ROOM,

	DAT_TAIKAI_ROOM,

	DAT_NAME_ERROR,
	DAT_CHAT_ERROR,
	DAT_CONNECT_ERROR,

	//���H
	DAT_MAZE_MEMBER_N,
	DAT_MAZE_MEMBER_DATA,
	DAT_MAZE_MEMBER_RECODE,
	DAT_MAZE_MEMBER_ADD,
	DAT_MAZE_MEMBER_YOURNO,

	DAT_MAZEINFO,
	DAT_MAZEINFO_NAME,
	DAT_MAZEINFO_DATA,

	DAT_MAZE_START,
	DAT_MAZE_GOAL,
	DAT_MAZE_CANCEL
}
DATATYPE;

/*
typedef struct
{
	//int rank;
	//int mazetime;
	//MAZERECODE recode;
}
GAMEINFO;
*/
typedef struct
{
	VECTOR Pos;
	double rot;
	double rotd;
	bool stopf;
}
JIKI;

typedef struct
{
	int seed;
	int level;
	bool iskeymode;
}
MAZEINFO;

typedef struct
{
	int mazetime;
}
FORSEND;

typedef struct
{
	FORSEND fs;
	//int mazetime_st;
	int rank;
	int index;
	int ischallange;		////0 : ������A1 : ���풆�A2 : ����ς�
	bool flag;
}
MEMBER_INFO;

//���H���
typedef struct
{
	//GAMEINFO info;
	char username[16];
	//int ischallange;		

	//VECTOR JikiPos;
	//double Jikirot;
	JIKI Jiki;
	double CamKakudo;
}
MAZE_DB;

//�l�b�g���[�N���
typedef struct
{
	int NHandle;
	IPDATA Ip;
	//int db_index;
	//char *name;
	//GAMEINFO ginfo;
}
NETINFO;

//�N���C�A���g���z��
typedef struct
{
	NETINFO	net;
	MAZE_DB mdb;
	int room_no;
	bool isuse;
	int make_roomno;
	//int member_no;
}
CLIENT;

typedef struct
{
	char *content;
	int len;
}
CHAT;

typedef struct
{
	char *content;
	int len;
}
MYLOG;

/*
typedef struct
{
	char *content;
	int len;
	char name[256];
	int no;
}
MYLOG;
*/
class ROOMS
{
private:
	CHAT Chat;
	//MYLOG mylog;
	//MYLOG clog;

	int no;
	//int *index;
	//int member_n = 0;
	MEMBER_INFO *member;
	int member_n = 0;

public:
	char Name[32];
	bool isuse;
	MAZEINFO minfo;

public:
	void init(int, char*);
	void update(void);
	void disp(void);
	void dest(void);

	void join(int);
	void dis(int);
	void taikai(int, char*);

	void member_sort(void);

	void addchat(char *m ...);

	/*
	char* GetName(void)
	{
		return Name;
	}

	bool isuse(void)
	{
		return flag;
	}
	*/

	//void setinfo(MAZEINFO minfo_)
	//{
	//	minfo = minfo_;
	//}

	ROOMS(void)
	{
		no = -1;
		isuse = false;
		minfo.seed = -1;
		minfo.level = -1;
	}
private:

	int  member_count(void);
	int  member_yourno(int);

	void Communicate(void);
	void rank_decide(void);
	void dis_chk_yobi(void);

	void send_memberinfo(int);
	int recv_memberinfo(int);
	//void addlog(const char *format, ...);

	/*
	void send_val(int, DATATYPE, void*, size_t);
	int recv_str(int, DATATYPE, char**);
	int recv_val(int, DATATYPE, void*);
	*/
};

class USERS
{
public:
	int no;

	void init(void);
	void update(void);
	void dest(void);
	void disp(void);
	USERS(void)
	{
		no = -1;
	}

private:
	void CheckNewConnection(void);
	void CheckLostConnection(void);

	void Communicate(void);

	void send_roominfo(int);
	void recv_mazeinfo(int, int);

	//void send_val(int, DATATYPE, void*, size_t);
	//int recv_str(int, DATATYPE, char**);
	//int recv_val(int, DATATYPE, void*);
};

//�v���g�^�C�v�錾

USERS Users;

ROOMS *MazeRoom = new ROOMS[0];
int MazeRoom_n	= 0;

//CLIENT *Client;
CLIENT *Client;
int client_n	= 0;

MYLOG MyLog;

/*
MAZE_DB *mdb;
int mdb_n		= 0;
*/

int port = 9850;

int count_client(void);
void init(void);
void update(void);
void dest(void);
void disp(void);
void input_port(void);

/*
CHAT Chat;
MYLOG mylog;
MYLOG clog;
*/

/*
void CheckNewConnection(void);
void CheckLostConnection(void);

void Communicate_Client(void);

void my_send(int, DATATYPE, void*, size_t);
int my_recv(int, DATATYPE, void*);

void addlog(const char *format, ...);
*/

void send_val(int, DATATYPE, void*, size_t);
int recv_str(int, DATATYPE, char**);
int recv_val(int, DATATYPE, void*);

void send(int, DATATYPE, void*, size_t);
int recv(int, DATATYPE, void*);

int hikaku(const void *, const void *);

void AddLogFiles(const char *, ...);

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	init();

	input_port();

	PreparationListenNetWork(port);

	//��������
	while(!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE)==0)
	{
		ClearDrawScreen();

		update();

		disp();

		/*
		CheckNewConnection();
		CheckLostConnection();

		Communicate_Client();
		*/

		ScreenFlip();
	}
	//�����܂�

	StopListenNetWork();

	dest();
	return 0;		//����I��
}

void init(void)
{
	//��ʃT�C�Y�ƃE�B���h�E���[�h�ݒ�
	SetGraphMode(640, 480, 32);
	ChangeWindowMode(true);										//�E�B���h�E���[�h�ŋN���A�R�����g�A�E�g�Ńt���X�N���[��
	SetDoubleStartValidFlag(true);
	SetWindowIconID(101);
	SetAlwaysRunFlag(true);
	SetOutApplicationLogValidFlag(false);

	//-------------------------------------------------------------------------------------------

	if (DxLib_Init() == -1) exit(-1);					//DX���C�u����������

	SetBackgroundColor(0, 0, 0);						//�w�i�F
	SetDrawScreen(DX_SCREEN_BACK);						//����ʂɕ`��

	//DX ���C�u���������������A�����������s������v���O�����I��
	if (DxLib_Init() == -1) exit(-1);

	//GetArray1Dim(int, Client.NHandle, 0);
	//GetArray1Dim(bool, Client.flag, 0);
	//GetArray1Dim(IPDATA, Client.Ip, 0);
	//GetArray1Dim(char*, Client.name, 0);

	Users.init();

	GetArray1Dim(char, MyLog.content, 1);
	MyLog.content[0] = '\0';
	MyLog.len = 0;
}

void input_port(void)
{
	char inputdata[16];
	static bool confflag = true;
	int cursor = 0;
	int c;
	int framecnt = 0;

	while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();

		if (confflag == true)
		{
			ClearInputCharBuf();
			itoa(port, inputdata, 10);
			cursor = strlen(inputdata);

			confflag = false;
		}

		c = m_inputnum();

		if (c >= 0 && c <= 9)
		{
			inputdata[cursor] = '0' + c;
			inputdata[cursor + 1] = '\0';

			if (atoi(inputdata) < 65536)
			{
				cursor++;
			}
			else
			{
				if (atoi(inputdata) < 100000)
				{
					cursor++;
				}

				inputdata[0] = '6';
				inputdata[1] = '5';
				inputdata[2] = '5';
				inputdata[3] = '3';
				inputdata[4] = '5';
				inputdata[5] = '\0';
			}
		}
		else if (c == 10)
		{
			if (cursor > 0) cursor--;
			if (cursor >= 1)
				inputdata[cursor] = '\0';
			else
			{
				inputdata[cursor] = '0';
				inputdata[cursor + 1] = '\0';
			}
		}

		if (Stick(KEY_INPUT_RETURN))
		{
			confflag = true;
			port = atoi(inputdata);

			break;
		}

		DrawFormatString(10			, SCREEN_Y - 20, STR_COLOR, "�|�[�g�ԍ� > ");
		DrawFormatString(10 + 9*13	, SCREEN_Y - 20, STR_COLOR, "%s", inputdata);

		if (framecnt % 60 < 30)
			DrawLine(
			10 + 9 * 13 + cursor * 9,
			SCREEN_Y - 21,
			10 + 9 * 13 + cursor * 9,
			SCREEN_Y - 2,
			STR_COLOR);

		framecnt++;

		ScreenFlip();
	}
}

void update(void)
{
	int i;
	Users.update();

	for (i = 0; i < MazeRoom_n; i++)
	{
		MazeRoom[i].update();
	}
}

void dest(void)
{
	int i;
	FILE *fp;

	for (i = 0; i < MazeRoom_n; i++)
	{
		if (MazeRoom[i].isuse == true)
			MazeRoom[i].dest();
	}

	delete MazeRoom;

	Users.dest();

	if (IsSaveLog)
	{
		fopen_s(&fp, "MyLog.txt", "w");
		fprintf(fp, "%s", MyLog.content);
		fclose(fp);
	}
	DelArray1Dim(MyLog.content);

	DxLib_End();	//DX ���C�u�������I��
}

void disp(void)
{
	static int disproom = -1;

	//���͕�
	if (StickMouse(MOUSE_INPUT_LEFT))
	{
		if (MazeRoom_n > 0)
		{
			if (disproom < MazeRoom_n - 1)
				disproom++;
			else
				disproom = -1;
		}
	}

	//�\�����i�֐��Ăяo���j
	if (disproom == -1)
	{
		Users.disp();
	}
	else
	{
		MazeRoom[disproom].disp();
	}
}

//�N���C�A���g���A�Q�[���f�[�^�x�[�X�z��m��
void USERS::init(void)
{
	GetArray1Dim(CLIENT, Client, 0);
	//GetArray1Dim(MAZE_DB, mdb, 0);
	client_n = 0;
}

//�����ɓ����Ă��Ȃ��l�̒ʐM�A
//�ڑ��A�ؒf��Ԃ𒲂ׁA��������B
void USERS::update(void)
{
	CheckNewConnection();
	CheckLostConnection();

	Communicate();
}

//�N���C�A���g���A�Q�[���f�[�^�x�[�X���폜
void USERS::dest(void)
{
	//int i;

	/*
	for (i = 0; i < client_n; i++)
		DelArray1Dim(Client[i].gdat.name);
		*/
	//DelArray1Dim(mdb);
	DelArray1Dim(Client);
}

void USERS::disp(void)
{
	char UserList[4096];

	SPLITDATA spldata;
	int spl_n;
	int i;
	int num;

	static int pos_y_min = 0;
	static int before_pos_y_min;
	static int pos_y = pos_y_min;

	int rot;

	static bool newinfoflag = false;

	before_pos_y_min = pos_y_min;

	sprintf_s(UserList, 4096, "�����ڑ��҈ꗗ\n");
	num = 0;
	for (i = 0; i < client_n; i++)
	{
		if (Client[i].isuse == true)
		{
			sprintf_s(UserList, 4096, "%s%02d, %s, IP : %3d.%3d.%3d.%3d, %dByte Buffer, room_no = %d\n",
				UserList,
				num + 1,
				Client[i].mdb.username,
				Client[i].net.Ip.d1,
				Client[i].net.Ip.d2,
				Client[i].net.Ip.d3,
				Client[i].net.Ip.d4,
				GetNetWorkDataLength(Client[i].net.NHandle),
				Client[i].room_no
				);
			num++;
		}
	}

	spl_n = split(UserList, "\n", &spldata);

	{
		rot = GetMouseWheelRotVol();
		pos_y += rot * 10;
		if (pos_y < pos_y_min)
		{
			pos_y = pos_y_min;
		}
		else if (pos_y > 0)
		{
			pos_y = 0;
		}
		if (pos_y == pos_y_min)
		{
			newinfoflag = false;
		}
	}

	//��ʂɎ��܂�s�ȏ�̃`���b�g���������獶����W��������
	if (spl_n * 20 + 10 >= 460)
	{
		pos_y_min = -(spl_n * 20 + 10 - 460);
	}
	else pos_y_min = 0;

	/*
	if (islog != islog_before)
	{
		before_pos_y_min = pos_y_min;
		pos_y = pos_y_min;
	}
	*/

	//�V�������b�Z�[�W��������
	if (before_pos_y_min != pos_y_min)
	{
		if (pos_y == before_pos_y_min)
		{
			pos_y = pos_y_min;
		}
		else
		{
			newinfoflag = true;
		}
	}

	for (i = 0; i < spl_n; i++)
	{
		DrawFormatString(10, pos_y + 10 + 20 * i, STR_COLOR, "%s", spldata[i]);
	}

	DrawBox(0, SCREEN_Y - 20, SCREEN_X, SCREEN_Y, 0x000000, TRUE);

	DrawFormatString(10, SCREEN_Y - 20, STR_COLOR, "[���[�U�[���] Client�ڑ�:%02d���A%d�����A�z�C�[��OK�A���N���b�N�Őؑ�", count_client(), MazeRoom_n);

	if (newinfoflag == true)
	{
		DrawBox(SCREEN_X - 100, 0, SCREEN_X, 20, 0x000000, TRUE);
		DrawFormatString(SCREEN_X - 100, 0, STR_COLOR, "�V�������");
	}

	delsplit(spl_n, spldata);
}

//�����쐬
void ROOMS::init(int room_no, char *room_name)
{
	int len;

	//�����ԍ��ƕ�������o�^
	no = room_no;
	strcpy_s(Name, 32, room_name);

	//�`���b�g���e
	GetArray1Dim(char, Chat.content, 1);
	Chat.content[0] = '\0';
	Chat.len = 0;

	//�N���C�A���g�A�f�[�^�x�[�X�A�N�Z�X�p�C���f�b�N�X
	GetArray1Dim(MEMBER_INFO, member, 0);
	member_n = 0;

	//�����͎g�p��
	isuse = true;

}

//�����X�V
//�������̒ʐM�Ȃǂ�����
void ROOMS::update(void)
{
	if (isuse == true)
	{
		Communicate();
		rank_decide();
		member_sort();
		dis_chk_yobi();
	}
}

void ROOMS::dis_chk_yobi(void)
{
	int i, indi;

	for (i = 0; i < member_n; i++)
	{
		indi = member[i].index;
		if (Client[indi].isuse == false && member[i].ischallange == 1)
		{
			member[i].flag = false;
			member[i].ischallange = 0;
		}
	}
}

//�����폜
void ROOMS::dest(void)
{
	//FILE *fp;

	/*
	MakeDir("./Logs");
	MakeDir("./Logs/CLog");

	clog = mylog;
	clog.content = Chat.content;
	clog.len = Chat.len;

	sprintf_s(clog.name, 256, "./Logs/CLog/ChatLog%2d.txt", clog.no);

	clsDx();

	fopen_s(&fp, clog.name, "w");
	if (fp != NULL)
	{
		fprintf(fp, "%s", clog.content);
		fclose(fp);
	}
	*/

	DelArray1Dim(Chat.content);

	DelArray1Dim(member);
	member_n = 0;
	//DelArray1Dim(mylog.content);

	isuse = false;
}

void ROOMS::disp(void)
{
	SPLITDATA spldata;
	int spl_n;
	int i;
	static int pos_y_min = 0;
	static int before_pos_y_min;
	static int pos_y = pos_y_min;
	int rot;
	static bool newchatflag = false;

	before_pos_y_min = pos_y_min;

	spl_n = split(Chat.content, "\n", &spldata);

	{
		rot = GetMouseWheelRotVol();
		pos_y += rot * 10;
		if (pos_y < pos_y_min)
		{
			pos_y = pos_y_min;
		}
		else if (pos_y > 0)
		{
			pos_y = 0;
		}
		if (pos_y == pos_y_min)
		{
			newchatflag = false;
		}
	}

	//��ʂɎ��܂�s�ȏ�̃`���b�g���������獶����W��������
	if (spl_n * 20 + 10 >= 460)
	{
		pos_y_min = -(spl_n * 20 + 10 - 460);
	}
	else pos_y_min = 0;

	//�V�������b�Z�[�W��������
	if (before_pos_y_min != pos_y_min)
	{
		if (pos_y == before_pos_y_min)
		{
			pos_y = pos_y_min;
		}
		else
		{
			newchatflag = true;
		}
	}

	for (i = 0; i < spl_n; i++)
	{
		if (strstr(spldata[i], " : ") == NULL && strstr(spldata[i], "�Q��") != NULL)
			DrawFormatString(10, pos_y + 10 + 20 * i, STR_JOIN, "%s", spldata[i]);
		else if (strstr(spldata[i], " : ") == NULL && strstr(spldata[i], "�ޏo") != NULL)
			DrawFormatString(10, pos_y + 10 + 20 * i, STR_DIS, "%s", spldata[i]);
		else if (strstr(spldata[i], "����̓^�C��") != NULL && strstr(spldata[i], "�ʂɂȂ�܂����I") != NULL)
			DrawFormatString(10, pos_y + 10 + 20 * i, STR_RANK, "%s", spldata[i]);
		else
			DrawFormatString(10, pos_y + 10 + 20 * i, STR_COLOR, "%s", spldata[i]);
	}

	DrawBox(0, SCREEN_Y - 20, SCREEN_X, SCREEN_Y, 0x000000, TRUE);
	DrawFormatString(10, SCREEN_Y - 20, STR_COLOR, "[�`���b�g���e] �v���C���A����%d�l�A�z�C�[��OK�A���N���b�N�ؑ�", member_count());

	if (newchatflag == true)
	{
		DrawBox(SCREEN_X - 100, 0, SCREEN_X, 20, 0x000000, TRUE);
		DrawFormatString(SCREEN_X - 100, 0, STR_COLOR, "�V�������O");
	}

	delsplit(spl_n, spldata);
}

//�����̎Q��
void ROOMS::join(int userno)
{
	int i;

	char joinmessage[256];
	int len;

	Client[userno].room_no = no;

	//����܂ł̕����̃`���b�g�𑗐M
	send(Client[userno].net.NHandle, DAT_CHAT, Chat.content, Chat.len + 1);

	//�Q�����b�Z�[�W�𑗂�
	sprintf_s(joinmessage, 256, "%s���񂪖��H�ɎQ�����܂����B\n", Client[userno].mdb.username);
	len = strlen(joinmessage);

	//�S�̂̃`���b�g�L�^�ɒǉ�����
	Chat.len += len;
	ChangeArraySize_1dim(char, Chat.content, Chat.len + 1);
	sprintf_s(Chat.content, Chat.len + 1, "%s%s", Chat.content, joinmessage);

	//���������̃����o�[�S���Ƀ`���b�g�Ƃ��đ���
	for (i = 0; i < client_n; i++)
	{
		if (Client[i].isuse == true && Client[i].room_no == Client[userno].room_no)
		{
			send(Client[i].net.NHandle, DAT_JOINED, joinmessage, sizeof(char)*(len + 1));
		}
	}

	//���H���i�V�[�h�l�A���x���j���Q���҂ɑ��M
	//send(Client[userno].net.NHandle, DAT_MAZE_INFO, &minfo, sizeof(MAZEINFO));

	//���[�U�[���𕔉��ɒǉ�
	ChangeArraySize_1dim(MEMBER_INFO, member, member_n + 1);
	member[member_n].index = userno;							//�N���C�A���g�\���̔z��Y��
	member[member_n].fs.mazetime = 0;								//���H���펞��
	//member[member_n].mazetime_best = 0;							//���H���펞��
	member[member_n].ischallange = 0;								//0:������A1:���풆�A2:����ς�
	member[member_n].rank = -1;									//����
	member[member_n].flag = true;								//�g�p���t���O�𗧂Ă�
	//Client[userno].member_no = member_n;						//���[�U�[���\���̑��Ƀ����o�[�ԍ���o�^
	member_n++;													//�Q���l�����J�E���g�A�b�v
}

//�����̑ޏo
void ROOMS::dis(int userno)
{
	int i;

	char dismessage[256];
	int len;

	//���H�ޏo���b�Z�[�W��������쐬
	sprintf_s(dismessage, 256, "%s���񂪖��H����ޏo���܂����B\n", Client[userno].mdb.username);
	len = strlen(dismessage);

	//���H�ޏo���b�Z�[�W���`���b�g�֒ǉ�
	Chat.len += len;
	ChangeArraySize_1dim(char, Chat.content, Chat.len + 1);
	sprintf_s(Chat.content, Chat.len + 1, "%s%s", Chat.content, dismessage);

	//���������̃����o�[�S���ɑ���
	for (i = 0; i < client_n; i++)
	{
		if (Client[i].isuse == true && Client[i].room_no == Client[userno].room_no && i != userno)
		{
			send(Client[i].net.NHandle, DAT_DIS, dismessage, sizeof(char)*(len + 1));
		}
	}

	//member[Client[userno].member_no].flag = false;	//�g�p���t���O���~�낷
	Client[userno].room_no = -1;
	//Client[userno].member_no = -1;					//�����o�[�ԍ��͑ޏo��Ȃ̂łȂ��ɂ���
}

//��������މ����
void ROOMS::taikai(int userno, char *reason)
{
	send(Client[userno].net.NHandle, DAT_TAIKAI_ROOM, reason, sizeof(reason) + 1);

	//�S�̂̃`���b�g�L�^�ɒǉ�����
	Chat.len += sizeof(reason);
	ChangeArraySize_1dim(char, Chat.content, Chat.len + 1);
	sprintf_s(Chat.content, Chat.len + 1, "%s%s", Chat.content, reason);
}

//�V�����ڑ��҂����o�A�������B
void USERS::CheckNewConnection(void)
{
	int NetHandle_Temp;
	int cursor = 0;

	int i;

	//�V�����ڑ��҂��m�F
	NetHandle_Temp = GetNewAcceptNetWork();

	//�V�����ڑ��҂����o������
	if (NetHandle_Temp != -1)
	{
		//�ڑ��ҏ��i�[�z��m��
		ChangeArraySize_1dim(CLIENT, Client, client_n + 1);
		//ChangeArraySize_1dim(MAZE_DB, mdb, client_n + 1);

		//������
		Client[client_n].net.NHandle = NetHandle_Temp;							//�n���h���͕ϐ��֊i�[
		GetNetWorkIP(Client[client_n].net.NHandle, &Client[client_n].net.Ip);	//IP�A�h���X���ꉞ�i�[
		Client[client_n].isuse = true;								//�z��͎g�p���܂��B
		Client[client_n].room_no = -1;								//�����ԍ��@�����ɂ͓����Ă��Ȃ��i�|�P�̏ꍇ�j
		Client[client_n].mdb.Jiki.Pos.x = 0.0f;
		Client[client_n].mdb.Jiki.Pos.y = 0.0f;
		Client[client_n].mdb.Jiki.Pos.z = 0.0f;
		Client[client_n].mdb.Jiki.stopf = true;
		Client[client_n].mdb.Jiki.rotd = 0.0f;
		Client[client_n].mdb.Jiki.rot = 0.0;
		Client[client_n].mdb.CamKakudo = 0.0;
		Client[client_n].make_roomno = -1;
		//Client[client_n].member_no			= -1;
		strcpy_s(Client[client_n].mdb.username, 32, "");						//���[�U�[���͑��M�����܂ŋ󕶎���

		//���H���[����񑗐M
		send_roominfo(Client[client_n].net.NHandle);
		/*
		send(Client[client_n].net.NHandle, DAT_ROOM_N, &MazeRoom_n, sizeof(int));
		for (i = 0; i < MazeRoom_n; i++)
		{
		send(Client[client_n].net.NHandle, DAT_ROOM_ADD, NULL, 0);
		send(Client[client_n].net.NHandle, DAT_ROOM_NAME, MazeRoom[i].Name, strlen(MazeRoom[i].Name) + 1);
		send(Client[client_n].net.NHandle, DAT_ROOM_ISUSE, &MazeRoom[i].isuse, sizeof(bool));
		}
		*/

		//�ڑ��҃J�E���g�𑝉�
		client_n++;
	}
}

//�ؒf�҂����o�A�㏈���B
void USERS::CheckLostConnection(void)
{
	int NetHandle_Temp;

	int i;

	//�ؒf�҂��m�F
	NetHandle_Temp = GetLostNetWork();

	//�ؒf�҂͂��ꂩ�T��
	for (i = 0; i < client_n; i++)
	{
		//�ڑ��t���O�������Ă�����
		if (Client[i].isuse == true)
		{
			//�ؒf�҂�����������
			if (Client[i].net.NHandle == NetHandle_Temp)
			{
				Client[i].isuse = false;						//�ڑ��t���O�~�낷

				//�����ɓ����Ă�����ޏo�B
				if (Client[i].room_no != -1)
					MazeRoom[Client[i].room_no].dis(i);

				break;
			}
		}
	}
}

//���ݐڑ����Ă���N���C�A���g�𐔂���
int count_client(void)
{
	int i, cnt = 0;

	//�t���O�������Ă�����𐔂���B
	for (i = 0; i < client_n; i++)
	{
		if (Client[i].isuse == true)
			cnt++;
	}
	return cnt;
}

//�����ɓ����Ă��Ȃ��N���C�A���g�ƒʐM�i��M�A������M�Ɍ���j
void USERS::Communicate(void)
{
	int i, j;
	int leng;

	DATATYPE d_type;
	char *d_s;
	int d_s_len;

	int room_no;

	static int framecnt = 0;

	GetArray1Dim(char, d_s, 0);

	//�N���C�A���g�̌���������M�o�b�t�@�𒲂ׂ�
	for (i = 0; i < client_n; i++)
	{
		//�N���C�A���g���ڑ�����Ă�����
		if (Client[i].isuse == true && Client[i].room_no == no)
		{
			//������M-------------
			//if (Client[i].room_no == no)
			if (framecnt % 3 == 0)
			{
				send_roominfo(Client[i].net.NHandle);
			}

			//��M-----------------
			//��M�T�C�Y�𒲂ׂ�
			leng = GetNetWorkDataLength(Client[i].net.NHandle);

			//�ǂ��������ɓ����Ă��Ȃ�������ino = -1�j
			//��M����������
			while (leng >= sizeof(DATATYPE) && Client[i].room_no == no && Client[i].isuse == true)
			{
				//�f�[�^�^�C�v���擾����
				NetWorkRecv(Client[i].net.NHandle, &d_type, sizeof(DATATYPE));
				leng = GetNetWorkDataLength(Client[i].net.NHandle);

				AddLogFiles("d_type = %d\n", d_type);

				//�f�[�^�^�C�v�ɉ���������
				switch (d_type)
				{
				//���O����M
				case DAT_NAME:
					//��M
					d_s_len = recv(Client[i].net.NHandle, d_type, &d_s);

					//���O��o�^����
					//ChangeArraySize_1dim(char, Client[i].mdb.username, d_s_len + 1);
					sprintf_s(Client[i].mdb.username, d_s_len + 1, "%s", d_s);

					break;

				//�����ւ̎Q��
				case DAT_JOIN_ROOM:
					recv(Client[i].net.NHandle, d_type, &Client[i].room_no);
					MazeRoom[Client[i].room_no].join(i);
					break;

				//�����̍쐬
				case DAT_CREATE_ROOM:
					//d_s_len = recv(Client[i].net.NHandle, d_type, &d_s);
					{
						ROOMS *temp;
						temp = new ROOMS[MazeRoom_n + 1];
						for (j = 0; j < MazeRoom_n; j++)
						{
							temp[j] = MazeRoom[j];
						}
						delete MazeRoom;
						MazeRoom = temp;
					}
					MazeRoom[MazeRoom_n].init(MazeRoom_n, "");
					Client[i].make_roomno = MazeRoom_n;
					MazeRoom_n++;
					break;

				//�V�[�h�l�A���x����M
				case DAT_MAZEINFO:
					recv_val(Client[i].net.NHandle, DAT_MAZEINFO, &room_no);

					if (room_no == -1)
						room_no = Client[i].make_roomno;

					recv_mazeinfo(Client[i].net.NHandle, room_no);

					//�Q�����b�Z�[�W�𑗂�
					MazeRoom[room_no].addchat(
						"------------------------------------------\n"
						"�X�e�[�W���F%s\n"
						"------------------------------------------\n",
						MazeRoom[room_no].Name);

					Client[i].make_roomno = -1;

					/*
					//�S�̂̃`���b�g�L�^�ɒǉ�����
					Chat.len += len;
					ChangeArraySize_1dim(char, Chat.content, Chat.len + 1);
					sprintf_s(Chat.content, Chat.len + 1, "%s%s", Chat.content, headmessage);
					break;
					*/
					break;

				//�����̔j��
				case DAT_DEST_ROOM:
					MazeRoom[Client[i].room_no].dest();
					for (j = 0; j < client_n; j++)
					{
						if (Client[i].room_no == Client[j].room_no && Client[j].isuse == true)
						{
							MazeRoom[Client[i].room_no].taikai(j, "�������Ǘ��҂ɂ���Ĕj������܂����B");
						}
						MazeRoom[Client[i].room_no].dis(i);
					}
					//Client[i].room_no = -1;
					break;

				}	//end switch
				ChangeArraySize_1dim(char, d_s, 0);
				leng = GetNetWorkDataLength(Client[i].net.NHandle);
			}
		}
	}
	DelArray1Dim(d_s);

	framecnt++;
}

//�����ɓ����Ă���N���C�A���g�ƒʐM�i�������ƂɑΉ�����j
void ROOMS::Communicate(void)
{
	int i, j;				//���������[�U�[�ԍ�
	int indi, indj;			//���������[�U�[�ԍ�����S�̃��[�U�[�ԍ��ւ̕ϊ��l
	int cnt;
	int leng;
	int yourno;

	DATATYPE d_type;		//�f�[�^�^�C�v��M�p�ϐ�
	char *d_s;				//�������M�p���I�z��
	int d_s_len;

	//char *a_content;
	//int content_len;

	//�������M�p�A�`���b�g1�s�p���I�z��m��
	GetArray1Dim(char, d_s, 0);
	//GetArray1Dim(char, a_content, 0);

	static int framecnt = 0;

	//�N���C�A���g�̌���������M�o�b�t�@�𒲂ׂ�
	for (i = 0; i < member_n; i++)
	{
		indi = member[i].index;

		if (member[i].flag == true && Client[indi].room_no == no)
		{
			//if (framecnt % 5 == 0)
			//{
				yourno = member_yourno(i);
				send_memberinfo(Client[indi].net.NHandle);
				send(Client[indi].net.NHandle, DAT_MAZE_MEMBER_YOURNO, &yourno, sizeof(int));
			//}

			//��M�T�C�Y�𒲂ׂ�
			leng = GetNetWorkDataLength(Client[indi].net.NHandle);

			//��M�����M�҂̕����Ɠ�������
			//��M����������
			while (leng >= sizeof(DATATYPE) && Client[indi].room_no == no && member[i].flag == true)
			{
				//�f�[�^�^�C�v���擾����
				NetWorkRecv(Client[indi].net.NHandle, &d_type, sizeof(DATATYPE));
				leng = GetNetWorkDataLength(Client[indi].net.NHandle);

				AddLogFiles("d_type = %d\n", d_type);

				//�f�[�^�^�C�v�ɉ���������
				switch (d_type)
				{
					//�`���b�g���N���C�A���g�����M
				case DAT_CHAT:
					//���O���o�^����Ă�����
					//��M
					d_s_len = recv(Client[indi].net.NHandle, d_type, &d_s);

					////�`���b�g���e�ɑ��M�Җ���ǉ�����
					//content_len = d_s_len + strlen(Client[indi].mdb.username) + 3 + 1;
					//ChangeArraySize_1dim(char, a_content, content_len + 1);
					//sprintf_s(a_content, content_len + 1, "%s : %s\n", Client[indi].mdb.username, d_s);

					////�S�̂̃`���b�g�L�^�ɒǉ�����
					//Chat.len += content_len;
					//ChangeArraySize_1dim(char, Chat.content, Chat.len + 1);
					//sprintf_s(Chat.content, Chat.len + 1, "%s%s", Chat.content, a_content);

					////���������̃����o�[�S���ɑ���
					//for (j = 0; j < client_n; j++)
					//{
					//	if (Client[indj].isuse == true && Client[indj].room_no == no)
					//	{
					//		send(Client[indj].net.NHandle, DAT_CHAT, a_content, sizeof(char)*(content_len + 1));
					//	}
					//}

					addchat("%s : %s\n", Client[indi].mdb.username, d_s);
					break;
					//�������甲����
				case DAT_DIS_ROOM:
					MazeRoom[Client[indi].room_no].dis(indi);
					member[i].flag = false;
					break;
					//���H�v���C�J�n
				case DAT_MAZE_START:
					member[i].ischallange = 1;						//���풆
					//member[i].mazetime		= 0;					//���Z�b�g
					//member[i].mazetime_st = GetNowCount();			//�X�^�[�g����
					addchat("%s���񂪃X�^�[�g���܂����I\n",
						Client[indi].mdb.username
						);
					break;
					//���H�S�[�����B
				case DAT_MAZE_GOAL:
					member[i].ischallange = 2;							//����ς�
					rank_decide();
					//{
					//	yourno = member_yourno(i);
					//	send_memberinfo(Client[indi].net.NHandle);
					//	send(Client[indi].net.NHandle, DAT_MAZE_MEMBER_YOURNO, &yourno, sizeof(int));
					//}
					/*
					send_memberinfo(Client[indi].net.NHandle);
					send(Client[indi].net.NHandle, DAT_MAZE_MEMBER_YOURNO, &yourno, sizeof(int));
					*/
					addchat("%s����̓^�C��%02d:%02d:%02d��%d�ʂɂȂ�܂����I\n",
						Client[indi].mdb.username,
						member[i].fs.mazetime / 60 / 60,
						member[i].fs.mazetime / 60 % 60,
						member[i].fs.mazetime % 60,
						member[i].rank
						);
					//�S�[�������畔�����甲����
					//MazeRoom[Client[indi].room_no].dis(i);
					break;

				case DAT_MAZE_CANCEL:
					member[i].ischallange = 0;							//�L�����Z�������̂Ŗ�����Ɠ����Ƃ���
					break;

				//case DAT_MAZE_MEMBER_DATA:
				//	recv(Client[indi].net.NHandle, d_type, &Client[indi].mdb);
				//	break;

				case DAT_MAZE_MEMBER_ADD:
					//recv(Client[indi].net.NHandle, d_type, &Client[indi].mdb);
					recv_memberinfo(i);
						break;
				}	//end switch

				ChangeArraySize_1dim(char, d_s, 0);
				leng = GetNetWorkDataLength(Client[indi].net.NHandle);
			}	// end while(��M�����M�҂̕�������M����������)
		}
	}

	framecnt++;

	//���I�z��폜
	DelArray1Dim(d_s);
	//DelArray1Dim(a_content);
}

void ROOMS::addchat(char *format, ...)
{
	va_list list;
	char buf[1024];
	int len;
	int i;
	int indi;

	va_start(list, format);
	vsprintf_s(buf, 256, format, list);
	va_end(list);

	len = strlen(buf);

	//�S�̂̃`���b�g�L�^�ɒǉ�����
	Chat.len += len;
	ChangeArraySize_1dim(char, Chat.content, Chat.len + 1);
	sprintf_s(Chat.content, Chat.len + 1, "%s%s", Chat.content, buf);

	//���������̃����o�[�S���ɑ���
	for (i = 0; i < member_n; i++)
	{
		indi = member[i].index;
		if (Client[indi].isuse == true && Client[indi].room_no == no && member[i].flag == true)
		{
			send(Client[indi].net.NHandle, DAT_CHAT, buf, sizeof(char)*(len + 1));
		}
	}
}

//���H�^�C�����Ƀ����N�t������
void ROOMS::rank_decide(void)
{
	int i, j;
	int itime, jtime;
	//int indi, indj;

	//���풆������ς݂͂��ׂ�1�ʂɂ���
	for (i = 0; i < member_n; i++)
	{
		if (member[i].ischallange == 2)
			member[i].rank = 1;			//�����l1��
		else if (member[i].ischallange == 1)
			member[i].rank = 32768;		//���풆��32768�ʁi���f�p�j
		else
			member[i].rank = -1;		//���ʂȂ�
	}

	//�����N�t�����s��
	for (i = 0; i < member_n - 1; i++)
	{
		//indi = member[i].index;
		for (j = i + 1; j < member_n; j++)
		{
			//indj = member[j].index;
			//��r�ΏۂƂ��āA
			//���풆�Ȃ獡�̃v���C���Ԃ��A
			//����ς݂Ȃ�ō��L�^���g�p
			switch (member[i].ischallange)
			{
			case 0: itime = -1;									break;	//������
			case 1: itime = member[i].fs.mazetime;				break;	//���풆
			case 2: itime = member[i].fs.mazetime;				break;	//����ς�
			}
			switch (member[j].ischallange)
			{
			case 0: jtime = -1;									break;	//������
			case 1: jtime = member[j].fs.mazetime;				break;	//���풆
			case 2: jtime = member[j].fs.mazetime;				break;	//����ς�
			}

			//�v���C���[�����풆������ς݂̏ꍇ��r����
			//�������ԂȂ瓯���ɂ���
			if (itime != jtime &&
				member[i].ischallange == 2 &&
				member[j].ischallange == 2)
			{
				if (itime > jtime)
				{
					member[i].rank++;
				}
				else
				{
					member[j].rank++;
				}
			}
		}
	}
}

//���������ʏ��Ƀ\�[�g����
void ROOMS::member_sort(void)
{
	int i, j;
	MEMBER_INFO temp;
	int indi, indj;

	//�I���\�[�g
	for (i = 0; i < member_n - 1; i++)
	{
		indi = member[i].index;
		for (j = i + 1; j < member_n; j++)
		{
			indj = member[j].index;
			if (member[i].rank > member[j].rank)
			{
				temp		= member[i];
				member[i]	= member[j];
				member[j]	= temp;
			}
		}
	}
	//qsort(member, member_n, sizeof(MEMBER_INFO), hikaku);
}

int  ROOMS::member_count(void)
{
	int i, cnt = 0;

	for (i = 0; i < member_n; i++)
	{
		if (member[i].ischallange >= 1)
			cnt++;
	}

	return cnt;
}

int  ROOMS::member_yourno(int index)
{
	int i, cnt = 0;

	for (i = 0; i < member_n && i < index; i++)
		if (member[i].ischallange >= 1)
			cnt++;

	return cnt;
}

void ROOMS::send_memberinfo(int NetHandle)
{
	int cnt, j;
	int indj;

	//���M�i����j
	//cnt = 0;
	//for (j = 0; j < member_n; j++)
	//{
	//	if (member[j].ischallange >= 1)
	//		cnt++;
	//}
	cnt = member_count();

	//���H��񑗐M
	send(NetHandle, DAT_MAZE_MEMBER_N, &cnt, sizeof(int));
	for (j = 0; j < member_n; j++)
	{
		indj = member[j].index;
		if (member[j].ischallange >= 1)
		{
			send(NetHandle, DAT_MAZE_MEMBER_ADD, NULL, 0);
			//send(Client[indi].net.NHandle, DAT_MAZE_MEMBER_USERNAME, Client[index[j]].mdb.username, strlen(mdb[j].username) + 1);
			send(NetHandle, DAT_MAZE_MEMBER_DATA, &Client[indj].mdb, sizeof(MAZE_DB));
			send(NetHandle, DAT_MAZE_MEMBER_RECODE, &member[j], sizeof(MEMBER_INFO));
		}
	}
}

int ROOMS::recv_memberinfo(int i)
{
	bool received = false;
	//int i;
	DATATYPE d_type;

	while (received == false)
	{
		if (GetNetWorkDataLength(Client[member[i].index].net.NHandle) >= sizeof(DATATYPE))
		{
			NetWorkRecv(Client[member[i].index].net.NHandle, &d_type, sizeof(DATATYPE));
			switch (d_type)
			{
			case DAT_MAZE_MEMBER_DATA:
				recv_val(Client[member[i].index].net.NHandle, d_type, &Client[member[i].index].mdb);
				break;
			case DAT_MAZE_MEMBER_RECODE:
				recv_val(Client[member[i].index].net.NHandle, d_type, &member[i].fs);
				received = true;
				break;
			}
		}
	}

	return 0;
}

void USERS::send_roominfo(int NetHandle)
{
	int j;

	send(NetHandle, DAT_ROOM_N, &MazeRoom_n, sizeof(int));
	for (j = 0; j < MazeRoom_n; j++)
	{
		//isuse = MazeRoom[j].isuse();
		//strcpy_s(name, 32, MazeRoom[j].GetName());
		send(NetHandle, DAT_ROOM_ADD, NULL, 0);
		send(NetHandle, DAT_ROOM_NAME, MazeRoom[j].Name, strlen(MazeRoom[j].Name) + 1);
		send(NetHandle, DAT_ROOM_MAZEINFO, &MazeRoom[j].minfo, sizeof(MAZEINFO));
		send(NetHandle, DAT_ROOM_ISUSE, &MazeRoom[j].isuse, sizeof(bool));
	}
}

void USERS::recv_mazeinfo(int NetHandle, int make_roomno_past)
{
	MAZEINFO minfo;
	DATATYPE d_type;
	bool received = false;
	char *d_s;
	int d_s_len;
	char test[32];

	GetArray1Dim(char, d_s, 0);

	if (make_roomno_past == -1)
		return;

	while (received == false)
	{
		if (GetNetWorkDataLength(NetHandle) >= sizeof(DATATYPE))
		{
			NetWorkRecv(NetHandle, &d_type, sizeof(DATATYPE));

			switch (d_type)
			{
			case DAT_MAZEINFO_NAME:
				d_s_len = recv(NetHandle, d_type, &d_s);
				//MazeRoom[room_no].minfo = minfo;
				strcpy_s(MazeRoom[make_roomno_past].Name, d_s_len + 1, d_s);
				strcpy_s(test, d_s_len + 1, d_s);
				break;
			case DAT_MAZEINFO_DATA:
				recv(NetHandle, d_type, &minfo);
				MazeRoom[make_roomno_past].minfo = minfo;
				received = true;
				break;
			}
		}
	}

	DelArray1Dim(d_s);
}

//�l�b�g���[�N���M�i�f�[�^�^�C�v�w�b�_�t���j
void send(int NetHandle, DATATYPE d_type, void *data, size_t size)
{
	NetWorkSend(NetHandle, &d_type, sizeof(DATATYPE));
	if (data != NULL)
		send_val(NetHandle, d_type, data, size);
}

//�l�b�g���[�N��M�i�f�[�^�^�C�v�w�b�_�t���j
int recv(int NetHandle, DATATYPE d_type, void *data)
{
	switch (d_type)
	{
		//������
	case DAT_CHAT:
	case DAT_NAME:
	case DAT_CREATE_ROOM:
	case DAT_MAZEINFO_NAME:
		return recv_str(NetHandle, d_type, (char**)data);
		break;
		//�ϐ��A�\����
	case DAT_JOIN_ROOM:
	case DAT_MAZE_MEMBER_DATA:
	case DAT_MAZEINFO_DATA:
		return recv_val(NetHandle, d_type, data);
		break;
	default:
		return -1;
	}
}

//�l�b�g���[�N�ϐ����M�i�f�[�^�^�C�v�w�b�_�t���j
void send_val(int NetHandle, DATATYPE d_type, void* data, size_t size)
{
	NetWorkSend(NetHandle, data, size);
}

//�l�b�g���[�N�������M�i�f�[�^�^�C�v�w�b�_�t���j
int recv_str(int NetHandle, DATATYPE d_type, char** data)
{
	int cursor = 0;

	do
	{
		while (GetNetWorkDataLength(NetHandle) < sizeof(char));
		ChangeArraySize_1dim(char, *data, cursor + 1);
		NetWorkRecv(NetHandle, &(*data)[cursor], sizeof(char));
		if ((*data)[cursor] == '\0')
			break;
		cursor++;
	} while (1);

	return cursor;
}

//�l�b�g���[�N�ϐ���M�i�f�[�^�^�C�v�w�b�_�t���j
int recv_val(int NetHandle, DATATYPE d_type, void* data)
{
	switch (d_type)
	{
		/*
	case DAT_JOIN_ROOM:
		while(GetNetWorkDataLength(NetHandle) < sizeof(int));
		NetWorkRecv(NetHandle, (int*)data, sizeof(int));
		return 0;
		break;
		*/
	case DAT_MAZE_MEMBER_DATA:
		while (GetNetWorkDataLength(NetHandle) < sizeof(MAZE_DB));
		NetWorkRecv(NetHandle, (MAZE_DB*)data, sizeof(MAZE_DB));
		return 0;
		break;
	case DAT_MAZEINFO_DATA:
		while (GetNetWorkDataLength(NetHandle) < sizeof(MAZEINFO));
		NetWorkRecv(NetHandle, (MAZEINFO*)data, sizeof(MAZEINFO));
		return 0;
		break;
	case DAT_MAZE_MEMBER_RECODE:
		while (GetNetWorkDataLength(NetHandle) < sizeof(FORSEND));
		NetWorkRecv(NetHandle, (FORSEND*)data, sizeof(FORSEND));
		return 0;
		break;
	default:
		while (GetNetWorkDataLength(NetHandle) < sizeof(int));
		NetWorkRecv(NetHandle, (int*)data, sizeof(int));
		return 0;
		break;
	}
}

int hikaku(const void *item1, const void *item2)
{
	return (*(MEMBER_INFO*)item1).rank - (*(MEMBER_INFO*)item2).rank;
}

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

		len = strlen(buf);

		MyLog.len += len;
		ChangeArraySize_1dim(char, MyLog.content, MyLog.len + 1);
		sprintf_s(MyLog.content, MyLog.len + 1, "%s%s", MyLog.content, buf);

		/*
		fopen_s(&fp, "MyLog.txt", "a");
		fprintf(fp, "%s", buf);
		fclose(fp);
		*/
	}
}
