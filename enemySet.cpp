
#include "enemy.h"
#include "enemyLinerData.h"


//黄色ブドウ球菌用のセット関数
//引数1:何フレームの時に出すのか、体力、攻撃力、防御力
void SetGrape(float time, int life, int power, int deffend, MOVERINE *move, int size)
{
	ENEMY *g_Enemy = GetEnemy();
	EnemyParts *g_Parts = GetEnemyParts();
	g_Enemy[GetEnemyNum()].load = TRUE;
	g_Enemy[GetEnemyNum()].charID = Grape;

	g_Enemy[GetEnemyNum()].rot = { 0.0f, 0.0f, 0.0f };
	g_Enemy[GetEnemyNum()].scl = { 0.8f, 1.0f, 1.0f };

	g_Enemy[GetEnemyNum()].size = 60.0f;	// 当たり判定の大きさ
	g_Enemy[GetEnemyNum()].life = life;
	g_Enemy[GetEnemyNum()].lifeMax = g_Enemy[GetEnemyNum()].life;
	g_Enemy[GetEnemyNum()].spd = 0.5f;
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
	g_Enemy[GetEnemyNum()].moveData = move;
	g_Enemy[GetEnemyNum()].moveCount = 0.0f;
	g_Enemy[GetEnemyNum()].moveTblSize = size;
	g_Enemy[GetEnemyNum()].pos = { move[0].start.x, 0.0f, move[0].start.z };
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

//A群β溶連菌
//引数1:何フレームの時に出すのか、体力、攻撃力、防御力
void SetStrept(float time, int life, int power, int deffend, MOVERINE *move, int size)
{
	ENEMY *g_Enemy = GetEnemy();
	EnemyParts *g_Parts = GetEnemyParts();
	g_Enemy[GetEnemyNum()].load = TRUE;
	g_Enemy[GetEnemyNum()].charID = Strept;

	g_Enemy[GetEnemyNum()].rot = { 0.0f, 0.0f, 0.0f };
	g_Enemy[GetEnemyNum()].scl = { 0.25f, 0.25f, 0.25f };

	g_Enemy[GetEnemyNum()].size = 30.0f;	// 当たり判定の大きさ
	g_Enemy[GetEnemyNum()].life = life;
	g_Enemy[GetEnemyNum()].lifeMax = g_Enemy[GetEnemyNum()].life;
	g_Enemy[GetEnemyNum()].spd = 0.5f;
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
	g_Enemy[GetEnemyNum()].moveData = move;
	g_Enemy[GetEnemyNum()].moveCount = 0.0f;
	g_Enemy[GetEnemyNum()].moveTblSize = size;
	g_Enemy[GetEnemyNum()].pos = { move[0].start.x, 0.0f, move[0].start.z };
	g_Enemy[GetEnemyNum()].nowTbl = 0;
	g_Enemy[GetEnemyNum()].tbl_adrA = strept_Attack;	// アニメデータのテーブル先頭アドレス
	g_Enemy[GetEnemyNum()].tbl_adrM = strept_Move;	// アニメデータのテーブル先頭アドレス
	g_Enemy[GetEnemyNum()].tbl_sizeA = sizeof(strept_Attack) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Enemy[GetEnemyNum()].tbl_sizeM = sizeof(strept_Move) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Enemy[GetEnemyNum()].move_time = 0.0f;	// 実行時間
	g_Enemy[GetEnemyNum()].partsNum = 3;
	g_Enemy[GetEnemyNum()].startNum = GetEnemyPartsNum();
	// 階層アニメーション用の初期化処理
	g_Enemy[GetEnemyNum()].parent = NULL;			// 本体（親）なのでNULLを入れる
	{
		g_Parts[GetEnemyPartsNum()].load = TRUE;

		g_Parts[GetEnemyPartsNum()].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
		g_Parts[GetEnemyPartsNum()].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[GetEnemyPartsNum()].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

		// 階層アニメーション用のメンバー変数
		g_Parts[GetEnemyPartsNum()].tbl_adrA = strept_headAt;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_adrM = strept_headMv;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_sizeA = sizeof(strept_headAt) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].tbl_sizeM = sizeof(strept_headMv) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].move_time = 0;	// 実行時間
		g_Parts[GetEnemyPartsNum()].parent = &g_Enemy[GetEnemyNum()];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
		SetEnemyPartsNum();
	}

	{
		g_Parts[GetEnemyPartsNum()].load = TRUE;

		g_Parts[GetEnemyPartsNum()].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
		g_Parts[GetEnemyPartsNum()].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[GetEnemyPartsNum()].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)
		// 階層アニメーション用のメンバー変数
		g_Parts[GetEnemyPartsNum()].tbl_adrA = strept_neckAt;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_adrM = strept_neckMv;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_sizeA = sizeof(strept_neckAt) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].tbl_sizeM = sizeof(strept_neckMv) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].move_time = 0;	// 実行時間
		g_Parts[GetEnemyPartsNum()].parent = &g_Enemy[GetEnemyNum()];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
		SetEnemyPartsNum();
	}
	{
		g_Parts[GetEnemyPartsNum()].load = TRUE;

		g_Parts[GetEnemyPartsNum()].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
		g_Parts[GetEnemyPartsNum()].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[GetEnemyPartsNum()].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

			// 階層アニメーション用のメンバー変数
		g_Parts[GetEnemyPartsNum()].tbl_adrA = strept_tailAt;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_adrM = strept_tailMv;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_sizeA = sizeof(strept_tailAt) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].tbl_sizeM = sizeof(strept_tailMv) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].move_time = 0;	// 実行時間
		g_Parts[GetEnemyPartsNum()].parent = &g_Enemy[GetEnemyNum()];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
		SetEnemyPartsNum();
	}
	SetEnemyNum();
}

