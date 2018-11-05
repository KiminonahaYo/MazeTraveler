/*2次元マップ描画関係　クラス化済*/

#ifndef MAP2DIM_DEFINED
#define MAP2DIM_DEFINED

#include "Defines.h"

#define MAP2DIM_SIZEXandY 10

class MapOf2nd
{
private:
	typedef struct
	{
		double rot;
		double leng;
	}MAPMOVE_INFO;

	int framecnt = 0;

	MAPMOVE_INFO **minfo;
	MAPMOVE_INFO Jikiinfo;
	MAP map;
	MAPITEM *Item;

	int sx, sy;
	double bai;

	bool iskeyflag_ = false;
	bool isansflag_ = false;
	bool isdispitem_ = false;

public:
	MapOf2nd(void);
	void Init(MAP, double, MAPITEM*);
	void SetBai(double);
	void SetScreenSize(int, int);
	void Dest(void);
	void disp(VECTOR, double, bool, bool, bool, bool, bool);

private:
	void disp_Cell(int, int, int, int, double, int);
	int EncodeKind(int, int);
}
Map2Dim;

MapOf2nd::MapOf2nd(void)
{
	map.data = NULL;
	map.ans = NULL;
	map.visited = NULL;
	Jikiinfo.leng = 0.0;
	Jikiinfo.rot = 0.0;
	map.x = 0;
	map.y = 0;
	map.z = 0;
	sx = 0;
	sy = 0;
	bai = 1.0;
}

void MapOf2nd::SetScreenSize(int scr_x, int scr_y)
{
	sx = scr_x;
	sy = scr_y;
}

void MapOf2nd::SetBai(double bairitu)
{
	int xi, zi;
	int tx, tz;

	if (map.data == NULL)
		return;

	bai = bairitu;

	for (xi = 0; xi < map.x; xi++)
	{
		for (zi = 0; zi < map.z; zi++)
		{
			tx = xi*MAP2DIM_SIZEXandY*bai;
			tz = zi*MAP2DIM_SIZEXandY*bai;
			minfo[xi][zi].rot = RadtoDeg(atan2((double)tx, (double)tz));
			minfo[xi][zi].leng = sqrt(double(tx*tx + tz*tz));
		}
	}
}

