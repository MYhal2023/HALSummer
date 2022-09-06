//=============================================================================
//
//  スキル処理[skill.cpp]
// Author : 
//好中球のみに行う特殊処理を記載
//=============================================================================
#include "player.h"
#include "skill.h"
#include "cost.h"
#include "game.h"
#include "sound.h"
#include "enemy.h"
#include "collision.h"
#include "base.h"
#include "bullet.h"
#define SKILL_TIME (600)
static int atNum = 0;
//好中球のスキル処理
void NeutroSkill(PLAYER *player)
{
	IncreaseCost(6);
	player->skillUse = FALSE;
	player->state = Standby;
}

//マクロファージのスキル処理
void MacroSkill(PLAYER *player, Playerliner *line, PlayerParts *parts)
{
	float attackCt = (float)(player->power);
	player->power = (int)(attackCt * 1.5f);
	//攻撃範囲内に敵がいる場合
	if (player->count > 0)
	{
		MacroInterPoration(player, line, parts);
	}
	else
	{
		MacroStand(player, line, parts);
	}

	//スキル時間を超過したならスキルを切る
	player->skillTime++;
	if (player->skillTime >= player->skillTimeMax)
	{
		player->skillUse = FALSE;
		player->state = Standby;
		player->skillTime = 0;
	}
	player->power = (int)(attackCt);
}

//ヘルパーTのスキル処理
void helperTSkill(PLAYER *player, Playerliner *line, PlayerParts *parts)
{
	//2倍速で行動
	for (int i = 0; i < 2; i++) {
		//攻撃範囲内に味方がいる場合
		if (player->count > 0)
		{
			HelperInterPoration(player, line, parts);
		}
		else
		{
			HelperStand(player, line, parts);
		}
	}
	//スキル時間を超過したならスキルを切る
	player->skillTime++;
	if (player->skillTime >= player->skillTimeMax)
	{
		player->skillUse = FALSE;
		player->state = Standby;
		player->skillTime = 0;
	}
}

//キラーTのスキル処理
void KillerSkill(PLAYER *player, Playerliner *line, PlayerParts *parts)
{
	float attackCt = (float)(player->power);
	float time = (float)(player->skillPointMax);
	player->power = (int)(attackCt * 1.2f);
	player->skillPointMax = (int)(time * 1.2f);
	player->skillUse = FALSE;
	player->state = Standby;
	player->skillTime = 0;
}

//NKのスキル処理 これの中身キモいので開けないの推奨
void NKSkill(PLAYER *player, Playerliner *line, PlayerParts *parts)
{
	for (int i = 0; i < 2; i++) {
		//エネミー探査
		player->count = 0;
		for (int k = 0; k < MAX_TARGET; k++)
		{
			player->targetable[k] = 99;
		}

		for (int k = 0; k < MAX_ENEMY; k++)
		{
			ENEMY *enemy = GetEnemy();
			if (enemy[k].use != TRUE)continue;
			//プレイヤーの攻撃範囲に1体でも敵がいるならば攻撃準備に入る。ターゲット情報も保存
			if (CollisionBC(player->pos, enemy[k].pos, player->size * 0.5f, player->size * 0.5f))
			{
				player->targetable[player->count] = k;
				player->count++;
			}
		}
		//攻撃範囲内に敵がいる場合
		if (player->count > 0)
		{
			Base *base = GetBase();
			ENEMY *enemy = GetEnemy();
			float cmp = 0.0f;
			for (int k = 0; k < player->count; k++)
			{
				for (int j = 0; j < base->baseNum; j++)
				{
					XMVECTOR v1 = XMLoadFloat3(&base->pos[j]) - XMLoadFloat3(&enemy[player->targetable[k]].pos);
					XMFLOAT3 countData;
					XMStoreFloat3(&countData, v1);
					float dist = fabsf(countData.x) + fabsf(countData.y) + fabsf(countData.z);
					if (dist < cmp || cmp <= 0.0f)//初めてここを通る場合は絶対入れるために初期値を参照
					{
						cmp = dist;
						player->target = player->targetable[k];	//エネミーの配列番号で識別。ポインターで渡したいけど、お互いの構造体にポインターメンバ変数を入れると怒られる…
						XMVECTOR v2 = XMLoadFloat3(&enemy[player->targetable[k]].pos) - XMLoadFloat3(&player->pos);
						XMStoreFloat3(&countData, v2);
						float angle = atan2f(countData.x, countData.z);
						if (player->rot.y == 0.0f)player->rot.y = XM_PI * 2.0f;	//バグ修正の為、処理を付け足した
						for (int d = 0; d < player->tbl_sizeA; d++)
						{
							float buffx = player->tbl_adrA[d].pos.x;
							float buffz = player->tbl_adrA[d].pos.z;
							player->tbl_adrA[d].pos.x = buffx * cosf(angle - player->rot.y) + buffz * sinf(angle - player->rot.y);
							player->tbl_adrA[d].pos.z = buffz * cosf(angle - player->rot.y) + buffx * -sinf(angle - player->rot.y);
						}
						player->rot.y = angle;
					}
				}
			}


			NKInterPoration(player, line, parts);
		}
		else
		{
			NKStand(player, line, parts);
		}
	}

	//スキル時間を超過したならスキルを切る
	player->skillTime++;
	if (player->skillTime >= player->skillTimeMax)
	{
		player->skillUse = FALSE;
		player->state = Standby;
		player->skillTime = 0;
	}
}

