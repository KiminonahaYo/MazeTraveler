/*迷路作成ヘッダ*/

#ifndef MYMAZE_DEFINED

#define MYMAZE_DEFINED

#include "Defines.h"

#define ROAD 0									//迷路道
#define WALL 1									//迷路壁

void SetPoint(int **,int,int,int);				//迷路の配列の要素の内容を変更する
int GetPoint(int **,int,int);					//迷路の配列の要素の内容を取得する

void Maze_Init(int **, int,int);				//迷路を作る準備をする
void Maze_Sub(int **, int,int);					//迷路を作る。何回も呼び出される。

void GetMazeArray(int ***,int,int);				//迷路用の配列確保
void DeleteMazeArray(int **);					//迷路用の配列開放

void MakeMaze(int**, unsigned int, int, int);	//迷路を作成する関数メイン
void MazeLog(int**, int, int);					//迷路のログを残す

//-------------------------------------------------------------------------------------

//迷路のための配列確保
void GetMazeArray(int ***MazeData, int Size_X, int Size_Y)
{
	int i;

	(*MazeData) = (int **)malloc(sizeof(int*)*Size_X);
	for(i=0; i<Size_X; i++) (*MazeData)[i]=(int *)malloc(sizeof(int)*Size_Y);
}

//迷路の配列開放
void DeleteMazeArray(int **MazeData, int Size_X)
{
	int x;

	for(x=0; x<Size_X; x++)
		free(MazeData[x]);
	free(MazeData);
}

//迷路の配列の要素の内容を変更する
void SetPoint(int **MazeData, int x, int y, int Type)
{
	//AddLogFiles("Pos(%d,%d) = %s\n", x, y, Type == 0 ? "ROAD" : "WALL");
	MazeData[x][y]=Type;
}

//迷路の配列の要素の内容を取得する
int GetPoint(int **MazeData, int x, int y)
{
	return MazeData[x][y];
}

//-------------------------------------------------------------------------------------

//迷路を作成する関数メイン
void MakeMaze(int **MazeData, unsigned int seed, int Size_X, int Size_Y)
{
	int i, xi, yi;
	int mazetype;

	//シード値からランダム関数を初期化する。
	srand(seed);

	//迷路を初期化する。
	Maze_Init(MazeData, Size_X, Size_Y);

	//----------------迷路を作成------------------
	mazetype = RangeRand(0, 9);			//迷路の正解ルートの傾向を決める

	//正解ルートが真ん中を突き抜ける傾向がある。
	//ゴール
	//↑←
	//　↑←
	//　　↑←
	//　　　↑←
	//　　　　↑←
	//　　　　　↑←スタート　
	if (mazetype >= 0 && mazetype < 6)
	{
		//迷路を作成
		for (i = 2; i <= max(Size_X, Size_Y) - 3; i += 2)
		{
			for (xi = 2; xi <= Size_X - 3 && xi < i + 2; xi += 2)
			{
				for (yi = 2; yi <= Size_Y - 3 && yi < xi + 2; yi += 2)
				{
					Maze_Sub(MazeData, xi, yi);
				}
			}
			for (yi = 2; yi <= Size_Y - 3 && yi < i; yi += 2)
			{
				for (xi = 2; xi <= Size_X - 3 && xi < yi + 2; xi += 2)
				{
					Maze_Sub(MazeData, xi, yi);
				}
			}
		}
		//迷路が作られていない場所を埋める
		for (xi = 2; xi <= Size_X - 3; xi += 2)
		{
			for (yi = 2; yi <= Size_Y - 3; yi += 2)
			{
				Maze_Sub(MazeData, xi, yi);
			}
		}
	}
	//正解ルートが最初、スタート位置から見て左に寄る傾向がある
	//ゴール
	//↑
	//↑
	//↑
	//↑
	//↑
	//↑←←←←←スタート
	else if (mazetype >= 6 && mazetype < 8)
	{
		//迷路を作成
		for (xi = 2; xi <= Size_X - 3; xi += 2)
		{
			for (yi = 2; yi <= Size_Y - 3; yi += 2)
			{
				Maze_Sub(MazeData, xi, yi);
			}
		}
		//迷路が作られていない場所を埋める
		for (xi = 2; xi <= Size_X - 3; xi += 2)
		{
			for (yi = 2; yi <= Size_Y - 3; yi += 2)
			{
				Maze_Sub(MazeData, xi, yi);
			}
		}
	}
	//正解ルートが最初、スタート位置から見て正面に寄る傾向がある
	//ゴール←←←←
	//　　　　　　↑
	//　　　　　　↑
	//　　　　　　↑
	//　　　　　　↑
	//　　　　　　↑
	//　　　スタート
	else
	{
		//迷路を作成
		for (yi = 2; yi <= Size_Y - 3; yi += 2)
		{
			for (xi = 2; xi <= Size_X - 3; xi += 2)
			{
				Maze_Sub(MazeData, xi, yi);
			}
		}
		//迷路が作られていない場所を埋める
		for (yi = 2; yi <= Size_Y - 3; yi += 2)
		{
			for (xi = 2; xi <= Size_X - 3; xi += 2)
			{
				Maze_Sub(MazeData, xi, yi);
			}
		}
	}

	//スタート
	SetPoint(MazeData, 1, 2, ROAD);
	SetPoint(MazeData, 0, 2, ROAD);
	SetPoint(MazeData, 0, 1, WALL);
	SetPoint(MazeData, 0, 3, WALL);

	//ゴール
	SetPoint(MazeData, Size_X - 2, Size_Y - 3, ROAD);
	SetPoint(MazeData, Size_X - 1, Size_Y - 3, ROAD);
	SetPoint(MazeData, Size_X - 1, Size_Y - 2, WALL);
	SetPoint(MazeData, Size_X - 1, Size_Y - 4, WALL);

	//MazeLog(MazeData, Size_X, Size_Y);
}