void MapOf2nd::disp(VECTOR Center, double rot, bool CenterDispFlag, bool isDispSolve, bool isdispitem = false, bool iskeyflag = false, bool isansflag = false)
{
	int xi, yi;
	int xist, yist, xied, yied;
	int jikicx, jikicy;
	int scx, scy;
	int lx, ly;
	int tx, ty;
	int jikix, jikiy;
	int kind;

	if (map.data == NULL)
		return;

	iskeyflag_  = iskeyflag;
	isansflag_  = isansflag;
	isdispitem_ = isdispitem;

	scx = sx / 2;
	scy = sy / 2;

	jikicx = (Center.x - Masu_1 / 2) / Masu_1;
	jikicy = (Center.z - Masu_1 / 2) / Masu_1;

	jikix = int((Center.z - Masu_1 / 2) * (double(MAP2DIM_SIZEXandY*bai) / Masu_1));
	jikiy = int((Center.x - Masu_1 / 2) * (double(MAP2DIM_SIZEXandY*bai) / Masu_1));

	tx = jikix;
	ty = jikiy;

	Jikiinfo.rot = RadtoDeg(atan2((double)ty, (double)tx));
	Jikiinfo.leng = sqrt(double(tx*tx + ty*ty));

	lx = Degcos(Jikiinfo.rot + 180.0 - rot - 90.0) * Jikiinfo.leng + scx;
	ly = Degsin(Jikiinfo.rot + 180.0 - rot - 90.0) * Jikiinfo.leng + scy;

	xist = jikicx - int(scx / (MAP2DIM_SIZEXandY*bai) * 1.8);
	yist = jikicy - int(scy / (MAP2DIM_SIZEXandY*bai) * 1.8);
	xied = jikicx + int(scx / (MAP2DIM_SIZEXandY*bai) * 1.8);
	yied = jikicy + int(scy / (MAP2DIM_SIZEXandY*bai) * 1.8);

	xist = xist >= 0 ? xist : 0;
	yist = yist >= 0 ? yist : 0;
	xied = xied <= map.x ? xied : map.x;
	yied = yied <= map.z ? yied : map.z;

	for (xi = xist; xi < xied; xi++)
	{
		for (yi = yist; yi < yied; yi++)
		{
			tx = Degcos(minfo[xi][yi].rot - rot - 90.0) * minfo[xi][yi].leng + lx;
			ty = Degsin(minfo[xi][yi].rot - rot - 90.0) * minfo[xi][yi].leng + ly;
			kind = EncodeKind(xi, yi);
			if (kind < 5)
			{
				if (
					tx >= -(MAP2DIM_SIZEXandY*bai) * 1.5 &&
					ty >= -(MAP2DIM_SIZEXandY*bai) * 1.5 &&
					tx < sx + (MAP2DIM_SIZEXandY*bai) * 1.5 &&
					ty < sy + (MAP2DIM_SIZEXandY*bai) * 1.5)
					disp_Cell(tx, ty, xi, yi, DegtoRad(-rot + 90.0), kind);
			}
			else
			{
				if (
					tx >= -(MAP2DIM_SIZEXandY*bai) * 1.5 &&
					ty >= -(MAP2DIM_SIZEXandY*bai) * 1.5 &&
					tx < sx + (MAP2DIM_SIZEXandY*bai) * 1.5 &&
					ty < sy + (MAP2DIM_SIZEXandY*bai) * 1.5)
					disp_Cell(tx, ty, xi, yi, DegtoRad(-rot + 90.0), 2);
			}
		}
	}

	if (isDispSolve)
	{
		for (xi = xist; xi < xied; xi++)
		{
			for (yi = yist; yi < yied; yi++)
			{
				if (map.ans[xi][yi] == 1)
				{
					tx = Degcos(minfo[xi][yi].rot - rot - 90.0) * minfo[xi][yi].leng + lx;
					ty = Degsin(minfo[xi][yi].rot - rot - 90.0) * minfo[xi][yi].leng + ly;
					if (
						tx >= -(MAP2DIM_SIZEXandY*bai) * 1.5 &&
						ty >= -(MAP2DIM_SIZEXandY*bai) * 1.5 &&
						tx < sx + (MAP2DIM_SIZEXandY*bai) * 1.5 &&
						ty < sy + (MAP2DIM_SIZEXandY*bai) * 1.5)
						disp_Cell(tx, ty, xi, yi, DegtoRad(-rot + 90.0), 8);
				}
			}
		}
	}

	for (xi = xist; xi < xied; xi++)
	{
		for (yi = yist; yi < yied; yi++)
		{
			tx = Degcos(minfo[xi][yi].rot - rot - 90.0) * minfo[xi][yi].leng + lx;
			ty = Degsin(minfo[xi][yi].rot - rot - 90.0) * minfo[xi][yi].leng + ly;
			kind = EncodeKind(xi, yi);
			if (kind >= 5)
			{
				if (
					tx >= -(MAP2DIM_SIZEXandY*bai) * 1.5 &&
					ty >= -(MAP2DIM_SIZEXandY*bai) * 1.5 &&
					tx < sx + (MAP2DIM_SIZEXandY*bai) * 1.5 &&
					ty < sy + (MAP2DIM_SIZEXandY*bai) * 1.5)
					disp_Cell(tx, ty, xi, yi, DegtoRad(-rot + 90.0), kind);
			}
		}
	}

	if (CenterDispFlag)
	{
		DrawCircle(scx, scy, (MAP2DIM_SIZEXandY*bai) / 2, GetColor(100, 255, 100), true);
		DrawCircle(scx, scy, (MAP2DIM_SIZEXandY*bai) / 2, GetColor(0, 0, 0), false);
		DrawTriangle(
			Degcos(0 - 90) * (MAP2DIM_SIZEXandY*bai) / 2 + scx,
			Degsin(0 - 90) * (MAP2DIM_SIZEXandY*bai) / 2 + (scy - (MAP2DIM_SIZEXandY*bai) / 2 * 3),
			Degcos(120 - 90) * (MAP2DIM_SIZEXandY*bai) / 2 + scx,
			Degsin(120 - 90) * (MAP2DIM_SIZEXandY*bai) / 2 + (scy - (MAP2DIM_SIZEXandY*bai) / 2 * 3),
			Degcos(240 - 90) * (MAP2DIM_SIZEXandY*bai) / 2 + scx,
			Degsin(240 - 90) * (MAP2DIM_SIZEXandY*bai) / 2 + (scy - (MAP2DIM_SIZEXandY*bai) / 2 * 3),
			GetColor(100, 255, 100),
			true
			);
		DrawTriangle(
			Degcos(0 - 90) * (MAP2DIM_SIZEXandY*bai) / 2 + scx,
			Degsin(0 - 90) * (MAP2DIM_SIZEXandY*bai) / 2 + (scy - (MAP2DIM_SIZEXandY*bai) / 2 * 3),
			Degcos(120 - 90) * (MAP2DIM_SIZEXandY*bai) / 2 + scx,
			Degsin(120 - 90) * (MAP2DIM_SIZEXandY*bai) / 2 + (scy - (MAP2DIM_SIZEXandY*bai) / 2 * 3),
			Degcos(240 - 90) * (MAP2DIM_SIZEXandY*bai) / 2 + scx,
			Degsin(240 - 90) * (MAP2DIM_SIZEXandY*bai) / 2 + (scy - (MAP2DIM_SIZEXandY*bai) / 2 * 3),
			GetColor(0, 0, 0),
			false
			);
	}

	framecnt++;
}

