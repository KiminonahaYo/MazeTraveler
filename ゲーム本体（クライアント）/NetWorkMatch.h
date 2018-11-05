#ifndef NETWORKMATCH_DEFINED
#define NETWORKMATCH_DEFINED

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

typedef struct
{
	char *content;
	int len;
}
CHAT;

typedef struct
{
	int seed;
	int level;
	bool iskeymode;
}
MAZEINFO;

typedef struct
{
	char *name;
	bool isuse;
	MAZEINFO minfo;
}
ROOMS;

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

typedef struct
{
	ROOMS *room;
	int room_n;
}
ROOM_RET;

typedef struct
{
	CHAT Chat;

	MAZE_DB *mdb;
	int mdb_n;

	MEMBER_INFO *member;
	int member_n;

	int yourno;
}
MEMBER_RET;

class NetWorkMatch
{
private:
	CHAT Chat;

	ROOMS *room = NULL;
	int room_n = 0;

	MAZE_DB *mdb = NULL;
	int mdb_n = 0;

	MEMBER_INFO *member = NULL;
	int member_n = 0;

	int yourno = -1;

	//bool activeflag;

	MEDIA_HANDLER NetHandle;
	MAZEINFO minfo;

	//MAZE_DB mdb_mine;

	bool member_f = false;
	//bool minfo_f  = false;
	bool room_f   = false;
	bool challange_f = false;
	bool chat_f = false;

public:
	void init(MEDIA_HANDLER);
	void dest(void);
	int update(void);
	int checklost(void);

	bool GetMemberInfo(MEMBER_RET*);
	bool GetRoomInfo(ROOM_RET*);
	//bool GetMazeInfo(MAZEINFO*);
	void Make_Room(void);
	void SetMazeInfo(int, char*, MAZEINFO);
	void SetMyInfo(MAZE_DB, FORSEND);
	void send_chat(char *);
	void send_name(char *);
	void maze_start(void);
	void maze_goal(void);
	void maze_cancel(void);
	void room_join(int);
	void room_dis(void);

	void send(DATATYPE, void*, size_t);
	int	 recv(DATATYPE, void*);

	NetWorkMatch(void)
	{
		room	= NULL;
		mdb		= NULL;
		//activeflag = false;
	}

private:
	void send_val(DATATYPE, void*, size_t);
	void send_str(DATATYPE, char*);
	int recv_str(DATATYPE, char**);
	int recv_val(DATATYPE, void*);
	int recv_roominfo(void);
	int recv_mazeinfo(void);

	int communicate(void);
}
NetMatch;

int hikaku(const void *, const void *);

int NetWorkMatch::update(void)
{
	return communicate();
	//qsort(member, member_n, sizeof(MEMBER_DATA), hikaku);
}

void NetWorkMatch::init(MEDIA_HANDLER NetHandle_)
{
	NetHandle = NetHandle_;

	GetArray1Dim(char, Chat.content, 1);
	Chat.content[0] = '\0';
	Chat.len = 0;

	GetArray1Dim(ROOMS, room, 0);
	room_n = 0;

	GetArray1Dim(MAZE_DB, mdb, 0);
	mdb_n = 0;

	GetArray1Dim(MEMBER_INFO, member, 0);
	member_n = 0;

	yourno = -1;
}

void NetWorkMatch::dest(void)
{
	int i;

	DelArray1Dim(Chat.content);

	for (i = 0; i < room_n; i++)
		DelArray1Dim(room[i].name);
	DelArray1Dim(room);

	DelArray1Dim(mdb);

	DelArray1Dim(member);

	member_f = false;
	room_f = false;
	challange_f = false;
	chat_f = false;
}

bool NetWorkMatch::GetMemberInfo(MEMBER_RET* ret_)
{
	MEMBER_RET ret;

	ret.Chat		= Chat;
	ret.mdb			= mdb;
	ret.mdb_n		= mdb_n;
	ret.member		= member;
	ret.member_n	= member_n;
	ret.yourno		= yourno;
	//	ret.flag	 = activeflag;
	*ret_ = ret;

	return member_f && chat_f;
}

