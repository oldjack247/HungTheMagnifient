//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene2d.h"
#include "renderer.h"
#include "main.h"
#include "manager.h"
#include "keyboard.h"
#include "manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CScene2d::CScene2d(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_MoveBg = D3DXVECTOR2(0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene2d::~CScene2d()
{
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CScene2d::Init(D3DXVECTOR3 pos, float size_x, float size_y)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	VERTEX_2D* pVtx;

	m_pos = pos;

	//頂点バッファ生成
	pRenderer->GetDevice()->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//バッファサイズ
		D3DUSAGE_WRITEONLY,//(固定)
		FVF_VERTEX_2D,//(固定)
		D3DPOOL_MANAGED,//(固定)
		&m_pVtxBuff,//変数名が変わると変更が必要
		NULL);//頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない

	//頂点座標の設定(右回り)
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (size_x / 2), m_pos.y - (size_y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (size_x / 2), m_pos.y - (size_y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (size_x / 2), m_pos.y + (size_y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (size_x / 2), m_pos.y + (size_y / 2), 0.0f);

	//テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定(0～255の数値で設定)
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CScene2d::Uninit(void)
{
	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CScene2d::Update(void)
{
}

//=============================================================================
//描画処理
//=============================================================================
void CScene2d::Draw(void)
{
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{
		//頂点バッファをデバイスのストリームにバインド
		pRenderer->GetDevice()->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pRenderer->GetDevice()->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pRenderer->GetDevice()->SetTexture(0, m_pTexture);
		//ポリゴンの描画
		pRenderer->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
//posのセット
//=============================================================================
void CScene2d::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
//位置取得用
//=============================================================================
D3DXVECTOR3 CScene2d::GetPosition(void)
{
	return m_pos;
}

//=============================================================================
//色のセット
//=============================================================================
void CScene2d::SetCol(D3DXCOLOR col)
{
	m_col = col;
}

//=============================================================================
// 色の取得用
//=============================================================================
D3DXCOLOR CScene2d::GetCol(void)
{
	return m_col;
}

//=============================================================================
// テクスチャのセット
//=============================================================================
void CScene2d::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

void CScene2d::MoveBg(D3DXVECTOR2 move, CScene2d::BGOPRETION bg)
{
	//頂点情報へのポインタ
	VERTEX_2D* pVtx;

	//頂点データの範囲をロックし頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (bg)
	{
	case BG_MOVE:

		m_MoveBg += move;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f) + m_MoveBg;
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f) + m_MoveBg;
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f) + m_MoveBg;
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f) + m_MoveBg;

		break;

	case BG_INPUT:

		break;

	default:
		break;
	}

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}
