//=============================================================================
//
// バトル中の線形補完データ(敵) [enemyLinerData.h]
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
////黄色ブドウ球菌モーションデータ
//////////////////////////
////
// 本体攻撃モーション
////
static INTERPOLATION_DATA grape_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f), 70 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f), 15 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),  0 },
};
// パーツ1の攻撃モーション
static INTERPOLATION_DATA grape_Parts001Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 35 },
	{ XMFLOAT3(13.5f,-3.0f,  5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.25f, 0.25f, 0.25f), 40 },
	{ XMFLOAT3(13.5f,-3.0f,  5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.25f, 0.25f, 0.25f), 5 },
	{ XMFLOAT3(13.5f,-3.0f, 25.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.5f, 1.5f, 1.5f), 5 },
	{ XMFLOAT3(13.5f,-3.0f,  5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 0.0f,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),  0 },
};
//パーツ2
static INTERPOLATION_DATA grape_Parts002Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 35 },
	{ XMFLOAT3(3.5f,-5.0f,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.25f, 0.25f, 0.25f), 40 },
	{ XMFLOAT3(3.5f,-5.0f,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.25f, 0.25f, 0.25f), 5 },
	{ XMFLOAT3(3.5f,-5.0f, 20.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.5f, 1.5f, 1.5f), 5 },
	{ XMFLOAT3(3.5f,-5.0f,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 0.0f,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),  0 },

};
//パーツ3
static INTERPOLATION_DATA grape_Parts003Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 35 },
	{ XMFLOAT3(-3.5f,-5.0f,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.25f, 0.25f, 0.25f), 40 },
	{ XMFLOAT3(-3.5f,-5.0f,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.25f, 0.25f, 0.25f), 5 },
	{ XMFLOAT3(-3.5f,-5.0f, 20.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.5f, 1.5f, 1.5f), 5 },
	{ XMFLOAT3(-3.5f,-5.0f,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 5 },
	{ XMFLOAT3(0.0f, 0.0f,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),  0 },
};
////
// 本体移動時モーション
////
static INTERPOLATION_DATA grape_Move[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f,  0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f), 30 },
	{ XMFLOAT3(0.0f, 20.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f), 30 },
	{ XMFLOAT3(0.0f,  0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),  0 },
};
// パーツ1
static INTERPOLATION_DATA grape_Parts001Move[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),			XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -2.0f*XM_PI, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  0 },

};
// パーツ2
static INTERPOLATION_DATA grape_Parts002Move[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f,   0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),			XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(0.0f, -20.0f, 0.0f), XMFLOAT3(0.0f, XM_PI, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(0.0f,   0.0f, 0.0f), XMFLOAT3(0.0f, 2.0f*XM_PI, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  0 },

};
// パーツ3
static INTERPOLATION_DATA grape_Parts003Move[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f,   0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),			XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(0.0f, -20.0f, 0.0f), XMFLOAT3(0.0f, XM_PI, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(0.0f,   0.0f, 0.0f), XMFLOAT3(0.0f, 2.0f*XM_PI, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  0 },

};
////////////////////////////////////