//好塩基球のスキル処理
void KouenSkill(PLAYER *player, Playerliner *line, PlayerParts *parts)
{
	if (player->skillTime <= 0) {
		player->attackUse = FALSE;
		player->atFrame = 5;
		atNum = 0;
	}
	//エネミー探査
	player->count = 0;
	for (int k = 0; k < MAX_TARGET; k++)
	{
		player->targetable[k] = 99;
	}

	for (int k = 0; k < MAX_ENEMY; k++)
	{
		ENEMY *enemy = GetEnemy();
		if (enemy[k].use != TRUE)continue;
		//プレイヤーの攻撃範囲に1体でも敵がいるならば攻撃準備に入る。ターゲット情報も保存
		if (CollisionBC(player->pos, enemy[k].pos, player->size * 0.5f, player->size * 0.5f))
		{
			player->targetable[player->count] = k;
			player->count++;
		}
	}
	//攻撃範囲内に敵がいる場合
	if (player->count > 0)
	{
		Base *base = GetBase();
		ENEMY *enemy = GetEnemy();
		float cmp = 0.0f;
		for (int k = 0; k < player->count; k++)
		{
			for (int j = 0; j < base->baseNum; j++)
			{
				XMVECTOR v1 = XMLoadFloat3(&base->pos[j]) - XMLoadFloat3(&enemy[player->targetable[k]].pos);
				XMFLOAT3 countData;
				XMStoreFloat3(&countData, v1);
				float dist = fabsf(countData.x) + fabsf(countData.y) + fabsf(countData.z);
				if (dist < cmp || cmp <= 0.0f)//初めてここを通る場合は絶対入れるために初期値を参照
				{
					cmp = dist;
					player->target = player->targetable[k];	//エネミーの配列番号で識別。ポインターで渡したいけど、お互いの構造体にポインターメンバ変数を入れると怒られる…
					XMVECTOR v2 = XMLoadFloat3(&enemy[player->targetable[k]].pos) - XMLoadFloat3(&player->pos);
					XMStoreFloat3(&countData, v2);
					float angle = atan2f(countData.x, countData.z);
					if (player->rot.y == 0.0f)player->rot.y = XM_PI * 2.0f;	//バグ修正の為、処理を付け足した
					for (int d = 0; d < player->tbl_sizeA; d++)
					{
						float buffx = player->tbl_adrA[d].pos.x;
						float buffz = player->tbl_adrA[d].pos.z;
						player->tbl_adrA[d].pos.x = buffx * cosf(angle - player->rot.y) + buffz * sinf(angle - player->rot.y);
						player->tbl_adrA[d].pos.z = buffz * cosf(angle - player->rot.y) + buffx * -sinf(angle - player->rot.y);
					}
					player->rot.y = angle;
				}
			}
		}


		KouenInterPoration(player, line, parts);
	}
	else
	{
		KouenStand(player, line, parts);
	}

	//スキル時間を超過したならスキルを切る
	player->skillTime++;
	if (player->skillTime >= player->skillTimeMax)
	{
		player->skillUse = FALSE;
		player->state = Standby;
		player->skillTime = 0;
		player->atFrame = 5;
		atNum = 0;
	}
}

