//=============================================================================
//インクルードファイル
//=============================================================================
#include "bullet.h"
#include "scene2d.h"
#include "renderer.h"
#include "main.h"
#include "manager.h"
#include "enemy.h"
#include "explosion.h"
#include "player.h"
#include "game.h"
#include "effect.h"
#include "sound.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture[BULLETTYPE_MAX] = {};

//=============================================================================
//コンストラクタ
//=============================================================================
CBullet::CBullet(int nPriority) : CScene2d(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
	m_type = BULLETTYPE_PLAYER;
	m_r = 0.0f;
	m_fAngle = 0.0f;
	m_fAngle2 = 0.0f;
	m_pSound = NULL;
}

//=============================================================================
//デストラクタ
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
//テクスチャの読み込み処理
//=============================================================================
HRESULT CBullet::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = NULL;
	if (pRenderer == NULL)
	{
		pRenderer = CManager::GetRenderer();
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/Bullet/bullet000.png",//ファイルの名前
		&m_pTexture[BULLETTYPE_PLAYER]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/Bullet/bullet001.png",//ファイルの名前
		&m_pTexture[BULLETTYPE_PLAYER_2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/Bullet/bullet002.png",//ファイルの名前
		&m_pTexture[BULLETTYPE_PLAYER_3]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/Bullet/enemy_bullet002.png",//ファイルの名前
		&m_pTexture[BULLETTYPE_ENEMY]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/Bullet/enemy_bullet000.png",//ファイルの名前
		&m_pTexture[BULLETTYPE_1FBOSS]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/Bullet/enemy_bullet003.png",//ファイルの名前
		&m_pTexture[BULLETTYPE_2FBOSS]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pRenderer->GetDevice(),
		"data/TEXTURE/Bullet/flame000.png",//ファイルの名前
		&m_pTexture[BULLETTYPE_2FBOSS2]);

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CBullet::Unload(void)
{
	for (int nCount = 0; nCount < BULLETTYPE_MAX; nCount++)
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
CBullet* CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size_x, float size_y, int nLife, BULLETTYPE type)
{
	CBullet* pBullet = NULL;
	if (pBullet == NULL)
	{
		pBullet = new CBullet;
		if (pBullet != NULL)
		{
			pBullet->Init(pos, move, size_x, size_y, nLife, type);
		}
	}

	return pBullet;
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size_x, float size_y, int nLife, BULLETTYPE type)
{
	CScene2d::Init(pos, size_x, size_y);

	BindTexture(m_pTexture[type]);
	m_pos = pos;
	m_type = type;
	m_move = move;
	m_nLife = nLife;

	//回転処理
	m_r = sqrtf(100 / 2 * 100 / 2 + 100 / 2 * 100 / 2);
	m_fAngle = atan2f((100 / 2) + (100 / 2), (100 / 2) + (100 / 2));
	m_fAngle2 = atan2f((100 / 2) + (100 / 2), (100 / 2) + (100 / 2));
	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	CScene2d::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void CBullet::Update(void)
{
	VERTEX_2D* pVtx;

	m_fAngle -= ANGLE;
	m_fAngle2 += ANGLE2;

	m_r = RADIUS;

	//位置を取得
	m_pos = GetPosition();

	m_nCounterAnim++;

	for (int nPriority = 0; nPriority < PRIORITYTYPE_MAX; nPriority++)
	{
		for (int nCount = 0; nCount < MAX_SCENE; nCount++)
		{
			CScene* pScene = GetScene(nPriority, nCount);
			if (pScene != NULL)
			{
				OBJTYPE objType;
				objType = pScene->GetObjType();
				if (m_type == BULLETTYPE_1FBOSS ||
					m_type == BULLETTYPE_2FBOSS3)
				{
					CPlayer* pPlayer = (CPlayer*)pScene;
					if (objType == CScene::OBJTYPE_PLAYER)
					{
						//当たり判定
						if (HitCcollision(pPlayer->GetPosition(), PLAYER_SIZE, PLAYER_SIZE))
						{
							CGame::SetPlayerLife(1);
						}
					}
				}

				if (m_type == BULLETTYPE_ENEMY)
				{
					CPlayer* pPlayer = (CPlayer*)pScene;

					m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
					pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f + 0.25f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f + 0.25f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f, 1.0f);
					if (m_nCounterAnim >= 3)
					{//一定時間経過した
						m_nCounterAnim = 0;
						m_nPatternAnim++;
						if (m_nPatternAnim >= 4)
						{//パターン数を超えた
							m_nPatternAnim = 0;
						}
					}

					if (objType == CScene::OBJTYPE_PLAYER)
					{
						//当たり判定
						if (HitCcollision(pPlayer->GetPosition(), PLAYER_SIZE, PLAYER_SIZE))
						{
							CGame::SetPlayerLife(1);
						}
					}
				}

				if (m_type == BULLETTYPE_2FBOSS2)
				{
					CPlayer* pPlayer = (CPlayer*)pScene;

					m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
					pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.2f + 0.2f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.2f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.2f + 0.2f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.2f, 1.0f);
					if (m_nCounterAnim >= 3)
					{//一定時間経過した
						m_nCounterAnim = 0;
						m_nPatternAnim++;
						if (m_nPatternAnim >= 5)
						{//パターン数を超えた
							m_nPatternAnim = 0;
						}
					}

					if (objType == CScene::OBJTYPE_PLAYER)
					{
						//当たり判定
						if (HitCcollision(pPlayer->GetPosition(), PLAYER_SIZE, PLAYER_SIZE))
						{
							CGame::SetPlayerLife(1);
						}
					}
				}

				if (m_type == BULLETTYPE_2FBOSS)
				{
					CPlayer* pPlayer = (CPlayer*)pScene;

					m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
					pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.333f + 0.333f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.333f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.333f + 0.333f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.333f, 1.0f);
					if (m_nCounterAnim >= 3)
					{//一定時間経過した
						m_nCounterAnim = 0;
						m_nPatternAnim++;
						if (m_nPatternAnim >= 9)
						{//パターン数を超えた
							m_nPatternAnim = 0;
						}
					}

					if (objType == CScene::OBJTYPE_PLAYER)
					{
						//当たり判定
						if (HitCcollision(pPlayer->GetPosition(), PLAYER_SIZE, PLAYER_SIZE))
						{
							CGame::SetPlayerLife(1);
						}
					}
				}

				//プレイヤーだったら
				if (m_type == BULLETTYPE_PLAYER)
				{
					//キャスト
					CEnemy* pEnemy = (CEnemy*)pScene;
					//敵だったら
					m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
					pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.125f + 0.125f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.125f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.125f + 0.125f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.125f, 1.0f);
					if (m_nCounterAnim >= 3)
					{//一定時間経過した
						m_nCounterAnim = 0;
						m_nPatternAnim++;
						if (m_nPatternAnim >= 9)
						{//パターン数を超えた
							m_nPatternAnim = 0;
						}
					}

					/*	CBulletEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),
							BULLET_EFFECT_SIZE_X, BULLET_EFFECT_SIZE_Y);*/

					if (objType == CScene::OBJTYPE_ENEMY)
					{
						//当たり判定
						if (HitCcollision(pEnemy->GetPosition(), ENEMY_SIZE, ENEMY_SIZE))
						{
							//敵に1ダメージ
							pEnemy->SetLife(1);
							CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
								0.0f, 0.0f,
								CEffect::TYPE_BULLET_DEATH);
							//弾のライフを0にする
							m_nLife = 0;
							m_pSound->PlaySound((m_pSound = CManager::GetSound())->SOUND_LABEL_BULLET_HIT);
						}
					}
				}
				if (m_type == BULLETTYPE_PLAYER_2)
				{
					//キャスト
					CEnemy* pEnemy = (CEnemy*)pScene;
					//敵だったら

					m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
					pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f + 0.25f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f + 0.25f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f, 1.0f);
					if (m_nCounterAnim >= 3)
					{//一定時間経過した
						m_nCounterAnim = 0;
						m_nPatternAnim++;
						if (m_nPatternAnim >= 3)
						{//パターン数を超えた
							m_nPatternAnim = 0;
						}
					}

					if (objType == CScene::OBJTYPE_ENEMY)
					{
						//当たり判定
						if (HitCcollision(pEnemy->GetPosition(), ENEMY_SIZE, ENEMY_SIZE))
						{
							//敵に1ダメージ
							pEnemy->SetLife(1);
							CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
								0.0f, 0.0f,
								CEffect::TYPE_BULLET_DEATH);
							//弾のライフを0にする
							m_nLife = 0;
							m_pSound->PlaySound((m_pSound = CManager::GetSound())->SOUND_LABEL_BULLET_HIT);
						}
					}
				}

				if (m_type == BULLETTYPE_PLAYER_3)
				{
					//キャスト
					CEnemy* pEnemy = (CEnemy*)pScene;
					//敵だったら

					m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
					pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.083f + 0.083f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.083f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.083f + 0.083f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.083f, 1.0f);
					if (m_nCounterAnim >= 3)
					{//一定時間経過した
						m_nCounterAnim = 0;
						m_nPatternAnim++;
						if (m_nPatternAnim >= 11)
						{//パターン数を超えた
							m_nPatternAnim = 0;
						}
					}

					if (objType == CScene::OBJTYPE_ENEMY)
					{
						//当たり判定
						if (HitCcollision(pEnemy->GetPosition(), ENEMY_SIZE, ENEMY_SIZE))
						{
							//敵に1ダメージ
							pEnemy->SetLife(1);
							CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
								0.0f, 0.0f,
								CEffect::TYPE_BULLET_DEATH);
							//弾のライフを0にする
							m_nLife = 0;
						}
					}
				}
			}
		}
	}

	//ボスだったら
	if (m_type == BULLETTYPE_1FBOSS)
	{
		D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		SetCol(col);

		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
		pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f + 0.25f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f + 0.25f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.25f, 1.0f);
		if (m_nCounterAnim >= 3)
		{//一定時間経過した
			m_nCounterAnim = 0;
			m_nPatternAnim++;
			if (m_nPatternAnim >= 4)
			{//パターン数を超えた
				m_nPatternAnim = 0;
			}
		}
	}

	//ライフを減らす
	m_nLife--;
	//位置更新
	m_pos += m_move;
	//位置をセット
	SetPosition(m_pos);

	if (m_nLife <= 0 && m_type == BULLETTYPE_2FBOSS)
	{
		for (int nCount = 0; nCount < 16; nCount++)
		{
			//円形に飛ばす
			CBullet::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f),
				D3DXVECTOR3(3.0f * cosf((float)nCount) * 2.0f, 3.0f * sinf((float)nCount) * 2.0f, 0.0f),
				BULLET_SIZE, BULLET_SIZE,
				100,
				CBullet::BULLETTYPE_2FBOSS2);
		}
	}

	if (m_nLife <= 0 || m_pos.y <= 0.0f || m_pos.y >= 720.0f || m_pos.x <= 0.0f || m_pos.y >= 1280.0f && m_type != BULLETTYPE_2FBOSS)
	{
		Uninit();
	}
	else if (m_type == BULLETTYPE_PLAYER)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない

		//頂点座標の設定(右回り)
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - (PLAYER_BULLET_SIZE), m_pos.y - (PLAYER_BULLET_SIZE), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + (PLAYER_BULLET_SIZE), m_pos.y - (PLAYER_BULLET_SIZE), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - (PLAYER_BULLET_SIZE), m_pos.y + (PLAYER_BULLET_SIZE), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + (PLAYER_BULLET_SIZE), m_pos.y + (PLAYER_BULLET_SIZE), 0.0f);

		m_pVtxBuff->Unlock();
	}
	else if (m_type == BULLETTYPE_PLAYER_2)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない

		//頂点座標の設定(右回り)
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - (PLAYER_BULLET_2_SIZE), m_pos.y - (PLAYER_BULLET_2_SIZE), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + (PLAYER_BULLET_2_SIZE), m_pos.y - (PLAYER_BULLET_2_SIZE), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - (PLAYER_BULLET_2_SIZE), m_pos.y + (PLAYER_BULLET_2_SIZE), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + (PLAYER_BULLET_2_SIZE), m_pos.y + (PLAYER_BULLET_2_SIZE), 0.0f);

		m_pVtxBuff->Unlock();
	}
	else if (m_type == BULLETTYPE_PLAYER_3)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない

		//頂点座標の設定(右回り)
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - (PLAYER_BULLET_3_SIZE), m_pos.y - (PLAYER_BULLET_3_SIZE), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + (PLAYER_BULLET_3_SIZE), m_pos.y - (PLAYER_BULLET_3_SIZE), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - (PLAYER_BULLET_3_SIZE), m_pos.y + (PLAYER_BULLET_3_SIZE), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + (PLAYER_BULLET_3_SIZE), m_pos.y + (PLAYER_BULLET_3_SIZE), 0.0f);

		m_pVtxBuff->Unlock();
	}
	else if (m_type == BULLETTYPE_1FBOSS)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
		//頂点座標の設定(右回り)
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_r * cosf(m_fAngle)), m_pos.y - (m_r * sinf(m_fAngle)), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_r * cosf(m_fAngle2)), m_pos.y - (m_r * sinf(m_fAngle2)), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_r * cosf(m_fAngle2)), m_pos.y + (m_r * sinf(m_fAngle2)), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_r * cosf(m_fAngle)), m_pos.y + (m_r * sinf(m_fAngle)), 0.0f);

		m_pVtxBuff->Unlock();
	}
	else if (m_type == BULLETTYPE_2FBOSS)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない

		//頂点座標の設定(右回り)
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - (BOSS2_BULLET_SIZE / 2), m_pos.y - (BOSS2_BULLET_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + (BOSS2_BULLET_SIZE / 2), m_pos.y - (BOSS2_BULLET_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - (BOSS2_BULLET_SIZE / 2), m_pos.y + (BOSS2_BULLET_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + (BOSS2_BULLET_SIZE / 2), m_pos.y + (BOSS2_BULLET_SIZE / 2), 0.0f);

		m_pVtxBuff->Unlock();
	}
	else if (m_type == BULLETTYPE_ENEMY)
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない

		//頂点座標の設定(右回り)
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - (ENEMY_BULLET_SIZE), m_pos.y - (ENEMY_BULLET_SIZE), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + (ENEMY_BULLET_SIZE), m_pos.y - (ENEMY_BULLET_SIZE), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - (ENEMY_BULLET_SIZE), m_pos.y + (ENEMY_BULLET_SIZE), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + (ENEMY_BULLET_SIZE), m_pos.y + (ENEMY_BULLET_SIZE), 0.0f);

		m_pVtxBuff->Unlock();
	}
	else
	{
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない

		//頂点座標の設定(右回り)
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - (BULLET_SIZE / 2), m_pos.y - (BULLET_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + (BULLET_SIZE / 2), m_pos.y - (BULLET_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - (BULLET_SIZE / 2), m_pos.y + (BULLET_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + (BULLET_SIZE / 2), m_pos.y + (BULLET_SIZE / 2), 0.0f);

		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CBullet::Draw(void)
{
	CScene2d::Draw();
}

//=============================================================================
//当たり判定処理
//=============================================================================
bool CBullet::HitCcollision(D3DXVECTOR3 Pos, float size_x, float size_y)
{
	bool bHit = false;
	m_pos = GetPosition();
	if (m_pos.x <= Pos.x + (size_x / 2) &&
		m_pos.x >= Pos.x - (size_x / 2) &&
		m_pos.y <= Pos.y + (size_y / 2) &&
		m_pos.y >= Pos.y - (size_y / 2))//当たり判定
	{
		bHit = true;
	}
	return bHit;
}


