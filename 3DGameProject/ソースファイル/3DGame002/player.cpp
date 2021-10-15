//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "joystick.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "motion.h"
#include "collision.h"
#include "sound.h"
#include "camera.h"
#include "slash.h"
#include "debugcollision.h"
#include "swordeffect.h"
#include "hiteffect.h"
#include "specialattackeffect.h"
#include "magiccircle.h"
#include "fade.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority) : CCharacter(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fMotionTime = 0.0f;
	m_fDamageTime = 0.0f;
	m_fSpGauge = 0.0f;
	m_nSwordEffectTime = 0;
	m_Playerstate = PLAYERSTATE_NOMAL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	CPlayer* pPlayer;
	pPlayer = new CPlayer;
	pPlayer->SetPlayer(pos, rot, size);
	pPlayer->Init();
	return pPlayer;
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	SetObjType(OBJTYPE_PLAYER);
	SetType(CHARACTERTYPE_PLAYER);
	LoadInitialPosition(MOTION_PLAYER_TEXT);
	CCharacter::Init();

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	CCharacter::Uninit();

	// リリース
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CPlayer::Update(void)
{
	CCharacter::Update();

	D3DXVECTOR3 pos = GetPos();

	// モーションの時間が0以下だったら
	if (m_fMotionTime <= 0.0f)
	{
		m_Playerstate = PLAYERSTATE_NOMAL;
	}

	// 剣の先と持ち手の位置の距離
	D3DXVECTOR3 Top = D3DXVECTOR3(GetModelParts(12)->GetMtxWorld()._41, GetModelParts(12)->GetMtxWorld()._42, GetModelParts(12)->GetMtxWorld()._43);
	D3DXVECTOR3 Bottom = D3DXVECTOR3(GetModelParts(11)->GetMtxWorld()._41, GetModelParts(11)->GetMtxWorld()._42, GetModelParts(11)->GetMtxWorld()._43);
	D3DXVECTOR3 Distance = D3DXVECTOR3(Top.x - Bottom.x, Top.y - Bottom.y, Top.z - Bottom.z);

	// 死んでいないなら
	if (GetLife() > 0)
	{
		switch (m_Playerstate)
		{
		case PLAYERSTATE_NOMAL:
			//ゲームパッド操作
			GamePad();
			break;

		case PLAYERSTATE_ATTACK:
			//攻撃の処理
			Attack(Distance);
			break;

		case PLAYERSTATE_MAGIC:
			//魔法の処理
			Magic();
			break;

		default:
			break;
		}

		// 剣のエフェクトの処理
		Swordeffect(Distance);
	}

	// モーションの時間を減らしていく
	m_fMotionTime -= SUBTRACT_FLAME;

	// ダメージを受ける時間を減算する
	m_fDamageTime--;

	// SPゲージが最大値を超えないようにする
	if (m_fSpGauge >= PLAYER_SP_MAX)
	{
		m_fSpGauge = PLAYER_SP_MAX;
	}

	// 移動量を加算
	pos += m_move;

	//// 敵との当たり判定
	//pos = EnemyCollision(pos);

	// マップ制限
	if (pos.x - GetSize().x < -PLAYER_MOVE_MAP_LIMIT)
	{
		pos.x = -PLAYER_MOVE_MAP_LIMIT + GetSize().x;
	}
	if (pos.x + GetSize().x > PLAYER_MOVE_MAP_LIMIT)
	{
		pos.x = PLAYER_MOVE_MAP_LIMIT - GetSize().x;
	}
	if (pos.z - GetSize().z < -PLAYER_MOVE_MAP_LIMIT)
	{
		pos.z = -PLAYER_MOVE_MAP_LIMIT + GetSize().z;
	}
	if (pos.z + GetSize().z > PLAYER_MOVE_MAP_LIMIT)
	{
		pos.z = PLAYER_MOVE_MAP_LIMIT - GetSize().z;
	}

	// 座標のセット
	SetPos(pos);

#ifdef _DEBUG
	// 当たり判定の可視化
	CDebugCollision::Create(GetPos() + GetMotion()->GetPos(0), GetSize(), CDebugCollision::TYPE_SQUARE);

	//CDebugCollision::Create(GetPos(), D3DXVECTOR3(GetSize().y, GetSize().y, GetSize().y), CDebugCollision::TYPE_SPHERE);
#endif
}

