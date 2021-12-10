#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "scene.h"
#include "joystick.h"
#include "billboard.h"
#include "particle.h"
#include "magiccircle.h"
#include "fire.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMagiccercle::CMagiccercle(int nPriority) : CParticle(nPriority)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CMagiccercle::~CMagiccercle()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CMagiccercle* CMagiccercle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, int Life, MAGICCIRCLETYPE type)
{
	// CMagiccercleポインタ
	CMagiccercle* pMagiccerclet = NULL;

	// メモリ確保
	pMagiccerclet = new CMagiccercle;

	// 位置座標設定
	pMagiccerclet->SetPos(pos);

	// 角度設定
	pMagiccerclet->SetRot(rot);

	// サイズ設定
	pMagiccerclet->SetSize(size);

	// カラー設定
	pMagiccerclet->SetColor(col);

	// カラー設定
	pMagiccerclet->SetLife(Life);

	// 初期化
	pMagiccerclet->Init(type);

	// ポインタを返す
	return pMagiccerclet;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMagiccercle::Init(MAGICCIRCLETYPE type)
{
	switch (type)
	{
	case MAGICCIRCLETYPE_PLAYER:
		CParticle::Init(TEX_TYPE_MAGICCIRCLE);
		break;

	case MAGICCIRCLETYPE_ENEMY:
		SetAnimation(MAGICCERCLE_ENEMY_COUNTANIM, MAGICCERCLE_ENEMY_PATTERNANIM, MAGICCERCLE_ENEMY_TIMEANIM);
		CParticle::Init(TEX_TYPE_ENEMYCREATE_MAGICCIRCLE);
		break;

	default:
		break;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMagiccercle::Uninit(void)
{
	// 終了
	CParticle::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMagiccercle::Update(void)
{
	// 更新
	CParticle::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CMagiccercle::Draw(void)
{
	// 描画
	CParticle::Draw();
}