//-------------------------------------------------------------------------------------
//ログを残す（デバッグ用）
void MazeLog(int** MazeData, int Size_X, int Size_Y)
{
	int xi, yi;
	FILE *fp;
	char filename[256];
	int fileno = 0;

	MakeDir("./Logs");
	do
	{
		fileno++;
		sprintf_s(filename, 256, "./Logs/MazeLog%d.txt", fileno);
	}
	while (exist(filename) == true);

	fopen_s(&fp, filename, "w");

	for (xi = 0; xi < Size_X; xi++)
	{
		for (yi = 0; yi < Size_Y; yi++)
		{
			if (MazeData[xi][yi] == ROAD)
			{
				fprintf(fp, "　");
			}
			else
			{
				fprintf(fp, "■");
			}
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
}

//迷路を作成する準備
//アルゴリズムは穴掘り法のため、外側を道、内側を壁とする。
void Maze_Init(int **MazeData, int Size_X, int Size_Y)
{
	int x,y;

	for(x=0; x<Size_X; x++)
	{
		for(y=0; y<Size_Y; y++)
		{
			if(x==0 || y==0 || x==Size_X-1 || y==Size_Y-1)
				SetPoint(MazeData, x,y, ROAD);
			else
				SetPoint(MazeData, x,y, WALL);
		}
	}

	//最初に掘るための元となる道を作る。
	//（Maze_Sub関数にて、道ではなかったら処理を実行しないため。）
	SetPoint(MazeData, 2,2, ROAD);
}

//ここで迷路を作成する。何回も呼び出される。
//アルゴリズムは穴掘り法
void Maze_Sub(int **MazeData, int x, int y)
{
	int x1,y1, px,py, r,c,p;

	//道でなかったら関数を抜ける。
	if(GetPoint(MazeData,x,y)!=ROAD) return;

	//位置を初期化
	x1=x;
	y1=y;

	while(1)
	{
		//初めの向きを決める
		r = rand()%4;

		//壁にぶつかった時の振り向く方向を決める
		if(rand()%2) p = (rand()%100000)%2 ? 1 : -1;
		else         p = (rand()%100000)%2 ? -1 : 1;

		//最大４回向きを変える
		for(c=0; c<4; c++)
		{
			//向きに応じて座標を決める
			switch((r+c*p+3)%4)
			{
				case 0:
					px=0;
					py=-1;
					break;
				case 1:
					px=-1;
					py=0;
					break;
				case 2:
					px=0;
					py=1;
					break;
				case 3:
					px=1;
					py=0;
					break;
			}
			//先が壁だったら掘る
			if( GetPoint(MazeData, x1+px*2, y1+py*2) == WALL )
			{
				//向いている方向に２マス掘る
				SetPoint(MazeData, x1+px, y1+py, ROAD);
				SetPoint(MazeData, x1+px*2, y1+py*2, ROAD);
				x1+=px*2;
				y1+=py*2;
				break;
			}
			//先が全部道だったら関数を抜ける
			else if(c==3) return;
		}
	}
}

//-------------------------------------------------------------------------------------

//迷路を解く関数。
void SolveMaze(int **MazeData,int **SolveData, int Size_X,int Size_Y, int StartX,int StartY, int GoalX,int GoalY)
{
	int x,y, px,py;
	//int i,j;
	int Muki;
	int IkidomariFlag=0;

	//GetMazeArray(SolveData, Size_X,Size_Y);
	//答えのデータを初期化
	for(x=0; x<Size_X; x++)
	{
		for(y=0; y<Size_Y; y++)
		{
			SolveData[x][y]=0;
		}
	}

	//迷路の現在位置を初期化（スタート位置にする。）
	x = StartX;
	y = StartY;
	SolveData[x][y]=1;

	//迷路を解く。
	//ゴールまでの道が繋がっていなかった場合について、現在考慮しておりません。
	while(1)
	{
		//最大４回向きを変える。
		for(Muki=0; Muki<4; Muki++)
		{
			//向きに応じて座標を決める。
			switch(Muki)
			{
				case 0: px=0; py=1; break;
				case 1: px=1; py=0; break;
				case 2: px=0; py=-1; break;
				case 3: px=-1; py=0; break;
			}
			//先が道だったらとりあえず進む
			if (
				x + px >= 0 && x + px <Size_X && 
				y + py >= 0 && y + py <Size_Y && 
				MazeData[x + px][y + py] == ROAD && SolveData[x + px][y + py] == 0)
			{
				SolveData[x+px][y+py]=1;
				x+=px;
				y+=py;
				break;
			}
		}
		//行き止まりだったら
		if(Muki==4)
		{
			//最大４回向きを変える。
			for(Muki=0; Muki<4; Muki++)
			{
				//向きに応じて座標を決める。
				switch(Muki)
				{
					case 0: px=0; py=1; break;
					case 1: px=1; py=0; break;
					case 2: px=0; py=-1; break;
					case 3: px=-1; py=0; break;
				}
				//先が道だったら前通ったところを戻る。
				if(
					x + px >= 0 && x + px <Size_X &&
					y + py >= 0 && y + py <Size_Y &&
					MazeData[x + px][y + py] == ROAD && SolveData[x + px][y + py] == 1)
				{
					SolveData[x][y]=2;
					x+=px;
					y+=py;
					break;
				}
			}
		}

		//ゴールに着いたらループを抜け、関数を抜ける。
		if(x==GoalX && y==GoalY) break;
	}

	return;
}

#endif