//=============================================================================
//描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	CCharacter::Draw();
}

//=============================================================================
//ゲームパッド処理
//=============================================================================
void CPlayer::GamePad(void)
{
	//コントローラーの取得処理
	DIJOYSTATE pStick = {};
	CInputJoystick* pInputJoystick = CManager::GetInputJoystick();
	LPDIRECTINPUTDEVICE8 pJoystickDevice = CInputJoystick::GetDevice();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	if (pJoystickDevice != NULL)
	{
		pJoystickDevice->Poll();
		pJoystickDevice->GetDeviceState(sizeof(DIJOYSTATE), &pStick);
	}
	if (pJoystickDevice != NULL)
	{
		//通常モーション
		GetMotion()->SetMotion(PLAYERMOTION_IDLE);

		//--------------------------
		//移動
		//--------------------------
		// スティックの入力があったら
		if (pStick.lY != 0 || pStick.lX != 0)
		{
			// スティックの角度を求める
			float fAngle = atan2(pStick.lX * -1.0f, pStick.lY);

			// スティックの角度から移動する方向を決める
			m_move.x = -sinf(CManager::GetCamera()->GetAngle() + fAngle) * PLAYER_MOVE_SPEED;
			m_move.z = -cosf(CManager::GetCamera()->GetAngle() + fAngle) * PLAYER_MOVE_SPEED;

			// キャラの向きを変える
			rot.y = CManager::GetCamera()->GetAngle() + fAngle;

			// 走るモーション
			GetMotion()->SetMotion(PLAYERMOTION_RUN);
		}
		else
		{
			// 操作がないときは値を0に
			m_move.x = 0.0f;
			m_move.z = 0.0f;
		}

		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2;
		}
		if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2;
		}

		// Xボタンを押したら
		if (pInputJoystick->GetJoystickTrigger(CInputJoystick::BUTTON_X))
		{
			// 斬るモーション
			GetMotion()->SetMotion(PLAYERMOTION_SLASH);
			m_Playerstate = PLAYERSTATE_ATTACK;
			m_fMotionTime = MOTION_SLASH_FLAME;

			//サウンドの再生
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SLASH);
		}

		// ゲージが最大になったら
		if (m_fSpGauge == PLAYER_SP_MAX)
		{
			// Yボタンを押したら
			if (pInputJoystick->GetJoystickTrigger(CInputJoystick::BUTTON_Y))
			{
				// 魔法を出すモーション
				GetMotion()->SetMotion(PLAYERMOTION_MAGIC);
				m_Playerstate = PLAYERSTATE_MAGIC;
				m_fMotionTime = MOTION_MAGICCIRCLE_FLAME;

				// 必殺技のエフェクトの生成
				CSpecialattackeffect::Create(GetPos(), SPECIALATTACKEFFECT_SIZE,
					SPECIALATTACKEFFECT_COLOR, SPECIALATTACKEFFECT_COUNTANIM * SPECIALATTACKEFFECT_PATTERNANIM * SPECIALATTACKEFFECT_TIMEANIM);

				// ゲージの量と移動量を0に
				m_fSpGauge = 0.0f;
				m_move.x = 0.0f;
				m_move.z = 0.0f;

				CScene::SetbUpdate(true, CScene::OBJTYPE_ENEMY);

				//サウンドの再生
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_MAGICCIRCLE);
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_MAGIC);
			}
		}
	}

	// 回転のセット
	SetRot(rot);
}

