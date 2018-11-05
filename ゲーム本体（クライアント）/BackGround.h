/*ìÆÇ≠îwåiâÊëúÇï\é¶Ç∑ÇÈä÷êî*/

#ifndef BACKGROUND_DEFINED
#define BACKGROUND_DEFINED

#include "Defines.h"

void DrawBackGround(int, int, int);

void DrawBackGround(int sx, int sy, int kind = 0)
{
	static int framecnt = 0;
	int nowmode, frame240;
	int xi, yi;

	MEDIA_HANDLER Handle_NowBack[2];

	if (kind == 0)
	{
		Handle_NowBack[0] = Handle_BackGround[0];
		Handle_NowBack[1] = Handle_BackGround[1];
	}
	else if (kind == 1)
	{
		Handle_NowBack[0] = Handle_BackGround_2[0];
		Handle_NowBack[1] = Handle_BackGround_2[1];
	}
	else if (kind == 2)
	{
		Handle_NowBack[0] = Handle_BackGround_3[0];
		Handle_NowBack[1] = Handle_BackGround_3[1];
	}

	for (xi = 0; xi < sx / 40 + 1; xi++)
	{
		for (yi = 0; yi < sy / 40 + 1; yi++)
		{
			frame240 = framecnt % 240;

			if (frame240 < 60)			nowmode = 0;
			else if (frame240 < 120)	nowmode = 1;
			else if (frame240 < 180)	nowmode = 2;
			else						nowmode = 3;

			DrawExtendGraph(
				xi * 40 + 20 - 20,
				yi * 40 + 20 - 20,
				xi * 40 + 20 + 20,
				yi * 40 + 20 + 20,
				Handle_NowBack[1],
				false);

			if ((xi + yi) % 2 == 0)
			{
				if (frame240 < 120)
				{
					if (nowmode == 0)
					{
						DrawExtendGraph(
							xi * 40 + 20 - 20 * (frame240 % 60 / 60.0),
							yi * 40 + 20 - 20 * (frame240 % 60 / 60.0),
							xi * 40 + 20 + 20 * (frame240 % 60 / 60.0),
							yi * 40 + 20 + 20 * (frame240 % 60 / 60.0),
							Handle_NowBack[0],
							false
							);
					}
					else if (nowmode == 1)
					{
						DrawExtendGraph(
							xi * 40 + 20 - 20 * (1.0 - frame240 % 60 / 60.0),
							yi * 40 + 20 - 20 * (1.0 - frame240 % 60 / 60.0),
							xi * 40 + 20 + 20 * (1.0 - frame240 % 60 / 60.0),
							yi * 40 + 20 + 20 * (1.0 - frame240 % 60 / 60.0),
							Handle_NowBack[0],
							false
							);
					}
				}
			}
			else
			{
				if (frame240 >= 120)
				{
					if (nowmode == 2)
					{
						DrawExtendGraph(
							xi * 40 + 20 - 20 * (frame240 % 60 / 60.0),
							yi * 40 + 20 - 20 * (frame240 % 60 / 60.0),
							xi * 40 + 20 + 20 * (frame240 % 60 / 60.0),
							yi * 40 + 20 + 20 * (frame240 % 60 / 60.0),
							Handle_NowBack[0],
							false
							);
					}
					else if (nowmode == 3)
					{
						DrawExtendGraph(
							xi * 40 + 20 - 20 * (1.0 - frame240 % 60 / 60.0),
							yi * 40 + 20 - 20 * (1.0 - frame240 % 60 / 60.0),
							xi * 40 + 20 + 20 * (1.0 - frame240 % 60 / 60.0),
							yi * 40 + 20 + 20 * (1.0 - frame240 % 60 / 60.0),
							Handle_NowBack[0],
							false
							);
					}
				}
			}
		}
	}

	framecnt++;
}

#endif