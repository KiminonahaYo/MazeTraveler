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
//ログファイルセーブフラグ
//これをtrueにすればログファイルを保存します。
//-------------------------------------------------------------------------------------------

bool IsSaveLog = false;

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

typedef enum
{
	DAT_JOINED,
	DAT_DIS,
	DAT_NAME,

	//チャット
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

	//迷路
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
	int ischallange;		////0 : 未挑戦、1 : 挑戦中、2 : 挑戦済み
	bool flag;
}
MEMBER_INFO;

//迷路情報
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

//ネットワーク情報
typedef struct
{
	int NHandle;
	IPDATA Ip;
	//int db_index;
	//char *name;
	//GAMEINFO ginfo;
}
NETINFO;

//クライアント情報配列
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

//プロトタイプ宣言

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

	//ここから
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
	//ここまで

	StopListenNetWork();

	dest();
	return 0;		//正常終了
}

void init(void)
{
	//画面サイズとウィンドウモード設定
	SetGraphMode(640, 480, 32);
	ChangeWindowMode(true);										//ウィンドウモードで起動、コメントアウトでフルスクリーン
	SetDoubleStartValidFlag(true);
	SetWindowIconID(101);
	SetAlwaysRunFlag(true);
	SetOutApplicationLogValidFlag(false);

	//-------------------------------------------------------------------------------------------

	if (DxLib_Init() == -1) exit(-1);					//DXライブラリ初期化

	SetBackgroundColor(0, 0, 0);						//背景色
	SetDrawScreen(DX_SCREEN_BACK);						//裏画面に描画

	//DX ライブラリを初期化し、初期化が失敗したらプログラム終了
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

		DrawFormatString(10			, SCREEN_Y - 20, STR_COLOR, "ポート番号 > ");
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

	DxLib_End();	//DX ライブラリを終了
}

void disp(void)
{
	static int disproom = -1;

	//入力部
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

	//表示部（関数呼び出し）
	if (disproom == -1)
	{
		Users.disp();
	}
	else
	{
		MazeRoom[disproom].disp();
	}
}

//クライアント情報、ゲームデータベース配列確保
void USERS::init(void)
{
	GetArray1Dim(CLIENT, Client, 0);
	//GetArray1Dim(MAZE_DB, mdb, 0);
	client_n = 0;
}

//部屋に入っていない人の通信、
//接続、切断状態を調べ、処理する。
void USERS::update(void)
{
	CheckNewConnection();
	CheckLostConnection();

	Communicate();
}

//クライアント情報、ゲームデータベースを削除
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

	sprintf_s(UserList, 4096, "総合接続者一覧\n");
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

	//画面に収まる行以上のチャットがあったら左上座標を下げる
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

	//新しいメッセージが来たら
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

	DrawFormatString(10, SCREEN_Y - 20, STR_COLOR, "[ユーザー情報] Client接続:%02d件、%d部屋、ホイールOK、左クリックで切替", count_client(), MazeRoom_n);

	if (newinfoflag == true)
	{
		DrawBox(SCREEN_X - 100, 0, SCREEN_X, 20, 0x000000, TRUE);
		DrawFormatString(SCREEN_X - 100, 0, STR_COLOR, "新しい情報");
	}

	delsplit(spl_n, spldata);
}

//部屋作成
void ROOMS::init(int room_no, char *room_name)
{
	int len;

	//部屋番号と部屋名を登録
	no = room_no;
	strcpy_s(Name, 32, room_name);

	//チャット内容
	GetArray1Dim(char, Chat.content, 1);
	Chat.content[0] = '\0';
	Chat.len = 0;

	//クライアント、データベースアクセス用インデックス
	GetArray1Dim(MEMBER_INFO, member, 0);
	member_n = 0;

	//部屋は使用中
	isuse = true;

}

//部屋更新
//部屋内の通信などをする
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

