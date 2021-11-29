//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "joystick.h"
#include "keyboard.h"
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
#include "magiccircle.h"
#include "fade.h"
#include "fire.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority) : CCharacter(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fMotionTime = 0.0f;
	m_fDamageTime = 0.0f;
	m_nSwordEffectTime = 0;
	m_bDodge = false;
	m_nDodgeCounter = 0;
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
	D3DXVECTOR3 Top = D3DXVECTOR3(GetModelParts(17)->GetMtxWorld()._41, GetModelParts(17)->GetMtxWorld()._42, GetModelParts(17)->GetMtxWorld()._43);
	D3DXVECTOR3 Bottom = D3DXVECTOR3(GetModelParts(16)->GetMtxWorld()._41, GetModelParts(16)->GetMtxWorld()._42, GetModelParts(16)->GetMtxWorld()._43);
	D3DXVECTOR3 Distance = D3DXVECTOR3(Bottom.x - Top.x, Bottom.y - Top.y, Bottom.z - Top.z);

	// 死んでいないなら
	if (GetLife() > 0)
	{
		switch (m_Playerstate)
		{
		case PLAYERSTATE_NOMAL:
			//ゲームパッド操作
			//Movement();
			GamePad();
			//Keyboard();
			break;

		case PLAYERSTATE_ATTACK:
			//攻撃の処理
			Attack(Distance);
			break;

		case PLAYERSTATE_DODGE:
			// 回避モーション
			Dodge();
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

	// 移動量を加算
	pos += m_move;

	// 敵との当たり判定
	pos = EnemyCollision(pos);

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
	int nKey = GetMotion()->GetKey();
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
			GetMotion()->SetMotion(PLAYERMOTION_ATTACK000);
			m_Playerstate = PLAYERSTATE_ATTACK;
			m_fMotionTime = MOTION_SLASH_FRAME;
			//サウンドの再生
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SLASH);
		}

		if (pInputJoystick->GetJoystickTrigger(CInputJoystick::BUTTON_A))
		{
			// 斬るモーション
			GetMotion()->SetMotion(PLAYERMOTION_DODGE);
			m_Playerstate = PLAYERSTATE_DODGE;
			m_fMotionTime = MOTION_DODGE_FRAME;

			m_move.x = -sinf(GetRot().y) * DODGE_RANGE;
			m_move.z = -cosf(GetRot().y) * DODGE_RANGE;
		}
	}

	// 回転のセット
	SetRot(rot);
}

