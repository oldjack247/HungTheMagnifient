//=============================================================================
//インクルードファイル
//=============================================================================
#include "item.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "player.h"
#include "life.h"
#include "game.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CItem::m_pTexture[TYPE_MAX] = {};

//=============================================================================
//コンストラクタ
//=============================================================================
CItem::CItem()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
	m_type = TYPE_ITEM_HP;

	m_bHit = false;
}

//=============================================================================
//デストラクタ
//=============================================================================
CItem::~CItem()
{
}

//=============================================================================
//テクスチャ読み込み処理
//=============================================================================
HRESULT CItem::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	if (pRenderer == NULL)
	{
		pRenderer = CManager::GetRenderer();
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/item/item002.png",//ファイルの名前
		&m_pTexture[TYPE_ITEM_HP]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/item/item000.png",//ファイルの名前
		&m_pTexture[TYPE_ITEM_BULLET]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/item/item001.png",//ファイルの名前
		&m_pTexture[TYPE_ITEM_MINION]);

	return E_NOTIMPL;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CItem::Unload(void)
{
	//テクスチャの開放
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
CItem* CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size_x, float size_y, ITEMTYPE type)
{
	CItem* pItem = NULL;
	if (pItem == NULL)
	{
		pItem = new CItem;
		if (pItem != NULL)
		{
			pItem->Init(pos, move, size_x, size_y, type);
		}
	}
	return pItem;
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size_x, float size_y, ITEMTYPE type)
{

	CScene2d::Init(pos, size_x, size_y);

	BindTexture(m_pTexture[type]);
	m_pos = pos;
	m_move = move;
	m_type = type;
	m_nLife = ITEM_LIFE;

	SetObjType(OBJTYPE_ITEM);
	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CItem::Uninit(void)
{
	CScene2d::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CItem::Update(void)
{
	VERTEX_2D* pVtx;
	m_nCounterAnim++;

	if (m_type == TYPE_ITEM_BULLET)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.11f + 0.11f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.11f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.11f + 0.11f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.11f, 1.0f);
		if (m_nCounterAnim >= 5)
		{//一定時間経過した
			m_nCounterAnim = 0;
			m_nPatternAnim++;
			if (m_nPatternAnim >= 9)
			{//パターン数を超えた
				m_nPatternAnim = 0;
			}
		}
		m_pVtxBuff->Unlock();

		for (int nPriority = 0; nPriority < PRIORITYTYPE_MAX; nPriority++)
		{
			for (int nCount = 0; nCount < MAX_SCENE; nCount++)
			{
				CScene* pScene = GetScene(nPriority, nCount);
				if (pScene != NULL)
				{
					CPlayer* pPlayer = (CPlayer*)pScene;
					OBJTYPE objType;

					m_move.y = 2.0f;
					objType = pScene->GetObjType();
					if (objType == CScene::OBJTYPE_PLAYER)
					{
						//当たり判定
						if (HitCcollision(pPlayer->GetPosition(), PLAYER_SIZE, PLAYER_SIZE))
						{
							m_bHit = true;
							CPlayer::SetBulletChange(m_bHit);
							m_nLife = 0;
						}
					}

					if (m_pos.y >= 500)
					{
						//動きを止める
						m_move.y = 0.0f;
					}
				}
			}
		}
	}

	if (m_type == TYPE_ITEM_HP)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.047f + 0.047f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.047f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.047f + 0.047f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.047f, 1.0f);
		if (m_nCounterAnim >= 5)
		{//一定時間経過した
			m_nCounterAnim = 0;
			m_nPatternAnim++;
			if (m_nPatternAnim >= 9)
			{//パターン数を超えた
				m_nPatternAnim = 0;
			}
		}
		m_pVtxBuff->Unlock();

		for (int nPriority = 0; nPriority < PRIORITYTYPE_MAX; nPriority++)
		{
			for (int nCount = 0; nCount < MAX_SCENE; nCount++)
			{
				CScene* pScene = GetScene(nPriority, nCount);
				if (pScene != NULL)
				{
					CPlayer* pPlayer = (CPlayer*)pScene;
					OBJTYPE objType;

					m_move.y = 2.0f;
					objType = pScene->GetObjType();
					if (objType == CScene::OBJTYPE_PLAYER)
					{
						//当たり判定
						if (HitCcollision(pPlayer->GetPosition(), PLAYER_SIZE, PLAYER_SIZE))
						{
							m_bHit = true;
							CGame::SetPlayerLife(+1);
							m_nLife = 0;
						}
					}

					if (m_pos.y >= 500)
					{
						//動きを止める
						m_move.y = 0.0f;
					}
				}
			}
		}
	}


	if (m_type == TYPE_ITEM_MINION)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f + 0.25f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f + 0.25f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f, 1.0f);
		if (m_nCounterAnim >= 5)
		{//一定時間経過した
			m_nCounterAnim = 0;
			m_nPatternAnim++;
			if (m_nPatternAnim >= 4)
			{//パターン数を超えた
				m_nPatternAnim = 0;
			}
		}
		m_pVtxBuff->Unlock();

		for (int nPriority = 0; nPriority < PRIORITYTYPE_MAX; nPriority++)
		{
			for (int nCount = 0; nCount < MAX_SCENE; nCount++)
			{
				CScene* pScene = GetScene(nPriority, nCount);
				if (pScene != NULL)
				{
					CPlayer* pPlayer = (CPlayer*)pScene;
					OBJTYPE objType;

					m_move.y = 2.0f;
					objType = pScene->GetObjType();
					if (objType == CScene::OBJTYPE_PLAYER)
					{
						//当たり判定
						if (HitCcollision(pPlayer->GetPosition(), PLAYER_SIZE, PLAYER_SIZE))
						{
							m_bHit = true;
							CPlayer::SetMinion(m_bHit);
							m_nLife = 0;
						}
					}

					if (m_pos.y >= 500)
					{
						//動きを止める
						m_move.y = 0.0f;
					}
				}
			}
		}
	}

	//位置更新
	m_pos += m_move;
	//位置をセット
	SetPosition(m_pos);

	if (m_nLife <= 0)
	{
		Uninit();
	}
	else
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない

		//頂点座標の設定(右回り)
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - (ITEM_SIZE / 2), m_pos.y - (ITEM_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + (ITEM_SIZE / 2), m_pos.y - (ITEM_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - (ITEM_SIZE / 2), m_pos.y + (ITEM_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + (ITEM_SIZE / 2), m_pos.y + (ITEM_SIZE / 2), 0.0f);

		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CItem::Draw(void)
{
	CScene2d::Draw();
}

//=============================================================================
// ダメージ処理
//=============================================================================
void CItem::SetLife(int nLife)
{
	m_nLife -= nLife;
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CItem::HitCcollision(D3DXVECTOR3 Pos, float size_x, float size_y)
{
	bool bHit = false;
	D3DXVECTOR3 pos;
	pos = GetPosition();
	if (pos.x <= Pos.x + (size_x / 2) &&
		pos.x >= Pos.x - (size_x / 2) &&
		pos.y <= Pos.y + (size_y / 2) &&
		pos.y >= Pos.y - (size_y / 2))//当たり判定
	{
		bHit = true;
	}
	return bHit;
}
