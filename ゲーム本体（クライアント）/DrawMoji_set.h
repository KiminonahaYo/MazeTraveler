/*ドット文字（3×5）データ集*/

#ifndef DISP_MOJI_SET_DEFINED

#define DISP_MOJI_SET_DEFINED

#define MOJI_XSIZE 3
#define MOJI_YSIZE 5

typedef struct
{
	int value[MOJI_YSIZE][MOJI_XSIZE];
	char moji;
}
MOJIDATA;

MOJIDATA dat_0 =
{
	{
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
	},
	'0'
};

MOJIDATA dat_1 =
{
	{
		{ 0, 1, 0 },
		{ 1, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 1, 1, 1 },
	},
	'1'
};

MOJIDATA dat_2 =
{
	{
		{ 1, 1, 1 },
		{ 0, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 1, 1 },
	},
	'2'
};

MOJIDATA dat_3 =
{
	{
		{ 1, 1, 1 },
		{ 0, 0, 1 },
		{ 1, 1, 1 },
		{ 0, 0, 1 },
		{ 1, 1, 1 },
	},
	'3'
};

MOJIDATA dat_4 =
{
	{
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 0, 0, 1 },
		{ 0, 0, 1 },
	},
	'4'
};

MOJIDATA dat_5 =
{
	{
		{ 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 1, 1 },
		{ 0, 0, 1 },
		{ 1, 1, 1 },
	},
	'5'
};

MOJIDATA dat_6 =
{
	{
		{ 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
	},
	'6'
};

MOJIDATA dat_7 =
{
	{
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 0, 0, 1 },
		{ 0, 0, 1 },
	},
	'7'
};

MOJIDATA dat_8 =
{
	{
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
	},
	'8'
};

MOJIDATA dat_9 =
{
	{
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 0, 0, 1 },
		{ 1, 1, 1 },
	},
	'9'
};

MOJIDATA dat_A =
{
	{
		{ 0, 1, 0 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
	},
	'A'
};

MOJIDATA dat_B =
{
	{
		{ 1, 1, 0 },
		{ 1, 0, 1 },
		{ 1, 1, 0 },
		{ 1, 0, 1 },
		{ 1, 1, 0 },
	},
	'B'
};

MOJIDATA dat_C =
{
	{
		{ 0, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 0, 0 },
		{ 1, 0, 0 },
		{ 0, 1, 1 },
	},
	'C'
};

MOJIDATA dat_D =
{
	{
		{ 1, 1, 0 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 0 },
	},
	'D'
};

MOJIDATA dat_E =
{
	{
		{ 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 1, 1 },
	},
	'E'
};

MOJIDATA dat_F =
{
	{
		{ 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 0, 0 },
	},
	'F'
};

MOJIDATA dat_G =
{
	{
		{ 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
	},
	'G'
};

MOJIDATA dat_H =
{
	{
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
	},
	'H'
};

MOJIDATA dat_I =
{
	{
		{ 1, 1, 1 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 1, 1, 1 },
	},
	'I'
};

MOJIDATA dat_J =
{
	{
		{ 0, 0, 1 },
		{ 0, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 0, 1, 0 },
	},
	'J'
};

MOJIDATA dat_K =
{
	{
		{ 1, 0, 1 },
		{ 1, 1, 0 },
		{ 1, 0, 0 },
		{ 1, 1, 0 },
		{ 1, 0, 1 },
	},
	'K'
};

MOJIDATA dat_L =
{
	{
		{ 1, 0, 0 },
		{ 1, 0, 0 },
		{ 1, 0, 0 },
		{ 1, 0, 0 },
		{ 1, 1, 1 },
	},
	'L'
};

MOJIDATA dat_M =
{
	{
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
	},
	'M'
};

MOJIDATA dat_N =
{
	{
		{ 0, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 0 },
	},
	'N'
};

MOJIDATA dat_O =
{
	{
		{ 0, 1, 0 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 0, 1, 0 },
	},
	'O'
};

MOJIDATA dat_P =
{
	{
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 0, 0 },
	},
	'P'
};

MOJIDATA dat_Q =
{
	{
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 0, 0, 1 },
	},
	'Q'
};

