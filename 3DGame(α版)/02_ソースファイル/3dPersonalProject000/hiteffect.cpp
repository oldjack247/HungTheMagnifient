#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "billboard.h"
#include "particle.h"
#include "hiteffect.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CHiteffect::CHiteffect()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CHiteffect::~CHiteffect()
{
}
//=============================================================================
// 生成処理
//=============================================================================
CHiteffect* CHiteffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int Life, HITEFFECTTYPE type)
{
	// CHiteffectクラスのポインタ
	CHiteffect* pHiteffect;

	// メモリ確保
	pHiteffect = new CHiteffect;

	// 位置座標設定
	pHiteffect->SetPos(pos);

	// サイズ設定
	pHiteffect->SetSize(size);

	// カラー設定
	pHiteffect->SetColor(col);

	// カラー設定
	pHiteffect->SetLife(Life);

	// 初期化
	pHiteffect->Init(type);

	// CHiteffectクラスのポインタを返す
	return pHiteffect;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CHiteffect::Init(HITEFFECTTYPE type)
{
	switch (type)
	{
	case HITEFFECTTYPE_PLAYER:
		// アニメーションの設定
		SetAnimation(HITEFFECT_PLAYER_COUNTANIM, HITEFFECT_PLAYER_PATTERNANIM, HITEFFECT_PLAYER_TIMEANIM);

		// CParticleクラスのInit
		CParticle::Init(TEX_TYPE_PLAYERATTACK);
		break;

	case HITEFFECTTYPE_ENEMY:
		// アニメーションの設定
		SetAnimation(HITEFFECT_ENEMY_COUNTANIM, HITEFFECT_ENEMY_PATTERNANIM, HITEFFECT_ENEMY_TIMEANIM);

		// CParticleクラスのInit
		CParticle::Init(TEX_TYPE_ENEMYATTACK);
		break;

	default:
		break;
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CHiteffect::Uninit(void)
{
	// 終了
	CParticle::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CHiteffect::Update(void)
{
	// 更新
	CParticle::Update();

}
//=============================================================================
// 描画処理
//=============================================================================
void CHiteffect::Draw(void)
{
	// 描画
	CParticle::Draw();
}