//こっから主に線形補間と細かい攻撃処理
void MacroInterPoration(PLAYER *player, Playerliner *line, PlayerParts *parts)
{
	if (player->tbl_adrA == NULL)return;	// 線形補間を実行する？

	//
	// 線形補間の処理
	// 移動処理
	int		index = (int)player->move_time;
	float	time = player->move_time - index;
	int		size = player->tbl_sizeA;

	float dt = 1.0f / player->tbl_adrA[index].frame;	// 1フレームで進める時間
	player->move_time += dt;							// アニメーションの合計時間に足す

	if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
	{
		player->move_time = 0.0f;
		index = 0;
		player->atCount = 0;
		player->state = Standby;
		player->attackUse = FALSE;
		line->pos = { 0.0f, 0.0f, 0.0f };
		line->rot = { 0.0f, 0.0f, 0.0f };
		line->scl = { 0.0f, 0.0f, 0.0f };
	}
	// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
	XMVECTOR p1 = XMLoadFloat3(&player->tbl_adrA[index + 1].pos);	// 次の場所
	XMVECTOR p0 = XMLoadFloat3(&player->tbl_adrA[index + 0].pos);	// 現在の場所
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&line->pos, p0 + vec * time);

	// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
	XMVECTOR r1 = XMLoadFloat3(&player->tbl_adrA[index + 1].rot);	// 次の角度
	XMVECTOR r0 = XMLoadFloat3(&player->tbl_adrA[index + 0].rot);	// 現在の角度
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&line->rot, r0 + rot * time);

	// scaleを求める S = StartX + (EndX - StartX) * 今の時間
	XMVECTOR s1 = XMLoadFloat3(&player->tbl_adrA[index + 1].scl);	// 次のScale
	XMVECTOR s0 = XMLoadFloat3(&player->tbl_adrA[index + 0].scl);	// 現在のScale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&line->scl, s0 + scl * time);

	for (int k = player->startNum; k < player->startNum + player->partsNum; k++)
	{
		if (parts[k].tbl_adrA == NULL)continue;;	// 線形補間を実行する？

		int		index = (int)parts[k].move_time;
		float	time = parts[k].move_time - index;
		int		size = parts[k].tbl_sizeA;

		float dt = 1.0f / parts[k].tbl_adrA[index].frame;	// 1フレームで進める時間
		parts[k].move_time += dt;							// アニメーションの合計時間に足す

		if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
		{
			parts[k].move_time = 0.0f;
			index = 0;
		}

		// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
		XMVECTOR p1 = XMLoadFloat3(&parts[k].tbl_adrA[index + 1].pos);	// 次の場所
		XMVECTOR p0 = XMLoadFloat3(&parts[k].tbl_adrA[index + 0].pos);	// 現在の場所
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&parts[k].pos, p0 + vec * time);

		// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
		XMVECTOR r1 = XMLoadFloat3(&parts[k].tbl_adrA[index + 1].rot);	// 次の角度
		XMVECTOR r0 = XMLoadFloat3(&parts[k].tbl_adrA[index + 0].rot);	// 現在の角度
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&parts[k].rot, r0 + rot * time);

		// scaleを求める S = StartX + (EndX - StartX) * 今の時間
		XMVECTOR s1 = XMLoadFloat3(&parts[k].tbl_adrA[index + 1].scl);	// 次のScale
		XMVECTOR s0 = XMLoadFloat3(&parts[k].tbl_adrA[index + 0].scl);	// 現在のScale
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&parts[k].scl, s0 + scl * time);
	}

	//ここから攻撃処理
	if (player->attackUse == TRUE)return;	//セットしていない、セットする必要がない攻撃があるかも？
	player->atFrameCount++;
	ENEMY *enemy = GetEnemy();
	//攻撃フレームに達したら、ダメージ計算関数を元にターゲットにダメージ
	if (player->atFrameCount >= player->atFrame) {
		for (int s = 0; s < player->count; s++) {
			enemy[player->targetable[s]].life -= DamageFunc(player->power, enemy[player->targetable[s]].diffend);
		}
		player->atFrameCount = 0;
		player->attackUse = TRUE;
		PlaySound(player->attackSE);
	}
}

void MacroStand(PLAYER *player, Playerliner *line, PlayerParts *parts)
{
	if (player->tbl_adrM == NULL)return;	// 線形補間を実行する？

//
// 線形補間の処理
// 移動処理
	int		index = (int)player->move_time;
	float	time = player->move_time - index;
	int		size = player->tbl_sizeM;

	float dt = 1.0f / player->tbl_adrM[index].frame;	// 1フレームで進める時間
	player->move_time += dt;							// アニメーションの合計時間に足す

	if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
	{
		player->move_time = 0.0f;
		index = 0;
		line->pos = { 0.0f, 0.0f, 0.0f };
		line->rot = { 0.0f, 0.0f, 0.0f };
		line->scl = { 0.0f, 0.0f, 0.0f };
	}

	// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
	XMVECTOR p1 = XMLoadFloat3(&player->tbl_adrM[index + 1].pos);	// 次の場所
	XMVECTOR p0 = XMLoadFloat3(&player->tbl_adrM[index + 0].pos);	// 現在の場所
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&line->pos, p0 + vec * time);

	// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
	XMVECTOR r1 = XMLoadFloat3(&player->tbl_adrM[index + 1].rot);	// 次の角度
	XMVECTOR r0 = XMLoadFloat3(&player->tbl_adrM[index + 0].rot);	// 現在の角度
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&line->rot, r0 + rot * time);

	// scaleを求める S = StartX + (EndX - StartX) * 今の時間
	XMVECTOR s1 = XMLoadFloat3(&player->tbl_adrM[index + 1].scl);	// 次のScale
	XMVECTOR s0 = XMLoadFloat3(&player->tbl_adrM[index + 0].scl);	// 現在のScale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&line->scl, s0 + scl * time);

	for (int k = player->startNum; k < player->startNum + player->partsNum; k++)
	{
		if (parts[k].tbl_adrM == NULL)continue;;	// 線形補間を実行する？

		int		index = (int)parts[k].move_time;
		float	time = parts[k].move_time - index;
		int		size = parts[k].tbl_sizeM;

		float dt = 1.0f / parts[k].tbl_adrM[index].frame;	// 1フレームで進める時間
		parts[k].move_time += dt;							// アニメーションの合計時間に足す

		if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
		{
			parts[k].move_time = 0.0f;
			index = 0;
		}

		// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
		XMVECTOR p1 = XMLoadFloat3(&parts[k].tbl_adrM[index + 1].pos);	// 次の場所
		XMVECTOR p0 = XMLoadFloat3(&parts[k].tbl_adrM[index + 0].pos);	// 現在の場所
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&parts[k].pos, p0 + vec * time);

		// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
		XMVECTOR r1 = XMLoadFloat3(&parts[k].tbl_adrM[index + 1].rot);	// 次の角度
		XMVECTOR r0 = XMLoadFloat3(&parts[k].tbl_adrM[index + 0].rot);	// 現在の角度
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&parts[k].rot, r0 + rot * time);

		// scaleを求める S = StartX + (EndX - StartX) * 今の時間
		XMVECTOR s1 = XMLoadFloat3(&parts[k].tbl_adrM[index + 1].scl);	// 次のScale
		XMVECTOR s0 = XMLoadFloat3(&parts[k].tbl_adrM[index + 0].scl);	// 現在のScale
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&parts[k].scl, s0 + scl * time);
	}
}

