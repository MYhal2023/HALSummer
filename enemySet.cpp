
#include "enemy.h"
#include "enemyLinerData.h"


//黄色ブドウ球菌用のセット関数
//引数1:何フレームの時に出すのか、体力、攻撃力、防御力
void SetGrape(float time, int life, int power, int deffend)
{
	ENEMY *g_Enemy = GetEnemy();
	EnemyParts *g_Parts = GetEnemyParts();
	LoadModel(MODEL_GRAPE, &g_Enemy[GetEnemyNum()].model);
	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_Enemy[GetEnemyNum()].model, &g_Enemy[GetEnemyNum()].diffuse[0]);
	g_Enemy[GetEnemyNum()].load = TRUE;

	g_Enemy[GetEnemyNum()].pos = { moveTbl[0].start.x, 0.0f, moveTbl[0].start.z };
	g_Enemy[GetEnemyNum()].rot = { 0.0f, 0.0f, 0.0f };
	g_Enemy[GetEnemyNum()].scl = { 0.8f, 1.0f, 1.0f };

	g_Enemy[GetEnemyNum()].size = ENEMY_SIZE;	// 当たり判定の大きさ
	g_Enemy[GetEnemyNum()].life = life;
	g_Enemy[GetEnemyNum()].lifeMax = g_Enemy[GetEnemyNum()].life;
	g_Enemy[GetEnemyNum()].spd = 1.0f;
	g_Enemy[GetEnemyNum()].use = FALSE;
	g_Enemy[GetEnemyNum()].attack = FALSE;
	g_Enemy[GetEnemyNum()].attackUse = FALSE;
	g_Enemy[GetEnemyNum()].atInterval = 180;
	g_Enemy[GetEnemyNum()].atFrame = 20;
	g_Enemy[GetEnemyNum()].atFrameCount = 0;
	g_Enemy[GetEnemyNum()].power = power;
	g_Enemy[GetEnemyNum()].diffend = deffend;
	g_Enemy[GetEnemyNum()].target = NULL;
	g_Enemy[GetEnemyNum()].atCount = g_Enemy[GetEnemyNum()].atInterval;	//最初はノータイムで攻撃モーションへ
	g_Enemy[GetEnemyNum()].type = Proximity;;
	g_Enemy[GetEnemyNum()].start = time;
	g_Enemy[GetEnemyNum()].moveData = moveTbl;
	g_Enemy[GetEnemyNum()].moveCount = 0.0f;
	g_Enemy[GetEnemyNum()].moveTblSize = sizeof(moveTbl) / sizeof(MOVERINE);
	g_Enemy[GetEnemyNum()].nowTbl = 0;
	g_Enemy[GetEnemyNum()].tbl_adrA = grape_Attack;	// アニメデータのテーブル先頭アドレス
	g_Enemy[GetEnemyNum()].tbl_adrM = grape_Move;	// アニメデータのテーブル先頭アドレス
	g_Enemy[GetEnemyNum()].tbl_sizeA = sizeof(grape_Attack) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Enemy[GetEnemyNum()].tbl_sizeM = sizeof(grape_Move) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Enemy[GetEnemyNum()].move_time = 0.0f;	// 実行時間
	g_Enemy[GetEnemyNum()].partsNum = 3;
	g_Enemy[GetEnemyNum()].startNum = GetEnemyPartsNum();
	// 階層アニメーション用の初期化処理
	g_Enemy[GetEnemyNum()].parent = NULL;			// 本体（親）なのでNULLを入れる
	{
		LoadModel(MODEL_GRAPE_PARTS001, &g_Parts[GetEnemyPartsNum()].model);
		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Parts[GetEnemyPartsNum()].model, &g_Parts[GetEnemyPartsNum()].diffuse[0]);
		g_Parts[GetEnemyPartsNum()].load = TRUE;

		g_Parts[GetEnemyPartsNum()].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
		g_Parts[GetEnemyPartsNum()].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[GetEnemyPartsNum()].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

		// 階層アニメーション用のメンバー変数
		g_Parts[GetEnemyPartsNum()].tbl_adrA = grape_Parts001Attack;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_adrM = grape_Parts001Move;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_sizeA = sizeof(grape_Parts001Attack) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].tbl_sizeM = sizeof(grape_Parts001Move) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].move_time = 0;	// 実行時間
		g_Parts[GetEnemyPartsNum()].parent = &g_Enemy[GetEnemyNum()];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
		SetEnemyPartsNum();
	}

	{
		LoadModel(MODEL_GRAPE_PARTS002, &g_Parts[GetEnemyPartsNum()].model);
		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Parts[GetEnemyPartsNum()].model, &g_Parts[GetEnemyPartsNum()].diffuse[0]);
		g_Parts[GetEnemyPartsNum()].load = TRUE;

		g_Parts[GetEnemyPartsNum()].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
		g_Parts[GetEnemyPartsNum()].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[GetEnemyPartsNum()].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)
		// 階層アニメーション用のメンバー変数
		g_Parts[GetEnemyPartsNum()].tbl_adrA = grape_Parts002Attack;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_adrM = grape_Parts002Move;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_sizeA = sizeof(grape_Parts002Attack) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].tbl_sizeM = sizeof(grape_Parts002Move) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].move_time = 0;	// 実行時間
		g_Parts[GetEnemyPartsNum()].parent = &g_Enemy[GetEnemyNum()];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
		SetEnemyPartsNum();
	}
	{
		LoadModel(MODEL_GRAPE_PARTS003, &g_Parts[GetEnemyPartsNum()].model);
		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Parts[GetEnemyPartsNum()].model, &g_Parts[GetEnemyPartsNum()].diffuse[0]);
		g_Parts[GetEnemyPartsNum()].load = TRUE;

		g_Parts[GetEnemyPartsNum()].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
		g_Parts[GetEnemyPartsNum()].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[GetEnemyPartsNum()].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

			// 階層アニメーション用のメンバー変数
		g_Parts[GetEnemyPartsNum()].tbl_adrA = grape_Parts003Attack;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_adrM = grape_Parts003Move;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_sizeA = sizeof(grape_Parts003Attack) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].tbl_sizeM = sizeof(grape_Parts003Move) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].move_time = 0;	// 実行時間
		g_Parts[GetEnemyPartsNum()].parent = &g_Enemy[GetEnemyNum()];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
		SetEnemyPartsNum();
	}
	SetEnemyNum();
}
