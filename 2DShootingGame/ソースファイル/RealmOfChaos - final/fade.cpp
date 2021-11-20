//=============================================================================
//インクルードファイル
//=============================================================================
#include "fade.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
CFade::FADE CFade::m_Fade = FADE_NONE;
CManager::MODE CFade::m_ModeNext = CManager::MODE_TITLE;
D3DXCOLOR CFade::m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
bool CFade::m_bFadeSwitch = false;

//=============================================================================
//コンストラクタ
//=============================================================================
CFade::CFade()
{
	m_pVtxBuff = NULL;
}

//=============================================================================
//デストラクタ
//=============================================================================
CFade::~CFade()
{
}

//=============================================================================
//生成処理
//=============================================================================
CFade* CFade::Create(D3DXVECTOR3 pos, float size_x, float size_y, CManager::MODE modenext)
{
	CFade* pFade = NULL;
	if (pFade == NULL)
	{
		pFade = new CFade;
		if (pFade != NULL)
		{
			pFade->Init(pos, size_x, size_y, modenext);
		}
	}
	return pFade;
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CFade::Init(D3DXVECTOR3 pos, float size_x, float size_y, CManager::MODE modenext)
{
	m_ModeNext = modenext;
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//頂点情報のポインタ
	VERTEX_2D* pVtx;

	//オブジェクトの頂点バッファ生成
	pRenderer->GetDevice()->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//バッファサイズ
		D3DUSAGE_WRITEONLY,//(固定）
		FVF_VERTEX_2D,//頂点フォーマット
		D3DPOOL_MANAGED,//（固定）
		&m_pVtxBuff,//変数名が変わると変更が必要
		NULL);
	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（右回りで設定する）
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(size_x, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, size_y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size_x, size_y, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データアンロック
	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
//終了処理処理
//=============================================================================
void CFade::Uninit(void)
{
	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//更新処理処理
//=============================================================================
void CFade::Update(void)
{
	VERTEX_2D* pVtx;//頂点情報のポインタ
	if (m_bFadeSwitch == true)
	{
		if (m_Fade != FADE_NONE)
		{//フェード処理中の時
			if (m_Fade == FADE_IN)
			{//フェードイン
				m_col.a -= FADE_RATE;
				if (m_col.a <= 0.0f)
				{
					m_col.a = 0.0f;
					m_Fade = FADE_NONE;
					if (m_col.a >= 0)
					{
						m_bFadeSwitch = false;
					}
				}
			}
			if (m_Fade == FADE_OUT)
			{//フェード
				m_col.a += FADE_RATE;
				if (m_col.a >= 1.0f)
				{
					m_col.a = 1.0f;
					m_Fade = FADE_IN;

					CManager::SetMode(m_ModeNext);//モードの設定
				}
			}
		}
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		//頂点情報(カラーのみ更新)
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;
		//頂点データアンロック
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CFade::Draw(void)
{
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{
		pRenderer->GetDevice()->SetTexture(0, NULL);
		//頂点バッファをデバイスのストリームにバインド
		pRenderer->GetDevice()->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pRenderer->GetDevice()->SetFVF(FVF_VERTEX_2D);

		//ポリゴンの描画
		pRenderer->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
//フェードのセット
//=============================================================================
void CFade::SetFade(CManager::MODE modenext)
{
	m_Fade = FADE_OUT;
	m_ModeNext = modenext;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
//セット
//=============================================================================
void CFade::SetFadeSwitch(bool bFadeSwitch)
{
	m_bFadeSwitch = bFadeSwitch;
}
