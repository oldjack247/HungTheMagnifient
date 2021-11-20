//=============================================================================
//インクルードファイル
//=============================================================================
#include "explosion.h"
#include "scene2d.h"
#include "renderer.h"
#include "main.h"
#include "manager.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture[TYPE_MAX] = {};

//=============================================================================
//コンストラクタ
//=============================================================================
CExplosion::CExplosion(int nPriority) : CScene2d(nPriority)
{
	m_nCounterAnim = NULL;
	m_nPatternAnim = NULL;
	m_type = TYPE_SIMPLE;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_size_x = 0.0f;
	m_size_y = 0.0f;
}

//=============================================================================
//デストラクタ
//=============================================================================
CExplosion::~CExplosion()
{
}

//=============================================================================
//テクスチャの読み込み処理
//=============================================================================
HRESULT CExplosion::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/Explosion/explosion000.png",//ファイルの名前
		&m_pTexture[TYPE_SIMPLE]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/Explosion/bossFog.png",//ファイルの名前
		&m_pTexture[TYPE_ENEMY2_DEATH]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/Explosion/playerExplosion.png",//ファイルの名前
		&m_pTexture[TYPE_PLAYER_DEATH]);

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CExplosion::Unload(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		//テクスチャの開放
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
//生成処理
//=============================================================================
CExplosion* CExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float size_x, float size_y, EXPLOSIONTYPE type)
{
	CExplosion* pExplosion = NULL;
	if (pExplosion == NULL)
	{
		pExplosion = new CExplosion;
		if (pExplosion != NULL)
		{
			pExplosion->Init(pos, move, col, size_x, size_y, type);
		}
	}

	return pExplosion;
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float size_x, float size_y, EXPLOSIONTYPE type)
{
	CScene2d::Init(pos, size_x, size_y);


	BindTexture(m_pTexture[type]);

	m_pos = pos;
	m_move = move;
	m_col = col;
	m_size_x = size_x;
	m_size_y = size_y;
	m_type = type;
	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CExplosion::Uninit(void)
{
	CScene2d::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CExplosion::Update(void)
{
	VERTEX_2D* pVtx;

	m_nCounterAnim++;
	//爆発のカウンターを進めてパターンを切り替える
	if (m_nCounterAnim >= 1 && m_type == TYPE_SIMPLE)
	{//一定時間経過した
		m_nCounterAnim = 0;
		m_nPatternAnim++;
	}

	if (m_type == TYPE_ENEMY2_DEATH)
	{
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.1f + 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.1f + 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.1f, 1.0f);
		if (m_nCounterAnim >= 2)
		{//一定時間経過した
			m_nCounterAnim = 0;
			m_nPatternAnim++;
			if (m_nPatternAnim >= 10)
			{//パターン数を超えた
				m_nPatternAnim = 0;
			}
		}
		m_pVtxBuff->Unlock();
	}

	if (m_type == TYPE_PLAYER_DEATH)
	{
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.09f + 0.09f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.09f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.09f + 0.09f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.09f, 1.0f);
		if (m_nCounterAnim >= 3)
		{//一定時間経過した
			m_nCounterAnim = 0;
			m_nPatternAnim++;
			if (m_nPatternAnim >= 11)
			{//パターン数を超えた
				m_nPatternAnim = 0;
			}
		}
		m_pVtxBuff->Unlock();
	}

	//位置更新
	m_pos += m_move;
	//位置をセット
	SetPosition(m_pos);
	//色をセット
	SetCol(m_col);

	if (m_nPatternAnim >= 8 && m_type == TYPE_SIMPLE)
	{
		Uninit();
	}
	if (m_nPatternAnim >= 10 && m_type == TYPE_PLAYER_DEATH)
	{
		Uninit();
	}
	else if (m_col <= 0)
	{
		Uninit();
	}
	else if (m_type == TYPE_SIMPLE)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない

		//頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.125f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.125f + 0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.125f + 0.125f, 1.0f);

		m_pVtxBuff->Unlock();
	}
	else if (m_type == TYPE_ENEMY2_DEATH)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない

		//頂点座標の設定(右回り)
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - ((EXPLOSION_SIZE + m_size_x) / 2), m_pos.y - ((EXPLOSION_SIZE + m_size_y) / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + ((EXPLOSION_SIZE + m_size_x) / 2), m_pos.y - ((EXPLOSION_SIZE + m_size_y) / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - ((EXPLOSION_SIZE + m_size_x) / 2), m_pos.y + ((EXPLOSION_SIZE + m_size_y) / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + ((EXPLOSION_SIZE + m_size_x) / 2), m_pos.y + ((EXPLOSION_SIZE + m_size_y) / 2), 0.0f);

		//頂点カラーの設定(0～255の数値で設定)
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		m_pVtxBuff->Unlock();
	}

	else if (m_type == TYPE_PLAYER_DEATH)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない

		//頂点座標の設定(右回り)
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - ((PLAYER_EXPLOSION_SIZE + m_size_x) / 2), m_pos.y - ((PLAYER_EXPLOSION_SIZE + m_size_y) / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + ((PLAYER_EXPLOSION_SIZE + m_size_x) / 2), m_pos.y - ((PLAYER_EXPLOSION_SIZE + m_size_y) / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - ((PLAYER_EXPLOSION_SIZE + m_size_x) / 2), m_pos.y + ((PLAYER_EXPLOSION_SIZE + m_size_y) / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + ((PLAYER_EXPLOSION_SIZE + m_size_x) / 2), m_pos.y + ((PLAYER_EXPLOSION_SIZE + m_size_y) / 2), 0.0f);

		//頂点カラーの設定(0～255の数値で設定)
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CExplosion::Draw(void)
{
	CScene2d::Draw();
}