void MapOf2nd::disp_Cell(int px, int pz, int mx, int mz, double rot, int kind)
{
	int picno;

	//答えを表示（kindが8の時）
	if (kind == 8)
	{
		DrawRotaGraph2(px, pz, MAP2DIM_SIZEXandY*bai / 2, MAP2DIM_SIZEXandY*bai / 2, bai, rot, Handle_SolveRoute, true);
	}
	else if (kind >= 0 && kind<8)
	{
		if (framecnt % 180 >= 90 && framecnt % 180 < 90 + 60)
		{
			picno = (framecnt % 180 - 90) / 6;
			if (mx < map.x - 1 && mz < map.z - 1 && kind < 5)
				DrawRotaGraph2(px, pz, MAP2DIM_SIZEXandY*bai / 2, MAP2DIM_SIZEXandY*bai / 2, bai*1.2, rot, Handle_2dimmap[kind + 8 * picno], kind >= 5 ? true : false);
			DrawRotaGraph2(px, pz, MAP2DIM_SIZEXandY*bai / 2, MAP2DIM_SIZEXandY*bai / 2, bai, rot, Handle_2dimmap[kind + 8 * picno], kind >= 5 ? true : false);
		}
		else
		{
			if (mx < map.x - 1 && mz < map.z - 1 && kind < 5)
				DrawRotaGraph2(px, pz, MAP2DIM_SIZEXandY*bai / 2, MAP2DIM_SIZEXandY*bai / 2, bai*1.2, rot, Handle_2dimmap[kind], kind >= 5 ? true : false);
			DrawRotaGraph2(px, pz, MAP2DIM_SIZEXandY*bai / 2, MAP2DIM_SIZEXandY*bai / 2, bai, rot, Handle_2dimmap[kind], kind >= 5 ? true : false);
		}
	}
	else;

}

void MapOf2nd::Init(MAP mSource, double bairitu, MAPITEM* iSource = NULL)
{
	int xi, zi;
	int tx, tz;

	if (map.data != NULL)
		return;

	map  = mSource;
	Item = iSource;
	bai  = bairitu;
	framecnt = 0;

	GetArray2Dim(MAPMOVE_INFO, minfo, map.x, map.z);

	for (xi = 0; xi < map.x; xi++)
	{
		for (zi = 0; zi < map.z; zi++)
		{
			tx = xi*MAP2DIM_SIZEXandY*bai;
			tz = zi*MAP2DIM_SIZEXandY*bai;
			minfo[xi][zi].rot = RadtoDeg(atan2((double)tx, (double)tz));
			minfo[xi][zi].leng = sqrt(double(tx*tx + tz*tz));
		}
	}
}

void MapOf2nd::Dest(void)
{
	if (map.data == NULL)
		return;

	DelArray2Dim(minfo, map.x);
	map.data = NULL;
	map.ans = NULL;
	map.visited = NULL;
	Jikiinfo.leng = 0.0;
	Jikiinfo.rot = 0.0;
	map.x = 0;
	map.y = 0;
	map.z = 0;
	sx = 0;
	sy = 0;
	bai = 1.0;
}

int MapOf2nd::EncodeKind(int mapx, int mapz)
{
	if (map.data == NULL)
		return -1;
	if (mapx < 0 || mapz < 0 || mapx >= map.x || mapz >= map.z)
		return -1;

	if (mapx == 0 && mapz == 2)
		return 0;
	else if (mapx == map.x - 1 && mapz == map.z - 3)
		return 1;
	else if (Item != NULL && isdispitem_ == true && mapx == Item->map.x && mapz == Item->map.z && Item->map.isgotten == false)
		return 5;
	else if (Item != NULL && isdispitem_ == true && isansflag_ == true && mapx == Item->ans.x && mapz == Item->ans.z && Item->ans.isgotten == false)
		return 6;
	else if (Item != NULL && isdispitem_ == true && iskeyflag_ == true && mapx == Item->key.x && mapz == Item->key.z && Item->key.isgotten == false)
		return 7;
	else if (map.visited[mapx][mapz] == 1)
		return 4;
	else if (map.data[mapx][mapz] == ROAD)
		return 2;
	else if (map.data[mapx][mapz] == WALL)
		return 3;
	else
		return -1;
}

#endif