//緑膿菌
//引数1:何フレームの時に出すのか、体力、攻撃力、防御力
void SetRyoku(float time, int life, int power, int deffend, MOVERINE *move, int size)
{
	ENEMY *g_Enemy = GetEnemy();
	EnemyParts *g_Parts = GetEnemyParts();
	g_Enemy[GetEnemyNum()].load = TRUE;
	g_Enemy[GetEnemyNum()].charID = Ryoku;

	g_Enemy[GetEnemyNum()].rot = { 0.0f, 0.0f, 0.0f };
	g_Enemy[GetEnemyNum()].scl = { 2.0f, 2.0f, 2.0f };

	g_Enemy[GetEnemyNum()].size = 30.0f;	// 当たり判定の大きさ
	g_Enemy[GetEnemyNum()].life = life;
	g_Enemy[GetEnemyNum()].lifeMax = g_Enemy[GetEnemyNum()].life;
	g_Enemy[GetEnemyNum()].spd = 0.5f;
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
	g_Enemy[GetEnemyNum()].type = Proximity;
	g_Enemy[GetEnemyNum()].start = time;
	g_Enemy[GetEnemyNum()].moveData = move;
	g_Enemy[GetEnemyNum()].moveCount = 0.0f;
	g_Enemy[GetEnemyNum()].moveTblSize = size;
	g_Enemy[GetEnemyNum()].pos = { move[0].start.x, 0.0f, move[0].start.z };
	g_Enemy[GetEnemyNum()].nowTbl = 0;
	g_Enemy[GetEnemyNum()].tbl_adrA = ryoku_Attack;	// アニメデータのテーブル先頭アドレス
	g_Enemy[GetEnemyNum()].tbl_adrM = ryoku_Move;	// アニメデータのテーブル先頭アドレス
	g_Enemy[GetEnemyNum()].tbl_sizeA = sizeof(ryoku_Attack) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Enemy[GetEnemyNum()].tbl_sizeM = sizeof(ryoku_Move) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Enemy[GetEnemyNum()].move_time = 0.0f;	// 実行時間
	g_Enemy[GetEnemyNum()].partsNum = 2;
	g_Enemy[GetEnemyNum()].startNum = GetEnemyPartsNum();
	// 階層アニメーション用の初期化処理
	g_Enemy[GetEnemyNum()].parent = NULL;			// 本体（親）なのでNULLを入れる
	{
		g_Parts[GetEnemyPartsNum()].load = TRUE;

		g_Parts[GetEnemyPartsNum()].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
		g_Parts[GetEnemyPartsNum()].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[GetEnemyPartsNum()].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

		// 階層アニメーション用のメンバー変数
		g_Parts[GetEnemyPartsNum()].tbl_adrA = ryoku_leftAt;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_adrM = ryoku_leftMv;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_sizeA = sizeof(ryoku_leftAt) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].tbl_sizeM = sizeof(ryoku_leftMv) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].move_time = 0;	// 実行時間
		g_Parts[GetEnemyPartsNum()].parent = &g_Enemy[GetEnemyNum()];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
		SetEnemyPartsNum();
	}

	{
		g_Parts[GetEnemyPartsNum()].load = TRUE;

		g_Parts[GetEnemyPartsNum()].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
		g_Parts[GetEnemyPartsNum()].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[GetEnemyPartsNum()].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)
		// 階層アニメーション用のメンバー変数
		g_Parts[GetEnemyPartsNum()].tbl_adrA = ryoku_rightAt;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_adrM = ryoku_rightMv;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_sizeA = sizeof(ryoku_rightAt) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].tbl_sizeM = sizeof(ryoku_rightMv) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].move_time = 0;	// 実行時間
		g_Parts[GetEnemyPartsNum()].parent = &g_Enemy[GetEnemyNum()];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
		SetEnemyPartsNum();
	}
	SetEnemyNum();
}

