//=============================================================================
// インクルードファイル
//=============================================================================	
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "joystick.h"
#include "billboard.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CBillboard::CBillboard(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;
	m_pBuffMat = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nLife = 0;
	m_fTexX1 = 0.0f;
	m_fTexX2 = 1.0f;
	m_fTexY1 = 0.0f;
	m_fTexY2 = 1.0f;
	m_Animation = false;
	m_nCountAnim = 0;
	m_nPatternAnim = 0;
	m_nTime = 0;
	m_nCountAnimChange = 0;
	m_nPatternAnimChange = 0;
	m_nTimeChange = 0;
	m_bAdditiveSynthesis = false;
	m_bUseZbuf = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBillboard::~CBillboard()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBillboard::Init(void)
{
	//レンダラー取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	//頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,													//頂点バッファの使用法
		FVF_VERTEX_3D,														//使用する頂点フォーマット
		D3DPOOL_MANAGED,													//リソースのバッファを保持するメモリクラスを指定
		&m_pBuffMat,														//頂点バッファへのポインタ
		NULL)))																//NULLに設定
	{
		return E_FAIL;
	}

	//頂点情報を設定
	VERTEX_3D* pVtx;

	//頂点バッファのロック
	m_pBuffMat->Lock(0, 0, (void**)&pVtx, 0);

	//場所の設定
	pVtx[0].pos = D3DXVECTOR3((-m_size.x / 2), (m_size.y / 2), (m_size.z / 2));
	pVtx[1].pos = D3DXVECTOR3((+m_size.x / 2), (m_size.y / 2), (m_size.z / 2));
	pVtx[2].pos = D3DXVECTOR3((-m_size.x / 2), (-m_size.y / 2), (m_size.z / 2));
	pVtx[3].pos = D3DXVECTOR3((+m_size.x / 2), (-m_size.y / 2), (m_size.z / 2));

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	if (m_Animation == true)
	{
		m_nCountAnimChange++;

		if (m_nCountAnim % m_nCountAnimChange == 0)
		{
			m_nCountAnimChange = 0;
			m_nPatternAnimChange++;
			if (m_nPatternAnim % m_nPatternAnimChange == 0)
			{
				m_nPatternAnimChange = 0;
			}
		}

		// 頂点情報を設定
		pVtx[0].tex = D3DXVECTOR2((float)(1.0f / m_nCountAnimChange) * m_fTexX1, (float)(1.0f / m_nPatternAnim) * m_fTexY1);
		pVtx[1].tex = D3DXVECTOR2((float)(1.0f / m_nCountAnimChange) * m_fTexX2, (float)(1.0f / m_nPatternAnim) * m_fTexY1);
		pVtx[2].tex = D3DXVECTOR2((float)(1.0f / m_nCountAnimChange) * m_fTexX1, (float)(1.0f / m_nPatternAnim) * m_fTexY2);
		pVtx[3].tex = D3DXVECTOR2((float)(1.0f / m_nCountAnimChange) * m_fTexX2, (float)(1.0f / m_nPatternAnim) * m_fTexY2);

		m_nTimeChange = 0;
	}

	else
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(m_fTexX1, m_fTexY1);
		pVtx[1].tex = D3DXVECTOR2(m_fTexX2, m_fTexY1);
		pVtx[2].tex = D3DXVECTOR2(m_fTexX1, m_fTexY2);
		pVtx[3].tex = D3DXVECTOR2(m_fTexX2, m_fTexY2);
	}

	//頂点バッファのアンロック
	m_pBuffMat->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBillboard::Uninit(void)
{
	// 破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBillboard::Update(void)
{
	//頂点情報を設定
	VERTEX_3D* pVtx;

	//頂点バッファのロック
	m_pBuffMat->Lock(0, 0, (void**)&pVtx, 0);

	//場所の設定
	pVtx[0].pos = D3DXVECTOR3((-m_size.x / 2), (m_size.y / 2), (m_size.z / 2));
	pVtx[1].pos = D3DXVECTOR3((+m_size.x / 2), (m_size.y / 2), (m_size.z / 2));
	pVtx[2].pos = D3DXVECTOR3((-m_size.x / 2), (-m_size.y / 2), (m_size.z / 2));
	pVtx[3].pos = D3DXVECTOR3((+m_size.x / 2), (-m_size.y / 2), (m_size.z / 2));

	//カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// アニメーションをかけるとき
	if (m_Animation == true)
	{
		// 設定したタイムと加算したタイムが一緒なら
		if (m_nTime == m_nTimeChange)
		{
			// 横の分割の加算
			m_nCountAnimChange++;

			// 横の分割数と加算した数が一緒なら
			if (m_nCountAnimChange % m_nCountAnim == 0)
			{
				m_nCountAnimChange = 0;

				// 縦の分割の加算
				m_nPatternAnimChange++;

				// 縦の分割数と加算した数が一緒なら
				if (m_nPatternAnimChange % m_nPatternAnim == 0)
				{
					m_nPatternAnimChange = 0;
				}
			}

			// 頂点情報を設定
			pVtx[0].tex = D3DXVECTOR2((float)(1.0f / m_nCountAnim) * m_nCountAnimChange, (float)(1.0f / m_nPatternAnim) * m_nPatternAnimChange);
			pVtx[1].tex = D3DXVECTOR2((float)(1.0f / m_nCountAnim) * (m_nCountAnimChange + 1), (float)(1.0f / m_nPatternAnim) * m_nPatternAnimChange);
			pVtx[2].tex = D3DXVECTOR2((float)(1.0f / m_nCountAnim) * m_nCountAnimChange, (float)(1.0f / m_nPatternAnim) * (m_nPatternAnimChange + 1));
			pVtx[3].tex = D3DXVECTOR2((float)(1.0f / m_nCountAnim) * (m_nCountAnimChange + 1), (float)(1.0f / m_nPatternAnim) * (m_nPatternAnimChange + 1));

			m_nTimeChange = 0;
		}
		// 時間の加算
		m_nTimeChange++;
	}
	else
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(m_fTexX1, m_fTexY1);
		pVtx[1].tex = D3DXVECTOR2(m_fTexX2, m_fTexY1);
		pVtx[2].tex = D3DXVECTOR2(m_fTexX1, m_fTexY2);
		pVtx[3].tex = D3DXVECTOR2(m_fTexX2, m_fTexY2);
	}

	//頂点バッファのアンロック
	m_pBuffMat->Unlock();


	if (m_nLife > 0 && m_col > 0.0f)
	{
		m_nLife--;
	}
	else
	{
		Uninit();
		return;
	}
}

