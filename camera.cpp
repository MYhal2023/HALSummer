//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "game.h"
//#include "player.h"
#include "time.h"
#include "debugproc.h"
#include "fieldchip.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	POS_X_CAM		(0.0f)			// カメラの初期位置(X座標)
//#define	POS_Y_CAM		(200.0f)		// カメラの初期位置(Y座標)
//#define	POS_Z_CAM		(-400.0f)		// カメラの初期位置(Z座標)


#define	VIEW_ANGLE		(XMConvertToRadians(45.0f))						// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(10000.0f)										// ビュー平面のFarZ値

#define	VALUE_MOVE_CAMERA	(2.0f)										// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(XM_PI * 0.015f)								// カメラの回転量

typedef enum
{
	RUN,
	BETWEEN,
	WALK
}CAMERA_PHASE;
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAMERA			g_Cam;		// カメラデータ

static int				g_ViewPortType = TYPE_FULL_SCREEN;
//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	g_Cam.pos = { 0.0f, 300.0f, 350.0f };
	g_Cam.at = { 0.0f, 0.0f, 0.0f };
	g_Cam.atPos = { 0.0f, 0.0f, 0.0f };
	g_Cam.up  = { 0.0f, 1.0f, 0.0f };
	g_Cam.rot = { 0.0f, 0.0f, 0.0f };

	// 視点と注視点の距離を計算
	float vx, vz;
	vx = g_Cam.pos.x - g_Cam.at.x;
	vz = g_Cam.pos.z - g_Cam.at.z;
	g_Cam.len = sqrtf(vx * vx + vz * vz);
	
	g_Cam.tbl_adr = NULL;		// 再生するアニメデータの先頭アドレスをセット
	g_Cam.move_time = 0.0f;	// 線形補間用のタイマーをクリア
	g_Cam.tbl_size = NULL;		// 再生するアニメデータのレコード数をセット

	// ビューポートタイプの初期化
	g_ViewPortType = TYPE_FULL_SCREEN;
}


//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	if (GetKeyboardPress(DIK_Q) || IsButtonPressed(0, BUTTON_R_LEFT))
	{// 注視点旋回「左」
		g_Cam.rot.y -= VALUE_ROTATE_CAMERA;
		if (g_Cam.rot.y < -XM_PI)
		{
			g_Cam.rot.y += XM_PI * 2.0f;
		}

		g_Cam.at.x = g_Cam.pos.x + sinf(g_Cam.rot.y) * g_Cam.len;
		g_Cam.at.z = g_Cam.pos.z + cosf(g_Cam.rot.y) * g_Cam.len;
	}

	if (GetKeyboardPress(DIK_E) || IsButtonPressed(0, BUTTON_R_RIGHT))
	{// 注視点旋回「右」
		g_Cam.rot.y += VALUE_ROTATE_CAMERA;
		if (g_Cam.rot.y > XM_PI)
		{
			g_Cam.rot.y -= XM_PI * 2.0f;
		}

		g_Cam.at.x = g_Cam.pos.x + sinf(g_Cam.rot.y) * g_Cam.len;
		g_Cam.at.z = g_Cam.pos.z + cosf(g_Cam.rot.y) * g_Cam.len;
	}

	if (GetKeyboardPress(DIK_R))
	{
		g_Cam.rot.x += 0.1f;
	}
	else if (GetKeyboardPress(DIK_F))
	{
		g_Cam.rot.x -= 0.1f;

	}

	if (g_Cam.tbl_adr != NULL)
	{
		// 移動処理
		int		index = (int)g_Cam.move_time;
		float	time = g_Cam.move_time - index;
		int		size = g_Cam.tbl_size;

		float dt = 1.0f / g_Cam.tbl_adr[index].frame;	// 1フレームで進める時間
		g_Cam.move_time += dt;							// アニメーションの合計時間に足す

		if (index > (size - 2))	// ゴールをオーバーしていたら、最初へ戻す。線形補間データを消去
		{
			g_Cam.move_time = 0.0f;
			index = 0;
			g_Cam.tbl_adr = NULL;
			return;
		}

		// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
		XMVECTOR p1 = XMLoadFloat3(&g_Cam.tbl_adr[index + 1].pos);	// 次の場所
		XMVECTOR p0 = XMLoadFloat3(&g_Cam.tbl_adr[index + 0].pos);	// 現在の場所
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&g_Cam.pos, p0 + vec * time);

		// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
		XMVECTOR r1 = XMLoadFloat3(&g_Cam.tbl_adr[index + 1].rot);	// 次の角度
		XMVECTOR r0 = XMLoadFloat3(&g_Cam.tbl_adr[index + 0].rot);	// 現在の角度
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&g_Cam.rot, r0 + rot * time);

		// 注視点の変更量を求める S = StartX + (EndX - StartX) * 今の時間
		XMVECTOR s1 = XMLoadFloat3(&g_Cam.tbl_adr[index + 1].scl);	// 次の注視点
		XMVECTOR s0 = XMLoadFloat3(&g_Cam.tbl_adr[index + 0].scl);	// 現在の注視点
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&g_Cam.atPos, s0 + scl * time);

	}
}