void HelperInterPoration(PLAYER *player, Playerliner *line, PlayerParts *parts)
{
	if (player->tbl_adrA == NULL)return;	// 線形補間を実行する？

	//
	// 線形補間の処理
	// 移動処理
	int		index = (int)player->move_time;
	float	time = player->move_time - index;
	int		size = player->tbl_sizeA;

	float dt = 1.0f / player->tbl_adrA[index].frame;	// 1フレームで進める時間
	player->move_time += dt;							// アニメーションの合計時間に足す

	if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
	{
		player->move_time = 0.0f;
		index = 0;
		player->atCount = 0;
		player->state = Standby;
		player->attackUse = FALSE;
		line->pos = { 0.0f, 0.0f, 0.0f };
		line->rot = { 0.0f, 0.0f, 0.0f };
		line->scl = { 0.0f, 0.0f, 0.0f };
	}
	// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
	XMVECTOR p1 = XMLoadFloat3(&player->tbl_adrA[index + 1].pos);	// 次の場所
	XMVECTOR p0 = XMLoadFloat3(&player->tbl_adrA[index + 0].pos);	// 現在の場所
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&line->pos, p0 + vec * time);

	// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
	XMVECTOR r1 = XMLoadFloat3(&player->tbl_adrA[index + 1].rot);	// 次の角度
	XMVECTOR r0 = XMLoadFloat3(&player->tbl_adrA[index + 0].rot);	// 現在の角度
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&line->rot, r0 + rot * time);

	// scaleを求める S = StartX + (EndX - StartX) * 今の時間
	XMVECTOR s1 = XMLoadFloat3(&player->tbl_adrA[index + 1].scl);	// 次のScale
	XMVECTOR s0 = XMLoadFloat3(&player->tbl_adrA[index + 0].scl);	// 現在のScale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&line->scl, s0 + scl * time);

	for (int k = player->startNum; k < player->startNum + player->partsNum; k++)
	{
		if (parts[k].tbl_adrA == NULL)continue;;	// 線形補間を実行する？

		int		index = (int)parts[k].move_time;
		float	time = parts[k].move_time - index;
		int		size = parts[k].tbl_sizeA;

		float dt = 1.0f / parts[k].tbl_adrA[index].frame;	// 1フレームで進める時間
		parts[k].move_time += dt;							// アニメーションの合計時間に足す

		if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
		{
			parts[k].move_time = 0.0f;
			index = 0;
		}

		// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
		XMVECTOR p1 = XMLoadFloat3(&parts[k].tbl_adrA[index + 1].pos);	// 次の場所
		XMVECTOR p0 = XMLoadFloat3(&parts[k].tbl_adrA[index + 0].pos);	// 現在の場所
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&parts[k].pos, p0 + vec * time);

		// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
		XMVECTOR r1 = XMLoadFloat3(&parts[k].tbl_adrA[index + 1].rot);	// 次の角度
		XMVECTOR r0 = XMLoadFloat3(&parts[k].tbl_adrA[index + 0].rot);	// 現在の角度
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&parts[k].rot, r0 + rot * time);

		// scaleを求める S = StartX + (EndX - StartX) * 今の時間
		XMVECTOR s1 = XMLoadFloat3(&parts[k].tbl_adrA[index + 1].scl);	// 次のScale
		XMVECTOR s0 = XMLoadFloat3(&parts[k].tbl_adrA[index + 0].scl);	// 現在のScale
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&parts[k].scl, s0 + scl * time);
	}

	//ここから攻撃処理
	if (player->attackUse == TRUE)return;	//セットしていない、セットする必要がない攻撃があるかも？
	player->atFrameCount++;
	PLAYER *g_Player = GetPlayer();
	//攻撃フレームに達したら、ダメージ計算関数を元にターゲットにダメージ
	if (player->atFrameCount >= player->atFrame) {
		if (player->atFrameCount >= player->atFrame) {
			g_Player[player->target].life += DamageFunc(player->power, 0);
			//回復後は最大値に合わせる
			if (g_Player[player->target].life > g_Player[player->target].lifeMax)
				g_Player[player->target].life = g_Player[player->target].lifeMax;
			player->atFrameCount = 0;
			player->attackUse = TRUE;
			PlaySound(player->attackSE);
		}
	}
}