//////////////////////////
////A群β溶連菌モーションデータ
//////////////////////////
////
// 本体攻撃モーション
////
static INTERPOLATION_DATA strept_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f, 0.0f, 0.0f), 10, },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI*0.25f, 0.0f, 0.0f),	   XMFLOAT3(0.0f, 0.0f, 0.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f, 0.0f, 0.0f), 60 },

};
// パーツ1の攻撃モーション
static INTERPOLATION_DATA strept_headAt[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10, },
	{ XMFLOAT3(0.0f, -30.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	   XMFLOAT3(1.375f, 1.375f, 1.375f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};
//パーツ2
static INTERPOLATION_DATA strept_neckAt[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10, },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	   XMFLOAT3(1.0f, 1.0f, 1.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};
//パーツ3
static INTERPOLATION_DATA strept_tailAt[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10, },
	{ XMFLOAT3(0.0f, 27.0f, -19.0f), XMFLOAT3(-XM_PI * 0.375f, 0.0f, 0.0f),	   XMFLOAT3(1.0f, 1.0f, 1.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};
////
// 本体移動時モーション
////
static INTERPOLATION_DATA strept_Move[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f, 0.0f, 0.0f), 30, },
	{ XMFLOAT3(0.0f, 0.0f, 5.0f), XMFLOAT3(XM_PI*0.125f, 0.0f, 0.0f),	   XMFLOAT3(0.0f, 0.0f, 0.0f), 30 },
	{ XMFLOAT3(0.0f, 0.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f, 0.0f, 0.0f), 0 },

};
// パーツ1
static INTERPOLATION_DATA strept_headMv[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 30, },
	{ XMFLOAT3(0.0f, 30.0f, -40.0f), XMFLOAT3(XM_PI*0.125f, 0.0f, 0.0f),	   XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },

};
// パーツ2
static INTERPOLATION_DATA strept_neckMv[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 30, },
	{ XMFLOAT3(0.0f, 40.0f,40.0f), XMFLOAT3(-XM_PI * 0.25f,0.0f ,0.0f),	   XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },

};
// パーツ3
static INTERPOLATION_DATA strept_tailMv[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 30, },
	{ XMFLOAT3(0.0f, 25.0f, -15.0f), XMFLOAT3(-XM_PI * 0.25f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },

};

//////////////////////////
////緑膿菌モーションデータ
//////////////////////////
////
// 本体攻撃モーション
////
static INTERPOLATION_DATA ryoku_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f, 0.0f, 0.0f), 5, },
	{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT3(-XM_PI * 0.25f, 0.0f, 0.0f),	   XMFLOAT3(0.0f, 0.0f, 0.0f), 5 },
	{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT3(-XM_PI * 0.25f, 0.0f, -XM_PI * 0.25f),	   XMFLOAT3(0.0f, 0.0f, 0.0f), 5 },
	{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT3(-XM_PI * 0.25f, 0.0f,  XM_PI * 0.25f),	   XMFLOAT3(0.0f, 0.0f, 0.0f), 5 },
	{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT3(-XM_PI * 0.25f, 0.0f, -XM_PI * 0.25f),	   XMFLOAT3(0.0f, 0.0f, 0.0f), 5 },
	{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT3(-XM_PI * 0.25f, 0.0f,  XM_PI * 0.25f),	   XMFLOAT3(0.0f, 0.0f, 0.0f), 5 },
		{ XMFLOAT3(0.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f, 0.0f, 0.0f), 5, },
	{ XMFLOAT3(0.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f, 0.0f, 0.0f), 25, },
	{ XMFLOAT3(0.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f, 0.0f, 0.0f), 0 },

};
// パーツ1の攻撃モーション
static INTERPOLATION_DATA ryoku_leftAt[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10, },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	   XMFLOAT3(1.0f, 1.0f, 1.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },

};
//パーツ2
static INTERPOLATION_DATA ryoku_rightAt[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10, },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),	   XMFLOAT3(1.0f, 1.0f, 1.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },

};
////
// 移動モーション
////
static INTERPOLATION_DATA ryoku_Move[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f, 0.0f, 0.0f), 10, },
	{ XMFLOAT3(0.0f, -5.0f, 0.0f), XMFLOAT3(XM_PI*0.0f, 0.0f, 0.0f),	   XMFLOAT3(0.0f, 0.0f, 0.0f), 50 },
	{ XMFLOAT3(0.0f, -5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(0.0f, 0.0f, 0.0f), 60 },

};
// パーツ1の攻撃モーション
static INTERPOLATION_DATA ryoku_leftMv[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI * 0.25f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10, },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI * 0.5f, 0.0f, 0.0f),	   XMFLOAT3(1.0f, 1.0f, 1.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI * 0.25f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },

};
//パーツ2
static INTERPOLATION_DATA ryoku_rightMv[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI * 0.25f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10, },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI * 0.5f, 0.0f, 0.0f),	   XMFLOAT3(1.0f, 1.0f, 1.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI * 0.25f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },

};

