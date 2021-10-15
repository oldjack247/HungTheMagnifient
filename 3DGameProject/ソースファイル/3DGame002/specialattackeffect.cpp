#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "billboard.h"
#include "particle.h"
#include "specialattackeffect.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CSpecialattackeffect::CSpecialattackeffect()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CSpecialattackeffect::~CSpecialattackeffect()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CSpecialattackeffect* CSpecialattackeffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int Life)
{
	// CSpesialattackeffectクラスのポインタ
	CSpecialattackeffect* pSpecialattackeffect;

	// メモリ確保
	pSpecialattackeffect = new CSpecialattackeffect;

	// 位置座標設定
	pSpecialattackeffect->SetPos(pos);

	// サイズ設定
	pSpecialattackeffect->SetSize(size);

	// カラー設定
	pSpecialattackeffect->SetColor(col);

	// カラー設定
	pSpecialattackeffect->SetLife(Life);

	// アニメーションの設定
	pSpecialattackeffect->SetAnimation(SPECIALATTACKEFFECT_COUNTANIM, SPECIALATTACKEFFECT_PATTERNANIM, SPECIALATTACKEFFECT_TIMEANIM);

	// 初期化
	pSpecialattackeffect->Init();

	// CSparkクラスのポインタを返す
	return pSpecialattackeffect;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSpecialattackeffect::Init()
{
	// CParticleクラスのInit
	CParticle::Init(TEX_TYPE_SPESIALATTACK);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSpecialattackeffect::Uninit(void)
{
	// 終了
	CParticle::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSpecialattackeffect::Update(void)
{
	// 更新
	CParticle::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CSpecialattackeffect::Draw(void)
{
	// 加算合成のフラグ
	SetAdditiveSynthesis();

	// 描画
	CParticle::Draw();
}