//待機ﾓｰｼｮﾝ処理
void HelperStand(PLAYER *player, Playerliner *line, PlayerParts *parts)
{
	if (player->tbl_adrM == NULL)return;	// 線形補間を実行する？

//
// 線形補間の処理
// 移動処理
	int		index = (int)player->move_time;
	float	time = player->move_time - index;
	int		size = player->tbl_sizeM;

	float dt = 1.0f / player->tbl_adrM[index].frame;	// 1フレームで進める時間
	player->move_time += dt;							// アニメーションの合計時間に足す

	if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
	{
		player->move_time = 0.0f;
		index = 0;
		line->pos = { 0.0f, 0.0f, 0.0f };
		line->rot = { 0.0f, 0.0f, 0.0f };
		line->scl = { 0.0f, 0.0f, 0.0f };
	}

	// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
	XMVECTOR p1 = XMLoadFloat3(&player->tbl_adrM[index + 1].pos);	// 次の場所
	XMVECTOR p0 = XMLoadFloat3(&player->tbl_adrM[index + 0].pos);	// 現在の場所
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&line->pos, p0 + vec * time);

	// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
	XMVECTOR r1 = XMLoadFloat3(&player->tbl_adrM[index + 1].rot);	// 次の角度
	XMVECTOR r0 = XMLoadFloat3(&player->tbl_adrM[index + 0].rot);	// 現在の角度
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&line->rot, r0 + rot * time);

	// scaleを求める S = StartX + (EndX - StartX) * 今の時間
	XMVECTOR s1 = XMLoadFloat3(&player->tbl_adrM[index + 1].scl);	// 次のScale
	XMVECTOR s0 = XMLoadFloat3(&player->tbl_adrM[index + 0].scl);	// 現在のScale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&line->scl, s0 + scl * time);

	for (int k = player->startNum; k < player->startNum + player->partsNum; k++)
	{
		if (parts[k].tbl_adrM == NULL)continue;;	// 線形補間を実行する？

		int		index = (int)parts[k].move_time;
		float	time = parts[k].move_time - index;
		int		size = parts[k].tbl_sizeM;

		float dt = 1.0f / parts[k].tbl_adrM[index].frame;	// 1フレームで進める時間
		parts[k].move_time += dt;							// アニメーションの合計時間に足す

		if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
		{
			parts[k].move_time = 0.0f;
			index = 0;
		}

		// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
		XMVECTOR p1 = XMLoadFloat3(&parts[k].tbl_adrM[index + 1].pos);	// 次の場所
		XMVECTOR p0 = XMLoadFloat3(&parts[k].tbl_adrM[index + 0].pos);	// 現在の場所
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&parts[k].pos, p0 + vec * time);

		// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
		XMVECTOR r1 = XMLoadFloat3(&parts[k].tbl_adrM[index + 1].rot);	// 次の角度
		XMVECTOR r0 = XMLoadFloat3(&parts[k].tbl_adrM[index + 0].rot);	// 現在の角度
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&parts[k].rot, r0 + rot * time);

		// scaleを求める S = StartX + (EndX - StartX) * 今の時間
		XMVECTOR s1 = XMLoadFloat3(&parts[k].tbl_adrM[index + 1].scl);	// 次のScale
		XMVECTOR s0 = XMLoadFloat3(&parts[k].tbl_adrM[index + 0].scl);	// 現在のScale
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&parts[k].scl, s0 + scl * time);
	}
}

