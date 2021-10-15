//=============================================================================
//インクルードファイル
//=============================================================================
#include "bg.h"
#include "scene2d.h"
#include "renderer.h"
#include "main.h"
#include "manager.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBg::m_pTexture[MAX_BG] = {};

//=============================================================================
//コンストラクタ
//=============================================================================
CBg::CBg(int nPriority) : CScene(nPriority)
{
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		m_apScene2D[nCnt] = NULL;
	}
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//=============================================================================
//デストラクタ
//=============================================================================
CBg::~CBg()
{

}

//=============================================================================
//テクスチャの読み込み処理
//=============================================================================
HRESULT CBg::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	if (pRenderer == NULL)
	{
		pRenderer = CManager::GetRenderer();
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/Texture/BG/bg102.png", &m_pTexture[CBg::BG_SPACE_1]);

	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/Texture/BG/bg101.png", &m_pTexture[CBg::BG_SPACE_2]);

	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/Texture/BG/bg102.png", &m_pTexture[CBg::BG_SPACE_3]);

	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/Texture/BG/bg102.png", &m_pTexture[CBg::BG_SUN]);

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CBg::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
//生成処理
//=============================================================================
CBg* CBg::Create(D3DXVECTOR3 pos, int nPriority, BG_MOVETYPE MoveType)
{
	//変数宣言
	CBg* pBg;

	//メモリ生成
	pBg = new CBg(nPriority);

	//初期化処理
	pBg->Init(pos, SCREEN_WIDTH, SCREEN_HEIGHT);

	pBg->SetObjType(OBJTYPE_BG);

	pBg->SetBgType(MoveType);

	return pBg;
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CBg::Init(D3DXVECTOR3 pos, float size_x, float size_y)
{
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		//ポリゴンの生成
		m_apScene2D[nCnt] = new CScene2d(PRIORITYTYPE_BG());
		m_apScene2D[nCnt]->Init(pos, size_x, size_y);
		m_apScene2D[nCnt]->BindTexture(m_pTexture[nCnt]);
		m_apScene2D[nCnt]->SetObjType(OBJTYPE_BG);
	}

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CBg::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = NULL;
		}
	}

	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CBg::Update(void)
{
	VERTEX_2D* pVtx;

	switch (m_BgType)
	{
	case BG_MOVE:
		//入れたい引数、速さ、向き
		m_apScene2D[CBg::BG_SPACE_1]->MoveBg(D3DXVECTOR2(0.0f, -0.005f), CScene2d::BG_MOVE);
		m_apScene2D[CBg::BG_SPACE_2]->MoveBg(D3DXVECTOR2(0.0f, -0.003f), CScene2d::BG_MOVE);
		m_apScene2D[CBg::BG_SPACE_3]->MoveBg(D3DXVECTOR2(0.0f, -0.001f), CScene2d::BG_MOVE);

		break;
	default:

		break;
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CBg::Draw(void)
{
}
