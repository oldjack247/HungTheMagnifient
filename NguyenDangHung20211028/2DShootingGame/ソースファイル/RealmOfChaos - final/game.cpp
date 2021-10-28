//=============================================================================
//インクルードファイル
//=============================================================================
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "bg.h"
#include "score.h"
#include "life.h"
#include "main.h"
#include "item.h"
#include "effect.h"
#include "sound.h"
#include "item.h"
#include "warning.h"

//=============================================================================
//　静的メンバ変数宣言
//=============================================================================
CPlayer* CGame::m_pPlayer = NULL;
CEnemy* CGame::m_pEnemy = NULL;
CBg* CGame::m_pBg = NULL;
CScore* CGame::m_pScore = NULL;
CLife* CGame::m_pLife = NULL;
CEffect* CGame::m_pEffect = NULL;
CItem* CGame::m_pItem = NULL;
CWarning* CGame::m_pWarning = NULL;
CMinion* CGame::m_pMinion = NULL;
bool CGame::m_b1FBossSpawn = false;
bool CGame::m_b2FEnemySpawn = false;
bool CGame::m_b3FEnemySpawn = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame()
{
	m_gamestate = CGame::GAMESTATE_NORMAL;
	m_nCounterGameState = 0;
	m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	n1FEnemySpawnTime = 0;
	n2FEnemySpawnTime = 0;
	b2FBossAwakening = false;
	m_b1FBossSpawn = false;
	m_b2FEnemySpawn = false;
	m_b3FEnemySpawn = false;
	m_pSound = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, float size_x, float size_y)
{
	m_pPlayer = NULL;
	m_pEnemy = NULL;
	m_pBg = NULL;
	m_pScore = NULL;
	m_pLife = NULL;
	m_pEffect = NULL;
	m_pItem = NULL;
	m_pWarning = NULL;
	m_pMinion = NULL;
	CBg::Load();
	CBullet::Load();
	CExplosion::Load();
	CEnemy::Load();
	CPlayer::Load();
	CScore::Load();
	CLife::Load();
	CItem::Load();
	CEffect::Load();
	CItem::Load();
	CWarning::Load();

	//m_pBg = CBg::Create(m_pos, BG_SIZE_X, BG_SIZE_Y);
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), CScene::PRIORITYTYPE_BG, CBg::BG_MOVE);

	m_pLife = CLife::Create(m_pos, HP_SIZE_X, HP_SIZE_Y);

	m_pScore = CScore::Create(D3DXVECTOR3(SCORE_POS_X, SCORE_POS_Y, 0.0f), SCORE_WIDTH, SCORE_HEIGHT);

	m_pPlayer = CPlayer::Create(D3DXVECTOR3(640.0f, 600.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, PLAYER_SIZE);

	//m_pMinion = CMinion::Create(D3DXVECTOR3(700.0f, 650.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MINION_SIZE, MINION_SIZE);

	////サウンドを止める
	m_pSound->StopSound((m_pSound = CManager::GetSound())->SOUND_LABEL_TITLE_BGM);
	//サウンドを再生
	m_pSound->PlaySound((m_pSound = CManager::GetSound())->SOUND_LABEL_GAME_BGM);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	CPlayer::Unload();
	CEnemy::Unload();
	CBullet::Unload();
	CExplosion::Unload();
	CBg::Unload();
	CScore::Unload();
	CLife::Unload();
	CItem::Unload();
	CEffect::Unload();
	CItem::Unload();
	CWarning::Unload();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{

	//毎フレーム進める
	n1FEnemySpawnTime++;

	if (n1FEnemySpawnTime == 1)
	{
		//右に向かって移動するおばけを5体生成
		for (int nCount = 0; nCount < 5; nCount++)
		{
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(0.0f + (50 * (nCount * 3)), 300.0f, 0.0f),
				D3DXVECTOR3(2.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				ENEMY_SIZE, ENEMY_SIZE, ENEMY_NOMAL_LIFE,
				CEnemy::TYPE_ENEMY_GHOST, CEnemy::ACTION_MOVE_RIGHT);
		}
	}

	if (n1FEnemySpawnTime == 100 ||
		n1FEnemySpawnTime == 1850)
	{
		//骸骨を3体ずつ生成
		for (int nCount = 0; nCount < 3; nCount++)
		{
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(100.0f + (-50 * (nCount * 3)),
				-350.0f + 50 * (nCount * 3), 0.0f),
				D3DXVECTOR3(2.0f, 2.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				ENEMY_SIZE, ENEMY_SIZE, ENEMY_NOMAL_LIFE,
				CEnemy::TYPE_ENEMY_HARPY, CEnemy::ACTION_MOVE_NONE);

			m_pEnemy = CEnemy::Create(D3DXVECTOR3(1180.0f + (-50 * (nCount * 3)), -350.0f + 50 * (nCount * 3), 0.0f),
				D3DXVECTOR3(-2.0f, -2.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				ENEMY_SIZE, ENEMY_SIZE, ENEMY_NOMAL_LIFE,
				CEnemy::TYPE_ENEMY_HARPY, CEnemy::ACTION_MOVE_NONE);
		}
	}

	/*if (n1FEnemySpawnTime == 600)
	{
		CItem::Create(D3DXVECTOR3(640.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			ITEM_SIZE, ITEM_SIZE, CItem::TYPE_ITEM_MINION);
	}*/

	if (n1FEnemySpawnTime == 600)
	{
		//上からくるおばけを左右に3体ずつ生成
		for (int nCount = 0; nCount < 3; nCount++)
		{
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(400.0f, -600.0f + 50 * (nCount * 3), 0.0f),
				D3DXVECTOR3(0.0f, 2.3f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				ENEMY_SIZE, ENEMY_SIZE, ENEMY_NOMAL_LIFE,
				CEnemy::TYPE_ENEMY_GHOST, CEnemy::ACTION_MOVE_NONE);

			m_pEnemy = CEnemy::Create(D3DXVECTOR3(780.0f, -600.0f + 50 * (nCount * 3), 0.0f),
				D3DXVECTOR3(0.0f, 2.3f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				ENEMY_SIZE, ENEMY_SIZE, ENEMY_NOMAL_LIFE,
				CEnemy::TYPE_ENEMY_GHOST, CEnemy::ACTION_MOVE_NONE);
		}
	}
	if (n1FEnemySpawnTime == 2600)
	{
		//Warningを生成
		m_pWarning = CWarning::Create(D3DXVECTOR3(640.0f, 350.0f, 0.0f), BOSS_WARNING_SIZE_X, BOSS_WARNING_SIZE_Y, CWarning::WARNINGTYPE_BOSS1);

		//アイテムを落とす敵を生成
		m_pEnemy = CEnemy::Create(D3DXVECTOR3(640.0f, 100.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f),
			ENEMY_SIZE, ENEMY_SIZE, ENEMY_NOMAL_LIFE,
			CEnemy::TYPE_ENEMY_BOSS_FOG, CEnemy::ACTION_ENEMY2_DROP_ITEM);
	}

	if (m_b1FBossSpawn == true)
	{
		//ボスを生成
		m_pEnemy = CEnemy::Create(D3DXVECTOR3(640.0f, 200.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f),
			ENEMY_1FBOSS_SIZE, ENEMY_1FBOSS_SIZE, ENEMY_1FBOSS_LIFE,
			CEnemy::TYPE_ENEMY_1FBOSS,
			CEnemy::ACTION_MOVE_NONE);
		m_b1FBossSpawn = false;
	}

	if (m_b2FEnemySpawn == true)
	{
		//毎フレーム進める
		n2FEnemySpawnTime++;

		if (n2FEnemySpawnTime == 500 || n2FEnemySpawnTime == 1400)
		{
			//回る敵を3体ずつ生成
			for (int nCount = 0; nCount < 3; nCount++)
			{
				m_pEnemy = CEnemy::Create(D3DXVECTOR3(100.0f, -600.0f + 50 * (nCount * 3), 0.0f),
					D3DXVECTOR3(0.0f, 1.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					ENEMY_SIZE, ENEMY_SIZE, ENEMY_NOMAL_LIFE,
					CEnemy::TYPE_ENEMY_2FGHOST, CEnemy::ACTION_CIRCLE_RIGHT);

				m_pEnemy = CEnemy::Create(D3DXVECTOR3(1100.0f, -600.0f + 50 * (nCount * 3), 0.0f),
					D3DXVECTOR3(0.0f, 1.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					ENEMY_SIZE, ENEMY_SIZE, ENEMY_NOMAL_LIFE,
					CEnemy::TYPE_ENEMY_2FGHOST, CEnemy::ACTION_CIRCLE_LEFT);
			}
		}

		if (n2FEnemySpawnTime == 1000)
		{
			//斜めに飛んでくる蝙蝠を生成
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(1280.0f, 0.0f, 0.0f),
				D3DXVECTOR3(-10.0f, 10.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
				200.0f, 200.0f, ENEMY_NOMAL_LIFE,
				CEnemy::TYPE_ENEMY_2FBAT, CEnemy::ACTION_CIRCLE_RIGHT);
		}
		if (n2FEnemySpawnTime == 1300)
		{
			//斜めに飛んでくる蝙蝠を生成
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(10.0f, 10.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
				200.0f, 200.0f, ENEMY_NOMAL_LIFE,
				CEnemy::TYPE_ENEMY_2FBAT, CEnemy::ACTION_CIRCLE_RIGHT);
		}

		if (n2FEnemySpawnTime == 1200)
		{
			//上から来て戻る敵を生成
			for (int nCount = 0; nCount < 3; nCount++)
			{
				m_pEnemy = CEnemy::Create(D3DXVECTOR3(400.0f, -100.0f + 50 * (nCount * 3), 0.0f),
					D3DXVECTOR3(0.0f, 1.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					ENEMY_SIZE, ENEMY_SIZE, ENEMY_NOMAL_LIFE,
					CEnemy::TYPE_ENEMY_GHOST, CEnemy::ACTION_MOVE_NONE);
			}
		}

		if (n2FEnemySpawnTime == 1250)
		{
			//上から来て戻る敵を生成
				//上から来て戻る敵を生成
			for (int nCount = 0; nCount < 3; nCount++)
			{
				m_pEnemy = CEnemy::Create(D3DXVECTOR3(780.0f, -100.0f + 50 * (nCount * 3), 0.0f),
					D3DXVECTOR3(0.0f, 1.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					ENEMY_SIZE, ENEMY_SIZE, ENEMY_NOMAL_LIFE,
					CEnemy::TYPE_ENEMY_GHOST, CEnemy::ACTION_MOVE_NONE);
			}
		}

		if (n2FEnemySpawnTime == 500)
		{
			m_pWarning = CWarning::Create(D3DXVECTOR3(640.0f, 350.0f, 0.0f), BOSS_WARNING_SIZE_X, BOSS_WARNING_SIZE_Y, CWarning::WARNINGTYPE_BOSS2);
			//ボスを生成
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(640.0f, 200.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				ENEMY_1FBOSS_SIZE, ENEMY_1FBOSS_SIZE, ENEMY_2FBOSS_LIFE,
				CEnemy::TYPE_ENEMY_2FBOSS, CEnemy::ACTION_MOVE_NONE);
			////サウンドを止める
			m_pSound->StopSound((m_pSound = CManager::GetSound())->SOUND_LABEL_GAME_BGM);
			////サウンドを再生
			m_pSound->PlaySound((m_pSound = CManager::GetSound())->SOUND_LABEL_2FBOSS_BATTLE_BGM);
		}

		if (n2FEnemySpawnTime == 3100 && b2FBossAwakening == false)
		{
			//ボスを覚醒状態にする
			b2FBossAwakening = true;
			CEnemy::Set2FBossAwakening(b2FBossAwakening);
		}
	}

	//if (n3FEnemySpawnTime == 50)
	//{
	//	//右に向かって移動するおばけを5体生成
	//	for (int nCount = 0; nCount < 7; nCount++)
	//	{
	//		m_pEnemy = CEnemy::Create(D3DXVECTOR3(1180.0f + (-50 * (nCount * 3)), -350.0f + 50 * (nCount * 3), 0.0f),
	//			D3DXVECTOR3(-2.5f, -2.5f, 0.0f),
	//			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
	//			ENEMY_SIZE, ENEMY_SIZE, ENEMY_3F_LIFE,
	//			CEnemy::TYPE_ENEMY_3FDEMON, CEnemy::ACTION_MOVE_NONE);

	//		m_pEnemy = CEnemy::Create(D3DXVECTOR3(1000.0f, -350.0f, 0.0f),
	//			D3DXVECTOR3(-2.5f, -2.5f, 0.0f),
	//			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
	//			ENEMY_SIZE, ENEMY_SIZE, ENEMY_3F_LIFE,
	//			CEnemy::TYPE_ENEMY_3FDEMON, CEnemy::ACTION_MOVE_NONE);
	//	}
	//}

	//if (n3FEnemySpawnTime == 200 ||
	//	n3FEnemySpawnTime == 1850)
	//{
	//	//骸骨を3体ずつ生成
	//	for (int nCount = 0; nCount < 3; nCount++)
	//	{
	//		m_pEnemy = CEnemy::Create(D3DXVECTOR3(100.0f + (-50 * (nCount * 3)),
	//			-350.0f + 50 * (nCount * 3), 0.0f),
	//			D3DXVECTOR3(0.5f, 0.5f, 0.0f),
	//			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
	//			ENEMY_SIZE, ENEMY_SIZE, ENEMY_3F_LIFE,
	//			CEnemy::TYPE_ENEMY_2FGHOST, CEnemy::ACTION_MOVE_NONE);

	//		m_pEnemy = CEnemy::Create(D3DXVECTOR3(1180.0f + (-50 * (nCount * 3)), -350.0f + 50 * (nCount * 3), 0.0f),
	//			D3DXVECTOR3(-0.5f, -0.5f, 0.0f),
	//			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
	//			ENEMY_SIZE, ENEMY_SIZE, ENEMY_3F_LIFE,
	//			CEnemy::TYPE_ENEMY_2FGHOST, CEnemy::ACTION_MOVE_NONE);
	//	}
	//}
	//if (n3FEnemySpawnTime == 600)
	//{
	//	//上からくるおばけを左右に3体ずつ生成
	//	for (int nCount = 0; nCount < 3; nCount++)
	//	{
	//		m_pEnemy = CEnemy::Create(D3DXVECTOR3(400.0f, -600.0f + 50 * (nCount * 3), 0.0f),
	//			D3DXVECTOR3(0.0f, 2.3f, 0.0f),
	//			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
	//			ENEMY_SIZE, ENEMY_SIZE, ENEMY_3F_LIFE,
	//			CEnemy::TYPE_ENEMY_GHOST, CEnemy::ACTION_MOVE_NONE);

	//		m_pEnemy = CEnemy::Create(D3DXVECTOR3(780.0f, -600.0f + 50 * (nCount * 3), 0.0f),
	//			D3DXVECTOR3(0.0f, 2.3f, 0.0f),
	//			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
	//			ENEMY_SIZE, ENEMY_SIZE, ENEMY_3F_LIFE,
	//			CEnemy::TYPE_ENEMY_GHOST, CEnemy::ACTION_MOVE_NONE);
	//	}
	//}
	//if (n3FEnemySpawnTime == 2600)
	//{
	//	//Warningを生成
	//	m_pWarning = CWarning::Create(D3DXVECTOR3(640.0f, 350.0f, 0.0f), BOSS_WARNING_SIZE_X, BOSS_WARNING_SIZE_Y, CWarning::WARNINGTYPE_BOSS1);

	//	////アイテムを落とす敵を生成
	//	//m_pEnemy = CEnemy::Create(D3DXVECTOR3(640.0f, 100.0f, 0.0f),
	//	//	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	//	//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f),
	//	//	ENEMY_SIZE, ENEMY_SIZE, ENEMY_NOMAL_LIFE,
	//	//	CEnemy::TYPE_ENEMY_BOSS_FOG, CEnemy::ACTION_ENEMY2_DROP_ITEM);

	//	m_pEnemy = CEnemy::Create(D3DXVECTOR3(640.0f, 200.0f, 0.0f),
	//		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	//		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
	//		ENEMY_3FBOSS_SIZE, ENEMY_3FBOSS_SIZE, ENEMY_3FBOSS_LIFE,
	//		CEnemy::TYPE_ENEMY_3FBOSS, CEnemy::ACTION_MOVE_NONE);
	//}

	switch (m_gamestate)
	{
	case CGame::GAMESTATE_NORMAL:
		break;
	case CGame::GAMESTATE_END:
		m_nCounterGameState++;
		if (m_nCounterGameState >= COUNT_WAIT_FINISH_GAME)
		{
			m_gamestate = CGame::GAMESTATE_NONE;
		}
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{
}

//=============================================================================
// シーン遷移
//=============================================================================
void CGame::SetGameState(GAMESTATE state)
{
	m_gamestate = state;
	m_nCounterGameState = 0;
}

//=============================================================================
// enemyの取得
//=============================================================================
CEnemy* CGame::GetEnemy(void)
{
	return m_pEnemy;
}

void CGame::SetScore(int nScore)
{
	m_pScore->AddScore(nScore);
}

void CGame::SetPlayerLife(int nLife)
{
	m_pPlayer->SetLife(nLife);
}

void CGame::Set1FBossSpawn(bool bSpawn)
{
	m_b1FBossSpawn = bSpawn;
}

void CGame::Set2FEnemySpawn(bool b2FSpawn)
{
	m_b2FEnemySpawn = b2FSpawn;
}

void CGame::Set3FEnemySpawn(bool b3FSpawn)
{
	m_b3FEnemySpawn = b3FSpawn;
}