void NKInterPoration(PLAYER *player, Playerliner *line, PlayerParts *parts)
{
	if (player->tbl_adrA == NULL)return;	// 線形補間を実行する？

	//
	// 線形補間の処理
	// 移動処理
	int		index = (int)player->move_time;
	float	time = player->move_time - index;
	int		size = player->tbl_sizeA;

	float dt = 1.0f / player->tbl_adrA[index].frame;	// 1フレームで進める時間
	player->move_time += dt;							// アニメーションの合計時間に足す

	if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
	{
		player->move_time = 0.0f;
		index = 0;
		player->atCount = 0;
		player->state = Standby;
		player->attackUse = FALSE;
		line->pos = { 0.0f, 0.0f, 0.0f };
		line->rot = { 0.0f, 0.0f, 0.0f };
		line->scl = { 0.0f, 0.0f, 0.0f };
	}
	// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
	XMVECTOR p1 = XMLoadFloat3(&player->tbl_adrA[index + 1].pos);	// 次の場所
	XMVECTOR p0 = XMLoadFloat3(&player->tbl_adrA[index + 0].pos);	// 現在の場所
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&line->pos, p0 + vec * time);

	// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
	XMVECTOR r1 = XMLoadFloat3(&player->tbl_adrA[index + 1].rot);	// 次の角度
	XMVECTOR r0 = XMLoadFloat3(&player->tbl_adrA[index + 0].rot);	// 現在の角度
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&line->rot, r0 + rot * time);

	// scaleを求める S = StartX + (EndX - StartX) * 今の時間
	XMVECTOR s1 = XMLoadFloat3(&player->tbl_adrA[index + 1].scl);	// 次のScale
	XMVECTOR s0 = XMLoadFloat3(&player->tbl_adrA[index + 0].scl);	// 現在のScale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&line->scl, s0 + scl * time);

	for (int k = player->startNum; k < player->startNum + player->partsNum; k++)
	{
		if (parts[k].tbl_adrA == NULL)continue;;	// 線形補間を実行する？

		int		index = (int)parts[k].move_time;
		float	time = parts[k].move_time - index;
		int		size = parts[k].tbl_sizeA;

		float dt = 1.0f / parts[k].tbl_adrA[index].frame;	// 1フレームで進める時間
		parts[k].move_time += dt;							// アニメーションの合計時間に足す

		if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
		{
			parts[k].move_time = 0.0f;
			index = 0;
		}

		// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
		XMVECTOR p1 = XMLoadFloat3(&parts[k].tbl_adrA[index + 1].pos);	// 次の場所
		XMVECTOR p0 = XMLoadFloat3(&parts[k].tbl_adrA[index + 0].pos);	// 現在の場所
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&parts[k].pos, p0 + vec * time);

		// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
		XMVECTOR r1 = XMLoadFloat3(&parts[k].tbl_adrA[index + 1].rot);	// 次の角度
		XMVECTOR r0 = XMLoadFloat3(&parts[k].tbl_adrA[index + 0].rot);	// 現在の角度
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&parts[k].rot, r0 + rot * time);

		// scaleを求める S = StartX + (EndX - StartX) * 今の時間
		XMVECTOR s1 = XMLoadFloat3(&parts[k].tbl_adrA[index + 1].scl);	// 次のScale
		XMVECTOR s0 = XMLoadFloat3(&parts[k].tbl_adrA[index + 0].scl);	// 現在のScale
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&parts[k].scl, s0 + scl * time);
	}

	//ここから攻撃処理
	if (player->attackUse == TRUE)return;	//セットしていない、セットする必要がない攻撃があるかも？
	player->atFrameCount++;
	ENEMY *enemy = GetEnemy();
	//演出用のバレットセット
	if (player->skillID == NK_skill && player->atFrameCount == player->atFrame - 5) {
		ENEMY *enemy = GetEnemy();
		XMFLOAT3 setpos = { player->pos.x, player->pos.y + 15.0f, player->pos.z };
		SetBullet(setpos, player->rot, 1.0f, &enemy[player->target]);
	}
	//攻撃フレームに達したら、ダメージ計算関数を元にターゲットにダメージ
	if (player->atFrameCount >= player->atFrame) {
		enemy[player->target].life -= DamageFunc(player->power, enemy[player->target].diffend);
		player->atFrameCount = 0;
		player->attackUse = TRUE;
		PlaySound(player->attackSE);
	}
}

void NKStand(PLAYER *player, Playerliner *line, PlayerParts *parts)
{
	if (player->tbl_adrM == NULL)return;	// 線形補間を実行する？

//
// 線形補間の処理
// 移動処理
	int		index = (int)player->move_time;
	float	time = player->move_time - index;
	int		size = player->tbl_sizeM;

	float dt = 1.0f / player->tbl_adrM[index].frame;	// 1フレームで進める時間
	player->move_time += dt;							// アニメーションの合計時間に足す

	if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
	{
		player->move_time = 0.0f;
		index = 0;
		line->pos = { 0.0f, 0.0f, 0.0f };
		line->rot = { 0.0f, 0.0f, 0.0f };
		line->scl = { 0.0f, 0.0f, 0.0f };
	}

	// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
	XMVECTOR p1 = XMLoadFloat3(&player->tbl_adrM[index + 1].pos);	// 次の場所
	XMVECTOR p0 = XMLoadFloat3(&player->tbl_adrM[index + 0].pos);	// 現在の場所
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&line->pos, p0 + vec * time);

	// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
	XMVECTOR r1 = XMLoadFloat3(&player->tbl_adrM[index + 1].rot);	// 次の角度
	XMVECTOR r0 = XMLoadFloat3(&player->tbl_adrM[index + 0].rot);	// 現在の角度
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&line->rot, r0 + rot * time);

	// scaleを求める S = StartX + (EndX - StartX) * 今の時間
	XMVECTOR s1 = XMLoadFloat3(&player->tbl_adrM[index + 1].scl);	// 次のScale
	XMVECTOR s0 = XMLoadFloat3(&player->tbl_adrM[index + 0].scl);	// 現在のScale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&line->scl, s0 + scl * time);

	for (int k = player->startNum; k < player->startNum + player->partsNum; k++)
	{
		if (parts[k].tbl_adrM == NULL)continue;;	// 線形補間を実行する？

		int		index = (int)parts[k].move_time;
		float	time = parts[k].move_time - index;
		int		size = parts[k].tbl_sizeM;

		float dt = 1.0f / parts[k].tbl_adrM[index].frame;	// 1フレームで進める時間
		parts[k].move_time += dt;							// アニメーションの合計時間に足す

		if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
		{
			parts[k].move_time = 0.0f;
			index = 0;
		}

		// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
		XMVECTOR p1 = XMLoadFloat3(&parts[k].tbl_adrM[index + 1].pos);	// 次の場所
		XMVECTOR p0 = XMLoadFloat3(&parts[k].tbl_adrM[index + 0].pos);	// 現在の場所
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&parts[k].pos, p0 + vec * time);

		// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
		XMVECTOR r1 = XMLoadFloat3(&parts[k].tbl_adrM[index + 1].rot);	// 次の角度
		XMVECTOR r0 = XMLoadFloat3(&parts[k].tbl_adrM[index + 0].rot);	// 現在の角度
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&parts[k].rot, r0 + rot * time);

		// scaleを求める S = StartX + (EndX - StartX) * 今の時間
		XMVECTOR s1 = XMLoadFloat3(&parts[k].tbl_adrM[index + 1].scl);	// 次のScale
		XMVECTOR s0 = XMLoadFloat3(&parts[k].tbl_adrM[index + 0].scl);	// 現在のScale
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&parts[k].scl, s0 + scl * time);
	}
}

