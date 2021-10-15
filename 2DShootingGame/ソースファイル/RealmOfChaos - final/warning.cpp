//=============================================================================
//インクルードファイル
//=============================================================================
#include "warning.h"
#include "game.h"
#include "renderer.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CWarning::m_pTexture[WARNINGTYPE_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CWarning::CWarning(int nPriority) : CScene2d(nPriority)
{
	for (int nCount = 0; nCount < WARNINGTYPE_MAX; nCount++)
	{
		m_apScene2D[nCount] = NULL;
	}
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_type = WARNINGTYPE_NONE;
	m_pSound = NULL;
	nStopCount = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CWarning::~CWarning()
{

}

//=============================================================================
// テクスチャの描画処理
//=============================================================================
HRESULT CWarning::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/warning/warning001.png",//ファイルの名前
		&m_pTexture[WARNINGTYPE_BOSS1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/warning/warning002.png",//ファイルの名前
		&m_pTexture[WARNINGTYPE_BOSS2]);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CWarning::Unload(void)
{
	for (int nCount = 0; nCount < WARNINGTYPE_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CWarning* CWarning::Create(D3DXVECTOR3 pos, float size_x, float size_y, WARNINGTYPE type)
{
	CWarning* pWarning = NULL;
	pWarning = new CWarning;
	pWarning->Init(pos, size_x * 2, size_y * 2, type);
	return pWarning;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CWarning::Init(D3DXVECTOR3 pos, float size_x, float size_y, WARNINGTYPE type)
{
	CScene2d::Init(pos, size_x, size_y);

	BindTexture(m_pTexture[type]);
	m_pos = pos;
	m_size_x = size_x;
	m_size_y = size_y;
	m_type = type;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CWarning::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CWarning::Update(void)
{
	VERTEX_2D* pVtx;
	nStopCount++;

	if (m_type == WARNINGTYPE_BOSS1)
	{
		if (m_col.a >= 0.0f)
		{
			m_col.a -= 0.01f;
		}
		else if (m_col.a <= 0.0f)
		{
			m_col.a = 1.0f;
		}
	}

	if (m_type == WARNINGTYPE_BOSS2)
	{
		if (m_col.a >= 0.0f)
		{
			m_col.a -= 0.01f;
		}
		else if (m_col.a <= 0.0f)
		{
			m_col.a = 1.0f;
		}
	}

	if (nStopCount >= 100 && m_type == WARNINGTYPE_BOSS1)
	{
		Uninit();
	}
	else if (nStopCount >= 100 && m_type == WARNINGTYPE_BOSS2)
	{
		Uninit();
	}

	else if (m_type == WARNINGTYPE_BOSS1)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない

		//頂点カラーの設定(0～255の数値で設定)
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		m_pVtxBuff->Unlock();
	}

	else if (m_type == WARNINGTYPE_BOSS2)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない

		//頂点カラーの設定(0～255の数値で設定)
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CWarning::Draw(void)
{
	CRenderer* pRenderer = NULL;
	//NILLだったら
	if (pRenderer == NULL)
	{
		pRenderer = CManager::GetRenderer();
	}

	// 加算合成を行う
	pRenderer->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//ディスティネーションカラーの指定
	pRenderer->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	CScene2d::Draw();
}