MOJIDATA dat_R =
{
	{
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 1, 0 },
		{ 1, 0, 1 },
	},
	'R'
};

MOJIDATA dat_S =
{
	{
		{ 0, 1, 1 },
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 },
		{ 1, 1, 0 },
	},
	'S'
};

MOJIDATA dat_T =
{
	{
		{ 1, 1, 1 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
	},
	'T'
};

MOJIDATA dat_U =
{
	{
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
	},
	'U'
};

MOJIDATA dat_V =
{
	{
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 0, 1, 0 },
	},
	'V'
};

MOJIDATA dat_W =
{
	{
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
	},
	'W'
};

MOJIDATA dat_X =
{
	{
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 0, 1, 0 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
	},
	'X'
};

MOJIDATA dat_Y =
{
	{
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
	},
	'Y'
};

MOJIDATA dat_Z =
{
	{
		{ 1, 1, 1 },
		{ 0, 0, 1 },
		{ 0, 1, 0 },
		{ 1, 0, 0 },
		{ 1, 1, 1 },
	},
	'Z'
};

MOJIDATA dat_a =
{
	{
		{ 0, 0, 0 },
		{ 0, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
	},
	'a'
};

MOJIDATA dat_b =
{
	{
		{ 1, 0, 0 },
		{ 1, 0, 0 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
	},
	'b'
};
MOJIDATA dat_c =
{
	{
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 1, 1 },
	},
	'c'
};
MOJIDATA dat_d =
{
	{
		{ 0, 0, 1 },
		{ 0, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
	},
	'd'
};
MOJIDATA dat_e =
{
	{
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 1, 1 },
	},
	'e'
};
MOJIDATA dat_f =
{
	{
		{ 0, 1, 1 },
		{ 0, 1, 0 },
		{ 1, 1, 1 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
	},
	'f'
};
MOJIDATA dat_g =
{
	{
		{ 1, 1, 0 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 0, 0, 1 },
		{ 1, 1, 1 },
	},
	'g'
};
MOJIDATA dat_h =
{
	{
		{ 1, 0, 0 },
		{ 1, 0, 0 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
	},
	'h'
};
MOJIDATA dat_i =
{
	{
		{ 0, 1, 0 },
		{ 0, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
	},
	'i'
};
MOJIDATA dat_j =
{
	{
		{ 0, 1, 0 },
		{ 0, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 1, 1, 0 },
	},
	'j'
};
MOJIDATA dat_k =
{
	{
		{ 1, 0, 0 },
		{ 1, 0, 0 },
		{ 1, 0, 1 },
		{ 1, 1, 0 },
		{ 1, 0, 1 },
	},
	'k'
};

MOJIDATA dat_l =
{
	{
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
	},
	'l'
};

MOJIDATA dat_n =
{
	{
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
	},
	'n'
};
MOJIDATA dat_m =
{
	{
		{ 0, 0, 0 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
	},
	'm'
};
MOJIDATA dat_o =
{
	{
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
	},
	'o'
};
MOJIDATA dat_p =
{
	{
		{ 0, 0, 0 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 0 },
	},
	'p'
};
MOJIDATA dat_q =
{
	{
		{ 0, 0, 0 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 0, 0, 1 },
	},
	'q'
};
MOJIDATA dat_r =
{
	{
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 0, 0 },
	},
	'r'
};

MOJIDATA dat_s =
{
	{
		{ 0, 0, 0 },
		{ 1, 1, 1 },
		{ 1, 1, 0 },
		{ 0, 0, 1 },
		{ 1, 1, 1 },
	},
	's'
};


MOJIDATA dat_t =
{
	{
		{ 0, 1, 0 },
		{ 1, 1, 1 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 1 },
	},
	't'
};
MOJIDATA dat_u =
{
	{
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
	},
	'u'
};
MOJIDATA dat_v =
{
	{
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 0, 1, 0 },
	},
	'v'
};

MOJIDATA dat_w =
{
	{
		{ 0, 0, 0 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
	},
	'w'
};

MOJIDATA dat_x =
{
	{
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 1, 0, 1 },
		{ 0, 1, 0 },
		{ 1, 0, 1 },
	},
	'x'
};
MOJIDATA dat_y =
{
	{
		{ 0, 0, 0 },
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 0, 1, 0 },
		{ 1, 0, 0 },
	},
	'y'
};
MOJIDATA dat_z =
{
	{
		{ 0, 0, 0 },
		{ 1, 1, 1 },
		{ 0, 1, 0 },
		{ 1, 0, 0 },
		{ 1, 1, 1 },
	},
	'z'
};


//記号

MOJIDATA dat_colon =
{
	{
		{ 0, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 0 },
	},
	':'
};

MOJIDATA dat_semicolon =
{
	{
		{ 0, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 0 },
		{ 0, 1, 0 },
		{ 1, 0, 0 },
	},
	';'
};

MOJIDATA dat_kakko1 =
{
	{
		{ 0, 0, 1 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 },
	},
	'('
};

MOJIDATA dat_kakko2 =
{
	{
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 1, 0, 0 },
	},
	')'
};

MOJIDATA dat_sqkakko1 =
{
	{
		{ 0, 1, 1 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 1 },
	},
	'['
};

MOJIDATA dat_sqkakko2 =
{
	{
		{ 1, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 1, 1, 0 },
	},
	']'
};

MOJIDATA dat_midkakko1 =
{
	{
		{ 0, 1, 1 },
		{ 0, 1, 0 },
		{ 1, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 1 },
	},
	'{'
};

MOJIDATA dat_midkakko2 =
{
	{
		{ 1, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 1 },
		{ 0, 1, 0 },
		{ 1, 1, 0 },
	},
	'}'
};

MOJIDATA dat_plus =
{
	{
		{ 0, 0, 0 },
		{ 0, 1, 0 },
		{ 1, 1, 1 },
		{ 0, 1, 0 },
		{ 0, 0, 0 },
	},
	'+'
};

MOJIDATA dat_minus =
{
	{
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 1, 1, 1 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
	},
	'-'
};

MOJIDATA dat_asta =
{
	{
		{ 0, 0, 0 },
		{ 1, 0, 1 },
		{ 0, 1, 0 },
		{ 1, 0, 1 },
		{ 0, 0, 0 },
	},
	'*'
};

MOJIDATA dat_srush =
{
	{
		{ 0, 0, 1 },
		{ 0, 0, 1 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 1, 0, 0 },
	},
	'/'
};

MOJIDATA dat_bikkuri =
{
	{
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 0 },
		{ 0, 1, 0 },
	},
	'!'
};