void NetWorkMatch::SetMyInfo(MAZE_DB mdb_, FORSEND fs_)
{
	send(DAT_MAZE_MEMBER_ADD, NULL, 0);
	send(DAT_MAZE_MEMBER_DATA, &mdb_, sizeof(mdb_));
	send(DAT_MAZE_MEMBER_RECODE, &fs_, sizeof(fs_));
}

void NetWorkMatch::Make_Room(void)
{
	send(DAT_CREATE_ROOM, NULL, 0);
}

bool NetWorkMatch::GetRoomInfo(ROOM_RET *ret_)
{
	ROOM_RET ret;

	ret.room = room;
	ret.room_n = room_n;
	//	ret.flag	 = activeflag;
	*ret_ = ret;

	return room_f;
}

void NetWorkMatch::SetMazeInfo(int no, char *name, MAZEINFO minfo_)
{
	send(DAT_MAZEINFO, &no, sizeof(int));
	send(DAT_MAZEINFO_NAME, name, NULL);
	send(DAT_MAZEINFO_DATA, &minfo_, sizeof(MAZEINFO));
	minfo = minfo_;
}

//bool NetWorkMatch::GetMazeInfo(MAZEINFO *minfo_)
//{
//	*minfo_ = minfo;
//	return minfo_f;
//}

void NetWorkMatch::send_chat(char *value)
{
	send(DAT_CHAT, value, strlen(value) + 1);
}

void NetWorkMatch::send_name(char *name)
{
	send(DAT_NAME, name, strlen(name) + 1);
}

void NetWorkMatch::room_join(int room_no)
{
	send(DAT_JOIN_ROOM, &room_no, sizeof(int));

	member_f = false;
	chat_f = false;

	ChangeArraySize_1dim(char, Chat.content, 1);
	Chat.content[0] = '\0';
	Chat.len = 0;

	ChangeArraySize_1dim(MAZE_DB, mdb, 0);
	mdb_n = 0;

	ChangeArraySize_1dim(MEMBER_INFO, member, 0);
	member_n = 0;
}

void NetWorkMatch::room_dis(void)
{
	send(DAT_DIS_ROOM, NULL, 0);
	//member_f = false;
}

void NetWorkMatch::maze_start(void)
{
	send(DAT_MAZE_START, NULL, 0);
	challange_f = true;
}

void NetWorkMatch::maze_goal(void)
{
	send(DAT_MAZE_GOAL, NULL, 0);
	challange_f = false;
}

void NetWorkMatch::maze_cancel(void)
{
	send(DAT_MAZE_CANCEL, NULL, 0);
	challange_f = false;
}

void NetWorkMatch::send(DATATYPE d_type, void *data, size_t size = 0)
{
	NetWorkSend(NetHandle, &d_type, sizeof(DATATYPE));
	if (data != NULL)
	{
		switch (d_type)
		{
		case DAT_NAME:
		case DAT_CHAT:
		case DAT_MAZEINFO_NAME:
			send_str(d_type, (char*)data);
			break;
		default:
			send_val(d_type, data, size);
			break;
		}
	}
}

void NetWorkMatch::send_str(DATATYPE d_type, char* data)
{
	NetWorkSend(NetHandle, data, sizeof(char)*(strlen(data) + 1));
}

int NetWorkMatch::recv(DATATYPE d_type, void *data)
{
	switch (d_type)
	{
	case DAT_CHAT:
	case DAT_JOINED:
	case DAT_DIS:
	case DAT_TAIKAI_ROOM:
		return recv_str(d_type, (char**)data);
		break;
		/*
	case DAT_MAZEINFO:
		return recv_val(d_type, data);
		break;
		*/
	case DAT_ROOM_N:
		return recv_roominfo();
		break;
	case DAT_MAZE_MEMBER_N:
		return recv_mazeinfo();
		break;
	default:
		return -1;
		break;
	}
}

