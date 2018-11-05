/*���H�쐬�w�b�_*/

#ifndef MYMAZE_DEFINED

#define MYMAZE_DEFINED

#include "Defines.h"

#define ROAD 0									//���H��
#define WALL 1									//���H��

void SetPoint(int **,int,int,int);				//���H�̔z��̗v�f�̓��e��ύX����
int GetPoint(int **,int,int);					//���H�̔z��̗v�f�̓��e���擾����

void Maze_Init(int **, int,int);				//���H����鏀��������
void Maze_Sub(int **, int,int);					//���H�����B������Ăяo�����B

void GetMazeArray(int ***,int,int);				//���H�p�̔z��m��
void DeleteMazeArray(int **);					//���H�p�̔z��J��

void MakeMaze(int**, unsigned int, int, int);	//���H���쐬����֐����C��
void MazeLog(int**, int, int);					//���H�̃��O���c��

//-------------------------------------------------------------------------------------

//���H�̂��߂̔z��m��
void GetMazeArray(int ***MazeData, int Size_X, int Size_Y)
{
	int i;

	(*MazeData) = (int **)malloc(sizeof(int*)*Size_X);
	for(i=0; i<Size_X; i++) (*MazeData)[i]=(int *)malloc(sizeof(int)*Size_Y);
}

//���H�̔z��J��
void DeleteMazeArray(int **MazeData, int Size_X)
{
	int x;

	for(x=0; x<Size_X; x++)
		free(MazeData[x]);
	free(MazeData);
}

//���H�̔z��̗v�f�̓��e��ύX����
void SetPoint(int **MazeData, int x, int y, int Type)
{
	//AddLogFiles("Pos(%d,%d) = %s\n", x, y, Type == 0 ? "ROAD" : "WALL");
	MazeData[x][y]=Type;
}

//���H�̔z��̗v�f�̓��e���擾����
int GetPoint(int **MazeData, int x, int y)
{
	return MazeData[x][y];
}

//-------------------------------------------------------------------------------------

//���H���쐬����֐����C��
void MakeMaze(int **MazeData, unsigned int seed, int Size_X, int Size_Y)
{
	int i, xi, yi;
	int mazetype;

	//�V�[�h�l���烉���_���֐�������������B
	srand(seed);

	//���H������������B
	Maze_Init(MazeData, Size_X, Size_Y);

	//----------------���H���쐬------------------
	mazetype = RangeRand(0, 9);			//���H�̐������[�g�̌X�������߂�

	//�������[�g���^�񒆂�˂�������X��������B
	//�S�[��
	//����
	//�@����
	//�@�@����
	//�@�@�@����
	//�@�@�@�@����
	//�@�@�@�@�@�����X�^�[�g�@
	if (mazetype >= 0 && mazetype < 6)
	{
		//���H���쐬
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
		//���H������Ă��Ȃ��ꏊ�𖄂߂�
		for (xi = 2; xi <= Size_X - 3; xi += 2)
		{
			for (yi = 2; yi <= Size_Y - 3; yi += 2)
			{
				Maze_Sub(MazeData, xi, yi);
			}
		}
	}
	//�������[�g���ŏ��A�X�^�[�g�ʒu���猩�č��Ɋ��X��������
	//�S�[��
	//��
	//��
	//��
	//��
	//��
	//�������������X�^�[�g
	else if (mazetype >= 6 && mazetype < 8)
	{
		//���H���쐬
		for (xi = 2; xi <= Size_X - 3; xi += 2)
		{
			for (yi = 2; yi <= Size_Y - 3; yi += 2)
			{
				Maze_Sub(MazeData, xi, yi);
			}
		}
		//���H������Ă��Ȃ��ꏊ�𖄂߂�
		for (xi = 2; xi <= Size_X - 3; xi += 2)
		{
			for (yi = 2; yi <= Size_Y - 3; yi += 2)
			{
				Maze_Sub(MazeData, xi, yi);
			}
		}
	}
	//�������[�g���ŏ��A�X�^�[�g�ʒu���猩�Đ��ʂɊ��X��������
	//�S�[����������
	//�@�@�@�@�@�@��
	//�@�@�@�@�@�@��
	//�@�@�@�@�@�@��
	//�@�@�@�@�@�@��
	//�@�@�@�@�@�@��
	//�@�@�@�X�^�[�g
	else
	{
		//���H���쐬
		for (yi = 2; yi <= Size_Y - 3; yi += 2)
		{
			for (xi = 2; xi <= Size_X - 3; xi += 2)
			{
				Maze_Sub(MazeData, xi, yi);
			}
		}
		//���H������Ă��Ȃ��ꏊ�𖄂߂�
		for (yi = 2; yi <= Size_Y - 3; yi += 2)
		{
			for (xi = 2; xi <= Size_X - 3; xi += 2)
			{
				Maze_Sub(MazeData, xi, yi);
			}
		}
	}

	//�X�^�[�g
	SetPoint(MazeData, 1, 2, ROAD);
	SetPoint(MazeData, 0, 2, ROAD);
	SetPoint(MazeData, 0, 1, WALL);
	SetPoint(MazeData, 0, 3, WALL);

	//�S�[��
	SetPoint(MazeData, Size_X - 2, Size_Y - 3, ROAD);
	SetPoint(MazeData, Size_X - 1, Size_Y - 3, ROAD);
	SetPoint(MazeData, Size_X - 1, Size_Y - 2, WALL);
	SetPoint(MazeData, Size_X - 1, Size_Y - 4, WALL);

	//MazeLog(MazeData, Size_X, Size_Y);
}