MOJIDATA dat_hatena =
{
	{
		{ 1, 1, 1 },
		{ 0, 0, 1 },
		{ 0, 1, 1 },
		{ 0, 0, 0 },
		{ 0, 1, 0 },
	},
	'?'
};

MOJIDATA dat_shonari =
{
	{
		{ 0, 0, 1 },
		{ 0, 1, 0 },
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 },
	},
	'<'
};

MOJIDATA dat_dainari =
{
	{
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 },
		{ 0, 1, 0 },
		{ 1, 0, 0 },
	},
	'>'
};

MOJIDATA dat_canma =
{
	{
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 1, 0 },
		{ 1, 0, 0 },
	},
	','
};


MOJIDATA dat_ten =
{
	{
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 1, 0 },
	},
	'.'
};

MOJIDATA dat_underbar =
{
	{
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 1, 1, 1 },
	},
	'_'
};

MOJIDATA dat_sharp =
{
	{
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
	},
	'#'
};

MOJIDATA dat_singleq =
{
	{
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
	},
	'\''
};

MOJIDATA dat_doubleq =
{
	{
		{ 1, 0, 1 },
		{ 1, 0, 1 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
	},
	'\"'
};

MOJIDATA dat_equal =
{
	{
		{ 0, 0, 0 },
		{ 1, 1, 1 },
		{ 0, 0, 0 },
		{ 1, 1, 1 },
		{ 0, 0, 0 },
	},
	'='
};

MOJIDATA dat_doller =
{
	{
		{ 0, 1, 1 },
		{ 1, 1, 0 },
		{ 1, 1, 1 },
		{ 0, 1, 1 },
		{ 1, 1, 0 },
	},
	'$'
};

MOJIDATA dat_caret =
{
	{
		{ 0, 1, 0 },
		{ 1, 0, 1 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
	},
	'^'
};

MOJIDATA dat_backsrash =
{
	{
		{ 1, 0, 0 },
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 },
	},
	'\\'
};

MOJIDATA dat_parcent =
{
	{
		{ 1, 0, 0 },
		{ 0, 0, 1 },
		{ 0, 1, 0 },
		{ 1, 0, 0 },
		{ 0, 0, 1 },
	},
	'%'
};

MOJIDATA dat_and =
{
	{
		{ 1, 1, 0 },
		{ 1, 0, 0 },
		{ 1, 1, 0 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
	},
	'&'
};

MOJIDATA dat_or =
{
	{
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
	},
	'|'
};

MOJIDATA dat_childer =
{
	{
		{ 1, 0, 0 },
		{ 1, 1, 1 },
		{ 0, 0, 1 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
	},
	'~'
};

MOJIDATA dat_chon =
{
	{
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
	},
	'`'
};

MOJIDATA dat_atmark = 
{
	{
		{ 1, 1, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 0 },
		{ 1, 1, 1 },
	},
	'@'
};

//文字データ配列の終わりを示すデータ
MOJIDATA dat_endofdata =
{
	{
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
	},
	'\0'
};

MOJIDATA mojidata[] =
{
	//数字
	dat_0,
	dat_1,
	dat_2,
	dat_3,
	dat_4,
	dat_5,
	dat_6,
	dat_7,
	dat_8,
	dat_9,

	//大文字
	dat_A,
	dat_B,
	dat_C,
	dat_D,
	dat_E,
	dat_F,
	dat_G,
	dat_H,
	dat_I,
	dat_J,
	dat_K,
	dat_L,
	dat_M,
	dat_N,
	dat_O,
	dat_P,
	dat_Q,
	dat_R,
	dat_S,
	dat_T,
	dat_U,
	dat_V,
	dat_W,
	dat_X,
	dat_Y,
	dat_Z,

	//小文字
	dat_a,
	dat_b,
	dat_c,
	dat_d,
	dat_e,
	dat_f,
	dat_g,
	dat_h,
	dat_i,
	dat_j,
	dat_k,
	dat_l,
	dat_m,
	dat_n,
	dat_o,
	dat_p,
	dat_q,
	dat_r,
	dat_s,
	dat_t,
	dat_u,
	dat_v,
	dat_w,
	dat_x,
	dat_y,
	dat_z,

	//記号
	dat_asta,
	dat_bikkuri,
	dat_canma,
	dat_colon,
	dat_dainari,
	dat_hatena,
	dat_kakko1,
	dat_kakko2,
	dat_midkakko1,
	dat_midkakko2,
	dat_minus,
	dat_plus,
	dat_semicolon,
	dat_sharp,
	dat_shonari,
	dat_sqkakko1,
	dat_sqkakko2,
	dat_srush,
	dat_ten,
	dat_underbar,
	dat_singleq,
	dat_doubleq,
	dat_equal,
	dat_doller,
	dat_caret,
	dat_backsrash,
	dat_parcent,
	dat_and,
	dat_or,
	dat_childer,
	dat_chon,
	dat_atmark,

	//データの最後
	dat_endofdata
};

#endif