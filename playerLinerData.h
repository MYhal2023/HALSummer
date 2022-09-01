//=============================================================================
//
// バトル中の線形補完データ(味方) [playerLinerData.h]
// Author : 
//
//=============================================================================
#pragma once
#include "main.h"
////////////////////////////////////////
////仕様説明////
//////本体データは、現データに”加算”する数値を入れればよい(0.0ｆで何もしないデータを作れる。特にsclは注意)
/////パーツデータも同じように作ってくれてよいが、sclだけは”1.0fがデフォルト”なため注意
/////また、フレーム総数は必ず統一すること。
////本体モーションが120フレームで終わるとするならば、パーツも120フレームで終わらせる必要がある。
////最終テーブルデータのフレームは参照されることがないため、そこを加算せずに総数を合わせる。
////テーブル数を合わせる必要はない。テーブル数に関してはパーツごとに違っても問題ない。
////
//////////////////////////
////好中球攻撃モーションデータ
//////////////////////////
static INTERPOLATION_DATA neutro_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(0.5f, 0.5f, 0.5f), 40 },
	{ XMFLOAT3(-10.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI*0.125f, 0.0f, 0.0f),	XMFLOAT3(0.5f, 0.5f, 0.5f),  5 },
	{ XMFLOAT3(10.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI * 0.125f, 0.0f, 0.0f),	XMFLOAT3(0.5f, 0.5f, 0.5f), 15 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(0.5f, 0.5f, 0.5f),  0 },
};
//////////////////////////
////好中球待機モーションデータ
static INTERPOLATION_DATA neutro_Standby[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(0.5f, 0.5f, 0.5f), 15 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(0.5f, 0.5f, 0.5f), 15 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(0.5f, 0.5f, 0.5f), 15 },
	{ XMFLOAT3(0.0f,-5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(0.5f, 0.5f, 0.5f), 15 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(0.5f, 0.5f, 0.5f),  0 },
};
////////////////////////////////////
//////////////////////////
////マクロファージ攻撃モーションデータ
//////////////////////////
static INTERPOLATION_DATA macro_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 15 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 5 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.125f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  25 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 5 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 10 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 0},

};
static INTERPOLATION_DATA macro_ArmAt[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  20 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  5 },
	{ XMFLOAT3(0.0f, 0.0f,-15.0f), XMFLOAT3(XM_PI * 0.125f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  5 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI * 0.125f * 0.125f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  5 },
	{ XMFLOAT3(0.0f, 0.0f,-15.0f), XMFLOAT3(XM_PI * 0.125f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  5 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  5 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  15 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  0 },
};
static INTERPOLATION_DATA macro_LegAt[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 40 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  5 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	 0 },

};
//////////////////////////
////マクロファージ待機モーションデータ
static INTERPOLATION_DATA macro_Standby[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.125f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 65 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.25f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 10 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.125f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  0 },
};
static INTERPOLATION_DATA macro_ArmSb[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 0.0f, -12.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 0.0f, -12.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 10 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },


};
static INTERPOLATION_DATA macro_LegSb[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },
};
////////////////////////////////////
//////////////////////////
////ヘルパーT攻撃モーションデータ
//////////////////////////
static INTERPOLATION_DATA helperT_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 3.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 30 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 30 },
	{ XMFLOAT3(0.0f, 3.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  0 },
};
static INTERPOLATION_DATA helperT_TopAt[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, -2.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },
};
//////////////////////////
////ヘルパーT待機モーションデータ
static INTERPOLATION_DATA helperT_Standby[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 3.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 30 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 30 },
	{ XMFLOAT3(0.0f, 3.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  0 },
};
static INTERPOLATION_DATA helperT_TopSb[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },
};
////////////////////////////////////
//////////////////////////
////キラーT攻撃モーションデータ
//////////////////////////
static INTERPOLATION_DATA killerT_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.75f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 50 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 10 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.75f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  0 },

};
static INTERPOLATION_DATA killerT_AxeAt[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, XM_PI * 0.5f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  5 },
	{ XMFLOAT3(0.0f, 0.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, XM_PI * 0.25f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  5 },
	{ XMFLOAT3(0.0f, 0.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },

};
//////////////////////////
////キラーT待機モーションデータ
static INTERPOLATION_DATA killerT_Standby[] = {	// pos, rot, scl, frame

	{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 50 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  5 },
	{ XMFLOAT3(0.0f,10.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  5 },
	{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  0 },
};
static INTERPOLATION_DATA killerT_AxeSb[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 10.0f), XMFLOAT3(0.0f, XM_PI * 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 10.0f), XMFLOAT3(0.0f, XM_PI * 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, XM_PI * 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 0.0f, 10.0f), XMFLOAT3(0.0f, XM_PI * 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  0 },

};
////////////////////////////////////
//////////////////////////
////NK細胞攻撃モーションデータ
//////////////////////////
static INTERPOLATION_DATA NK_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * -0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 75 },
	{ XMFLOAT3(10.0f,-5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * -0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 5 },
	{ XMFLOAT3(0.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * -0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  0 },
};
//////////////////////////
////NK細胞待機モーションデータ
static INTERPOLATION_DATA NK_Standby[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -5.0f, 0.0f), XMFLOAT3(0.0f,XM_PI * -0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 600 },
	{ XMFLOAT3(0.0f, -5.0f, 0.0f), XMFLOAT3(0.0f,XM_PI * -2.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 0 },
};

////////////////////////////////////
//////////////////////////
////好塩基球攻撃モーションデータ
//////////////////////////
static INTERPOLATION_DATA Kouen_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * -0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 90 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * -0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),  0 },
};
static INTERPOLATION_DATA Kouen_Arm_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 15.0f), XMFLOAT3(XM_PI * 6.0f, XM_PI * 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 85 },
	{ XMFLOAT3(0.0f, 0.0f, 15.0f), XMFLOAT3(0.0f, XM_PI * 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 0.0f, 15.0f), XMFLOAT3(0.0f, XM_PI * 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  0 },
};
static INTERPOLATION_DATA Kouen_Arm2_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, -15.0f), XMFLOAT3(XM_PI * 6.0f, XM_PI * 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 85 },
	{ XMFLOAT3(0.0f, 0.0f, -15.0f), XMFLOAT3(0.0f, XM_PI * 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 0.0f, -15.0f), XMFLOAT3(0.0f, XM_PI * 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  0 },
};
//////////////////////////
////好塩基球待機モーションデータ
static INTERPOLATION_DATA Kouen_Standby[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f,XM_PI * -0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f,XM_PI * -0.5f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f), 0 },
};
static INTERPOLATION_DATA Kouen_Arm_Standby[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 15.0f), XMFLOAT3(-XM_PI * 2.0f, XM_PI * 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 15.0f), XMFLOAT3(0.0f, XM_PI * 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  0 },
};
static INTERPOLATION_DATA Kouen_Arm2_Standby[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, -15.0f), XMFLOAT3(-XM_PI * 2.0f, XM_PI * 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, -15.0f), XMFLOAT3(0.0f, XM_PI * 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  0 },
};