//部屋削除
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

	//画面に収まる行以上のチャットがあったら左上座標を下げる
	if (spl_n * 20 + 10 >= 460)
	{
		pos_y_min = -(spl_n * 20 + 10 - 460);
	}
	else pos_y_min = 0;

	//新しいメッセージが来たら
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
		if (strstr(spldata[i], " : ") == NULL && strstr(spldata[i], "参加") != NULL)
			DrawFormatString(10, pos_y + 10 + 20 * i, STR_JOIN, "%s", spldata[i]);
		else if (strstr(spldata[i], " : ") == NULL && strstr(spldata[i], "退出") != NULL)
			DrawFormatString(10, pos_y + 10 + 20 * i, STR_DIS, "%s", spldata[i]);
		else if (strstr(spldata[i], "さんはタイム") != NULL && strstr(spldata[i], "位になりました！") != NULL)
			DrawFormatString(10, pos_y + 10 + 20 * i, STR_RANK, "%s", spldata[i]);
		else
			DrawFormatString(10, pos_y + 10 + 20 * i, STR_COLOR, "%s", spldata[i]);
	}

	DrawBox(0, SCREEN_Y - 20, SCREEN_X, SCREEN_Y, 0x000000, TRUE);
	DrawFormatString(10, SCREEN_Y - 20, STR_COLOR, "[チャット内容] プレイ中、完了%d人、ホイールOK、左クリック切替", member_count());

	if (newchatflag == true)
	{
		DrawBox(SCREEN_X - 100, 0, SCREEN_X, 20, 0x000000, TRUE);
		DrawFormatString(SCREEN_X - 100, 0, STR_COLOR, "新しいログ");
	}

	delsplit(spl_n, spldata);
}

//部屋の参加
void ROOMS::join(int userno)
{
	int i;

	char joinmessage[256];
	int len;

	Client[userno].room_no = no;

	//これまでの部屋のチャットを送信
	send(Client[userno].net.NHandle, DAT_CHAT, Chat.content, Chat.len + 1);

	//参加メッセージを送る
	sprintf_s(joinmessage, 256, "%sさんが迷路に参加しました。\n", Client[userno].mdb.username);
	len = strlen(joinmessage);

	//全体のチャット記録に追加する
	Chat.len += len;
	ChangeArraySize_1dim(char, Chat.content, Chat.len + 1);
	sprintf_s(Chat.content, Chat.len + 1, "%s%s", Chat.content, joinmessage);

	//同じ部屋のメンバー全員にチャットとして送る
	for (i = 0; i < client_n; i++)
	{
		if (Client[i].isuse == true && Client[i].room_no == Client[userno].room_no)
		{
			send(Client[i].net.NHandle, DAT_JOINED, joinmessage, sizeof(char)*(len + 1));
		}
	}

	//迷路情報（シード値、レベル）を参加者に送信
	//send(Client[userno].net.NHandle, DAT_MAZE_INFO, &minfo, sizeof(MAZEINFO));

	//ユーザー情報を部屋に追加
	ChangeArraySize_1dim(MEMBER_INFO, member, member_n + 1);
	member[member_n].index = userno;							//クライアント構造体配列添字
	member[member_n].fs.mazetime = 0;								//迷路挑戦時間
	//member[member_n].mazetime_best = 0;							//迷路挑戦時間
	member[member_n].ischallange = 0;								//0:未挑戦、1:挑戦中、2:挑戦済み
	member[member_n].rank = -1;									//順位
	member[member_n].flag = true;								//使用中フラグを立てる
	//Client[userno].member_no = member_n;						//ユーザー情報構造体側にメンバー番号を登録
	member_n++;													//参加人数をカウントアップ
}

//部屋の退出
void ROOMS::dis(int userno)
{
	int i;

	char dismessage[256];
	int len;

	//迷路退出メッセージ文字列を作成
	sprintf_s(dismessage, 256, "%sさんが迷路から退出しました。\n", Client[userno].mdb.username);
	len = strlen(dismessage);

	//迷路退出メッセージをチャットへ追加
	Chat.len += len;
	ChangeArraySize_1dim(char, Chat.content, Chat.len + 1);
	sprintf_s(Chat.content, Chat.len + 1, "%s%s", Chat.content, dismessage);

	//同じ部屋のメンバー全員に送る
	for (i = 0; i < client_n; i++)
	{
		if (Client[i].isuse == true && Client[i].room_no == Client[userno].room_no && i != userno)
		{
			send(Client[i].net.NHandle, DAT_DIS, dismessage, sizeof(char)*(len + 1));
		}
	}

	//member[Client[userno].member_no].flag = false;	//使用中フラグを降ろす
	Client[userno].room_no = -1;
	//Client[userno].member_no = -1;					//メンバー番号は退出後なのでなしにする
}

//部屋から退会させる
void ROOMS::taikai(int userno, char *reason)
{
	send(Client[userno].net.NHandle, DAT_TAIKAI_ROOM, reason, sizeof(reason) + 1);

	//全体のチャット記録に追加する
	Chat.len += sizeof(reason);
	ChangeArraySize_1dim(char, Chat.content, Chat.len + 1);
	sprintf_s(Chat.content, Chat.len + 1, "%s%s", Chat.content, reason);
}

