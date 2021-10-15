//=============================================================================
// インクルードファイル
//=============================================================================
#include "life.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"
#include "player.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CLife::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CLife::CLife(int nPriority) : CScene(nPriority)
{
	memset(m_apScene2D, 0, sizeof(m_apScene2D));
}

//=============================================================================
// デストラクタ
//=============================================================================
CLife::~CLife()
{
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CLife::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/player/life000.png",//ファイルの名前
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CLife::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CLife* CLife::Create(D3DXVECTOR3 pos, float size_x, float size_y)
{
	CLife* pLife = NULL;
	pLife = new CLife;
	pLife->Init(pos, size_x, size_y);
	return pLife;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLife::Init(D3DXVECTOR3 pos, float size_x, float size_y)
{
	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
	{
		if (m_apScene2D[nCnt] == NULL)
		{
			m_apScene2D[nCnt] = new CScene2d(GetPriority());
			m_apScene2D[nCnt]->Init(pos, size_x, size_y);
			m_apScene2D[nCnt]->BindTexture(m_pTexture);
		}
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLife::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLife::Update(void)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	int nLife = CPlayer::GetLife();
	int nScene = 2;
	//ポインタの生成
	VERTEX_2D* pVtx;
	for (int nCnt = 0; nCnt < nLife; nCnt++)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_apScene2D[nCnt]->m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
		//頂点座標の設定(右回り)
		pVtx[0].pos = D3DXVECTOR3(0.0f + HP_SIZE_X * (nCnt * 3), 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(50.0f + HP_SIZE_X * (nCnt * 3), 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f + HP_SIZE_X * (nCnt * 3), 50.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(50.0f + HP_SIZE_X * (nCnt * 3), 50.0f, 0.0f);
		pVtx += 4;//頂点データのポインタを4つ分進める
				  //頂点データをアンロックする
		m_apScene2D[nCnt]->m_pVtxBuff->Unlock();
	}
	for (int nCnt = nLife; nCnt < MAX_LIFE; nCnt++)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_apScene2D[nCnt]->m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		//頂点データをアンロックする
		m_apScene2D[nCnt]->m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLife::Draw(void)
{
}
