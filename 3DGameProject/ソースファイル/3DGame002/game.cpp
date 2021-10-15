//=============================================================================
// インクルードファイル
//=============================================================================
#include "game.h"
#include "manager.h"
#include "keyboard.h"
#include "joystick.h"
#include "fade.h"
#include "sound.h"
#include "ui.h"
#include "mode.h"
#include "player.h"
#include "enemy.h"
#include "camera.h"
#include "light.h"
#include "scene.h"
#include "map.h"
#include "skybox.h"
#include "fire.h"
#include "number.h"
#include "magiccircle.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CPlayer* CGame::m_pPlayer = NULL;
int CGame::m_nEnemyNum = 0;
int CGame::m_nEnemyDeathTotal = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame()
{
	memset(m_apNumber, NULL, sizeof(m_apNumber));
	m_bEnemyCreate = true;
	m_nEnemyCreateTime = ENEMY_CREATE_TIME;
	m_nDisplace = 0;
	m_nEnemyDeathTotalOld = 0;
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
HRESULT CGame::Init(void)
{
	//randの初期化
	srand((unsigned)time(NULL));

	//サウンドの再生
	CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_GAME);

	//カメラ
	CManager::CreateCamera();

	//ライト
	CManager::CreateLight();
	m_pPlayer = CPlayer::Create(PLAYER_DEFAULT_POS, PLAYER_DEFAULT_ROT, PLAYER_DEFAULT_SIZE);
	CSkyBox::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1000.0f, 1000.0f, 2000.0f));
	CMap::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 200.0f));

	// HPのUI
	CUi::Create(PLAYER_HP_POS, PLAYER_HP_SIZE, CUi::TYPE_HP);
	CUi::Create(PLAYER_HP_GAUGE, PLAYER_HP_GAUGE_SIZE, CUi::TYPE_HPSPBACK);
	CUi::Create(PLAYER_HP_GAUGE, PLAYER_HP_GAUGE_SIZE, CUi::TYPE_PLAYER_LIFE);
	CUi::Create(PLAYER_HP_GAUGE, PLAYER_HP_GAUGE_SIZE, CUi::TYPE_HPFLAME);

	// SPのUI
	CUi::Create(PLAYER_SP_POS, PLAYER_SP_SIZE, CUi::TYPE_SP);
	CUi::Create(PLAYER_SP_GAUGE, PLAYER_SP_GAUGE_SIZE, CUi::TYPE_HPSPBACK);
	CUi::Create(PLAYER_SP_GAUGE, PLAYER_SP_GAUGE_SIZE, CUi::TYPE_PLAYER_SP);
	CUi::Create(PLAYER_SP_GAUGE, PLAYER_SP_GAUGE_SIZE, CUi::TYPE_SPFLAME);

	// あと何体の配置
	CUi::Create(ENEMYREMAINDER_POS, ENEMYREMAINDER_SIZE, CUi::TYPE_ENEMYREMAINDER);

	// 数字の配置
	m_apNumber[TWO_DIGITS_SPOT] = CNumber::Create(ENEMYREMAINDER_POS - ENEMYREMAINDER_POS_DISTANCE, ENEMYREMAINDER_NUM_SIZE);
	m_apNumber[ONE_DIGITS_SPOT] = CNumber::Create(ENEMYREMAINDER_POS + ENEMYREMAINDER_POS_DISTANCE, ENEMYREMAINDER_NUM_SIZE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	//サウンドの停止
	CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_GAME);

	//指定したオブジェクト以外のメモリの開放処理
	CScene::DesignationReleaseAll(CScene::OBJTYPE_FADE);

	m_pPlayer = NULL;
	m_nEnemyNum = 0;
	m_nEnemyDeathTotal = 0;

	m_bEnemyCreate = false;
	m_nEnemyCreateTime = 0;

	for (int nCount = 0; nCount < MAX_NUMBER_CREATE; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	if (CManager::GetFade()->GetFadeState() == CFade::FADE_NONE)
	{
		// 生成するフラグが立っているなら
		if (m_bEnemyCreate == true)
		{
			// 敵を生成する時間の加算
			m_nEnemyCreateTime++;
			if (m_nEnemyCreateTime >= ENEMY_CREATE_TIME)
			{
				// 生成する角度
				float fCreateAngle = D3DXToRadian((rand() % ENEMY_CREATE_ANGLE));
				float fDivide = (float)(rand() % 3 + 1);

				// 敵を生成するときの魔法陣の生成
				CMagiccercle::Create(D3DXVECTOR3((ENEMY_CREATE_DISTANCE.x / fDivide) * sinf(fCreateAngle), 0.0f, (ENEMY_CREATE_DISTANCE.z / fDivide) * cosf(fCreateAngle)),
					MAGICCERCLE_DEFAULT_ROT, MAGICCERCLE_ENEMY_DEFAULT_SIZE, MAGICCERCLE_DEFAULT_COLOR,
					MAGICCERCLE_ENEMY_COUNTANIM * MAGICCERCLE_ENEMY_PATTERNANIM * MAGICCERCLE_ENEMY_TIMEANIM, CMagiccercle::MAGICCIRCLETYPE_ENEMY);

				// 敵の数の加算
				m_nEnemyNum++;
				m_nEnemyCreateTime = 0;

				// 敵の総数が超えたら
				if (m_nEnemyNum >= ENEMY_CREATE_NUM)
				{
					m_bEnemyCreate = false;
				}
			}
		}
	}

	// 敵の数が0になったら
	if (m_nEnemyNum == 0)
	{
		m_bEnemyCreate = true;
	}

	// 倒した数が上回ったなら
	if (m_nEnemyDeathTotal >= MAX_DEATH_ENEMY_NUM)
	{
		if (CManager::GetFade()->GetFadeState() == CFade::FADE_NONE)
		{
			//サウンドの停止
			CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_GAME);

			//フェードの生成
			CManager::GetFade()->SetFade(CManager::MODE_RESULT);
		}
	}

	// 数字を大きくするフラグが立っているなら
	if (m_bNumberScaling == true)
	{
		// 座標をずらす
		D3DXVECTOR3 pos = m_apNumber[TWO_DIGITS_SPOT]->GetPos();
		pos.x -= ENEMYREMAINDER_POS_DISPLACE.x;
		m_apNumber[TWO_DIGITS_SPOT]->SetPos(pos);

		pos = m_apNumber[ONE_DIGITS_SPOT]->GetPos();
		pos.x += ENEMYREMAINDER_POS_DISPLACE.x;
		m_apNumber[ONE_DIGITS_SPOT]->SetPos(pos);

		// サイズを変える
		for (int nCount = 0; nCount < MAX_NUMBER_CREATE; nCount++)
		{
			D3DXVECTOR3 size = m_apNumber[nCount]->GetSize();
			size += ENEMYREMAINDER_SIZE_DISPLACE;
			m_apNumber[nCount]->SetSize(size);
		}

		// ずらす時間の加算
		m_nDisplace++;

		// ずらす時間が超えたら
		if (m_nDisplace >= DISPLACE_TIME)
		{
			// 初期化して小さくする
			m_nDisplace = 0;
			m_bNumberScaling = false;
		}
	}
	else
	{
		// 二桁目
		// 座標を元に戻す
		D3DXVECTOR3 pos = m_apNumber[TWO_DIGITS_SPOT]->GetPos();
		pos.x += ENEMYREMAINDER_POS_DISPLACE.x * 2;

		// 元の位置を超えないようにする
		if (pos.x >= ENEMYREMAINDER_POS.x - ENEMYREMAINDER_POS_DISTANCE.x)
		{
			pos.x = ENEMYREMAINDER_POS.x - ENEMYREMAINDER_POS_DISTANCE.x;
		}
		// 座標のセット
		m_apNumber[TWO_DIGITS_SPOT]->SetPos(pos);

		// 一桁目
		pos = m_apNumber[ONE_DIGITS_SPOT]->GetPos();
		pos.x -= ENEMYREMAINDER_POS_DISPLACE.x * 2;
		// 元の位置を超えないようにする
		if (pos.x <= ENEMYREMAINDER_POS.x + ENEMYREMAINDER_POS_DISTANCE.x)
		{
			pos.x = ENEMYREMAINDER_POS.x + ENEMYREMAINDER_POS_DISTANCE.x;
		}
		// 座標のセット
		m_apNumber[ONE_DIGITS_SPOT]->SetPos(pos);

		// サイズを変える
		for (int nCount = 0; nCount < MAX_NUMBER_CREATE; nCount++)
		{
			D3DXVECTOR3 size = m_apNumber[nCount]->GetSize();
			size -= ENEMYREMAINDER_SIZE_DISPLACE * 2;

			// 元のサイズを超えないようにする
			if (size.x <= ENEMYREMAINDER_NUM_SIZE.x || size.y <= ENEMYREMAINDER_NUM_SIZE.y)
			{
				size = ENEMYREMAINDER_NUM_SIZE;
			}
			m_apNumber[nCount]->SetSize(size);
		}
	}

	// 1フレーム前の敵の倒した数が現在の倒した数より少なかったら
	if (m_nEnemyDeathTotalOld < CGame::GetEnemyDeathTotal())
	{
		// 加算して大きくする
		m_nEnemyDeathTotalOld++;

		// 1フレーム前の敵の倒した数が最大値を超えないように
		if (m_nEnemyDeathTotalOld >= MAX_DEATH_ENEMY_NUM)
		{
			m_nEnemyDeathTotalOld = MAX_DEATH_ENEMY_NUM;
			m_bNumberScaling = false;
		}
		else
		{
			m_bNumberScaling = true;
		}
	}

	// 数字の設定
	for (int nCount = 0; nCount < MAX_NUMBER_CREATE; nCount++)
	{
		int nScore = (int)pow(10.0f, MAX_NUMBER_CREATE - nCount);
		int nScore2 = (int)pow(10.0f, MAX_NUMBER_CREATE - nCount - 1);
		int nAnswer = (MAX_DEATH_ENEMY_NUM - m_nEnemyDeathTotalOld) % nScore / nScore2;

		// 各ナンバーのセット
		m_apNumber[nCount]->SetNumber(nAnswer);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{
	// 描画処理
	for (int nCount = 0; nCount < MAX_NUMBER_CREATE; nCount++)
	{
		m_apNumber[nCount]->Draw();
	}
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
CPlayer* CGame::GetPlayer(void)
{
	return m_pPlayer;
}