#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "scene.h"
#include "joystick.h"
#include "billboard.h"
#include "particle.h"
#include "fire.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CFire::CFire(int nPriority) : CParticle(nPriority)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CFire::~CFire()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CFire* CFire::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXCOLOR col, int Life)
{
	// CFireポインタ
	CFire* pSwordeffect = NULL;

	// メモリ確保
	pSwordeffect = new CFire;

	// 位置座標設定
	pSwordeffect->SetPos(pos);

	// サイズ設定
	pSwordeffect->SetSize(size);

	// カラー設定
	pSwordeffect->SetColor(col);

	// カラー設定
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
HRESULT CFire::Init()
{
	CParticle::Init(TEX_TYPE_FIRE);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFire::Uninit(void)
{
	// 終了
	CParticle::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CFire::Update(void)
{
	// 更新
	CParticle::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CFire::Draw(void)
{
	// 加算合成のフラグ
	SetAdditiveSynthesis();

	// Zバッファのフラグ
	SetZbuffer();

	// 描画
	CParticle::Draw();
}