#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "scene.h"
#include "joystick.h"
#include "billboard.h"
#include "particle.h"
#include "enemydeatheffect.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemydeatheffect::CEnemydeatheffect(int nPriority) : CParticle(nPriority)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemydeatheffect::~CEnemydeatheffect()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CEnemydeatheffect* CEnemydeatheffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXCOLOR col, int Life)
{
	// CEnemydeatheffectポインタ
	CEnemydeatheffect* pSwordeffect = NULL;

	// メモリ確保
	pSwordeffect = new CEnemydeatheffect;

	// 位置座標設定
	pSwordeffect->SetPos(pos);

	// サイズ設定
	pSwordeffect->SetSize(size);

	// カラー設定
	pSwordeffect->SetColor(col);

	// ライフ設定
	pSwordeffect->SetLife(Life);

	// 動く向きの代入
	pSwordeffect->SetMove(move);

	// 初期化
	pSwordeffect->Init();

	// ポインタを返す
	return pSwordeffect;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemydeatheffect::Init()
{
	SetAnimation(ENEMYDEATHEFFEC_COUNTANIM, ENEMYDEATHEFFEC_PATTERNANIM, ENEMYDEATHEFFEC_TIMEANIM);
	CParticle::Init(TEX_TYPE_ENEMYDEATH);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemydeatheffect::Uninit(void)
{
	// 終了
	CParticle::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemydeatheffect::Update(void)
{
	// 更新
	CParticle::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemydeatheffect::Draw(void)
{
	// 加算合成のフラグ
	SetAdditiveSynthesis();

	// Zバッファのフラグ
	SetZbuffer();

	// 描画
	CParticle::Draw();
}