//=============================================================================
// テクスチャ回転させる更新処理
//=============================================================================
void CBillboard::Update(float fangle)
{
	//頂点情報を設定
	VERTEX_3D* pVtx;

	//頂点バッファのロック
	m_pBuffMat->Lock(0, 0, (void**)&pVtx, 0);

	//斜辺の計算
	float fHypotenuse = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y);

	//角度
	float frot = atan2f(m_size.y, m_size.x);


	float cos1 = cosf(frot + fangle) * fHypotenuse; //xが+
	float cos2 = cosf(frot - fangle) * fHypotenuse; //xが-
	float sin1 = sinf(frot + fangle) * fHypotenuse; //yが+
	float sin2 = sinf(frot - fangle) * fHypotenuse; //yが-

	//場所の設定
	pVtx[0].pos = D3DXVECTOR3(-cos1, -sin1, (m_size.z / 2));
	pVtx[1].pos = D3DXVECTOR3(-cos2, +sin2, (m_size.z / 2));
	pVtx[2].pos = D3DXVECTOR3(+cos2, -sin2, (m_size.z / 2));
	pVtx[3].pos = D3DXVECTOR3(+cos1, +sin1, (m_size.z / 2));


	//カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// アニメーションをかけるとき
	if (m_Animation == true)
	{
		// 設定したタイムと加算したタイムが一緒なら
		if (m_nTime == m_nTimeChange)
		{
			// 横の分割の加算
			m_nCountAnimChange++;

			// 横の分割数と加算した数が一緒なら
			if (m_nCountAnimChange % m_nCountAnim == 0)
			{
				m_nCountAnimChange = 0;

				// 縦の分割の加算
				m_nPatternAnimChange++;

				// 縦の分割数と加算した数が一緒なら
				if (m_nPatternAnimChange % m_nPatternAnim == 0)
				{
					m_nPatternAnimChange = 0;
				}
			}

			// 頂点情報を設定
			pVtx[0].tex = D3DXVECTOR2((float)(1.0f / m_nCountAnim) * m_nCountAnimChange, (float)(1.0f / m_nPatternAnim) * m_nPatternAnimChange);
			pVtx[1].tex = D3DXVECTOR2((float)(1.0f / m_nCountAnim) * (m_nCountAnimChange + 1), (float)(1.0f / m_nPatternAnim) * m_nPatternAnimChange);
			pVtx[2].tex = D3DXVECTOR2((float)(1.0f / m_nCountAnim) * m_nCountAnimChange, (float)(1.0f / m_nPatternAnim) * (m_nPatternAnimChange + 1));
			pVtx[3].tex = D3DXVECTOR2((float)(1.0f / m_nCountAnim) * (m_nCountAnimChange + 1), (float)(1.0f / m_nPatternAnim) * (m_nPatternAnimChange + 1));

			m_nTimeChange = 0;
		}
		// 時間の加算
		m_nTimeChange++;
	}
	else
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(m_fTexX1, m_fTexY1);
		pVtx[1].tex = D3DXVECTOR2(m_fTexX2, m_fTexY1);
		pVtx[2].tex = D3DXVECTOR2(m_fTexX1, m_fTexY2);
		pVtx[3].tex = D3DXVECTOR2(m_fTexX2, m_fTexY2);
	}

	// 頂点バッファのアンロック
	m_pBuffMat->Unlock();


	if (m_nLife > 0 && m_col > 0.0f)
	{
		m_nLife--;
	}
	else
	{
		Uninit();
		return;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBillboard::Draw(void)
{
	// レンダラー取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans; // 行列計算用のマトリクス

	//ライト無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zバッファを使うなら
	if (m_bUseZbuf == true)
	{
		// Zバッファを無効化
		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	}

	// 加算合成
	if (m_bAdditiveSynthesis == true)
	{
		// 加算合成する
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// アルファテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	// アルファテスト基準値の設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	// アルファテストの比較方法の設定（GREATERは基準値より大きい場合)
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_rot == D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		// 回転の逆行列
		pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
		D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxRot);
		m_mtxWorld._41 = 0;
		m_mtxWorld._42 = 0;
		m_mtxWorld._43 = 0;

		// 向き反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}
	else
	{
		// 向き反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}


	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pBuffMat, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// セットテクスチャ
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// セットテクスチャ
	pDevice->SetTexture(0, NULL);

	// アルファテスト無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// 加算合成を戻す
	if (m_bAdditiveSynthesis == true)
	{
		// 加算合成を戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// フラグをfalseに戻す
		m_bAdditiveSynthesis = false;
	}

	// Zバッファを使うなら
	if (m_bUseZbuf == true)
	{
		// Zバッファを有効化
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

		m_bUseZbuf = false;
	}

	//ライト有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// テクスチャ受け渡し
//=============================================================================
void CBillboard::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	//代入
	m_pTexture = pTexture;
}