void KouenInterPoration(PLAYER *player, Playerliner *line, PlayerParts *parts)
{
	if (player->tbl_adrA == NULL)return;	// 線形補間を実行する？

	//
	// 線形補間の処理
	// 移動処理
	int		index = (int)player->move_time;
	float	time = player->move_time - index;
	int		size = player->tbl_sizeA;

	float dt = 1.0f / player->tbl_adrA[index].frame;	// 1フレームで進める時間
	player->move_time += dt;							// アニメーションの合計時間に足す

	if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
	{
		player->move_time = 0.0f;
		index = 0;
		player->atCount = 0;
		player->state = Standby;
		player->attackUse = FALSE;
		atNum = 0;
		line->pos = { 0.0f, 0.0f, 0.0f };
		line->rot = { 0.0f, 0.0f, 0.0f };
		line->scl = { 0.0f, 0.0f, 0.0f };
	}
	// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
	XMVECTOR p1 = XMLoadFloat3(&player->tbl_adrA[index + 1].pos);	// 次の場所
	XMVECTOR p0 = XMLoadFloat3(&player->tbl_adrA[index + 0].pos);	// 現在の場所
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&line->pos, p0 + vec * time);

	// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
	XMVECTOR r1 = XMLoadFloat3(&player->tbl_adrA[index + 1].rot);	// 次の角度
	XMVECTOR r0 = XMLoadFloat3(&player->tbl_adrA[index + 0].rot);	// 現在の角度
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&line->rot, r0 + rot * time);

	// scaleを求める S = StartX + (EndX - StartX) * 今の時間
	XMVECTOR s1 = XMLoadFloat3(&player->tbl_adrA[index + 1].scl);	// 次のScale
	XMVECTOR s0 = XMLoadFloat3(&player->tbl_adrA[index + 0].scl);	// 現在のScale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&line->scl, s0 + scl * time);

	for (int k = player->startNum; k < player->startNum + player->partsNum; k++)
	{
		if (parts[k].tbl_adrA == NULL)continue;;	// 線形補間を実行する？

		int		index = (int)parts[k].move_time;
		float	time = parts[k].move_time - index;
		int		size = parts[k].tbl_sizeA;

		float dt = 1.0f / parts[k].tbl_adrA[index].frame;	// 1フレームで進める時間
		parts[k].move_time += dt;							// アニメーションの合計時間に足す

		if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
		{
			parts[k].move_time = 0.0f;
			index = 0;
		}

		// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
		XMVECTOR p1 = XMLoadFloat3(&parts[k].tbl_adrA[index + 1].pos);	// 次の場所
		XMVECTOR p0 = XMLoadFloat3(&parts[k].tbl_adrA[index + 0].pos);	// 現在の場所
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&parts[k].pos, p0 + vec * time);

		// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
		XMVECTOR r1 = XMLoadFloat3(&parts[k].tbl_adrA[index + 1].rot);	// 次の角度
		XMVECTOR r0 = XMLoadFloat3(&parts[k].tbl_adrA[index + 0].rot);	// 現在の角度
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&parts[k].rot, r0 + rot * time);

		// scaleを求める S = StartX + (EndX - StartX) * 今の時間
		XMVECTOR s1 = XMLoadFloat3(&parts[k].tbl_adrA[index + 1].scl);	// 次のScale
		XMVECTOR s0 = XMLoadFloat3(&parts[k].tbl_adrA[index + 0].scl);	// 現在のScale
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&parts[k].scl, s0 + scl * time);
	}

	//ここから攻撃処理
	if (player->attackUse &&
		atNum < 12) {
		player->atFrameCount++;
		if (player->atFrameCount >= player->atFrame)
			player->attackUse = FALSE;
	}
	if (player->attackUse == TRUE)return;	//セットしていない、セットする必要がない攻撃があるかも？
	player->atFrameCount++;
	ENEMY *enemy = GetEnemy();
	if (player->atFrameCount >= player->atFrame - 1) {
		ENEMY *enemy = GetEnemy();
		XMFLOAT3 setpos = { player->pos.x + cosf(player->rot.y) * 20.0f, player->pos.y, player->pos.z + sinf(player->rot.y) *20.0f };
		XMFLOAT3 invpos = { player->pos.x - cosf(player->rot.y) * 20.0f, player->pos.y, player->pos.z - sinf(player->rot.y) *20.0f };
		SetBullet(setpos, player->rot, 0.5f, &enemy[player->target]);
		SetBullet(invpos, player->rot, 0.5f, &enemy[player->target]);
	}
	//攻撃フレームに達したら、ダメージ計算関数を元にターゲットにダメージ
	if (player->atFrameCount >= player->atFrame) {
		enemy[player->target].life -= DamageFunc(player->power, enemy[player->target].diffend);
		player->atFrameCount = 0;
		player->attackUse = TRUE;
		if (atNum == 0)
			PlaySound(player->attackSE + 1);
		atNum++;
	}
}