void NetWorkMatch::send_val(DATATYPE d_type, void* data, size_t size)
{
	NetWorkSend(NetHandle, data, size);
}

int NetWorkMatch::recv_str(DATATYPE d_type, char** data)
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

int NetWorkMatch::recv_val(DATATYPE d_type, void* data)
{
	switch (d_type)
	{
	case DAT_ROOM_ISUSE:
		while (GetNetWorkDataLength(NetHandle) < sizeof(bool));
		NetWorkRecv(NetHandle, (bool*)data, sizeof(bool));
		return 0;
		break;
	case DAT_MAZE_MEMBER_DATA:
		while (GetNetWorkDataLength(NetHandle) < sizeof(MAZE_DB));
		NetWorkRecv(NetHandle, (MAZE_DB*)data, sizeof(MAZE_DB));
		return 0;
		break;
		/*
	case DAT_MAZEINFO:
		while (GetNetWorkDataLength(NetHandle) < sizeof(MAZEINFO));
		NetWorkRecv(NetHandle, (MAZEINFO*)data, sizeof(MAZEINFO));
		return 0;
		break;
		*/
	case DAT_MAZE_MEMBER_RECODE:
		while (GetNetWorkDataLength(NetHandle) < sizeof(MEMBER_INFO));
		NetWorkRecv(NetHandle, (MEMBER_INFO*)data, sizeof(MEMBER_INFO));
		return 0;
		break;
	case DAT_ROOM_MAZEINFO:
		while (GetNetWorkDataLength(NetHandle) < sizeof(MAZEINFO));
		NetWorkRecv(NetHandle, (MAZEINFO*)data, sizeof(MAZEINFO));
		return 0;
		break;
	default:
		while (GetNetWorkDataLength(NetHandle) < sizeof(int));
		NetWorkRecv(NetHandle, (int*)data, sizeof(int));
		return 0;
		break;
	}
}

int NetWorkMatch::recv_roominfo(void)
{
	int cnt = 0;
	int i;
	DATATYPE d_type;

	for (i = 0; i < room_n; i++)
		DelArray1Dim(room[i].name);
	//DelArray1Dim(room);

	recv_val(DAT_ROOM_N, &room_n);

	//GetArray1Dim(ROOMS, room, 0);

	while (cnt < room_n)
	{
		ProcessMessage();
		if (GetNetWorkDataLength(NetHandle) >= sizeof(DATATYPE))
		{
			NetWorkRecv(NetHandle, &d_type, sizeof(DATATYPE));

			switch (d_type)
			{
			case DAT_ROOM_ADD:
				ChangeArraySize_1dim(ROOMS, room, cnt + 1);
				break;
			case DAT_ROOM_NAME:
				GetArray1Dim(char, room[cnt].name, 0);
				recv_str(d_type, &room[cnt].name);
				break;
			case DAT_ROOM_MAZEINFO:
				recv_val(d_type, &room[cnt].minfo);
				break;
			case DAT_ROOM_ISUSE:
				recv_val(d_type, &room[cnt].isuse);
				cnt++;
				break;
			}
		}
	}

	return 0;
}