//新しい接続者を検出、初期化。
void USERS::CheckNewConnection(void)
{
	int NetHandle_Temp;
	int cursor = 0;

	int i;

	//新しい接続者を確認
	NetHandle_Temp = GetNewAcceptNetWork();

	//新しい接続者を検出したら
	if (NetHandle_Temp != -1)
	{
		//接続者情報格納配列確保
		ChangeArraySize_1dim(CLIENT, Client, client_n + 1);
		//ChangeArraySize_1dim(MAZE_DB, mdb, client_n + 1);

		//初期化
		Client[client_n].net.NHandle = NetHandle_Temp;							//ハンドルは変数へ格納
		GetNetWorkIP(Client[client_n].net.NHandle, &Client[client_n].net.Ip);	//IPアドレスも一応格納
		Client[client_n].isuse = true;								//配列は使用します。
		Client[client_n].room_no = -1;								//部屋番号　部屋には入っていない（－１の場合）
		Client[client_n].mdb.Jiki.Pos.x = 0.0f;
		Client[client_n].mdb.Jiki.Pos.y = 0.0f;
		Client[client_n].mdb.Jiki.Pos.z = 0.0f;
		Client[client_n].mdb.Jiki.stopf = true;
		Client[client_n].mdb.Jiki.rotd = 0.0f;
		Client[client_n].mdb.Jiki.rot = 0.0;
		Client[client_n].mdb.CamKakudo = 0.0;
		Client[client_n].make_roomno = -1;
		//Client[client_n].member_no			= -1;
		strcpy_s(Client[client_n].mdb.username, 32, "");						//ユーザー名は送信されるまで空文字列

		//迷路ルーム情報送信
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

		//接続者カウントを増加
		client_n++;
	}
}

//切断者を検出、後処理。
void USERS::CheckLostConnection(void)
{
	int NetHandle_Temp;

	int i;

	//切断者を確認
	NetHandle_Temp = GetLostNetWork();

	//切断者はだれか探す
	for (i = 0; i < client_n; i++)
	{
		//接続フラグが立っていたら
		if (Client[i].isuse == true)
		{
			//切断者が見つかったら
			if (Client[i].net.NHandle == NetHandle_Temp)
			{
				Client[i].isuse = false;						//接続フラグ降ろす

				//部屋に入っていたら退出。
				if (Client[i].room_no != -1)
					MazeRoom[Client[i].room_no].dis(i);

				break;
			}
		}
	}
}

//現在接続しているクライアントを数える
int count_client(void)
{
	int i, cnt = 0;

	//フラグが立っている個数を数える。
	for (i = 0; i < client_n; i++)
	{
		if (Client[i].isuse == true)
			cnt++;
	}
	return cnt;
}