void KouenStand(PLAYER *player, Playerliner *line, PlayerParts *parts)
{
	if (player->tbl_adrM == NULL)return;	// 線形補間を実行する？

//
// 線形補間の処理
// 移動処理
	int		index = (int)player->move_time;
	float	time = player->move_time - index;
	int		size = player->tbl_sizeM;

	float dt = 1.0f / player->tbl_adrM[index].frame;	// 1フレームで進める時間
	player->move_time += dt;							// アニメーションの合計時間に足す

	if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
	{
		player->move_time = 0.0f;
		index = 0;
		line->pos = { 0.0f, 0.0f, 0.0f };
		line->rot = { 0.0f, 0.0f, 0.0f };
		line->scl = { 0.0f, 0.0f, 0.0f };
	}

	// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
	XMVECTOR p1 = XMLoadFloat3(&player->tbl_adrM[index + 1].pos);	// 次の場所
	XMVECTOR p0 = XMLoadFloat3(&player->tbl_adrM[index + 0].pos);	// 現在の場所
	XMVECTOR vec = p1 - p0;
	XMStoreFloat3(&line->pos, p0 + vec * time);

	// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
	XMVECTOR r1 = XMLoadFloat3(&player->tbl_adrM[index + 1].rot);	// 次の角度
	XMVECTOR r0 = XMLoadFloat3(&player->tbl_adrM[index + 0].rot);	// 現在の角度
	XMVECTOR rot = r1 - r0;
	XMStoreFloat3(&line->rot, r0 + rot * time);

	// scaleを求める S = StartX + (EndX - StartX) * 今の時間
	XMVECTOR s1 = XMLoadFloat3(&player->tbl_adrM[index + 1].scl);	// 次のScale
	XMVECTOR s0 = XMLoadFloat3(&player->tbl_adrM[index + 0].scl);	// 現在のScale
	XMVECTOR scl = s1 - s0;
	XMStoreFloat3(&line->scl, s0 + scl * time);

	for (int k = player->startNum; k < player->startNum + player->partsNum; k++)
	{
		if (parts[k].tbl_adrM == NULL)continue;;	// 線形補間を実行する？

		int		index = (int)parts[k].move_time;
		float	time = parts[k].move_time - index;
		int		size = parts[k].tbl_sizeM;

		float dt = 1.0f / parts[k].tbl_adrM[index].frame;	// 1フレームで進める時間
		parts[k].move_time += dt;							// アニメーションの合計時間に足す

		if (index > (size - 2))	// ゴールをオーバーしていたら、データを最初に戻して攻撃を終了
		{
			parts[k].move_time = 0.0f;
			index = 0;
		}

		// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
		XMVECTOR p1 = XMLoadFloat3(&parts[k].tbl_adrM[index + 1].pos);	// 次の場所
		XMVECTOR p0 = XMLoadFloat3(&parts[k].tbl_adrM[index + 0].pos);	// 現在の場所
		XMVECTOR vec = p1 - p0;
		XMStoreFloat3(&parts[k].pos, p0 + vec * time);

		// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
		XMVECTOR r1 = XMLoadFloat3(&parts[k].tbl_adrM[index + 1].rot);	// 次の角度
		XMVECTOR r0 = XMLoadFloat3(&parts[k].tbl_adrM[index + 0].rot);	// 現在の角度
		XMVECTOR rot = r1 - r0;
		XMStoreFloat3(&parts[k].rot, r0 + rot * time);

		// scaleを求める S = StartX + (EndX - StartX) * 今の時間
		XMVECTOR s1 = XMLoadFloat3(&parts[k].tbl_adrM[index + 1].scl);	// 次のScale
		XMVECTOR s0 = XMLoadFloat3(&parts[k].tbl_adrM[index + 0].scl);	// 現在のScale
		XMVECTOR scl = s1 - s0;
		XMStoreFloat3(&parts[k].scl, s0 + scl * time);
	}
}