//////////////////////////
////インフルエンザモーションデータ
//////////////////////////
////
// 本体攻撃モーション
////
static INTERPOLATION_DATA infule_Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(7.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(-0.4f, -0.5f, -0.5f), 45 },
	{ XMFLOAT3(7.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(-0.25f, -0.375f, -0.375f),  10 },
	{ XMFLOAT3(7.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(-0.4f, -0.5f, -0.5f), 5 },
	{ XMFLOAT3(7.0f, -4.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(-0.4f, -0.5f, -0.5f), 0 },
};
// パーツ1の攻撃モーション
static INTERPOLATION_DATA infule_Parts001Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(43.0f,-25.0f, -25.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(43.0f,-25.0f, -25.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 2 },
	{ XMFLOAT3(43.0f, 25.0f,  25.0f),  XMFLOAT3(0.0f, 2.0f*XM_PI, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(43.0f, 25.0f,  25.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 2 },
	{ XMFLOAT3(43.0f, 15.0f, -15.0f),  XMFLOAT3(0.0f, 2.0f*XM_PI, 0.0f),		XMFLOAT3(1.5f, 1.5f, 1.5f), 8 },
	{ XMFLOAT3(43.0f, 15.0f, -15.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.5f, 1.5f, 1.5f), 10 },
	{ XMFLOAT3(43.0f, 15.0f, -25.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.5f, 1.5f, 1.5f), 2 },
	{ XMFLOAT3(-7.0f, 15.0f,  15.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.5f, 1.5f, 1.5f), 5 },
	{ XMFLOAT3(43.0f,-15.0f, -25.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(43.0f,-15.0f, -25.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },
};
//パーツ2
static INTERPOLATION_DATA infule_Parts002Attack[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-57.0f, 25.0f,  25.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-57.0f, 25.0f,  25.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 2 },
	{ XMFLOAT3(-57.0f,-25.0f, -25.0f),  XMFLOAT3(0.0f, 2.0f*XM_PI, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 8 },
	{ XMFLOAT3(-57.0f,-25.0f, -25.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 2 },
	{ XMFLOAT3(-57.0f, 15.0f, -15.0f),  XMFLOAT3(0.0f, 2.0f*XM_PI, 0.0f),		XMFLOAT3(1.5f, 1.5f, 1.5f), 8 },
	{ XMFLOAT3(-57.0f, 15.0f, -15.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.5f, 1.5f, 1.5f), 10 },
	{ XMFLOAT3(-57.0f, 15.0f, -25.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.5f, 1.5f, 1.5f), 2 },
	{ XMFLOAT3(-7.0f, 15.0f,  15.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.5f, 1.5f, 1.5f), 5 },
	{ XMFLOAT3(-57.0f, 15.0f, -25.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-57.0f, 15.0f, -25.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },
};
////
// 本体移動時モーション
////
static INTERPOLATION_DATA infule_Move[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-7.0f,   0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(-0.4f, -0.5f, -0.5f), 30 },
	{ XMFLOAT3(-7.0f,   0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(-0.25f, -0.375f, -0.375f), 30 },
	{ XMFLOAT3(-7.0f,   0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(-0.4f, -0.5f, -0.5f), 0 },
};
// パーツ1
static INTERPOLATION_DATA infule_Parts001Move[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(43.0f, -0.0f, -25.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(43.0f,  25.0f, -25.0f),  XMFLOAT3(0.0f, 0.5f*XM_PI, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  15 },
	{ XMFLOAT3(43.0f,  0.0f, -25.0f),  XMFLOAT3(0.0f, 1.0f*XM_PI, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(43.0f, -25.0f, -25.0f),  XMFLOAT3(0.0f, 1.5f*XM_PI, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  15 },
	{ XMFLOAT3(43.0f, -0.0f, -25.0f),  XMFLOAT3(0.0f, 2.0f*XM_PI, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 0 },

};
// パーツ2
static INTERPOLATION_DATA infule_Parts002Move[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-57.0f, -0.0f,-30.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-57.0f, -25.0f,-30.0f), XMFLOAT3(0.0f, 0.5f*XM_PI, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-57.0f,  0.0f,-30.0f), XMFLOAT3(0.0f, 1.0f*XM_PI, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-57.0f,  25.0f,-30.0f), XMFLOAT3(0.0f, 1.5f*XM_PI, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 15 },
	{ XMFLOAT3(-57.0f, -0.0f,-30.0f), XMFLOAT3(0.0f, 2.0f*XM_PI, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f),  0 },

};

////////////////////////////////////

static MOVERINE		moveTbl[] = { XMFLOAT3(300.0f, 0.0f, 100.0f), XMFLOAT3(0.0f, 0.0f, 100.0f) };
static MOVERINE		FirstmoveTbl[] = {
	XMFLOAT3(350.0f, 0.0f, 100.0f), XMFLOAT3(300.0f, 0.0f, 100.0f), XMFLOAT3(300.0f, 0.0f, 150.0f),
	XMFLOAT3(200.0f, 0.0f, 150.0f), XMFLOAT3(200.0f, 0.0f, 100.0f), XMFLOAT3(150.0f, 0.0f, 100.0f),
	XMFLOAT3(150.0f, 0.0f, 50.0f),XMFLOAT3(50.0f, 0.0f, 50.0f), XMFLOAT3(50.0f, 0.0f, 100.0f),
	XMFLOAT3(0.0f, 0.0f, 100.0f)
};
static MOVERINE		FirstmoveTbl2[] = {
	XMFLOAT3(350.0f, 0.0f, 100.0f), XMFLOAT3(0.0f, 0.0f, 100.0f),
};

static MOVERINE		SecondmoveTbl[] = {
	XMFLOAT3(350.0f, 0.0f, 100.0f), XMFLOAT3(300.0f, 0.0f, 100.0f), XMFLOAT3(300.0f, 0.0f, 50.0f),
	XMFLOAT3(200.0f, 0.0f, 50.0f), XMFLOAT3(200.0f, 0.0f, 100.0f), XMFLOAT3(150.0f, 0.0f, 100.0f),
	XMFLOAT3(150.0f, 0.0f, 150.0f),XMFLOAT3(50.0f, 0.0f, 150.0f), XMFLOAT3(50.0f, 0.0f, 100.0f),
	XMFLOAT3(0.0f, 0.0f, 100.0f)
};
static MOVERINE		ThirdmoveTbl[] = {
	XMFLOAT3(350.0f, 0.0f, 100.0f), XMFLOAT3(200.0f, 0.0f, 100.0f), XMFLOAT3(200.0f, 0.0f, 50.0f),
	XMFLOAT3(150.0f, 0.0f, 50.0f), XMFLOAT3(150.0f, 0.0f, 150.0f), XMFLOAT3(100.0f, 0.0f, 150.0f),
	XMFLOAT3(100.0f, 0.0f, 50.0f), XMFLOAT3(50.0f, 0.0f, 50.0f), XMFLOAT3(50.0f, 0.0f, 100.0f),
	XMFLOAT3(0.0f, 0.0f, 100.0f),
};
static MOVERINE		ForthmoveTbl[] = {
	XMFLOAT3(350.0f, 0.0f, 100.0f), XMFLOAT3(300.0f, 0.0f, 100.0f), XMFLOAT3(300.0f, 0.0f, 250.0f),
	XMFLOAT3(150.0f, 0.0f, 250.0f), XMFLOAT3(150.0f, 0.0f, 200.0f), XMFLOAT3(100.0f, 0.0f, 200.0f),
	XMFLOAT3(100.0f, 0.0f, 50.0f), XMFLOAT3(50.0f, 0.0f, 50.0f), XMFLOAT3(50.0f, 0.0f, 100.0f),
	XMFLOAT3(0.0f, 0.0f, 100.0f),
};

#pragma once