//=============================================================================
//ゲームパッド処理
//=============================================================================
void CPlayer::Keyboard(void)
{
	//コントローラーの取得処理
	CInputMouse* pInputMouse = CManager::GetInputMouse();
	float fAngle = CManager::GetCamera()->GetAngle();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	//通常モーション
	GetMotion()->SetMotion(PLAYERMOTION_IDLE);

	// Aキーが押された(左移動)
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_A))
	{
		if (CManager::GetInputKeyboard()->GetKeyPress(DIK_W))// AとWが同時に押された
		{
			m_move.x = sinf(fAngle - D3DXToRadian(45.0f)) * PLAYER_MOVE_SPEED;
			m_move.z = cosf(fAngle - D3DXToRadian(45.0f)) * PLAYER_MOVE_SPEED;
			rot.y = fAngle;
			SetRot(D3DXVECTOR3(rot.x, fAngle + D3DXToRadian(135.0f), rot.z));

			// モーションを歩きにする
			GetMotion()->SetMotion(PLAYERMOTION_RUN);
		}
		else if (CManager::GetInputKeyboard()->GetKeyPress(DIK_S))// AとSが同時に押された
		{
			m_move.x = sinf(fAngle - D3DXToRadian(135.0f)) * PLAYER_MOVE_SPEED;
			m_move.z = cosf(fAngle - D3DXToRadian(135.0f)) * PLAYER_MOVE_SPEED;
			rot.y = fAngle;
			SetRot(D3DXVECTOR3(rot.x, fAngle + D3DXToRadian(45.0f), rot.z));
			// モーションを後ろ歩きにする
			GetMotion()->SetMotion(PLAYERMOTION_RUN);
		}
		else				// 左だけ押した
		{
			m_move.x = sinf(fAngle - D3DXToRadian(90.0f)) * PLAYER_MOVE_SPEED;
			m_move.z = cosf(fAngle - D3DXToRadian(90.0f)) * PLAYER_MOVE_SPEED;
			rot.y = fAngle;
			SetRot(D3DXVECTOR3(rot.x, fAngle + D3DXToRadian(90.0f), rot.z));
			// モーションを歩きにする
			GetMotion()->SetMotion(PLAYERMOTION_RUN);
		}
	}
	// Dキーが押された(右移動)
	else if (CManager::GetInputKeyboard()->GetKeyPress(DIK_D))
	{
		if (CManager::GetInputKeyboard()->GetKeyPress(DIK_W))// DとWが同時に押された
		{
			m_move.x = sinf(fAngle + D3DXToRadian(45.0f)) * PLAYER_MOVE_SPEED;
			m_move.z = cosf(fAngle + D3DXToRadian(45.0f)) * PLAYER_MOVE_SPEED;
			rot.y = fAngle;
			SetRot(D3DXVECTOR3(rot.x, fAngle + D3DXToRadian(-135.0f), rot.z));
			// モーションを歩きにする
			GetMotion()->SetMotion(PLAYERMOTION_RUN);
		}
		else if (CManager::GetInputKeyboard()->GetKeyPress(DIK_S))// DとSが同時に押された
		{
			m_move.x = sinf(fAngle + D3DXToRadian(135.0f)) * PLAYER_MOVE_SPEED;
			m_move.z = cosf(fAngle + D3DXToRadian(135.0f)) * PLAYER_MOVE_SPEED;
			rot.y = fAngle;
			SetRot(D3DXVECTOR3(rot.x, fAngle + D3DXToRadian(-45.0f), rot.z));
			// モーションを後ろ歩きにする
			GetMotion()->SetMotion(PLAYERMOTION_RUN);
		}
		else				// 右だけ押した
		{
			m_move.x = sinf(fAngle + D3DXToRadian(90.0f)) * PLAYER_MOVE_SPEED;
			m_move.z = cosf(fAngle + D3DXToRadian(90.0f)) * PLAYER_MOVE_SPEED;
			rot.y = fAngle;
			SetRot(D3DXVECTOR3(rot.x, fAngle + D3DXToRadian(-90.0f), rot.z));
			// モーションを歩きにする
			GetMotion()->SetMotion(PLAYERMOTION_RUN);
		}
	}
	// Wキーが押された(上移動)
	else if (CManager::GetInputKeyboard()->GetKeyPress(DIK_W))
	{
		m_move.x = sinf(fAngle) * PLAYER_MOVE_SPEED;
		m_move.z = cosf(fAngle) * PLAYER_MOVE_SPEED;
		rot.y = fAngle;
		SetRot(D3DXVECTOR3(rot.x, fAngle + D3DXToRadian(-180.0f), rot.z));
		// モーションを歩きにする
		GetMotion()->SetMotion(PLAYERMOTION_RUN);
	}
	// Sキーが押された(下移動)
	else if (CManager::GetInputKeyboard()->GetKeyPress(DIK_S))
	{

		m_move.x = sinf(fAngle + D3DXToRadian(180.0f)) * PLAYER_MOVE_SPEED;
		m_move.z = cosf(fAngle + D3DXToRadian(180.0f)) * PLAYER_MOVE_SPEED;
		rot.y = fAngle;
		SetRot(D3DXVECTOR3(rot.x, fAngle, rot.z));
		// モーションを後ろ歩きにする
		GetMotion()->SetMotion(PLAYERMOTION_RUN);
	}

	else
	{
		// 操作がないときは値を0に
		m_move.x = 0.0f;
		m_move.z = 0.0f;
	}

	if (CManager::GetInputKeyboard()->GetKeyTrigger(DIK_SPACE))
	{
		// 斬るモーション
		GetMotion()->SetMotion(PLAYERMOTION_DODGE);
		m_Playerstate = PLAYERSTATE_DODGE;
		m_fMotionTime = MOTION_DODGE_FRAME;

		m_move.x = -sinf(GetRot().y) * DODGE_RANGE;
		m_move.z = -cosf(GetRot().y) * DODGE_RANGE;
		rot.y =	fAngle;
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void CPlayer::Movement(void)
{
	//コントローラーの取得処理
	CInputMouse* pInputMouse = CManager::GetInputMouse();
	float fAngle = CManager::GetCamera()->GetAngle();
	DIJOYSTATE pStick = {};
	CInputJoystick* pInputJoystick = CManager::GetInputJoystick();
	LPDIRECTINPUTDEVICE8 pJoystickDevice = CInputJoystick::GetDevice();
	D3DXVECTOR3 rot = GetRot();
	//PLAYERMOTION PlayerMotion = (PLAYERMOTION)GetMotion->GetMotionState();


	//gamepadの処理
	if (pJoystickDevice != NULL)
	{
		pJoystickDevice->Poll();
		pJoystickDevice->GetDeviceState(sizeof(DIJOYSTATE), &pStick);
	}
	if (pJoystickDevice != NULL)
	{
		//通常モーション
		GetMotion()->SetMotion(PLAYERMOTION_IDLE);

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
		// セット
		SetRot(rot);
	}

	//keyboardの処理
	// Aキーが押された(左移動)
	if (CManager::GetInputKeyboard()->GetKeyPress(DIK_A))
	{
		if (CManager::GetInputKeyboard()->GetKeyPress(DIK_W))// AとWが同時に押された
		{
			m_move.x = sinf(CManager::GetCamera()->GetAngle() - D3DXToRadian(45.0f)) * PLAYER_MOVE_SPEED;
			m_move.z = cosf(CManager::GetCamera()->GetAngle() - D3DXToRadian(45.0f)) * PLAYER_MOVE_SPEED;
			rot.y = CManager::GetCamera()->GetAngle();
			SetRot(D3DXVECTOR3(rot.x, CManager::GetCamera()->GetAngle() + D3DXToRadian(135.0f), rot.z));

			// モーションを歩きにする
			GetMotion()->SetMotion(PLAYERMOTION_RUN);
		}
		else if (CManager::GetInputKeyboard()->GetKeyPress(DIK_S))// AとSが同時に押された
		{
			m_move.x = sinf(CManager::GetCamera()->GetAngle() - D3DXToRadian(135.0f)) * PLAYER_MOVE_SPEED;
			m_move.z = cosf(CManager::GetCamera()->GetAngle() - D3DXToRadian(135.0f)) * PLAYER_MOVE_SPEED;
			rot.y = CManager::GetCamera()->GetAngle();
			SetRot(D3DXVECTOR3(rot.x, CManager::GetCamera()->GetAngle() + D3DXToRadian(45.0f), rot.z));
			// モーションを後ろ歩きにする
			GetMotion()->SetMotion(PLAYERMOTION_RUN);
		}
		else				// 左だけ押した
		{
			m_move.x = sinf(CManager::GetCamera()->GetAngle() - D3DXToRadian(90.0f)) * PLAYER_MOVE_SPEED;
			m_move.z = cosf(CManager::GetCamera()->GetAngle() - D3DXToRadian(90.0f)) * PLAYER_MOVE_SPEED;
			rot.y = CManager::GetCamera()->GetAngle();
			SetRot(D3DXVECTOR3(rot.x, CManager::GetCamera()->GetAngle() + D3DXToRadian(90.0f), rot.z));
			// モーションを歩きにする
			GetMotion()->SetMotion(PLAYERMOTION_RUN);
		}
	}
	// Dキーが押された(右移動)
	else if (CManager::GetInputKeyboard()->GetKeyPress(DIK_D))
	{
		if (CManager::GetInputKeyboard()->GetKeyPress(DIK_W))// DとWが同時に押された
		{
			m_move.x = sinf(CManager::GetCamera()->GetAngle() + D3DXToRadian(45.0f)) * PLAYER_MOVE_SPEED;
			m_move.z = cosf(CManager::GetCamera()->GetAngle() + D3DXToRadian(45.0f)) * PLAYER_MOVE_SPEED;
			rot.y = CManager::GetCamera()->GetAngle();
			SetRot(D3DXVECTOR3(rot.x, CManager::GetCamera()->GetAngle() + D3DXToRadian(-135.0f), rot.z));
			// モーションを歩きにする
			GetMotion()->SetMotion(PLAYERMOTION_RUN);
		}
		else if (CManager::GetInputKeyboard()->GetKeyPress(DIK_S))// DとSが同時に押された
		{
			m_move.x = sinf(CManager::GetCamera()->GetAngle() + D3DXToRadian(135.0f)) * PLAYER_MOVE_SPEED;
			m_move.z = cosf(CManager::GetCamera()->GetAngle() + D3DXToRadian(135.0f)) * PLAYER_MOVE_SPEED;
			rot.y = CManager::GetCamera()->GetAngle();
			SetRot(D3DXVECTOR3(rot.x, CManager::GetCamera()->GetAngle() + D3DXToRadian(-45.0f), rot.z));
			// モーションを後ろ歩きにする
			GetMotion()->SetMotion(PLAYERMOTION_RUN);
		}
		else				// 右だけ押した
		{
			m_move.x = sinf(CManager::GetCamera()->GetAngle() + D3DXToRadian(90.0f)) * PLAYER_MOVE_SPEED;
			m_move.z = cosf(CManager::GetCamera()->GetAngle() + D3DXToRadian(90.0f)) * PLAYER_MOVE_SPEED;
			rot.y = CManager::GetCamera()->GetAngle();
			SetRot(D3DXVECTOR3(rot.x, CManager::GetCamera()->GetAngle() + D3DXToRadian(-90.0f), rot.z));
			// モーションを歩きにする
			GetMotion()->SetMotion(PLAYERMOTION_RUN);
		}
	}
	// Wキーが押された(上移動)
	else if (CManager::GetInputKeyboard()->GetKeyPress(DIK_W))
	{
		m_move.x = sinf(CManager::GetCamera()->GetAngle()) * PLAYER_MOVE_SPEED;
		m_move.z = cosf(CManager::GetCamera()->GetAngle()) * PLAYER_MOVE_SPEED;
		rot.y = CManager::GetCamera()->GetAngle();
		SetRot(D3DXVECTOR3(rot.x, CManager::GetCamera()->GetAngle() + D3DXToRadian(-180.0f), rot.z));
		// モーションを歩きにする
		GetMotion()->SetMotion(PLAYERMOTION_RUN);
	}
	// Sキーが押された(下移動)
	else if (CManager::GetInputKeyboard()->GetKeyPress(DIK_S))
	{

		m_move.x = sinf(CManager::GetCamera()->GetAngle() + D3DXToRadian(180.0f)) * PLAYER_MOVE_SPEED;
		m_move.z = cosf(CManager::GetCamera()->GetAngle() + D3DXToRadian(180.0f)) * PLAYER_MOVE_SPEED;
		rot.y = CManager::GetCamera()->GetAngle();
		SetRot(D3DXVECTOR3(rot.x, fAngle, rot.z));
		// モーションを後ろ歩きにする
		GetMotion()->SetMotion(PLAYERMOTION_RUN);
	}

	else
	{
		// 操作がないときは値を0に
		m_move.x = 0.0f;
		m_move.z = 0.0f;
	}
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
		if (m_fMotionTime < MOTION_SLASH_FRAME - MOTION_SLASH_FRAME_COLLISION_START && m_fMotionTime > 10.0f)
		{
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
							CDebugCollision::Create(D3DXVECTOR3(GetModelParts(17)->GetMtxWorld()._41,
								GetModelParts(17)->GetMtxWorld()._42,
								GetModelParts(17)->GetMtxWorld()._43) + (GetDistance * (float)nCount),
								SWORD_COLISION_SIZE, CDebugCollision::TYPE_SQUARE);
#endif
							// 当たり判定
							if (CCollision::RectangleCollision3D(D3DXVECTOR3(GetModelParts(17)->GetMtxWorld()._41,
								GetModelParts(17)->GetMtxWorld()._42,
								GetModelParts(17)->GetMtxWorld()._43) + (GetDistance * (float)nCount),
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
							CHiteffect::Create(D3DXVECTOR3(GetModelParts(17)->GetMtxWorld()._41,
								GetModelParts(17)->GetMtxWorld()._42,
								GetModelParts(17)->GetMtxWorld()._43) + Hitpos, HITEFFECT_PLAYER_SIZE, HITEFFECT_PLAYER_COLOR,
								HITEFFECT_PLAYER_COUNTANIM * HITEFFECT_PLAYER_PATTERNANIM * HITEFFECT_PLAYER_TIMEANIM, CHiteffect::HITEFFECTTYPE_PLAYER);
						}
					}
					pScene = pScene->GetSceneNext();
				}
			} while (pScene != NULL);
		}
	}
}

//=============================================================================
// 回避処理
//=============================================================================
void CPlayer::Dodge(void)
{
	// 操作できない時間
	if (m_fMotionTime > 0.0f)
	{
		if (m_fMotionTime < MOTION_DODGE_FRAME - MOTION_DODGE_END_FRAME && m_fMotionTime > 32.0f)
		{
			GetMotion()->SetMotion(PLAYERMOTION_IDLE);
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
		CSwordeffect::Create(D3DXVECTOR3(GetModelParts(17)->GetMtxWorld()._41,
			GetModelParts(17)->GetMtxWorld()._42,
			GetModelParts(17)->GetMtxWorld()._43) + GetDistance,
			SWORD_EFFECT_SIZE, D3DXVECTOR3(0.0f, 0.01f, 0.0f),
			SWORD_EFFECT_COLOR, SWORD_EFFECT_LIFE);
		m_nSwordEffectTime = 0;
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