//=============================================================================
// 攻撃処理
//=============================================================================
void CPlayer::Attack(D3DXVECTOR3 Distance)
{
	D3DXVECTOR3 GetDistance = Distance / SWORD_EFFECT_NUM;

	// 向いてる方向に動かす
	m_move.x = -sinf(GetRot().y) * PLAYER_ATTACK_MOVE_SPEED;
	m_move.z = -cosf(GetRot().y) * PLAYER_ATTACK_MOVE_SPEED;

	// 操作できない時間
	if (m_fMotionTime > 0.0f)
	{
		if (m_fMotionTime < MOTION_SLASH_FLAME - MOTION_SLASH_FLAME_COLLISION_START && m_fMotionTime > 10.0f)
		{
			//// 移動の角度を決める
			//D3DXVECTOR3 moveAngle = D3DXVECTOR3(sinf(GetRot().y) * -1, 0.0f, cosf(GetRot().y) * -1);
			//
			//CSlash::Create(D3DXVECTOR3(GetModelParts(11)->GetMtxWorld()._41,
			//	GetModelParts(11)->GetMtxWorld()._42,
			//	GetModelParts(11)->GetMtxWorld()._43) + Distance,
			//	SLASH_SIZE, moveAngle,
			//	SLASH_COLOR, SLASH_LIFE, CSlash::TYPE_SLASH_1);

			// 敵への当たり判定
			CScene* pScene = CScene::GetSceneTop(CScene::OBJTYPE_ENEMY);
			do
			{
				if (pScene != NULL)
				{
					OBJTYPE objType = pScene->GetObjType();
					if (objType == OBJTYPE_ENEMY)
					{
						// 当たったかどうかと当たった場所
						bool bHit = false;
						D3DXVECTOR3 Hitpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

						// 剣の当たり判定分回す
						for (int nCount = 0; nCount < SWORD_EFFECT_NUM; nCount++)
						{
#ifdef _DEBUG
							// 当たり判定の可視化
							CDebugCollision::Create(D3DXVECTOR3(GetModelParts(11)->GetMtxWorld()._41,
								GetModelParts(11)->GetMtxWorld()._42,
								GetModelParts(11)->GetMtxWorld()._43) + (GetDistance * (float)nCount),
								SWORD_COLISION_SIZE, CDebugCollision::TYPE_SQUARE);
#endif
							// 当たり判定
							if (CCollision::RectangleCollision3D(D3DXVECTOR3(GetModelParts(11)->GetMtxWorld()._41,
								GetModelParts(11)->GetMtxWorld()._42,
								GetModelParts(11)->GetMtxWorld()._43) + (GetDistance * (float)nCount),
								SWORD_COLISION_SIZE, ((CEnemy*)pScene)->GetPos(), ((CEnemy*)pScene)->GetSize()) == true)
							{
								// 敵へのダメージ(敵側で当たったか返す)
								bHit = ((CEnemy*)pScene)->HitDamage(5, CEnemy::DAMAGESTATE_SLASH);
								Hitpos = GetDistance * (float)nCount;
								break;
							}
						}

						// 敵に当たったなら
						if (bHit == true)
						{
							// 火花を出す
							CHiteffect::Create(D3DXVECTOR3(GetModelParts(11)->GetMtxWorld()._41,
								GetModelParts(11)->GetMtxWorld()._42,
								GetModelParts(11)->GetMtxWorld()._43) + Hitpos, HITEFFECT_PLAYER_SIZE, HITEFFECT_PLAYER_COLOR,
								HITEFFECT_PLAYER_COUNTANIM * HITEFFECT_PLAYER_PATTERNANIM * HITEFFECT_PLAYER_TIMEANIM, CHiteffect::HITEFFECTTYPE_PLAYER);

							// spのゲージを加算する
							m_fSpGauge++;
						}
					}
					pScene = pScene->GetSceneNext();
				}
			} while (pScene != NULL);
		}
	}
}