void SetInflue(float time, int life, int power, int deffend, MOVERINE *move, int size)
{
	ENEMY *g_Enemy = GetEnemy();
	EnemyParts *g_Parts = GetEnemyParts();
	g_Enemy[GetEnemyNum()].load = TRUE;
	g_Enemy[GetEnemyNum()].charID = Infule;

	g_Enemy[GetEnemyNum()].rot = { 0.0f, 0.0f, 0.0f };
	g_Enemy[GetEnemyNum()].scl = { 0.8f, 1.0f, 1.0f };

	g_Enemy[GetEnemyNum()].size = 60.0f;	// 当たり判定の大きさ
	g_Enemy[GetEnemyNum()].life = life;
	g_Enemy[GetEnemyNum()].lifeMax = g_Enemy[GetEnemyNum()].life;
	g_Enemy[GetEnemyNum()].spd = 0.5f;
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
	g_Enemy[GetEnemyNum()].moveData = move;
	g_Enemy[GetEnemyNum()].moveCount = 0.0f;
	g_Enemy[GetEnemyNum()].moveTblSize = size;
	g_Enemy[GetEnemyNum()].pos = { move[0].start.x, 0.0f, move[0].start.z };
	g_Enemy[GetEnemyNum()].nowTbl = 0;
	g_Enemy[GetEnemyNum()].tbl_adrA = infule_Attack;	// アニメデータのテーブル先頭アドレス
	g_Enemy[GetEnemyNum()].tbl_adrM = infule_Move;	// アニメデータのテーブル先頭アドレス
	g_Enemy[GetEnemyNum()].tbl_sizeA = sizeof(infule_Attack) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Enemy[GetEnemyNum()].tbl_sizeM = sizeof(infule_Move) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
	g_Enemy[GetEnemyNum()].move_time = 0.0f;	// 実行時間
	g_Enemy[GetEnemyNum()].partsNum = 2;
	g_Enemy[GetEnemyNum()].startNum = GetEnemyPartsNum();
	// 階層アニメーション用の初期化処理
	g_Enemy[GetEnemyNum()].parent = NULL;			// 本体（親）なのでNULLを入れる
	{
		g_Parts[GetEnemyPartsNum()].load = TRUE;

		g_Parts[GetEnemyPartsNum()].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
		g_Parts[GetEnemyPartsNum()].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[GetEnemyPartsNum()].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)

		// 階層アニメーション用のメンバー変数
		g_Parts[GetEnemyPartsNum()].tbl_adrA = infule_Parts001Attack;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_adrM = infule_Parts001Move;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_sizeA = sizeof(infule_Parts001Attack) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].tbl_sizeM = sizeof(infule_Parts001Move) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].move_time = 0;	// 実行時間
		g_Parts[GetEnemyPartsNum()].parent = &g_Enemy[GetEnemyNum()];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
		SetEnemyPartsNum();
	}

	{
		g_Parts[GetEnemyPartsNum()].load = TRUE;

		g_Parts[GetEnemyPartsNum()].pos = { 0.0f, 0.0f, 0.0f };		// ポリゴンの位置
		g_Parts[GetEnemyPartsNum()].rot = { 0.0f, 0.0f, 0.0f };		// ポリゴンの向き(回転)
		g_Parts[GetEnemyPartsNum()].scl = { 1.0f, 1.0f, 1.0f };		// ポリゴンの大きさ(スケール)
		// 階層アニメーション用のメンバー変数
		g_Parts[GetEnemyPartsNum()].tbl_adrA = infule_Parts002Attack;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_adrM = infule_Parts002Move;	// アニメデータのテーブル先頭アドレス
		g_Parts[GetEnemyPartsNum()].tbl_sizeA = sizeof(infule_Parts002Attack) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].tbl_sizeM = sizeof(infule_Parts002Move) / sizeof(INTERPOLATION_DATA);	// 登録したテーブルのレコード総数
		g_Parts[GetEnemyPartsNum()].move_time = 0;	// 実行時間
		g_Parts[GetEnemyPartsNum()].parent = &g_Enemy[GetEnemyNum()];	// 自分が親ならNULL、自分が子供なら親のenemyアドレス
		SetEnemyPartsNum();
	}
	SetEnemyNum();
}
