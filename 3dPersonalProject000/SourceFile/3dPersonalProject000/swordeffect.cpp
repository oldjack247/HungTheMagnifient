#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "scene.h"
#include "joystick.h"
#include "billboard.h"
#include "particle.h"
#include "swordeffect.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CSwordeffect::CSwordeffect(int nPriority) : CParticle(nPriority)
{
	m_MoveAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CSwordeffect::~CSwordeffect()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CSwordeffect* CSwordeffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXCOLOR col, int Life)
{
	// CSwordeffectポインタ
	CSwordeffect* pSwordeffect = NULL;

	// メモリ確保
	pSwordeffect = new CSwordeffect;

	// 位置座標設定
	pSwordeffect->SetPos(pos);

	// サイズ設定
	pSwordeffect->SetSize(size);

	// カラー設定
	pSwordeffect->SetColor(col);

	// カラー設定
	pSwordeffect->SetLife(Life);

	// 移動量設定
	pSwordeffect->SetMove(move);

	// 初期化
	pSwordeffect->Init();

	// ポインタを返す
	return pSwordeffect;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSwordeffect::Init()
{
	CParticle::Init(TEX_TYPE_FIRE);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSwordeffect::Uninit(void)
{
	// 終了
	CParticle::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSwordeffect::Update(void)
{
	// 更新
	CParticle::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CSwordeffect::Draw(void)
{
	// 描画
	CParticle::Draw();
}