//=============================================================================
// 剣のエフェクトの処理
//=============================================================================
void CPlayer::Swordeffect(D3DXVECTOR3 Distance)
{
	D3DXVECTOR3 GetDistance = (float)(rand() % SWORD_EFFECT_NUM) * (Distance / SWORD_EFFECT_NUM);

	m_nSwordEffectTime++;
	if (m_nSwordEffectTime >= SWORD_EFFECT_CREATE_TIME)
	{
		// 剣のエフェクト
		CSwordeffect::Create(D3DXVECTOR3(GetModelParts(11)->GetMtxWorld()._41,
			GetModelParts(11)->GetMtxWorld()._42,
			GetModelParts(11)->GetMtxWorld()._43) + GetDistance,
			SWORD_EFFECT_SIZE, D3DXVECTOR3(0.0f, 0.1f, 0.0f),
			SWORD_EFFECT_COLOR, SWORD_EFFECT_LIFE);
		m_nSwordEffectTime = 0;
	}
}

//=============================================================================
// 魔法の処理
//=============================================================================
void CPlayer::Magic(void)
{
	if (m_fMotionTime == MOTION_MAGICCIRCLE_FLAME - MOTION_MAGICCIRCLE_CREATE_START)
	{
		// 魔法陣の生成
		CMagiccercle::Create(D3DXVECTOR3(GetPos().x, 0.0f, GetPos().z),
			MAGICCERCLE_DEFAULT_ROT, MAGICCERCLE_DEFAULT_SIZE, MAGICCERCLE_DEFAULT_COLOR, (int)m_fMotionTime, CMagiccercle::MAGICCIRCLETYPE_PLAYER);
	}
}

//=============================================================================
// 敵との当たり判定
//=============================================================================
D3DXVECTOR3 CPlayer::EnemyCollision(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 Hitpos = pos;
	CScene* pScene = GetSceneTop(OBJTYPE_ENEMY);
	do
	{
		if (pScene != NULL)
		{
			OBJTYPE objType = pScene->GetObjType();
			if (objType == OBJTYPE_ENEMY)
			{
				if (((CEnemy*)pScene)->GetEnemyState() != CEnemy::ENEMYSTATE_DEATH)
				{
					// 当たり判定
					if (CCollision::SphereCollision(Hitpos, GetSize().x / 2.0f, ((CEnemy*)pScene)->GetPos(), ((CEnemy*)pScene)->GetSize().x / 2.0f) == true)
					{
						// 当たったら1フレーム前の座標を入れる
						Hitpos = GetPos();
					}
				}
			}
			// 次のポインタの受け取り
			pScene = pScene->GetSceneNext();
		}
	} while (pScene != NULL);

	return Hitpos;
}

//=============================================================================
// 初期設定
//=============================================================================
void CPlayer::SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	SetObjType(OBJTYPE_PLAYER); //オブジェクトタイプの設定
	SetPos(pos);
	SetRot(rot);
	SetSize(size);
	SetLife(PLAYER_LIFE);
}

//=============================================================================
//プレイヤーがダメージを受けたら
//=============================================================================
bool CPlayer::HitDamage(int nDamage)
{
	// 当たったかどうか
	bool bHit = false;

	// ダメージを受ける時間が0以下の時
	if (m_fDamageTime <= 0)
	{
		m_fDamageTime = PLAYER_DAMAGE_TIME;

		// ライフを減らす
		int nLife = GetLife();
		nLife -= nDamage;
		SetLife(nLife);

		//サウンドの再生
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_DAMAGE);

		// ライフが0以下の時
		if (nLife <= 0)
		{
			// フェードの生成
			CManager::GetFade()->SetFade(CManager::MODE_GAMEOVER);
		}

		// 当たったフラグを返す
		bHit = true;
	}

	return bHit;
}