int NetWorkMatch::recv_mazeinfo(void)
{
	int cnt = 0;
	static int kazu;
	static bool kazu_initflag = true;
	bool recvflag = false;
	DATATYPE d_type;
	int ret = -1;

	//DelArray1Dim(mdb);
	//DelArray1Dim(member);

	//GetArray1Dim(MAZE_DB, mdb, 0);
	//GetArray1Dim(MEMBER_INFO, member, 0);

	while (1)
	{
		if (kazu_initflag)
		{
			recv_val(DAT_MAZE_MEMBER_N, &kazu);
			kazu_initflag = false;
		}

		if (GetNetWorkDataLength(NetHandle) <
			sizeof(DATATYPE)* (3 * kazu + 1) +
			sizeof(MAZE_DB)* kazu +
			sizeof(MEMBER_INFO)* kazu +
			sizeof(int))
		{
			ret = -1;
			break;
		}
		else
		{
			recvflag = true;
		}

		if (recvflag)
		{
			member_n = mdb_n = kazu;
			cnt = 0;

			while (cnt < mdb_n)
			{
				ProcessMessage();
				if (GetNetWorkDataLength(NetHandle) >= sizeof(DATATYPE))
				{
					NetWorkRecv(NetHandle, &d_type, sizeof(DATATYPE));
					switch (d_type)
					{
					case DAT_MAZE_MEMBER_ADD:
						ChangeArraySize_1dim(MAZE_DB, mdb, cnt + 1);
						ChangeArraySize_1dim(MEMBER_INFO, member, cnt + 1);
						break;
						/*
					case DAT_MAZE_MEMBER_USERNAME:
					GetArray1Dim(char, member[cnt].name, 0);
					recv_str(d_type, &member[cnt].name);
					break;
					*/
					case DAT_MAZE_MEMBER_DATA:
						recv_val(d_type, &mdb[cnt]);
						break;
					case DAT_MAZE_MEMBER_RECODE:
						recv_val(d_type, &member[cnt]);
						cnt++;
						break;
					}
				}
			}
			while (1)
			{
				ProcessMessage();
				if (GetNetWorkDataLength(NetHandle) >= sizeof(DATATYPE))
				{
					NetWorkRecv(NetHandle, &d_type, sizeof(DATATYPE));
					if (d_type == DAT_MAZE_MEMBER_YOURNO)
					{
						recv_val(DAT_MAZE_MEMBER_YOURNO, &yourno);
						break;
					}
				}
			}
			kazu_initflag = true;
			ret = 0;

			if (GetNetWorkDataLength(NetHandle) >= sizeof(DATATYPE))
			{
				NetWorkRecvToPeek(NetHandle, &d_type, sizeof(DATATYPE));
				if (d_type != DAT_MAZE_MEMBER_N)
				{
					ret = 0;
					break;
				}
				else
				{
					NetWorkRecv(NetHandle, &d_type, sizeof(DATATYPE));
				}
			}
			else
			{
				ret = 0;
				break;
			}
		}
	}

	return ret;
}

int NetWorkMatch::communicate(void)
{
	DATATYPE d_type;
	int len;
	char *d_s;
	static bool member_receiving_f = false;
	int ret;

	GetArray1Dim(char, d_s, 0);

	if(GetNetWorkDataLength(NetHandle) >= sizeof(DATATYPE))
	{
		if (member_receiving_f == false)
		{
			NetWorkRecv(NetHandle, &d_type, sizeof(DATATYPE));

			//clsDx();
			//AddLogFiles("d_type = %d\n", d_type);

			switch (d_type)
			{
			case DAT_JOINED:
			case DAT_DIS:
			case DAT_CHAT:
			case DAT_TAIKAI_ROOM:
				len = recv_str(d_type, &d_s);

				Chat.len += len;
				ChangeArraySize_1dim(char, Chat.content, Chat.len + 1);
				sprintf_s(Chat.content, Chat.len + 1, "%s%s", Chat.content, d_s);
				chat_f = true;

				break;
				/*
			case DAT_MAZE_INFO:
			recv(d_type, &minfo);
			//minfo_f = true;
			break;
			*/
			case DAT_ROOM_N:
				recv(d_type, NULL);
				room_f = true;
				break;
			case DAT_MAZE_MEMBER_N:
				member_receiving_f = true;

				ret = recv(d_type, NULL);
				if (ret == 0)
				{
					member_receiving_f = false;
					member_f = true;
				}
				break;
			}
		}
		else
		{
			ret = recv(DAT_MAZE_MEMBER_N, NULL);
			if (ret == 0)
			{
				member_receiving_f = false;
				member_f = true;
			}
		}
	}

	DelArray1Dim(d_s);

	return GetNetWorkDataLength(NetHandle);
}

int NetWorkMatch::checklost(void)
{
	int lost_handle;

	lost_handle = GetLostNetWork();

	if (lost_handle == NetHandle)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

#endif