//部屋に入っていないクライアントと通信（受信、定期送信に限る）
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

	//クライアントの個数分だけ受信バッファを調べる
	for (i = 0; i < client_n; i++)
	{
		//クライアントが接続されていたら
		if (Client[i].isuse == true && Client[i].room_no == no)
		{
			//定期送信-------------
			//if (Client[i].room_no == no)
			if (framecnt % 3 == 0)
			{
				send_roominfo(Client[i].net.NHandle);
			}

			//受信-----------------
			//受信サイズを調べる
			leng = GetNetWorkDataLength(Client[i].net.NHandle);

			//どこも部屋に入っていなかったら（no = -1）
			//受信があったら
			while (leng >= sizeof(DATATYPE) && Client[i].room_no == no && Client[i].isuse == true)
			{
				//データタイプを取得する
				NetWorkRecv(Client[i].net.NHandle, &d_type, sizeof(DATATYPE));
				leng = GetNetWorkDataLength(Client[i].net.NHandle);

				AddLogFiles("d_type = %d\n", d_type);

				//データタイプに応じた処理
				switch (d_type)
				{
				//名前を受信
				case DAT_NAME:
					//受信
					d_s_len = recv(Client[i].net.NHandle, d_type, &d_s);

					//名前を登録する
					//ChangeArraySize_1dim(char, Client[i].mdb.username, d_s_len + 1);
					sprintf_s(Client[i].mdb.username, d_s_len + 1, "%s", d_s);

					break;

				//部屋への参加
				case DAT_JOIN_ROOM:
					recv(Client[i].net.NHandle, d_type, &Client[i].room_no);
					MazeRoom[Client[i].room_no].join(i);
					break;

				//部屋の作成
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

				//シード値、レベル受信
				case DAT_MAZEINFO:
					recv_val(Client[i].net.NHandle, DAT_MAZEINFO, &room_no);

					if (room_no == -1)
						room_no = Client[i].make_roomno;

					recv_mazeinfo(Client[i].net.NHandle, room_no);

					//参加メッセージを送る
					MazeRoom[room_no].addchat(
						"------------------------------------------\n"
						"ステージ名：%s\n"
						"------------------------------------------\n",
						MazeRoom[room_no].Name);

					Client[i].make_roomno = -1;

					/*
					//全体のチャット記録に追加する
					Chat.len += len;
					ChangeArraySize_1dim(char, Chat.content, Chat.len + 1);
					sprintf_s(Chat.content, Chat.len + 1, "%s%s", Chat.content, headmessage);
					break;
					*/
					break;

				//部屋の破棄
				case DAT_DEST_ROOM:
					MazeRoom[Client[i].room_no].dest();
					for (j = 0; j < client_n; j++)
					{
						if (Client[i].room_no == Client[j].room_no && Client[j].isuse == true)
						{
							MazeRoom[Client[i].room_no].taikai(j, "部屋が管理者によって破棄されました。");
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

//部屋に入っているクライアントと通信（部屋ごとに対応する）
void ROOMS::Communicate(void)
{
	int i, j;				//部屋内ユーザー番号
	int indi, indj;			//部屋内ユーザー番号から全体ユーザー番号への変換値
	int cnt;
	int leng;
	int yourno;

	DATATYPE d_type;		//データタイプ受信用変数
	char *d_s;				//文字列受信用動的配列
	int d_s_len;

	//char *a_content;
	//int content_len;

	//文字列受信用、チャット1行用動的配列確保
	GetArray1Dim(char, d_s, 0);
	//GetArray1Dim(char, a_content, 0);

	static int framecnt = 0;

	//クライアントの個数分だけ受信バッファを調べる
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

			//受信サイズを調べる
			leng = GetNetWorkDataLength(Client[indi].net.NHandle);

			//受信が送信者の部屋と同じかつ
			//受信があったら
			while (leng >= sizeof(DATATYPE) && Client[indi].room_no == no && member[i].flag == true)
			{
				//データタイプを取得する
				NetWorkRecv(Client[indi].net.NHandle, &d_type, sizeof(DATATYPE));
				leng = GetNetWorkDataLength(Client[indi].net.NHandle);

				AddLogFiles("d_type = %d\n", d_type);

				//データタイプに応じた処理
				switch (d_type)
				{
					//チャットをクライアントから受信
				case DAT_CHAT:
					//名前が登録されていたら
					//受信
					d_s_len = recv(Client[indi].net.NHandle, d_type, &d_s);

					////チャット内容に送信者名を追加する
					//content_len = d_s_len + strlen(Client[indi].mdb.username) + 3 + 1;
					//ChangeArraySize_1dim(char, a_content, content_len + 1);
					//sprintf_s(a_content, content_len + 1, "%s : %s\n", Client[indi].mdb.username, d_s);

					////全体のチャット記録に追加する
					//Chat.len += content_len;
					//ChangeArraySize_1dim(char, Chat.content, Chat.len + 1);
					//sprintf_s(Chat.content, Chat.len + 1, "%s%s", Chat.content, a_content);

					////同じ部屋のメンバー全員に送る
					//for (j = 0; j < client_n; j++)
					//{
					//	if (Client[indj].isuse == true && Client[indj].room_no == no)
					//	{
					//		send(Client[indj].net.NHandle, DAT_CHAT, a_content, sizeof(char)*(content_len + 1));
					//	}
					//}

					addchat("%s : %s\n", Client[indi].mdb.username, d_s);
					break;
					//部屋から抜ける
				case DAT_DIS_ROOM:
					MazeRoom[Client[indi].room_no].dis(indi);
					member[i].flag = false;
					break;
					//迷路プレイ開始
				case DAT_MAZE_START:
					member[i].ischallange = 1;						//挑戦中
					//member[i].mazetime		= 0;					//リセット
					//member[i].mazetime_st = GetNowCount();			//スタート時刻
					addchat("%sさんがスタートしました！\n",
						Client[indi].mdb.username
						);
					break;
					//迷路ゴール到達
				case DAT_MAZE_GOAL:
					member[i].ischallange = 2;							//挑戦済み
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
					addchat("%sさんはタイム%02d:%02d:%02dで%d位になりました！\n",
						Client[indi].mdb.username,
						member[i].fs.mazetime / 60 / 60,
						member[i].fs.mazetime / 60 % 60,
						member[i].fs.mazetime % 60,
						member[i].rank
						);
					//ゴールしたら部屋から抜ける
					//MazeRoom[Client[indi].room_no].dis(i);
					break;

				case DAT_MAZE_CANCEL:
					member[i].ischallange = 0;							//キャンセルしたので未挑戦と同じとする
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
			}	// end while(受信が送信者の部屋かつ受信があったら)
		}
	}

	framecnt++;

	//動的配列削除
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

	//全体のチャット記録に追加する
	Chat.len += len;
	ChangeArraySize_1dim(char, Chat.content, Chat.len + 1);
	sprintf_s(Chat.content, Chat.len + 1, "%s%s", Chat.content, buf);

	//同じ部屋のメンバー全員に送る
	for (i = 0; i < member_n; i++)
	{
		indi = member[i].index;
		if (Client[indi].isuse == true && Client[indi].room_no == no && member[i].flag == true)
		{
			send(Client[indi].net.NHandle, DAT_CHAT, buf, sizeof(char)*(len + 1));
		}
	}
}

//迷路タイム順にランク付けする
void ROOMS::rank_decide(void)
{
	int i, j;
	int itime, jtime;
	//int indi, indj;

	//挑戦中か挑戦済みはすべて1位にする
	for (i = 0; i < member_n; i++)
	{
		if (member[i].ischallange == 2)
			member[i].rank = 1;			//初期値1位
		else if (member[i].ischallange == 1)
			member[i].rank = 32768;		//挑戦中は32768位（判断用）
		else
			member[i].rank = -1;		//順位なし
	}

	//ランク付けを行う
	for (i = 0; i < member_n - 1; i++)
	{
		//indi = member[i].index;
		for (j = i + 1; j < member_n; j++)
		{
			//indj = member[j].index;
			//比較対象として、
			//挑戦中なら今のプレイ時間を、
			//挑戦済みなら最高記録を使用
			switch (member[i].ischallange)
			{
			case 0: itime = -1;									break;	//未挑戦
			case 1: itime = member[i].fs.mazetime;				break;	//挑戦中
			case 2: itime = member[i].fs.mazetime;				break;	//挑戦済み
			}
			switch (member[j].ischallange)
			{
			case 0: jtime = -1;									break;	//未挑戦
			case 1: jtime = member[j].fs.mazetime;				break;	//挑戦中
			case 2: jtime = member[j].fs.mazetime;				break;	//挑戦済み
			}

			//プレイヤーが挑戦中か挑戦済みの場合比較する
			//同じ時間なら同率にする
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

//索引を順位順にソートする
void ROOMS::member_sort(void)
{
	int i, j;
	MEMBER_INFO temp;
	int indi, indj;

	//選択ソート
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

	//送信（定期）
	//cnt = 0;
	//for (j = 0; j < member_n; j++)
	//{
	//	if (member[j].ischallange >= 1)
	//		cnt++;
	//}
	cnt = member_count();

	//迷路情報送信
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

//ネットワーク送信（データタイプヘッダ付き）
void send(int NetHandle, DATATYPE d_type, void *data, size_t size)
{
	NetWorkSend(NetHandle, &d_type, sizeof(DATATYPE));
	if (data != NULL)
		send_val(NetHandle, d_type, data, size);
}

//ネットワーク受信（データタイプヘッダ付き）
int recv(int NetHandle, DATATYPE d_type, void *data)
{
	switch (d_type)
	{
		//文字列
	case DAT_CHAT:
	case DAT_NAME:
	case DAT_CREATE_ROOM:
	case DAT_MAZEINFO_NAME:
		return recv_str(NetHandle, d_type, (char**)data);
		break;
		//変数、構造体
	case DAT_JOIN_ROOM:
	case DAT_MAZE_MEMBER_DATA:
	case DAT_MAZEINFO_DATA:
		return recv_val(NetHandle, d_type, data);
		break;
	default:
		return -1;
	}
}

//ネットワーク変数送信（データタイプヘッダ付き）
void send_val(int NetHandle, DATATYPE d_type, void* data, size_t size)
{
	NetWorkSend(NetHandle, data, size);
}

//ネットワーク文字列受信（データタイプヘッダ付き）
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

//ネットワーク変数受信（データタイプヘッダ付き）
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