//-------------------------------------------------------------------------------------
//���O���c���i�f�o�b�O�p�j
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
				fprintf(fp, "�@");
			}
			else
			{
				fprintf(fp, "��");
			}
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
}

//���H���쐬���鏀��
//�A���S���Y���͌��@��@�̂��߁A�O���𓹁A������ǂƂ���B
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

	//�ŏ��Ɍ@�邽�߂̌��ƂȂ铹�����B
	//�iMaze_Sub�֐��ɂāA���ł͂Ȃ������珈�������s���Ȃ����߁B�j
	SetPoint(MazeData, 2,2, ROAD);
}

//�����Ŗ��H���쐬����B������Ăяo�����B
//�A���S���Y���͌��@��@
void Maze_Sub(int **MazeData, int x, int y)
{
	int x1,y1, px,py, r,c,p;

	//���łȂ�������֐��𔲂���B
	if(GetPoint(MazeData,x,y)!=ROAD) return;

	//�ʒu��������
	x1=x;
	y1=y;

	while(1)
	{
		//���߂̌��������߂�
		r = rand()%4;

		//�ǂɂԂ��������̐U��������������߂�
		if(rand()%2) p = (rand()%100000)%2 ? 1 : -1;
		else         p = (rand()%100000)%2 ? -1 : 1;

		//�ő�S�������ς���
		for(c=0; c<4; c++)
		{
			//�����ɉ����č��W�����߂�
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
			//�悪�ǂ�������@��
			if( GetPoint(MazeData, x1+px*2, y1+py*2) == WALL )
			{
				//�����Ă�������ɂQ�}�X�@��
				SetPoint(MazeData, x1+px, y1+py, ROAD);
				SetPoint(MazeData, x1+px*2, y1+py*2, ROAD);
				x1+=px*2;
				y1+=py*2;
				break;
			}
			//�悪�S������������֐��𔲂���
			else if(c==3) return;
		}
	}
}

//-------------------------------------------------------------------------------------

//���H�������֐��B
void SolveMaze(int **MazeData,int **SolveData, int Size_X,int Size_Y, int StartX,int StartY, int GoalX,int GoalY)
{
	int x,y, px,py;
	//int i,j;
	int Muki;
	int IkidomariFlag=0;

	//GetMazeArray(SolveData, Size_X,Size_Y);
	//�����̃f�[�^��������
	for(x=0; x<Size_X; x++)
	{
		for(y=0; y<Size_Y; y++)
		{
			SolveData[x][y]=0;
		}
	}

	//���H�̌��݈ʒu���������i�X�^�[�g�ʒu�ɂ���B�j
	x = StartX;
	y = StartY;
	SolveData[x][y]=1;

	//���H�������B
	//�S�[���܂ł̓����q�����Ă��Ȃ������ꍇ�ɂ��āA���ݍl�����Ă���܂���B
	while(1)
	{
		//�ő�S�������ς���B
		for(Muki=0; Muki<4; Muki++)
		{
			//�����ɉ����č��W�����߂�B
			switch(Muki)
			{
				case 0: px=0; py=1; break;
				case 1: px=1; py=0; break;
				case 2: px=0; py=-1; break;
				case 3: px=-1; py=0; break;
			}
			//�悪����������Ƃ肠�����i��
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
		//�s���~�܂肾������
		if(Muki==4)
		{
			//�ő�S�������ς���B
			for(Muki=0; Muki<4; Muki++)
			{
				//�����ɉ����č��W�����߂�B
				switch(Muki)
				{
					case 0: px=0; py=1; break;
					case 1: px=1; py=0; break;
					case 2: px=0; py=-1; break;
					case 3: px=-1; py=0; break;
				}
				//�悪����������O�ʂ����Ƃ����߂�B
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

		//�S�[���ɒ������烋�[�v�𔲂��A�֐��𔲂���B
		if(x==GoalX && y==GoalY) break;
	}

	return;
}

#endif