//=============================================================================
// カメラの更新
//=============================================================================
void SetCamera(void) 
{
	// ビューマトリックス設定
	XMMATRIX mtxView;
	mtxView = XMMatrixLookAtLH(XMLoadFloat3(&g_Cam.pos), XMLoadFloat3(&g_Cam.at), XMLoadFloat3(&g_Cam.up));
	SetViewMatrix(&mtxView);
	XMStoreFloat4x4(&g_Cam.mtxView, mtxView);

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);
	XMStoreFloat4x4(&g_Cam.mtxInvView, mtxInvView);


	// プロジェクションマトリックス設定
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z);

	SetProjectionMatrix(&mtxProjection);
	XMStoreFloat4x4(&g_Cam.mtxProjection, mtxProjection);

	SetShaderCamera(g_Cam.pos);
}


//=============================================================================
// カメラの取得
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Cam;
}

//=============================================================================
// ビューポートの設定
//=============================================================================
void SetViewPort(int type)
{
	//ID3D11DeviceContext *g_ImmediateContext = GetDeviceContext();
	//D3D11_VIEWPORT vp;

	//g_ViewPortType = type;

	//// ビューポート設定
	//switch (g_ViewPortType)
	//{
	//case TYPE_FULL_SCREEN:
	//case TYPE_LIGHT_SCREEN:
	//	vp.Width = (FLOAT)SCREEN_WIDTH;
	//	vp.Height = (FLOAT)SCREEN_HEIGHT;
	//	vp.MinDepth = 0.0f;
	//	vp.MaxDepth = 1.0f;
	//	vp.TopLeftX = 0;
	//	vp.TopLeftY = 0;
	//	break;

	//case TYPE_LEFT_HALF_SCREEN:
	//	vp.Width = (FLOAT)SCREEN_WIDTH / 2;
	//	vp.Height = (FLOAT)SCREEN_HEIGHT;
	//	vp.MinDepth = 0.0f;
	//	vp.MaxDepth = 1.0f;
	//	vp.TopLeftX = 0;
	//	vp.TopLeftY = 0;
	//	break;

	//case TYPE_RIGHT_HALF_SCREEN:
	//	vp.Width = (FLOAT)SCREEN_WIDTH / 2;
	//	vp.Height = (FLOAT)SCREEN_HEIGHT;
	//	vp.MinDepth = 0.0f;
	//	vp.MaxDepth = 1.0f;
	//	vp.TopLeftX = (FLOAT)SCREEN_WIDTH / 2;
	//	vp.TopLeftY = 0;
	//	break;

	//case TYPE_UP_HALF_SCREEN:
	//	vp.Width = (FLOAT)SCREEN_WIDTH;
	//	vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
	//	vp.MinDepth = 0.0f;
	//	vp.MaxDepth = 1.0f;
	//	vp.TopLeftX = 0;
	//	vp.TopLeftY = 0;
	//	break;

	//case TYPE_DOWN_HALF_SCREEN:
	//	vp.Width = (FLOAT)SCREEN_WIDTH;
	//	vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
	//	vp.MinDepth = 0.0f;
	//	vp.MaxDepth = 1.0f;
	//	vp.TopLeftX = 0;
	//	vp.TopLeftY = (FLOAT)SCREEN_HEIGHT / 2;
	//	break;

	//}
	//g_ImmediateContext->RSSetViewports(1, &vp);

}


int GetViewPortType(void)
{
	return g_ViewPortType;
}


// カメラの視点と注視点セット
void SetCameraAT(XMFLOAT3 pos)
{
	XMVECTOR v1 = XMLoadFloat3(&pos) - XMLoadFloat3(&g_Cam.at);
	XMVECTOR nor = XMVector3Normalize(v1);
	XMStoreFloat3(&g_Cam.at, XMLoadFloat3(&g_Cam.at) + nor * 1.0f);

	//引数の座標に変更量を加算。
	pos.x += g_Cam.atPos.x;
	pos.y += g_Cam.atPos.y;
	pos.z += g_Cam.atPos.z;
	// カメラの注視点をセット
	g_Cam.at = { pos.x,pos.y,pos.z };
	g_Cam.rot = { 0.0f, 0.0f, 0.0f };

	// カメラの視点をカメラのY軸回転に対応させている
	g_Cam.pos.x = g_Cam.at.x - sinf(g_Cam.rot.y) * g_Cam.len;
	g_Cam.pos.z = g_Cam.at.z - cosf(g_Cam.rot.y) * g_Cam.len;
}

void SetCharaCamera(XMFLOAT3 pos)
{
	// カメラの注視点をセット
	g_Cam.at = { pos.x,pos.y,pos.z };

	XMVECTOR v1 = XMLoadFloat3(&pos) - XMLoadFloat3(&g_Cam.pos);
	XMFLOAT3 fpos;
	XMStoreFloat3(&fpos, v1);
	float angle = atan2f(fpos.z, fpos.x);
	g_Cam.rot.y = angle;
}