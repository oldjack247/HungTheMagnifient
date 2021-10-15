//=============================================================================
//インクルードファイル
//=============================================================================
#include "score.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CScore::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore(int nPriority) : CScene(nPriority)
{
	memset(m_apScene2D, 0, sizeof(m_apScene2D));
	m_nScore = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CScore::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/number000.png",//ファイルの名前
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// デストラクタ
//=============================================================================
void CScore::Unload(void)
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
CScore* CScore::Create(D3DXVECTOR3 pos, float size_x, float size_y)
{
	CScore* pScore = NULL;
	pScore = new CScore;
	pScore->Init(pos, size_x, size_y);
	return pScore;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, float size_x, float size_y)
{
	int nCntPlace;
	for (nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++)
	{
		m_apScene2D[nCntPlace] = new CScene2d(GetPriority());
		m_apScene2D[nCntPlace]->BindTexture(m_pTexture);

		//デバイスの取得
		CRenderer* pRenderer = NULL;
		pRenderer = CManager::GetRenderer();

		VERTEX_2D* pVtx;

		//オブジェクトの頂点バッファを生成
		pRenderer->GetDevice()->CreateVertexBuffer(
			sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法
			FVF_VERTEX_2D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&m_apScene2D[nCntPlace]->m_pVtxBuff,					// 頂点バッファインターフェースへのポインタ
			NULL);										// NULLに設定

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_apScene2D[nCntPlace]->m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x + nCntPlace * SCORE_INTERVAL_X, pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + nCntPlace * SCORE_INTERVAL_X + size_x, pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x + nCntPlace * SCORE_INTERVAL_X, pos.y + size_y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + nCntPlace * SCORE_INTERVAL_X + size_x, pos.y + size_y, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		// 頂点データをアンロックする
		m_apScene2D[nCntPlace]->m_pVtxBuff->Unlock();
	}


	return S_OK;
}

//=============================================================================
// 終了処理処理
//=============================================================================
void CScore::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScore::Update(void)
{
}

//=============================================================================
// 描画処理処理
//=============================================================================
void CScore::Draw(void)
{

}

//=============================================================================
//スコアの加算処理
//=============================================================================
void CScore::AddScore(int nValue)
{
	VERTEX_2D* pVtx;
	int nCntPlace;
	int nNumber;

	m_nScore += nValue;
	if (m_nScore < 0)
	{
		m_nScore = 0;
	}
	else if (m_nScore >= (int)(powf(10.0f, (float)(NUM_PLACE))))
	{
		m_nScore = (int)(powf(10.0f, (float)(NUM_PLACE))) - 1;
	}

	for (nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++)
	{
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_apScene2D[nCntPlace]->m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// スコア数字の更新
		nNumber = (m_nScore % (int)(powf(10.0f, (float)(NUM_PLACE - nCntPlace)))) / (int)(powf(10.0f, (float)(NUM_PLACE - nCntPlace - 1)));

		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);

		// 頂点データをアンロックする
		m_apScene2D[nCntPlace]->m_pVtxBuff->Unlock();
	}

}

